#include "Common Utilities_Precompiled.h"
#include "InputWrapper.h"
#include "EventManager.h"
#include "CUString.h"

#define KEY_DOWN(aBuffer, aKey) ((aBuffer)[aKey] & 0x80)

#ifdef SUPPORT_XBOX_CONTROLLER
const float locUpdateCheckActiveControllers = 2.0f; //Amount of time in seconds
const WORD locNumberOfControllersToCheck = 1; // 0-4 controllers
const WORD locNumberOfButtonsOnController = 14;
#endif

namespace CommonUtilities
{

	InputWrapper::InputWrapper()
	{

	}

	InputWrapper::~InputWrapper()
	{

#ifdef SUPPORT_XBOX_CONTROLLER
		delete[] myXboxControllers;
		myXboxControllers = nullptr;
#endif
		TerminateDirectInput();
	}

	bool InputWrapper::Init()
	{
		myControllerLogs.Init(locNumberOfControllersToCheck);

		for (unsigned short i = 0; i < locNumberOfControllersToCheck; i++)
		{
			myControllerLogs.Add(SingleControllerLog(i));
		}

		myHInstance = GetModuleHandle(NULL);

		//Create direct input object.
		myHRESULT = DirectInput8Create(myHInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&myLPDirectInput, NULL);

		if FAILED(myHRESULT)
		{
			TerminateDirectInput();
			assert(false && "Couldn't create DirectInput Object.");
		}

		if (CreateMouse() == false)
		{
			TerminateDirectInput();
			return false;
		}

		if (CreateKeyboard() == false)
		{
			TerminateDirectInput();
			return false;
		}

#ifdef SUPPORT_XBOX_CONTROLLER
		myTimeInSecondsSinceLastConnectionUpdate = locUpdateCheckActiveControllers;
		if (CreateXboxControllers() == false)
		{
			TerminateDirectInput();
			return false;
		}
#endif
		return true;
	}

	void InputWrapper::TerminateDirectInput()
	{
		if (myLPDirectInput)
		{
			if (myLPDirectInputDeviceKeyboard)
			{
				myLPDirectInputDeviceKeyboard->Unacquire();
				myLPDirectInputDeviceKeyboard->Release();
				myLPDirectInputDeviceKeyboard = nullptr;
			}
			if (myLPDirectInputDeviceMouse)
			{
				myLPDirectInputDeviceMouse->Unacquire();
				myLPDirectInputDeviceMouse->Release();
				myLPDirectInputDeviceMouse = nullptr;
			}
			myLPDirectInput->Release();
			myLPDirectInput = nullptr;
		}
	}

	void InputWrapper::SetMousePosition(const Position& aMousePosition)
	{
		tagPOINT aNewPosition;

		aNewPosition.x = static_cast<LONG>(aMousePosition.myX);
		aNewPosition.y = static_cast<LONG>(aMousePosition.myY);

		ClientToScreen(GetActiveWindow(), &aNewPosition);
		SetCursorPos(static_cast<int>(aNewPosition.x), static_cast<int>(aNewPosition.y));

	}

	void InputWrapper::ProcessKeyboardInput()
	{
		//take data from current and put it in last state
		for (int unsigned i = 0; i < 256; i++)
		{
			myKeyboardLastState[i] = myKeyboardCurrentState[i];
		}

		myHRESULT = myLPDirectInputDeviceKeyboard->Acquire();
		myHRESULT = myLPDirectInputDeviceKeyboard->GetDeviceState(sizeof(myKeyboardCurrentState),
			(LPVOID)&myKeyboardCurrentState);

		if FAILED(myHRESULT)
		{
			if (myHRESULT == DIERR_INPUTLOST)
			{
				myHRESULT = myLPDirectInputDeviceKeyboard->Acquire();
			}
			if (myHRESULT == DIERR_INPUTLOST)
			{
				assert("Failed to aquire keyboard device");
			}
		}
	}

	void InputWrapper::Update()
	{
		ProcessKeyboardInput();
		ProcessMouseInput();

#ifdef SUPPORT_XBOX_CONTROLLER //---------------------------

		ProcessXboxControllers();

#endif //---------------------------------------------------

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING
		UpdateEventLog();
#endif
	}

	bool InputWrapper::IsKeyDown(const unsigned char aKey) const
	{
		if (KEY_DOWN(myKeyboardCurrentState, aKey)
			&& KEY_DOWN(myKeyboardLastState, aKey))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputWrapper::IsKeyPressed(const unsigned char aKey) const
	{
		if (KEY_DOWN(myKeyboardCurrentState, aKey)
			&& !KEY_DOWN(myKeyboardLastState, aKey))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputWrapper::IsKeyReleased(const unsigned char aKey) const
	{
		if (KEY_DOWN(myKeyboardLastState, aKey)
			&& !KEY_DOWN(myKeyboardCurrentState, aKey))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputWrapper::CreateKeyboard()
	{
		//Retrieve Pointer to an IDirectInputDevice8 interface
		myHRESULT = myLPDirectInput->CreateDevice(
			GUID_SysKeyboard, &myLPDirectInputDeviceKeyboard, NULL);

		if FAILED(myHRESULT)
		{
			TerminateDirectInput();
			assert(false && "Couldn't retrieve pointer to input device interface.");
		}

		//Set dataformat of device.
		myHRESULT = myLPDirectInputDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard);

		if FAILED(myHRESULT)
		{
			TerminateDirectInput();
			assert(false && "Couldn't set dataformat of device.");
		}

		//Set cooperative level.
		myHRESULT = myLPDirectInputDeviceKeyboard->SetCooperativeLevel(GetActiveWindow(),
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


		if FAILED(myHRESULT)
		{
			TerminateDirectInput();
			assert(false && "Couldn't set cooperative level of device.");
		}

		//Get Access to the input device
		myHRESULT = myLPDirectInputDeviceKeyboard->Acquire();
		while (FAILED(myHRESULT) == true)
		{
			myHRESULT = myLPDirectInputDeviceKeyboard->Acquire();
		}
		if FAILED(myHRESULT)
		{
			TerminateDirectInput();
			assert(false && "Couldn't acquire device.");
		}
		return true;
	}

	bool InputWrapper::CreateMouse()
	{
		myHRESULT = myLPDirectInput->CreateDevice(GUID_SysMouse,
			&myLPDirectInputDeviceMouse, NULL);

		if FAILED(myHRESULT)
		{
			assert(false && "Couldn't create mouse device.");
			return false;
		}

		myHRESULT = myLPDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse2);

		if FAILED(myHRESULT)
		{
			assert(false && "Couldn't set data format for mouse");
			return false;
		}

		myHRESULT = myLPDirectInputDeviceMouse->SetCooperativeLevel(GetActiveWindow()
			, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		if FAILED(myHRESULT)
		{
			assert(false && "Couldn't set cooperative level for mouse");
			return false;
		}
		return true;
	}

	void InputWrapper::ProcessMouseInput()
	{
		myMouseLastState = myMouseCurrentState;

		myHRESULT = myLPDirectInputDeviceMouse->Acquire();

		ZeroMemory(&myMouseCurrentState, sizeof(myMouseCurrentState));
		if FAILED(myHRESULT)
		{
			return;
		}

		myHRESULT = myLPDirectInputDeviceMouse->GetDeviceState
			(sizeof(DIMOUSESTATE2), &myMouseCurrentState);

		if FAILED(myHRESULT)
		{
			assert(false && "Couldn't get mouse current state level");
		}

		tagPOINT cursorPoint;
		GetCursorPos(&cursorPoint);
		ScreenToClient(GetActiveWindow(), &cursorPoint);
		myMousePositionOnScreen = cursorPoint;
	}

	bool InputWrapper::IsMouseButtonDown(
		const eMouseButtons aMouseButton) const
	{

		if (KEY_DOWN(myMouseCurrentState.rgbButtons, static_cast<int>(aMouseButton))
			&& (KEY_DOWN(myMouseLastState.rgbButtons, static_cast<int>(aMouseButton))))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputWrapper::IsMouseButtonPressed(const eMouseButtons aMouseButton) const
	{
		if (KEY_DOWN(myMouseCurrentState.rgbButtons, static_cast<int> (aMouseButton))
			&& (!KEY_DOWN(myMouseLastState.rgbButtons, static_cast<int> (aMouseButton))))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputWrapper::IsMouseButtonReleased(const eMouseButtons aMouseButton) const
	{
		if (!KEY_DOWN(myMouseCurrentState.rgbButtons, static_cast<int> (aMouseButton))
			&& (KEY_DOWN(myMouseLastState.rgbButtons, static_cast<int> (aMouseButton))))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const tagPOINT& InputWrapper::GetMyMousePositionOnScreen() const
	{
		return myMousePositionOnScreen;
	}

	LONG InputWrapper::GetScrollWheelValue()const
	{
		return myMouseCurrentState.lZ;
	}

	MousePosition InputWrapper::GetRelativeMousePosition()
	{
		myMousePosition.myX = static_cast<float>(myMouseCurrentState.lX);
		myMousePosition.myY = static_cast<float>(myMouseCurrentState.lY);
		myMousePosition.myScrollWheel = static_cast<float>(myMouseCurrentState.lZ);

		return myMousePosition;
	}

	MousePosition InputWrapper::GetAbsoluteMousePosition()
	{
		tagPOINT getMouseXAndY;
		GetCursorPos(&getMouseXAndY);
		ScreenToClient(GetActiveWindow(), &getMouseXAndY);

		myMousePosition.myX = static_cast<float>(getMouseXAndY.x);
		myMousePosition.myY = static_cast<float>(getMouseXAndY.y);
		myMousePosition.myScrollWheel += static_cast<float>(myMouseCurrentState.lZ);

		return myMousePosition;
	}

#ifdef SUPPORT_XBOX_CONTROLLER

	void InputWrapper::ProcessXboxControllers()
	{
		float deltaTime = CU::EventManager::GetInstance()->GetDeltaTime();

		for (WORD i = 0; i < locNumberOfControllersToCheck; i++)
		{
			myXboxControllers[i].Update(deltaTime);
		}

		myTimeInSecondsSinceLastConnectionUpdate += deltaTime;
		if (myTimeInSecondsSinceLastConnectionUpdate > locUpdateCheckActiveControllers)
		{
			for (WORD i = 0; i < locNumberOfControllersToCheck; i++)
			{
				myXboxControllers[i].UpdateConnection();
			}
			myTimeInSecondsSinceLastConnectionUpdate = 0;
		}
	}

	void InputWrapper::CheckXboxControllers()
	{
		for (WORD i = 0; i < locNumberOfControllersToCheck; i++)
		{
			myXboxControllers[i].UpdateConnection();
		}
	}

	XboxController& InputWrapper::GetMyXboxController(WORD aPlayerNumber)
	{
		assert(aPlayerNumber >= 0 && aPlayerNumber <= locNumberOfControllersToCheck
			&& "No controller with player number.");
		return myXboxControllers[aPlayerNumber];
	}

	bool InputWrapper::CreateXboxControllers()
	{
		assert(locUpdateCheckActiveControllers > 0 && "Can't have an update time with a negative time.");
		assert(locNumberOfControllersToCheck >= 0 && locNumberOfControllersToCheck <= 4 &&
			"Can only check 0-4 controllers.");
		myXboxControllers = new XboxController[locNumberOfControllersToCheck];
		for (WORD i = 0; i < locNumberOfControllersToCheck; i++)
		{
			myXboxControllers[i] = XboxController(i);
		}
		return true;
	}

#endif

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING

	void InputWrapper::UpdateEventLog()
	{
		UpdateKeyboardEventLog();
		UpdateMouseEventLog();

#ifdef SUPPORT_XBOX_CONTROLLER //----------------------------------
		UpdateXboxControllersEventLog();
#endif  //---------------------------------------------------------
	}

	const CU::GrowingArray<KeyboardEvent>& InputWrapper::GetKeyBoardEventLog()const
	{
		return myKeyboardEventLog.GetLog();
	}

	const CU::GrowingArray<MouseEvent>& InputWrapper::GetMouseEventLog()const
	{
		return myMouseEventLog.GetButtonLog();
	}

	const CU::GrowingArray<MouseAxisLog>& InputWrapper::GetMouseAxisLog()const
	{
		return myMouseEventLog.GetAxisLog();
	}

#ifdef SUPPORT_XBOX_CONTROLLER //----------------------------------
	const SingleControllerLog InputWrapper::GetXboxControllerEventLog(WORD aPlayerNumber)const
	{
		assert(aPlayerNumber <= locNumberOfControllersToCheck && aPlayerNumber >= 0);
		return myControllerLogs[aPlayerNumber];
	}

#ifdef SUPPORT_INPUT_WRAPPER_EVENT_LOGGING

	const CU::GrowingArray<SingleControllerLog>& InputWrapper::GetControllerLogs()const
	{
		return myControllerLogs;
	}

#endif
	void InputWrapper::UpdateXboxControllersEventLog()
	{
		for (unsigned short i = 0; i < locNumberOfControllersToCheck; i++)
		{
			SingleControllerLog& log = myControllerLogs[i];
			XboxController& controller = GetMyXboxController(i);

			log.Clear();

			if (controller.IsConnected())
			{
				log.myIsConnected = true;
				for (unsigned short j = 0; j < locNumberOfButtonsOnController; j++)
				{
					ControllerButton button;
					if (j == 0)
					{
						button.myButton = eXboxControllerButtons::GAMEPAD_DPAD_UP;
					}
					else
					{
						button.myButton = static_cast<eXboxControllerButtons>(static_cast<WORD>(pow(2, j)));
					}
					if (controller.IsButtonDown(button.myButton) == true)
					{
						button.myKeyState = eKeyState::DOWN;
						log.myButtons.push_back(button);
					}

					if (controller.IsButtonPressed(button.myButton) == true)
					{
						button.myKeyState = eKeyState::PRESSED;
						log.myButtons.push_back(button);
					}

					if (controller.IsButtonReleased(button.myButton) == true)
					{
						button.myKeyState = eKeyState::RELEASED;
						log.myButtons.push_back(button);
					}
				}

				for (unsigned short j = 0; j < eXboxAnalogue::MY_SIZE; j++)
				{
					Vector2f value = controller.GetAnalogueValue(static_cast<eXboxAnalogue>(j));

					if (value.x > 0 || value.x < 0 || value.y > 0 || value.y < 0)
					{
						AnalogueLog data;
						data.myAnalogueType = static_cast<eXboxAnalogue>(j);
						data.myNormalizedValue = value;

						log.myAnalogueLogs.push_back(data);
					}
				}
			}
		}
	}
#endif //----------------------------------------------------------

	void InputWrapper::UpdateKeyboardEventLog()
	{
		myKeyboardEventLog.ClearLog();
		for (unsigned char i = 0; i < 255; i++)
		{
			if (IsKeyDown(i) == true)
			{
				myKeyboardEventLog.AddKeyboardEvent(KeyboardEvent(static_cast<eKeyboardKeys>(i), eKeyState::DOWN));
			}

			else if (IsKeyReleased(i) == true)
			{
				myKeyboardEventLog.AddKeyboardEvent(KeyboardEvent(static_cast<eKeyboardKeys>(i), eKeyState::RELEASED));
			}

			else if (IsKeyPressed(i) == true)
			{
				myKeyboardEventLog.AddKeyboardEvent(KeyboardEvent(static_cast<eKeyboardKeys>(i), eKeyState::PRESSED));
			}
		}
	}

	void InputWrapper::UpdateMouseEventLog()
	{
		myMouseEventLog.ClearLog();
		for (unsigned short i = 0; i < 3; i++)
		{
			if (IsMouseButtonDown(static_cast<eMouseButtons>(i)))
			{
				myMouseEventLog.AddMouseEvent(MouseEvent(static_cast<eMouseButtons>(i), eKeyState::DOWN));
			}

			if (IsMouseButtonPressed(static_cast<eMouseButtons>(i)))
			{
				myMouseEventLog.AddMouseEvent(MouseEvent(static_cast<eMouseButtons>(i), eKeyState::PRESSED));
			}

			if (IsMouseButtonReleased(static_cast<eMouseButtons>(i)))
			{
				myMouseEventLog.AddMouseEvent(MouseEvent(static_cast<eMouseButtons>(i), eKeyState::RELEASED));
			}
		}

		/*long deltaMouseStateX;
		long deltaMouseStateY;
		long deltaMouseStateZ;

		if (myMouseCurrentState.lX != 0)
		{
		deltaMouseStateX = myMouseCurrentState.lX - myMouseLastState.lX;

		CU::String<128> kek = "DeltamouseX: ";
		kek += deltaMouseStateX;

		DL_PRINT(kek.c_str());
		}
		else
		{
		deltaMouseStateX = 0;
		}

		if (myMouseCurrentState.lY != 0)
		{
		deltaMouseStateY = myMouseCurrentState.lY - myMouseLastState.lY;
		}
		else
		{
		deltaMouseStateY = 0;
		}

		if (myMouseCurrentState.lZ != 0)
		{
		deltaMouseStateZ = myMouseCurrentState.lZ - myMouseLastState.lZ;
		}
		else
		{
		deltaMouseStateZ = 0;
		}*/

		/*if (deltaMouseStateX != 0)
		{*/
		myMouseEventLog.AddMouseEvent(MouseAxisLog(eMouseAxis::X, myMouseCurrentState.lX));
		//	}

		/*if (deltaMouseStateY != 0)
		{*/
		myMouseEventLog.AddMouseEvent(MouseAxisLog(eMouseAxis::Y, myMouseCurrentState.lY));
		//}

		/*	if (deltaMouseStateZ != 0)
			{*/
		myMouseEventLog.AddMouseEvent(MouseAxisLog(eMouseAxis::SCROLL_WHEEL, myMouseCurrentState.lZ));
		//}
	}

#endif

}

