#pragma once
#include "ParticleEmitter.h"
#include <string>
#include "../Common Utilities/Map.h"
#include "../Common Utilities/PoolPointer.h"
#include "../Common Utilities/ObjectPool.h"

namespace GraphicsEngine
{
	class EmitterData;

	class EmitterFactory
	{
	public:

		EmitterFactory();
		~EmitterFactory();

		void Init();

		CU::PoolPointer<ParticleEmitter> CreateEmitter(const std::string& anEmitterName);

		void LoadEmitters();

		SharedPointer<EmitterData> GetEmitterData(const std::string& aEmitterName);
	private:
		void CreateDefaultEmitter(const std::string& aName, const std::string& aTexture = "White");
		void CreateEmitterViaXML(const std::string& aName);

		bool CheckIfExists(const std::string& aEmitterName);

		CU::Map<std::string, SharedPointer<EmitterData>> myEmitterData;
		CU::GrowingArray<std::string> myEmitterDataNames;
		CU::GrowingArray<CU::PoolPointer<ParticleEmitter>> myEmitterPointers;
		CU::ObjectPool<1024 * sizeof(ParticleEmitter)> myEmitters;
	};
}

namespace GE = GraphicsEngine;