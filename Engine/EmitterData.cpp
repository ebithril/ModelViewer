#include "Engine_Precompiled.h"
#include "EmitterData.h"
#include "ParticleData.h"
#include "TextureFactory.h"
#include "EffectFactory.h"
#include <D3D11.h>
#include "Effect.h"
#include "../Common Utilities/XMLReader.h"
#include "../Common Utilities/file_watcher.h"

namespace GraphicsEngine
{
	EmitterData::EmitterData()
	{
		myParticleData = nullptr;
	}

	EmitterData::~EmitterData()
	{
		SAFE_DELETE(myParticleData);
	}

	void EmitterData::LoadFile(const std::string& aFileName)
	{
		CU::CFileWatcher::GetInstance()->WatchFileChange(aFileName, [this](const std::string& fileName){this->Load(fileName); });

		Load(aFileName);
	}

	void EmitterData::Load(const std::string& aFileName)
	{
		SAFE_DELETE(myParticleData);
		XMLReader file;
		file.OpenDocument(aFileName);

		XMLElement element = file.FindFirstChild();

		XMLElement emitter = element->FirstChildElement("emitter");

		if (emitter != nullptr)
		{

			XMLElement emitterData = emitter->FirstChildElement("emitterData");

			myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("ParticleShader");

			myParticleInputLayout = myEffect->GetInputLayout();

			if (emitterData != nullptr)
			{
				myTexture = GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture(emitterData->FirstChildElement("texture")->GetText());

				myEmissionExtents.x = emitterData->FirstChildElement("emissionExtents")->FloatAttribute("x");
				myEmissionExtents.y = emitterData->FirstChildElement("emissionExtents")->FloatAttribute("y");
				myEmissionExtents.z = emitterData->FirstChildElement("emissionExtents")->FloatAttribute("z");

				myEmissionVelocityDelta.x = emitterData->FirstChildElement("emissionVelocityDelta")->FloatAttribute("x");
				myEmissionVelocityDelta.y = emitterData->FirstChildElement("emissionVelocityDelta")->FloatAttribute("y");
				myEmissionVelocityDelta.z = emitterData->FirstChildElement("emissionVelocityDelta")->FloatAttribute("z");

				emitterData->FirstChildElement("emissionLifeTime")->QueryFloatText(&myEmissionLifeTime);

				emitterData->FirstChildElement("emissionRatePerSecond")->QueryFloatText(&myEmissionRatePerSecond);
			}

			XMLElement particleData = emitter->FirstChildElement("particleData");

			if (particleData != nullptr)
			{
				myParticleData = new ParticleData();

				myParticleData->myMaxVelocity.x = particleData->FirstChildElement("maxVelocity")->FloatAttribute("x");
				myParticleData->myMaxVelocity.y = particleData->FirstChildElement("maxVelocity")->FloatAttribute("y");
				myParticleData->myMaxVelocity.z = particleData->FirstChildElement("maxVelocity")->FloatAttribute("z");

				myParticleData->myMinVelocity.x = particleData->FirstChildElement("minVelocity")->FloatAttribute("x");
				myParticleData->myMinVelocity.y = particleData->FirstChildElement("minVelocity")->FloatAttribute("y");
				myParticleData->myMinVelocity.z = particleData->FirstChildElement("minVelocity")->FloatAttribute("z");

				particleData->FirstChildElement("deAcceleration")->QueryFloatText(&myParticleData->myDeAcceleration);

				particleData->FirstChildElement("maxRotation")->QueryFloatText(&myParticleData->myMaxRotation);

				particleData->FirstChildElement("minRotation")->QueryFloatText(&myParticleData->myMinRotation);

				particleData->FirstChildElement("lifeTime")->QueryFloatText(&myParticleData->myLifeTime);

				myParticleData->myStartScale.x = particleData->FirstChildElement("startScale")->FloatAttribute("x");
				myParticleData->myStartScale.y = particleData->FirstChildElement("startScale")->FloatAttribute("y");

				myParticleData->myScaleDelta.x = particleData->FirstChildElement("scaleDelta")->FloatAttribute("x");
				myParticleData->myScaleDelta.y = particleData->FirstChildElement("scaleDelta")->FloatAttribute("y");

				particleData->FirstChildElement("startAlpha")->QueryFloatText(&myParticleData->myStartAlpha);

				particleData->FirstChildElement("deltaAlpha")->QueryFloatText(&myParticleData->myDeltaAlpha);

				particleData->FirstChildElement("isAffectedByGravity")->QueryBoolText(&myParticleData->myIsAffectedByGravity);
			}

			myMaxNumberOfParticles = 2000;//unsigned short(myEmissionRatePerSecond * myParticleData->myLifeTime) + 1;
		}
	}

}