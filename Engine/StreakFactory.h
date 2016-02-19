#pragma once
#include "StreakEmitter.h"
#include "../Common Utilities/Map.h"
#include "../Common Utilities/ObjectPool.h"

#define NUMBER_OF_STREAK_EMITTERS 4096

namespace GraphicsEngine
{
	class StreakFactory
	{
	public:
		StreakFactory();
		~StreakFactory();

		void Init();

		CU::PoolPointer<StreakEmitter> CreateEmitter(const std::string& anEmitterName);

		const CU::GrowingArray<CU::PoolPointer<StreakEmitter>>& GetStreakEmitters() const;
		
		void Test();

		void LoadEmitters();

	private:
		CU::Map<std::string, SharedPointer<StreakEmitterData>> myStreakData;
		CU::GrowingArray<std::string> myStreakDataNames;
		CU::GrowingArray<CU::PoolPointer<StreakEmitter>> myEmitterPointers;
		CU::ObjectPool<NUMBER_OF_STREAK_EMITTERS * sizeof(StreakEmitter)> myEmitters;
	};
}

namespace GE = GraphicsEngine;