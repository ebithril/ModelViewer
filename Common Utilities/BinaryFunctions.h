#pragma once
#include <string>
#include <fstream>

namespace CommonUtilities
{
	namespace BinaryFunctions
	{
		template <typename Type>
		void GetDataFromBinary(std::istream& anIfstream, Type& aType);

		inline void GetStringFromBinary(std::istream& aStream, std::string& aString);
		
		void GetStringFromBinary(std::istream& aStream, std::string& aString)
		{
			unsigned int count = 0;
			char charArray[1];
			char* currentChar(reinterpret_cast<char*>(&charArray));
			bool firstTime = true;
			do
			{
				if (count > 20000)
				{
					DL_ASSERT("String Binary loading corrupt.");
				}

				if (firstTime == false)
				{
					aString += *currentChar;
				}
				else
				{
					firstTime = false;
				}

				aStream.read(currentChar, 1);
				count++;

			} while (*currentChar != '\0');

		};

		template <typename Type>
		void GetDataFromBinary(std::istream& aStream, Type& aType)
		{
			aStream.read(reinterpret_cast<char*>(&aType), sizeof(aType));
		};	
	}
	namespace BF = BinaryFunctions;
}

namespace CU = CommonUtilities;