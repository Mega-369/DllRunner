#include "DR_Common.h"
#include "DR_Common.h"

#define DR_DEFAULT_TEXT_ERROR_MESSAGE												TEXT("Failed to get error message. ")

DR_OperationResultEntry::DR_OperationResultEntry() :
	internalStatus(false),
	operationStatus(internalStatus)
{
}

DR_OperationResultEntry::DR_OperationResultEntry(bool& OperationStatus) :
	internalStatus(false),
	operationStatus(OperationStatus)
{
	this->operationStatus = false;
}

bool DR_OperationResultEntry::GetOperationStatus() const
{
	return this->operationStatus;
}

void DR_OperationResultEntry::SetSuccess()
{
	this->operationStatus = true;
}

void DR_OperationResultEntry::SetError()
{
	this->operationStatus = false;
}

DR_CriticalSectionEntry::DR_CriticalSectionEntry(CRITICAL_SECTION& CriticalSection) :
	criticalSection(CriticalSection),
	isLocked(false)
{
	this->Lock();
}

DR_CriticalSectionEntry::~DR_CriticalSectionEntry()
{
	this->Unlock();
}

void DR_CriticalSectionEntry::Lock()
{
	if (this->isLocked)
	{
		return;
	}

	EnterCriticalSection(&this->criticalSection);
	this->isLocked = true;
}

void DR_CriticalSectionEntry::Unlock()
{
	if (!this->isLocked)
	{
		return;
	}

	LeaveCriticalSection(&this->criticalSection);
	this->isLocked = false;
}

std::wstring DR_COMMON_FUNC DR_GetErrorMessageByCode(DWORD ErrorCode)
{
	LPWSTR errorBuffer = NULL;

	DWORD res = FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US),
		errorBuffer,
		0,
		NULL
	);
	if (res == 0 || errorBuffer == NULL)
	{
		return DR_DEFAULT_TEXT_ERROR_MESSAGE;
	}

	std::wstring errorMessage(errorBuffer);
	LocalFree(errorBuffer);
	return std::move(errorMessage);
}
