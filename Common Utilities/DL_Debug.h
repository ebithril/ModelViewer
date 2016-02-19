#pragma once
#include <string>
#include <fstream>
#include "DL_StackWalker.h"
#include "DL_Assert.h"
#include <cstdarg>
#include "StaticArray.h"
#include "ID.h"

#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__,(string))

#define DL_PRINT(string)  DL_Debug::Debug::GetInstance()->PrintMessage(string)

#define DL_DEBUG( ... )  DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__,__FILE__,__VA_ARGS__)

#define DL_WRITELOG(log, ...)  DL_Debug::Debug::GetInstance()->WriteLog(log,__VA_ARGS__)

#define USE_FILTERLOG

#ifdef USE_FILTERLOG  
#define RESOURCE_LOG(...) DL_WRITELOG(eFilterLog::RESOURCE, __VA_ARGS__)
#define ENGINE_LOG(...) DL_WRITELOG(eFilterLog::GRAPHICS_ENGINE, __VA_ARGS__)
#define GAMEPLAY_LOG(...) DL_WRITELOG(eFilterLog::GAMEPLAY, __VA_ARGS__)
#define START_LOG_TIMER() DL_Debug::Debug::GetInstance()->StartTimer()
#define END_LOG_TIMER() DL_Debug::Debug::GetInstance()->EndTimer()

enum class eFilterLog
{
	RESOURCE,
	GRAPHICS_ENGINE,
	GAMEPLAY,
	MY_SIZE
};

#else
#define RESOURCE_LOG(...) 
#define ENGINE_LOG(...)
#define GAMEPLAY_LOG(...)
#define START_LOG_TIMER()
#define END_LOG_TIMER() 0.f;
#endif

struct _EXCEPTION_POINTERS;

namespace DL_Debug
{
	class Debug
	{
	public:
		static bool Create(const std::string& aFileName = "DebugLogger.txt", const std::string& aLogFileName = "LogFile.txt");
		static bool Destroy();
		static Debug* GetInstance();

		void AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString);
		void PrintMessage(const char *aString);
		void DebugMessage(const int aLine, const char *aFileName, const char *aFormattedString, ...);
		
		int GenerateDump(_EXCEPTION_POINTERS* pExceptionPointers);
#ifdef USE_FILTERLOG
		void WriteLog(const eFilterLog, const char* aString, ...);
		void SetFilterLog(const eFilterLog aFilter, const bool aLogMessages);
		void StartTimer();
		float EndTimer();
#endif

	private:
		Debug();
		~Debug();
		static Debug* ourInstance;
		std::ofstream ourDebugFile;
		std::ofstream ourLogFile;
		StackWalker ourCallStack;

#ifdef USE_FILTERLOG
		std::string GetLogName(const eFilterLog aFilter);
		CU::StaticArray<bool, static_cast<int>(eFilterLog::MY_SIZE)> myFilters;

		CU::ID myTimerID;
#endif 
	};
}