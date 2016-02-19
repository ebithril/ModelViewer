#include "Common Utilities_Precompiled.h"
#include "TimerManager.h"
#include "IDFactory.h"
#include <Query.h>

namespace CommonUtilities
{
	const int loc1SecondInMicroseconds = 1000000;

	TimerManager* TimerManager::ourInstance = nullptr;

	TimerManager::TimerManager() :
		myMasterTimer(IDFactory::CreateID(), &myMasterDeltaTime),
		myFramesTimer(IDFactory::CreateID(), &myMasterDeltaTime),
		myFrameDeltaTimer(IDFactory::CreateID(), &myMasterDeltaTime)
	{
		myProcessorFrequency = new LARGE_INTEGER;
		myStartingTickCounter = new LARGE_INTEGER;
		myCurrentTickCounter = new LARGE_INTEGER;

		myMasterDeltaTime.SetMyTime(_60FPSInMicroseconds);
		QueryPerformanceCounter(myStartingTickCounter);
		QueryPerformanceFrequency(myProcessorFrequency);
		myFramesCounter = 0;
		myTimers.Init(16);
	}

	TimerManager::~TimerManager()
	{
		SAFE_DELETE(myProcessorFrequency);
		SAFE_DELETE(myStartingTickCounter);
		SAFE_DELETE(myCurrentTickCounter);
	}

	void TimerManager::UpdateTimers()
	{
		myMasterTimer.Update();
		myFramesTimer.Update();
		myFrameDeltaTimer.Update();

		for (unsigned short i = 0; i < myTimers.Size(); i++)
		{
			myTimers[i].Update();
		}
	}

	Timer& TimerManager::GetTimer(ID anID)
	{
		for (unsigned short i = 0; i < myTimers.Size(); i++)
		{
			if (myTimers[i].GetMyID() == anID)
			{
				return myTimers[i];
			}
		}

		assert("Timer ID not found.");
		return myMasterTimer;
	}

	const ID TimerManager::CreateTimer()
	{
		myTimers.Add(
			Timer(IDFactory::CreateID(), &myMasterDeltaTime));
		return myTimers[myTimers.Size() - 1].GetMyID();
	}

	void TimerManager::CalculateMasterDeltaTime()
	{
		QueryPerformanceCounter(myCurrentTickCounter);
		myElapsedMicroseconds.QuadPart = myCurrentTickCounter->QuadPart - myStartingTickCounter->QuadPart;
		myStartingTickCounter->QuadPart = myCurrentTickCounter->QuadPart;

		myElapsedMicroseconds.QuadPart *= 1000000;
		myElapsedMicroseconds.QuadPart /= myProcessorFrequency->QuadPart;

		myMasterDeltaTime = myElapsedMicroseconds.QuadPart;
	}

	void TimerManager::Create()
	{
		ourInstance = new TimerManager();
	}

	void TimerManager::Destroy()
	{
		SAFE_DELETE(ourInstance);
	}

	TimerManager* TimerManager::GetInstance()
	{
		if (ourInstance == nullptr)
		{
			DL_ASSERT("TimerManager not initiliazed.");
		}
		return ourInstance;
	}

	void TimerManager::Update()
	{
		CalculateMasterDeltaTime();
		UpdateTimers();
	}

	const float CommonUtilities::TimerManager::UpdateNewFrame()
	{
		Update();

		myFramesCounter++;

		myFrameDeltaTime = myFrameDeltaTimer.GetMyTotalTime();
		myFrameDeltaTimer.ResetTimer();
		if (myMasterDeltaTime.GetMyTimeInSeconds() > 1.0)
		{
			myMasterDeltaTime.SetMyTime(_60FPSInMicroseconds);
		}

		if (myFramesTimer.GetMyTotalTime().GetMyTimeInSeconds() > 0.5)
		{
			CalculateFramesPerSecond();
		}

		return GetFrameDeltaTime();
	}

	const float CommonUtilities::TimerManager::GetFrameDeltaTime() const
	{
		float deltaTime = myFrameDeltaTime.GetMyTimeInSeconds();

		if (deltaTime > 1.0f)
		{
			deltaTime = _60FPSInSeconds;
		}

		return deltaTime;
	}

	void TimerManager::CalculateFramesPerSecond()
	{
		myFramesPerSecond = static_cast<int>(loc1SecondInMicroseconds /
			(myFramesTimer.GetMyTotalTime().GetMyTimeInMicrosecond() / myFramesCounter));
		myFramesCounter = 0;
		myFramesTimer.ResetTimer();
	}

	const Timer& TimerManager::GetMyMasterTimer()
	{
		return myMasterTimer;
	}

	const int TimerManager::GetMyFramesPerSecond() const
	{
		return myFramesPerSecond;
	}

}

