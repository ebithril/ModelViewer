#include "Common Utilities_Precompiled.h"
#include "IDFactory.h"

namespace CommonUtilities
{

	unsigned int IDFactory::ourIDCounter  = 0;
	IDFactory* IDFactory::ourIDFactory = nullptr;

	IDFactory::IDFactory()
	{
		ourIDCounter = 0;
		ourIDFactory = nullptr;
	}

	void IDFactory::Create()
	{
		assert(ourIDFactory == nullptr && "IDFactory already initialized.");
		ourIDFactory = new IDFactory();
	}

	void IDFactory::Destroy()
	{
		if (ourIDFactory != nullptr)
		{
			delete ourIDFactory;
			ourIDFactory = nullptr;
		}
	}

	ID CU::IDFactory::CreateID()
	{
		assert(ourIDCounter != UINT_MAX && "ID number reached it's limit.");
		assert(ourIDFactory != nullptr && "IDFactory not initialized.");
		return ID(ourIDCounter++);
	}

	ID CU::IDFactory::CreateID(const std::string& aTextID)
	{
		assert(ourIDCounter != UINT_MAX && "ID number reached it's limit.");
		assert(ourIDFactory != nullptr && "IDFactory not initialized.");
		return ID(ourIDCounter++, aTextID);
	}

	IDFactory::~IDFactory()
	{
	}

}