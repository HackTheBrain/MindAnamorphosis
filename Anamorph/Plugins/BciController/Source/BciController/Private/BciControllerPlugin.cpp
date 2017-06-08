#include "BciControllerPrivatePCH.h"

#include "Internationalization.h" // LOCTEXT
#include "InputCoreTypes.h"

#include "BciControllerPlugin.h"

#include "Engine.h" // Are these both necessary?
//#include "EngineUserInterfaceClasses.h" // Are these both necessary?

#include "IBciControllerPlugin.h"

//#include "BciController.generated.inl"

IMPLEMENT_MODULE(FBciControllerPlugin, BciController)
DEFINE_LOG_CATEGORY_STATIC(BciControllerPlugin, Log, All);

#define LOCTEXT_NAMESPACE "InputKeys"


// This function is called by *Application.cpp after startup to instantiate the modules InputDevice
TSharedPtr< class IInputDevice > FBciControllerPlugin::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
    UE_LOG(BciControllerPlugin, Log, TEXT("Create Input Device"));

    FBciControllerPlugin::BciInputDevice = MakeShareable(new FBciControllerInputDevice(InMessageHandler));

    return FBciControllerPlugin::BciInputDevice;
}

#undef LOCTEXT_NAMESPACE

// This function may be called during shutdown to clean up the module.
void FBciControllerPlugin::ShutdownModule()
{
    FBciControllerPlugin::BciInputDevice->~FBciControllerInputDevice();

    UE_LOG(BciControllerPlugin, Log, TEXT("Shutdown Module"));
}