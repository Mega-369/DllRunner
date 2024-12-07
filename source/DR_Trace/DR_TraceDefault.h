#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_TraceFileManager.h"
#include "DR_TraceSettingsProvider.h"

class DR_TraceDefault : public DR_ITrace
{
private:
	DR_UPTraceSettingsProvider traceSettingsUPtr;
	DR_UPTraceFileManager traceFileManagerUPtr;
	
public:
	DR_TraceDefault();
	virtual void Trace(LPCWSTR Message, ...) override;
	virtual void TraceError(LPCWSTR Message, ...) override;
	virtual void TraceWarning(LPCWSTR Message, ...) override;
	virtual void TraceInformation(LPCWSTR Message, ...) override;
	virtual DR_TraceLevel GetTraceLevel() const override;
	virtual ~DR_TraceDefault() override = default;

private:
	std::wstring getCurrentDateTimeFormat() const;
	std::optional<std::wstring> formatMessage(LPCWSTR Message, va_list args);
	std::wstring makeMessage(const std::wstring& FormatMessageVal);
	std::wstring makeMessage(const std::wstring& FormatMessageVal, const std::wstring& MessagePrefix);
};
