#include "Engine_Precompiled.h"
#include "ParticleSystemFactory.h"

#include "WindowsFunctions.h"

#include "ParticleSystem.h"

namespace GraphicsEngine
{
	ParticleSystemFactory::ParticleSystemFactory()
	{
		myCreatedSystems.Init();
	}

	ParticleSystemFactory::~ParticleSystemFactory()
	{
	}

	void ParticleSystemFactory::LoadAll()
	{
		CU::GrowingArray<std::string> particleSystems = WF::GetFilesWithExtension("Data/XML/Particles/Systems/", ".xml");

		for (const std::string& aFileName : particleSystems)
		{
			if (aFileName != "")
			{
				std::string name = WF::GetFileNameWithoutExtension(aFileName);
				myParticleSystems[name] = new ParticleSystemData[1]();
				myParticleSystems[name]->LoadFromFile(aFileName);
			}
		}
	}

	CU::PoolPointer<ParticleSystem> ParticleSystemFactory::CreateParticleSystem(const std::string& aName)
	{
		if (CheckIfExists(aName) == true)
		{
			CU::PoolPointer<ParticleSystem> newSystem = myCreatedSystems.Allocate<ParticleSystem, ParticleSystem>();

			newSystem->Init(myParticleSystems[aName]);

			return newSystem;
		}

		return nullptr;
	}

	bool ParticleSystemFactory::CheckIfExists(const std::string& aName)
	{
		return myParticleSystems.find(aName) != myParticleSystems.end();
	}
}