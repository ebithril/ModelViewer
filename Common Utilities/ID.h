#pragma once
#include <string>
namespace CommonUtilities
{
	class ID
	{
	public:

		ID(const unsigned int anID);
		~ID();
		ID(const ID &anID);
		ID(const unsigned int, const std::string& aTextID);

		const std::string& GetMyTextID() const;
		const unsigned int GetIntID() const;

		bool operator==(const ID &anID)const;
		bool operator==(const std::string& aTextID) const;
		ID& operator=(const ID &anID);
		ID();
	private:
		unsigned int myID;
		std::string myTextID;
	};
}

namespace CU = CommonUtilities;