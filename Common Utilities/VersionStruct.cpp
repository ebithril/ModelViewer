#include "Common Utilities_Precompiled.h"
#include "VersionStruct.h"

namespace CommonUtilities
{
	VersionStruct::VersionStruct()
	{
		myMajor = 0;
		myMinor = 0;
		myPatch = 0;
	}

	VersionStruct::~VersionStruct()
	{
	}

	VersionStruct::VersionStruct(const int aMajor, const int aPatch, const int aMinor)
	{
		myMajor = aMajor;
		myPatch = aPatch;
		myMinor = aMinor;
	}

	bool VersionStruct::operator<(const VersionStruct& aVersion)
	{
		return operator>=(aVersion) == false;
	}

	bool VersionStruct::operator>=(const VersionStruct& aVersion)
	{
		if (myMajor > aVersion.myMajor)
		{
			return true;
		}
		else if (myMajor == aVersion.myMajor)
		{
			if (myPatch > aVersion.myPatch)
			{
				return true;
			}
			else if (myPatch == aVersion.myPatch)
			{
				if (myMinor >= aVersion.myMinor)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

