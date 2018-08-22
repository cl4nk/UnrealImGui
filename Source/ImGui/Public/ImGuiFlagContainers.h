#pragma once

#include "CoreMinimal.h"
#include "ImGuiBlueprintLibrary.h"
#include "ImGuiFlagContainers.generated.h"

USTRUCT(BlueprintType)
struct FImGuiWindowParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiWindowFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiInputTextParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiInputTextFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiTreeNodeParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiTreeNodeFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiSelectableParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiSelectableFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiComboParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiComboFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiFocusedParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiFocusedFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiHoveredParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiHoveredFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiDragDropParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiDragDropFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiConfigParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiConfigFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiBackendParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiBackendFlags_))
	int32 Flags;
};

USTRUCT(BlueprintType)
struct FImGuiColorEditParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EImGuiColorEditFlags_))
	int32 Flags;
};