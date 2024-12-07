#include "DR_ExportNTHeaderInfo.h"

DR_ExportNTHeaderInfo::DR_ExportNTHeaderInfo(const LPVOID ModuleAddress) :
    headerRVA(NULL),
    headerSize(0),
    isHeaderPresent(false),
    headerType(DR_NTHeaderExport),
    ordinalBase(0)
{
    DR_OperationResultEntry resEntry(this->isHeaderPresent);

    const PIMAGE_DOS_HEADER DOSHeaderPtr = reinterpret_cast<const PIMAGE_DOS_HEADER>(ModuleAddress);
    if (DOSHeaderPtr == NULL)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid DOS header. "));
        return;
    }

    const PIMAGE_NT_HEADERS NTHeadersPtr = reinterpret_cast<const PIMAGE_NT_HEADERS>((PBYTE)ModuleAddress + DOSHeaderPtr->e_lfanew);
    if (NTHeadersPtr == NULL)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid NT header. "));
        return;
    }

    this->headerRVA = NTHeadersPtr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    this->headerSize = NTHeadersPtr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    if (this->headerRVA == NULL || this->headerSize == 0)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid header. "));
        return;
    }

    const PIMAGE_EXPORT_DIRECTORY exportDirectory = reinterpret_cast<const PIMAGE_EXPORT_DIRECTORY>((PBYTE)ModuleAddress + this->headerRVA);
    
    PDWORD addressOfFunctions = reinterpret_cast<PDWORD>((PBYTE)ModuleAddress + exportDirectory->AddressOfFunctions);
    PDWORD addressOfNames = reinterpret_cast<PDWORD>((PBYTE)ModuleAddress + exportDirectory->AddressOfNames);
    PWORD addressOfOrdinals = reinterpret_cast<PWORD>((PBYTE)ModuleAddress + exportDirectory->AddressOfNameOrdinals);

    for (int currentFuncIndex = 0; currentFuncIndex < exportDirectory->NumberOfFunctions; currentFuncIndex++)
    {
        DWORD currentFuncRVA = addressOfFunctions[currentFuncIndex];
        if (currentFuncRVA != NULL)
        {
            DWORD currentOrdinal = exportDirectory->Base + currentFuncIndex;
            this->ordinals.insert(std::make_pair(currentOrdinal, currentFuncRVA));
        }
    }

    for (int currentNameIndex = 0; currentNameIndex < exportDirectory->NumberOfNames; currentNameIndex++)
    {
        WORD currentOrdinal = addressOfOrdinals[currentNameIndex];
        DWORD nameRVA = addressOfNames[currentNameIndex];
        std::string functionName = (LPCSTR)((PBYTE)ModuleAddress + nameRVA);
        DWORD functionRVA = addressOfFunctions[currentOrdinal];
        this->functionNames.insert(std::make_pair(std::wstring(functionName.begin(), functionName.end()), functionRVA));
    }
    
    DR_TRACE->TraceInformation(TEXT(__FUNCTION__": Export header was init. "));
    resEntry.SetSuccess();
}
