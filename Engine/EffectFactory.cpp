#include "Engine_Precompiled.h"
#include "EffectFactory.h"
#include "Engine.h"
#include "d3dx11effect.h"
#include "Effect.h"
#include "VertexDataWrapper.h"
#include "WindowsFunctions.h"

namespace GraphicsEngine
{
	EffectFactory::EffectFactory()
	{
	}


	EffectFactory::~EffectFactory()
	{
		DeleteAll();
	}

	void EffectFactory::Init(COMObjectPointer<ID3D11Device>& aDevice)
	{
		myDevice = aDevice;
		myEffectNames.Init(8);
		CreateEffect("PostProcessShader", "Data/Shaders/PostProcessShader.fso", GE::eVertexType::VERTEX_POS_UV);
		CreateEffect("ComputeShader", "Data/Shaders/ComputeShader.fso", GE::eVertexType::NONE);
	}

	Effect* EffectFactory::GetEffect(const std::string& anEffectName)
	{
		if (myEffects.KeyExists(anEffectName))
		{
			return myEffects.Get(anEffectName);
		}
		else
		{
			std::string errorText = "Effect with name " + anEffectName + " doesn't exist in effect factory.";
			DL_ASSERT(errorText.c_str());
			return nullptr;
		}
	}

	Effect* EffectFactory::CreateEffect(const std::string anEffectName, const std::string aFileName, eVertexType aVertexType, const bool anIsUsingAlpha)
	{
		Effect* newEffect = new Effect();
		newEffect->Init(aFileName, aVertexType, myDevice);
		newEffect->SetIsUsingAlpha(anIsUsingAlpha);
		myEffects.Insert(anEffectName, newEffect);
		myEffectNames.Add(anEffectName);
		return newEffect;
	}

	void EffectFactory::CreateAllEffects()
	{
		CU::GrowingArray<std::string> files = WF::GetFilesWithExtension("Data\\Shaders\\", ".fso");

		for (unsigned short i = 0; i < files.Size(); i++)
		{
			std::string fileName = WF::GetFileNameWithoutExtension(files[i]);

			if (fileName != "Default" && fileName != "VertexPosNormUVBiTanInclude" && fileName != "ParticleShader")
			{
				CreateEffect(fileName, files[i], eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN);
			}
			else if (fileName == "ParticleShader")
			{
				CreateEffect(fileName, files[i], eVertexType::VERTEX_POS_VELOCITY_SIZE_ALPHA_TIME);
			}
		}
	}

	void EffectFactory::DeleteAll()
	{
		for (unsigned short i = 0; i < myEffectNames.Size(); i++)
		{
			SAFE_DELETE(myEffects[myEffectNames[i]]);
		}
	}
}


