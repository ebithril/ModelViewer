#pragma once
#include "Vector.h"
#include "PlaneVolume.h"
#include "LineVolume.h"
#include "GrowingArray.h"

namespace CommonUtilities
{
	namespace Intersection
	{
		bool PointInsideCircle(const Vector2<float>& aPoint, const Vector2<float>& aCircleCenterPosition, const float& aCircleRadius);

		bool CircleInsideBox(const LineVolume<float>& aBox, const Vector2<float>& aBoxCenterPosition,
			const Position& aCircleCenterPosition, const float &aCircleRadius);

		bool CircleInsideCircle(const Vector2<float>& aFirstCircleCenterPosition, const float& aFirstCircleRadius,
			const Position& aSecondCircleCenterPosition, const float& aSecondCircleRadius);

		bool CircleInsideLine(const Line<float> aLine, const Vector2<float>& aCircleCenterPosition, const float& aCircleRadius);

		bool LineVsLine(const Line<float>& aLine1, const Line<float>& aLine2, Vector2<float>& aIntersectionPoint);

		bool PointInLine(const Vector2<float>& aPosition, const Line<float>& aline);

		bool LineVolumeVsLineVolume(const LineVolume<float>& aFirstLineVolume, const Vector2<float>& aFirstVolumeWorldPosition, const LineVolume<float>& aSecondLineVolume, const Vector2<float>& aSecondVolumeWorldPosition);

		const Line<float> AddWorldPositionToLine(const Line<float>& aLine, const Vector2<float>& aWorldPosition);

		int GetAngleBetweenVectors(Vector2<float> aFirst, Vector2<float> aSecond);

		bool SphereVsSphere(const Vector3<float>& aFirstSpherePosition, const float& aFirstRadius, const Vector3<float>& aSecondSpherePosition, const float& aSecondRadius);

		bool RayVsSphere(const Vector3<float>& aRayDirection, const Vector3<float>& aRayOrigin, const Vector3<float>& aSpherePosition, const float& aRadius);

		bool RayVsPlane(const Vector3<float>& aRayDirection, const Vector3<float>& aRayOrigin, const Vector3<float>& aPlaneNormal, const Vector3<float>& aPlanePosition, Vector3<float>& anIntersectionPoint);


	};
}
namespace CU = CommonUtilities;


