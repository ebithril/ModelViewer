#pragma once
#include "VertexBufferWrapper.h"
#include "COMObjectPointer.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11InputLayout;

namespace CommonUtilities
{
	struct RenderCommandLine;
}

namespace CU = CommonUtilities;

namespace GraphicsEngine
{
	class DirectXEngine;
	class Camera;

	class LineRenderer
	{
	public:
		LineRenderer();
		~LineRenderer();

		void Init(DirectXEngine* anEngine);

		void DrawLines(CU::GrowingArray<CU::RenderCommandLine, unsigned int>& someLines);

		void UpdateLineShader(const Camera& aCamera);
		

	private:

		void LineSetup();

		DirectXEngine* myEngine;

		COMObjectPointer<ID3DX11Effect> myLineEffect;
		COMObjectPointer<ID3DX11EffectTechnique> myLineTechnique;

		COMObjectPointer<ID3DX11EffectMatrixVariable> myLineCameraMatrix;
		COMObjectPointer<ID3DX11EffectMatrixVariable> myLineProjectionMatrix;
		VertexBufferWrapper myLineDummyBuffer;

		COMObjectPointer<ID3D11InputLayout> myLineInputLayout;
	};
}

namespace GE = GraphicsEngine;