#pragma once

#define TYPE_TEMPLATE template<typename Type>

namespace CommonUtilities
{
	struct ObjectPoolData;

	class BaseObjectPool
	{
		TYPE_TEMPLATE
		friend class PoolPointer;
	public:
		
	protected:
		
		virtual	void* GetData(const unsigned int aMemoryIndex) = 0;
		virtual void DeAllocate(const ObjectPoolData& someData) = 0;

		inline BaseObjectPool();
		virtual inline ~BaseObjectPool();

	};

	BaseObjectPool::BaseObjectPool()
	{
	}

	BaseObjectPool::~BaseObjectPool()
	{
	}
}

#undef TYPE_TEMPLATE
namespace CU = CommonUtilities;