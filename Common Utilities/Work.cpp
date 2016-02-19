#include "Common Utilities_Precompiled.h"
#include "Work.h"

namespace CommonUtilities
{
	Work::Work()
	{

	}

	Work::~Work()
	{

	}

	Work::Work(const std::function<void(void)>& aTaskFunction)

	{
		myTaskFunction = aTaskFunction;
	}

	void Work::DoWork()
	{
		if (myTaskFunction._Empty() == false)
		{
			myTaskFunction();
		}
	}


}