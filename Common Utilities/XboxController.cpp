#include "Common Utilities_Precompiled.h"
#include "XboxController.h"



#define BUTTON_DOWN(aBuffer, aKey) ((aBuffer.Gamepad.wButtons) & aKey)

const WORD loc1PercentVibration = 655;
const float locStickMaxRange = 32767;
const float locTriggerMaxRange = 255;
const float locVibrationMaxRange = 65535.0f;

namespace CommonUtilities
{
	XboxController::XboxController()
	{

	}

	XboxController::XboxController(WORD aPlayerNumber)
	{
		myControllerNumber = aPlayerNumber;

		myLeftStick = { 0.f, 0.f };

		myLeftStickMagnitude = 0;
		myNormalizedLeftStick = { 0.f, 0.f };

		myNormalizedLeftStickMagnitude = 0;

		myRightStick = { 0.f, 0.f };
		myRightStickMagnitude = 0;
		myNormalizedRightStick = { 0.f, 0.f };
		myNormalizedRightStickMagnitude = 0;

		myLeftTrigger = 0;
		myRightTrigger = 0;
		myNormalizedTriggers = { 0.f, 0.f };

		myTimedVibration = false;
		myTimeLeftOnVibration = 0.0f;

		myLeftMotorVibration = 0;
		myRightMotorVibration = 0;

		myIsConnected = false;
		Vibrate();

	}

	XboxController::~XboxController()
	{
		//turn off vibration when shutting down
		Vibrate();
	}

	void XboxController::Update(float aDeltaTime)
	{
		myLastState = myCurrentState;
		XInputGetState(myControllerNumber, &myCurrentState);
		UpdateLeftStick();
		UpdateRightStick();
		UpdateTriggers();
		UpdateVibration(aDeltaTime);
	}

	void XboxController::Vibrate(const float aLowFrequencyMotor, const float aHighFrequencyMotor
		, const float anAmountOfTimeInSeconds)
	{
		float lowFrequency = CheckBetween0And1(aLowFrequencyMotor);
		float highFrequency = CheckBetween0And1(aHighFrequencyMotor);

		myLeftMotorVibration = static_cast<WORD>(lowFrequency * locVibrationMaxRange);
		myRightMotorVibration = static_cast<WORD>(highFrequency * locVibrationMaxRange);

		myVibration.wLeftMotorSpeed = myLeftMotorVibration;
		myVibration.wRightMotorSpeed = myRightMotorVibration;

		if (anAmountOfTimeInSeconds > 0.0f)
		{
			myTimedVibration = true;
			myTimeLeftOnVibration = anAmountOfTimeInSeconds;
		}
		else
		{
			myTimedVibration = false;
		}
		XInputSetState(myControllerNumber, &myVibration);
	}

	void XboxController::UpdateVibration(float aDeltaTime)
	{
		if (myTimedVibration == true)
		{
			myTimeLeftOnVibration -= aDeltaTime;

			if (myTimeLeftOnVibration <= 0)
			{
				Vibrate();
			}
		}
	}

	void XboxController::UpdateTriggers()
	{
		myLeftTrigger = myCurrentState.Gamepad.bLeftTrigger;
		myRightTrigger = myCurrentState.Gamepad.bRightTrigger;

		if (myLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			myLeftTrigger -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
			myNormalizedTriggers.x = static_cast<float> (myLeftTrigger) / (locTriggerMaxRange - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		}
		else
		{
			myNormalizedTriggers.x = 0;
		}

		if (myRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			myRightTrigger -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
			myNormalizedTriggers.y = static_cast<float> (myRightTrigger) / (locTriggerMaxRange - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		}
		else
		{
			myNormalizedTriggers.y = 0;
		}
	}

	void XboxController::UpdateLeftStick()
	{
		myLeftStick = { static_cast<float>(myCurrentState.Gamepad.sThumbLX), static_cast<float>( myCurrentState.Gamepad.sThumbLY) };

		myLeftStickMagnitude = sqrt(myLeftStick.x * myLeftStick.x + myLeftStick.y * myLeftStick.y);

		myNormalizedLeftStickMagnitude = 0.0;

		if (myLeftStickMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (myLeftStickMagnitude > locStickMaxRange)
			{
				myLeftStickMagnitude = locStickMaxRange;
			}

			myLeftStickMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			myNormalizedLeftStickMagnitude = myLeftStickMagnitude / (locStickMaxRange - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else
		{
			myLeftStickMagnitude = 0.0;
			myNormalizedLeftStickMagnitude = 0.0;
		}

		if (myNormalizedLeftStickMagnitude > 0)
		{
			myNormalizedLeftStick.x = myLeftStick.x / locStickMaxRange;
			myNormalizedLeftStick.y = myLeftStick.y / locStickMaxRange;
		}
		else
		{
			myNormalizedLeftStick.x = 0;
			myNormalizedLeftStick.y = 0;
		}


	}

	void XboxController::UpdateRightStick()
	{
		myRightStick.x = myCurrentState.Gamepad.sThumbRX;
		myRightStick.y = myCurrentState.Gamepad.sThumbRY;

		myRightStickMagnitude = sqrt(myRightStick.x * myRightStick.x + myRightStick.y * myRightStick.y);

		myNormalizedRightStickMagnitude = 0.0;

		if (myRightStickMagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			if (myRightStickMagnitude > locStickMaxRange)
			{
				myRightStickMagnitude = locStickMaxRange;
			}

			myRightStickMagnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

			myNormalizedRightStickMagnitude = myRightStickMagnitude / (locStickMaxRange - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else
		{
			myRightStickMagnitude = 0.0;
			myNormalizedRightStickMagnitude = 0.0;
		}

		if (myNormalizedRightStickMagnitude > 0)
		{
			myNormalizedRightStick.x = myRightStick.x / locStickMaxRange;
			myNormalizedRightStick.y = myRightStick.y / locStickMaxRange;
		}
		else
		{
			myNormalizedRightStick.x = 0;
			myNormalizedRightStick.y = 0;
		}
	}

	void XboxController::UpdateConnection()
	{
		myHRESULT = XInputGetState(myControllerNumber, &testState);

		if (myHRESULT == ERROR_SUCCESS)
		{
			myIsConnected = true;
		}
		else
		{
			myIsConnected = false;
		}
	}

	bool XboxController::IsButtonDown(const WORD aButton) const
	{
		if (BUTTON_DOWN(myCurrentState, aButton)
			&& BUTTON_DOWN(myLastState, aButton))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool XboxController::IsButtonPressed(const WORD aButton) const
	{
		if (BUTTON_DOWN(myCurrentState, aButton)
			&& !BUTTON_DOWN(myLastState, aButton))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool XboxController::IsButtonReleased(const WORD aButton) const
	{
		if (BUTTON_DOWN(myLastState, aButton)
			&& !BUTTON_DOWN(myCurrentState, aButton))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool XboxController::IsConnected()
	{
		return myIsConnected;
	}

	const Vector2f& XboxController::GetAnalogueValue(eXboxAnalogue anAnalogue)
	{
		switch (anAnalogue)
		{
		case LEFT_STICK:
			return myNormalizedLeftStick;
			break;
		case RIGHT_STICK:
			return myNormalizedRightStick;
			break;
		case TRIGGERS:
			return myNormalizedTriggers;
			break;

		default:
			break;
		}

		DL_ASSERT("Couldn't get analoguevalue");

		return myNormalizedLeftStick;
	}

	float XboxController::CheckBetween0And1(const float aFloat)const
	{
		if (aFloat < 0)
		{
			return 0;
		}
		else if (aFloat > 1)
		{
			return 1;
		}
		else
		{
			return aFloat;
		}
	}
}