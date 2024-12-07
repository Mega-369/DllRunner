#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_ManagerInternal.h"

class DR_PELoader
{
private:
	bool isLastOperationSuccess;
	HMODULE PEDataPtr;
	std::wstring fileName;

public:
	DR_PELoader();
	~DR_PELoader();
	void LoadPEFromFile(const std::wstring& FilePath);
	void Unload();
	inline bool IsLastOperationSuccess() const { return this->isLastOperationSuccess; }
	inline bool IsPELoaded() const { return this->PEDataPtr != NULL; }
	inline const LPVOID GetPEData() const { return this->PEDataPtr; }
	inline const std::wstring& GetFileName() const { return this->fileName; }

private:
	void unload();
};
typedef std::unique_ptr<DR_PELoader> DR_UnPtrPELoader;
