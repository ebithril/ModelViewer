#pragma once

namespace CommonUtilities
{
		 
	class CU_Time
	{
	public:

		const unsigned long long& GetMyTimeInMicrosecond()const;
		const float GetMyTimeInMillisecond()const;
		const float GetMyTimeInSeconds() const;
		const float GetMyTimeInMinutes() const;
		const float GetMyTimeInHours() const;
		CU_Time ScaleTime(const long double &aTimeMultiplier) const;

		void SetMyTime(unsigned long long aTimeInMicroseconds);
		CU_Time(CU_Time const &aTime);

		~CU_Time();
		CU_Time(unsigned long long aTimeInMicrosecond = 0);

		CU_Time operator-(const CU_Time &aTime) const;
		CU_Time operator+=(const CU_Time &aTime);
		CU_Time operator=(const CU_Time &aTime);

		void AddTime(const CU_Time &aTime);

	private:
		unsigned long long myTimeMicrosecond;
	};

}

namespace CU = CommonUtilities;