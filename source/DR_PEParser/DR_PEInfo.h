#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"
#include "DR_PEParserInternal.h"

class DR_PEInfo : public DR_IPEInfo
{
private:
	bool isInit;
	DR_Subsystem subsystem;
	std::vector<DR_FileCharacteristic> fileCharacteristics;
	std::vector<DR_DllCharacteristic> dllCharacteristics;
	std::vector<DR_ShPtrSectionInfo> sectionsInfo;
	std::vector<DR_ShPtrNTHeaderInfo> NTHeaders;
	bool isDOSSignatureCorrect;
	bool isNTSignatureCorrect;
	DWORD NTHeaderRVA;
	DR_Machine targetMahine;
	std::int32_t imageSize;
	DWORD entryPointRVA;
	std::chrono::system_clock::time_point timeStamp;

public:
	DR_PEInfo(const LPVOID ModulePtr);
	inline virtual bool IsInit() const override { return this->isInit; }
	inline virtual bool IsSignaturesCorrect() const override { return this->isDOSSignatureCorrect && this->isNTSignatureCorrect; }
	inline virtual bool IsDOSSignatureCorrect() const override { return this->isDOSSignatureCorrect; }
	inline virtual bool IsNTSignatureCorrect() const override { return this->isNTSignatureCorrect; }
	inline virtual const std::vector<DR_FileCharacteristic>& GetFileCharacteristics() const override { return this->fileCharacteristics; }
	inline virtual const std::vector<DR_DllCharacteristic>& GetDllCharacteristics() const override { return this->dllCharacteristics; }
	inline virtual const std::vector<DR_ShPtrSectionInfo>& GetSectionsInfo() const override { return this->sectionsInfo; }
	inline virtual DR_Subsystem GetSubsystem() const override { return this->subsystem; }
	inline virtual DR_Machine GetTargetMachine() const override { return this->targetMahine; }
	inline virtual std::int32_t GetImageSize() const override { return this->imageSize; }
	inline virtual std::chrono::system_clock::time_point GetTimeStamp() const override { return this->timeStamp; }
	inline virtual DWORD GetEntryPointRVA() const override { return this->entryPointRVA; }
	inline virtual DWORD GetNTHeaderRVA() const override { return this->NTHeaderRVA; }
	inline virtual const DR_ShPtrNTHeaderInfo& GetNTHeader(DR_NTHeader HeaderId) const override { return this->NTHeaders[HeaderId]; }

private:
	std::chrono::system_clock::time_point convertTimeStampToChrono(DWORD timeStamp) const;
	DR_ShPtrNTHeaderInfo analyzeHeader(const LPVOID ModulePtr, DR_NTHeader HeaderType);
};

