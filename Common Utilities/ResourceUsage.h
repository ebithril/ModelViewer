#pragma once

#include <Windows.h>

int GetMemoryUsage();

unsigned long long FixCPUTimings(const FILETIME &a, const FILETIME &b);

float GetCPUUsage(FILETIME *prevSysKernel, FILETIME *prevSysUser, FILETIME *prevProcKernel, FILETIME *prevProcUser, bool firstRun = false);