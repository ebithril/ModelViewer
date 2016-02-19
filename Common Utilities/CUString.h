#pragma once

#include "DL_Debug.h"
#include "Map.h"

namespace CommonUtilities
{
#define STRING_TEMPLATE template <unsigned int MaxSize>
#define OTHER_STRING_TEMPLATE template <unsigned int OtherMaxSize>
#define STRING_OBJECT String<MaxSize>


	template <unsigned int MaxSize = 30>
	class String
	{
	public:

		String();
		~String();
		OTHER_STRING_TEMPLATE
			String(const String<OtherMaxSize> &aCopy);
		String(const char* aCopy);
		OTHER_STRING_TEMPLATE
			String operator+(const String<OtherMaxSize> &aString) const;
		String operator+(const float aFloat) const;
		String operator+(const double aDouble) const;
		String operator+(const int aInt) const;
		String operator+(const char* aLiteralString) const;

		OTHER_STRING_TEMPLATE
			String& operator+=(const String<OtherMaxSize> &aString);
		String& operator+=(const float aFloat);
		String& operator+=(const double aDouble);
		String& operator+=(const int aInt);
		String& operator+=(const char* aLiteralString);

		OTHER_STRING_TEMPLATE
			bool operator==(const String<OtherMaxSize> &aString) const;
		bool operator==(const String &aString) const;
		bool operator==(const char* aLiteralString) const;

		OTHER_STRING_TEMPLATE
			void operator=(const String<OtherMaxSize> &aString);
		void operator=(const char* aLiteralString);

		int Find(const char* aLiteralString) const;
		OTHER_STRING_TEMPLATE
			int Find(const String<OtherMaxSize> &aString) const;

		String SubStr(const int aIndex, const int aLength) const;
		const char* c_str() const;

		int Size() const;

		const char& operator[](const int aIndex) const;
		char& operator[](const int aIndex);

	private:

		char myString[MaxSize + 1];
		int mySize;

		void AddToArray(int aStartIndex, const char* aLiteralString);
		OTHER_STRING_TEMPLATE
			bool ExactMatch(const String<OtherMaxSize>& aString) const;
		OTHER_STRING_TEMPLATE
			int FindSubString(const String<OtherMaxSize>& aString) const;
		OTHER_STRING_TEMPLATE
			void CreateHorsTable(const String<OtherMaxSize>& aString, Map<char, int, 32>& aTable) const;

	};
	STRING_TEMPLATE
		STRING_OBJECT::String()
	{
		mySize = 0;
	}

	STRING_TEMPLATE
		STRING_OBJECT::~String()
	{

	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		STRING_OBJECT::String(const String<OtherMaxSize> &aCopy)
	{
		*this = aCopy;
	}

	STRING_TEMPLATE
		STRING_OBJECT::String(const char* aCopy)
	{
		AddToArray(0, aCopy);
	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::operator + (const String<OtherMaxSize> &aString) const
	{
		return *this + aString.c_str();
	}

	STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::operator + (const float aFloat) const
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%f", aFloat);

		return *this + buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::operator + (const double aDouble) const
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%f", aDouble);

		return *this + buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::operator + (const int aInt) const
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%i", aInt);

		return *this + buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::operator + (const char* aLiteralString) const
	{
		String newString = *this;
		newString.AddToArray(Size(), aLiteralString);
		return newString;
	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		STRING_OBJECT& STRING_OBJECT::operator += (const String<OtherMaxSize> &aString)
	{
		return *this += aString.c_str();
	}

	STRING_TEMPLATE
		STRING_OBJECT& STRING_OBJECT::operator += (const float aFloat)
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%f", aFloat);

		return *this += buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT& STRING_OBJECT::operator += (const double aDouble)
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%f", aDouble);

		return *this += buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT& STRING_OBJECT::operator += (const int aInt)
	{
		char buffer[MaxSize];
		sprintf_s(buffer, "%i", aInt);

		return *this += buffer;
	}

	STRING_TEMPLATE
		STRING_OBJECT& STRING_OBJECT::operator += (const char* aLiteralString)
	{
		this->AddToArray(Size(), aLiteralString);
		return *this;
	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		bool STRING_OBJECT::operator == (const String<OtherMaxSize> &aString) const
	{
		if (Size() == aString.Size())
		{
			return ExactMatch(aString);
		}
		else
		{
			return false;
		}
	}

	STRING_TEMPLATE
		bool STRING_OBJECT::operator==(const String &aString) const
	{
		if (Size() == aString.Size())
		{
			return ExactMatch(aString);
		}
		else
		{
			return false;
		}
	}

	STRING_TEMPLATE
		bool STRING_OBJECT::operator == (const char* aLiteralString) const
	{
		int counter = 0;
		char currentChar = '\0';
		do
		{
			currentChar = aLiteralString[counter];
			counter++;
		} while (currentChar != '\0');

		counter--;

		if (counter == mySize)
		{
			String<MaxSize> stringToCheck(aLiteralString);
			if (stringToCheck == *this)
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


	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		void STRING_OBJECT::operator = (const String<OtherMaxSize> &aString)
	{
		if (aString.Size() <= static_cast<int>(MaxSize))
		{
			*this = aString.c_str();
		}
		else
		{
			DL_ASSERT("Right value string is larger than left value string max size.");
		}
	}

	STRING_TEMPLATE
		void STRING_OBJECT::operator = (const char* aLiteralString)
	{
		AddToArray(0, aLiteralString);
	}

	STRING_TEMPLATE
		int STRING_OBJECT::Find(const char* aLiteralString) const
	{
		return Find(String(aLiteralString));
	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		int STRING_OBJECT::Find(const String<OtherMaxSize> &aString) const
	{
		if (aString.Size() > 1)
		{
			return FindSubString(aString);
		}
		else if (aString.Size() == 1)
		{
			char charToCheck = aString[0];
//			char currentChar = '\0';

			for (int i = 0; i < mySize; i++)
			{
				if (myString[i] == charToCheck)
				{
					return i;
				}
			}
		}
		return -1;

	}

	STRING_TEMPLATE
		STRING_OBJECT STRING_OBJECT::SubStr(const int aIndex, const int aLength) const
	{
		if (aLength < 0)
		{
			DL_ASSERT("Length can't be negative in a substring!");
		}

		if (aIndex + aLength >= MaxSize)
		{
			DL_ASSERT("Substring outside string array!");
		}

		char subString[MaxSize];

		for (int i = 0; i < aLength; i++)
		{
			subString[i] = myString[aIndex + i];
		}

		subString[aLength] = '\0';

		return String(subString);
	}

	STRING_TEMPLATE
		const char* STRING_OBJECT::c_str() const
	{
		return myString;
	}

	STRING_TEMPLATE
		int STRING_OBJECT::Size() const
	{
		return mySize;
	}

	STRING_TEMPLATE
		const char& STRING_OBJECT::operator[](const int aIndex) const
	{
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index outside of array!");
			return myString[0];
		}
		else
		{
			return myString[aIndex];
		}
	}

	STRING_TEMPLATE
		char& STRING_OBJECT::operator[](const int aIndex)
	{
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index outside of array!");
			return myString[0];
		}
		else
		{
			return myString[aIndex];
		}
	}

	STRING_TEMPLATE
		void STRING_OBJECT::AddToArray(int aStartIndex, const char* aLiteralString)
	{
		int index = aStartIndex;
		int literalIndex = 0;
		mySize = index - 1;

		do
		{
			mySize++;
			if (index > MaxSize)
			{
				DL_ASSERT("Right value char* is larger than left value string max size.");
			}

			myString[index] = aLiteralString[literalIndex++];

		} while (myString[index++] != '\0');

	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		bool STRING_OBJECT::ExactMatch(const String<OtherMaxSize>& aString) const
	{
		int currentIndex = 0;
		int currentSize = mySize;

		while (currentSize != 0)
		{
			if (currentSize >= 8)
			{
				INT64 leftData = static_cast<INT64>(myString[currentIndex]);
				INT64 rightData = static_cast<INT64>(aString[currentIndex]);

				if (leftData != rightData)
				{
					return false;
				}
				else
				{
					currentSize -= 8;
					currentIndex += 8;
				}
			}
			else if (currentSize >= 4)
			{
				INT32 leftData = static_cast<INT32>(myString[currentIndex]);
				INT32 rightData = static_cast<INT32>(aString[currentIndex]);

				if (leftData != rightData)
				{
					return false;
				}
				else
				{
					currentSize -= 4;
					currentIndex += 4;
				}
			}
			else if (currentSize >= 2)
			{
				INT16 leftData = static_cast<INT16>(myString[currentIndex]);
				INT16 rightData = static_cast<INT16>(aString[currentIndex]);

				if (leftData != rightData)
				{
					return false;
				}
				else
				{
					currentSize -= 2;
					currentIndex += 2;
				}
			}
			else if (currentSize >= 1)
			{
				if (myString[currentIndex] != aString[currentIndex])
				{
					return false;
				}
				else
				{
					currentSize -= 1;
					currentIndex += 1;
				}
			}
			else
			{
				DL_ASSERT("Something wrong happend while checking strings for exact match");
			}

		}

		return true;
	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		int STRING_OBJECT::FindSubString(const String<OtherMaxSize>& aString) const
	{
		Map<char, int, 32> table;

		CreateHorsTable(aString, table);

		int currentLastIndex = aString.Size() - 1;
		int currentCheckingIndex = currentLastIndex;
		int subStringIndex = currentLastIndex;

		while (currentLastIndex < Size())
		{
			if (aString[subStringIndex] == myString[currentCheckingIndex])
			{
				if (subStringIndex == 0)
				{
					return currentCheckingIndex;
				}
				else
				{
					currentCheckingIndex--;
					subStringIndex--;
				}
			}
			else
			{
				char key = myString[currentCheckingIndex];

				if (table.KeyExists(key) == true)
				{
					currentLastIndex = currentLastIndex + table.Get(key);
				}
				else
				{
					currentLastIndex = currentLastIndex + aString.Size() - 1;
				}

				currentCheckingIndex = currentLastIndex;
				subStringIndex = aString.Size() - 1;
			}
		}

		return -1;

	}

	STRING_TEMPLATE
		OTHER_STRING_TEMPLATE
		void STRING_OBJECT::CreateHorsTable(const String<OtherMaxSize>& aString, Map<char, int, 32>& aTable) const
	{

		int length = aString.Size();

		for (int i = 0; i < length; i++)
		{
			aTable.Insert(aString[i], length - (i + 1));
		}
	}


#undef STRING_TEMPLATE 
#undef OTHER_STRING_TEMPLATE 
#undef STRING_OBJECT 

}

namespace CU = CommonUtilities;