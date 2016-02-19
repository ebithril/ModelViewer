#pragma once

namespace CommonUtilities
{
	class StateStack;

	class State
	{
	public:

		State();
		virtual ~State();
		void Pop();
		void PopAllChildren();
		void Push(State* aState);
		void SetStateStack(StateStack* aStateStack);

		void SetHandleEvents(bool aFlag);

		bool Update();
		bool Render();

		virtual bool StateUpdateFunction() = 0;
		virtual bool StateRenderFunction() = 0;
	protected:

		bool UpdateUntilTop();
		bool UpdateUntilLowest();
		bool RenderUntilTop();
		bool RenderUntilLowest();

		bool myRenderParent;
		bool myUpdateParent;
		bool myHandleEvents;

		State* myParentState;
		State* myChildState;
		StateStack* myStateStack;
	};
}

namespace CU = CommonUtilities;