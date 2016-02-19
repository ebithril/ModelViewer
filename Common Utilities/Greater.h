#pragma once

#define GREATER_TEMPLATE template<typename Type>
#define GREATER_OBJECT Greater<Type>

namespace CommonUtilities
{

	GREATER_TEMPLATE
	class Greater
	{
	public:
		const bool operator()(const Type& someLeftData, const Type& someRightData) const;

	};

	GREATER_TEMPLATE
		const bool GREATER_OBJECT::operator()(const Type& someLeftData, const Type& someRightData) const
	{
			if (someLeftData > someRightData)
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

#undef GREATER_TEMPLATE
#undef GREATER_OBJECT