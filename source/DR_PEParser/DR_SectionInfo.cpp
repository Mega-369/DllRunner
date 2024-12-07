#include "DR_SectionInfo.h"

DR_SectionInfo::DR_SectionInfo(const LPVOID SectionRVA) :
    isInit(false),
    sectionRVA(0),
    sectionSize(0)
{
    DR_OperationResultEntry initEntry(this->isInit);

    const PIMAGE_SECTION_HEADER sectionHeader = reinterpret_cast<const PIMAGE_SECTION_HEADER>(SectionRVA);
    if (sectionHeader == NULL)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to cast sectios header. "));
        return;
    }

    std::unique_ptr<WCHAR[]> sectionNamePtr = std::make_unique<WCHAR[]>(sizeof(sectionHeader->Name));
    ZeroMemory(sectionNamePtr.get(), sizeof(sectionHeader->Name) * sizeof(WCHAR));
    int res = MultiByteToWideChar(
        CP_UTF8,
        0,
        (LPCSTR)sectionHeader->Name,
        sizeof(sectionHeader->Name),
        sectionNamePtr.get(),
        sizeof(sectionHeader->Name)
    );
    if (res == 0)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to convert secton name. "));
        return;
    }

    std::vector<DR_SectionCharacteristic> allCharacteritics = {
        DR_SectionCharacteristicCode                ,
        DR_SectionCharacteristicInitializedData     ,
        DR_SectionCharacteristicUninitializedData   ,
        DR_SectionCharacteristicLnkInfo             ,
        DR_SectionCharacteristicLnkRemove           ,
        DR_SectionCharacteristicLnkComdat           ,
        DR_SectionCharacteristicNoDeferSpecExc      ,
        DR_SectionCharacteristicFarData             ,
        DR_SectionCharacteristicMemPurgeable        ,
        DR_SectionCharacteristicMemLocked           ,
        DR_SectionCharacteristicMemPreload
    };
    for (auto currentCharacteristic = allCharacteritics.begin(); currentCharacteristic != allCharacteritics.end(); currentCharacteristic++)
    {
        if (sectionHeader->Characteristics & *currentCharacteristic)
        {
            this->sectionCaracteristics.push_back(*currentCharacteristic);
        }
    }

    this->sectionName = std::move(std::wstring(sectionNamePtr.get(), sizeof(sectionHeader->Name)));
    this->sectionRVA = sectionHeader->VirtualAddress;
    this->sectionSize = sectionHeader->SizeOfRawData;

    initEntry.SetSuccess();
}