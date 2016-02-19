#pragma once

#include "Line.h"
#include "Vector2.h"
#include <vector>
#include "GrowingArray.h"

#define LINE_VOLUME_TEMPLATE template<typename Type>
#define LINE_VOLUME_OBJECT LineVolume<Type>

LINE_VOLUME_TEMPLATE
class LineVolume
{
public:
	LineVolume();
	~LineVolume();

	LineVolume(CU::GrowingArray<Line<Type>> aLineList);
	void AddLine(Line<Type> aLine);
	void RemoveLine(Line<Type> aLine);
	void RemoveAllLines();
	int GetSize()const;
	bool Inside(const Vector2<Type>& aPosition) const;
	const CU::GrowingArray<Line<Type>>& GetLineList()const;

private:
	CU::GrowingArray<Line<Type>> myLineList;
};

LINE_VOLUME_TEMPLATE
LINE_VOLUME_OBJECT::LineVolume()
{
	myLineList.Init(4);
}

LINE_VOLUME_TEMPLATE
LINE_VOLUME_OBJECT::~LineVolume()
{
}

LINE_VOLUME_TEMPLATE
const CU::GrowingArray<Line<Type>>& LINE_VOLUME_OBJECT::GetLineList()const
{
	return myLineList;
}

LINE_VOLUME_TEMPLATE
LINE_VOLUME_OBJECT::LineVolume(CU::GrowingArray<Line<Type>> aLineList)
{
	for (unsigned short i = 0; i < aLineList.Size(); i++)
	{
		myLineList.Add(aLineList[i]);
	}
}

LINE_VOLUME_TEMPLATE
void LINE_VOLUME_OBJECT::AddLine(Line<Type> aLine)
{
	myLineList.Add(aLine);
}

LINE_VOLUME_TEMPLATE
void LINE_VOLUME_OBJECT::RemoveLine(Line<Type> aLine)
{
	for (unsigned int i = 0; i < myLineList.Size(); i++)
	{
		if (myLineList[i] == aLine)
		{
			myLineList[i] = myLineList[myLineList.Size() - 1];
			myLineList.RemoveLast();
			break;
		}
	}
}

LINE_VOLUME_TEMPLATE
void LINE_VOLUME_OBJECT::RemoveAllLines()
{
		myLineList.RemoveAll();
}

LINE_VOLUME_TEMPLATE
bool LINE_VOLUME_OBJECT::Inside(const Vector2<Type>& aPosition) const 
{
	for (unsigned short i = 0; i < myLineList.Size(); i++)
	{
		if (myLineList[i].Inside(aPosition) == false)
		{
			return false;
		}
	}
	return true;
}

LINE_VOLUME_TEMPLATE
int LINE_VOLUME_OBJECT::GetSize()const
{
	return myLineList.Size();
}