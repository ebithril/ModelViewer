#pragma once

#include "ParticleSystemData.h"

#include "../Common Utilities/SharedPointer.h"
#include "../Common Utilities/ObjectPool.h"
#include "../Common Utilities/PoolPointer.h"

#include "ParticleSystem.h"

#include <unordered_map>

namespace GraphicsEngine
{
	class ParticleSystemFactory
	{
	public:
		ParticleSystemFactory();
		~ParticleSystemFactory();

		void LoadAll();

		CU::PoolPointer<ParticleSystem> CreateParticleSystem(const std::string& aName);

	private:
		bool CheckIfExists(const std::string& aName);

		std::unordered_map<std::string, SharedPointer<ParticleSystemData>> myParticleSystems;
		CU::ObjectPool<sizeof(ParticleSystem) * 1024> myCreatedSystems;
	};
}

namespace GE = GraphicsEngine;