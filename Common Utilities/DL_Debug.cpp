#include "Common Utilities_Precompiled.h"
#include "DL_Debug.h"
#include "TimerManager.h"
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <direct.h>
#include "CUString.h"

#pragma comment(lib, "dbghelp.lib")

namespace DL_Debug
{
	Debug* Debug::ourInstance = nullptr;

	Debug::Debug()
	{

	}

	Debug::~Debug()
	{
	}

	bool Debug::Create(const std::string& aDebugFileName, const std::string& aLogFileName)
	{
		assert(ourInstance == nullptr && "Debugobject already created");
		ourInstance = new Debug();
		ourInstance->ourDebugFile = std::ofstream(aDebugFileName, std::ofstream::trunc);

		if (ourInstance->ourDebugFile.is_open() == false)
		{
			ourInstance->ourDebugFile.open(aDebugFileName, std::ofstream::trunc);
		}

		ourInstance->ourLogFile = std::ofstream(aLogFileName, std::ofstream::trunc);

		if (ourInstance->ourLogFile.is_open() == false)
		{
			ourInstance->ourLogFile.open(aLogFileName, std::ofstream::trunc);
		}

		if (ourInstance == nullptr)
		{
			return(false);
		}

#ifdef USE_FILTERLOG
		for (int i = 0; i < static_cast<int>(eFilterLog::MY_SIZE); i++)
		{
			ourInstance->myFilters[i] = true;
		}

		ourInstance->myTimerID = CU::TimerManager::GetInstance()->CreateTimer();

#endif 
		return(true);
	}

	bool Debug::Destroy()
	{
		if (ourInstance != nullptr)
		{
			ourInstance->ourDebugFile.close();
			if (ourInstance->ourDebugFile.is_open() == true)
			{
				return(false);
			}

			ourInstance->ourLogFile.close();
			if (ourInstance->ourLogFile.is_open() == true)
			{
				return(false);
			}

			delete ourInstance;
			ourInstance = nullptr;
		}
		return true;
	}

	Debug* Debug::GetInstance()
	{
		return(ourInstance);
	}

	void Debug::AssertMessage(const char *aFileName, int aLine,
		const char *aFunctionName, const char *aString)
	{	
		ourDebugFile << "\n\n--------------------------Assert-------------------------\n\n"
			<< "Assert message : " << aString
			<< "\nBuild: " <<
#ifdef _DEBUG 
			"Debug"
#else
			"Release"
#endif
			<< "\nVersion Date: " << __DATE__ << "\n" << "Version Time: " << __TIME__
			<< "\nFile name: " << aFileName << "\nLine number: " << aLine
			<< "\nFunction name: " << aFunctionName
			<< "\nStack call: \n\n";
		ourCallStack.ShowCallstack();

		ourDebugFile << "----------------------End of assert----------------------\n\n";
		ourDebugFile.close();

#ifdef _DEBUG
		MessageBoxA(nullptr, (CU::String<1024>("Program Crash!\n") + aString + "Press OK to break.").c_str(), "Program Crash!", MB_OK | MB_APPLMODAL);
		__debugbreak();
#else
		int ans = MessageBoxA(nullptr, (CU::String<1024>("Program Crash!\n") + aString + " Do you want to view the debug log?").c_str(), "Program Crash!", MB_YESNO | MB_APPLMODAL);
		if (ans == IDYES)
		{
			system("notepad.exe debuglogger.txt");
			ExitProcess(0);
		}
		else if (ans == IDNO)
		{
			ExitProcess(0);
		}
#endif

		/*	int answer = MessageBoxA(nullptr, "Program crashed, do you want to let a programmer remote debug?", "Crash!", MB_YESNO);

			if (answer == IDYES)
			{
			system("msvsmon.exe");
			MessageBoxA(nullptr, "LEAVE ME BE! Don't press ok until a programmer has debugged the crash.\nStart remote debugger program if it's not up.", "Crash!", MB_OK);
			}*/
	}

	void Debug::PrintMessage(const char *aString)
	{
		GetInstance()->ourDebugFile << aString << "\n";
	}

	void Debug::DebugMessage(const int aLine, const char *aFileName, 
		const char *aFormattedString, ...)
	{
		char charBuffer[1024];
		va_list arguments;
		va_start(arguments, aFormattedString);
		vsprintf_s(charBuffer, aFormattedString, arguments);

		ourDebugFile << "\n\n----------------------Debug Message----------------------\n\n"
			<< "Debug message : " << charBuffer
			<< "\nBuild: " <<
#ifdef _DEBUG 
			"Debug"
#else
			"Release"
#endif

			<< "\nVersion Date: " << __DATE__ << "\n" << "Version Time: " << __TIME__
			<< "\nFile name: " << aFileName << "\nLine number: " << aLine
			<< "\n\n-------------------End of Debug Message------------------\n\n";
	}



	int Debug::GenerateDump(_EXCEPTION_POINTERS* pExceptionPointers)
	{
		BOOL bMiniDumpSuccessful;
		
		CHAR* szVersion = "v1.0";
		HANDLE hDumpFile;
		SYSTEMTIME stLocalTime;
		MINIDUMP_EXCEPTION_INFORMATION ExpParam;

		GetLocalTime(&stLocalTime);

		CU::String<256> filename = "DumpFile";
		filename += " ";
		filename += szVersion;
		filename += " ";
		filename += stLocalTime.wYear;
		filename += "-";
		filename += stLocalTime.wDay;
		filename += "-";
		filename += stLocalTime.wMonth;
		filename += " ";
		filename += stLocalTime.wHour;
		filename += ".";
		filename += stLocalTime.wMinute;
		filename += ".";
		filename += stLocalTime.wSecond;
		filename += ".dmp";
		
		hDumpFile = CreateFile(filename.c_str(), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

		ExpParam.ThreadId = GetCurrentThreadId();
		ExpParam.ExceptionPointers = pExceptionPointers;
		ExpParam.ClientPointers = TRUE;

		bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

		return EXCEPTION_EXECUTE_HANDLER;
	}

#ifdef USE_FILTERLOG

	void Debug::WriteLog(eFilterLog aFilter, const char* aString, ...)
	{
		if (myFilters[static_cast<int>(aFilter)] == true)
		{
			char charBuffer[1024];
			va_list arguments;
			va_start(arguments, aString);
			vsprintf_s(charBuffer, aString, arguments);

			ourLogFile << "\n\n----------------------Message sent by [" << GetLogName(aFilter) << "] filterlog----------------------\n\n"

				<< charBuffer

				<< "\n\n---------------------------------End of log message---------------------------------\n\n";
		}	
	}

	void Debug::SetFilterLog(const eFilterLog aFilter, const bool aLogMessages)
	{
		myFilters[static_cast<int>(aFilter)] = aLogMessages;
	}

	void Debug::StartTimer()
	{
		CU::TimerManager::GetInstance()->Update();
		CU::TimerManager::GetInstance()->GetTimer(myTimerID).SetPause(false);
		CU::TimerManager::GetInstance()->GetTimer(myTimerID).ResetTimer();
	}

	float Debug::EndTimer()
	{
		CU::TimerManager::GetInstance()->Update();
		CU::TimerManager::GetInstance()->GetTimer(myTimerID).SetPause(true);
		return CU::TimerManager::GetInstance()->GetTimer(myTimerID).GetMyTotalTime().GetMyTimeInMillisecond();
	}

	std::string Debug::GetLogName(const eFilterLog aFilter)
	{
		switch (aFilter)
		{
		case eFilterLog::RESOURCE:
			return "Resource";
			break;
		case eFilterLog::GRAPHICS_ENGINE:
			return "Graphics Engine";
			break;
		case eFilterLog::GAMEPLAY:
			return "Gameplay";
			break;

		default:
			return "Unknown";
			break;
		}
	}
#endif
}