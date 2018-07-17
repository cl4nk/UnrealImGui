// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SImGuiWidget.h"
#include "ImGuiWidget.generated.h"

/**
 * 
 */


UCLASS()
class UImGuiWidget : public UWidget
{
	GENERATED_BODY()
	
public:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
	
	TSharedPtr< SImGuiWidget > MyImGuiWidget;
};
