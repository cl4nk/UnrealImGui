// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiInputState.h"

#include <Widgets/SLeafWidget.h>


class FImGuiModuleManager;
class FImGuiContextProxy;
class UImGuiInputHandler;

// Slate widget for rendering ImGui output and storing Slate inputs.
class SImGuiWidget : public SLeafWidget
{
	typedef SLeafWidget Super;

public:

	SLATE_BEGIN_ARGS(SImGuiWidget)
		: _IsFocusable(true)
		, _ContextProxy(nullptr)
	{} 

		SLATE_ARGUMENT(bool, IsFocusable)
		SLATE_ARGUMENT(FImGuiContextProxy * , ContextProxy)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	~SImGuiWidget();

	void SetContextProxy(FImGuiContextProxy * ContextProxy);

	FImGuiContextProxy* GetContextProxy() const;


	//----------------------------------------------------------------------------------------------------
	// SWidget overrides
	//----------------------------------------------------------------------------------------------------

	virtual bool SupportsKeyboardFocus() const override;

	virtual FReply OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent) override;

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent) override;

	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent) override;

	virtual FReply OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& FocusEvent) override;

	virtual void OnFocusLost(const FFocusEvent& FocusEvent) override;

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;

	virtual bool IsInteractable() const override;

	FORCEINLINE FImGuiInputState * GetInputState() const;
	

private:

	void CreateInputHandler();
	void ReleaseInputHandler();
	void RecreateInputHandler();

	void RegisterInputHandlerChangedDelegate();
	void UnregisterInputHandlerChangedDelegate();

	FORCEINLINE void CopyModifierKeys(const FInputEvent& InputEvent);
	FORCEINLINE void CopyModifierKeys(const FPointerEvent& MouseEvent);

	void SetMouseCursorOverride(EMouseCursor::Type InMouseCursorOverride);

	FORCEINLINE bool HasMouseEventNotification() const { return bReceivedMouseEvent; }
	FORCEINLINE void NotifyMouseEvent() { bReceivedMouseEvent = true; }
	FORCEINLINE void ClearMouseEventNotification() { bReceivedMouseEvent = false; }

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& WidgetStyle, bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float) const override;

	void OnDebugDraw();

	TWeakObjectPtr<UImGuiInputHandler> InputHandler;

	mutable TArray<FSlateVertex> VertexBuffer;
	mutable TArray<SlateIndex> IndexBuffer;

	FImGuiContextProxy * ContextProxy;

	bool bIsFocusable = false;
	bool bReceivedMouseEvent = false;
	bool bMouseLock = false;

	EMouseCursor::Type MouseCursorOverride = EMouseCursor::None;
};
