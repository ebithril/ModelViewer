#pragma once

namespace CommonUtilities
{
	struct VersionStruct
	{
		VersionStruct();
		~VersionStruct();
		VersionStruct(const int aMajor, const int aPatch, const int aMinor);
		bool operator>=(const VersionStruct& aVersion);
		bool operator<(const VersionStruct& aVersion);
		int myMajor;
		int myPatch;
		int myMinor;
	};
}