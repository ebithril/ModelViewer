#pragma once
#include "LightShaderData.h"
#include "../Common Utilities/GrowingArray.h"
#include "ShadowBuffer.h"
#include "ShadowRenderData.h"

namespace GraphicsEngine
{
	class Camera;

	class LightBuffer
	{
	public:

		LightBuffer();
		~LightBuffer();

		void SwitchBuffer();
		
		void SetDirectionalLight(const LightShaderData& someData);
		void AddLightShaderData(const LightShaderData& someData);

		//Adds shadow data to current light being processed
		void AddShadowData(bool aIsDirectionalLight ,const GE::ShadowRenderData& someData);

		const unsigned short GetNextIndex() const;

		const LightShaderData& GetDirectionalLight()const;
		const LightShaderData& GetLightWithIndex(const unsigned short anIndex) const;

		const CU::GrowingArray<LightShaderData>& GetReadLightDataBuffer() const;

		void CreateShadowCamera(const Camera& aCamera);

		void RenderShadowCamera();

		ShadowBuffer& GetWriteDirectionalLightShadowBuffer();
		const ShadowBuffer& GetReadDirectionalLightShadowBuffer()const;

		bool IsFirstFrame() const;
		void SetIsFirstFrame(const bool aBool);

	private:

		LightShaderData& GetPrivateDirectionalLight();

		CU::GrowingArray<LightShaderData>& GetWriteLightDataBuffer();
		const CU::GrowingArray<LightShaderData>& GetWriteLightDataBuffer() const;

		volatile bool myCurrentReadBufferIs1;
		volatile bool myFirstFrame;

		ShadowBuffer myDirectionalShadowBuffer1;
		ShadowBuffer myDirectionalShadowBuffer2;

		LightShaderData myDirectionalLightData1;
		LightShaderData myDirectionalLightData2;

		CU::GrowingArray<LightShaderData> myLightData1;
		CU::GrowingArray<LightShaderData> myLightData2;


	};
}

namespace GE = GraphicsEngine;