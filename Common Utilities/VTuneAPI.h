#pragma once
#include "ittnotify.h"
#include "AutoPointer.h"
#include "GrowingArray.h"

#ifndef WORKBED_RELEASE
#define USE_VTUNE_API
#endif

#ifdef USE_VTUNE_API
#pragma comment(lib, "../Libraries/libittnotify.lib.")

#define VTUNE_API_CREATE_DOMAIN(aVariableName, aDomainName) AutoPointer<__itt_domain> (aVariableName) = __itt_domain_create(aDomainName);
#define VTUNE_API_CREATE_HANDLE(aTaskName, aTimeStampObjectName) AutoPointer<__itt_string_handle> (aTimeStampObjectName) = __itt_string_handle_create(aTaskName);
#define VTUNE_API_TASK_BEGIN(aDomain, aTimeStampObject) __itt_task_begin(aDomain.get(), __itt_null, __itt_null, aTimeStampObject.get());
#define VTUNE_API_TASK_END(aDomain) __itt_task_end(aDomain.get());
#define VTUNE_API_TASK_PAUSE __itt_pause();
#define VTUNE_API_TASK_RESUME __itt_resume();

#else

#define VTUNE_API_CREATE_DOMAIN(aVariableName, aDomainName)
#define VTUNE_API_CREATE_HANDLE(aTaskName, aTimeStampObjectName)
#define VTUNE_API_TASK_BEGIN(aDomain, aTimeStampObject)
#define VTUNE_API_TASK_END(aDomain)
#define VTUNE_API_TASK_PAUSE
#define VTUNE_API_TASK_RESUME

#endif
