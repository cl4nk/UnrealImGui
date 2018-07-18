// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiPrivatePCH.h"

#include "ImGuiContextManager.h"

#include "ImGuiImplementation.h"
#include "Utilities/ScopeGuards.h"
#include "Utilities/WorldContext.h"
#include "Utilities/WorldContextIndex.h"

#include <imgui.h>


namespace CVars
{
	TAutoConsoleVariable<int> DebugDrawOnWorldTick(TEXT("ImGui.DebugDrawOnWorldTick"), 1,
		TEXT("If this is enabled then all the ImGui debug draw events will be called during World Tick Start.\n")
		TEXT("This has an advantage that all the global variables like GWorld are set to correct values.\n")
		TEXT("A disadvantage is that objects are not yet updated. That can be changed by disabling this feature,")
		TEXT("but preferable solution is to call debug code from object's own Tick function.\n")
		TEXT("NOTE: Order of multi-context and world draw events depends on this value and is arranged in a way ")
		TEXT("that world draw events and objects updates are closer together.\n")
		TEXT("0: disabled, ImGui Debug Draw is called during Post-Tick\n")
		TEXT("1: enabled (default), ImGui Debug Draw is called during World Tick Start"),
		ECVF_Default);
}

namespace
{
#if WITH_EDITOR

	// Name for editor ImGui context.
	FORCEINLINE FString GetEditorContextName()
	{
		return TEXT("Editor");
	}

	// Name for world ImGui context.
	FORCEINLINE FString GetWorldContextName(const UWorld* World)
	{
		using namespace Utilities;

		const FWorldContext* WorldContext = GetWorldContext(World);
		if (WorldContext)
		{
			switch (WorldContext->WorldType)
			{
			case EWorldType::PIE:
				return FString::Printf(TEXT("PIEContext%d"), GetWorldContextIndex(*WorldContext));
			case EWorldType::Game:
				return TEXT("Game");
			case EWorldType::Editor:
				return TEXT("Editor");
			default:
				return TEXT("Other");
			}
		}
		return TEXT("Null");
	}

#else

	FORCEINLINE FString GetWorldContextName()
	{
		return TEXT("Game");
	}

	FORCEINLINE FString GetWorldContextName(const UWorld&)
	{
		return TEXT("Game");
	}

#endif // WITH_EDITOR
}

FImGuiContextManager::FImGuiContextManager()
{
	unsigned char* Pixels;
	int Width, Height, Bpp;
	FontAtlas.GetTexDataAsRGBA32(&Pixels, &Width, &Height, &Bpp);
}

FImGuiContextManager::~FImGuiContextManager()
{
	// Order matters because contexts can be created during World Tick Start events.
	FWorldDelegates::OnWorldTickStart.RemoveAll(this);
}

void FImGuiContextManager::Tick(float DeltaSeconds)
{
	// In editor, worlds can get invalid. We could remove corresponding entries, but that would mean resetting ImGui
	// context every time when PIE session is restarted. Instead we freeze contexts until their worlds are re-created.

	for (auto& Pair : Contexts)
	{
		auto& ContextProxy = Pair.Value;
		ContextProxy.Tick(DeltaSeconds);
	}
}

#if WITH_EDITOR
FImGuiContextProxy* FImGuiContextManager::GetEditorContextProxy()
{
	FImGuiContextProxy* Data = Contexts.Find("");

	if (UNLIKELY(!Data))
	{
		Data = &Contexts.Emplace("", FImGuiContextProxy("", &DrawMultiContextEvent, &FontAtlas));
	}

	return Data;
}
#endif // WITH_EDITOR

FImGuiContextProxy* FImGuiContextManager::GetWorldContextProxy(const UWorld* World, const FName & Key)
{
	using namespace Utilities;

#if WITH_EDITOR
	if (World == nullptr || World->WorldType == EWorldType::Editor || World->WorldType == EWorldType::EditorPreview)
	{
		return GetEditorContextProxy();
	}
#endif

	FImGuiContextProxy* Data = Contexts.Find(Key);

	if (UNLIKELY(!Data))
	{
		Data = &Contexts.Emplace(Key, FImGuiContextProxy(Key.ToString(), &DrawMultiContextEvent, &FontAtlas));
	}

	return Data;
}
