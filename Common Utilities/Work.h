#pragma once
#include <memory>
#include <functional>

namespace CommonUtilities
{
	class Work //Example for assign work function - auto RenderLamba = [&](){RenderFrame(); };
				
	{
	public:

		Work();
		virtual ~Work();

		Work(const std::function<void(void)>& aTaskFunction);

		void DoWork();

	private:

		std::function<void()> myTaskFunction;
	};
}

namespace CU = CommonUtilities;