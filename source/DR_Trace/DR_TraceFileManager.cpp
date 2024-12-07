#include "DR_TraceFileManager.h"
#include <shlobj_core.h>
#include <Shlwapi.h>

#define DR_LOG_FILE_EXTENSION												TEXT(".LOG")
#define DR_LOG_FILE_INFO_SPLITTER											TEXT('-')

DR_TraceFileManager::DR_TraceFileManager(const std::wstring& TracePath, const std::int64_t& TraceMaxFileSize) :
	traceMaxFileSize(TraceMaxFileSize),
	currentFileHandle(INVALID_HANDLE_VALUE),
	fileCounter(0),
	isWarning(false),
	tracePath(TracePath)
{
	InitializeCriticalSection(&this->traceFileGuard);
	this->createCurrentFile();
}

DR_TraceFileManager::~DR_TraceFileManager()
{
	if (this->currentFileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->currentFileHandle);
	}
	DeleteCriticalSection(&this->traceFileGuard);
}

void DR_TraceFileManager::WriteContent(const std::wstring& TraceText)
{
	DR_CriticalSectionEntry guard(this->traceFileGuard);

	LARGE_INTEGER fileSize;
	BOOL res = GetFileSizeEx(this->currentFileHandle, &fileSize);
	if (!res)
	{
		DWORD lastError = GetLastError();
		this->informOfTraceError(TEXT(__FUNCTION__": Failed to get file size. ERROR - %d. "), lastError);
		return;
	}

	if (fileSize.QuadPart + TraceText.size() * sizeof(WCHAR) > this->traceMaxFileSize)
	{
		this->createNextFile();
		if (this->currentFileHandle == INVALID_HANDLE_VALUE)
		{
			DWORD lastError = GetLastError();
			this->informOfTraceError(TEXT(__FUNCTION__": Failed to create next trace file. ERROR - %d. "), lastError);
			return;
		}
	}

	DWORD bytesWritten;
	res = WriteFile(this->currentFileHandle, TraceText.c_str(), TraceText.size() * sizeof(WCHAR), &bytesWritten, NULL);
	if (!res)
	{
		DWORD lastError = GetLastError();
		this->informOfTraceError(TEXT(__FUNCTION__": Failed to write file content. ERROR - %d. "), lastError);
		return;
	}
}

void DR_TraceFileManager::createNextFile()
{
	this->fileCounter++;
	this->createCurrentFile();
}

void DR_TraceFileManager::createCurrentFile()
{
	if (this->currentFileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->currentFileHandle);
		this->currentFileHandle = INVALID_HANDLE_VALUE;
	}
	
	int res = SHCreateDirectoryExW(NULL, this->tracePath.c_str(), NULL);
	if (res != ERROR_SUCCESS && res != ERROR_ALREADY_EXISTS)
	{
		//err
		return;
	}

	bool isEndSlash = true;
	if (!this->tracePath.empty())
	{
		if (this->tracePath.at(this->tracePath.size() - 1) != TEXT('\\'))
		{
			isEndSlash = false;
		}
	}

	auto newFileName = std::move(this->getNewFileName());
	if (!newFileName.has_value())
	{
		//err
		return;
	}

	std::wstring newFullFileName = this->tracePath;
	if (!isEndSlash)
	{
		newFullFileName += TEXT("\\");
	}
	newFullFileName += newFileName.value();

	this->currentFileHandle = CreateFileW(
		newFullFileName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_DELETE | FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (this->currentFileHandle == INVALID_HANDLE_VALUE)
	{
		this->informOfTraceError(TEXT(__FUNCTION__": Failed to create trace file. "));
	}	
}

std::optional<std::wstring> DR_TraceFileManager::getNewFileName() const
{
	DWORD processId = GetCurrentProcessId();
	LPWSTR fileName = NULL;

	WCHAR filePath[MAX_PATH];
	ZeroMemory(filePath, MAX_PATH * sizeof(WCHAR));
	DWORD pathSize = GetModuleFileNameW(NULL, filePath, MAX_PATH);
	if (pathSize == MAX_PATH)
	{
		SetLastError(ERROR_FILENAME_EXCED_RANGE);
		return std::nullopt;
	}

	fileName = PathFindFileNameW(filePath);
	std::wstringstream newFileNameStream;
	newFileNameStream	<< fileName				<< DR_LOG_FILE_INFO_SPLITTER
						<< processId			<< DR_LOG_FILE_INFO_SPLITTER
						<< this->fileCounter	<< DR_LOG_FILE_EXTENSION;
	
	return std::move(newFileNameStream.str());
}

void DR_TraceFileManager::informOfTraceError(const std::wstring& ErrorMsg, ...)
{
	if (this->isWarning)
	{
		return;
	}

	//TO DO
	this->isWarning = true;
}
