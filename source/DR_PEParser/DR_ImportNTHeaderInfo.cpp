#include "DR_ImportNTHeaderInfo.h"

DR_ImportNTHeaderInfo::DR_ImportNTHeaderInfo(const LPVOID ModuleAddress) :
    isHeaderPresent(false),
    headerRVA(NULL),
    headerSize(0),
    headerType(DR_NTHeaderImport)
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

    this->headerRVA = NTHeadersPtr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    this->headerSize = NTHeadersPtr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
    if (this->headerRVA == NULL || this->headerSize == 0)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid header. "));
        return;
    }

    PIMAGE_IMPORT_DESCRIPTOR importDescriporPtr = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>((PBYTE)ModuleAddress + this->headerRVA);
    DR_ShPtrImportInfo currentImportPtr = nullptr;
    do
    {
        currentImportPtr = std::make_shared<DR_ImportInfo>(ModuleAddress, importDescriporPtr);
        if (currentImportPtr->IsInit())
        {
            this->imports.push_back(currentImportPtr);
        }
        else
        {
            currentImportPtr.reset(); // todo check
        }

        importDescriporPtr++;
    } 
    while (currentImportPtr.get() != nullptr);

    resEntry.SetSuccess();
}

DR_ImportInfo::DR_ImportInfo(LPVOID ModuleAddress, LPVOID ImportDescripor)
{
    DR_OperationResultEntry resEntry(this->isInit);

    PIMAGE_IMPORT_DESCRIPTOR importDescriptorPtr = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(ImportDescripor);
    if (importDescriptorPtr == nullptr)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid import descriptor. "));
        return;
    }

    if (importDescriptorPtr->Name == 0)
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to analize import. "));
        return;
    }

    std::string askiName = reinterpret_cast<const char*>((PBYTE)ModuleAddress + importDescriptorPtr->Name);
    this->name = std::move(std::wstring(askiName.begin(), askiName.end()));
    if (this->name.empty())
    {
        DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid import name. "));
        return;
    }

    PIMAGE_THUNK_DATA currentThunkPtr = reinterpret_cast<PIMAGE_THUNK_DATA>((PBYTE)ModuleAddress + importDescriptorPtr->OriginalFirstThunk);
    while (currentThunkPtr->u1.AddressOfData != NULL)
    {
        if (currentThunkPtr->u1.Ordinal & IMAGE_ORDINAL_FLAG)
        {
            WORD functionOrdinal = currentThunkPtr->u1.Ordinal & 0xFFFF;
            this->functionOrdinals.push_back(functionOrdinal);
        }
        else
        {
            PIMAGE_IMPORT_BY_NAME namedImportPtr = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>((PBYTE)ModuleAddress + currentThunkPtr->u1.AddressOfData);
            std::string askiFunctionName = namedImportPtr->Name;
            std::wstring functionName(askiFunctionName.begin(), askiFunctionName.end());
            this->functionNames.push_back(std::move(functionName));
        }

        currentThunkPtr++;
    }

    resEntry.SetSuccess();
}
