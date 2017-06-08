#pragma once
#include "BciControllerPrivatePCH.h"
 
class FBciControllerPlugin : public IBciControllerPlugin
{
public:
	/** IBciControllerInterface implementation */
	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
 
	//virtual void StartupModule() OVERRIDE; // This is not required as IInputDeviceModule handels it!
	virtual void ShutdownModule() override;
 
	TSharedPtr< class FBciControllerInputDevice > BciInputDevice;
};