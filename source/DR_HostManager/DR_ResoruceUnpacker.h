#pragma once
#include "stdafx.h"
#include  <zlib.h>

#include "DR_HostManagerInternal.h"

class DR_ResoruceUnpacker
{
public:
	std::optional<std::vector<BYTE>> UnpackBinResource(const std::wstring& ResourceName) const;

private:
	std::optional<std::pair<LPVOID, DWORD>> loadResource(const std::wstring& ResourceName) const;

	std::optional<WORD> getBinSectionCount(const std::wstring& ResourceName) const;
	std::optional<DWORD> getDecompressedSize(const std::wstring& ResourceName) const;
	std::optional<std::vector<BYTE>> getDataSection(const std::wstring& ResourceName, unsigned int SectionId) const;
	std::optional<std::vector<BYTE>> decompressedData(const std::vector<BYTE>& Data, DWORD DecompressedImageSize) const;
};
typedef std::unique_ptr<DR_ResoruceUnpacker> DR_UnPtrResoruceUnpacker;

