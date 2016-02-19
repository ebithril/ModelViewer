#include "Engine_Precompiled.h"
#include "ParticleSystemData.h"

#include "../Common Utilities/XMLReader.h"

#include "EmitterFactory.h"

#include "../Common Utilities/file_watcher.h"

namespace GraphicsEngine
{
	ParticleSystemData::ParticleSystemData()
	{
	}

	ParticleSystemData::~ParticleSystemData()
	{
	}

	void ParticleSystemData::LoadFromFile(const std::string& aFilePath)
	{
		CU::CFileWatcher::GetInstance()->WatchFileChange(aFilePath, [this](const std::string& fileName){this->Load(fileName); });

		Load(aFilePath);
	}

	void ParticleSystemData::Load(const std::string& aFilePath)
	{
		myEmitterData.Init(4);
		myOrientations.Init(4);
		myTimeStamps.Init(4);

		XMLReader xmlDoc;
		xmlDoc.OpenDocument(aFilePath);

		XMLElement emitterElement = xmlDoc.FindFirstChild();

		emitterElement = emitterElement->FirstChildElement("Emitter");

		while (emitterElement != nullptr)
		{
			XMLElement dataElement = emitterElement->FirstChildElement("Name");

			std::string emitterName = dataElement->GetText();

			dataElement = emitterElement->FirstChildElement("Time");

			float time;
			dataElement->QueryFloatText(&time);

			dataElement = emitterElement->FirstChildElement("Position");

			Vector3<float> position;

			position.x = dataElement->FloatAttribute("x");
			position.y = dataElement->FloatAttribute("y");
			position.z = dataElement->FloatAttribute("z");

			dataElement = emitterElement->FirstChildElement("Rotation");

			Vector3<float> rotation;

			rotation.x = dataElement->FloatAttribute("x");
			rotation.y = dataElement->FloatAttribute("y");
			rotation.z = dataElement->FloatAttribute("z");

			Matrix44<float> orientation;

			orientation *= Matrix44<float>::CreateRotateAroundX(rotation.x);
			orientation *= Matrix44<float>::CreateRotateAroundY(rotation.y);
			orientation *= Matrix44<float>::CreateRotateAroundZ(rotation.z);

			orientation.SetTranslation(position);

			myEmitterData.Add(emitterName);
			myOrientations.Add(orientation);
			myTimeStamps.Add(time);

			emitterElement = emitterElement->NextSiblingElement("Emitter");
		}
	}
}