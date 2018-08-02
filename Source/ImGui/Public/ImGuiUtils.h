#pragma once

#include <imgui.h>

#define TEXT_BOOL(Val) ((Val) ? TEXT("true") : TEXT("false"))

namespace ImGuiUtils
{
	// Column layout utilities.
	namespace Columns
	{
		template<typename FunctorType>
		static void TwoColumnsTab(int Columns, FunctorType&& DrawContent)
		{
			const int LastColumns = ImGui::GetColumnsCount();
			ImGui::Columns(Columns, nullptr, false);
			DrawContent();
			ImGui::Columns(LastColumns);	
		}

		template<typename FunctorType>
		static void CollapsingTwoColumnsTab(const char* Name, int Columns, FunctorType&& DrawContent)
		{
			if (ImGui::CollapsingHeader(Name, ImGuiTreeNodeFlags_DefaultOpen))
			{
				TwoColumnsTab(Columns, std::forward<FunctorType>(DrawContent));
			}
		}
	}

	// Controls tweaked for 2-columns layout.
	namespace TwoColumns
	{
		template<typename FunctorType>
		static inline void CollapsingGroup(const char* Name, FunctorType&& DrawContent)
		{
			Columns::CollapsingTwoColumnsTab(Name, 2, std::forward<FunctorType>(DrawContent));
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
}
