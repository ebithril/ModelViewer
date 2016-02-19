#include "Common Utilities_Precompiled.h"
#include "ThreadPool.h"

namespace CommonUtilities
{
	ThreadPool* ThreadPool::ourThreadPool = nullptr;

	void ThreadPool::Create()
	{
		if (ourThreadPool == nullptr)
		{
			ourThreadPool = new ThreadPool();
			ourThreadPool->Init();
		}
	}

	ThreadPool* ThreadPool::GetInstance()
	{
		return ourThreadPool;
	}

	void ThreadPool::Init()
	{
		myQuit = false;
#ifndef SINGLE_THREADED
		myWorkers.Init(10);

		myWorkPile1.Init(10);
		myWorkPile2.Init(10);

		myMasterThread = new std::thread(&ThreadPool::StartMasterThread, std::ref(*this));

		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			myWorkers.Add(new Worker());
		}
#endif
	}


	void ThreadPool::StartMasterThread()
	{
		myLastPileChecked1 = false;
		while (myQuit == false)
		{
			SwitchPilePointer();

			DeliverWork();

			UnlockMutex();

			//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}

	}

	void ThreadPool::SwitchPilePointer()
	{
		if (myLastPileChecked1 == true)
		{
			myLastPileChecked1 = false;
			if (myWorkPile2Mutex.try_lock() == true)
			{
				myCurrentWorkPile = &myWorkPile2;
			}
			else
			{
				myCurrentWorkPile = nullptr;
			}
		}
		else
		{
			myLastPileChecked1 = true;
			if (myWorkPile1Mutex.try_lock() == true)
			{
				myCurrentWorkPile = &myWorkPile1;
			}
			else
			{
				myCurrentWorkPile = nullptr;
			}
		}
	}

	void ThreadPool::DeliverWork()
	{
		if (myCurrentWorkPile != nullptr)
		{
			for (unsigned short workLoop = 0; workLoop < myCurrentWorkPile->Size(); workLoop++)
			{
				bool workDelivered = false;
				while (workDelivered == false)
				{
					for (unsigned short i = 0; i < myWorkers.Size(); i++)
					{
						if (myWorkers[i]->IsWorking() == false)
						{
							myWorkers[i]->AddWork((*myCurrentWorkPile)[workLoop]);
							workDelivered = true;
							break;
						}
					}
				}
			}
			myCurrentWorkPile->RemoveAll();
		}
	}

	void ThreadPool::UnlockMutex()
	{
		if (myCurrentWorkPile == &myWorkPile1)
		{
			myWorkPile1Mutex.unlock();
		}
		else if (myCurrentWorkPile == &myWorkPile2)
		{
			myWorkPile2Mutex.unlock();
		}
	}

	ThreadPool::ThreadPool()
	{
		myMasterThread = nullptr;
	}

	ThreadPool::~ThreadPool()
	{

	}

	void ThreadPool::AddWork(Work& someWork)
	{
#ifndef SINGLE_THREADED

		bool workAdded = false;
		while (workAdded == false)
		{
			if (myWorkPile1Mutex.try_lock() == true)
			{
				myWorkPile1.Add(someWork);
				myWorkPile1Mutex.unlock();
				workAdded = true;
			}
			else if (myWorkPile2Mutex.try_lock() == true)
			{
				myWorkPile2.Add(someWork);
				myWorkPile2Mutex.unlock();
				workAdded = true;
			}
		}

#else
		someWork.DoWork();
#endif
	}

	void ThreadPool::Destroy()
	{
		if (ourThreadPool != nullptr)
		{
			GetInstance()->myQuit = true;

			if (GetInstance()->myMasterThread != nullptr)
			{
				GetInstance()->myMasterThread->join();
				SAFE_DELETE(GetInstance()->myMasterThread);

				for (unsigned short i = 0; i < GetInstance()->myWorkers.Size(); i++)
				{
					GetInstance()->myWorkers[i]->Destroy();
				}

				GetInstance()->myWorkers.DeleteAll();
			}
		}	
	}
}