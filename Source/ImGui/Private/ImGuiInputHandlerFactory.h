// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

class FImGuiModuleManager;
class UImGuiInputHandler;

class FImGuiInputHandlerFactory
{
public:

	static UImGuiInputHandler* NewHandler(FImGuiModuleManager* ModuleManager, int32 ContextIndex);

	static void ReleaseHandler(UImGuiInputHandler* Handler);
};
