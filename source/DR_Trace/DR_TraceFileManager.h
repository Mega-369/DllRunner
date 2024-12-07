#pragma once
#include "stdafx.h"
#include "DR_Common.h"

class DR_TraceFileManager
{
private:
	std::int64_t traceMaxFileSize;
	std::wstring tracePath;
	HANDLE currentFileHandle;
	std::uint32_t fileCounter;
	bool isWarning;
	CRITICAL_SECTION traceFileGuard;

public:
	DR_TraceFileManager(const std::wstring& TracePath, const std::int64_t& TraceMaxFileSize);
	~DR_TraceFileManager();
	void WriteContent(const std::wstring& TraceText);

private:
	void createNextFile();
	void createCurrentFile();
	std::optional<std::wstring> getNewFileName() const;
	void informOfTraceError(const std::wstring& ErrorMsg, ...);
};
typedef std::unique_ptr<DR_TraceFileManager> DR_UPTraceFileManager;