// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGuiDelegates.h"

#include <ModuleManager.h>

class FImGuiModuleManager;

class FImGuiModule : public IModuleInterface
{
public:

	/**
	 * Singleton-like access to this module's interface. This is just for convenience!
	 * Beware of calling this during the shutdown phase, though. Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FImGuiModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FImGuiModule>("ImGui");
	}

	/**
	 * Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ImGui");
	}


	/**
	 * Add shared delegate called for each ImGui context at the end of debug frame, after calling context specific
	 * delegate. This delegate will be used for any ImGui context, created before or after it is registered.
	 *
	 * @param Delegate - Delegate that we want to add (@see FImGuiDelegate::Create...)
	 * @returns Returns handle that can be used to remove delegate (@see RemoveImGuiDelegate)
	 */
	virtual FImGuiDelegateHandle AddMultiContextImGuiDelegate(const FImGuiDelegate& Delegate);

	/**
	 * Remove delegate added with any version of Add...ImGuiDelegate
	 *
	 * @param Handle - Delegate handle that was returned by adding function
	 */
	virtual void RemoveImGuiDelegate(const FImGuiDelegateHandle& Handle);

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	FImGuiModuleManager* ImGuiModuleManager = nullptr;

public:
	FORCEINLINE FImGuiModuleManager* GetImGuiModuleManager() const { return ImGuiModuleManager; }

};
