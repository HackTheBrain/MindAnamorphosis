#pragma once

#include "BciControllerInputState.h"
#include "IInputDevice.h"
#include <chrono>

//#include <Winsock2.h>
#include <vrpn_Button.h>
#include <vrpn_Analog.h>

#define MAX_NUM_BCI_INPUT_CONTROLLERS	4 // We dont realy have any input controllers, this is a sham! :P
#define NUM_BCI_INPUT_BUTTONS			6 // I've only used the one button but w/evs

/**
* Type definition for shared pointers to instances of FMessageEndpoint.
*/
// ToDo: Is this necessary?
typedef TSharedPtr<class FBciControllerInputDevice> FBciControllerInputDevicePtr;

/**
* Type definition for shared references to instances of FMessageEndpoint.
*/
// ToDo: Is this necessary?
typedef TSharedRef<class FBciControllerInputDevice> FBciControllerInputDeviceRef;

/**
* Interface class for WiiInput devices (wii devices)
*/
class FBciControllerInputDevice : public IInputDevice
{
public:
    FBciControllerInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& MessageHandler);

    /** Tick the interface (e.g. check for new controllers) */
    virtual void Tick(float DeltaTime) override;

    /** Poll for controller state and send events if needed */
    virtual void SendControllerEvents() override;

    /** Set which MessageHandler will get the events from SendControllerEvents. */
    virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

    /** Exec handler to allow console commands to be passed through for debugging */
    virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

    // IForceFeedbackSystem pass through functions
    virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
    virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;

    virtual ~FBciControllerInputDevice();

private:

    // VPRN Stuff
    //
    static void VRPN_CALLBACK vrpn_button_callback(void* user_data, vrpn_BUTTONCB button);
    static void VRPN_CALLBACK vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog);

    void vrpn_button_callback(vrpn_BUTTONCB button);
    void vrpn_analog_callback(vrpn_ANALOGCB analog);

    void InitVPRN();
    void ProcessVPRN();
    void CleanupVPRN();

    vrpn_Button_Remote* VRPNButton;
    vrpn_Analog_Remote* VRPNAnalog;

    float GetNormalizedAnalogueValue(vrpn_float64 value, int index);
    void CheckForDormantVRPN();

    vrpn_float64 maxAnalogue[2] = {0.0, 0.0};
    vrpn_float64 minAnalogue[2] = {0.0, 0.0};

    time_t lastUpdate;

    //
    // End VPRN Stuff    

    // ToDo: Is this necessary?
    bool Active;

    int fireEvent;
    int releaseEvent;

    /** Delay before sending a repeat message after a button was first pressed */
    float InitialButtonRepeatDelay; // How long a button is held for before you send a 2nd event

                                    /** Delay before sending a repeat message after a button has been pressed for a while */
    float ButtonRepeatDelay; // How long a button is held for before you send a 3rd/4th/e.t.c event

    FGamepadKeyNames::Type BciInputButtonMapping[NUM_BCI_INPUT_BUTTONS];

    /** Last frame's button states, so we only send events on edges */
    int PreviousButtonStates[NUM_BCI_INPUT_BUTTONS];

    /** Next time a repeat event should be generated for each button */
    double NextRepeatTime[NUM_BCI_INPUT_BUTTONS];

    TSharedRef< FGenericApplicationMessageHandler > MessageHandler;
    FBciControllerState BciControllerStates[MAX_NUM_BCI_INPUT_CONTROLLERS];
};