#include "Engine_Precompiled.h"
#include "CubeMapGenerator.h"
#include "SceneRenderData.h"
#include "../Common Utilities/EventManager.h"
#include "Engine.h"
#include "DirectXEngine.h"

namespace GraphicsEngine
{
	CubeMapGenerator::CubeMapGenerator()
	{
	}


	CubeMapGenerator::~CubeMapGenerator()
	{
	}

	Texture CubeMapGenerator::GenerateCubeMap(const Vector3<float>& aPosition)
	{
		Texture cubeMap(GE::Engine::GetInstance()->GetEngine(), { 512, 512 }, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false, false, true);
		Texture depth(GE::Engine::GetInstance()->GetEngine(), { 512, 512 }, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, false, true, false);

		GE::Camera cubeCamera;
		cubeCamera.Init({ 512, 512 }, aPosition, 0.1f, 1000, PI *0.5f, false);

		CU::RenderBuffer& renderBuffer = CU::EventManager::GetInstance()->GetRenderBuffer();
		CU::GrowingArray<CU::ScenePrioID> mySceneIDs = CU::EventManager::GetInstance()->GetRenderBuffer().GetSceneIDInRenderOrder();

		CU::StaticArray<Matrix44f, 6> cameraRotations;
		cameraRotations[0] = Matrix44f::CreateRotateAroundY(PI * 0.5f);
		cameraRotations[1] = Matrix44f::CreateRotateAroundY(-PI * 0.5f);
		cameraRotations[2] = Matrix44f::CreateRotateAroundX(PI * 0.5f);
		cameraRotations[3] = Matrix44f::CreateRotateAroundX(-PI * 0.5f);
		cameraRotations[4] = Matrix44f();
		cameraRotations[5] = Matrix44f::CreateRotateAroundY(PI);

		CU::GrowingArray<Camera> oldCameras(8);


		for (unsigned short i = 0; i < mySceneIDs.Size(); i++)
		{
			oldCameras.Add(renderBuffer.GetRenderCommands(mySceneIDs[i].myID).mySceneData->myCamera);
		}

		
			Texture renderTargetView;
			for (unsigned short o = 0; o < 6; o++)
			{
				cubeCamera.SetOrientation(cameraRotations[o]);
				cubeCamera.SetPosition(aPosition);

				for (unsigned short c = 0; c < mySceneIDs.Size(); c++)
				{
					renderBuffer.GetRenderCommands(mySceneIDs[c].myID).mySceneData->myCamera = cubeCamera;
				}

				renderTargetView.SetRenderTargetView(cubeMap.GetRenderTargetView(o).Get(), { 512, 512 });
				GE::Engine::GetInstance()->GetEngine()->RenderScenes(renderTargetView, depth, false);
			}

		return cubeMap;
	}

}