#pragma once

#include "CoreMinimal.h"
#include "ImGuiParser.generated.h"

UCLASS()
class IMGUI_API UImGuiParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (Category = "ImGui|Parser"))
	static void ParseObject(UObject * Object, bool IncludeSuper = false);

	static void ParseStruct(UStruct * Struct, void * StructValue, bool IncludeSuper = false);

	static void ParseBoolProperty(UBoolProperty * BoolProperty, void * PropertyValue);
	static void ParseArrayProperty(UArrayProperty * ArrayProperty, void * PropertyValue);
	static void ParseMapProperty(UMapProperty * MapProperty, void * PropertyValue);
	static void ParseStructProperty(UStructProperty * StructProperty, void * PropertyValue);

	static void ParseProperty(UProperty * Property, void * PropertyValue);
};