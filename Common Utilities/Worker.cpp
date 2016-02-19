#include "Common Utilities_Precompiled.h"
#include "Worker.h"
#include "EventManager.h"

namespace CommonUtilities
{
	Worker::Worker()
	{
		myIsWorking = false;
		myQuit = false;
		myThread = new std::thread(&Worker::Start, std::ref(*this));
	}

	void Worker::Start()
	{
		std::unique_lock<std::mutex> lock(myMutex);
		CU::EventManager::GetInstance()->RegisterThread();

		while (myQuit == false)
		{
			myConditionVariable.wait_for(lock, std::chrono::milliseconds(1));

			if (myIsWorking == true)
			{
				myWork.DoWork();
				if (myQuit == false)
				{
					myIsWorking = false;
				}
			}
		}
		myConditionVariable._Unregister(myMutex);
		//myMutex.unlock();
	}


	const bool Worker::IsWorking() const
	{
		return myIsWorking;
	}

	bool Worker::AddWork(Work& someWork)
	{
		if (IsWorking() == false)
		{
			myWork = someWork;
			myConditionVariable.notify_one();
			myIsWorking = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Worker::Destroy()
	{
		if (myThread != nullptr)
		{
			myQuit = true;
			myConditionVariable.notify_all();
			myThread->join();
			delete myThread;
			myThread = nullptr;
		}
	}


	Worker::~Worker()
	{
		if (myThread != nullptr)
		{
			DL_ASSERT("Worker not destroyed.");
		}
	}
}