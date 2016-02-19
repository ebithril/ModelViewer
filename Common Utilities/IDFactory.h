#pragma once

#include "ID.h"
#include <assert.h>
#include <numeric>

namespace CommonUtilities
{
	class IDFactory
	{
	public:
		
		static ID CreateID();
		static ID CreateID(const std::string& aTextID);
		static void Create();
		static void Destroy();

	private:
		static unsigned int ourIDCounter;
		static IDFactory* ourIDFactory;

		IDFactory();
		~IDFactory();

	};
}
namespace CU = CommonUtilities;