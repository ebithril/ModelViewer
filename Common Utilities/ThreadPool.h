#pragma once

#include "GrowingArray.h"
#include "Work.h"
#include "Worker.h"
#include <mutex>
#include <thread>
#include <functional>

namespace CommonUtilities
{
	class ThreadPool
	{
	public:

		~ThreadPool();
		static void Create();
		void AddWork(Work& someWork);
		static void Destroy();

		static ThreadPool* GetInstance();

	private:
		ThreadPool();
		void SwitchPilePointer();
		void DeliverWork();
		void UnlockMutex();
		void Init();
	
		static ThreadPool* ourThreadPool;

		ThreadPool(const ThreadPool& aThreadManager) = delete;
		void StartMasterThread();

		std::thread* myMasterThread;

		CU::GrowingArray<Work>* myCurrentWorkPile;
		CU::GrowingArray<Work>* myCurrentAddWorkPile;
						
		CU::GrowingArray<Work> myWorkPile1;
		CU::GrowingArray<Work> myWorkPile2;
		CU::GrowingArray<Worker*> myWorkers;

		std::mutex myWorkPile1Mutex;
		std::mutex myWorkPile2Mutex;
		bool myLastPileChecked1;
		volatile bool myQuit;

	};

}

namespace CU = CommonUtilities;
