#pragma once
#include "GrowingArray.h"
#include "EventEnums.h"
#include <string>
#include "Event.h"
#include "PoolPointer.h"
#include "CUString.h"
#include "EnumKeys.h"
#include "EventEnums.h"
#include <functional>
#include "Matrix.h"
#include "RayStruct.h"
#include "RenderCommandFont.h"

class Node;
enum class eControlCommands;

namespace GraphicsEngine
{
	class Decal;
	class Texture;
}
namespace GE = GraphicsEngine;

namespace CommonUtilities
{
	class MultiEvent : public Event
	{
	public:
		MultiEvent() : Event(CU::eEvent::MULTI_EVENT) {};
		virtual ~MultiEvent(){};
		GrowingArray<PoolPointer<Event>> myPoolpointers;
	};

	class FunctionEvent : public Event
	{
	public:

		FunctionEvent() : Event(CU::eEvent::FUNCTION) {};
		virtual ~FunctionEvent(){};
		std::function<void()> myFunction;
	};

	class SetFlagEvent : public Event // DELETE WHEN Events are more testable
	{
	public:
		virtual ~SetFlagEvent(){};
		CU::String<128> myFlagName;
		bool mySetFlagTo;
	};

	class TimedEvent : public Event
	{
	public:
		TimedEvent() : Event(eEvent::TIMED_EVENT){};
		virtual ~TimedEvent(){};
		float myTimeLeft;
		GrowingArray<PoolPointer<Event>> myPoolpointers;
		std::function<void()> myFunction;
	};

	class StartSceneEvent : public Event
	{
	public:
		StartSceneEvent() : Event(eEvent::START_SCENE) {};
		virtual ~StartSceneEvent(){};
		CU::String<128> mySceneName;
		std::string myVideoName;
	};

	class PlayerInputEnabledEvent : public Event
	{
	public:
		PlayerInputEnabledEvent(const bool aIsEnabled) : Event(eEvent::PLAYER_INPUT_ENABLED) { myIsEnabled = aIsEnabled; };
		virtual ~PlayerInputEnabledEvent(){};
		bool myIsEnabled;
	};

	class ModifyDeltaTimeEvent : public Event
	{
	public:
		ModifyDeltaTimeEvent(const float aModifier) : Event(eEvent::MODIFY_DELTATIME) { myModifier = aModifier; };
		virtual ~ModifyDeltaTimeEvent(){};
		float myModifier;
	};

	class KeyboardInputEvent : public Event
	{
	public:
		KeyboardInputEvent(const eKeyboardKeys aKey, const eKeyState aKeyState) : Event(eEvent::KEYBOARD_INPUT_EVENT) { myKeyboardKey = aKey; myKeyState = aKeyState; };
		virtual ~KeyboardInputEvent(){};
		eKeyboardKeys myKeyboardKey;
		eKeyState myKeyState;
	};

	class PlaySubtitleEvent : public Event
	{
	public:
		PlaySubtitleEvent() : Event(eEvent::PLAY_SUBTITLE){};
		virtual ~PlaySubtitleEvent(){};
		CU::String<128> mySubtitle;
	};

	class LoadSubtitleEvent : public Event
	{
	public:
		LoadSubtitleEvent() : Event(eEvent::LOAD_SUBTITLE){};
		virtual ~LoadSubtitleEvent(){};
		CU::String<128> mySubtitle;
	};

	class VibrateControllerEvent : public Event
	{
	public:
		VibrateControllerEvent() : Event(eEvent::VIBRATE_CONTROLLER) {};
		virtual ~VibrateControllerEvent(){};
		unsigned short myPlayernumber;
		float myLowFrequencyMotorStrength;
		float myHighFrequencyMotorStrength;
		float myTimeToVibrate;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const Vector2f& aMovement) : Event(eEvent::MOUSE_MOVE_EVENT) { myMovement = aMovement; };
		virtual ~MouseMoveEvent(){};
		Vector2f myMovement;
	};

	class CommandInputEvent : public Event
	{
	public:
		CommandInputEvent() : Event(eEvent::CONTROL_COMMAND){};
		virtual ~CommandInputEvent(){};
		Vector2f myMovement;
		eControlCommands myCommand;
		eKeyState myKeyState;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float aScrollValue) : Event(eEvent::MOUSE_SCROLL_EVENT) { myScrollValue = aScrollValue; };
		virtual ~MouseScrollEvent(){};
		float myScrollValue;
	};

	class MouseInputEvent : public Event
	{
	public:
		MouseInputEvent() : Event(eEvent::MOUSE_INPUT_EVENT) {};
		virtual ~MouseInputEvent(){};
		Vector2f myMousePosition;
		eMouseButtons myMouseButton;
		eKeyState myKeyState;
	};

	class ControllerButtonInputEvent : public Event
	{
	public:
		ControllerButtonInputEvent() : Event(eEvent::CONTROLLER_BUTTON_INPUT){};
		virtual ~ControllerButtonInputEvent(){};
		eXboxControllerButtons myButton;
		eKeyState myKeyState;
	};

	class ControllerAnalogueInputEvent : public Event
	{
	public:
		ControllerAnalogueInputEvent() : Event(eEvent::CONTROLLER_ANALOGUE_INPUT) {};
		virtual ~ControllerAnalogueInputEvent(){};
		eXboxAnalogue myAnalogue;
		Vector2f myNormalizedValue;
	};




	class RemoveGameObjectEvent : public Event
	{
	public:
		RemoveGameObjectEvent(){ myEventType = CU::eEvent::REMOVE_GAMEOBJECT; };
		virtual ~RemoveGameObjectEvent(){};
		int myGameObjectId;
	};


	class TextOnTexture : public Event
	{
	public:
		TextOnTexture(){ myEventType = CU::eEvent::TEXT_ON_TEXTURE; };
		int myID;
		std::string myOptionalTexture;
		CU::GrowingArray<RenderCommandFont> myFonts;
	};

}

namespace CU = CommonUtilities;
