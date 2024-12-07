#include "DR_PELoader.h"

DR_PELoader::DR_PELoader() :
	isLastOperationSuccess(true),
	PEDataPtr(nullptr)
{
}

DR_PELoader::~DR_PELoader()
{
	this->unload();
}

void DR_PELoader::LoadPEFromFile(const std::wstring& FilePath)
{
	DR_OperationResultEntry resEntry(this->isLastOperationSuccess);
	this->unload();

	this->PEDataPtr = LoadLibraryExW(FilePath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (this->PEDataPtr == NULL)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to load data from PE. "));
		return;
	}

	resEntry.SetSuccess();
}

void DR_PELoader::Unload()
{
	DR_OperationResultEntry resEntry(this->isLastOperationSuccess);
	this->unload();
	resEntry.SetSuccess();
}

void DR_PELoader::unload()
{
	if (this->PEDataPtr != NULL)
	{
		FreeLibrary(this->PEDataPtr);
		this->PEDataPtr = NULL;
	}

	this->fileName.clear();
}
