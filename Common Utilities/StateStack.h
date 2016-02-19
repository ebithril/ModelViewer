#pragma once
#include "../Common Utilities/GrowingArray.h"

namespace CommonUtilities
{
	class State;

	class StateStack
	{
	public:
		StateStack();
		~StateStack();
		void Render();
		void Update();
		void Pop();
		void PopMain();
		void Push(State* aState);
		void PushMain(State* aState);
		void PopMainAndPush(State* aState);
		unsigned short GetMainStateCount();
	private:

		CU::GrowingArray<State*> myMainStates;
	};
}

namespace CU = CommonUtilities;