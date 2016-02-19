#include "Common Utilities_Precompiled.h"
#include "Intersection.h"

namespace CommonUtilities
{
	namespace Intersection
	{
		bool PointInsideCircle(const Vector2<float>& aPoint, const Vector2<float>& aCircleCenterPosition, const float& aCircleRadius)
		{
			float lengthBetweenPoints = Length(aCircleCenterPosition - aPoint);

			if (aCircleRadius >= lengthBetweenPoints)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool CircleInsideBox(const LineVolume<float>& aBox, const Vector2<float>& aBoxCenterPosition,
			const Vector2<float>& aCircleCenterPosition, const float &aCircleRadius)
		{
			
			CU::GrowingArray<Line<float>> boxLines = aBox.GetLineList();

			for (unsigned short i = 0; i < boxLines.Size(); i++)
			{
				if (CircleInsideLine(boxLines[i], aCircleCenterPosition - aBoxCenterPosition, aCircleRadius) == false)
				{
					return false;
				}
			}

			return true;
		}

		bool CircleInsideLine(const Line<float> aLine, const Vector2<float>& aCircleCenterPosition, const float& aCircleRadius)
		{
			Vector2<float> circleVector = aCircleRadius * (-1.f * aLine.GetMyNormal());

			return (aLine.Inside(aCircleCenterPosition + circleVector));
		}

		bool CircleInsideCircle(const Vector2<float>& aFirstCircleCenterPosition, const float& aFirstCircleRadius,
			const Vector2<float>& aSecondCircleCenterPosition, const float& aSecondCircleRadius)
		{
			return PointInsideCircle(aSecondCircleCenterPosition, aFirstCircleCenterPosition, aFirstCircleRadius + aSecondCircleRadius);
		}
	
		bool LineVsLine(const Line<float>& aLine1, const Line<float>& aLine2, Vector2<float>& aIntersectionPoint)
		{
			Vector2<float> intersectionPoint;

			float line1DottedNormal = Dot(aLine1.GetMyPoint(), aLine1.GetMyNormal());
			float line2DottedNormal = Dot(aLine2.GetMyPoint(), aLine2.GetMyNormal());

			if (line1DottedNormal == line2DottedNormal)
			{
				if (PointInLine(aLine2.GetMyPoint(), aLine1) == true)
				{
					aIntersectionPoint = aLine1.GetMySecondPoint();
					return true;
				}
				else if (PointInLine(aLine2.GetMySecondPoint(), aLine1) == true)
				{
					aIntersectionPoint = aLine2.GetMySecondPoint();
					return true;
				}
				
				else
				{
					return false;
				}

			}

			intersectionPoint.myX =
				(aLine2.GetMyNormal().myY * line1DottedNormal - aLine1.GetMyNormal().myY * line2DottedNormal) /
				(aLine1.GetMyNormal().myX * aLine2.GetMyNormal().myY - aLine2.GetMyNormal().myX * aLine1.GetMyNormal().myY);

			intersectionPoint.myY =
				(aLine1.GetMyNormal().myX * line2DottedNormal - aLine2.GetMyNormal().myX * line1DottedNormal) /
			(aLine1.GetMyNormal().myX * aLine2.GetMyNormal().myY - aLine2.GetMyNormal().myX * aLine1.GetMyNormal().myY);


			if (PointInLine(intersectionPoint, aLine1))
			{
				aIntersectionPoint = intersectionPoint;
				return true;
			}

			return false;
		}

		bool PointInLine(const Vector2<float>& aPosition, const Line<float>& aLine)
		{

			if (aLine.GetMyPoint().myX < aLine.GetMySecondPoint().myX)
			{
				if (aPosition.myX >= aLine.GetMyPoint().myX && aPosition.myY >= aLine.GetMyPoint().myY &&
					aPosition.myX <= aLine.GetMySecondPoint().myX && aPosition.myY <= aLine.GetMySecondPoint().myY)
				{
					return true;
				}
			}
			else
			{
				if (aPosition.myX <= aLine.GetMyPoint().myX && aPosition.myY <= aLine.GetMyPoint().myY &&
					aPosition.myX >= aLine.GetMySecondPoint().myX && aPosition.myY >= aLine.GetMySecondPoint().myY)
				{
					return true;
				}
			}
			return false;
		}

		bool LineVolumeVsLineVolume(const LineVolume<float>& aFirstLineVolume, const Vector2<float>& aFirstVolumeWorldPosition, const LineVolume<float>& aSecondLineVolume, const Vector2<float>& aSecondVolumeWorldPosition)
		{
			LineVolume<float> firstLineVolume;
			LineVolume<float> secondLineVolume;
			Vector2<float> intersectionPoint;

			for (unsigned short i = 0; i < aFirstLineVolume.GetLineList().Size(); i++)
			{
				firstLineVolume.AddLine(AddWorldPositionToLine(aFirstLineVolume.GetLineList()[i], aFirstVolumeWorldPosition));
			}

			for (unsigned short i = 0; i < aFirstLineVolume.GetLineList().Size(); i++)
			{
				secondLineVolume.AddLine(AddWorldPositionToLine(aSecondLineVolume.GetLineList()[i], aSecondVolumeWorldPosition));
			}

			for (unsigned short i = 0; i < firstLineVolume.GetSize(); i++)
			{
				for (unsigned short j = 0; j < secondLineVolume.GetSize(); j++)
				{
					if (LineVsLine(firstLineVolume.GetLineList()[i], secondLineVolume.GetLineList()[j], intersectionPoint) == true)
					{
						return true;
					}
				}
			}
			return false;
		}

		const Line<float> AddWorldPositionToLine(const Line<float>& aLine, const Vector2<float>& aWorldPosition)
		{
			return Line<float>(aLine.GetMyPoint() + aWorldPosition, aLine.GetMySecondPoint() + aWorldPosition);
		}

		int GetAngleBetweenVectors(Vector2<float> aFirst, Vector2<float> aSecond)
		{
			float firstAtan = atan2(aFirst.y, aFirst.x);
			float secondAtan = atan2(aSecond.y, aSecond.x);

			float angle = firstAtan - secondAtan;

			if (angle < 0)
			{
				angle += 2 * 3.14159265359f;
			}

			return int(angle * 180 / 3.14159265359f + 0.5f);
		}

		bool SphereVsSphere(const Vector3<float>& aFirstSpherePosition, const float& aFirstRadius, const Vector3<float>& aSecondSpherePosition, const float& aSecondRadius)
		{
			float length = Length2(aFirstSpherePosition - aSecondSpherePosition);
			if (length <= (aFirstRadius * aFirstRadius + aSecondRadius * aSecondRadius))
			{
				return true;
			}
			return false;
		}

		bool RayVsSphere(const Vector3<float>& aRayDirection, const Vector3<float>& aRayOrigin, const Vector3<float>& aSpherePosition, const float& aRadius)
		{
			float det, b;
			Vector3<float> p = aRayOrigin - aSpherePosition;
			b = -Dot(p, aRayDirection);
			det = b*b - Dot(p, p) + aRadius * aRadius;

			if (det < 0)
			{
				return false;
			}

			det = sqrt(det);

			float firstIntersectionDistance = b - det;
			float secondIntersectionDistance = b + det;

			if (secondIntersectionDistance < 0)
			{
				return false;
			}
			if (firstIntersectionDistance < 0)
			{
				firstIntersectionDistance = 0;
			}
			return true;

		}

		bool RayVsPlane(const Vector3<float>& aRayDirection, const Vector3<float>& aRayOrigin, const Vector3<float>& aPlaneNormal, const Vector3<float>& aPlanePosition, Vector3<float>& anIntersectionPoint)
		{
			float T = Dot(aPlaneNormal, (aPlanePosition - aRayOrigin)) / Dot(aPlaneNormal, aRayDirection);

			anIntersectionPoint = aRayOrigin + (aRayDirection * T);
			return true;
		}
	}
}