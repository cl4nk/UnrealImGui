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

void UImGuiWidget::SetContextName(FName InContextName)
{
	ContextName = InContextName;
	if (MyImGuiWidget.IsValid())
	{
		//Module should be loaded, the check is inside the get
		FImGuiModule& ImGuiModule = FImGuiModule::Get();

		FImGuiModuleManager* ImGuiModuleManager = ImGuiModule.GetImGuiModuleManager();

		MyImGuiWidget->SetContextProxy(ImGuiModuleManager ? ImGuiModuleManager->GetContextProxy(GetWorld(), ContextName) : nullptr);
	}
}

void UImGuiWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImGuiWidget.Reset();
}

TSharedRef<SWidget> UImGuiWidget::RebuildWidget()
{
	//Module should be loaded, the check is inside the get
	FImGuiModule& ImGuiModule = FImGuiModule::Get();

	FImGuiModuleManager* ImGuiModuleManager = ImGuiModule.GetImGuiModuleManager();

	MyImGuiWidget = SNew(SImGuiWidget).
		IsFocusable(IsFocusable).
		ContextProxy(ImGuiModuleManager ? ImGuiModuleManager->GetContextProxy(GetWorld(), ContextName) : nullptr);
	
	return MyImGuiWidget.ToSharedRef();
}

#if WITH_EDITOR

const FText UImGuiWidget::GetPaletteCategory()
{
	return LOCTEXT("Misc", "Misc");
}

#endif

#undef LOCTEXT_NAMESPACE
