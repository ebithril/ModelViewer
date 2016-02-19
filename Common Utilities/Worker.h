#pragma once
#include "Work.h"
#include <condition_variable>
#include <thread>

namespace CommonUtilities
{
	class Worker
	{
	public:

		Worker();
		~Worker();
		void Destroy();

		const bool IsWorking() const;
		bool AddWork(Work& someWork);

	private:

		std::thread* myThread;
		void Start();
		Work myWork;
		volatile bool myQuit;
		volatile bool myIsWorking;

		std::condition_variable myConditionVariable;
		std::mutex myMutex;
	};

}

namespace CU = CommonUtilities;