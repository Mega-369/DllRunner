#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"

class DR_TraceSettingsProvider
{
private:
	std::wstring tracePath;
	DR_TraceLevel traceLevel;
	std::int64_t traceMaxFileSize;
	bool isTraceDateEnable;
	bool isTraceThreadIdEnable;

public:
	DR_TraceSettingsProvider();
	const DR_TraceLevel& GetTraceLevel() const;
	const std::wstring& GetTracePath() const;
	const std::uint64_t& GetTraceMaxFileSize() const;
	const bool& IsTraceDateEnable() const;
	const bool& IsTraceThreadIdEnable() const;
	
private:
	std::optional<DR_TraceLevel> getTraceLevelFormReg() const;
	std::optional<std::wstring> getTracePathFormReg() const;
	std::optional<std::uint64_t> getTraceMaxFileSizeFromReg() const;
	std::optional<bool> getDateEnableFromReg() const;
	std::optional<bool> getTreadIdEnableFromReg() const;
	void tryGetRegSettings();
};
typedef std::unique_ptr<DR_TraceSettingsProvider> DR_UPTraceSettingsProvider;