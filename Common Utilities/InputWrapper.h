#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include "dinput.h"
#include "EnumKeys.h"
#include "DL_Assert.h"
#include "Vector.h"

//Uncomment to support controllers
#define SUPPORT_XBOX_CONTROLLER 

//Uncomment to support event logging *not finished*
#define SUPPORT_INPUT_WRAPPER_EVENT_LOGGING

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING	//EVENT LOGGING --------------------------

#include "KeyboardEventLog.h"
#include "MouseEventLog.h"

#ifdef SUPPORT_XBOX_CONTROLLER
#include "SingleControllerLog.h"
#endif

#endif	//EVENT LOGGING END-----------------------------------------------------------

#ifdef SUPPORT_XBOX_CONTROLLER
#include "XboxController.h"
#endif

namespace CommonUtilities
{
	struct MousePosition
	{
		float myX;
		float myY;
		float myScrollWheel = 0.0f;
	};
	class InputWrapper
	{
	public:

		InputWrapper();
		~InputWrapper();

		void Update();

		bool Init();

		bool IsKeyDown(const unsigned char aKey) const;
		bool IsKeyPressed(const unsigned char aKey) const;
		bool IsKeyReleased(const unsigned char aKey) const;

		bool IsMouseButtonDown(const eMouseButtons aMouseButton) const;
		bool IsMouseButtonPressed(const eMouseButtons aMouseButton) const;
		bool IsMouseButtonReleased(const eMouseButtons aMouseButton) const;

		MousePosition GetRelativeMousePosition();
		MousePosition GetAbsoluteMousePosition();

		void SetMousePosition(const Position& aMousePosition);

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING
		const CU::GrowingArray<KeyboardEvent>&  GetKeyBoardEventLog()const;
		const CU::GrowingArray<MouseEvent>& GetMouseEventLog()const;
		const CU::GrowingArray<MouseAxisLog>& GetMouseAxisLog()const;

#ifdef SUPPORT_XBOX_CONTROLLER

		const CU::GrowingArray<SingleControllerLog>& GetControllerLogs()const;
#endif
#endif

#ifdef SUPPORT_XBOX_CONTROLLER

		XboxController& GetMyXboxController(WORD aPlayerNumber);

#endif
		LONG GetScrollWheelValue()const;
		const tagPOINT& GetMyMousePositionOnScreen() const;

	private:
	
		HINSTANCE myHInstance;
		HRESULT myHRESULT;
		LPDIRECTINPUT8 myLPDirectInput;
		LPDIRECTINPUTDEVICE8 myLPDirectInputDeviceKeyboard;
		LPDIRECTINPUTDEVICE8 myLPDirectInputDeviceMouse;

		tagPOINT myMousePositionOnScreen;

		DIMOUSESTATE2 myMouseCurrentState;
		DIMOUSESTATE2 myMouseLastState;
		MousePosition myMousePosition;
		char myKeyboardCurrentState[256];
		char myKeyboardLastState[256];

		bool CreateKeyboard();
		bool CreateMouse();

		void TerminateDirectInput();
		void ProcessKeyboardInput();
		void ProcessMouseInput();

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING
		void UpdateEventLog();
		void UpdateKeyboardEventLog();
		void UpdateMouseEventLog();
		KeyboardEventLog myKeyboardEventLog;
		MouseEventLog myMouseEventLog;
#endif

#ifdef SUPPORT_XBOX_CONTROLLER

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING //---------------------------------
		void UpdateXboxControllersEventLog();
		const SingleControllerLog GetXboxControllerEventLog(WORD aPlayerNumber)const;
		GrowingArray<SingleControllerLog> myControllerLogs;
#endif//----------------------------------------------------------------------

		void ProcessXboxControllers();

		XboxController *myXboxControllers;
		float myTimeInSecondsSinceLastConnectionUpdate;
		void CheckXboxControllers();
		bool CreateXboxControllers();

#endif
	};

}

namespace CU = CommonUtilities;