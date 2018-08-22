#pragma once

#include "CoreMinimal.h"
#include "ImGuiBlueprintLibrary.generated.h"

// Flags for ImGui::Begin()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiWindowFlags_
{
	EImGuiWindowFlags_None = 0,
	EImGuiWindowFlags_NoTitleBar = 1 << 0,   // Disable title-bar
	EImGuiWindowFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
	EImGuiWindowFlags_NoMove = 1 << 2,   // Disable user moving the window
	EImGuiWindowFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programatically)
	EImGuiWindowFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
	EImGuiWindowFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it
	EImGuiWindowFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
	//EImGuiWindowFlags_ShowBorders          = 1 << 7,   // Show borders around windows and items (OBSOLETE! Use e.g. style.FrameBorderSize=1.0f to enable borders).
	EImGuiWindowFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
	EImGuiWindowFlags_NoInputs = 1 << 9,   // Disable catching mouse or keyboard inputs, hovering test with pass through.
	EImGuiWindowFlags_MenuBar = 1 << 10,  // Has a menu-bar
	EImGuiWindowFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
	EImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
	EImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
	EImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
	EImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
	EImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
	EImGuiWindowFlags_ResizeFromAnySide = 1 << 17,  // [BETA] Enable resize from any corners and borders. Your back-end needs to honor the different values of io.MouseCursor set by imgui.
	EImGuiWindowFlags_NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
	EImGuiWindowFlags_NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
	EImGuiWindowFlags_NoNav = EImGuiWindowFlags_NoNavInputs | EImGuiWindowFlags_NoNavFocus,
};

// Flags for ImGui::InputText()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiInputTextFlags_
{
	EImGuiInputTextFlags_None = 0,
	EImGuiInputTextFlags_CharsDecimal = 1 << 0,   // Allow 0123456789.+-*/
	EImGuiInputTextFlags_CharsHexadecimal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
	EImGuiInputTextFlags_CharsUppercase = 1 << 2,   // Turn a..z into A..Z
	EImGuiInputTextFlags_CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
	EImGuiInputTextFlags_AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
	EImGuiInputTextFlags_EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to when the value was modified)
	EImGuiInputTextFlags_CallbackCompletion = 1 << 6,   // Call user function on pressing TAB (for completion handling)
	EImGuiInputTextFlags_CallbackHistory = 1 << 7,   // Call user function on pressing Up/Down arrows (for history handling)
	EImGuiInputTextFlags_CallbackAlways = 1 << 8,   // Call user function every time. User code may query cursor position, modify text buffer.
	EImGuiInputTextFlags_CallbackCharFilter = 1 << 9,   // Call user function to filter character. Modify data->EventChar to replace/filter input, or return 1 to discard character.
	EImGuiInputTextFlags_AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
	EImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
	EImGuiInputTextFlags_NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
	EImGuiInputTextFlags_AlwaysInsertMode = 1 << 13,  // Insert mode
	EImGuiInputTextFlags_ReadOnly = 1 << 14,  // Read-only mode
	EImGuiInputTextFlags_Password = 1 << 15,  // Password mode, display all characters as '*'
	EImGuiInputTextFlags_NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
	EImGuiInputTextFlags_CharsScientific = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
};

// Flags for ImGui::TreeNodeEx(), ImGui::CollapsingHeader*()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiTreeNodeFlags_
{
	EImGuiTreeNodeFlags_None = 0,
	EImGuiTreeNodeFlags_Selected = 1 << 0,   // Draw as selected
	EImGuiTreeNodeFlags_Framed = 1 << 1,   // Full colored frame (e.g. for CollapsingHeader)
	EImGuiTreeNodeFlags_AllowItemOverlap = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
	EImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
	EImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
	EImGuiTreeNodeFlags_DefaultOpen = 1 << 5,   // Default node to be open
	EImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6,   // Need double-click to open node
	EImGuiTreeNodeFlags_OpenOnArrow = 1 << 7,   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
	EImGuiTreeNodeFlags_Leaf = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
	EImGuiTreeNodeFlags_Bullet = 1 << 9,   // Display a bullet instead of arrow
	EImGuiTreeNodeFlags_FramePadding = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
	//EImGuITreeNodeFlags_SpanAllAvailWidth  = 1 << 11,  // FIXME: TODO: Extend hit box horizontally even if not framed
	//EImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 12,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
	EImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
	EImGuiTreeNodeFlags_CollapsingHeader = EImGuiTreeNodeFlags_Framed | EImGuiTreeNodeFlags_NoTreePushOnOpen | EImGuiTreeNodeFlags_NoAutoOpenOnLog
};

// Flags for ImGui::Selectable()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiSelectableFlags_
{
	EImGuiSelectableFlags_None = 0,
	EImGuiSelectableFlags_DontClosePopups = 1 << 0,   // Clicking this don't close parent popup window
	EImGuiSelectableFlags_SpanAllColumns = 1 << 1,   // Selectable frame can span all columns (text will still fit in current column)
	EImGuiSelectableFlags_AllowDoubleClick = 1 << 2    // Generate press events on double clicks too
};

// Flags for ImGui::BeginCombo()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiComboFlags_
{
	EImGuiComboFlags_None = 0,
	EImGuiComboFlags_PopupAlignLeft = 1 << 0,   // Align the popup toward the left by default
	EImGuiComboFlags_HeightSmall = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
	EImGuiComboFlags_HeightRegular = 1 << 2,   // Max ~8 items visible (default)
	EImGuiComboFlags_HeightLarge = 1 << 3,   // Max ~20 items visible
	EImGuiComboFlags_HeightLargest = 1 << 4,   // As many fitting items as possible
	EImGuiComboFlags_NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
	EImGuiComboFlags_NoPreview = 1 << 6,   // Display only a square arrow button
	EImGuiComboFlags_HeightMask_ = EImGuiComboFlags_HeightSmall | EImGuiComboFlags_HeightRegular | EImGuiComboFlags_HeightLarge | EImGuiComboFlags_HeightLargest
};

// Flags for ImGui::IsWindowFocused()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiFocusedFlags_
{
	EImGuiFocusedFlags_None = 0,
	EImGuiFocusedFlags_ChildWindows = 1 << 0,   // IsWindowFocused(): Return true if any children of the window is focused
	EImGuiFocusedFlags_RootWindow = 1 << 1,   // IsWindowFocused(): Test from root window (top most parent of the current hierarchy)
	EImGuiFocusedFlags_AnyWindow = 1 << 2,   // IsWindowFocused(): Return true if any window is focused
	EImGuiFocusedFlags_RootAndChildWindows = EImGuiFocusedFlags_RootWindow | EImGuiFocusedFlags_ChildWindows
};

// Flags for ImGui::IsItemHovered(), ImGui::IsWindowHovered()
// Note: If you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that. Please read the FAQ!
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiHoveredFlags_
{
	EImGuiHoveredFlags_None = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
	EImGuiHoveredFlags_ChildWindows = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
	EImGuiHoveredFlags_RootWindow = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
	EImGuiHoveredFlags_AnyWindow = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
	EImGuiHoveredFlags_AllowWhenBlockedByPopup = 1 << 3,   // Return true even if a popup window is normally blocking access to this item/window
	//EImGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 4,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
	EImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 5,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
	EImGuiHoveredFlags_AllowWhenOverlapped = 1 << 6,   // Return true even if the position is overlapped by another window
	EImGuiHoveredFlags_RectOnly = EImGuiHoveredFlags_AllowWhenBlockedByPopup | EImGuiHoveredFlags_AllowWhenBlockedByActiveItem | EImGuiHoveredFlags_AllowWhenOverlapped,
	EImGuiHoveredFlags_RootAndChildWindows = EImGuiHoveredFlags_RootWindow | EImGuiHoveredFlags_ChildWindows
};

// Flags for ImGui::BeginDragDropSource(), ImGui::AcceptDragDropPayload()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiDragDropFlags_
{
	EImGuiDragDropFlags_None = 0,
	// BeginDragDropSource() flags
	EImGuiDragDropFlags_SourceNoPreviewTooltip = 1 << 0,   // By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disable this behavior.
	EImGuiDragDropFlags_SourceNoDisableHover = 1 << 1,   // By default, when dragging we clear data so that IsItemHovered() will return true, to avoid subsequent user code submitting tooltips. This flag disable this behavior so you can still call IsItemHovered() on the source item.
	EImGuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,   // Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.
	EImGuiDragDropFlags_SourceAllowNullID = 1 << 3,   // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit.
	EImGuiDragDropFlags_SourceExtern = 1 << 4,   // External source (from outside of imgui), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active simultaneously.
	// AcceptDragDropPayload() flags
	EImGuiDragDropFlags_AcceptBeforeDelivery = 1 << 10,  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
	EImGuiDragDropFlags_AcceptNoDrawDefaultRect = 1 << 11,  // Do not draw the default highlight rectangle when hovering over target.
	EImGuiDragDropFlags_AcceptNoPreviewTooltip = 1 << 12,  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
	EImGuiDragDropFlags_AcceptPeekOnly = EImGuiDragDropFlags_AcceptBeforeDelivery | EImGuiDragDropFlags_AcceptNoDrawDefaultRect  // For peeking ahead and inspecting the payload before delivery.
};

// A primary data type
UENUM(BlueprintType)
enum EImGuiDataType_
{
	EImGuiDataType_S32,      // int
	EImGuiDataType_U32,      // unsigned int
	EImGuiDataType_S64,      // long long, __int64
	EImGuiDataType_U64,      // unsigned long long, unsigned __int64
	EImGuiDataType_Float,    // float
	EImGuiDataType_Double,   // double
	EImGuiDataType_COUNT
};

// A cardinal direction
UENUM(BlueprintType)
enum EImGuiDir_
{
	EImGuiDir_None = -1,
	EImGuiDir_Left = 0,
	EImGuiDir_Right = 1,
	EImGuiDir_Up = 2,
	EImGuiDir_Down = 3,
	EImGuiDir_COUNT
};

// User fill ImGuiIO.KeyMap[] array with indices into the ImGuiIO.KeysDown[512] array
UENUM(BlueprintType)
enum EImGuiKey_
{
	EImGuiKey_Tab,
	EImGuiKey_LeftArrow,
	EImGuiKey_RightArrow,
	EImGuiKey_UpArrow,
	EImGuiKey_DownArrow,
	EImGuiKey_PageUp,
	EImGuiKey_PageDown,
	EImGuiKey_Home,
	EImGuiKey_End,
	EImGuiKey_Insert,
	EImGuiKey_Delete,
	EImGuiKey_Backspace,
	EImGuiKey_Space,
	EImGuiKey_Enter,
	EImGuiKey_Escape,
	EImGuiKey_A,         // for text edit CTRL+A: select all
	EImGuiKey_C,         // for text edit CTRL+C: copy
	EImGuiKey_V,         // for text edit CTRL+V: paste
	EImGuiKey_X,         // for text edit CTRL+X: cut
	EImGuiKey_Y,         // for text edit CTRL+Y: redo
	EImGuiKey_Z,         // for text edit CTRL+Z: undo
	EImGuiKey_COUNT
};

// [BETA] Gamepad/Keyboard directional navigation
// Keyboard: Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard to enable. NewFrame() will automatically fill io.NavInputs[] based on your io.KeysDown[] + io.KeyMap[] arrays.
// Gamepad:  Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad to enable. Back-end: set ImGuiBackendFlags_HasGamepad and fill the io.NavInputs[] fields before calling NewFrame(). Note that io.NavInputs[] is cleared by EndFrame().
// Read instructions in imgui.cpp for more details. Download PNG/PSD at goo.gl/9LgVZW.
UENUM(BlueprintType)
enum EImGuiNavInput_
{
	// Gamepad Mapping
	EImGuiNavInput_Activate,      // activate / open / toggle / tweak value       // e.g. Cross  (PS4), A (Xbox), A (Switch), Space (Keyboard)
	EImGuiNavInput_Cancel,        // cancel / close / exit                        // e.g. Circle (PS4), B (Xbox), B (Switch), Escape (Keyboard)
	EImGuiNavInput_Input,         // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
	EImGuiNavInput_Menu,          // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
	EImGuiNavInput_DpadLeft,      // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
	EImGuiNavInput_DpadRight,     //
	EImGuiNavInput_DpadUp,        //
	EImGuiNavInput_DpadDown,      //
	EImGuiNavInput_LStickLeft,    // scroll / move window (w/ PadMenu)            // e.g. Left Analog Stick Left/Right/Up/Down
	EImGuiNavInput_LStickRight,   //
	EImGuiNavInput_LStickUp,      //
	EImGuiNavInput_LStickDown,    //
	EImGuiNavInput_FocusPrev,     // next window (w/ PadMenu)                     // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
	EImGuiNavInput_FocusNext,     // prev window (w/ PadMenu)                     // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
	EImGuiNavInput_TweakSlow,     // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
	EImGuiNavInput_TweakFast,     // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
};

// Configuration flags stored in io.ConfigFlags. Set by user/application.
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiConfigFlags_
{
	EImGuiConfigFlags_NavEnableKeyboard = 1 << 0,   // Master keyboard navigation enable flag. NewFrame() will automatically fill io.NavInputs[] based on io.KeysDown[].
	EImGuiConfigFlags_NavEnableGamepad = 1 << 1,   // Master gamepad navigation enable flag. This is mostly to instruct your imgui back-end to fill io.NavInputs[]. Back-end also needs to set ImGuiBackendFlags_HasGamepad.
	EImGuiConfigFlags_NavEnableSetMousePos = 1 << 2,   // Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=true. If enabled you MUST honor io.WantSetMousePos requests in your binding, otherwise ImGui will react as if the mouse is jumping around back and forth.
	EImGuiConfigFlags_NavNoCaptureKeyboard = 1 << 3,   // Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.
	EImGuiConfigFlags_NoMouse = 1 << 4,   // Instruct imgui to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the back-end.
	EImGuiConfigFlags_NoMouseCursorChange = 1 << 5,   // Instruct back-end to not alter mouse cursor shape and visibility. Use if the back-end cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from imgui by reading GetMouseCursor() yourself instead.

	// User storage (to allow your back-end/engine to communicate to code that may be shared between multiple projects. Those flags are not used by core ImGui)
	EImGuiConfigFlags_IsSRGB = 1 << 20,  // Application is SRGB-aware.
	EImGuiConfigFlags_IsTouchScreen = 1 << 21   // Application is using a touch screen instead of a mouse.
};

// Back-end capabilities flags stored in io.BackendFlags. Set by imgui_impl_xxx or custom back-end.
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiBackendFlags_
{
	EImGuiBackendFlags_HasGamepad = 1 << 0,   // Back-end supports gamepad and currently has one connected.
	EImGuiBackendFlags_HasMouseCursors = 1 << 1,   // Back-end supports honoring GetMouseCursor() value to change the OS cursor shape.
	EImGuiBackendFlags_HasSetMousePos = 1 << 2    // Back-end supports io.WantSetMousePos requests to reposition the OS mouse position (only used if ImGuiConfigFlags_NavEnableSetMousePos is set).
};

// Enumeration for PushStyleColor() / PopStyleColor()
UENUM(BlueprintType)
enum EImGuiCol_
{
	EImGuiCol_Text,
	EImGuiCol_TextDisabled,
	EImGuiCol_WindowBg,              // Background of normal windows
	EImGuiCol_ChildBg,               // Background of child windows
	EImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
	EImGuiCol_Border,
	EImGuiCol_BorderShadow,
	EImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
	EImGuiCol_FrameBgHovered,
	EImGuiCol_FrameBgActive,
	EImGuiCol_TitleBg,
	EImGuiCol_TitleBgActive,
	EImGuiCol_TitleBgCollapsed,
	EImGuiCol_MenuBarBg,
	EImGuiCol_ScrollbarBg,
	EImGuiCol_ScrollbarGrab,
	EImGuiCol_ScrollbarGrabHovered,
	EImGuiCol_ScrollbarGrabActive,
	EImGuiCol_CheckMark,
	EImGuiCol_SliderGrab,
	EImGuiCol_SliderGrabActive,
	EImGuiCol_Button,
	EImGuiCol_ButtonHovered,
	EImGuiCol_ButtonActive,
	EImGuiCol_Header,
	EImGuiCol_HeaderHovered,
	EImGuiCol_HeaderActive,
	EImGuiCol_Separator,
	EImGuiCol_SeparatorHovered,
	EImGuiCol_SeparatorActive,
	EImGuiCol_ResizeGrip,
	EImGuiCol_ResizeGripHovered,
	EImGuiCol_ResizeGripActive,
	EImGuiCol_PlotLines,
	EImGuiCol_PlotLinesHovered,
	EImGuiCol_PlotHistogram,
	EImGuiCol_PlotHistogramHovered,
	EImGuiCol_TextSelectedBg,
	EImGuiCol_ModalWindowDarkening,  // Darken/colorize entire screen behind a modal window, when one is active
	EImGuiCol_DragDropTarget,
	EImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
	EImGuiCol_NavWindowingHighlight, // Gamepad/keyboard: when holding NavMenu to focus/move/resize windows
	EImGuiCol_COUNT
};

// Enumeration for PushStyleVar() / PopStyleVar() to temporarily modify the ImGuiStyle structure.
// NB: the enum only refers to fields of ImGuiStyle which makes sense to be pushed/popped inside UI code. During initialization, feel free to just poke into ImGuiStyle directly.
// NB: if changing this enum, you need to update the associated internal table GStyleVarInfo[] accordingly. This is where we link enum values to members offset/type.
UENUM(BlueprintType)
enum EImGuiStyleVar_
{
	// Enum name ......................// Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
	EImGuiStyleVar_Alpha,               // float     Alpha
	EImGuiStyleVar_WindowPadding,       // ImVec2    WindowPadding
	EImGuiStyleVar_WindowRounding,      // float     WindowRounding
	EImGuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
	EImGuiStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
	EImGuiStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
	EImGuiStyleVar_ChildRounding,       // float     ChildRounding
	EImGuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
	EImGuiStyleVar_PopupRounding,       // float     PopupRounding
	EImGuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
	EImGuiStyleVar_FramePadding,        // ImVec2    FramePadding
	EImGuiStyleVar_FrameRounding,       // float     FrameRounding
	EImGuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
	EImGuiStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
	EImGuiStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
	EImGuiStyleVar_IndentSpacing,       // float     IndentSpacing
	EImGuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
	EImGuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
	EImGuiStyleVar_GrabMinSize,         // float     GrabMinSize
	EImGuiStyleVar_GrabRounding,        // float     GrabRounding
	EImGuiStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
	EImGuiStyleVar_COUNT
};

// Enumeration for ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()
UENUM(BlueprintType, meta = (Bitflags))
enum EImGuiColorEditFlags_
{
	EImGuiColorEditFlags_None = 0,
	EImGuiColorEditFlags_NoAlpha = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (read 3 components from the input pointer).
	EImGuiColorEditFlags_NoPicker = 1 << 2,   //              // ColorEdit: disable picker when clicking on colored square.
	EImGuiColorEditFlags_NoOptions = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
	EImGuiColorEditFlags_NoSmallPreview = 1 << 4,   //              // ColorEdit, ColorPicker: disable colored square preview next to the inputs. (e.g. to show only the inputs)
	EImGuiColorEditFlags_NoInputs = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview colored square).
	EImGuiColorEditFlags_NoTooltip = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
	EImGuiColorEditFlags_NoLabel = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
	EImGuiColorEditFlags_NoSidePreview = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small colored square preview instead.
	EImGuiColorEditFlags_NoDragDrop = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.

	// User Options (right-click on widget to change some of them). You can set application defaults using SetColorEditOptions(). The idea is that you probably don't want to override them in most of your calls, let the user choose and/or call SetColorEditOptions() during startup.
	EImGuiColorEditFlags_AlphaBar = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
	EImGuiColorEditFlags_AlphaPreview = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
	EImGuiColorEditFlags_AlphaPreviewHalf = 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
	EImGuiColorEditFlags_HDR = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).
	EImGuiColorEditFlags_RGB = 1 << 20,  // [Inputs]     // ColorEdit: choose one among RGB/HSV/HEX. ColorPicker: choose any combination using RGB/HSV/HEX.
	EImGuiColorEditFlags_HSV = 1 << 21,  // [Inputs]     // "
	EImGuiColorEditFlags_HEX = 1 << 22,  // [Inputs]     // "
	EImGuiColorEditFlags_Uint8 = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
	EImGuiColorEditFlags_Float = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
	EImGuiColorEditFlags_PickerHueBar = 1 << 25,  // [PickerMode] // ColorPicker: bar for Hue, rectangle for Sat/Value.
	EImGuiColorEditFlags_PickerHueWheel = 1 << 26,  // [PickerMode] // ColorPicker: wheel for Hue, triangle for Sat/Value.
};

// Enumeration for GetMouseCursor()
// User code may request binding to display given cursor by calling SetMouseCursor(), which is why we have some cursors that are marked unused here
UENUM(BlueprintType)
enum EImGuiMouseCursor_
{
	EImGuiMouseCursor_None = -1,
	EImGuiMouseCursor_Arrow = 0,
	EImGuiMouseCursor_TextInput,         // When hovering over InputText, etc.
	EImGuiMouseCursor_ResizeAll,         // Unused by imgui functions
	EImGuiMouseCursor_ResizeNS,          // When hovering over an horizontal border
	EImGuiMouseCursor_ResizeEW,          // When hovering over a vertical border or a column
	EImGuiMouseCursor_ResizeNESW,        // When hovering over the bottom-left corner of a window
	EImGuiMouseCursor_ResizeNWSE,        // When hovering over the bottom-right corner of a window
	EImGuiMouseCursor_COUNT
};

// Condition for ImGui::SetWindow***(), SetNextWindow***(), SetNextTreeNode***() functions
// Important: Treat as a regular enum! Do NOT combine multiple values using binary operators! All the functions above treat 0 as a shortcut to ImGuiCond_Always.
UENUM(BlueprintType)
enum EImGuiCond_
{
	EImGuiCond_Always = 1 << 0,   // Set the variable
	EImGuiCond_Once = 1 << 1,   // Set the variable once per runtime session (only the first call with succeed)
	EImGuiCond_FirstUseEver = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
	EImGuiCond_Appearing = 1 << 3    // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};

UCLASS()
class IMGUI_API UImGuiBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows")) static bool          Begin(FString name, UPARAM(ref) bool& p_open, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows")) static void          End();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows")) static bool          BeginChildStr(FString str_id, FVector2D size = FVector2D(0, 0), bool border = false, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0); // Begin a scrolling region. size==0.0f: use remaining window size, size<0.0f: use remaining window size minus abs(size). size>0.0f: fixed size. each axis can use a different mode, e.g. FVector2D(0,400).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows")) static bool          BeginChildInt(int32 id, FVector2D size = FVector2D(0, 0), bool border = false, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows")) static void          EndChild();

	// Windows Utilities
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static bool          IsWindowAppearing();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static bool          IsWindowCollapsed();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static bool          IsWindowFocused(UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiHoveredFlags_)) int32 flags); // is current window focused? or its root/child, depending on flags. see flags for options.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static bool          IsWindowHovered(UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiHoveredFlags_)) int32 flags = 0); // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetWindowPos();                             // get current window position in screen space (useful if you want to do your own drawing via the DrawList API)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetWindowSize();                            // get current window size
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static float         GetWindowWidth();                           // get current window width (shortcut for GetWindowSize().x)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static float         GetWindowHeight();                          // get current window height (shortcut for GetWindowSize().y)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetContentRegionMax();                      // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetContentRegionAvail();                    // == GetContentRegionMax() - GetCursorPos()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static float         GetContentRegionAvailWidth();               //
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetWindowContentRegionMin();                // content boundaries min (roughly (0,0)-Scroll), in window coordinates
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static FVector2D        GetWindowContentRegionMax();                // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static float         GetWindowContentRegionWidth();              //

	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowPos(FVector2D pos, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once, FVector2D pivot = FVector2D(0, 0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowSize(FVector2D size, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);                  // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowContentSize(FVector2D size);                               // set next window content size (~ enforce the range of scrollbars). not including window decorations (title bar, menu bar, etc.). set an axis to 0.0f to leave it automatic. call before Begin()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowCollapsed(bool collapsed, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);                 // set next window collapsed state. call before Begin()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowFocus();                                                       // set next window to be focused / front-most. call before Begin()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetNextWindowBgAlpha(float alpha);                                          // set next window background color alpha. helper to easily modify ImGuiCol_WindowBg/ChildBg/PopupBg.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowPos(FVector2D pos, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowSize(FVector2D size, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);                      // (not recommended) set current window size - call within Begin()/End(). set to FVector2D(0,0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowCollapsed(bool collapsed, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);                     // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowFocus();                                                           // (not recommended) set current window to be focused / front-most. prefer using SetNextWindowFocus().
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowFontScale(float scale);                                            // set font scale. Adjust IO.FontGlobalScale if you want to scale all windows
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowPosByName(FString name, FVector2D pos, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);      // set named window position.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowSizeByName(FString name, FVector2D size, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowCollapsedByName(FString name, bool collapsed, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);   // set named window collapsed state
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Utilities")) static void          SetWindowFocusByName(FString name);                                           // set named window to be focused / front-most. use NULL to remove focus.

	// Windows Scrolling
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static float         GetScrollX();                                                   // get scrolling amount [0..GetScrollMaxX()]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static float         GetScrollY();                                                   // get scrolling amount [0..GetScrollMaxY()]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static void          SetScrollX(float scroll_x);                                     // set scrolling amount [0..GetScrollMaxX()]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static void          SetScrollY(float scroll_y);                                     // set scrolling amount [0..GetScrollMaxY()]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static void          SetScrollHere(float center_y_ratio = 0.5f);                     // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Windows Scrolling")) static void          SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f);    // adjust scrolling amount to make given position valid. use GetCursorPos() or GetCursorStartPos()+offset to get valid positions.

	// Parameters stacks (shared)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static void          PushStyleColor(EImGuiCol_ idx, FLinearColor col);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static void          PopStyleColor(int count = 1);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static void          PushStyleVar(EImGuiStyleVar_ idx, float val);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static void          PushStyleVarVec2(EImGuiStyleVar_ idx, FVector2D val);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static void          PopStyleVar(int count = 1);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static FVector4 GetStyleColorVec4(EImGuiCol_ idx);                                // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static float         GetFontSize();                                                  // get current font size (= height in pixels) of current font with current scale applied
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Shared")) static FVector2D        GetFontTexUvWhitePixel();                                       // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API

	// Parameters stacks (current window)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PushItemWidth(float item_width);                                // width of items for the common item+label case, pixels. 0.0f = default to ~2/3 of windows width, >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PopItemWidth();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PushTextWrapPos(float wrap_pos_x = 0.0f);                       // word-wrapping for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PopTextWrapPos();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PopAllowKeyboardFocus();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PushButtonRepeat(bool repeat);                                  // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parameters stacks|Current window")) static void          PopButtonRepeat();

	// Cursor / Layout
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);          // call between widgets or groups to layout them horizontally
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          NewLine();                                                      // undo a SameLine()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          Spacing();                                                      // add vertical spacing
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          Dummy(FVector2D size);                                      // add a dummy item of given size
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by style.IndentSpacing or indent_w if != 0
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          BeginGroup();                                                   // lock horizontal starting position + capture group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          EndGroup();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static FVector2D        GetCursorPos();                                                 // cursor position is relative to window position
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetCursorPosX();                                                // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetCursorPosY();                                                // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          SetCursorPos(FVector2D local_pos);                          // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          SetCursorPosX(float x);                                         // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          SetCursorPosY(float y);                                         // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static FVector2D        GetCursorStartPos();                                            // initial cursor position
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static FVector2D        GetCursorScreenPos();                                           // cursor position in absolute screen coordinates [0..io.DisplaySize] (useful to work with ImDrawList API)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          SetCursorScreenPos(FVector2D screen_pos);                   // cursor position in absolute screen coordinates [0..io.DisplaySize]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetTextLineHeight();                                            // ~ FontSize
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Cursor & Layout")) static float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

	// ID stack/scopes
	// Read the FAQ for more details about how ID are handled in dear imgui. If you are creating widgets in a loop you most
	// likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
	// You can also use the "##foobar" syntax within widget label to distinguish them from each others.
	// In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
	// whereas "str_id" denote a string that is only used as an ID and not aimed to be displayed.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static void          PushIDStr(FString str_id);                                     // push identifier into the ID stack. IDs are hash of the entire stack!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static void          PushIDSubStr(FString str_id_begin, FString str_id_end);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static void          PushIDInt(int32 int_id);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static void          PopID();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static int32       GetID(FString str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into ImGuiStorage yourself
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|ID Stack")) static int32       GetIDSubStr(FString str_id_begin, FString str_id_end);

	// Widgets: Text
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          Text(FString fmt);                                // simple formatted text
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          TextColored(FLinearColor col, FString fmt);            // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          TextDisabled(FString fmt);                            // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          TextWrapped(FString fmt);                            // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          LabelText(FString label, FString fmt);          // display text+label aligned the same way as value+label widgets
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Text")) static void          BulletText(FString fmt);                           // shortcut for Bullet()+Text()

	// Widgets: Main
	// Most widgets return true when the value has been changed or when pressed/selected
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          Button(FString label, FVector2D size = FVector2D(0, 0));    // button
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          SmallButton(FString label);                                 // button with FramePadding=(0,0) to easily embed within text
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          InvisibleButton(FString str_id, FVector2D size);        // button behavior without the visuals, useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          ArrowButton(FString str_id, EImGuiDir_ dir);                  // square button with an arrow shape
	//UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static void          Image(ImTextureID user_texture_id, FVector2D size, FVector2D uv0 = FVector2D(0, 0), FVector2D uv1 = FVector2D(1, 1), FLinearColor tint_col = FLinearColor(1, 1, 1, 1), FLinearColor border_col = FLinearColor(0, 0, 0, 0));
	//UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          ImageButton(ImTextureID user_texture_id, FVector2D size, FVector2D uv0 = FVector2D(0, 0), FVector2D uv1 = FVector2D(1, 1), int frame_padding = -1, FLinearColor bg_col = FLinearColor(0, 0, 0, 0), FLinearColor tint_col = FLinearColor(1, 1, 1, 1));    // <0 frame_padding uses default frame padding settings. 0 for no padding
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          Checkbox(FString label, UPARAM(ref) bool& v);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          RadioButton(FString label, bool active);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static bool          RadioButtonRef(FString label, UPARAM(ref) int& v, int v_button);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static void          PlotLines(FString label, TArray<float> values, FString overlay_text, float scale_min = 3.402823466e+38F, float scale_max = 3.402823466e+38F, FVector2D graph_size = FVector2D(0, 0));
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static void          PlotHistogram(FString label, TArray<float> values, FString overlay_text , float scale_min = 3.402823466e+38F, float scale_max = 3.402823466e+38F, FVector2D graph_size = FVector2D(0, 0));
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static void          ProgressBar(float fraction, FVector2D size_arg = FVector2D(-1, 0), FString overlay = "");
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets")) static void          Bullet();                                                       // draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses

	// Widgets: Combo Box
	// The new BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it.
	// The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Combo box")) static bool          BeginCombo(FString label, FString preview_value, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiSelectableFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Combo box")) static void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Combo box")) static bool          ComboArray(FString label, UPARAM(ref) int& current_item, TArray<FString> items, int popup_max_height_in_items = -1);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Combo box")) static bool          Combo(FString label, UPARAM(ref) int& current_item, FString items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"

	// Widgets: Drags (tip: ctrl+click on a drag box to input with keyboard. manually input values aren't clamped, can go off-bounds)
	// For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
	// Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
	// Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragFloat(FString label, UPARAM(ref) float& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, FString format = "%.3f", float power = 1.0f);     // If v_min >= v_max we have no bound
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragFloatVec2(FString label, UPARAM(ref) FVector2D & v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, FString format = "%.3f", float power = 1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragFloatVec3(FString label, UPARAM(ref) FVector & v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, FString format = "%.3f", float power = 1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragFloatVec4(FString label, UPARAM(ref) FVector4 & v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, FString format = "%.3f", float power = 1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragFloatRange2(FString label, UPARAM(ref) float& v_current_min, float& v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, FString format = "%.3f", FString format_max = "", float power = 1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragInt(FString label, UPARAM(ref) int& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, FString format = "%d");                                       // If v_min >= v_max we have no bound
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Drags")) static bool          DragIntRange2(FString label, UPARAM(ref) int& v_current_min, UPARAM(ref) int& v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, FString format = "%d", FString format_max = "");

	// Widgets: Input with Keyboard
	// Handle it later
	/*UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputText(FString label, char* buf, size_t buf_size, EImGuiInputTextFlags_ flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputTextMultiline(FString label, char* buf, size_t buf_size, FVector2D size = FVector2D(0, 0), EImGuiInputTextFlags_ flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputFloat(FString label, float* v, float step = 0.0f, float step_fast = 0.0f, FString format = "%.3f", EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputFloat2(FString label, float v[2], FString format = "%.3f", EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputFloat3(FString label, float v[3], FString format = "%.3f", EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputFloat4(FString label, float v[4], FString format = "%.3f", EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputInt(FString label, int* v, int step = 1, int step_fast = 100, EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputInt2(FString label, int v[2], EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputInt3(FString label, int v[3], EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputInt4(FString label, int v[4], EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputDouble(FString label, double* v, double step = 0.0f, double step_fast = 0.0f, FString format = "%.6f", EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputScalar(FString label, EImGuiDataType_ data_type, void* v, const void* step = NULL, const void* step_fast = NULL, FString format = NULL, EImGuiInputTextFlags_ extra_flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Input with keyboard")) static bool          InputScalarN(FString label, EImGuiDataType_ data_type, void* v, int components, const void* step = NULL, const void* step_fast = NULL, FString format = NULL, EImGuiInputTextFlags_ extra_flags = 0);*/

	// Widgets: Sliders (tip: ctrl+click on a slider to input with keyboard. manually input values aren't clamped, can go off-bounds)
	// Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderFloat(FString label, UPARAM(ref) float& v, float v_min, float v_max, FString format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderFloat2(FString label, UPARAM(ref) FVector2D & v, float v_min, float v_max, FString format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderFloat3(FString label, UPARAM(ref) FVector& v, float v_min, float v_max, FString format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderFloat4(FString label, UPARAM(ref) FVector4& v, float v_min, float v_max, FString format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderAngle(FString label, UPARAM(ref) float& v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          SliderInt(FString label, UPARAM(ref) int& v, int v_min, int v_max, FString format = "%d");
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          VSliderFloat(FString label, FVector2D size, UPARAM(ref) float& v, float v_min, float v_max, FString format = "%.3f", float power = 1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Sliders")) static bool          VSliderInt(FString label, FVector2D size, UPARAM(ref) int& v, int v_min, int v_max, FString format = "%d");

	// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little colored preview square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
	// Note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can the pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Color")) static bool          ColorEdit(FString label, FLinearColor col, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiColorEditFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Color")) static bool          ColorPicker(FString label, FLinearColor col, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiColorEditFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Color")) static bool          ColorButton(FString desc_id, FLinearColor col, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiColorEditFlags_)) int32 flags = 0, FVector2D size = FVector2D(0, 0));  // display a colored square/button, hover for details, return true when pressed.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Color")) static void          SetColorEditOptions(UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiColorEditFlags_)) int32 flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

	// Widgets: Trees
	// TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          TreeNode(FString label);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          TreeNodeID(FString str_id, FString fmt);  // helper variation to completely decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          TreeNodeEx(FString label, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiTreeNodeFlags_)) int32 flags = 0);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          TreeNodeExID(FString str_id, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiTreeNodeFlags_)) int32 flags, FString fmt);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static void          TreePush(FString str_id);                                       // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static void          TreePop();                                                          // ~ Unindent()+PopId()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static void          TreeAdvanceToLabelPos();                                            // advance cursor x position by GetTreeNodeToLabelSpacing()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static void          SetNextTreeNodeOpen(bool is_open, EImGuiCond_ cond = EImGuiCond_::EImGuiCond_Once);              // set next TreeNode/CollapsingHeader open state.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          CollapsingHeader(FString label, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiTreeNodeFlags_)) int32 flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Trees")) static bool          CollapsingHeaderWithOpen(FString label, UPARAM(ref) bool& p_open, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiTreeNodeFlags_)) int32 flags = 0); // when 'p_open' isn't NULL, display an additional small close button on upper right of the header

	// Widgets: Selectable / Lists
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static bool          Selectable(FString label, bool selected = false, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiSelectableFlags_)) int32 flags = 0, FVector2D size = FVector2D(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static bool          SelectableByRef(FString label, UPARAM(ref) bool& p_selected, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiSelectableFlags_)) int32 flags = 0, FVector2D size = FVector2D(0, 0));       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static bool          ListBox(FString label, UPARAM(ref) int& current_item, TArray<FString> items, int height_in_items = -1);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static bool          ListBoxHeader(FString label, FVector2D size = FVector2D(0, 0)); // use if you want to reimplement ListBox() will custom data or interactions. if the function return true, you can output elements then call ListBoxFooter() afterwards.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static bool          ListBoxHeaderByCount(FString label, int items_count, int height_in_items = -1); // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Selectable & Lists")) static void          ListBoxFooter();                                                    // terminate the scrolling region. only call ListBoxFooter() if ListBoxHeader() returned true!

	// Widgets: Value() Helpers. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Values helpers")) static void          ValueBool(FString prefix, bool b);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Values helpers")) static void          ValueInt(FString prefix, int32 v);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Values helpers")) static void          ValueFloat(FString prefix, float v, FString float_format);

	// Tooltips
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Tooltips")) static void          BeginTooltip();                                                     // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Tooltips")) static void          EndTooltip();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Tooltips")) static void          SetTooltip(FString fmt);                  // set a text-only tooltip, typically use with ImGui::IsItemHovered(). overidde any previous call to SetTooltip().

	// Menus
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires ImGuiWindowFlags__MenuBar flag set on parent window).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static bool          BeginMenu(FString label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static bool          MenuItem(FString label, FString shortcut, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Menus")) static bool          MenuItemByRef(FString label, FString shortcut, UPARAM(ref) bool& p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

	// Popups
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static void          OpenPopup(FString str_id);                                      // call to mark popup as open (don't call every frame!). popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. By default, Selectable()/MenuItem() are calling CloseCurrentPopup(). Popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          BeginPopup(FString str_id, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0);                                             // return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          BeginPopupContextItem(FString str_id, int mouse_button = 1);                                 // helper to open and begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          BeginPopupContextWindow(FString str_id, int mouse_button = 1, bool also_over_items = true);  // helper to open and begin popup when clicked on current window.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          BeginPopupContextVoid(FString str_id, int mouse_button = 1);                                 // helper to open and begin popup when clicked in void (where there are no imgui windows).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          BeginPopupModal(FString name, UPARAM(ref) bool& p_open, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0);                     // modal dialog (regular window with title bar, block interactions behind the modal window, can't close the modal window by clicking outside)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static void          EndPopup();                                                                                             // only call EndPopup() if BeginPopupXXX() returns true!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          OpenPopupOnItemClick(FString str_id, int mouse_button = 1);                                  // helper to open popup when clicked on last item. return true when just opened.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static bool          IsPopupOpen(FString str_id);                                    // return true if the popup is open
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Popups")) static void          CloseCurrentPopup();                                                // close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.

	// Columns
	// You can also use SameLine(pos_x) for simplified columns. The columns API is still work-in-progress and rather lacking.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static void          Columns(int count = 1, FString id = "", bool border = true);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static void          NextColumn();                                                       // next column, defaults to current row or next row if the current row is finished
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static int           GetColumnIndex();                                                   // get current column index
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static float         GetColumnWidth(int column_index = -1);                              // get column width (in pixels). pass -1 to use current column
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static void          SetColumnWidth(int column_index, float width);                      // set column width (in pixels). pass -1 to use current column
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static float         GetColumnOffset(int column_index = -1);                             // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static void          SetColumnOffset(int column_index, float offset_x);                  // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Columns")) static int           GetColumnsCount();

	// Logging/Capture: all text output from interface is captured to tty/file/clipboard. By default, tree nodes are automatically opened during logging.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogToTTY(int max_depth = -1);                                       // start logging to tty
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogToFile(int max_depth = -1, FString filename = "");         // start logging to file
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogToClipboard(int max_depth = -1);                                 // start logging to OS clipboard
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogFinish();                                                        // stop logging (close file, etc.)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Logging")) static void          LogText(FString fmt);                     // pass text data straight to log (without being displayed)                                                     // only call EndDragDropTarget() if BeginDragDropTarget() returns true!

	// Clipping
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Clipping")) static void          PushClipRect(FVector2D clip_rect_min, FVector2D clip_rect_max, bool intersect_with_current_clip_rect);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Clipping")) static void          PopClipRect();

	// Focus, Activation
	// (Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHere()" when applicable, to make your code more forward compatible when navigation branch is merged)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Focus")) static void          SetItemDefaultFocus();                                              // make last item the default focused item of a window. Please use instead of "if (IsWindowAppearing()) SetScrollHere()" to signify "default item".
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Focus")) static void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

	// Utilities
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemHovered(UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiHoveredFlags_)) int32 flags = 0);                         // is the last item hovered? (and usable, aka not blocked by a popup, etc.). See EImGuiHoveredFlags_ for more options.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemActive();                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemFocused();                                                    // is the last item focused for keyboard/gamepad navigation?
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemClicked(int mouse_button = 0);                                // is the last item clicked? (e.g. button/node just clicked on) == IsMouseClicked(mouse_button) && IsItemHovered()
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemVisible();                                                    // is the last item visible? (items may be out of sight because of clipping/scrolling)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemDeactivated();                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsItemDeactivatedAfterChange();                                     // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsAnyItemHovered();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsAnyItemActive();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsAnyItemFocused();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static FVector2D        GetItemRectMin();                                                   // get bounding rectangle of last item, in screen space
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static FVector2D        GetItemRectMax();                                                   // "
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static FVector2D        GetItemRectSize();                                                  // get size of last item, in screen space
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static void          SetItemAllowOverlap();                                              // allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsRectVisible(FVector2D size);                                  // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          IsRectVisibleByCorners(FVector2D rect_min, FVector2D rect_max);      // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static float         GetTime();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static int           GetFrameCount();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static FString   GetStyleColorName(EImGuiCol_ idx);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static FVector2D        CalcTextSize(FString text, FString text_end, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static void          CalcListClipping(int items_count, float items_height, int& out_items_display_start, int& out_items_display_end);    // calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.

	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static bool          BeginChildFrame(int32 id, FVector2D size, UPARAM(meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_)) int32 flags = 0); // helper to create a child window / scrolling region that looks like a normal widget frame
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static void          EndChildFrame();                                                    // always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)

	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Utilities")) static void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

	// Inputs
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static int           GetKeyIndex(EImGuiKey_ imgui_key);                                    // map ImGuiKey_* values into user's key index. == io.KeyMap[key]
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsKeyDown(int user_key_index);                                      // is key being held. == io.KeysDown[user_key_index]. note that imgui doesn't know the semantic of each entry of io.KeysDown[]. Use your own indices/enums according to how your backend/engine stored them into io.KeysDown[]!
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsKeyPressed(int user_key_index, bool repeat = true);               // was key pressed (went from !Down to Down). if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsKeyReleased(int user_key_index);                                  // was key released (went from Down to !Down)..
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static int           GetKeyPressedAmount(int key_index, float repeat_delay, float rate); // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseDown(int button);                                            // is mouse button held
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsAnyMouseDown();                                                   // is any mouse button held
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseClicked(int button, bool repeat = false);                    // did mouse button clicked (went from !Down to Down)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseDoubleClicked(int button);                                   // did mouse button double-clicked. a double-click returns false in IsMouseClicked(). uses io.MouseDoubleClickTime.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseReleased(int button);                                        // did mouse button released (went from Down to !Down)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseDragging(int button = 0, float lock_threshold = -1.0f);      // is mouse dragging. if lock_threshold < -1.0f uses io.MouseDraggingThreshold
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMouseHoveringRect(FVector2D r_min, FVector2D r_max, bool clip = true);  // is mouse hovering given bounding rect (in screen space). clipped by current clipping settings. disregarding of consideration of focus/window ordering/blocked by a popup.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static bool          IsMousePosValid(const FVector2D& mouse_pos);                    //
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static FVector2D        GetMousePos();                                                      // shortcut to ImGui::GetIO().MousePos provided by user, to be consistent with other calls
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static FVector2D        GetMousePosOnOpeningCurrentPopup();                                 // retrieve backup of mouse position at the time of opening popup we have BeginPopup() into
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static FVector2D        GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);    // dragging amount since clicking. if lock_threshold < -1.0f uses io.MouseDraggingThreshold
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static void          ResetMouseDragDelta(int button = 0);                                //
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static EImGuiMouseCursor_ GetMouseCursor();                                                // get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static void          SetMouseCursor(EImGuiMouseCursor_ type);                              // set desired cursor type
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static void          CaptureKeyboardFromApp(bool capture = true);                        // manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered.
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Inputs")) static void          CaptureMouseFromApp(bool capture = true);                           // manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle).

	// Clipboard Utilities (also see the LogToClipboard() function to capture or output text data to the clipboard)
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Clipboard")) static FString   GetClipboardText();
	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Widgets|Clipboard")) static void          SetClipboardText(FString text);

};