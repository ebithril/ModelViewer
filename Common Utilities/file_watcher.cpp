#include "Common Utilities_Precompiled.h"
#include "file_watcher.h"
#include <fstream>
#include <algorithm>

namespace CommonUtilities
{

	CFileWatcher* CFileWatcher::ourInstance = nullptr;

	CFileWatcher::CFileWatcher()
		: myThread(nullptr)
		, myShouldEndThread(false)
		, myThreadIsDone(false)
	{

	}

	CFileWatcher* CFileWatcher::GetInstance()
	{
		if (ourInstance == nullptr)
		{
			ourInstance = new CFileWatcher();
		}

		return ourInstance;
	}



	CFileWatcher::~CFileWatcher()
	{
		myShouldEndThread = true;
		while (!myThreadIsDone)
		{
			Sleep(1);
		}
	}


	void CFileWatcher::FlushChanges()
	{
		std::lock_guard<std::mutex> guard(myMutex);


		myFileChanged.swap(myFileChangedThreaded);

		for (std::string& theString : myFileChanged)
		{
			std::string directoryOfFile(theString);
			directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of("\\/"));

			std::string theFile(theString);
			theFile = theFile.substr(theFile.find_first_of("\\/") + 1, theFile.size());

			std::vector<callback_function_file> callbacks = myCallbacks[theFile];
			for (unsigned int i = 0; i < callbacks.size(); i++)
			{
				if (callbacks[i])
				{
					if (callbacks[i]._Empty() == false)
					{
						callbacks[i](theString);
					}
				}
			}
		}


		myFileChanged.clear();


	}

	void CFileWatcher::UpdateChanges(const std::string& aDir)
	{
		const DWORD timeOut = 1000;
		while (!myShouldEndThread)
		{
			HANDLE  ChangeHandle = FindFirstChangeNotification(aDir.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_ACTION_ADDED | FILE_ACTION_REMOVED | FILE_ACTION_MODIFIED);
			DWORD Wait = WaitForSingleObject(ChangeHandle, timeOut);
			if (Wait == WAIT_OBJECT_0)
			{
				myMutex.lock();
				OnFolderChange(aDir);
				FindNextChangeNotification(ChangeHandle);
				myMutex.unlock();
			}

			Sleep(1);
		}
		myThreadIsDone = true;
	}

	void CFileWatcher::OnFolderChange(const std::string& aDir)
	{
		std::vector<WIN32_FIND_DATA> currentFolderFiles = GetAllFilesInFolder(aDir);
		std::vector<WIN32_FIND_DATA>& savedFolderFiles = myFolders[aDir];

		for (WIN32_FIND_DATA& currentFile : currentFolderFiles)
		{
			for (WIN32_FIND_DATA& savedFile : savedFolderFiles)
			{
				if (std::string(currentFile.cFileName).compare(savedFile.cFileName) == 0)
				{
					ULARGE_INTEGER currentFileTime;
					SYSTEMTIME currentFileTimeSystem;
					FileTimeToSystemTime(&currentFile.ftLastWriteTime, &currentFileTimeSystem);
					currentFileTime.LowPart = currentFile.ftLastWriteTime.dwLowDateTime;
					currentFileTime.HighPart = currentFile.ftLastWriteTime.dwHighDateTime;
					__int64 currentFileTime64 = currentFileTime.QuadPart;

					ULARGE_INTEGER savedFileTime;
					SYSTEMTIME savedFileTimeSystem;
					FileTimeToSystemTime(&savedFile.ftLastWriteTime, &savedFileTimeSystem);
					savedFileTime.LowPart = savedFile.ftLastWriteTime.dwLowDateTime;
					savedFileTime.HighPart = savedFile.ftLastWriteTime.dwHighDateTime;
					__int64 savedFileTime64 = savedFileTime.QuadPart;

					if (currentFileTime64 != savedFileTime64)
					{
						std::string fileThatChangedPath = aDir + "\\" + std::string(currentFile.cFileName);
						bool isDependency = myDependencies.find(fileThatChangedPath) != myDependencies.end();
						if (isDependency)
						{
							std::vector<std::string>& deps = myDependencies[fileThatChangedPath];
							for (std::string& file : deps)
							{
								OnFileChange(file);
							}

						}
						else
						{
							OnFileChange(fileThatChangedPath);
						}

						// We have a change

						savedFile = currentFile;
					}
				}
			}
		}
	}

	void CFileWatcher::OnFileChange(std::string& aFile)
	{
		for (unsigned int i = 0; i < myFileChangedThreaded.size(); i++)
		{
			if (myFileChangedThreaded[i].compare(aFile) == 0)
			{
				return;
			}
		}
		myFileChangedThreaded.push_back(aFile);

	}

	bool CFileWatcher::WatchFileChangeWithDependencies(std::string aFile, callback_function_file aFunctionToCallOnChange)
	{
		std::ifstream stream(aFile);
		if (!stream.good())
		{
			stream.close();
			return false;
		}

		std::string directoryOfFile(aFile);
		directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of("\\/"));

		std::string line;
		const std::string includeString = "include";
		while (getline(stream, line))
		{
			std::size_t found = line.find(includeString);
			if (found != std::string::npos)
			{
				std::string foundFile(line);
				foundFile = foundFile.substr(foundFile.find_first_of("\"") + 1, foundFile.size());
				foundFile = foundFile.substr(0, foundFile.find_last_of("\""));

				if (foundFile.substr(foundFile.length() - 2, 2) == "fx")
				{
					foundFile.substr(0, foundFile.length() - 2);
					foundFile.append("fso");
				}

				if (!foundFile.empty())
				{
					std::string depFile = directoryOfFile + "\\" + foundFile;
					WatchFileChange(depFile, aFunctionToCallOnChange);
					myDependencies[depFile].push_back(aFile);
				}
			}
		}

		stream.close();
		return WatchFileChange(aFile, aFunctionToCallOnChange);
	}

	bool CFileWatcher::WatchFileChange(std::string aFile, callback_function_file aFunctionToCallOnChange)
	{
		std::replace(aFile.begin(), aFile.end(), '/', '\\'); // replace all '/' to '\\'

		std::ifstream stream(aFile);
		if (!stream.good())
		{
			stream.close();
			return false;
		}
		stream.close();

		std::string directoryOfFile(aFile);
		directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of("\\/"));

		std::string theFile(aFile);
		theFile = theFile.substr(theFile.find_first_of("\\/") + 1, theFile.size());

		myCallbacks[theFile].push_back(aFunctionToCallOnChange);
		return WatchDirectory(directoryOfFile);
	}

	bool CFileWatcher::WatchDirectory(const std::string& aDir)
	{
		FolderMap::iterator iter = myFolders.find(aDir);
		if (iter != myFolders.end())
		{
			//Already in our watch list
			return true;
		}

		myFolders[aDir] = GetAllFilesInFolder(aDir);

		myThread = new std::thread(&CFileWatcher::UpdateChanges, this, aDir);
		return true;
	}

	std::vector<WIN32_FIND_DATA> CFileWatcher::GetAllFilesInFolder(std::string aDir)
	{
		std::vector<WIN32_FIND_DATA> filesInFolder;
		aDir += "/*.*";
		WIN32_FIND_DATA fd;

		HANDLE hFind = ::FindFirstFile(aDir.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					filesInFolder.push_back(fd);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return filesInFolder;
	}

	void CFileWatcher::RemoveFileWatch(const std::string& aFile)
	{
		myCallbacks.erase(aFile);
	}
}