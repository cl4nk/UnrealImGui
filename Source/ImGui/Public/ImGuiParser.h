#pragma once

#include "CoreMinimal.h"
#include "ImGuiParser.generated.h"

UCLASS()
class IMGUI_API UImGuiParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void ParseObject(UObject * Object, bool IncludeSuper = false);
	static void ParseStruct(UStruct * Struct, void * StructValue, bool IncludeSuper = false);
	static void ParseProperty(UProperty * Property, void * PropertyValue);

};