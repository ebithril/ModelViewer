#include "Common Utilities_Precompiled.h"
#include "MemoryStackWalker.h"
#include "MemoryWrapper.h"


MemoryStackWalker::MemoryStackWalker() : BaseStackWalker()
{
}


MemoryStackWalker::~MemoryStackWalker()
{
}

MemoryStackWalker::MemoryStackWalker(unsigned int aProcessId, HANDLE aProcess)
	: BaseStackWalker(aProcessId, aProcess)
{
}

void MemoryStackWalker::OnOutput(char* aString)
{
	MemoryWrapper::GetInstance()->StackWalkOutputText(aString);
}
