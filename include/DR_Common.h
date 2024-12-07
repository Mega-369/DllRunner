#pragma once
#include "stdafx.h"

#ifdef DR_COMMON_STATIC
#define DR_COMMON_CLASS
#define DR_COMMON_FUNC
#else
#ifdef DR_COMMON_EXPORT
#define DR_COMMON_CLASS __declspec(dllexport)
#define DR_COMMON_FUNC __declspec(dllexport)
#else
#define DR_COMMON_CLASS __declspec(dllimport)
#define DR_COMMON_FUNC __declspec(dllimport)
#endif
#endif

class DR_COMMON_CLASS DR_OperationResultEntry
{
private:
	bool internalStatus;
	bool& operationStatus;

public:
	DR_OperationResultEntry();
	DR_OperationResultEntry(bool& OperationStatus);
	bool GetOperationStatus() const;
	void SetSuccess();
	void SetError();
};

class DR_COMMON_CLASS DR_CriticalSectionEntry
{
private:
	CRITICAL_SECTION& criticalSection;
	bool isLocked;

public:
	DR_CriticalSectionEntry(CRITICAL_SECTION& CriticalSection);
	~DR_CriticalSectionEntry();
	void Lock();
	void Unlock();
};

std::wstring DR_COMMON_FUNC DR_GetErrorMessageByCode(DWORD ErrorCode);