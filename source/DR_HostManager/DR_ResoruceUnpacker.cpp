#include "DR_ResoruceUnpacker.h"

std::optional<std::vector<BYTE>> DR_ResoruceUnpacker::UnpackBinResource(const std::wstring& ResourceName) const
{
	auto sectionCount = this->getBinSectionCount(ResourceName);
	if (!sectionCount.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to get resource \"%ws\". "), ResourceName.c_str());
		return std::nullopt;
	}

	auto decompressedSize = this->getDecompressedSize(ResourceName);
	if (!decompressedSize.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to get decompressed data size of resource \"%ws\". "), ResourceName.c_str());
		return std::nullopt;
	}

	bool bRes = true;
	std::vector<BYTE> dataBuffer;
	for (int currentSectionIndex = 1; currentSectionIndex <= *sectionCount; currentSectionIndex++)
	{
		auto dataSection = this->getDataSection(ResourceName, currentSectionIndex);
		if (!dataSection.has_value())
		{
			DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to get section %d of resource \"%ws\". "), currentSectionIndex, ResourceName.c_str());
			break;
		}
		dataBuffer.insert(dataBuffer.end(), dataSection->begin(), dataSection->end());
	}
	if (!bRes)
	{
		return std::nullopt;
	}

	auto decompressedBuffer = this->decompressedData(dataBuffer, *decompressedSize);
	if (!decompressedBuffer.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to decompressed data. "));
		return std::nullopt;
	}
	
	return std::move(*decompressedBuffer);
}

std::optional<std::pair<LPVOID, DWORD>> DR_ResoruceUnpacker::loadResource(const std::wstring& ResourceName) const
{
	HRSRC hRec = FindResourceW(NULL, ResourceName.c_str(), RT_RCDATA);
	if (hRec == NULL)
	{
		//todo GetLastError
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to find resource. "));
		return std::nullopt;
	}

	DWORD dwSize = SizeofResource(NULL, hRec);
	if (dwSize == 0)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Empty recource. "));
		return std::nullopt;
	}

	HGLOBAL hLoadedResource = LoadResource(NULL, hRec);
	if (hLoadedResource == NULL)
	{
		//todo GetLastError
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to load resource. "));
		return std::nullopt;
	}

	LPVOID resourceDataPtr = LockResource(hLoadedResource);
	if (resourceDataPtr == NULL)
	{
		//todo GetLastError
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to lock recource. "));
		return std::nullopt;
	}
	
	auto outRes = std::make_pair(resourceDataPtr, dwSize);
	return std::move(outRes);
}

std::optional<WORD> DR_ResoruceUnpacker::getBinSectionCount(const std::wstring& ResourceName) const
{
	auto loadedResource = this->loadResource(ResourceName);
	if (!loadedResource.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to load resource \"%ws\". "), ResourceName.c_str());
		return std::nullopt;
	}

	if (loadedResource->second != sizeof(WORD))
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid size of resource \"%ws\". "), ResourceName.c_str());
		return std::nullopt;
	}

	WORD sectionCount = *reinterpret_cast<const WORD*>(loadedResource->first);
	return sectionCount;
}

std::optional<DWORD> DR_ResoruceUnpacker::getDecompressedSize(const std::wstring& ResourceName) const
{
	std::wstring preparedName = ResourceName;
	preparedName.append(TEXT("D"));

	auto loadedResource = this->loadResource(preparedName);
	if (!loadedResource.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to loaded resource \"%ws\". "), preparedName.c_str());
		return std::nullopt;
	}

	if (loadedResource->second != sizeof(DWORD))
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Ivalid size of resource \"%ws\". "), preparedName.c_str());
		return std::nullopt;
	}

	DWORD decompressedSize = *reinterpret_cast<const DWORD*>(loadedResource->first);
	return decompressedSize;
}

std::optional<std::vector<BYTE>> DR_ResoruceUnpacker::getDataSection(const std::wstring& ResourceName, unsigned int SectionId) const
{
	std::wstring preparedName = ResourceName;
	preparedName.append(std::to_wstring(SectionId));

	auto loadedResource = this->loadResource(preparedName);
	if (!loadedResource.has_value())
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to load resource \"%ws\". "), preparedName.c_str());
		return std::nullopt;
	}

	std::vector<WORD> rawData(
		static_cast<const WORD*>(loadedResource->first),
		static_cast<const WORD*>(loadedResource->first) + loadedResource->second / sizeof(WORD)
		);
	std::vector<BYTE> preparedData;
	preparedData.reserve(loadedResource->second / sizeof(WORD));
	for (auto currentByte : rawData)
	{
		preparedData.push_back((BYTE)currentByte);
	}

	return std::move(preparedData);
}

std::optional<std::vector<BYTE>> DR_ResoruceUnpacker::decompressedData(const std::vector<BYTE>& Data, DWORD DecompressedImageSize) const
{
	if (Data.size() == 0 || DecompressedImageSize == 0)
	{
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Invalid args. "));
		return std::nullopt;
	}

	std::vector<BYTE> decompressedData(DecompressedImageSize);
	DWORD decompressedDataSize = decompressedData.size();

	int res = uncompress(decompressedData.data(), &decompressedDataSize, Data.data(), Data.size());
	while (res == Z_BUF_ERROR)
	{
		DR_TRACE->TraceWarning(TEXT(__FUNCTION__": To small decompressed buffer - %d. "), decompressedDataSize);
		decompressedDataSize = decompressedDataSize * 4 / 3;
		decompressedData.resize(decompressedDataSize);
		res = uncompress(decompressedData.data(), &decompressedDataSize, Data.data(), Data.size());
		DR_TRACE->TraceInformation(TEXT(__FUNCTION__": New decompressed buffer size - %d. "), decompressedDataSize);
	}
	if (res != Z_OK)
	{
		//todo errorRepord
		DR_TRACE->TraceError(TEXT(__FUNCTION__": Failed to decompressed data. "));
		return std::nullopt;
	}

	return std::move(decompressedData);
}
