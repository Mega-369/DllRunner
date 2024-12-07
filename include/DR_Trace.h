#pragma once
#include "stdafx.h"
#include "DR_Common.h"

//Shell32.lib, Shlwapi.lib

#ifdef DR_TRACE_STATIC
#define DR_TRACE_CLASS
#define DR_TRACE_FUNC
#else
#ifdef DR_TRACE_EXPORT
#define DR_TRACE_CLASS __declspec(dllexport)
#define DR_TRACE_FUNC __declspec(dllexport)
#else
#define DR_TRACE_CLASS __declspec(dllimport)
#define DR_TRACE_FUNC __declspec(dllimport)
#endif
#endif

enum DR_TRACE_CLASS DR_TraceLevel
{
	DR_TraceLevelDisabled = 0,
	DR_TraceLevelError = 1,
	DR_TraceLevelWarning = 2,
	DR_TraceLevelInformation = 3
};

class DR_TRACE_CLASS DR_ITrace
{
public:
	virtual void Trace(LPCWSTR Message, ...) = 0;
	virtual void TraceError(LPCWSTR Message, ...) = 0;
	virtual void TraceWarning(LPCWSTR Message, ...) = 0;
	virtual void TraceInformation(LPCWSTR Message, ...) = 0;
	virtual DR_TraceLevel GetTraceLevel() const = 0;
	virtual ~DR_ITrace() = default;
};
typedef std::shared_ptr<DR_ITrace> DR_SPTrace;

DR_SPTrace DR_TRACE_FUNC DR_ImplementTrace();