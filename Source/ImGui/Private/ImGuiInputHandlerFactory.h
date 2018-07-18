// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

class SImGuiWidget;
class UImGuiInputHandler;

class FImGuiInputHandlerFactory
{
public:

	static UImGuiInputHandler* NewHandler(SImGuiWidget * Widget);

	static void ReleaseHandler(UImGuiInputHandler* Handler);
};
