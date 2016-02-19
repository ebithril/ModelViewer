#pragma once

#define LESSER_TEMPLATE template<typename Type>
#define LESSER_OBJECT Lesser<Type>

namespace CommonUtilities
{
	LESSER_TEMPLATE
	class Lesser
	{
	public:

		const bool operator()(const Type& someLeftData, const Type& someRightData) const;

	private:
	};

	LESSER_TEMPLATE
		const bool LESSER_OBJECT::operator()(const Type& someLeftData, const Type& someRightData) const
	{
			if (someLeftData < someRightData)
			{
				return true;
			}
			else
			{
				return false;
			}
	}

}

namespace CU = CommonUtilities;

#undef LESSER_TEMPLATE
#undef LESSER_OBJECT