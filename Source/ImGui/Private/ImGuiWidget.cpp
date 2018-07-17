// Fill out your copyright notice in the Description page of Project Settings.

#include "ImGuiPrivatePCH.h"

#include "ImGui.h"
#include "ImGuiModuleManager.h"
#include "ImGuiWidget.h"

#define LOCTEXT_NAMESPACE "UMG"

void UImGuiWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImGuiWidget.Reset();

}
TSharedRef<SWidget> UImGuiWidget::RebuildWidget()
{
	if (FImGuiModule::IsAvailable())
	{
		FImGuiModule& ImGuiModule = FImGuiModule::Get();

		FImGuiModuleManager* ImGuiModuleManager = ImGuiModule.GetImGuiModuleManager();

		if (ImGuiModuleManager)
		{
			MyImGuiWidget = ImGuiModuleManager->Create(GetWorld());
		}
	}
	
	return MyImGuiWidget.ToSharedRef();
}


#if WITH_EDITOR

const FText UImGuiWidget::GetPaletteCategory()
{
	return LOCTEXT("Misc", "Misc");
}

#endif

#undef LOCTEXT_NAMESPACE
