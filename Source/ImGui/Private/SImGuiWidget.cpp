// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiPrivatePCH.h"

#include "SImGuiWidget.h"

#include "ImGuiContextManager.h"
#include "ImGuiContextProxy.h"
#include "ImGuiImplementation.h"
#include "ImGuiInputHandler.h"
#include "ImGuiInputHandlerFactory.h"
#include "ImGuiInteroperability.h"
#include "ImGuiModuleManager.h"
#include "ImGuiSettings.h"
#include "TextureManager.h"
#include "Utilities/Arrays.h"
#include "Utilities/ScopeGuards.h"
#include "ImGuiUtils.h"

#include <Engine/Console.h>

#include <utility>


// High enough z-order guarantees that ImGui output is rendered on top of the game UI.
constexpr int32 IMGUI_WIDGET_Z_ORDER = 10000;


DEFINE_LOG_CATEGORY_STATIC(LogImGuiWidget, Warning, All);

#define TEXT_INPUT_MODE(Val) (\
	(Val) == EInputMode::Full ? TEXT("Full") :\
	(Val) == EInputMode::MousePointerOnly ? TEXT("MousePointerOnly") :\
	TEXT("None"))



namespace
{
	const FColor CanvasFrameColor = { 16, 16, 16 };
	const FColor ViewportFrameColor = { 204, 74, 10 };
	const FColor ViewportFrameHighlightColor = { 255, 110, 38 };

	constexpr const char* PlainTextureName = "ImGuiModule_Plain";
	constexpr const char* FontAtlasTextureName = "ImGuiModule_FontAtlas";
}


namespace CVars
{
	TAutoConsoleVariable<int> DrawMouseCursor(TEXT("ImGui.DrawMouseCursor"), 0,
		TEXT("Whether or not mouse cursor in input mode should be drawn by ImGui.\n")
		TEXT("0: disabled, hardware cursor will be used (default)\n")
		TEXT("1: enabled, ImGui will take care for drawing mouse cursor"),
		ECVF_Default);

	TAutoConsoleVariable<int> DebugWidget(TEXT("ImGui.Debug.Widget"), 0,
		TEXT("Show debug for SImGuiWidget.\n")
		TEXT("0: disabled (default)\n")
		TEXT("1: enabled"),
		ECVF_Default);

}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SImGuiWidget::Construct(const FArguments& InArgs)
{
	bIsFocusable = InArgs._IsFocusable;

	SetContextProxy(InArgs._ContextProxy);

	// Disable mouse cursor over this widget as we will use ImGui to draw it.
	SetCursor(EMouseCursor::None);

	// Create ImGui Input Handler.
	CreateInputHandler();
	RegisterInputHandlerChangedDelegate();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SImGuiWidget::~SImGuiWidget()
{
	// Release ImGui Input Handler.
	UnregisterInputHandlerChangedDelegate();
	ReleaseInputHandler();

	// Remove binding between this widget and its context proxy.
	SetContextProxy(nullptr);

	// Unregister from post-update notifications.
	//ModuleManager->OnPostImGuiUpdate().RemoveAll(this);
}

void SImGuiWidget::SetContextProxy(FImGuiContextProxy * InContextProxy)
{
	if (ContextProxy != nullptr)
	{
		ContextProxy->OnDraw().RemoveAll(this);
	}

	ContextProxy = InContextProxy;

	if (ContextProxy != nullptr)
	{
		ContextProxy->OnDraw().AddRaw(this, &SImGuiWidget::OnDebugDraw);
	}
}

FImGuiContextProxy * SImGuiWidget::GetContextProxy() const
{
	return ContextProxy;
}

bool SImGuiWidget::SupportsKeyboardFocus() const
{
	return bIsFocusable;
}

namespace
{
	FReply ToSlateReply(const FImGuiInputResponse& HandlingResponse)
	{
		return HandlingResponse.HasConsumeRequest() ? FReply::Handled() : FReply::Unhandled();
	}
}

FReply SImGuiWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent)
{
	if (auto InputState = GetInputState())
	{
		const FImGuiInputResponse Response = InputHandler->OnKeyChar(CharacterEvent);
		if (Response.HasProcessingRequest())
		{
			InputState->AddCharacter(CharacterEvent.GetCharacter());
		}

		return ToSlateReply(Response);
	}
	return FReply::Unhandled();
}

FReply SImGuiWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (auto InputState = GetInputState())
	{
		if (KeyEvent.GetKey().IsGamepadKey())
		{
			if (InputState->IsGamepadNavigationEnabled())
			{
				const FImGuiInputResponse Response = InputHandler->OnGamepadKeyDown(KeyEvent);
				if (Response.HasProcessingRequest())
				{
					InputState->SetGamepadNavigationKey(KeyEvent, true);
				}

				return ToSlateReply(Response);
			}
			else
			{
				return Super::OnKeyDown(MyGeometry, KeyEvent);
			}
		}
		else
		{

			const FImGuiInputResponse Response = InputHandler->OnKeyDown(KeyEvent);
			if (Response.HasProcessingRequest())
			{
				InputState->SetKeyDown(KeyEvent, true);
				CopyModifierKeys(KeyEvent);
			}

			// Maybe release reply, why?
			return ToSlateReply(Response);
		}
	}
	return FReply::Unhandled();
}

FReply SImGuiWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (auto InputState = GetInputState())
	{
		if (KeyEvent.GetKey().IsGamepadKey())
		{
			if (InputState->IsGamepadNavigationEnabled())
			{
				// Always handle key up events to protect from leaving accidental keys not cleared in ImGui input state.
				InputState->SetGamepadNavigationKey(KeyEvent, false);

				return ToSlateReply(InputHandler->OnGamepadKeyUp(KeyEvent));
			}
			else
			{
				return Super::OnKeyUp(MyGeometry, KeyEvent);
			}
		}
		else
		{

			// Always handle key up events to protect from leaving accidental keys not cleared in ImGui input state.
			InputState->SetKeyDown(KeyEvent, false);
			CopyModifierKeys(KeyEvent);

			return ToSlateReply(InputHandler->OnKeyUp(KeyEvent));
		}
	}

	return FReply::Unhandled();
}

FReply SImGuiWidget::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent)
{
	auto InputState = GetInputState();
	if (AnalogInputEvent.GetKey().IsGamepadKey() && InputState && InputState->IsGamepadNavigationEnabled())
	{
		const FImGuiInputResponse Response = InputHandler->OnGamepadAxis(AnalogInputEvent);
		if (Response.HasProcessingRequest())
		{
			InputState->SetGamepadNavigationAxis(AnalogInputEvent, AnalogInputEvent.GetAnalogValue());
		}

		return ToSlateReply(Response);
	}
	else
	{
		return Super::OnAnalogValueChanged(MyGeometry, AnalogInputEvent);
	}
}

FReply SImGuiWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto InputState = GetInputState();
	if (InputState)
	{
		InputState->SetMouseDown(MouseEvent, true);
		CopyModifierKeys(MouseEvent);
	}

	return InputState ? FReply::Handled() : FReply::Unhandled();
}

FReply SImGuiWidget::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto InputState = GetInputState();
	if (InputState)
	{
		InputState->SetMouseDown(MouseEvent, true);
		CopyModifierKeys(MouseEvent);
	}

	return InputState ? FReply::Handled() : FReply::Unhandled();
}

FReply SImGuiWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto InputState = GetInputState();
	if (InputState)
	{
		InputState->SetMouseDown(MouseEvent, false);
		CopyModifierKeys(MouseEvent);
	}

	return InputState ? FReply::Handled() : FReply::Unhandled();
}

FReply SImGuiWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto InputState = GetInputState();
	if (InputState)
	{
		InputState->AddMouseWheelDelta(MouseEvent.GetWheelDelta());
		CopyModifierKeys(MouseEvent);
	}

	return InputState ? FReply::Handled() : FReply::Unhandled();
}

FReply SImGuiWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto InputState = GetInputState();
	if (InputState)
	{
		InputState->SetMousePosition(MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition());
		CopyModifierKeys(MouseEvent);
	}

	return FReply::Handled();
}

void SImGuiWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseEnter(MyGeometry, MouseEvent);

	//UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Mouse Enter."), ContextIndex);

	if (ContextProxy)
	{
		ContextProxy->RequestInputState(AsShared());
	}

	if (FImGuiInputState * InputState = GetInputState())
	{
		InputState->SetMousePointer(true);

		for (const FKey& Button : { EKeys::LeftMouseButton, EKeys::MiddleMouseButton, EKeys::RightMouseButton, EKeys::ThumbMouseButton, EKeys::ThumbMouseButton2 })
		{
			InputState->SetMouseDown(Button, MouseEvent.IsMouseButtonDown(Button));
		}
	}
}

void SImGuiWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::OnMouseLeave(MouseEvent);

	//UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Mouse Leave."), ContextIndex);

	if (auto InputState = GetInputState())
	{
		InputState->SetMousePointer(false);
	}

	if (ContextProxy)
	{
		ContextProxy->ReleaseInputState(AsShared());
	}
}

FCursorReply SImGuiWidget::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	EMouseCursor::Type MouseCursor = EMouseCursor::None;
	if (MouseCursorOverride != EMouseCursor::None)
	{
		MouseCursor = MouseCursorOverride;
	}
	else if (CVars::DrawMouseCursor.GetValueOnGameThread() <= 0)
	{
		if (FImGuiContextProxy* ContextProxy = GetContextProxy())
		{
			MouseCursor = ContextProxy->GetMouseCursor();
		}
	}

	return FCursorReply::Cursor(MouseCursor);
}

bool SImGuiWidget::IsInteractable() const
{
	return IsEnabled();
}

FImGuiInputState * SImGuiWidget::GetInputState() const
{
	return ContextProxy ? ContextProxy->TryGetInputState(this) : nullptr;
}

void SImGuiWidget::CreateInputHandler()
{
	if (!InputHandler.IsValid())
	{
		InputHandler = FImGuiInputHandlerFactory::NewHandler(this);
	}
}

void SImGuiWidget::ReleaseInputHandler()
{
	if (InputHandler.IsValid())
	{
		FImGuiInputHandlerFactory::ReleaseHandler(InputHandler.Get());
		InputHandler.Reset();
	}
}

void SImGuiWidget::RecreateInputHandler()
{
	ReleaseInputHandler();
	CreateInputHandler();
}

void SImGuiWidget::RegisterInputHandlerChangedDelegate()
{
	if (UImGuiSettings* Settings = GetMutableDefault<UImGuiSettings>())
	{
		if (!Settings->OnImGuiInputHandlerClassChanged.IsBoundToObject(this))
		{
			Settings->OnImGuiInputHandlerClassChanged.AddRaw(this, &SImGuiWidget::RecreateInputHandler);
		}
	}
}

void SImGuiWidget::UnregisterInputHandlerChangedDelegate()
{
	if (UImGuiSettings* Settings = GetMutableDefault<UImGuiSettings>())
	{
		Settings->OnImGuiInputHandlerClassChanged.RemoveAll(this);
	}
}

void SImGuiWidget::CopyModifierKeys(const FInputEvent& InputEvent)
{
	if (FImGuiInputState * InputState = GetInputState())
	{
		InputState->SetControlDown(InputEvent.IsControlDown());
		InputState->SetShiftDown(InputEvent.IsShiftDown());
		InputState->SetAltDown(InputEvent.IsAltDown());
	}
}

void SImGuiWidget::CopyModifierKeys(const FPointerEvent& MouseEvent)
{
	CopyModifierKeys(static_cast<const FInputEvent&>(MouseEvent));
}

void SImGuiWidget::SetMouseCursorOverride(EMouseCursor::Type InMouseCursorOverride)
{
	if (MouseCursorOverride != InMouseCursorOverride)
	{
		MouseCursorOverride = InMouseCursorOverride;
		FSlateApplication::Get().QueryCursor();
	
		if (auto InputState = GetInputState())
		{
			InputState->SetMousePointer(MouseCursorOverride == EMouseCursor::None && IsDirectlyHovered() && CVars::DrawMouseCursor.GetValueOnGameThread() > 0);
		}
	}
}

namespace
{
	FORCEINLINE FVector2D Min(const FVector2D& A, const FVector2D& B)
	{
		return { FMath::Min(A.X, B.X), FMath::Min(A.Y, B.Y) };
	}

	FORCEINLINE FVector2D Max(const FVector2D& A, const FVector2D& B)
	{
		return { FMath::Max(A.X, B.X), FMath::Max(A.Y, B.Y) };
	}

	FORCEINLINE FVector2D Clamp(const FVector2D& V, const FVector2D& Min, const FVector2D& Max)
	{
		return { FMath::Clamp(V.X, Min.X, Max.X), FMath::Clamp(V.Y, Min.Y, Max.Y) };
	}
}

namespace
{
	FORCEINLINE FVector2D RoundToFloat(const FVector2D& Vector)
	{
		return FVector2D{ FMath::RoundToFloat(Vector.X), FMath::RoundToFloat(Vector.Y) };
	}

	void AddLocalRectanglePoints(TArray<FVector2D> &OutPoints, const FGeometry& AllottedGeometry, const FVector2D& AbsoluteMin, const FVector2D& AbsoluteSize)
	{
		FVector2D LocalMin = AllottedGeometry.AbsoluteToLocal(AbsoluteMin) + FVector2D::UnitVector;
		FVector2D LocalMax = AllottedGeometry.AbsoluteToLocal(AbsoluteMin + AbsoluteSize);
		OutPoints.Append({
			FVector2D(LocalMin.X, LocalMin.Y),
			FVector2D(LocalMax.X, LocalMin.Y),
			FVector2D(LocalMax.X, LocalMax.Y),
			FVector2D(LocalMin.X, LocalMax.Y),
			FVector2D(LocalMin.X, LocalMin.Y - 1.f) // -1 to close properly
			});
	}

#if WITH_OBSOLETE_CLIPPING_API
	void AddQuad(TArray<FSlateVertex>& OutVertexBuffer, TArray<SlateIndex>& OutIndexBuffer, const FVector2D& Position, const FVector2D& Size,
		const FVector2D& UVMin, const FVector2D& UVMax, const FColor& Color, const FSlateRotatedClipRectType& InClipRect)
	{

		const uint32 IndexOffset = static_cast<uint32>(OutVertexBuffer.Num());

		FVector2D Min = RoundToFloat(Position) + FVector2D::UnitVector * 0.5f;
		FVector2D Max = RoundToFloat(Position + Size) + FVector2D::UnitVector * 0.5f;
		OutVertexBuffer.Append({
			FSlateVertex({}, { Min.X, Min.Y }, { UVMin.X, UVMin.Y }, Color, InClipRect),
			FSlateVertex({}, { Max.X, Min.Y }, { UVMax.X, UVMin.Y }, Color, InClipRect),
			FSlateVertex({}, { Max.X, Max.Y }, { UVMax.X, UVMax.Y }, Color, InClipRect),
			FSlateVertex({}, { Min.X, Max.Y }, { UVMin.X, UVMax.Y }, Color, InClipRect)
			});

		OutIndexBuffer.Append({ IndexOffset + 0U, IndexOffset + 1U, IndexOffset + 2U, IndexOffset + 0U, IndexOffset + 2U, IndexOffset + 3U });
	}
#else
	void AddQuad(TArray<FSlateVertex>& OutVertexBuffer, TArray<SlateIndex>& OutIndexBuffer, const FVector2D& Position, const FVector2D& Size,
		const FVector2D& UVMin, const FVector2D& UVMax, const FColor& Color)
	{
		const uint32 IndexOffset = static_cast<uint32>(OutVertexBuffer.Num());

		FVector2D Min = RoundToFloat(Position) + FVector2D::UnitVector * 0.5f;
		FVector2D Max = RoundToFloat(Position + Size) + FVector2D::UnitVector * 0.5f;
		OutVertexBuffer.Append({
			FSlateVertex::Make<ESlateVertexRounding::Disabled>({}, { Min.X, Min.Y }, { UVMin.X, UVMin.Y }, Color),
			FSlateVertex::Make<ESlateVertexRounding::Disabled>({}, { Max.X, Min.Y }, { UVMax.X, UVMin.Y }, Color),
			FSlateVertex::Make<ESlateVertexRounding::Disabled>({}, { Max.X, Max.Y }, { UVMax.X, UVMax.Y }, Color),
			FSlateVertex::Make<ESlateVertexRounding::Disabled>({}, { Min.X, Max.Y }, { UVMin.X, UVMax.Y }, Color)
			});

		OutIndexBuffer.Append({ IndexOffset + 0U, IndexOffset + 1U, IndexOffset + 2U, IndexOffset + 0U, IndexOffset + 2U, IndexOffset + 3U });
	}
#endif // WITH_OBSOLETE_CLIPPING_API
}

int32 SImGuiWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& WidgetStyle, bool bParentEnabled) const
{
	if (FImGuiContextProxy* ContextProxy = GetContextProxy())
	{
		ContextProxy->SetDisplaySize(AllottedGeometry.GetAbsoluteSize());

		// Calculate transform between ImGui canvas ans screen space (scale and then offset in Screen Space).
		const FTransform2D Transform{ 1.0f, AllottedGeometry.GetAbsolutePosition() };

		FImGuiModule& ImGuiModule = FImGuiModule::Get();

		FImGuiModuleManager* ImGuiModuleManager = ImGuiModule.GetImGuiModuleManager();

#if WITH_OBSOLETE_CLIPPING_API
		// Convert clipping rectangle to format required by Slate vertex.
		const FSlateRotatedRect VertexClippingRect{ MyClippingRect };
#endif // WITH_OBSOLETE_CLIPPING_API

		for (const auto& DrawList : ContextProxy->GetDrawData())
		{
#if WITH_OBSOLETE_CLIPPING_API
			DrawList.CopyVertexData(VertexBuffer, Transform, WidgetStyle, VertexClippingRect);

			// Get access to the Slate scissor rectangle defined in Slate Core API, so we can customize elements drawing.
			extern SLATECORE_API TOptional<FShortRect> GSlateScissorRect;
			auto GSlateScissorRectSaver = ScopeGuards::MakeStateSaver(GSlateScissorRect);
#else
			DrawList.CopyVertexData(VertexBuffer, Transform, WidgetStyle);
#endif // WITH_OBSOLETE_CLIPPING_API

			int IndexBufferOffset = 0;
			for (int CommandNb = 0; CommandNb < DrawList.NumCommands(); CommandNb++)
			{
				const auto& DrawCommand = DrawList.GetCommand(CommandNb, Transform);

				DrawList.CopyIndexData(IndexBuffer, IndexBufferOffset, DrawCommand.NumElements);

				// Advance offset by number of copied elements to position it for the next command.
				IndexBufferOffset += DrawCommand.NumElements;

				// Get texture resource handle for this draw command (null index will be also mapped to a valid texture).
				const FSlateResourceHandle& Handle = ImGuiModuleManager->GetTextureManager().GetTextureHandle(DrawCommand.TextureId);

				// Transform clipping rectangle to screen space and apply to elements that we draw.
				const FSlateRect ClippingRect = DrawCommand.ClippingRect.IntersectionWith(MyClippingRect);

#if WITH_OBSOLETE_CLIPPING_API
				GSlateScissorRect = FShortRect{ ClippingRect };
#else
				OutDrawElements.PushClip(FSlateClippingZone{ ClippingRect });
#endif // WITH_OBSOLETE_CLIPPING_API

				// Add elements to the list.
				FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, Handle, VertexBuffer, IndexBuffer, nullptr, 0, 0);

#if !WITH_OBSOLETE_CLIPPING_API
				OutDrawElements.PopClip();
#endif // WITH_OBSOLETE_CLIPPING_API
			}
		}
	}

	return LayerId;
}

FVector2D SImGuiWidget::ComputeDesiredSize(float) const
{
	return FVector2D{ 3840.f, 2160.f };
}

static TArray<FKey> GetImGuiMappedKeys()
{
	TArray<FKey> Keys;
	Keys.Reserve(Utilities::ArraySize<ImGuiInterops::ImGuiTypes::FKeyMap>::value + 8);

	// ImGui IO key map.
	Keys.Emplace(EKeys::Tab);
	Keys.Emplace(EKeys::Left);
	Keys.Emplace(EKeys::Right);
	Keys.Emplace(EKeys::Up);
	Keys.Emplace(EKeys::Down);
	Keys.Emplace(EKeys::PageUp);
	Keys.Emplace(EKeys::PageDown);
	Keys.Emplace(EKeys::Home);
	Keys.Emplace(EKeys::End);
	Keys.Emplace(EKeys::Delete);
	Keys.Emplace(EKeys::BackSpace);
	Keys.Emplace(EKeys::Enter);
	Keys.Emplace(EKeys::Escape);
	Keys.Emplace(EKeys::A);
	Keys.Emplace(EKeys::C);
	Keys.Emplace(EKeys::V);
	Keys.Emplace(EKeys::X);
	Keys.Emplace(EKeys::Y);
	Keys.Emplace(EKeys::Z);

	// Modifier keys.
	Keys.Emplace(EKeys::LeftShift);
	Keys.Emplace(EKeys::RightShift);
	Keys.Emplace(EKeys::LeftControl);
	Keys.Emplace(EKeys::RightControl);
	Keys.Emplace(EKeys::LeftAlt);
	Keys.Emplace(EKeys::RightAlt);
	Keys.Emplace(EKeys::LeftCommand);
	Keys.Emplace(EKeys::RightCommand);

	return Keys;
}

void SImGuiWidget::OnDebugDraw()
{
	if (CVars::DebugWidget.GetValueOnGameThread() > 0)
	{
		bool bDebug = true;
		ImGui::SetNextWindowSize(ImVec2(380, 480), ImGuiSetCond_Once);
		if (ImGui::Begin("ImGui Widget Debug", &bDebug))
		{
			using namespace ImGuiUtils;

			ImGui::Spacing();

			TwoColumns::CollapsingGroup("Context", [&]()
			{
				FImGuiContextProxy* ContextProxy = GetContextProxy();
				TwoColumns::Value("Context Name", ContextProxy ? *ContextProxy->GetName() : TEXT("< Null >"));
			});

			TwoColumns::CollapsingGroup("Input Mode", [&]()
			{
				auto InputState = GetInputState();
				TwoColumns::Value("Has Input", InputState != nullptr);
				TwoColumns::Value("Input Has Mouse Pointer", InputState ? InputState->HasMousePointer() : false);
			});

			TwoColumns::CollapsingGroup("Widget", [&]()
			{
				TwoColumns::Value("Visibility", *GetVisibility().ToString());
				TwoColumns::Value("Is Hovered", IsHovered());
				TwoColumns::Value("Is Directly Hovered", IsDirectlyHovered());
				TwoColumns::Value("Has Keyboard Input", HasKeyboardFocus());
			});

			TwoColumns::CollapsingGroup("Img", [&]()
			{
				FImGuiContextProxy* ContextProxy = GetContextProxy();
				TwoColumns::Value("Display Size", ContextProxy ? *ContextProxy->GetDisplaySize().ToString() : TEXT("< Null >"));
			});
		}
		ImGui::End();

		if (!bDebug)
		{
			CVars::DebugWidget->Set(0, ECVF_SetByConsole);
		}
	}
}

#undef TEXT_INPUT_MODE
#undef TEXT_BOOL
