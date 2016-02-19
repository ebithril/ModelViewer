#include "Common Utilities_Precompiled.h"
#include "Timer.h"

void CU::Timer::Update()
{
	if (myIsPaused == false)
	{
		myTotalTime += myMasterDeltaTime->ScaleTime(myTimescale);
	}
}

void CU::Timer::SetPause(bool aPause)
{
	myIsPaused = aPause;
}

void CU::Timer::ResetTimer()
{
	myTotalTime = 0;
}

const CU::CU_Time& CU::Timer::GetMyTotalTime() const
{
	return myTotalTime;
}

const CU::ID& CU::Timer::GetMyID() const
{
	return myID;
}

void CU::Timer::SetMyTimescale(float aTimeMultiplier)
{
	myTimescale = aTimeMultiplier;
}

CU::Timer::Timer(const Timer &aTimer) : 
myID (aTimer.GetMyID()),
myMasterDeltaTime(aTimer.myMasterDeltaTime)
{
	myIsPaused = aTimer.myIsPaused;
	myTimescale = aTimer.myTimescale;
	myTotalTime = aTimer.GetMyTotalTime();
}

CU::Timer::Timer(const ID anID, CU_Time *aMasterDeltaTime) :
myID(anID),
myMasterDeltaTime(aMasterDeltaTime)
{
	myIsPaused = false;
	myTimescale = 1;
	myTotalTime = 0;
}

CU::Timer::~Timer()
{

}

const bool CU::Timer::GetMyIsPaused()
{
	return myIsPaused;
}

CommonUtilities::Timer::Timer()
{
	myMasterDeltaTime = nullptr;
	myTotalTime = 0;
	myTimescale = 1;
	myIsPaused = true;
}
