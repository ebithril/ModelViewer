#pragma once

#include "CU_Time.h"
#include "ID.h"
#include <assert.h>
#include "Macros.h"

namespace CommonUtilities
{
	class Timer
	{
	public:

		Timer(const ID anID, CU_Time* aMasterDeltaTime);

		void Update();
		void SetPause(bool aPause);
		void ResetTimer();
		const CU_Time& GetMyTotalTime() const;
		const ID& GetMyID() const;
		void SetMyTimescale(float aTimeMultiplier);

		const bool GetMyIsPaused();

		Timer (const Timer &aTimer);
		~Timer();
		Timer();

	private:
		
		CU_Time *myMasterDeltaTime;
		CU_Time myTotalTime;

		ID myID;

		long double myTimescale;

		bool myIsPaused;

	};
}

namespace CU = CommonUtilities;