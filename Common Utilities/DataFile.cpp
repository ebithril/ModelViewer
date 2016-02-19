#include "Common Utilities_Precompiled.h"
#include "DataFile.h"
#include "file_watcher.h"

namespace CommonUtilities
{
	DataFile::DataFile()
	{
		myFileName = "Default";
	}


	DataFile::DataFile(const std::string& aFileName)
	{
		myFileName = aFileName;
		CFileWatcher::GetInstance()->WatchFileChangeWithDependencies(myFileName, std::bind(&DataFile::FileChange, this, std::placeholders::_1));
	}

	DataFile::~DataFile()
	{
		if (myFileName != "Default")
		{
			CFileWatcher::GetInstance()->RemoveFileWatch(myFileName);
		}
	}

	void DataFile::InitOnFileChange(const std::string& aFileName)
	{
		myFileName = aFileName;
		CFileWatcher::GetInstance()->WatchFileChangeWithDependencies(myFileName, std::bind(&DataFile::FileChange, this, std::placeholders::_1));
	}

	DataFile& DataFile::operator=(const DataFile& aFile)
	{
		myFileName = aFile.myFileName;
		CFileWatcher::GetInstance()->WatchFileChangeWithDependencies(myFileName, std::bind(&DataFile::FileChange, this, std::placeholders::_1));
		return *this;
	}

	void DataFile::FileChange(const std::string& aFileName)
	{
		aFileName;
		OnFileChange(myFileName);
	}

	void DataFile::OnFileChange(const std::string& aFileName)
	{
		aFileName;
		OutputDebugStringA("Calling non overrided fileobserver function");
	}
	

}

