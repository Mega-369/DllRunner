#include "DR_PEInfo.h"
#include "DR_SectionInfo.h"
#include "DR_ExportNTHeaderInfo.h"
#include "DR_ImportNTHeaderInfo.h"
#include "DR_ResourceNTHeaderInfo.h"

#define DR_MAX_NT_HEADERS_COUNT                                      15

DR_PEInfo::DR_PEInfo(const LPVOID ModulePtr) :
    isInit(false),
    isDOSSignatureCorrect(false),
    isNTSignatureCorrect(false),
    subsystem(DR_SubsystemUnknown),
    NTHeaderRVA(NULL),
    targetMahine(DR_MachineUnknown),
    imageSize(0),
    entryPointRVA(NULL)
{
    DR_OperationResultEntry initEntry(this->isInit);

    PIMAGE_DOS_HEADER DOSHeaderPtr = reinterpret_cast<PIMAGE_DOS_HEADER>(ModulePtr);
    if (DOSHeaderPtr == nullptr)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to cast ptr to DOS header. "));
        return;
    }

    this->isDOSSignatureCorrect = DOSHeaderPtr->e_magic == IMAGE_DOS_SIGNATURE;
    if (!this->isDOSSignatureCorrect)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid DOS signature. "));
        return;
    }

    this->NTHeaderRVA = DOSHeaderPtr->e_lfanew;
    if (this->NTHeaderRVA == NULL)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid NT RVA address. "));
        return;
    }

    PIMAGE_NT_HEADERS NTHeaderPtr = reinterpret_cast<PIMAGE_NT_HEADERS>((PBYTE)ModulePtr + this->NTHeaderRVA);
    this->isNTSignatureCorrect = NTHeaderPtr->Signature == IMAGE_NT_SIGNATURE;
    if (!this->isNTSignatureCorrect)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid NT signature. "));
        return;
    }

    this->subsystem = static_cast<DR_Subsystem>(NTHeaderPtr->OptionalHeader.Subsystem);
    this->targetMahine = static_cast<DR_Machine>(NTHeaderPtr->FileHeader.Machine);
    this->entryPointRVA = NTHeaderPtr->OptionalHeader.AddressOfEntryPoint;
    this->imageSize = NTHeaderPtr->OptionalHeader.SizeOfImage;

    std::vector<DR_FileCharacteristic> allFileCharacteristics = { //todo make func
        DR_FileCharacteristicRelocsStripped,
        DR_FileCharacteristicExecutableIamage,
        DR_FileCharacteristicLineNumsStripped,
        DR_FileCharacteristicLocalSymsStripped,
        DR_FileCharacteristicAggresiveWsTrim,
        DR_FileCharacteristicLargeAddressAware,
        DR_FileCharacteristicBytesReversedLo,
        DR_FileCharacteristic32BitMachine,
        DR_FileCharacteristicDebugStripped,
        DR_FileCharacteristicRemovableRunFromSwap,
        DR_FileCharacteristicNetRunFromSwap,
        DR_FileCharacteristicSystem,
        DR_FileCharacteristicDll,
        DR_FileCharacteristicUpSystemOnly,
        DR_FileCharacteristicBytesReversedHI
    };
    for (auto currentCharacteristic : allFileCharacteristics)
    {
        if (NTHeaderPtr->FileHeader.Characteristics & currentCharacteristic)
        {
            this->fileCharacteristics.push_back(currentCharacteristic);
        }
    }

    std::vector<DR_DllCharacteristic> allDllCharacteristics = { // todo make func
        DR_DllCharacteristicHighEntropyVa,
        DR_DllCharacteristicDynamicBase,
        DR_DllCharacteristicForceIntegrity,
        DR_DllCharacteristicNXCompat,
        DR_DllCharacteristicNoIsolation,
        DR_DllCharacteristicNoSEH,
        DR_DllCharacteristicNoBind,
        DR_DllCharacteristicAppcontainer,
        DR_DllCharacteristicWDMDriver,
        DR_DllCharacteristicGuardCF,
        DR_DllCharacteristicTerminalServerAware
    };
    for (auto currentDllCharacteristic : allDllCharacteristics)
    {
        if (NTHeaderPtr->OptionalHeader.DllCharacteristics & currentDllCharacteristic)
        {
            this->dllCharacteristics.push_back(currentDllCharacteristic);
        }
    }

    DWORD timeStamp = NTHeaderPtr->FileHeader.TimeDateStamp;
    this->timeStamp = this->convertTimeStampToChrono(timeStamp);

    PIMAGE_SECTION_HEADER fistSection = IMAGE_FIRST_SECTION(NTHeaderPtr);
    this->sectionsInfo.reserve(NTHeaderPtr->FileHeader.NumberOfSections);
    for (int currentSectionIndex = 0; currentSectionIndex < NTHeaderPtr->FileHeader.NumberOfSections; currentSectionIndex++)
    {
        DR_ShPtrSectionInfo currentSectionShPtr = std::make_shared<DR_SectionInfo>(&fistSection[currentSectionIndex]);
        if (currentSectionShPtr->IsInit())
        {
            this->sectionsInfo.push_back(currentSectionShPtr);
        }
    }

    this->NTHeaders.resize(DR_MAX_NT_HEADERS_COUNT);
    this->NTHeaders[DR_NTHeaderExport] = this->analyzeHeader(ModulePtr, DR_NTHeaderExport);
    this->NTHeaders[DR_NTHeaderImport] = this->analyzeHeader(ModulePtr, DR_NTHeaderImport);
    this->NTHeaders[DR_NTHeaderResource] = this->analyzeHeader(ModulePtr, DR_NTHeaderResource);
    //TODO

    initEntry.SetSuccess();
}

std::chrono::system_clock::time_point DR_PEInfo::convertTimeStampToChrono(DWORD timeStamp) const
{
    std::chrono::seconds durationSinceEpoch(timeStamp);
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::time_point(durationSinceEpoch);
    return timePoint;
}

DR_ShPtrNTHeaderInfo DR_PEInfo::analyzeHeader(const LPVOID ModulePtr, DR_NTHeader HeaderType)
{
    DR_ShPtrNTHeaderInfo outHeaderInfo = nullptr;
    switch (HeaderType)
    {
    case DR_NTHeaderExport:
        outHeaderInfo = std::make_shared<DR_ExportNTHeaderInfo>(ModulePtr);
        break;
    case DR_NTHeaderImport:
        outHeaderInfo = std::make_shared<DR_ImportNTHeaderInfo>(ModulePtr);
        break;
    case DR_NTHeaderResource:
        outHeaderInfo = std::make_shared<DR_ResourceNTHeaderInfo>(ModulePtr);
        break;
    case DR_NTHeaderException:
        break;
    case DR_NTHeaderSecurity:
        break;
    case DR_NTHeaderBaseReloc:
        break;
    case DR_NTHeaderDebug:
        break;
    case DR_NTHeaderGlobalPtr:
        break;
    case DR_NTHeaderTLS:
        break;
    case DR_NTHeaderLoadConfig:
        break;
    }
    return outHeaderInfo;
}