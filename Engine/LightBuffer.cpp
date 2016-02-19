#include "Engine_Precompiled.h"
#include "LightBuffer.h"

namespace GraphicsEngine
{
	LightBuffer::LightBuffer()
	{
		myCurrentReadBufferIs1 = true;
		myFirstFrame = true;
		myLightData1.Init(256);
		myLightData2.Init(256);

	}


	LightBuffer::~LightBuffer()
	{
	}

	void LightBuffer::SwitchBuffer()
	{
		myCurrentReadBufferIs1 = !myCurrentReadBufferIs1;

		if (myCurrentReadBufferIs1 == true)
		{
			myLightData2.RemoveAll();
			myDirectionalShadowBuffer2.Clear();
		}
		else
		{
			myLightData1.RemoveAll();
			myDirectionalShadowBuffer1.Clear();
		}
	}

	const LightShaderData& LightBuffer::GetLightWithIndex(const unsigned short anIndex) const
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myLightData1[anIndex];
		}
		else
		{
			return myLightData2[anIndex];
		}
	}

	const CU::GrowingArray<LightShaderData>& LightBuffer::GetReadLightDataBuffer() const
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myLightData1;
		}
		else
		{
			return myLightData2;
		}
	}

	void LightBuffer::CreateShadowCamera(const Camera& aCamera)
	{
		GetWriteDirectionalLightShadowBuffer().InitDirectionalLightShadow(GetDirectionalLight().myDirection, aCamera);
	}

	void LightBuffer::RenderShadowCamera()
	{
		GetWriteDirectionalLightShadowBuffer().Render();
	}

	ShadowBuffer& LightBuffer::GetWriteDirectionalLightShadowBuffer()
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myDirectionalShadowBuffer2;
		}
		else
		{
			return myDirectionalShadowBuffer1;
		}
	}

	const ShadowBuffer& LightBuffer::GetReadDirectionalLightShadowBuffer() const
	{
		if (myCurrentReadBufferIs1 == false)
		{
			return myDirectionalShadowBuffer2;
		}
		else
		{
			return myDirectionalShadowBuffer1;
		}
	}

	bool LightBuffer::IsFirstFrame() const
	{
		return myFirstFrame;
	}

	void LightBuffer::SetIsFirstFrame(const bool aBool)
	{
		myFirstFrame = aBool;
	}

	const unsigned short LightBuffer::GetNextIndex() const
	{
		return GetWriteLightDataBuffer().Size();
	}

	const LightShaderData& LightBuffer::GetDirectionalLight() const
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myDirectionalLightData1;
		}
		else
		{
			return myDirectionalLightData2;
		}
	}

	LightShaderData& LightBuffer::GetPrivateDirectionalLight()
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myDirectionalLightData1;
		}
		else
		{
			return myDirectionalLightData2;
		}
	}

	CU::GrowingArray<LightShaderData>& LightBuffer::GetWriteLightDataBuffer()
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myLightData2;
		}
		else
		{
			return myLightData1;
		}
	}

	const CU::GrowingArray<LightShaderData>& LightBuffer::GetWriteLightDataBuffer() const
	{
		if (myCurrentReadBufferIs1 == true)
		{
			return myLightData2;
		}
		else
		{
			return myLightData1;
		}
	}

	void LightBuffer::SetDirectionalLight(const LightShaderData& someData)
	{
		GetPrivateDirectionalLight() = someData;
	}


	void LightBuffer::AddLightShaderData(const LightShaderData& someData)
	{
		GetWriteLightDataBuffer().Add(someData);
	}

	void LightBuffer::AddShadowData(bool aIsDirectionalLight, const GE::ShadowRenderData& someData)
	{
		if (aIsDirectionalLight == true)
		{
			GetWriteDirectionalLightShadowBuffer().AddShadowData(someData);
		}
	}

}

