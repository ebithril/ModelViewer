#pragma once

#include "Timer.h"
#include "GrowingArray.h"

namespace CommonUtilities
{

	class TimerManager
	{
	public:

		static void Create();
		static void Destroy();
		static TimerManager* GetInstance();

		void Update();
		const float UpdateNewFrame();
		const ID CreateTimer();
		Timer& GetTimer(const ID anID);
		const Timer& GetMyMasterTimer();
		const int GetMyFramesPerSecond() const;
		const float GetFrameDeltaTime() const;

	private:

		static TimerManager* ourInstance;

		CU_Time myMasterDeltaTime;
		CU_Time myFrameDeltaTime;
		Timer myMasterTimer;
		Timer myFramesTimer;
		Timer myFrameDeltaTimer;

		GrowingArray<Timer> myTimers;

		LARGE_INTEGER *myProcessorFrequency;
		LARGE_INTEGER *myStartingTickCounter;
		LARGE_INTEGER *myCurrentTickCounter;
		LARGE_INTEGER myElapsedMicroseconds;

		int myFramesPerSecond;
		int myFramesCounter;

		void CalculateMasterDeltaTime();
		void UpdateTimers();
		void CalculateFramesPerSecond();

		TimerManager();
		~TimerManager();
	};

}

namespace CU = CommonUtilities;