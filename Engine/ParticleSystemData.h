#pragma once

#include "EmitterData.h"
#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/Matrix44.h"

#include "../Common Utilities/SharedPointer.h"

namespace GraphicsEngine
{
	class ParticleSystem;

	class ParticleSystemData
	{
		friend ParticleSystem;
	public:
		ParticleSystemData();
		~ParticleSystemData();

		void LoadFromFile(const std::string& aFilePath);
	private:
		void Load(const std::string& aFilePath);


		CU::GrowingArray<std::string> myEmitterData;
		CU::GrowingArray<Matrix44<float>> myOrientations;
		CU::GrowingArray<float> myTimeStamps;
	};
}

namespace GE = GraphicsEngine;