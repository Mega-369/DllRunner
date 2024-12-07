#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"
#include "DR_PEParserInternal.h"

class DR_ExportNTHeaderInfo : public DR_IExportNTHeaderInfo
{
private:
	DWORD headerRVA;
	std::int32_t headerSize;
	bool isHeaderPresent;
	DR_NTHeader headerType;
	std::map<WORD, DWORD> ordinals;
	std::map<std::wstring, DWORD> functionNames;
	std::uint32_t ordinalBase;

public:
	DR_ExportNTHeaderInfo(const LPVOID ModuleAddress);
	inline virtual DWORD GetHeaderRVA() const override { return this->headerRVA; }
	inline virtual std::int32_t GetHeaderSize() const override { return this->headerSize; }
	inline virtual bool IsHeaderPresent() const override { return this->isHeaderPresent; }
	inline virtual DR_NTHeader GetHeaderType() const override { return this->headerType; }
	inline virtual const std::map<WORD, DWORD>& GetOrdinals() const { return this->ordinals; }
	inline virtual const std::map<std::wstring, DWORD>& GetFunctionNames() const { return this->functionNames; }
	inline virtual std::uint32_t GetOrdinalBase() const { return this->ordinalBase; }
};

