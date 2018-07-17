// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiPrivatePCH.h"

#include "ImGuiModuleManager.h"

#include "ImGuiInteroperability.h"
#include "Utilities/WorldContextIndex.h"

#include <ModuleManager.h>

#include <imgui.h>


FImGuiModuleManager::FImGuiModuleManager()
{
	// Try to register tick delegate (it may fail if Slate application isn't yet ready).
	RegisterTick();

	// If we failed to register, create an initializer that will do it later.
	if (!IsTickRegistered())
	{
		CreateTickInitializer();
	}
}

FImGuiModuleManager::~FImGuiModuleManager()
{
	// Deactivate this manager.
	ReleaseTickInitializer();
	UnregisterTick();
}

void FImGuiModuleManager::LoadTextures()
{
	checkf(FSlateApplication::IsInitialized(), TEXT("Slate should be initialized before we can create textures."));

	if (!bTexturesLoaded)
	{
		bTexturesLoaded = true;

		// Create an empty texture at index 0. We will use it for ImGui outputs with null texture id.
		TextureManager.CreatePlainTexture(FName{ "ImGuiModule_Plain" }, 2, 2, FColor::White);

		// Create a font atlas texture.
		ImFontAtlas& Fonts = ContextManager.GetFontAtlas();

		unsigned char* Pixels;
		int Width, Height, Bpp;
		Fonts.GetTexDataAsRGBA32(&Pixels, &Width, &Height, &Bpp);

		TextureIndex FontsTexureIndex = TextureManager.CreateTexture(FName{ "ImGuiModule_FontAtlas" }, Width, Height, Bpp, Pixels, false);

		// Set font texture index in ImGui.
		Fonts.TexID = ImGuiInterops::ToImTextureID(FontsTexureIndex);
	}
}

void FImGuiModuleManager::RegisterTick()
{
	// Slate Post-Tick is a good moment to end and advance ImGui frame as it minimises a tearing.
	if (!TickDelegateHandle.IsValid() && FSlateApplication::IsInitialized())
	{
		TickDelegateHandle = FSlateApplication::Get().OnPostTick().AddRaw(this, &FImGuiModuleManager::Tick);
	}
}

void FImGuiModuleManager::UnregisterTick()
{
	if (TickDelegateHandle.IsValid())
	{
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().OnPostTick().Remove(TickDelegateHandle);
		}
		TickDelegateHandle.Reset();
	}
}

void FImGuiModuleManager::CreateTickInitializer()
{
	if (!TickInitializerHandle.IsValid())
	{
		// Try to register tick delegate until we finally succeed.

		TickInitializerHandle = FModuleManager::Get().OnModulesChanged().AddLambda([this](FName Name, EModuleChangeReason Reason)
		{
			if (Reason == EModuleChangeReason::ModuleLoaded)
			{
				RegisterTick();
			}

			if (IsTickRegistered())
			{
				ReleaseTickInitializer();
			}
		});
	}
}

void FImGuiModuleManager::ReleaseTickInitializer()
{
	if (TickInitializerHandle.IsValid())
	{
		FModuleManager::Get().OnModulesChanged().Remove(TickInitializerHandle);
		TickInitializerHandle.Reset();
	}
}

bool FImGuiModuleManager::IsInUpdateThread()
{
	// We can get ticks from the Game thread and Slate loading thread. In both cases IsInGameThread() is true, so we
	// need to make additional test to filter out loading thread.
	return IsInGameThread() && !IsInSlateThread();
}

void FImGuiModuleManager::Tick(float DeltaSeconds)
{
	if (IsInUpdateThread())
	{
		// Update context manager to advance all ImGui contexts to the next frame.
		ContextManager.Tick(DeltaSeconds);

		// Inform that we finished updating ImGui, so other subsystems can react.
		PostImGuiUpdateEvent.Broadcast();
	}
}

int32 FImGuiModuleManager::GetContextIndex(UWorld * World)
{
	// Create and initialize the widget.

	checkf(FSlateApplication::IsInitialized(), TEXT("Slate should be initialized before we can add widget to game viewports."));

	// Make sure that we have a context for this viewport's world and get its index.
	int32 ContextIndex;
	auto& Proxy = ContextManager.GetWorldContextProxy(World, ContextIndex);

	// Make sure that textures are loaded before the first Slate widget is created.
	LoadTextures();

	return ContextIndex;
}