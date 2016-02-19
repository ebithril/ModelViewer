#include "Engine_Precompiled.h"
#include "Video.h"
#include "WMVVideoInstance.h"

CVideo::CVideo()
{
}


CVideo::~CVideo()
{
	SAFE_DELETE(myInstance);
}

void CVideo::Init(const WCHAR* szFileName)
{
	myInstance = new CWMVVideoInstance();
	myInstance->Init(szFileName);
}

void CVideo::Play()
{
	myInstance->Play();
}

void CVideo::Stop()
{
	myInstance->Stop();
}

void CVideo::Render()
{
	myInstance->Render();
}

bool CVideo::IsDone() const
{
	return myInstance->IsDone();
}

ID3D11ShaderResourceView* CVideo::GetView() const
{
	return myInstance->GetView();
}

