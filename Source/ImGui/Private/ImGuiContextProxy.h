// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiInputState.h"
#include "ImGuiDrawData.h"

#include <ICursor.h>

#include <imgui.h>

#include <string>



class SImGuiWidget;

// Represents a single ImGui context. All the context updates should be done through this proxy. During update it
// broadcasts draw events to allow listeners draw their controls. After update it stores draw data.
class FImGuiContextProxy
{
public:

	FImGuiContextProxy(const FString& Name, FSimpleMulticastDelegate* InSharedDrawEvent, ImFontAtlas* InFontAtlas);
	~FImGuiContextProxy();

	FImGuiContextProxy(const FImGuiContextProxy&) = delete;
	FImGuiContextProxy& operator=(const FImGuiContextProxy&) = delete;

	FImGuiContextProxy(FImGuiContextProxy&& Other) = default;
	FImGuiContextProxy& operator=(FImGuiContextProxy&& Other) = default;

	// Get the name of this context.
	const FString& GetName() const { return Name; }

	// Get draw data from the last frame.
	const TArray<FImGuiDrawList>& GetDrawData() const { return DrawLists; }

	// Get input state used by this context.
	FImGuiInputState * GetInputState() const { return const_cast<FImGuiInputState *>(&InputState); }

	void RequestInputState(TSharedRef<SWidget const> Asker);

	void ReleaseInputState(TSharedRef<SWidget const> Asker);

	// Get input state used by this context.
	FImGuiInputState* TryGetInputState(SWidget const * Asker);

	// Is this context the current ImGui context.
	bool IsCurrentContext() const { return ImGui::GetCurrentContext() == Context.Get(); }

	// Set this context as current ImGui context.
	void SetAsCurrent() { ImGui::SetCurrentContext(Context.Get()); }

	// Context display size (read once per frame during context update and cached here for easy access).
	const FVector2D& GetDisplaySize() const { return DisplaySize; }

	void SetDisplaySize(const FVector2D& Size);

	// Whether this context has an active item (read once per frame during context update and cached here for easy access).
	bool HasActiveItem() const { return bHasActiveItem; }

	// Cursor type desired by this context (this is updated during ImGui frame and cached here during context update, before it is reset).
	EMouseCursor::Type GetMouseCursor() const { return MouseCursor;  }

	// Delegate called right before ending the frame to allows listeners draw their controls.
	FSimpleMulticastDelegate& OnDraw() { return DrawEvent; }

	// Call draw events to allow listeners draw their widgets. Only one call per frame is processed. If it is not
	// called manually before, then it will be called from the Tick function.
	void Draw();

	// Tick to advance context to the next frame. Only one call per frame will be processed.
	void Tick(float DeltaSeconds);

private:

	void BeginFrame(float DeltaTime = 1.f / 60.f);
	void EndFrame();

	void UpdateDrawData(ImDrawData* DrawData);

	TUniquePtr<ImGuiContext> Context;

	FVector2D DisplaySize = FVector2D::ZeroVector;

	EMouseCursor::Type MouseCursor = EMouseCursor::None;
	bool bHasActiveItem = false;

	bool bIsFrameStarted = false;
	bool bIsDrawCalled = false;

	uint32 LastFrameNumber = 0;

	FSimpleMulticastDelegate DrawEvent;
	FSimpleMulticastDelegate* SharedDrawEvent = nullptr;

	FImGuiInputState InputState;

	TArray<TWeakPtr<SWidget const>> InputRequests;

	TArray<FImGuiDrawList> DrawLists;

	FString Name;
	std::string IniFilename;
};
