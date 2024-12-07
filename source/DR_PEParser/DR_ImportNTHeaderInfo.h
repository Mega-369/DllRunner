#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"
#include "DR_PEParserInternal.h"

class DR_ImportInfo : public DR_IImportInfo
{
private:
	std::wstring name;
	std::vector<std::wstring> functionNames;
	std::vector<WORD> functionOrdinals;
	bool isInit;

public:
	DR_ImportInfo(LPVOID ModuleAddress, LPVOID ImportDescripor);
	inline virtual const std::wstring& GetName() const override { return this->name; }
	inline virtual const std::vector<std::wstring>& GetFunctionNames() const override { return this->functionNames; }
	inline virtual const std::vector<WORD>& GetFunctionOrdinals() const override { return this->functionOrdinals; }
	inline virtual bool IsInit() const override { return this->isInit; }
};

class DR_ImportNTHeaderInfo : public DR_IImportNTHeaderInfo
{
private:
	bool isHeaderPresent;
	DWORD headerRVA;
	std::int32_t headerSize;
	DR_NTHeader headerType;
	std::vector<DR_ShPtrImportInfo> imports;

public:
	DR_ImportNTHeaderInfo(const LPVOID ModuleAddress);
	inline virtual DWORD GetHeaderRVA() const override { return this->headerRVA; }
	inline virtual std::int32_t GetHeaderSize() const override { return this->headerSize; }
	inline virtual bool IsHeaderPresent() const override { return this->isHeaderPresent; }
	inline virtual DR_NTHeader GetHeaderType() const override { return this->headerType; }
	inline virtual const std::vector<DR_ShPtrImportInfo>& GetImports() const override { return this->imports; }
};

