#include "Common Utilities_Precompiled.h"
#include "CU_Time.h"


CU::CU_Time::CU_Time(unsigned long long aTimeInMicroseconds)
{
	myTimeMicrosecond = aTimeInMicroseconds;
}

CU::CU_Time CU::CU_Time::operator-(const CU_Time &aTime) const
{
	return CU_Time(GetMyTimeInMicrosecond() - aTime.GetMyTimeInMicrosecond());
}

CU::CU_Time CU::CU_Time::operator=(const CU_Time &aTime)
{
	myTimeMicrosecond = aTime.GetMyTimeInMicrosecond();
	return *this;
}

CU::CU_Time CU::CU_Time::operator+=(const CU_Time &aTime)
{
	myTimeMicrosecond += aTime.GetMyTimeInMicrosecond();
	return *this;
}

CU::CU_Time::CU_Time(const CU_Time &aTime)
{
	myTimeMicrosecond = aTime.GetMyTimeInMicrosecond();
}

CU::CU_Time CU::CU_Time::ScaleTime(const long double &aTimeMultiplier) const
{
	unsigned long long scaledTime = static_cast <unsigned long long> (myTimeMicrosecond * aTimeMultiplier);
	return CU_Time(scaledTime);
}

CU::CU_Time::~CU_Time()
{
}

const unsigned long long& CU::CU_Time::GetMyTimeInMicrosecond() const
{
	return myTimeMicrosecond;
}

const float CU::CU_Time::GetMyTimeInMillisecond() const
{
	return (static_cast<float> (myTimeMicrosecond) / 1000);
}

const float CU::CU_Time::GetMyTimeInSeconds() const
{
	return (static_cast<float> (myTimeMicrosecond) / 1000000);
}

const float CU::CU_Time::GetMyTimeInMinutes() const
{
	return (GetMyTimeInSeconds() / 60);
}

const float CU::CU_Time::GetMyTimeInHours() const
{
	return (GetMyTimeInSeconds() / 3600);
}

void CU::CU_Time::SetMyTime(unsigned long long aTimeInMicroseconds)
{
	myTimeMicrosecond = aTimeInMicroseconds;
}

void CU::CU_Time::AddTime(const CU_Time &aTime)
{
	myTimeMicrosecond += aTime.GetMyTimeInMicrosecond();
}
