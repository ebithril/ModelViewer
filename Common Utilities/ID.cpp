#include "Common Utilities_Precompiled.h"
#include "ID.h"
#include "DL_Assert.h"

const std::string& CU::ID::GetMyTextID() const
{
	return myTextID;
}

const unsigned int CU::ID::GetIntID() const
{
	return myID;
}


CU::ID::ID(const unsigned int anId) : myID (anId)
{

}

CU::ID::ID(const unsigned int anId, const std::string& aTextID) 
	: myID (anId), myTextID(aTextID)
{

}

CU::ID::~ID()
{

}

CU::ID::ID(ID const &anID) : myID(anID.myID), myTextID(anID.myTextID)
{
	
}

bool CU::ID::operator==(const std::string& aTextID) const
{
	if (myTextID == aTextID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CU::ID::operator==(const ID &anID) const
{
	if (myID == anID.myID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CU::ID::ID() : myID(0), myTextID("defaultID")
{
	//DL_ASSERT("Default constructor for ID not allowed");
}

CU::ID& CU::ID::operator=(const ID &anID)  
{
	myID = anID.myID;
	myTextID = anID.myTextID;
	return *this;
}