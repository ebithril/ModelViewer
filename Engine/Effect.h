#pragma once
#include "../Common Utilities/Matrix.h"
#include "../Common Utilities/StaticArray.h"
#include "../Common Utilities/Observer.h"
#include "../Common Utilities/DataFile.h"
#include <string>
#include "TextureType.h"
#include "COMObjectPointer.h"

struct ID3D11ShaderResourceView;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectShaderResourceVariable;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11InputLayout;

namespace GraphicsEngine
{
	struct PerFrameCameraBuffer;
	struct PerObjectDataBuffer;
	struct PerFrameLightBuffer;
	struct PerObjectShadowBuffer;
	struct PerObjectLightBuffer;

	struct LightBufferData;
	
	struct OnResizeBuffer;

	class Texture;

	enum eVertexType;

	class Effect : public CU::Observer,  public CU::DataFile
	{
	public:
		Effect();
		~Effect();

		bool NeedNewFrameUpdate();
		bool NeedNewOnResizeUpdate();

		void SetTechnique(const std::string& aName);

		COMObjectPointer<ID3DX11Effect>& GetEffect();
		COMObjectPointer<ID3DX11EffectTechnique>& GetTechnique();
		ID3DX11EffectTechnique* GetNamedTechnique(const std::string& aName);
		COMObjectPointer<ID3D11InputLayout>& GetInputLayout();

		bool Init(const std::string& anEffectFile, eVertexType aVertexType, COMObjectPointer<ID3D11Device>& aDevice);
		bool ReInit();
		void InitBuffers();
		
		
		void OnResizeUpdate(const OnResizeBuffer& aResizeBuffer);
		void PerObjectUpdate(const PerObjectDataBuffer& aObjectDataBuffer);
		void PerFrameUpdate(const PerFrameCameraBuffer& aCameraBuffer, const PerFrameLightBuffer& aLightBuffer);
		void SetTextures(CU::StaticArray<Texture*, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)>& someTextures);

		void SetCubeMap(Texture* aCubeMap);
		void SetTextTexture(COMObjectPointer<ID3D11ShaderResourceView>& aView);

		void PerObjectShadowUpdate(const PerObjectShadowBuffer& aBuffer);
		void PerObjectLightBufferUpdate(const PerObjectLightBuffer& aBuffer);

		void SetIsUsingAlpha(bool aTruth);
		void SetIsUsingOneBitAlpha(bool aTruth);
		bool GetIsUsingAlpha() const;

		bool GetIsUsingOneBitAlpha() const;

	private:

		bool HandleNewFrameEvent(const CU::PoolPointer<CU::Event>& anEvent);
		bool HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent);

		bool myNeedNewFrameUpdate;
		bool myNeedOnResizeUpdate;
		bool myIsUsingAlpha;
		bool myIsUsingOneBitAlpha;

		eVertexType myVertexType;

		COMObjectPointer<ID3D11Device> myDevice;
		COMObjectPointer<ID3DX11Effect> myEffect;
		COMObjectPointer<ID3DX11EffectTechnique> myTechnique;
		COMObjectPointer<ID3D11Buffer> myObjectDataBuffer;
		COMObjectPointer<ID3D11Buffer> myCameraBuffer;
		COMObjectPointer<ID3D11Buffer> myDirectionalLightBuffer;
		COMObjectPointer<ID3D11Buffer> myOnResizeBuffer;
		COMObjectPointer<ID3D11Buffer> myPerObjectLightBuffer;

		COMObjectPointer<ID3D11Buffer> myObjectShadowBuffer;
		COMObjectPointer<ID3D11InputLayout> myInputLayout;
		CU::StaticArray<COMObjectPointer<ID3DX11EffectShaderResourceVariable>, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)> myShaderViews;

		void CreateBuffer(COMObjectPointer<ID3D11Buffer>& aBuffer, unsigned int aSizeInBytes);
		void UpdateShaderResource(COMObjectPointer<ID3D11Buffer>& someBuffer, const void* someBufferData, const char* aBufferName);

		virtual void OnFileChange(const std::string& aFileName) override;
	};
}
