// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include <Core.h>


/** Delegate that allows to subscribe for ImGui events.  */
typedef FSimpleMulticastDelegate::FDelegate FImGuiDelegate;

/**
 * Handle to ImGui delegate. Contains additional information locating delegates in different contexts.
 */
class FImGuiDelegateHandle
{
public:

	FImGuiDelegateHandle() = default;

	bool IsValid() const
	{
		return Handle.IsValid();
	}

	void Reset()
	{
		Handle.Reset();
		Name = "";
	}

private:

	FImGuiDelegateHandle(const FDelegateHandle& InHandle, int32 InCategory, FString InName = "")
		: Handle(InHandle)
		, Category(InCategory)
		, Name(InName)
	{
	}

	friend bool operator==(const FImGuiDelegateHandle& Lhs, const FImGuiDelegateHandle& Rhs)
	{
		return Lhs.Handle == Rhs.Handle && Lhs.Category == Rhs.Category && Lhs.Name == Rhs.Name;
	}

	friend bool operator!=(const FImGuiDelegateHandle& Lhs, const FImGuiDelegateHandle& Rhs)
	{
		return !(Lhs == Rhs);
	}

	FDelegateHandle Handle;
	int32 Category = 0;
	FString Name;

	friend class FImGuiModule;
};
