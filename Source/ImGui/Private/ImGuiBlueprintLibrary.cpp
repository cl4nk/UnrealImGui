#include "ImGuiPrivatePCH.h"

#include "ImGuiBlueprintLibrary.h"
#include "ImGuiModule.h"
#include "ImGuiModuleManager.h"
#include <string>
#include <imgui.h>

#define IMVEC2_TO_FVEC2(vec) FVector2D(vec.x, vec.y)
#define FVEC2_TO_IMVEC2(vec) ImVec2(vec.X, vec.Y)

#define IMVEC4_TO_FVEC4(vec) FVector4(vec.x, vec.y, vec.z, vec.w)
#define FVEC4_TO_IMVEC4(vec) ImVec4(vec.X, vec.Y, vec.Z, vec.W)
#define COLOR_TO_IMVEC4(vec) ImVec4(vec.R, vec.G, vec.B, vec.A)

namespace 
{
	inline static TArray<std::string> FNamesToStdStrings(const TArray<FName>& Items)
	{
		TArray<std::string> Results;
		for (FName Item : Items)
		{
			Results.Add(std::string(TCHAR_TO_ANSI(*Item.ToString())));
		}
		return Results;
	}

	inline static TArray<std::string> FStringsToStdStrings(const TArray<FString>& Items)
	{
		TArray<std::string> Results;
		for (FString Item : Items)
		{
			Results.Add(std::string(TCHAR_TO_ANSI(*Item)));
		}
		return Results;
	}

	inline static TArray<char const *> StdStringsToCCharPtrs(const TArray<std::string>& Items)
	{
		TArray<char const *> Results;
		for (int i = 0; i < Items.Num(); ++i)
		{
			Results.Add(Items[i].c_str());
		}
		return Results;
	}
}

bool UImGuiBlueprintLibrary::SetCurrentImGuiContext(UObject * WorldContextObject, FName ContextName)
{
	FImGuiModule& ImGuiModule = FImGuiModule::Get();
	FImGuiModuleManager* ImGuiModuleManager = ImGuiModule.GetImGuiModuleManager();

	if (ImGuiModuleManager)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		ImGuiModuleManager->SetContextAsCurrent(World, ContextName);
	}

	return ImGuiModuleManager != nullptr;
}

bool UImGuiBlueprintLibrary::Begin(FString name, int32 flags)
{
	return ImGui::Begin(TCHAR_TO_ANSI(*name), nullptr, flags);
}

bool UImGuiBlueprintLibrary::BeginWithOpen(FString name, bool& p_open, int32 flags)
{
	return ImGui::Begin(TCHAR_TO_ANSI(*name), &p_open, flags);
}

void UImGuiBlueprintLibrary::End()
{
	ImGui::End();
}

bool UImGuiBlueprintLibrary::BeginChildStr(FString str_id, FVector2D size, bool border, int32 flags)
{
	return ImGui::BeginChild(TCHAR_TO_ANSI(*str_id), FVEC2_TO_IMVEC2(size), border, flags);
}

bool UImGuiBlueprintLibrary::BeginChildInt(int32 id, FVector2D size, bool border, int32 flags)
{
	return ImGui::BeginChild(id, FVEC2_TO_IMVEC2(size), border, flags);
}

void UImGuiBlueprintLibrary::EndChild()
{
	ImGui::EndChild();
}

bool UImGuiBlueprintLibrary::IsWindowAppearing()
{
	return ImGui::IsWindowAppearing();
}

bool UImGuiBlueprintLibrary::IsWindowCollapsed()
{
	return ImGui::IsWindowCollapsed();
}

bool UImGuiBlueprintLibrary::IsWindowFocused(int32 flags)
{
	return ImGui::IsWindowFocused(flags);
}

bool UImGuiBlueprintLibrary::IsWindowHovered(int32 flags)
{
	return ImGui::IsWindowHovered(flags);
}

FVector2D UImGuiBlueprintLibrary::GetWindowPos()
{
	return IMVEC2_TO_FVEC2(ImGui::GetWindowPos());
}

FVector2D UImGuiBlueprintLibrary::GetWindowSize()
{
	return IMVEC2_TO_FVEC2(ImGui::GetWindowSize());
}

float UImGuiBlueprintLibrary::GetWindowWidth()
{
	return ImGui::GetWindowWidth();
}

float UImGuiBlueprintLibrary::GetWindowHeight()
{
	return ImGui::GetWindowHeight();
}

FVector2D UImGuiBlueprintLibrary::GetContentRegionMax()
{
	return IMVEC2_TO_FVEC2(ImGui::GetContentRegionMax());
}

FVector2D UImGuiBlueprintLibrary::GetContentRegionAvail()
{
	return IMVEC2_TO_FVEC2(ImGui::GetContentRegionAvail());
}

float UImGuiBlueprintLibrary::GetContentRegionAvailWidth()
{
	return ImGui::GetContentRegionAvailWidth();
}

FVector2D UImGuiBlueprintLibrary::GetWindowContentRegionMin()
{
	return IMVEC2_TO_FVEC2(ImGui::GetWindowContentRegionMin());

}

FVector2D UImGuiBlueprintLibrary::GetWindowContentRegionMax()
{
	return IMVEC2_TO_FVEC2(ImGui::GetWindowContentRegionMax());
}

float UImGuiBlueprintLibrary::GetWindowContentRegionWidth()
{
	return ImGui::GetWindowContentRegionWidth();
}


void UImGuiBlueprintLibrary::SetNextWindowPos(FVector2D pos, EImGuiCond_ cond, FVector2D pivot)
{
	ImGui::SetNextWindowPos(FVEC2_TO_IMVEC2(pos), cond, FVEC2_TO_IMVEC2(pivot));
}

void UImGuiBlueprintLibrary::SetNextWindowSize(FVector2D size, EImGuiCond_ cond)
{
	ImGui::SetNextWindowSize(FVEC2_TO_IMVEC2(size), cond);
}

void UImGuiBlueprintLibrary::SetNextWindowContentSize(FVector2D size)
{
	ImGui::SetNextWindowContentSize(FVEC2_TO_IMVEC2(size));
}

void UImGuiBlueprintLibrary::SetNextWindowCollapsed(bool collapsed, EImGuiCond_ cond)
{
	ImGui::SetNextWindowCollapsed(collapsed, cond);
}

void UImGuiBlueprintLibrary::SetNextWindowFocus()
{
	ImGui::SetNextWindowFocus();
}

void UImGuiBlueprintLibrary::SetNextWindowBgAlpha(float alpha)
{
	ImGui::SetNextWindowBgAlpha(alpha);
}

void UImGuiBlueprintLibrary::SetWindowPos(FVector2D pos, EImGuiCond_ cond)
{
	ImGui::SetWindowPos(FVEC2_TO_IMVEC2(pos), cond);
}

void UImGuiBlueprintLibrary::SetWindowSize(FVector2D size, EImGuiCond_ cond)
{
	ImGui::SetWindowSize(FVEC2_TO_IMVEC2(size), cond);
}

void UImGuiBlueprintLibrary::SetWindowCollapsed(bool collapsed, EImGuiCond_ cond)
{
	ImGui::SetWindowCollapsed(collapsed, cond);
}

void UImGuiBlueprintLibrary::SetWindowFocus()
{
	ImGui::SetWindowFocus();
}

void UImGuiBlueprintLibrary::SetWindowFontScale(float scale)
{
	ImGui::SetWindowFontScale(scale);
}

void UImGuiBlueprintLibrary::SetWindowPosByName(FString name, FVector2D pos, EImGuiCond_ cond)
{
	ImGui::SetWindowPos(TCHAR_TO_ANSI(*name), FVEC2_TO_IMVEC2(pos), cond);
}

void UImGuiBlueprintLibrary::SetWindowSizeByName(FString name, FVector2D size, EImGuiCond_ cond)
{
	ImGui::SetWindowSize(TCHAR_TO_ANSI(*name), FVEC2_TO_IMVEC2(size), cond);
}

void UImGuiBlueprintLibrary::SetWindowCollapsedByName(FString name, bool collapsed, EImGuiCond_ cond)
{
	ImGui::SetWindowCollapsed(TCHAR_TO_ANSI(*name), collapsed, cond);
}

void UImGuiBlueprintLibrary::SetWindowFocusByName(FString name)
{
	ImGui::SetWindowFocus(TCHAR_TO_ANSI(*name));
}
																																										 // Windows Scrolling
float UImGuiBlueprintLibrary::GetScrollX()
{
	return ImGui::GetScrollX();
}

float UImGuiBlueprintLibrary::GetScrollY()
{
	return ImGui::GetScrollY();
}

float UImGuiBlueprintLibrary::GetScrollMaxX()
{
	return ImGui::GetScrollMaxX();
}

float UImGuiBlueprintLibrary::GetScrollMaxY()
{
	return ImGui::GetScrollMaxY();
}

void UImGuiBlueprintLibrary::SetScrollX(float scroll_x)
{
	ImGui::SetScrollX(scroll_x);
}

void UImGuiBlueprintLibrary::SetScrollY(float scroll_y)
{
	ImGui::SetScrollY(scroll_y);
}

void UImGuiBlueprintLibrary::SetScrollHere(float center_y_ratio)
{
	ImGui::SetScrollHere(center_y_ratio);
}

void UImGuiBlueprintLibrary::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
	ImGui::SetScrollFromPosY(pos_y, center_y_ratio);
}

// Parameters stacks (shared

void UImGuiBlueprintLibrary::PushStyleColor(EImGuiCol_ idx, FLinearColor col)
{
	ImGui::PushStyleColor(idx, COLOR_TO_IMVEC4(col));
}

void UImGuiBlueprintLibrary::PopStyleColor(int count)
{
	ImGui::PopStyleColor(count);
}

void UImGuiBlueprintLibrary::PushStyleVar(EImGuiStyleVar_ idx, float val)
{
	ImGui::PushStyleVar(idx, val);
}

void UImGuiBlueprintLibrary::PushStyleVarVec2(EImGuiStyleVar_ idx, FVector2D val)
{
	ImGui::PushStyleVar(idx, FVEC2_TO_IMVEC2(val));
}

void UImGuiBlueprintLibrary::PopStyleVar(int count)
{
	ImGui::PopStyleVar(count);
}

FVector4 UImGuiBlueprintLibrary::GetStyleColorVec4(EImGuiCol_ idx)
{
	return IMVEC4_TO_FVEC4(ImGui::GetStyleColorVec4(idx));
}

float UImGuiBlueprintLibrary::GetFontSize()
{
	return ImGui::GetFontSize();
}

FVector2D UImGuiBlueprintLibrary::GetFontTexUvWhitePixel()
{
	return IMVEC2_TO_FVEC2(ImGui::GetFontTexUvWhitePixel());
}
																																										// Parameters stacks (current window)
void UImGuiBlueprintLibrary::PushItemWidth(float item_width)
{
	ImGui::PushItemWidth(item_width);
}

void UImGuiBlueprintLibrary::PopItemWidth()
{
	ImGui::PopItemWidth();
}

float UImGuiBlueprintLibrary::CalcItemWidth()
{
	return ImGui::CalcItemWidth();
}

void UImGuiBlueprintLibrary::PushTextWrapPos(float wrap_pos_x)
{
	ImGui::PushTextWrapPos(wrap_pos_x);
}

void UImGuiBlueprintLibrary::PopTextWrapPos()
{
	ImGui::PopTextWrapPos();
}

void UImGuiBlueprintLibrary::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
	ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
}

void UImGuiBlueprintLibrary::PopAllowKeyboardFocus()
{
	ImGui::PopAllowKeyboardFocus();
}

void UImGuiBlueprintLibrary::PushButtonRepeat(bool repeat)
{
	ImGui::PushButtonRepeat(repeat);
}

void UImGuiBlueprintLibrary::PopButtonRepeat()
{
	ImGui::PopButtonRepeat();
}

void UImGuiBlueprintLibrary::Separator()
{
	ImGui::Separator();
}

void UImGuiBlueprintLibrary::SameLine(float pos_x, float spacing_w)
{
	ImGui::SameLine(pos_x, spacing_w);
}

void UImGuiBlueprintLibrary::NewLine()
{
	ImGui::NewLine();
}

void UImGuiBlueprintLibrary::Spacing()
{
	ImGui::Spacing();
}

void UImGuiBlueprintLibrary::Dummy(FVector2D size)
{
	ImGui::Dummy(FVEC2_TO_IMVEC2(size));
}

void UImGuiBlueprintLibrary::Indent(float indent_w)
{
	ImGui::Indent(indent_w);
}

void UImGuiBlueprintLibrary::Unindent(float indent_w)
{
	ImGui::Unindent(indent_w);
}

void UImGuiBlueprintLibrary::BeginGroup()
{
	ImGui::BeginGroup();
}

void UImGuiBlueprintLibrary::EndGroup()
{
	ImGui::EndGroup();
}

FVector2D UImGuiBlueprintLibrary::GetCursorPos()
{
	return IMVEC2_TO_FVEC2(ImGui::GetCursorPos());
}

float UImGuiBlueprintLibrary::GetCursorPosX()
{
	return ImGui::GetCursorPosX();
}

float UImGuiBlueprintLibrary::GetCursorPosY()
{
	return ImGui::GetCursorPosY();
}

void UImGuiBlueprintLibrary::SetCursorPos(FVector2D local_pos)
{
	ImGui::SetCursorPos(FVEC2_TO_IMVEC2(local_pos));
}

void UImGuiBlueprintLibrary::SetCursorPosX(float x)
{
	ImGui::SetCursorPosX(x);
}

void UImGuiBlueprintLibrary::SetCursorPosY(float y)
{
	ImGui::SetCursorPosY(y);
}

FVector2D UImGuiBlueprintLibrary::GetCursorStartPos()
{
	return IMVEC2_TO_FVEC2(ImGui::GetCursorStartPos());
}

FVector2D UImGuiBlueprintLibrary::GetCursorScreenPos()
{
	return IMVEC2_TO_FVEC2(ImGui::GetCursorScreenPos());
}

void UImGuiBlueprintLibrary::SetCursorScreenPos(FVector2D screen_pos)
{
	ImGui::SetCursorScreenPos(FVEC2_TO_IMVEC2(screen_pos));
}

void UImGuiBlueprintLibrary::AlignTextToFramePadding()
{
	ImGui::AlignTextToFramePadding();
}

float UImGuiBlueprintLibrary::GetTextLineHeight()
{
	return ImGui::GetTextLineHeight();
}

float UImGuiBlueprintLibrary::GetTextLineHeightWithSpacing()
{
	return ImGui::GetTextLineHeightWithSpacing();
}

float UImGuiBlueprintLibrary::GetFrameHeight()
{
	return ImGui::GetFrameHeight();
}

float UImGuiBlueprintLibrary::GetFrameHeightWithSpacing()
{
	return ImGui::GetFrameHeightWithSpacing();
}

void UImGuiBlueprintLibrary::PushIDStr(FString str_id)
{
	ImGui::PushID(TCHAR_TO_ANSI(*str_id));
}

void UImGuiBlueprintLibrary::PushIDSubStr(FString str_id_begin, FString str_id_end)
{
	ImGui::PushID(TCHAR_TO_ANSI(*str_id_begin), TCHAR_TO_ANSI(*str_id_end));
}

void UImGuiBlueprintLibrary::PushIDInt(int32 int_id)
{
	ImGui::PushID(int_id);
}

void UImGuiBlueprintLibrary::PopID()
{
	ImGui::PopID();
}

int32 UImGuiBlueprintLibrary::GetID(FString str_id)
{
	return ImGui::GetID(TCHAR_TO_ANSI(*str_id));
}

int32 UImGuiBlueprintLibrary::GetIDSubStr(FString str_id_begin, FString str_id_end)
{
	return ImGui::GetID(TCHAR_TO_ANSI(*str_id_begin), TCHAR_TO_ANSI(*str_id_end));
}

void UImGuiBlueprintLibrary::Text(FString fmt)
{
	ImGui::Text(TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::TextColored(FLinearColor col, FString fmt)
{
	ImGui::TextColored(COLOR_TO_IMVEC4(col), TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::TextDisabled(FString fmt)
{
	ImGui::TextDisabled(TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::TextWrapped(FString fmt)
{
	ImGui::TextWrapped(TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::LabelText(FString label, FString fmt)
{
	ImGui::LabelText(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::BulletText(FString fmt)
{
	ImGui::BulletText(TCHAR_TO_ANSI(*fmt));
}

bool UImGuiBlueprintLibrary::Button(FString label, FVector2D size)
{
	return ImGui::Button(TCHAR_TO_ANSI(*label), FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::SmallButton(FString label)
{
	return ImGui::SmallButton(TCHAR_TO_ANSI(*label));
}

bool UImGuiBlueprintLibrary::InvisibleButton(FString str_id, FVector2D size)
{
	return ImGui::InvisibleButton(TCHAR_TO_ANSI(*str_id), FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::ArrowButton(FString str_id, EImGuiDir_ dir)
{
	return ImGui::ArrowButton(TCHAR_TO_ANSI(*str_id), dir);
}

/*
void UImGuiBlueprintLibrary::Image(ImTextureID user_texture_id, FVector2D size, FVector2D uv0, FVector2D uv1, FLinearColor tint_col, FLinearColor border_col)
{
	// TODO
}

bool UImGuiBlueprintLibrary::ImageButton(ImTextureID user_texture_id, FVector2D size, FVector2D uv0, FVector2D uv1, int frame_padding, FLinearColor bg_col, FLinearColor tint_col)
{
	// TODO
}
*/

bool UImGuiBlueprintLibrary::Checkbox(FString label, bool& v)
{
	return ImGui::Checkbox(TCHAR_TO_ANSI(*label), &v);
}

bool UImGuiBlueprintLibrary::RadioButton(FString label, bool active)
{
	return ImGui::RadioButton(TCHAR_TO_ANSI(*label), active);
}

bool UImGuiBlueprintLibrary::RadioButtonRef(FString label, int& v, int v_button)
{
	return ImGui::RadioButton(TCHAR_TO_ANSI(*label), &v, v_button);
}

void UImGuiBlueprintLibrary::PlotLines(FString label, TArray<float> values, FString overlay_text, float scale_min, float scale_max, FVector2D graph_size)
{
	ImGui::PlotLines(TCHAR_TO_ANSI(*label), values.GetData(), values.Num(), 0, TCHAR_TO_ANSI(*overlay_text), scale_min, scale_max, FVEC2_TO_IMVEC2(graph_size));
}

void UImGuiBlueprintLibrary::PlotHistogram(FString label, TArray<float> values, FString overlay_text, float scale_min, float scale_max, FVector2D graph_size)
{
	ImGui::PlotHistogram(TCHAR_TO_ANSI(*label), values.GetData(), values.Num(), 0, TCHAR_TO_ANSI(*overlay_text), scale_min, scale_max, FVEC2_TO_IMVEC2(graph_size));
}

void UImGuiBlueprintLibrary::ProgressBar(float fraction, FVector2D size_arg, FString overlay)
{
	ImGui::ProgressBar(fraction, FVEC2_TO_IMVEC2(size_arg), TCHAR_TO_ANSI(*overlay));
}

void UImGuiBlueprintLibrary::Bullet()
{
	ImGui::Bullet();
}

bool UImGuiBlueprintLibrary::BeginCombo(FString label, FString preview_value, int32 flags)
{
	return ImGui::BeginCombo(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*preview_value), flags);
}

void UImGuiBlueprintLibrary::EndCombo()
{
	ImGui::EndCombo();
}

bool UImGuiBlueprintLibrary::ComboArrayString(FString label, int& current_item, TArray<FString> items, int popup_max_height_in_items)
{
	TArray<std::string> items_str = FStringsToStdStrings(items);
	TArray<const char *> items_c = StdStringsToCCharPtrs(items_str);

	return ImGui::Combo(TCHAR_TO_ANSI(*label), &current_item, items_c.GetData(), items_c.Num(), popup_max_height_in_items);
}

bool UImGuiBlueprintLibrary::ComboArrayName(FString label, int& current_item, TArray<FName> items, int popup_max_height_in_items)
{
	TArray<std::string> items_str = FNamesToStdStrings(items);
	TArray<const char *> items_c = StdStringsToCCharPtrs(items_str);

	return ImGui::Combo(TCHAR_TO_ANSI(*label), &current_item, items_c.GetData(), items_c.Num(), popup_max_height_in_items);
}

bool UImGuiBlueprintLibrary::Combo(FString label, int& current_item, FString items_separated_by_zeros, int popup_max_height_in_items)
{
	return ImGui::Combo(TCHAR_TO_ANSI(*label), &current_item, TCHAR_TO_ANSI(*items_separated_by_zeros), popup_max_height_in_items);
}

bool UImGuiBlueprintLibrary::DragFloat(FString label, float& v, float v_speed, float v_min, float v_max, FString format, float power)
{
	return ImGui::DragFloat(TCHAR_TO_ANSI(*label), &v, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::DragFloatVec2(FString label, FVector2D & v, float v_speed, float v_min, float v_max, FString format, float power)
{
	return ImGui::DragFloat2(TCHAR_TO_ANSI(*label), &v.X, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::DragFloatVec3(FString label, FVector & v, float v_speed, float v_min, float v_max, FString format, float power)
{
	return ImGui::DragFloat3(TCHAR_TO_ANSI(*label), &v.X, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::DragFloatVec4(FString label, FVector4 & v, float v_speed, float v_min, float v_max, FString format, float power)
{
	return ImGui::DragFloat4(TCHAR_TO_ANSI(*label), &v.X, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::DragFloatRange2(FString label, float& v_current_min, float& v_current_max, float v_speed, float v_min, float v_max, FString format, FString format_max, float power)
{
	return ImGui::DragFloatRange2(TCHAR_TO_ANSI(*label), &v_current_min, &v_current_max, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), TCHAR_TO_ANSI(*format_max), power);
}

bool UImGuiBlueprintLibrary::DragInt(FString label, int& v, float v_speed, int v_min, int v_max, FString format)
{
	return ImGui::DragInt(TCHAR_TO_ANSI(*label), &v, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format));
}

bool UImGuiBlueprintLibrary::DragIntRange2(FString label, int& v_current_min, int& v_current_max, float v_speed, int v_min, int v_max, FString format, FString format_max)
{
	return ImGui::DragIntRange2(TCHAR_TO_ANSI(*label), &v_current_min, &v_current_max, v_speed, v_min, v_max, TCHAR_TO_ANSI(*format), TCHAR_TO_ANSI(*format_max));
}


/*bool          UImGuiBlueprintLibrary::InputText(FString label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
bool          UImGuiBlueprintLibrary::InputTextMultiline(FString label, char* buf, size_t buf_size, FVector2D size = FVector2D(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
bool          UImGuiBlueprintLibrary::InputFloat(FString label, float* v, float step = 0.0f, float step_fast = 0.0f, FString format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputFloat2(FString label, float v[2], FString format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputFloat3(FString label, float v[3], FString format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputFloat4(FString label, float v[4], FString format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputInt(FString label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputInt2(FString label, int v[2], ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputInt3(FString label, int v[3], ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputInt4(FString label, int v[4], ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputDouble(FString label, double* v, double step = 0.0f, double step_fast = 0.0f, FString format = "%.6f", ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputScalar(FString label, ImGuiDataType data_type, void* v, const void* step = NULL, const void* step_fast = NULL, FString format = NULL, ImGuiInputTextFlags extra_flags = 0);
bool          UImGuiBlueprintLibrary::InputScalarN(FString label, ImGuiDataType data_type, void* v, int components, const void* step = NULL, const void* step_fast = NULL, FString format = NULL, ImGuiInputTextFlags extra_flags = 0);*/

bool UImGuiBlueprintLibrary::SliderFloat(FString label, float& v, float v_min, float v_max, FString format, float power)
{
	return ImGui::SliderFloat(TCHAR_TO_ANSI(*label), &v, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::SliderFloat2(FString label, FVector2D& v, float v_min, float v_max, FString format, float power)
{
	return ImGui::SliderFloat2(TCHAR_TO_ANSI(*label), &v.X, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::SliderFloat3(FString label, FVector& v, float v_min, float v_max, FString format, float power)
{
	return ImGui::SliderFloat3(TCHAR_TO_ANSI(*label), &v.X, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::SliderFloat4(FString label, FVector4& v, float v_min, float v_max, FString format, float power)
{
	return ImGui::SliderFloat4(TCHAR_TO_ANSI(*label), &v.X, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::SliderAngle(FString label, float& v_rad, float v_degrees_min, float v_degrees_max)
{
	return ImGui::SliderAngle(TCHAR_TO_ANSI(*label), &v_rad, v_degrees_min, v_degrees_max);
}

bool UImGuiBlueprintLibrary::SliderInt(FString label, int& v, int v_min, int v_max, FString format)
{
	return ImGui::SliderInt(TCHAR_TO_ANSI(*label), &v, v_min, v_max, TCHAR_TO_ANSI(*format));
}

bool UImGuiBlueprintLibrary::VSliderFloat(FString label, FVector2D size, float& v, float v_min, float v_max, FString format, float power)
{
	return ImGui::VSliderFloat(TCHAR_TO_ANSI(*label), FVEC2_TO_IMVEC2(size), &v, v_min, v_max, TCHAR_TO_ANSI(*format), power);
}

bool UImGuiBlueprintLibrary::VSliderInt(FString label, FVector2D size, int& v, int v_min, int v_max, FString format)
{
	return ImGui::VSliderInt(TCHAR_TO_ANSI(*label), FVEC2_TO_IMVEC2(size), &v, v_min, v_max, TCHAR_TO_ANSI(*format));
}

bool UImGuiBlueprintLibrary::ColorEdit(FString label, FLinearColor& col, int32 flags)
{
	return ImGui::ColorEdit4(TCHAR_TO_ANSI(*label), &col.R, flags);
}

bool UImGuiBlueprintLibrary::ColorPicker(FString label, FLinearColor& col, int32 flags)
{
	return ImGui::ColorPicker4(TCHAR_TO_ANSI(*label), &col.R, flags);
}

bool UImGuiBlueprintLibrary::ColorButton(FString desc_id, FLinearColor col, int32 flags, FVector2D size)
{
	return ImGui::ColorButton(TCHAR_TO_ANSI(*desc_id), COLOR_TO_IMVEC4(col), flags, FVEC2_TO_IMVEC2(size));
}

void UImGuiBlueprintLibrary::SetColorEditOptions(int32 flags)
{
	ImGui::SetColorEditOptions(flags);
}

bool UImGuiBlueprintLibrary::TreeNode(FString label)
{
	return ImGui::TreeNode(TCHAR_TO_ANSI(*label));
}

bool UImGuiBlueprintLibrary::TreeNodeID(FString str_id, FString fmt)
{
	return ImGui::TreeNode(TCHAR_TO_ANSI(*str_id), TCHAR_TO_ANSI(*fmt));
}

bool UImGuiBlueprintLibrary::TreeNodeEx(FString label, int32 flags)
{
	return ImGui::TreeNodeEx(TCHAR_TO_ANSI(*label), flags);
}

bool UImGuiBlueprintLibrary::TreeNodeExID(FString str_id, int32 flags, FString fmt)
{
	return ImGui::TreeNodeEx(TCHAR_TO_ANSI(*str_id), flags, TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::TreePush(FString str_id)
{
	ImGui::TreePush(TCHAR_TO_ANSI(*str_id));
}

void UImGuiBlueprintLibrary::TreePop()
{
	ImGui::TreePop();
}

void UImGuiBlueprintLibrary::TreeAdvanceToLabelPos()
{
	ImGui::TreeAdvanceToLabelPos();
}

float UImGuiBlueprintLibrary::GetTreeNodeToLabelSpacing()
{
	return ImGui::GetTreeNodeToLabelSpacing();
}

void UImGuiBlueprintLibrary::SetNextTreeNodeOpen(bool is_open, EImGuiCond_ cond)
{
	ImGui::SetNextTreeNodeOpen(is_open, cond);
}

bool UImGuiBlueprintLibrary::CollapsingHeader(FString label, int32 flags)
{
	return ImGui::CollapsingHeader(TCHAR_TO_ANSI(*label), flags);
}

bool UImGuiBlueprintLibrary::CollapsingHeaderWithOpen(FString label, bool& p_open, int32 flags)
{
	return ImGui::CollapsingHeader(TCHAR_TO_ANSI(*label), &p_open, flags);
}

bool UImGuiBlueprintLibrary::Selectable(FString label, bool selected, ImGuiSelectableFlags flags, FVector2D size)
{
	return ImGui::Selectable(TCHAR_TO_ANSI(*label), selected, flags, FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::SelectableByRef(FString label, bool& p_selected, ImGuiSelectableFlags flags, FVector2D size)
{
	return ImGui::Selectable(TCHAR_TO_ANSI(*label), &p_selected, flags, FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::ListBox(FString label, int& current_item, TArray<FString> items, int height_in_items)
{
	TArray<std::string> items_str = FStringsToStdStrings(items);
	TArray<const char *> items_c = StdStringsToCCharPtrs(items_str);

	return ImGui::ListBox(TCHAR_TO_ANSI(*label), &current_item, items_c.GetData(), items.Num(), height_in_items);
}

bool UImGuiBlueprintLibrary::ListBoxHeader(FString label, FVector2D size)
{
	return ImGui::ListBoxHeader(TCHAR_TO_ANSI(*label), FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::ListBoxHeaderByCount(FString label, int items_count, int height_in_items)
{
	return ImGui::ListBoxHeader(TCHAR_TO_ANSI(*label), items_count, height_in_items);
}

void UImGuiBlueprintLibrary::ListBoxFooter()
{
	ImGui::ListBoxFooter();
}

void UImGuiBlueprintLibrary::ValueBool(FString prefix, bool b)
{
	ImGui::Value(TCHAR_TO_ANSI(*prefix), b);
}

void UImGuiBlueprintLibrary::ValueInt(FString prefix, int32 v)
{
	ImGui::Value(TCHAR_TO_ANSI(*prefix), v);
}

void UImGuiBlueprintLibrary::ValueFloat(FString prefix, float v, FString float_format)
{
	ImGui::Value(TCHAR_TO_ANSI(*prefix), v, TCHAR_TO_ANSI(*float_format));
}

void UImGuiBlueprintLibrary::BeginTooltip()
{
	ImGui::BeginTooltip();
}

void UImGuiBlueprintLibrary::EndTooltip()
{
	ImGui::EndTooltip();
}

void UImGuiBlueprintLibrary::SetTooltip(FString fmt)
{
	ImGui::SetTooltip(TCHAR_TO_ANSI(*fmt));
}

bool UImGuiBlueprintLibrary::BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void UImGuiBlueprintLibrary::EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool UImGuiBlueprintLibrary::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void UImGuiBlueprintLibrary::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool UImGuiBlueprintLibrary::BeginMenu(FString label, bool enabled)
{
	return ImGui::BeginMenu(TCHAR_TO_ANSI(*label), enabled);
}

void UImGuiBlueprintLibrary::EndMenu()
{
	ImGui::EndMenu();
}

bool UImGuiBlueprintLibrary::MenuItem(FString label, FString shortcut, bool selected, bool enabled)
{
	return ImGui::MenuItem(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*shortcut), selected, enabled);
}

bool UImGuiBlueprintLibrary::MenuItemByRef(FString label, FString shortcut, bool& p_selected, bool enabled)
{
	return ImGui::MenuItem(TCHAR_TO_ANSI(*label), TCHAR_TO_ANSI(*shortcut), &p_selected, enabled);
}

void UImGuiBlueprintLibrary::OpenPopup(FString str_id)
{
	ImGui::OpenPopup(TCHAR_TO_ANSI(*str_id));
}

bool UImGuiBlueprintLibrary::BeginPopup(FString str_id, int32 flag)
{
	return ImGui::BeginPopup(TCHAR_TO_ANSI(*str_id), flag);
}

bool UImGuiBlueprintLibrary::BeginPopupContextItem(FString str_id, int mouse_button)
{
	return ImGui::BeginPopupContextItem(TCHAR_TO_ANSI(*str_id), mouse_button);
}

bool UImGuiBlueprintLibrary::BeginPopupContextWindow(FString str_id, int mouse_button, bool also_over_items)
{
	return ImGui::BeginPopupContextWindow(TCHAR_TO_ANSI(*str_id), mouse_button, also_over_items);
}

bool UImGuiBlueprintLibrary::BeginPopupContextVoid(FString str_id, int mouse_button)
{
	return ImGui::BeginPopupContextVoid(TCHAR_TO_ANSI(*str_id), mouse_button);
}

bool UImGuiBlueprintLibrary::BeginPopupModal(FString name, int32 flags)
{
	return ImGui::BeginPopupModal(TCHAR_TO_ANSI(*name), nullptr, flags);
}

bool UImGuiBlueprintLibrary::BeginPopupModalWithOpen(FString name, bool& p_open, int32 flags)
{
	return ImGui::BeginPopupModal(TCHAR_TO_ANSI(*name), &p_open, flags);
}

void UImGuiBlueprintLibrary::EndPopup()
{
	ImGui::EndPopup();
}

bool UImGuiBlueprintLibrary::OpenPopupOnItemClick(FString str_id, int mouse_button)
{
	return ImGui::OpenPopupOnItemClick(TCHAR_TO_ANSI(*str_id), mouse_button);
}

bool UImGuiBlueprintLibrary::IsPopupOpen(FString str_id)
{
	return ImGui::IsPopupOpen(TCHAR_TO_ANSI(*str_id));
}

void UImGuiBlueprintLibrary::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

void UImGuiBlueprintLibrary::Columns(int count, FString id, bool border)
{
	ImGui::Columns(count, TCHAR_TO_ANSI(*id), border);
}

void UImGuiBlueprintLibrary::NextColumn()
{
	ImGui::NextColumn();
}

int UImGuiBlueprintLibrary::GetColumnIndex()
{
	return ImGui::GetColumnIndex();
}

float UImGuiBlueprintLibrary::GetColumnWidth(int column_index)
{
	return ImGui::GetColumnWidth(column_index);
}

void UImGuiBlueprintLibrary::SetColumnWidth(int column_index, float width)
{
	ImGui::SetColumnWidth(column_index, width);
}

float UImGuiBlueprintLibrary::GetColumnOffset(int column_index)
{
	return ImGui::GetColumnOffset(column_index);
}

void UImGuiBlueprintLibrary::SetColumnOffset(int column_index, float offset_x)
{
	ImGui::SetColumnOffset(column_index, offset_x);
}

int UImGuiBlueprintLibrary::GetColumnsCount()
{
	return ImGui::GetColumnsCount();
}

void UImGuiBlueprintLibrary::LogToTTY(int max_depth)
{
	ImGui::LogToTTY(max_depth);
}

void UImGuiBlueprintLibrary::LogToFile(int max_depth, FString filename)
{
	ImGui::LogToFile(max_depth, TCHAR_TO_ANSI(*filename));
}

void UImGuiBlueprintLibrary::LogToClipboard(int max_depth)
{
	ImGui::LogToClipboard(max_depth);
}

void UImGuiBlueprintLibrary::LogFinish()
{
	ImGui::LogFinish();
}

void UImGuiBlueprintLibrary::LogButtons()
{
	ImGui::LogButtons();
}

void UImGuiBlueprintLibrary::LogText(FString fmt)
{
	ImGui::LogText(TCHAR_TO_ANSI(*fmt));
}

void UImGuiBlueprintLibrary::PushClipRect(FVector2D clip_rect_min, FVector2D clip_rect_max, bool intersect_with_current_clip_rect)
{
	ImGui::PushClipRect(FVEC2_TO_IMVEC2(clip_rect_min), FVEC2_TO_IMVEC2(clip_rect_max), intersect_with_current_clip_rect);
}

void UImGuiBlueprintLibrary::PopClipRect()
{
	ImGui::PopClipRect();
}

void UImGuiBlueprintLibrary::SetItemDefaultFocus()
{
	ImGui::SetItemDefaultFocus();
}

void UImGuiBlueprintLibrary::SetKeyboardFocusHere(int offset)
{
	ImGui::SetKeyboardFocusHere(offset);
}

bool UImGuiBlueprintLibrary::IsItemHovered(int32 flags)
{
	return ImGui::IsItemHovered(flags);
}

bool UImGuiBlueprintLibrary::IsItemActive()
{
	return ImGui::IsItemActive();
}

bool UImGuiBlueprintLibrary::IsItemFocused()
{
	return ImGui::IsItemFocused();
}

bool UImGuiBlueprintLibrary::IsItemClicked(int mouse_button)
{
	return ImGui::IsItemClicked(mouse_button);
}

bool UImGuiBlueprintLibrary::IsItemVisible()
{
	return ImGui::IsItemVisible();
}

bool UImGuiBlueprintLibrary::IsItemDeactivated()
{
	return ImGui::IsItemDeactivated();
}

bool UImGuiBlueprintLibrary::IsItemDeactivatedAfterChange()
{
	return ImGui::IsItemDeactivatedAfterChange();
}

bool UImGuiBlueprintLibrary::IsAnyItemHovered()
{
	return ImGui::IsAnyItemHovered();
}

bool UImGuiBlueprintLibrary::IsAnyItemActive()
{
	return ImGui::IsAnyItemActive();
}

bool UImGuiBlueprintLibrary::IsAnyItemFocused()
{
	return ImGui::IsAnyItemFocused();
}

FVector2D UImGuiBlueprintLibrary::GetItemRectMin()
{
	return IMVEC2_TO_FVEC2(ImGui::GetItemRectMin());
}

FVector2D UImGuiBlueprintLibrary::GetItemRectMax()
{
	return IMVEC2_TO_FVEC2(ImGui::GetItemRectMax());
}

FVector2D UImGuiBlueprintLibrary::GetItemRectSize()
{
	return IMVEC2_TO_FVEC2(ImGui::GetItemRectSize());
}

void UImGuiBlueprintLibrary::SetItemAllowOverlap()
{
	ImGui::SetItemAllowOverlap();
}

bool UImGuiBlueprintLibrary::IsRectVisible(FVector2D size)
{
	return ImGui::IsRectVisible(FVEC2_TO_IMVEC2(size));
}

bool UImGuiBlueprintLibrary::IsRectVisibleByCorners(FVector2D rect_min, FVector2D rect_max)
{
	return ImGui::IsRectVisible(FVEC2_TO_IMVEC2(rect_min), FVEC2_TO_IMVEC2(rect_max));
}

float UImGuiBlueprintLibrary::GetTime()
{
	return ImGui::GetTime();
}

int UImGuiBlueprintLibrary::GetFrameCount()
{
	return ImGui::GetFrameCount();
}

FString   UImGuiBlueprintLibrary::GetStyleColorName(EImGuiCol_ idx)
{
	return FString(ImGui::GetStyleColorName(idx));
}

FVector2D UImGuiBlueprintLibrary::CalcTextSize(FString text, FString text_end, bool hide_text_after_double_hash, float wrap_width)
{
	return IMVEC2_TO_FVEC2(ImGui::CalcTextSize(TCHAR_TO_ANSI(*text), TCHAR_TO_ANSI(*text_end), hide_text_after_double_hash, wrap_width));
}

void UImGuiBlueprintLibrary::CalcListClipping(int items_count, float items_height, int& out_items_display_start, int& out_items_display_end)
{
	ImGui::CalcListClipping(items_count, items_height, &out_items_display_start, &out_items_display_end);
}

bool UImGuiBlueprintLibrary::BeginChildFrame(int32 id, FVector2D size, int32 flags)
{
	return ImGui::BeginChildFrame(id, FVEC2_TO_IMVEC2(size), flags);
}

void UImGuiBlueprintLibrary::EndChildFrame()
{
	ImGui::EndChildFrame();
}

void UImGuiBlueprintLibrary::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}

void UImGuiBlueprintLibrary::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}

int UImGuiBlueprintLibrary::GetKeyIndex(EImGuiKey_ imgui_key)
{
	return ImGui::GetKeyIndex(imgui_key);
}

bool UImGuiBlueprintLibrary::IsKeyDown(int user_key_index)
{
	return ImGui::IsKeyDown(user_key_index);
}

bool UImGuiBlueprintLibrary::IsKeyPressed(int user_key_index, bool repeat)
{
	return ImGui::IsKeyPressed(user_key_index, repeat);
}

bool UImGuiBlueprintLibrary::IsKeyReleased(int user_key_index)
{
	return ImGui::IsKeyReleased(user_key_index);
}

int UImGuiBlueprintLibrary::GetKeyPressedAmount(int key_index, float repeat_delay, float rate)
{
	return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate);
}

bool UImGuiBlueprintLibrary::IsMouseDown(int button)
{
	return ImGui::IsMouseDown(button);
}

bool UImGuiBlueprintLibrary::IsAnyMouseDown()
{
	return ImGui::IsAnyMouseDown();
}

bool UImGuiBlueprintLibrary::IsMouseClicked(int button, bool repeat)
{
	return ImGui::IsMouseClicked(button, repeat);
}

bool UImGuiBlueprintLibrary::IsMouseDoubleClicked(int button)
{
	return ImGui::IsMouseDoubleClicked(button);
}

bool UImGuiBlueprintLibrary::IsMouseReleased(int button)
{
	return ImGui::IsMouseReleased(button);
}

bool UImGuiBlueprintLibrary::IsMouseDragging(int button, float lock_threshold)
{
	return ImGui::IsMouseDragging(button, lock_threshold);
}

bool UImGuiBlueprintLibrary::IsMouseHoveringRect(FVector2D r_min, FVector2D r_max, bool clip)
{
	return ImGui::IsMouseHoveringRect(FVEC2_TO_IMVEC2(r_min), FVEC2_TO_IMVEC2(r_max), clip);
}

bool UImGuiBlueprintLibrary::IsMousePosValid(const FVector2D& mouse_pos)
{
	ImVec2 mouse_pos_im = FVEC2_TO_IMVEC2(mouse_pos);
	return ImGui::IsMousePosValid(&mouse_pos_im);
}

FVector2D UImGuiBlueprintLibrary::GetMousePos()
{
	return IMVEC2_TO_FVEC2(ImGui::GetMousePos());
}

FVector2D UImGuiBlueprintLibrary::GetMousePosOnOpeningCurrentPopup()
{
	return IMVEC2_TO_FVEC2(ImGui::GetMousePosOnOpeningCurrentPopup());
}

FVector2D UImGuiBlueprintLibrary::GetMouseDragDelta(int button, float lock_threshold)
{
	return IMVEC2_TO_FVEC2(ImGui::GetMouseDragDelta(button, lock_threshold));
}

void UImGuiBlueprintLibrary::ResetMouseDragDelta(int button)
{
	ImGui::ResetMouseDragDelta(button);
}

EImGuiMouseCursor_ UImGuiBlueprintLibrary::GetMouseCursor()
{
	return (EImGuiMouseCursor_) ImGui::GetMouseCursor();
}

void UImGuiBlueprintLibrary::SetMouseCursor(EImGuiMouseCursor_ type)
{
	ImGui::SetMouseCursor(type);
}

void UImGuiBlueprintLibrary::CaptureKeyboardFromApp(bool capture)
{
	ImGui::CaptureKeyboardFromApp(capture);
}

void UImGuiBlueprintLibrary::CaptureMouseFromApp(bool capture)
{
	ImGui::CaptureMouseFromApp(capture);
}

FString   UImGuiBlueprintLibrary::GetClipboardText()
{
	return FString(ImGui::GetClipboardText());
}

void UImGuiBlueprintLibrary::SetClipboardText(FString text)
{
	ImGui::SetClipboardText(TCHAR_TO_ANSI(*text));
}