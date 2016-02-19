#pragma once

namespace CommonUtilities
{
	enum class eLayers
	{
		TEXT,
		OBJECTIVE_TEXT,
		DEFAULT,
		MY_SIZE //SHOULD ALWAYS BE LAST
	};

	enum class eDirection
	{
		LEFT_UP,
		RIGHT_UP,
		LEFT_DOWN,
		RIGHT_DOWN,
		MY_SIZE,		
		ERROR_,
		NONE = 0
	};

	enum class eAnimation
	{
		IDLE,
		MOVE,
		ATTACK,
		TAKE_DAMAGE,
		DEATH,
		MY_SIZE
	};

}

namespace CU = CommonUtilities;