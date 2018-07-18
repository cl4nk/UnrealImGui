// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiContextProxy.h"
#include "ImGuiDemo.h"


// Manages ImGui context proxies.
class FImGuiContextManager
{
public:

	FImGuiContextManager();

	FImGuiContextManager(const FImGuiContextManager&) = delete;
	FImGuiContextManager& operator=(const FImGuiContextManager&) = delete;

	FImGuiContextManager(FImGuiContextManager&&) = delete;
	FImGuiContextManager& operator=(FImGuiContextManager&&) = delete;

	~FImGuiContextManager();

	ImFontAtlas& GetFontAtlas() { return FontAtlas; }
	const ImFontAtlas& GetFontAtlas() const { return FontAtlas; }


#if WITH_EDITOR
	// Get or create editor ImGui context proxy.
	FORCEINLINE FImGuiContextProxy* GetEditorContextProxy();
#endif

	// Get or create ImGui context proxy for given world. Additionally get context index for that proxy.
	FORCEINLINE FImGuiContextProxy* GetWorldContextProxy(const UWorld* World, const FName& ContextName);

	// Get context proxy by index, or null if context with that index doesn't exist.
	FORCEINLINE FImGuiContextProxy* GetContextProxy(const FName& ContextName)
	{
		return Contexts.Find(ContextName);
	}

	// Delegate called for all contexts in manager, right after calling context specific draw event. Allows listeners
	// draw the same content to multiple contexts.
	FSimpleMulticastDelegate& OnDrawMultiContext() { return DrawMultiContextEvent; }

	void Tick(float DeltaSeconds);

private:

	TMap<FName, FImGuiContextProxy> Contexts;

	FImGuiDemo ImGuiDemo;

	FSimpleMulticastDelegate DrawMultiContextEvent;

	ImFontAtlas FontAtlas;
};
