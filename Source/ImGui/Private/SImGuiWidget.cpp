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

#include <Engine/Console.h>

#include <utility>


// High enough z-order guarantees that ImGui output is rendered on top of the game UI.
constexpr int32 IMGUI_WIDGET_Z_ORDER = 10000;


DEFINE_LOG_CATEGORY_STATIC(LogImGuiWidget, Warning, All);

#define TEXT_INPUT_MODE(Val) (\
	(Val) == EInputMode::Full ? TEXT("Full") :\
	(Val) == EInputMode::MousePointerOnly ? TEXT("MousePointerOnly") :\
	TEXT("None"))

#define TEXT_BOOL(Val) ((Val) ? TEXT("true") : TEXT("false"))


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
	checkf(InArgs._ModuleManager, TEXT("Null Module Manager argument"));

	ModuleManager = InArgs._ModuleManager;
	ContextIndex = InArgs._ContextIndex;

	// Disable mouse cursor over this widget as we will use ImGui to draw it.
	SetCursor(EMouseCursor::None);

	// Sync visibility with default input enabled state.
	SetVisibilityFromInputEnabled();

	// Register to get post-update notifications, so we can clean frame updates.
	ModuleManager->OnPostImGuiUpdate().AddRaw(this, &SImGuiWidget::OnPostImGuiUpdate);

	// Bind this widget to its context proxy.
	auto* ContextProxy = ModuleManager->GetContextManager().GetContextProxy(ContextIndex);
	checkf(ContextProxy, TEXT("Missing context during widget construction: ContextIndex = %d"), ContextIndex);
	ContextProxy->OnDraw().AddRaw(this, &SImGuiWidget::OnDebugDraw);
	ContextProxy->SetInputState(&InputState);

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
	if (auto* ContextProxy = ModuleManager->GetContextManager().GetContextProxy(ContextIndex))
	{
		ContextProxy->OnDraw().RemoveAll(this);
		ContextProxy->RemoveInputState(&InputState);
	}

	// Unregister from post-update notifications.
	ModuleManager->OnPostImGuiUpdate().RemoveAll(this);
}

void SImGuiWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateMouseStatus();

	// Note: Moving that update to console variable sink or callback might seem like a better alternative but input
	// setup in this function is better handled here.
	UpdateInputEnabled();
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
	const FImGuiInputResponse Response = InputHandler->OnKeyChar(CharacterEvent);
	if (Response.HasProcessingRequest())
	{
		InputState.AddCharacter(CharacterEvent.GetCharacter());
	}

	return ToSlateReply(Response);
}

FReply SImGuiWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (KeyEvent.GetKey().IsGamepadKey())
	{
		if (InputState.IsGamepadNavigationEnabled())
		{
			const FImGuiInputResponse Response = InputHandler->OnGamepadKeyDown(KeyEvent);
			if (Response.HasProcessingRequest())
			{
				InputState.SetGamepadNavigationKey(KeyEvent, true);
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
			InputState.SetKeyDown(KeyEvent, true);
			CopyModifierKeys(KeyEvent);
		}

		// Maybe release reply, why?
		return ToSlateReply(Response);
	}
}

FReply SImGuiWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (KeyEvent.GetKey().IsGamepadKey())
	{
		if (InputState.IsGamepadNavigationEnabled())
		{
			// Always handle key up events to protect from leaving accidental keys not cleared in ImGui input state.
			InputState.SetGamepadNavigationKey(KeyEvent, false);

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
		InputState.SetKeyDown(KeyEvent, false);
		CopyModifierKeys(KeyEvent);

		return ToSlateReply(InputHandler->OnKeyUp(KeyEvent));
	}
}

FReply SImGuiWidget::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent)
{
	if (AnalogInputEvent.GetKey().IsGamepadKey() && InputState.IsGamepadNavigationEnabled())
	{
		const FImGuiInputResponse Response = InputHandler->OnGamepadAxis(AnalogInputEvent);
		if (Response.HasProcessingRequest())
		{
			InputState.SetGamepadNavigationAxis(AnalogInputEvent, AnalogInputEvent.GetAnalogValue());
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
	InputState.SetMouseDown(MouseEvent, true);
	CopyModifierKeys(MouseEvent);

	return FReply::Handled();
}

FReply SImGuiWidget::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	InputState.SetMouseDown(MouseEvent, true);
	CopyModifierKeys(MouseEvent);

	return FReply::Handled();
}

FReply SImGuiWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	InputState.SetMouseDown(MouseEvent, false);
	CopyModifierKeys(MouseEvent);

	return FReply::Handled();
}

FReply SImGuiWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	InputState.AddMouseWheelDelta(MouseEvent.GetWheelDelta());
	CopyModifierKeys(MouseEvent);

	return FReply::Handled();
}

FReply SImGuiWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	InputState.SetMousePosition(MouseEvent.GetScreenSpacePosition());
	CopyModifierKeys(MouseEvent);

	// This event is called in every frame when we have a mouse, so we can use it to raise notifications.
	NotifyMouseEvent();

	return FReply::Handled();
}

FReply SImGuiWidget::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& FocusEvent)
{
	Super::OnFocusReceived(MyGeometry, FocusEvent);

	UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Focus Received."), ContextIndex);

	// If widget has a keyboard focus we always maintain mouse input. Technically, if mouse is outside of the widget
	// area it won't generate events but we freeze its state until it either comes back or input is completely lost.
	UpdateInputMode(true, IsDirectlyHovered());

	FSlateApplication::Get().ResetToDefaultPointerInputSettings();
	return FReply::Handled();
}

void SImGuiWidget::OnFocusLost(const FFocusEvent& FocusEvent)
{
	Super::OnFocusLost(FocusEvent);

	UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Focus Lost."), ContextIndex);

	UpdateInputMode(false, IsDirectlyHovered());
}

void SImGuiWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseEnter(MyGeometry, MouseEvent);

	UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Mouse Enter."), ContextIndex);

	// If mouse enters while input is active then we need to update mouse buttons because there is a chance that we
	// missed some events.
	if (InputMode != EInputMode::None)
	{
		for (const FKey& Button : { EKeys::LeftMouseButton, EKeys::MiddleMouseButton, EKeys::RightMouseButton, EKeys::ThumbMouseButton, EKeys::ThumbMouseButton2 })
		{
			InputState.SetMouseDown(Button, MouseEvent.IsMouseButtonDown(Button));
		}
	}

	UpdateInputMode(HasKeyboardFocus(), true);
}

void SImGuiWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::OnMouseLeave(MouseEvent);

	UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Mouse Leave."), ContextIndex);

	// We don't get any events when application loses focus, but often this is followed by OnMouseLeave, so we can use
	// this event to immediately disable keyboard input if application lost focus.
	UpdateInputMode(HasKeyboardFocus(), false);
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
		if (FImGuiContextProxy* ContextProxy = ModuleManager->GetContextManager().GetContextProxy(ContextIndex))
		{
			MouseCursor = ContextProxy->GetMouseCursor();
		}
	}

	return FCursorReply::Cursor(MouseCursor);
}

void SImGuiWidget::CreateInputHandler()
{
	if (!InputHandler.IsValid())
	{
		InputHandler = FImGuiInputHandlerFactory::NewHandler(ModuleManager, ContextIndex);
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
	InputState.SetControlDown(InputEvent.IsControlDown());
	InputState.SetShiftDown(InputEvent.IsShiftDown());
	InputState.SetAltDown(InputEvent.IsAltDown());
}

void SImGuiWidget::CopyModifierKeys(const FPointerEvent& MouseEvent)
{
	if (InputMode == EInputMode::MousePointerOnly)
	{
		CopyModifierKeys(static_cast<const FInputEvent&>(MouseEvent));
	}
}

void SImGuiWidget::SetMouseCursorOverride(EMouseCursor::Type InMouseCursorOverride)
{
	if (MouseCursorOverride != InMouseCursorOverride)
	{
		MouseCursorOverride = InMouseCursorOverride;
		FSlateApplication::Get().QueryCursor();
		InputState.SetMousePointer(MouseCursorOverride == EMouseCursor::None && IsDirectlyHovered() && CVars::DrawMouseCursor.GetValueOnGameThread() > 0);
	}
}

void SImGuiWidget::SetVisibilityFromInputEnabled()
{
	// If we don't use input disable hit test to make this widget invisible for cursors hit detection.
	SetVisibility(bInputEnabled ? EVisibility::Visible : EVisibility::HitTestInvisible);

	UE_LOG(LogImGuiWidget, VeryVerbose, TEXT("ImGui Widget %d - Visibility updated to '%s'."),
		ContextIndex, *GetVisibility().ToString());
}

void SImGuiWidget::UpdateInputEnabled()
{
	const bool bEnabled = true; // TODO: Change the way input are handled
	if (bInputEnabled != bEnabled)
	{
		bInputEnabled = bEnabled;

		UE_LOG(LogImGuiWidget, Log, TEXT("ImGui Widget %d - Input Enabled changed to '%s'."),
			ContextIndex, TEXT_BOOL(bInputEnabled));

		SetVisibilityFromInputEnabled();

		if (!bInputEnabled)
		{
			auto& Slate = FSlateApplication::Get();
			if (Slate.GetKeyboardFocusedWidget().Get() == this)
			{
				Slate.ResetToDefaultPointerInputSettings();
				Slate.SetUserFocus(Slate.GetUserIndexForKeyboard(), nullptr);
			}

			UpdateInputMode(false, false);
		}
	}

	// Note: Some widgets, like console, can reset focus to viewport after we already grabbed it. If we detect that
	// viewport has a focus while input is enabled we will take it.
	if (bInputEnabled && !HasKeyboardFocus())
	{
		if (HasKeyboardFocus() || HasFocusedDescendants())
		{
			auto& Slate = FSlateApplication::Get();
			Slate.SetKeyboardFocus(SharedThis(this));
		}
	}

	// We don't get any events when application loses focus (we get OnMouseLeave but not always) but we fix it with
	// this manual check. We still allow the above code to run, even if we need to suppress keyboard input right after
	// that.
	if (bInputEnabled && InputMode == EInputMode::Full)
	{
		UpdateInputMode(false, IsDirectlyHovered());
	}

	if (bInputEnabled)
	{
		const auto& Application = FSlateApplication::Get().GetPlatformApplication();
		InputState.SetGamepad(Application.IsValid() && Application->IsGamepadAttached());
	}
}

void SImGuiWidget::UpdateInputMode(bool bHasKeyboardFocus, bool bHasMousePointer)
{
	const EInputMode NewInputMode =
		bHasKeyboardFocus ? EInputMode::Full :
		bHasMousePointer ? EInputMode::MousePointerOnly :
		EInputMode::None;

	if (InputMode != NewInputMode)
	{
		UE_LOG(LogImGuiWidget, Verbose, TEXT("ImGui Widget %d - Input Mode changed from '%s' to '%s'."),
			ContextIndex, TEXT_INPUT_MODE(InputMode), TEXT_INPUT_MODE(NewInputMode));

		// We need to reset input components if we are either fully shutting down or we are downgrading from full to
		// mouse-only input mode.
		if (NewInputMode == EInputMode::None)
		{
			InputState.ResetState();
		}
		else if (InputMode == EInputMode::Full)
		{
			InputState.ResetKeyboardState();
			InputState.ResetNavigationState();
		}

		InputMode = NewInputMode;

		ClearMouseEventNotification();
	}

	InputState.SetMousePointer(MouseCursorOverride == EMouseCursor::None && bHasMousePointer && CVars::DrawMouseCursor.GetValueOnGameThread() > 0);
}

void SImGuiWidget::UpdateMouseStatus()
{
	// Note: Mouse leave events can get lost if other viewport takes mouse capture (for instance console is opened by
	// different viewport when this widget is hovered). With that we lose a chance to cleanup and hide ImGui pointer.
	// We could either update ImGui pointer in every frame or like below, use mouse events to catch when mouse is lost.

	if (InputMode == EInputMode::MousePointerOnly)
	{
		if (!HasMouseEventNotification())
		{
			UpdateInputMode(false, IsDirectlyHovered());
		}
		ClearMouseEventNotification();
	}
}

void SImGuiWidget::OnPostImGuiUpdate()
{
	if (InputMode != EInputMode::None)
	{
		InputState.ClearUpdateState();
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
	if (FImGuiContextProxy* ContextProxy = ModuleManager->GetContextManager().GetContextProxy(ContextIndex))
	{
		// Manually update ImGui context to minimise lag between creating and rendering ImGui output. This will also
		// keep frame tearing at minimum because it is executed at the very end of the frame.
		ContextProxy->Tick(FSlateApplication::Get().GetDeltaTime());

		// Calculate offset that will transform vertex positions to screen space - rounded to avoid half pixel offsets.
		const FVector2D CanvasScreenSpacePosition = MyClippingRect.GetTopLeft();

		// Calculate transform between ImGui canvas ans screen space (scale and then offset in Screen Space).
		const FTransform2D Transform{ 1.0f, RoundToFloat(CanvasScreenSpacePosition) };

#if WITH_OBSOLETE_CLIPPING_API
		// Convert clipping rectangle to format required by Slate vertex.
		const FSlateRotatedRect VertexClippingRect{ MyClippingRect };
#endif // WITH_OBSOLETE_CLIPPING_API

		for (const auto& DrawList : ContextProxy->GetDrawData())
		{
#if WITH_OBSOLETE_CLIPPING_API
			DrawList.CopyVertexData(VertexBuffer, Transform, VertexClippingRect);

			// Get access to the Slate scissor rectangle defined in Slate Core API, so we can customize elements drawing.
			extern SLATECORE_API TOptional<FShortRect> GSlateScissorRect;
			auto GSlateScissorRectSaver = ScopeGuards::MakeStateSaver(GSlateScissorRect);
#else
			DrawList.CopyVertexData(VertexBuffer, Transform);
#endif // WITH_OBSOLETE_CLIPPING_API

			int IndexBufferOffset = 0;
			for (int CommandNb = 0; CommandNb < DrawList.NumCommands(); CommandNb++)
			{
				const auto& DrawCommand = DrawList.GetCommand(CommandNb, Transform);

				DrawList.CopyIndexData(IndexBuffer, IndexBufferOffset, DrawCommand.NumElements);

				// Advance offset by number of copied elements to position it for the next command.
				IndexBufferOffset += DrawCommand.NumElements;

				// Get texture resource handle for this draw command (null index will be also mapped to a valid texture).
				const FSlateResourceHandle& Handle = ModuleManager->GetTextureManager().GetTextureHandle(DrawCommand.TextureId);

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

// Column layout utilities.
namespace Columns
{
	template<typename FunctorType>
	static void CollapsingGroup(const char* Name, int Columns, FunctorType&& DrawContent)
	{
		if (ImGui::CollapsingHeader(Name, ImGuiTreeNodeFlags_DefaultOpen))
		{
			const int LastColumns = ImGui::GetColumnsCount();
			ImGui::Columns(Columns, nullptr, false);
			DrawContent();
			ImGui::Columns(LastColumns);
		}
	}
}

// Controls tweaked for 2-columns layout.
namespace TwoColumns
{
	template<typename FunctorType>
	static inline void CollapsingGroup(const char* Name, FunctorType&& DrawContent)
	{
		Columns::CollapsingGroup(Name, 2, std::forward<FunctorType>(DrawContent));
	}

	namespace
	{
		void LabelText(const char* Label)
		{
			ImGui::Text("%s:", Label);
		}

		void LabelText(const wchar_t* Label)
		{
			ImGui::Text("%ls:", Label);
		}
	}

	template<typename LabelType>
	static void Value(LabelType&& Label, int32 Value)
	{
		LabelText(Label); ImGui::NextColumn();
		ImGui::Text("%d", Value); ImGui::NextColumn();
	}

	template<typename LabelType>
	static void Value(LabelType&& Label, uint32 Value)
	{
		LabelText(Label); ImGui::NextColumn();
		ImGui::Text("%u", Value); ImGui::NextColumn();
	}

	template<typename LabelType>
	static void Value(LabelType&& Label, float Value)
	{
		LabelText(Label); ImGui::NextColumn();
		ImGui::Text("%f", Value); ImGui::NextColumn();
	}

	template<typename LabelType>
	static void Value(LabelType&& Label, bool bValue)
	{
		LabelText(Label); ImGui::NextColumn();
		ImGui::Text("%ls", TEXT_BOOL(bValue)); ImGui::NextColumn();
	}

	template<typename LabelType>
	static void Value(LabelType&& Label, const TCHAR* Value)
	{
		LabelText(Label); ImGui::NextColumn();
		ImGui::Text("%ls", Value); ImGui::NextColumn();
	}
}

namespace Styles
{
	template<typename FunctorType>
	static void TextHighlight(bool bHighlight, FunctorType&& DrawContent)
	{
		if (bHighlight)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.f, 1.f, 0.5f, 1.f });
		}
		DrawContent();
		if (bHighlight)
		{
			ImGui::PopStyleColor();
		}
	}
}

void SImGuiWidget::OnDebugDraw()
{
	if (CVars::DebugWidget.GetValueOnGameThread() > 0)
	{
		bool bDebug = true;
		ImGui::SetNextWindowSize(ImVec2(380, 480), ImGuiSetCond_Once);
		if (ImGui::Begin("ImGui Widget Debug", &bDebug))
		{
			ImGui::Spacing();

			TwoColumns::CollapsingGroup("Context", [&]()
			{
				TwoColumns::Value("Context Index", ContextIndex);
				FImGuiContextProxy* ContextProxy = ModuleManager->GetContextManager().GetContextProxy(ContextIndex);
				TwoColumns::Value("Context Name", ContextProxy ? *ContextProxy->GetName() : TEXT("< Null >"));
			});

			TwoColumns::CollapsingGroup("Input Mode", [&]()
			{
				TwoColumns::Value("Input Enabled", bInputEnabled);
				TwoColumns::Value("Input Mode", TEXT_INPUT_MODE(InputMode));
				TwoColumns::Value("Input Has Mouse Pointer", InputState.HasMousePointer());
			});

			TwoColumns::CollapsingGroup("Widget", [&]()
			{
				TwoColumns::Value("Visibility", *GetVisibility().ToString());
				TwoColumns::Value("Is Hovered", IsHovered());
				TwoColumns::Value("Is Directly Hovered", IsDirectlyHovered());
				TwoColumns::Value("Has Keyboard Input", HasKeyboardFocus());
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
