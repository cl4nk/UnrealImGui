
#include "ImGuiPrivatePCH.h"

#include "ImGuiParser.h"

#include "ImGuiUtils.h"
#include "ImGuiParser.h"
#include <imgui.h>

void UImGuiParser::ParseObject(UObject * Object, bool IncludeSuper)
{
	ParseStruct(Object->GetClass(), Object, IncludeSuper);
}

void UImGuiParser::ParseStruct(UStruct * Struct, void * StructValue, bool IncludeSuper)
{
	static FString StringID;

	EFieldIteratorFlags::SuperClassFlags SuperClassFlags = IncludeSuper ? EFieldIteratorFlags::IncludeSuper : EFieldIteratorFlags::ExcludeSuper;
	TFieldIterator<UProperty> PropertyIt(Struct, SuperClassFlags);

	// Test outside the ImGuiUtils call to prevent a blank space due to the empty tab
	if (PropertyIt)
	{
		StringID.Append(Struct->GetName());

		ImGuiUtils::Columns::TwoColumnsTab(TCHAR_TO_ANSI(*StringID), 2, [&]()
		{
			for (; PropertyIt; ++PropertyIt)
			{
				UProperty* Property = *PropertyIt;
				const FString & PropertyName = Property->GetName();

				for (int32 Index = 0; Index < PropertyIt->ArrayDim; Index++)
				{
					ImGui::Text("%ls:", *PropertyName); ImGui::NextColumn();

					void * PropertyValue = Property->ContainerPtrToValuePtr<void>(StructValue, Index);
					ParseProperty(Property, PropertyValue); ImGui::NextColumn();
				}
			}
		});

		StringID.RemoveFromEnd(Struct->GetName());
	}
}

void UImGuiParser::ParseProperty(UProperty * Property, void * PropertyValue)
{
	if (UBoolProperty * BoolProperty = Cast<UBoolProperty>(Property))
	{
		bool Result = BoolProperty->GetPropertyValue(PropertyValue);
		if (ImGui::Checkbox("", &Result))
		{
			BoolProperty->SetPropertyValue(PropertyValue, Result);
		}
	}
	else if (UArrayProperty * ArrayProperty = Cast<UArrayProperty>(Property))
	{
		FScriptArrayHelper ArrayHelper(ArrayProperty, PropertyValue);
		FString CollapsingHeaderTitle = FString::Printf(TEXT("Array, Size = %d"), ArrayHelper.Num());

		if (ImGui::CollapsingHeader(TCHAR_TO_ANSI(*CollapsingHeaderTitle), ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (int32 i = 0; i < ArrayHelper.Num(); i++)
			{
				uint8* PropData = ArrayHelper.GetRawPtr(i);
				ParseProperty(ArrayProperty->Inner, PropData);
			}
		}
	}
	else if (UMapProperty * MapProperty = Cast<UMapProperty>(Property))
	{
		FScriptMapHelper MapHelper(MapProperty, PropertyValue);

		uint8* PropData = MapHelper.GetPairPtr(0); //Maybe prefer without check

		int32 Index = 0;
		FString CollapsingHeaderTitle = FString::Printf(TEXT("Map, Size = %d"), MapHelper.Num());

		if (ImGui::CollapsingHeader(TCHAR_TO_ANSI(*CollapsingHeaderTitle), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiUtils::Columns::TwoColumnsTab(TCHAR_TO_ANSI(*MapProperty->GetFullName()), 2, [&]()
			{
				for (int32 Count = MapHelper.Num(); Count; PropData += MapProperty->MapLayout.SetLayout.Size, ++Index)
				{
					if (MapHelper.IsValidIndex(Index))
					{
						FString ValueString;
						ParseProperty(MapProperty->KeyProp, PropData);
						ImGui::NextColumn();
						ParseProperty(MapProperty->ValueProp, PropData + MapProperty->MapLayout.ValueOffset);
						ImGui::NextColumn();

						--Count;
					}
				}
			});
		}
	}
	else if (UStructProperty * StructProperty = Cast<UStructProperty>(Property))
	{
		if (StructProperty->Struct->GetFName() == "Color")
		{
			FColor * ColorPtr = (FColor *)PropertyValue;
			FLinearColor LinearColor = ColorPtr->ReinterpretAsLinear();

			if (ImGui::ColorEdit4("Color", (float*) &LinearColor))
			{
				FColor NewColor = LinearColor.ToFColor(false);

				ColorPtr->R = NewColor.R;
				ColorPtr->G = NewColor.G;
				ColorPtr->B = NewColor.B;
				ColorPtr->A = NewColor.A;

				// Maybe set value, dunno
			}
		}
		else if (StructProperty->Struct->GetFName() == "LinearColor")
		{
			FLinearColor * LinearColorPtr = (FLinearColor *)PropertyValue;
			ImGui::ColorEdit4("LinearColor", (float*)LinearColorPtr);
		}
		else if (StructProperty->Struct->GetFName() == "Vector")
		{
			FVector * VectorPtr = (FVector *)PropertyValue;
			// X Y and Z should be aligned
			ImGui::InputFloat3("Vector", (float*)VectorPtr);
		}
		else if (StructProperty->Struct->GetFName() == "Rotator")
		{
			FRotator * RotatorPtr = (FRotator *)PropertyValue;
			// Pitch Yaw and Roll should be aligned
			ImGui::InputFloat3("Rotator", (float*)RotatorPtr);
		}
		else if (StructProperty->Struct->GetFName() == "Quat")
		{
			FQuat * QuatPtr = (FQuat *)PropertyValue;
			// X Y Z and W should be aligned
			ImGui::InputFloat4("Quat", (float*)QuatPtr);
		}
		else
		{
			if (ImGui::CollapsingHeader(TCHAR_TO_ANSI(*StructProperty->Struct->GetFName().ToString()), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ParseStruct(StructProperty->Struct, PropertyValue, true);
			}
		}
	}
	else
	{
		FString ValueString;
		Property->ExportTextItem(ValueString, PropertyValue, nullptr, nullptr, 0);

		ImGui::Text("%ls", *ValueString);
	}
}
