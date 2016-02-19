/*
Use this class to create and show a video
Limitations:
-Video must be in the windows media format (.wmv)
-No audio will be avalible at the moment (workaround: play a sound at the same time you play the video)
*/

#pragma once
#include <wtypes.h>

struct ID3D11ShaderResourceView;
class CWMVVideoInstance;
class CVideo
{
public:
	CVideo();
	~CVideo();
	void Init(const WCHAR* szFileName);
	void Play();
	void Stop();
	void Render();
	bool IsDone() const;
	ID3D11ShaderResourceView* GetView() const;
private:
	CWMVVideoInstance* myInstance;
};
