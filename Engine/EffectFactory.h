#pragma once
#include "../Common Utilities/Map.h"
#include "COMObjectPointer.h"

struct ID3D11Device;

namespace GraphicsEngine
{
	enum eVertexType;
	class Effect;

	class EffectFactory
	{
	public:
		EffectFactory();
		~EffectFactory();
		void Init(COMObjectPointer<ID3D11Device>& aDevice);

		Effect* GetEffect(const std::string& anEffectName);
		Effect* CreateEffect(const std::string anEffectName, const std::string aFileName, eVertexType aVertexType, const bool anIsUsingAlpha = false);

		void CreateAllEffects();

	private:
		void DeleteAll();

		COMObjectPointer<ID3D11Device> myDevice;
		CU::Map<std::string, Effect*> myEffects;
		CU::GrowingArray<std::string> myEffectNames;
	};
}
namespace GE = GraphicsEngine;