#pragma once

#include "InputCoreTypes.h"

/**
* Digital weight sensors on the Wii Balance Board
*/


enum class EBciControllerAxes
{
    WeightSensor1 = 0,

    /** Total number of weight axes */
    TotalAxisCount = 1
};

struct FBciControllerKey
{
    static const FKey Bci_WeighingSensor1;
};

struct FBciControllerKeyNames
{
    typedef FName Type;

    static const FName Bci_WeighingSensor1;
};

/**
* Capacitive Axis State
*/
struct FBciControllerSensorState
{
    /** The axis that this sensor state maps to */
    FName Axis;

    /** The zero value (no additional weight) */
    unsigned short int Zero;

    /** What is the current sensor reading, from 0.f to 1.f */
    float Scale;

    /** What is the current sensor reading, from 0.f to 1.f */
    unsigned short int State;

    FBciControllerSensorState()
        : Axis(NAME_None)
        , Zero(900)
        , Scale(100.f)
        , State(0)
    {
    }

    float GetValue() {
        if (State < Zero) {
            return 0.f;
        }

        return (float)(State - Zero) * Scale;
    }
};

/**
* Input state for an pseudo controller
*/
struct FBciControllerState
{
    /** Weight axes */
    FBciControllerSensorState WeightAxes[(int32)EBciControllerAxes::TotalAxisCount];

    /** Default constructor */
    FBciControllerState()
    {
        WeightAxes[(int32)EBciControllerAxes::WeightSensor1].Axis = FBciControllerKeyNames::Bci_WeighingSensor1;
    }
};