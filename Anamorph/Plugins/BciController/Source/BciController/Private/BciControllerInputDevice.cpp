#include "BciControllerPrivatePCH.h"
 
#include "GenericPlatformMath.h"
 
#include "BciControllerInputDevice.h"
 
//#include "SlateBasics.h"
 
//#include "WindowsApplication.h"
//#include "WindowsWindow.h"
//#include "WindowsCursor.h"
#include "GenericApplicationMessageHandler.h"
#include "Internationalization.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"
 
DEFINE_LOG_CATEGORY_STATIC(LogBciControllerDevice, Warning, All);

#define LOCTEXT_NAMESPACE "BciController"
#define NO_EVENT_IDX -1

const FKey FBciControllerKey::Bci_WeighingSensor1("Bci_WeighingSensor1");
 
const FBciControllerKeyNames::Type FBciControllerKeyNames::Bci_WeighingSensor1("Bci_WeighingSensor1");
 
FBciControllerInputDevice::FBciControllerInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) 
:   fireEvent(NO_EVENT_IDX), 
    releaseEvent(NO_EVENT_IDX),
    Active(true), 
    MessageHandler(InMessageHandler),
    VRPNButton(nullptr),
    VRPNAnalog(nullptr),
    maxAnalogue(0.0f),
    minAnalogue(0.0f),
    lastUpdate(0)
{
	UE_LOG(LogBciControllerDevice, Log, TEXT("Starting BciControllerInputDevice"));
 
	// Initialize button repeat delays
	InitialButtonRepeatDelay = 0.2f;
	ButtonRepeatDelay = 0.1f;
 
	// Register the FKeys (Gamepad key for controllers, Mouse for mice, FloatAxis for non binary values e.t.c.)
	EKeys::AddKey(FKeyDetails(FBciControllerKey::Bci_WeighingSensor1, LOCTEXT("Bci_WeighingSensor1", "Bci Weighing Sensor #1"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
 
	// Setting all buttons on my pseudo controllers to 'not pressed' 
	// (You should check the inital state of your controllers)
	PreviousButtonStates[NUM_BCI_INPUT_BUTTONS] = { 0 };
	//NextRepeatTime[NUM_BCI_INPUT_BUTTONS] = { 0.0 };
 
	// Initialize mapping of controller button mask to unreal button mask
	// - mapping 0-3 to the 'FaceButtons'
	// --- 'X','Square','Circle','Triangle' for playstation
	// --- 'A','X','B','Y' on xbox
	BciInputButtonMapping[2] = FGamepadKeyNames::FaceButtonTop;		// BCI_BUTTON_ZERO
	BciInputButtonMapping[3] = FGamepadKeyNames::FaceButtonBottom;	// BCI_BUTTON_ONE
	BciInputButtonMapping[4] = FGamepadKeyNames::FaceButtonLeft;	// BCI_BUTTON_TWO
	BciInputButtonMapping[5] = FGamepadKeyNames::FaceButtonRight;	// BCI_BUTTON_THREE
	// - mapping 4-5 to the left joystick (traditionally used for movement controls)
	BciInputButtonMapping[0] = FGamepadKeyNames::LeftAnalogX;		// BCI_BUTTON_FOUR
	BciInputButtonMapping[1] = FGamepadKeyNames::LeftAnalogY;		// BCI_BUTTON_FIVE

    InitVPRN();
}
 
void FBciControllerInputDevice::Tick(float DeltaTime) {
	// This will spam the log heavily, comment it out for real plugins :)
	UE_LOG(LogBciControllerDevice, Log, TEXT("Tick %f"), DeltaTime);
}
 
void FBciControllerInputDevice::SendControllerEvents() {

    ProcessVPRN();

	// Here is where we check the state of our input device proberbly by calling a method in your third party libary...
	//  - I dont have a real device (xbox controller, wiimote, e.t.c.) in this tutorial :'( so we will just pretend!!!
 
	// I could make libary to read from a fancy set of 'matrix' cerebellum jacks and iterate over each of those 'controllers'.. but ill save that for the next tutorial
	int controllerIndex = 0; // Apparantly I was lazy so there is only one controller firing events today!

    if (fireEvent > NO_EVENT_IDX)
    {
        MessageHandler->OnControllerButtonPressed(BciInputButtonMapping[fireEvent], controllerIndex, false);
        fireEvent = NO_EVENT_IDX;
    }

    if (releaseEvent > NO_EVENT_IDX)
    {
        MessageHandler->OnControllerButtonReleased(BciInputButtonMapping[releaseEvent], controllerIndex, false);
        releaseEvent = NO_EVENT_IDX;
    }
    /*
	const double CurrentTime = FPlatformTime::Seconds(); 
	const float CurrentTimeFloat = FPlatformTime::ToSeconds(FPlatformTime::Cycles()); // Works with FMath functions
 
	// This weard statement simulates user holding down the button 1/3 of the time
	if (FMath::Fmod(CurrentTimeFloat, 1.5f) < .5f) {
		// If the button was pressed this tick
		if (!PreviousButtonStates[1]) {
			// Fire button pressed event
			MessageHandler->OnControllerButtonPressed(BciInputButtonMapping[1], controllerIndex, false);
 
			// this button was pressed - set the button's NextRepeatTime to the InitialButtonRepeatDelay
			NextRepeatTime[1] = CurrentTime + InitialButtonRepeatDelay;
			PreviousButtonStates[1] = true;
		// If the buttons has been held long enough to fire a nth event
		} else if (NextRepeatTime[1] <= CurrentTime) {
			// Fire button held event
			MessageHandler->OnControllerButtonPressed(BciInputButtonMapping[1], controllerIndex, true);
 
			// set the button's NextRepeatTime to the ButtonRepeatDelay
			NextRepeatTime[1] = CurrentTime + ButtonRepeatDelay;
		}
	// If the button was released this tick
	}
	else if (PreviousButtonStates[1]) {
		UE_LOG(LogBciControllerDevice, Log, TEXT("Release"));
		// Fire button released event
		MessageHandler->OnControllerButtonReleased(BciInputButtonMapping[1], controllerIndex, false);
		PreviousButtonStates[1] = false;
	}
 
	// This simulates the user moving the stick left and right repeatedly
	float xMove = FMath::Sin(CurrentTimeFloat * .223f * 2 * PI);
	// Fire analog input event
	MessageHandler->OnControllerAnalog(BciInputButtonMapping[4], controllerIndex, xMove);
 
	// This simulates the user moving the stick up and down repeatedly
	float yMove = FMath::Cos(CurrentTimeFloat * .278f * 2 * PI);
	// Fire analog input event
	MessageHandler->OnControllerAnalog(BciInputButtonMapping[5], controllerIndex, yMove);
 
	// This will spam the log heavily, comment it out for real plugins :)
	UE_LOG(LogBciControllerDevice, Log, TEXT("Sending Controller Events jump%d, x%f, y%f"), PreviousButtonStates[1], xMove, yMove);
 
	// This is how you fire your fancypantz new controller events... the ones you added because you couldn't find an existing EControllerButton that matched your needs!
 
	// We 'read' the value 75 from the weight sensor
	const unsigned short int sensorValue = 75;
 
	FBciControllerSensorState Sensor1 = BciControllerStates[controllerIndex].WeightAxes[(int32)EBciControllerAxes::WeightSensor1];
	if(sensorValue != Sensor1.State)
	{
		Sensor1.State = sensorValue;
 
		FSlateApplication::Get().OnControllerAnalog(Sensor1.Axis, controllerIndex, Sensor1.GetValue()); // This will spam the calculated weight to my fancy new output type!
	}*/
}
 
void FBciControllerInputDevice::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) {
	UE_LOG(LogBciControllerDevice, Log, TEXT("Set Message Handler"));
	MessageHandler = InMessageHandler;
}
 
bool FBciControllerInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) {
	UE_LOG(LogBciControllerDevice, Log, TEXT("Execute Console Command: %s"), Cmd);
 
	// Put your fancy custom console command code here... 
	// ToDo: use this to let you fire pseudo controller events
 
	return true;
}
 
// IForceFeedbackSystem pass through functions
//  - I *believe* this is a handel for the game to communicate back to your third party libary (i.e. game tells joystick to increase force feedback/vibrate/turn on/off a light)
void FBciControllerInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) {
	UE_LOG(LogBciControllerDevice, Log, TEXT("Set Force Feedback %f"), Value);
}
void FBciControllerInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) {
	// This will spam the log heavily, comment it out for real plugins :)
	UE_LOG(LogBciControllerDevice, Log, TEXT("Set Force Feedback Values"));
}
 
// This is where you nicely clean up your plugin when its told to shut down!
//  - USE THIS PLEASE!!! no one likes memory leaks >_<
FBciControllerInputDevice::~FBciControllerInputDevice() {
    CleanupVPRN();

	UE_LOG(LogBciControllerDevice, Log, TEXT("Closing BciControllerInputDevice"));
}

// VPRN Stuff
// 
void VRPN_CALLBACK FBciControllerInputDevice::vrpn_button_callback(void* user_data, vrpn_BUTTONCB button)
{
    if (user_data != nullptr)
    {
        FBciControllerInputDevice* localThis = reinterpret_cast<FBciControllerInputDevice*>(user_data);
        localThis->vrpn_button_callback(button);
    }
}

void VRPN_CALLBACK FBciControllerInputDevice::vrpn_analog_callback(void* user_data, vrpn_ANALOGCB analog)
{
    if (user_data != nullptr)
    {
        FBciControllerInputDevice* localThis = reinterpret_cast<FBciControllerInputDevice*>(user_data);
        localThis->vrpn_analog_callback(analog);
    }
}

void FBciControllerInputDevice::vrpn_button_callback(vrpn_BUTTONCB button)
{
    UE_LOG(LogBciControllerDevice, Log, TEXT("Received Button Callback; ID = %d, State = %d"), button.button, button.state);

    if(button.button <= NUM_BCI_INPUT_BUTTONS)
    {
        int buttonIdx = button.button;

        //if(PreviousButtonStates[buttonIdx] != button.state)
        //{
            if (button.state == 1)
            {
                MessageHandler->OnControllerButtonPressed(BciInputButtonMapping[buttonIdx], 0, false);
            }
            else
            {
                MessageHandler->OnControllerButtonReleased(BciInputButtonMapping[buttonIdx], 0, false);
            }

            PreviousButtonStates[buttonIdx] = button.state;
        //}
    }
    //if (button.button == 1)
    //{
    //    *(bool*)user_data = false;
    //}
}

void FBciControllerInputDevice::vrpn_analog_callback(vrpn_ANALOGCB analog)
{
    for (int i = 0; i < analog.num_channel; i++)
    {
        UE_LOG(LogBciControllerDevice, Log, TEXT("Received Analog Callback; Channel = %d, Value = %d"), i, analog.channel[i]);

        if(i == 0)
        {
            float xMove = GetNormalizedAnalogueValue(analog.channel[i]);
            MessageHandler->OnControllerAnalog(BciInputButtonMapping[0], 0, xMove);
        }
    }

    lastUpdate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void FBciControllerInputDevice::InitVPRN()
{
    VRPNButton = new vrpn_Button_Remote("openvibe_vrpn_button@localhost");
    VRPNButton->register_change_handler(this, vrpn_button_callback);

    VRPNAnalog = new vrpn_Analog_Remote("openvibe_vrpn_analog@localhost");
    VRPNAnalog->register_change_handler(this, vrpn_analog_callback);
}

void FBciControllerInputDevice::ProcessVPRN()
{
    VRPNButton->mainloop();
    VRPNAnalog->mainloop();

    CheckForDormantVRPN();
}

void FBciControllerInputDevice::CleanupVPRN()
{
    //VRPNButton->unregister_change_handler(this, vrpn_button_callback);
    delete VRPNButton;
    VRPNButton = nullptr;

    //VRPNAnalog->unregister_change_handler(this, vrpn_analog_callback);
    delete VRPNAnalog;
    VRPNAnalog = nullptr;
}

float FBciControllerInputDevice::GetNormalizedAnalogueValue(vrpn_float64 value)
{
    float normalizedValue = 0.0f;

    if (value > maxAnalogue)
    {
        maxAnalogue = value;
    }
    else if (value < minAnalogue)
    {
        minAnalogue = value;
    }
    
    if (maxAnalogue == minAnalogue)
    {
        normalizedValue = 0.0f;
    }
    else
    {
        normalizedValue = static_cast<float>( ( ( (value - minAnalogue) * 2 ) / (maxAnalogue - minAnalogue) ) - 1.0f );
    }

    return normalizedValue;
}

void FBciControllerInputDevice::CheckForDormantVRPN()
{
    if(lastUpdate != 0)
    {
        auto currentTime = std::chrono::system_clock::now();
        auto timeSinceUpdate = currentTime - std::chrono::system_clock::from_time_t(lastUpdate);
        std::chrono::seconds secondsSinceUpdate = std::chrono::duration_cast<std::chrono::seconds>(timeSinceUpdate);
        
        // If there's been no control signal for 5 seconds, reset the controller.
        //
        if(secondsSinceUpdate.count() > 5)
        {
            UE_LOG(LogBciControllerDevice, Log, TEXT("Resetting VRPN controller to zero"));
            MessageHandler->OnControllerAnalog(BciInputButtonMapping[0], 0, 0);
            lastUpdate = 0;
        }
    }
}

// 
// End VPRN Stuff