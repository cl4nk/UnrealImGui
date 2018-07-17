// Fill out your copyright notice in the Description page of Project Settings.

#include "ImGuiPrivatePCH.h"

#include "ImGui.h"
#include "ImGuiModuleManager.h"
#include "ImGuiWidget.h"

#define LOCTEXT_NAMESPACE "UMG"

void UImGuiWidget::SetAsCurrent() const
{
	if (MyImGuiWidget.IsValid())
	{
		if (FImGuiContextProxy * ContextProxy = MyImGuiWidget->GetContextProxy())
		{
			ContextProxy->SetAsCurrent();
		}
	}
}

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

		int32 ContextIndex = ImGuiModuleManager ? ImGuiModuleManager->GetContextIndex(GetWorld()) : 0;

		MyImGuiWidget = SNew(SImGuiWidget).
			ModuleManager(ImGuiModuleManager).
			ContextIndex(ContextIndex).
			IsFocusable(IsFocusable);
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
