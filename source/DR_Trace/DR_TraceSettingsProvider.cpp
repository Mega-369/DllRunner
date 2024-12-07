#include "DR_TraceSettingsProvider.h"

#define DR_TRACE_SETTINGS_SUBKEY														TEXT("Software\\DllRunner\\Trace")

#define DR_REG_KEY_NAME_TRACE_LEVEL														TEXT("Level")
#define DR_REG_KEY_NAME_TRACE_PATH														TEXT("Path")
#define DR_REG_KEY_NAME_TRACE_MAX_FILE_SIZE												TEXT("MaxFileSize")
#define DR_REG_KEY_NAME_TRACE_DATE_ENABLE												TEXT("DateEnable")
#define DR_REG_KEY_NAME_TRACE_THREAD_ID_ENABLE											TEXT("ThreadIdEnable")

#ifdef _DEBUG
#define DR_DEFAULT_VALUE_TRACE_LEVEL													DR_TraceLevel::DR_TraceLevelInformation
#else
#define DR_DEFAULT_VALUE_TRACE_LEVEL													DR_TraceLevel::DR_TraceLevelError
#endif
#define DR_DEFAULT_VALUE_TRACE_PATH														TEXT(".\\Trace")
#define DR_DEFAULT_VALUE_TRACE_MAX_FILE_SIZE											10485760
#define DR_DEFAULT_VALUE_TRACE_DATE_ENABLE												true
#define DR_DEFAULT_VALUE_TRACE_THREAD_ID_ENABLE											true

DR_TraceSettingsProvider::DR_TraceSettingsProvider() :
	tracePath(DR_DEFAULT_VALUE_TRACE_PATH),
	traceLevel(DR_DEFAULT_VALUE_TRACE_LEVEL),
	traceMaxFileSize(DR_DEFAULT_VALUE_TRACE_MAX_FILE_SIZE)
{
	this->tryGetRegSettings();
}

const DR_TraceLevel& DR_TraceSettingsProvider::GetTraceLevel() const
{
	return this->traceLevel;
}

const std::wstring& DR_TraceSettingsProvider::GetTracePath() const
{
	return this->tracePath;
}

const std::uint64_t& DR_TraceSettingsProvider::GetTraceMaxFileSize() const
{
	return this->traceMaxFileSize;
}

const bool& DR_TraceSettingsProvider::IsTraceDateEnable() const
{
	return this->isTraceDateEnable;
}

const bool& DR_TraceSettingsProvider::IsTraceThreadIdEnable() const
{
	return this->isTraceThreadIdEnable;
}

std::optional<DR_TraceLevel> DR_TraceSettingsProvider::getTraceLevelFormReg() const
{
	DWORD regTraceLevel;

	HKEY traceLevelKey;
	LSTATUS status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DR_TRACE_SETTINGS_SUBKEY, 0, KEY_READ, &traceLevelKey);
	if (status != ERROR_SUCCESS)
	{
		return std::nullopt;
	}

	DWORD regValueType;
	DWORD dataSize = sizeof(DWORD);
	status = RegQueryValueExW(traceLevelKey, DR_REG_KEY_NAME_TRACE_LEVEL, NULL, &regValueType, (LPBYTE)&regTraceLevel, &dataSize);
	if (status != ERROR_SUCCESS || regValueType != REG_DWORD)
	{
		RegCloseKey(traceLevelKey);
		return std::nullopt;
	}

	RegCloseKey(traceLevelKey);
	return (DR_TraceLevel)regTraceLevel;
}

std::optional<std::wstring> DR_TraceSettingsProvider::getTracePathFormReg() const
{
	std::wstring regTracePath;

	HKEY tracePathKey;
	LSTATUS status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DR_TRACE_SETTINGS_SUBKEY, 0, KEY_READ, &tracePathKey);
	if (status != ERROR_SUCCESS)
	{
		return std::nullopt;
	}

	DWORD regValueType;
	DWORD valueSize = 0;
	status = RegQueryValueExW(tracePathKey, DR_REG_KEY_NAME_TRACE_PATH, NULL, &regValueType, NULL, &valueSize);
	if (status != ERROR_SUCCESS)
	{
		RegCloseKey(tracePathKey);
		return std::nullopt;
	}

	std::vector<std::byte> tracePathBuffer;
	tracePathBuffer.resize(valueSize + sizeof(WCHAR));
	status = RegQueryValueExW(tracePathKey, DR_REG_KEY_NAME_TRACE_PATH, NULL, &regValueType, reinterpret_cast<LPBYTE>(tracePathBuffer.data()), &valueSize);
	if (status != ERROR_SUCCESS || regValueType != REG_EXPAND_SZ)
	{
		RegCloseKey(tracePathKey);
		return std::nullopt;
	}

	regTracePath = reinterpret_cast<std::wstring::value_type*>(tracePathBuffer.data());
	RegCloseKey(tracePathKey);
	return regTracePath;
}

std::optional<std::uint64_t> DR_TraceSettingsProvider::getTraceMaxFileSizeFromReg() const
{
	std::uint64_t regMaxFileSize;

	HKEY maxFileSizeKey;
	LSTATUS status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DR_TRACE_SETTINGS_SUBKEY, 0, KEY_READ, &maxFileSizeKey);
	if (status != ERROR_SUCCESS)
	{
		return std::nullopt;
	}

	DWORD regValueType;
	DWORD valueSize = sizeof(std::int64_t);
	status = RegQueryValueExW(maxFileSizeKey, DR_REG_KEY_NAME_TRACE_MAX_FILE_SIZE, NULL, &regValueType, (LPBYTE)&regMaxFileSize, &valueSize);
	if (status != ERROR_SUCCESS || regValueType != REG_QWORD)
	{
		RegCloseKey(maxFileSizeKey);
		return std::nullopt;
	}

	RegCloseKey(maxFileSizeKey);
	return regMaxFileSize;
}

std::optional<bool> DR_TraceSettingsProvider::getDateEnableFromReg() const
{
	DWORD isTraceEnable;

	HKEY traceEnableKey;
	LSTATUS status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DR_TRACE_SETTINGS_SUBKEY, 0, KEY_READ, &traceEnableKey);
	if (status != ERROR_SUCCESS)
	{
		return std::nullopt;
	}

	DWORD regValueType;
	DWORD valueSize = sizeof(DWORD);
	status = RegQueryValueExW(traceEnableKey, DR_REG_KEY_NAME_TRACE_DATE_ENABLE, NULL, &regValueType, (LPBYTE)&isTraceDateEnable, &valueSize);
	if (status != ERROR_SUCCESS || regValueType != REG_DWORD)
	{
		RegCloseKey(traceEnableKey);
		return std::nullopt;
	}

	RegCloseKey(traceEnableKey);
	return (bool)isTraceDateEnable;
}

std::optional<bool> DR_TraceSettingsProvider::getTreadIdEnableFromReg() const
{
	DWORD isTraceThreadIdEnable;

	HKEY traceThreadIdEnableKey;
	LSTATUS status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DR_TRACE_SETTINGS_SUBKEY, 0, KEY_READ, &traceThreadIdEnableKey);
	if (status != ERROR_SUCCESS)
	{
		std::nullopt;
	}

	DWORD regValueType;
	DWORD valueSize = sizeof(DWORD);
	status = RegQueryValueExW(traceThreadIdEnableKey, DR_REG_KEY_NAME_TRACE_THREAD_ID_ENABLE, NULL, &regValueType, (LPBYTE)&isTraceThreadIdEnable, &valueSize);
	if (status != ERROR_SUCCESS || regValueType != REG_DWORD)
	{
		RegCloseKey(traceThreadIdEnableKey);
		std::nullopt;
	}

	RegCloseKey(traceThreadIdEnableKey);
	return (bool)isTraceThreadIdEnable;
}

void DR_TraceSettingsProvider::tryGetRegSettings()
{
	auto regTraceLevel = this->getTraceLevelFormReg();
	if (regTraceLevel.has_value())
	{
		this->traceLevel = *regTraceLevel;
	}
	else
	{
		this->traceLevel = DR_DEFAULT_VALUE_TRACE_LEVEL;
	}

	auto regTracePath = this->getTracePathFormReg();
	if (regTracePath.has_value())
	{
		this->tracePath = *regTracePath;
	}
	else
	{
		this->tracePath = DR_DEFAULT_VALUE_TRACE_PATH;
	}

	auto regTraceMaxFileSize = this->getTraceMaxFileSizeFromReg();
	if (regTraceMaxFileSize.has_value())
	{
		this->traceMaxFileSize = *regTraceMaxFileSize;
	}
	else
	{
		this->traceMaxFileSize = DR_DEFAULT_VALUE_TRACE_MAX_FILE_SIZE;
	}

	auto regIsTraceDateEnable = this->getDateEnableFromReg();
	if (regIsTraceDateEnable.has_value())
	{
		this->isTraceDateEnable = *regIsTraceDateEnable;
	}
	else
	{
		this->isTraceDateEnable = DR_DEFAULT_VALUE_TRACE_DATE_ENABLE;
	}

	auto regIsTraceThreadIdEnable = this->getTreadIdEnableFromReg();
	if (regIsTraceThreadIdEnable.has_value())
	{
		this->isTraceThreadIdEnable = *regIsTraceThreadIdEnable;
	}
	else
	{
		this->isTraceThreadIdEnable = DR_DEFAULT_VALUE_TRACE_THREAD_ID_ENABLE;
	}
}