#pragma once
#include <string>



namespace CommonUtilities
{
	class DataFile
	{
	public:
		DataFile();
		DataFile(const std::string& aFileName);
		~DataFile();
		DataFile& operator=(const DataFile& aFile);
	protected: 
		std::string myFileName;

		void InitOnFileChange(const std::string& aFileName);
		virtual void OnFileChange(const std::string& aFileName);

	private:
		void FileChange(const std::string& aFileName);

	};
}

namespace CU = CommonUtilities;


