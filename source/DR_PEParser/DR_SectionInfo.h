#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"
#include "DR_PEParserInternal.h"

class DR_SectionInfo : public DR_ISectionInfo
{
private:
	std::vector<DR_SectionCharacteristic> sectionCaracteristics;
	std::wstring sectionName;
	DWORD sectionRVA;
	std::int32_t sectionSize;
	bool isInit;

public:
	DR_SectionInfo(const LPVOID SectionRVA);
	inline virtual const std::vector<DR_SectionCharacteristic>& GetSectionCharacteristics() const { return this->sectionCaracteristics; }
	inline virtual const std::wstring& GetSectionName() const { return this->sectionName; }
	inline virtual std::int32_t GetSectionRVA() const { return this->sectionRVA; }
	inline virtual std::int32_t GetSectionSize() const { return this->sectionSize; }
	inline virtual bool IsInit() const { return this->isInit; }
};
