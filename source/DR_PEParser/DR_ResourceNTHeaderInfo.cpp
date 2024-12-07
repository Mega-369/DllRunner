#include "DR_ResourceNTHeaderInfo.h"

DR_ResourceNTHeaderInfo::DR_ResourceNTHeaderInfo(const LPVOID ModuleAddress) :
	headerType(DR_NTHeaderResource),
	headerRVA(NULL),
	headerSize(0),
	isHeaderPresent(false)
{
	DR_OperationResultEntry resEntry(this->isHeaderPresent);

	const PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<const PIMAGE_DOS_HEADER>(ModuleAddress);
	if (DOSHeader == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Ivalid DOS header. "));
		return;
	}

	const PIMAGE_NT_HEADERS NTHeader = reinterpret_cast<const PIMAGE_NT_HEADERS>((PBYTE)ModuleAddress + DOSHeader->e_lfanew);
	if (NTHeader == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid NT header. "));
		return;
	}

	this->headerSize = NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size;
	this->headerRVA = NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	if (this->headerSize == 0 || this->headerRVA == NULL)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid resource header. "));
		return;
	}

	const PIMAGE_RESOURCE_DIRECTORY resourceHeaderPtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY>((PBYTE)ModuleAddress + this->headerRVA);
	const PIMAGE_RESOURCE_DIRECTORY_ENTRY mainNodePtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(resourceHeaderPtr + 1);
	for (int index = 0; index < resourceHeaderPtr->NumberOfNamedEntries + resourceHeaderPtr->NumberOfIdEntries; index++)
	{
		DR_ShPtrResourceNode currentNode = nullptr;
		if (mainNodePtr->DataIsDirectory && mainNodePtr->NameIsString)
		{
			currentNode = std::make_shared<DR_ResourceNodeNamedDirectory>(&mainNodePtr[index], resourceHeaderPtr, ModuleAddress);
		}
		else if (!mainNodePtr->DataIsDirectory && mainNodePtr->NameIsString)
		{
			currentNode = std::make_shared<DR_ResourceNodeNamedData>(&mainNodePtr[index], resourceHeaderPtr, ModuleAddress);
		}
		else if (mainNodePtr->DataIsDirectory && !mainNodePtr->NameIsString)
		{
			currentNode = std::make_shared<DR_ResourceNodeIdDirectory>(&mainNodePtr[index], resourceHeaderPtr, ModuleAddress);
		}
		else
		{
			currentNode = std::make_shared<DR_ResourceNodeIdData>(&mainNodePtr[index], resourceHeaderPtr, ModuleAddress);
		}

		if (currentNode != nullptr)
		{
			this->mainNodeData.push_back(std::move(currentNode));
		}
	}

	DR_TRACE->TraceInformation(TEXT(__FUNCTION__": Resource header was init successfully. "));
	resEntry.SetSuccess();
}

DR_BaseResourceNodeDirectory::DR_BaseResourceNodeDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	nodeType(DR_ResourceTypeUnknownEntry),
	isInit(false)
{
	DR_OperationResultEntry resEntry(this->isInit);

	const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentEntryNodePtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
	if (currentEntryNodePtr == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid directory entry. "));
		return;
	}

	if (!currentEntryNodePtr->DataIsDirectory)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Incorrect node type. "));
		return;
	}

	if (currentEntryNodePtr->NameIsString) 
	{
		this->nodeType = DR_ResourceTypeNamedDirectoryEntry;
	}
	else
	{
		this->nodeType = DR_ResourceTypeIdDirectoryEntry;
	}

	PIMAGE_RESOURCE_DIRECTORY directoryNodePtr = reinterpret_cast<PIMAGE_RESOURCE_DIRECTORY>((PBYTE)ResourceBase + currentEntryNodePtr->OffsetToDirectory);
	if (directoryNodePtr == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid directory value. "));
		return;
	}

	
	PIMAGE_RESOURCE_DIRECTORY_ENTRY childEntryNodeArr = reinterpret_cast<PIMAGE_RESOURCE_DIRECTORY_ENTRY>(directoryNodePtr + 1);
	if (childEntryNodeArr == nullptr)
	{
		DR_TRACE->TraceWarning(TEXT(__FUNCTION__": Empty directory. "));
		resEntry.SetSuccess();
		return;
	}

	int numOfEntrys = directoryNodePtr->NumberOfIdEntries + directoryNodePtr->NumberOfNamedEntries;
	for (int index = 0; index < numOfEntrys; index++)
	{
		DR_ShPtrResourceNode chiledNodePtr = nullptr;
		if (childEntryNodeArr[index].DataIsDirectory && childEntryNodeArr[index].NameIsString)
		{
			chiledNodePtr = std::make_shared<DR_ResourceNodeNamedDirectory>(&childEntryNodeArr[index], ResourceBase, ModuleBase);
		}
		else if (!childEntryNodeArr[index].DataIsDirectory && childEntryNodeArr[index].NameIsString)
		{
			chiledNodePtr = std::make_shared<DR_ResourceNodeNamedData>(&childEntryNodeArr[index], ResourceBase, ModuleBase);
		}
		else if (childEntryNodeArr[index].DataIsDirectory && !childEntryNodeArr[index].NameIsString)
		{
			chiledNodePtr = std::make_shared<DR_ResourceNodeIdDirectory>(&childEntryNodeArr[index], ResourceBase, ModuleBase);
		}
		else
		{
			chiledNodePtr = std::make_shared<DR_ResourceNodeIdData>(&childEntryNodeArr[index], ResourceBase, ModuleBase);
		}

		if (chiledNodePtr != nullptr)
		{
			this->nodes.push_back(std::move(chiledNodePtr));
		}
	}

	resEntry.SetSuccess();
}

DR_ResourceNodeNamedDirectory::DR_ResourceNodeNamedDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	DR_BaseResourceNodeDirectory(EntryNodeAddress, ResourceBase, ModuleBase)
{
	if (this->IsInit())
	{
		const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentEntryNodePtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
		std::string nameASKII = reinterpret_cast<LPCSTR>((PBYTE)ResourceBase + currentEntryNodePtr->NameOffset);
		this->name = std::move(std::wstring(nameASKII.begin(), nameASKII.end()));
	}
}

DR_ResourceNodeIdDirectory::DR_ResourceNodeIdDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	DR_BaseResourceNodeDirectory(EntryNodeAddress, ResourceBase, ModuleBase),
	id(0)
{
	if (this->IsInit())
	{
		const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentEntryNodePtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
		this->id = currentEntryNodePtr->Id;
	}
}

DR_BaseResourceNodeData::DR_BaseResourceNodeData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	nodeType(DR_ResourceTypeUnknownEntry),
	isInit(false),
	dataOffset(0),
	dataSize(0),
	codePage(0)
{
	DR_OperationResultEntry resEntry(this->isInit);

	const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentEntryPtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
	if (currentEntryPtr == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid recource directory entry. "));
		return;
	}

	if (currentEntryPtr->DataIsDirectory)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Incorrect node type. "));
		return;
	}

	if (currentEntryPtr->NameIsString)
	{
		this->nodeType = DR_ResourceTypeNamedDataEntry;
	}
	else
	{
		this->nodeType = DR_ResourceTypeIdDataEntry;
	}
	
	const PIMAGE_RESOURCE_DATA_ENTRY dataEntryPtr = reinterpret_cast<const PIMAGE_RESOURCE_DATA_ENTRY>((PBYTE)ResourceBase + currentEntryPtr->OffsetToData);
	if (dataEntryPtr == nullptr)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to cast data entry. "));
		return;
	}

	this->dataSize = dataEntryPtr->Size;
	this->dataOffset = dataEntryPtr->OffsetToData;
	this->codePage = dataEntryPtr->CodePage;
	if (this->dataOffset != NULL)
	{
		PBYTE dataPtr = (PBYTE)ModuleBase + this->dataOffset;
		this->data.resize(this->dataSize);
		memcpy(this->data.data(), dataPtr, this->dataSize);
	}
	
	resEntry.SetSuccess();
}

DR_ResourceNodeNamedData::DR_ResourceNodeNamedData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	DR_BaseResourceNodeData(EntryNodeAddress, ResourceBase, ModuleBase)
{
	if (this->IsInit())
	{
		const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentNodeEntryPtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
		std::string nameASKII = reinterpret_cast<LPCSTR>((PBYTE)ResourceBase + currentNodeEntryPtr->NameOffset);
		this->name = std::move(std::wstring(nameASKII.begin(), nameASKII.end()));
	}
}

DR_ResourceNodeIdData::DR_ResourceNodeIdData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase) :
	DR_BaseResourceNodeData(EntryNodeAddress, ResourceBase, ModuleBase),
	id(0)
{
	if (this->IsInit())
	{
		const PIMAGE_RESOURCE_DIRECTORY_ENTRY currentNodeEntryPtr = reinterpret_cast<const PIMAGE_RESOURCE_DIRECTORY_ENTRY>(EntryNodeAddress);
		this->id = currentNodeEntryPtr->Id;
	}
}
