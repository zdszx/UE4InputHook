// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Framework/Application/SlateApplication.h"
#include "Misc/OutputDevice.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Windows/WindowsApplication.h"

#include "WindowsMessageHelpers.h"


#define LOCTEXT_NAMESPACE "FWindowsMessageHandlerExampleModule"


/**
 * Example Windows message handler.
 */
class FExampleHandler
	: public IWindowsMessageHandler
{
public:
	//~ IWindowsMessageHandler interface
	//the processmessage in WindowsApplication?  zds
	virtual bool ProcessMessage(HWND Hwnd, uint32 Message, WPARAM WParam, LPARAM LParam, int32& OutResult) override
	{
		void* zdsdllHandle;
		FString filePath = "C://Users//desenzha//source//UE//InputTest//ThirdParty//bin//Dll.dll";
		zdsdllHandle = FPlatformProcess::GetDllHandle(*filePath);
		// log out some details for the received message

        /*
		if (Message == 512 || Message == 32|| Message == 132) {
			return false;
		}*/


		//筛选print
		if (Message == 536 ||Message ==258) {
			FString mmsg = GetMessageName(Message);
			GLog->Logf(TEXT("Zeld_WindowsMessageHandlerModule: hwnd = %i, msg = %s, wParam = %i, lParam = %i"), Hwnd, *mmsg, WParam, LParam);
		}

		// we did not handle this message, so make sure it gets passed on to other handlers
		return false;
	}
};


/**
 * Implements the WindowsMessageHandlerExample module.
 */
class FWindowsMessageHandlerExampleModule
	: public IModuleInterface
{
public:

	//~ IModuleInterface interface

	virtual void StartupModule() override
	{
		// register our handler
		FWindowsApplication* Application = GetApplication();

		if (Application != nullptr)
		{
			Application->AddMessageHandler(Handler);
		}
	}


	virtual void ShutdownModule() override
	{
		// unregister our handler
		FWindowsApplication* Application = GetApplication();

		if (Application != nullptr)
		{
			Application->RemoveMessageHandler(Handler);
		}
	}

protected:

	FWindowsApplication* GetApplication() const
	{
		if (!FSlateApplication::IsInitialized())
		{
			return nullptr;
		}

		return (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
	}

private:

	FExampleHandler Handler;
};


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWindowsMessageHandlerExampleModule, WindowsMessageHandlerExample)