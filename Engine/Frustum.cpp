#include "Engine_Precompiled.h"
#include "Frustum.h"
#include "../Common Utilities/RenderCommandLine.h"
#include <cmath>
#include "../Common Utilities/Intersection.h"
#include "../Common Utilities/EventManager.h"

enum class eFRUSTUMSIDES
{
	eRIGHT = 0,
	eLEFT,
	eUP,
	eDOWN,
	eFAR,
	eNEAR,
};

namespace GraphicsEngine
{
	Frustum::Frustum()
	{
	}

	Frustum::~Frustum()
	{
	}

	void Frustum::Init(float aFar, float aNear)
	{
		myFar = aFar;
		myNear = aNear;

		myRadius = Length(Vector3f(myFar / 2.f, myFar / 2.f, myFar / 2.f));
		myLines.Init(12);
	}

	void Frustum::Update(const Matrix44<float>& aCameraOrientation, float aAspectRatio, float aFov)
	{
		float Hnear = 2.f * tan(aFov / 2.f) * myNear;
		float Wnear = Hnear * aAspectRatio;

		myPosition = aCameraOrientation.GetTranslation();
		Vector3f rightVector = GetNormalized(aCameraOrientation.GetRightVector());
		Vector3f forwardVector = GetNormalized(aCameraOrientation.GetForwardVector());
		Vector3f upVector = GetNormalized(aCameraOrientation.GetUpVector());
		
		Vector3f nc = myPosition + forwardVector * myNear;

		Vector3f ntl = nc + (upVector * Hnear / 2.f) - (rightVector * Wnear / 2.f);
		Vector3f ntr = nc + (upVector * Hnear / 2.f) + (rightVector * Wnear / 2.f);
		Vector3f nbl = nc - (upVector * Hnear / 2.f) - (rightVector * Wnear / 2.f);
		Vector3f nbr = nc - (upVector * Hnear / 2.f) + (rightVector * Wnear / 2.f);

		float Hfar = 2.f * tan(aFov / 2.f) * myFar;
		float Wfar = Hfar * aAspectRatio;

		Vector3f fc = myPosition + forwardVector * myFar;

		Vector3f ftl = fc + (upVector * Hfar / 2.f) - (rightVector * Wfar / 2.f);
		Vector3f ftr = fc + (upVector * Hfar / 2.f) + (rightVector * Wfar / 2.f);
		Vector3f fbl = fc - (upVector * Hfar / 2.f) - (rightVector * Wfar / 2.f);
		Vector3f fbr = fc - (upVector * Hfar / 2.f) + (rightVector * Wfar / 2.f);

		myPointPositions[0] = ntl;
		myPointPositions[1] = ntr;
		myPointPositions[2] = nbl;
		myPointPositions[3] = nbr;
		myPointPositions[4] = ftl;
		myPointPositions[5] = ftr;
		myPointPositions[6] = fbl;
		myPointPositions[7] = fbr;

		Plane<float> plane;
		plane.InitWith3Points(myPosition, nbr, ntr);
		myPlanes[int(eFRUSTUMSIDES::eRIGHT)] = plane;

		plane.InitWith3Points(myPosition, ntl, nbl);
		myPlanes[int(eFRUSTUMSIDES::eLEFT)] = plane;

		plane.InitWith3Points(myPosition, ntr, ntl);
		myPlanes[int(eFRUSTUMSIDES::eUP)] = plane;

		plane.InitWith3Points(myPosition, nbl, nbr);
		myPlanes[int(eFRUSTUMSIDES::eDOWN)] = plane;

		plane.InitWithPointAndNormal(myPosition + (forwardVector * myFar), forwardVector);
		myPlanes[int(eFRUSTUMSIDES::eFAR)] = plane;

		plane.InitWithPointAndNormal(myPosition + (forwardVector * myNear), (forwardVector * -1.f));
		myPlanes[int(eFRUSTUMSIDES::eNEAR)] = plane;

		myPosition += forwardVector * (myFar / 2.f);

		myLines.RemoveAll();

		//Near Plane

		myLines.Add(CU::RenderCommandLine(ntl, ntr));
		myLines.Add(CU::RenderCommandLine(ntr, nbr));
		myLines.Add(CU::RenderCommandLine(nbr, nbl));
		myLines.Add(CU::RenderCommandLine(nbl, ntl));

		//Far plane

		myLines.Add(CU::RenderCommandLine(ftl, ftr));
		myLines.Add(CU::RenderCommandLine(ftr, fbr));
		myLines.Add(CU::RenderCommandLine(fbr, fbl));
		myLines.Add(CU::RenderCommandLine(fbl, ftl));

		//Sides

		myLines.Add(CU::RenderCommandLine(ntl, ftl));
		myLines.Add(CU::RenderCommandLine(ntr, ftr));
		myLines.Add(CU::RenderCommandLine(nbr, fbr));
		myLines.Add(CU::RenderCommandLine(nbl, fbl));
	}

	bool Frustum::CheckIfSphereInside(const Vector3<float>& aPosition, float aRadius) const
	{
		if (Length2(aPosition - myPosition) < aRadius * aRadius + myRadius* myRadius)
		{

			for (int i = 0; i < 6; i++)
			{
				if (myPlanes[i].Inside(aPosition, aRadius) == false)
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}

	bool Frustum::CheckPointInsideFrustum(const Vector3<float>& aPosition) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (myPlanes[i].Inside(aPosition) == false)
			{
				return false;
			}
		}
		return true;
	}

	bool Frustum::CheckQuadInsideFrustum(const Matrix44<float>& aOrientation, const Vector2<float>& aDimensions)
	{

	}

	bool Frustum::CheckAABBInsideFrustum(const Vector3<float>& someMinValues, const Vector3<float>& someMaxValues)
	{
		Vector3<float> minValue;
		Vector3<float> maxValue;
		Vector3<float> normal;

		bool intersection = false;

		for (int i = 0; i < 6; i++)
		{
			normal = myPlanes[i].GetNormal();

			if (normal.x > 0)
			{
				minValue.x = someMinValues.x;
				maxValue.x = someMaxValues.x;
			}
			else
			{
				minValue.x = someMaxValues.x;
				maxValue.x = someMinValues.x;
			}

			if (normal.y > 0)
			{
				minValue.y = someMinValues.y;
				maxValue.y = someMaxValues.y;
			}
			else
			{
				minValue.y = someMaxValues.y;
				maxValue.y = someMinValues.y;
			}

			if (normal.z > 0)
			{
				minValue.z = someMinValues.z;
				maxValue.z = someMaxValues.z;
			}
			else
			{
				minValue.z = someMaxValues.z;
				maxValue.z = someMinValues.z;
			}

			if (myPlanes[i].Inside(minValue) == true)
			{
				return false;
			}
			else if (myPlanes[i].Inside(maxValue) == true)
			{
				intersection = true;
			}
		}

		return intersection;
	}

	float Frustum::GetRadius() const
	{
		return myRadius;
	}
	
	const CU::StaticArray<Vector3<float>, 8>& Frustum::GetPointPositions() const
{
		return myPointPositions;
	}

	const Vector3f& Frustum::GetPosition() const
	{
		return myPosition;
	}

	void Frustum::RenderFrustum() const
	{
		CU::EventManager::GetInstance()->AddRenderCommand(myLines);
		CU::EventManager::GetInstance()->AddRenderCommand(myLines);
		CU::EventManager::GetInstance()->AddRenderCommand(myLines);
	}
}