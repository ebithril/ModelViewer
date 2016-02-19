#pragma once

#include "DL_Debug.h"

namespace CommonUtilities
{
	enum class eEvent
	{

		//Input Events ------------------------------------------
		INPUT_EVENTS,
		MOUSE_INPUT_EVENT,
		MOUSE_MOVE_EVENT,
		MOUSE_SCROLL_EVENT,
		KEYBOARD_INPUT_EVENT,
		KEYBOARD_STATE_EVENT,
		CONTROLLER_BUTTON_INPUT,
		CONTROLLER_ANALOGUE_INPUT,
		PLAYER_INPUT_ENABLED,
		//-------------------------------------------------------

		//Other events ------------------------------------------
		FUNCTION,
		SET_FLAG,
		IF_EVENT,
		MULTI_EVENT,
		TIMED_EVENT,
		QUIT,
		ERROR_,
		DEBUG_MODE,
		IS_LOADING,
		VIBRATE_CONTROLLER,
		CLEAR_PLAYER_GUI,
		SET_AFFECTED_BY_FOW,
		POP_MAIN_STATE,

		//State Events
		NO_SCENE_HANDLE,
		REMOVE_CHILD_STATE,
		REMOVE_MAIN_STATE,

		//-------------------------------------------------------

		//Renderer Events ------------------------------------------
		RESIZE,
		NEW_FRAME,
		TOGGLE_SHADOWS,
		TEXT_ON_TEXTURE,
		SET_TEXTURE,
		RENDER_OBJECTIVE_TEXT,
		FADE_IN,
		FADE_OUT,
		CHANGE_ALBEDO_EVENT,
		CHANGE_EMMISIVE_EVENT,
		CHANGE_SCALE_EVENT,
		SPAWN_PARTICLES,
		//-------------------------------------------------------

		// Game--------------------------------------
		PAUSE,
		START_SCENE,
		RESUME,
		LEVEL_CLEARED,
		LOAD_NEXT_SCENE,
		SCENE_FINISHED_LOADING,
		RELOAD_SCENE,
		MODIFY_DELTATIME,
		SET_COLLISIONS,
		CONTROL_COMMAND,
		CURSOR_MOVE_COMMAND,
		CAMERA_PROJECTION,
		ADD_GAME_OBJECT,
		ADD_GUI_OBJECT,
		SET_GAME_OBJECT_SCREEN_POSITION,
		SET_GAME_OBJECT_SCALE,
		REMOVE_GAMEOBJECT,
		REMOVE_FROM_OCTREE,
		COLLISION_EVENT,

		//Pathfind Events
		GENERATE_UNIT_PATH,
		REPORT_UNIT_PATH,

		//InternalEvents-----------------------------------------
		SET_SCALE,
		LOAD_ANIMATION,
		START_ANIMATION,
		STOP_ANIMATION,
		HIDE_BUTTON,
		BUTTON_EVENT,
		SET_CUT_OFF_VALUE,
		ADD_LIGHT,
		//-------------------------------------------------------

		PLAY_SUBTITLE,
		LOAD_SUBTITLE,
		PAUSE_SUBTITLE,
		RESUME_SUBTITLE,
		//-------------------------------------------------------

		MY_SIZE // Always last do not remove!
	};

	enum class eToolEvents
	{
		SPAWN_ENEMY_WITHIN_AREA,
		SET_FLAG,
		START_SCENE,
		QUIT,
		PLAY_SUBTITLE,
		REMOVE_MAIN_STATE,
		REMOVE_CHILD_STATE,
		MAX_SIZE
	};
	
}

namespace CU = CommonUtilities;