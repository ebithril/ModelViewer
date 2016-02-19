#pragma once

#include "../Common Utilities/Event.h"
#include "../Common Utilities/Vector.h"

namespace GraphicsEngine
{
	class FadeInEvent : public CU::Event
	{
	public:
		FadeInEvent(const Vector3f& aColor, float aFadeTime) : Event(CU::eEvent::FADE_IN) { myColor = aColor; myFadeTime = aFadeTime; };
		virtual ~FadeInEvent(){};
		Vector3f myColor;
		float myFadeTime;
	};

	class FadeOutEvent : public CU::Event
	{
	public:
		FadeOutEvent(const Vector3f& aColor, float aFadeTime) : Event(CU::eEvent::FADE_OUT) { myColor = aColor; myFadeTime = aFadeTime; };
		virtual ~FadeOutEvent(){};
		Vector3f myColor;
		float myFadeTime;
	};
}