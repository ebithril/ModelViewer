#pragma once
#pragma comment (lib, "Xinput9_1_0.lib")
#include <Windows.h>
#include <Xinput.h>
#include <assert.h>
#include <cmath>
#include "EnumKeys.h"
#include "Vector.h"

namespace CommonUtilities
{
	class XboxController
	{
	public:

		XboxController(WORD aPlayerNumber);
		XboxController();
		~XboxController();
		void Update(float aDeltaTime);
		void UpdateConnection();
		bool IsConnected();

		//----------------------------------------
		//Frequency motors range from 0.0f - 1.0f.
		//Vibration until next vibrate() call is default value on time variable.
		void Vibrate(const float aLowFrequencyMotor = 0.0f,const float aHighFrequencyMotor = 0.0f
			, const float anAmountOfTimeInSeconds = 0.0f);

		const Vector2f& GetAnalogueValue(eXboxAnalogue anAnalogue);
		
		bool IsButtonDown(const WORD aButton) const;
		bool IsButtonPressed(const WORD aButton) const;
		bool IsButtonReleased(const WORD aButton) const;

	private:

		WORD myControllerNumber;
		WORD myLeftMotorVibration;
		WORD myRightMotorVibration;

		Vector2f myLeftStick;
		float myLeftStickMagnitude;
		Vector2f myNormalizedLeftStick;
		float myNormalizedLeftStickMagnitude;

		Vector2f myRightStick;
		float myRightStickMagnitude;
		Vector2f myNormalizedRightStick;
		float myNormalizedRightStickMagnitude; 

		float myTimeLeftOnVibration;
		bool myTimedVibration;
		bool myIsConnected;
	
		BYTE myLeftTrigger;
		BYTE myRightTrigger;
		Vector2f myNormalizedTriggers; // x = left, y = right;
	
		XINPUT_VIBRATION myVibration;

		HRESULT myHRESULT;

		XINPUT_STATE myCurrentState;
		XINPUT_STATE myLastState;
		XINPUT_STATE testState;

		void UpdateRightStick();
		void UpdateLeftStick();
		void UpdateTriggers();
		void UpdateVibration(float aDeltaTime);
		
		float CheckBetween0And1(const float afloat)const;
	};
}

namespace CU = CommonUtilities;