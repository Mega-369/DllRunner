#include "DR_TraceDefault.h"

#define DR_FORMAT_MESSAGE_SIZE														4096
#define DR_TRACE_ERROR_PREFIX														TEXT("ERROR")
#define DR_TRACE_WARNING_PREFIX														TEXT("WARNING")
#define DR_TRACE_INFORMATION_PREFIX													TEXT("INFORMATION")

DR_TraceDefault::DR_TraceDefault() :
	traceSettingsUPtr(nullptr),
	traceFileManagerUPtr(nullptr)
{
	this->traceSettingsUPtr = std::make_unique<DR_TraceSettingsProvider>();
	this->traceFileManagerUPtr = std::make_unique<DR_TraceFileManager>(
		this->traceSettingsUPtr->GetTracePath(), 
		this->traceSettingsUPtr->GetTraceMaxFileSize()
	);
}

void DR_TraceDefault::Trace(LPCWSTR Message, ...)
{
	va_list args;
	va_start(args, Message);
	auto formatMessage = this->formatMessage(Message, args);
	va_end(args);
	if (!formatMessage.has_value())
	{
		return;
	}

	std::wstring prepareMessage = std::move(this->makeMessage(*formatMessage));
	this->traceFileManagerUPtr->WriteContent(prepareMessage);
}

void DR_TraceDefault::TraceError(LPCWSTR Message, ...)
{
	if (this->GetTraceLevel() < DR_TraceLevelError)
	{
		return;
	}

	va_list args;
	va_start(args, Message);
	auto formatMessage = this->formatMessage(Message, args);
	va_end(args);
	if (!formatMessage.has_value())
	{
		return;
	}

	std::wstring prepareMessage = std::move(this->makeMessage(*formatMessage, DR_TRACE_ERROR_PREFIX));
	this->traceFileManagerUPtr->WriteContent(prepareMessage);
}

void DR_TraceDefault::TraceWarning(LPCWSTR Message, ...)
{
	if (this->GetTraceLevel() < DR_TraceLevelWarning)
	{
		return;
	}

	va_list args;
	va_start(args, Message);
	auto formatMessage = this->formatMessage(Message, args);
	va_end(args);
	if (!formatMessage.has_value())
	{
		return;
	}

	std::wstring preapareMessage = std::move(this->makeMessage(*formatMessage, DR_TRACE_WARNING_PREFIX));
	this->traceFileManagerUPtr->WriteContent(preapareMessage);
}

void DR_TraceDefault::TraceInformation(LPCWSTR Message, ...)
{
	if (this->GetTraceLevel() < DR_TraceLevelInformation)
	{
		return;
	}

	va_list args;
	va_start(args, Message);
	auto formatMessage = std::move(this->formatMessage(Message, args));
	va_end(args);
	if (!formatMessage.has_value())
	{
		return;
	}

	std::wstring prepareMessage = std::move(this->makeMessage(*formatMessage, DR_TRACE_INFORMATION_PREFIX));
	this->traceFileManagerUPtr->WriteContent(prepareMessage);
}

DR_TraceLevel DR_TraceDefault::GetTraceLevel() const
{
	return this->traceSettingsUPtr->GetTraceLevel();
}

std::wstring DR_TraceDefault::getCurrentDateTimeFormat() const
{
	SYSTEMTIME sysCurrentTime;
	GetLocalTime(&sysCurrentTime);
	std::wstringstream formatTimeStream;
	formatTimeStream 
		<< std::setfill(TEXT('0')) << std::setw(2) << sysCurrentTime.wDay << TEXT('.')
		<< std::setfill(TEXT('0')) << std::setw(2) << sysCurrentTime.wMonth << TEXT('.')
		<< sysCurrentTime.wYear << TEXT(" ")
		<< std::setfill(TEXT('0')) << std::setw(2) << sysCurrentTime.wHour << TEXT(':')
		<< std::setfill(TEXT('0')) << std::setw(2) << sysCurrentTime.wMinute << TEXT(':')
		<< std::setfill(TEXT('0')) << std::setw(2) << sysCurrentTime.wSecond << TEXT('.')
		<< std::setfill(TEXT('0')) << std::setw(3) << sysCurrentTime.wMilliseconds;
	return formatTimeStream.str();
}

std::optional<std::wstring> DR_TraceDefault::formatMessage(LPCWSTR Message, va_list args)
{
	WCHAR formatMessage[DR_FORMAT_MESSAGE_SIZE];
	ZeroMemory(formatMessage, DR_FORMAT_MESSAGE_SIZE * sizeof(WCHAR));
	int res = vswprintf_s(formatMessage, Message, args);
	if (res < 0)
	{
		return std::nullopt;
	}

	return formatMessage;
}

std::wstring DR_TraceDefault::makeMessage(const std::wstring& FormatMessageVal)
{
	std::wstringstream formatMessageStream;

	if (this->traceSettingsUPtr->IsTraceDateEnable())
	{
		formatMessageStream << TEXT('[') << this->getCurrentDateTimeFormat() << TEXT("] ");
	}

	if (this->traceSettingsUPtr->IsTraceThreadIdEnable())
	{
		formatMessageStream << TEXT("<<<") << GetCurrentThreadId() << TEXT(">>> ");
	}

	formatMessageStream << FormatMessageVal;

	if (!FormatMessageVal.empty())
	{
		if (FormatMessageVal[FormatMessageVal.size() - 1] != TEXT('\n'))
		{
			formatMessageStream << TEXT('\n');
		}
	}

	return formatMessageStream.str();
}

std::wstring DR_TraceDefault::makeMessage(const std::wstring& FormatMessageVal, const std::wstring& MessagePrefix)
{
	std::wstringstream formatMessageStream;
	formatMessageStream << MessagePrefix << TEXT(" - ");

	if (this->traceSettingsUPtr->IsTraceDateEnable())
	{
		formatMessageStream << TEXT('[') << this->getCurrentDateTimeFormat() << TEXT("] ");
	}

	if (this->traceSettingsUPtr->IsTraceThreadIdEnable())
	{
		formatMessageStream << TEXT("<<<") << GetCurrentThreadId() << TEXT(">>> ");
	}

	formatMessageStream << FormatMessageVal;

	if (!FormatMessageVal.empty())
	{
		if (FormatMessageVal[FormatMessageVal.size() - 1] != TEXT('\n'))
		{
			formatMessageStream << TEXT('\n');
		}
	}

	return formatMessageStream.str();
}
