#pragma once
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"
#include "DR_PEParserInternal.h"

class DR_BaseResourceNodeDirectory
{
protected:
	bool isInit;
	DR_ResourceType nodeType;
	std::vector<DR_ShPtrResourceNode> nodes;

public:
	DR_BaseResourceNodeDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	virtual ~DR_BaseResourceNodeDirectory() = default;
};

class DR_BaseResourceNodeData
{
protected:
	bool isInit;
	DR_ResourceType nodeType;
	DWORD dataOffset;
	std::uint32_t dataSize;
	std::vector<BYTE> data;
	DWORD codePage;

public:
	DR_BaseResourceNodeData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	virtual ~DR_BaseResourceNodeData() = default;
};

class DR_ResourceNodeNamedDirectory : public DR_IResourceNodeNamedDirectory, public DR_BaseResourceNodeDirectory
{
private:
	std::wstring name;

public:
	DR_ResourceNodeNamedDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	inline virtual const std::wstring& GetName() const override { return this->name; }
	inline virtual DR_ResourceType GetNodeType() const override { return this->nodeType; }
	inline virtual const std::vector<DR_ShPtrResourceNode>& GetNodes() const override { return this->nodes; }
	inline virtual bool IsInit() const override { return this->isInit; }
};
class DR_ResourceNodeIdDirectory : public DR_IResourceNodeIdDirectory, public DR_BaseResourceNodeDirectory
{
private:
	std::uint32_t id;

public:
	DR_ResourceNodeIdDirectory(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	inline virtual std::uint32_t GetId() const override { return this->id; }
	inline virtual DR_ResourceType GetNodeType() const override { return this->nodeType; }
	inline virtual const std::vector<DR_ShPtrResourceNode>& GetNodes() const override { return this->nodes; }
	inline virtual bool IsInit() const override { return this->isInit; }
};
class DR_ResourceNodeNamedData : public DR_IResourceNodeNamedData, public DR_BaseResourceNodeData
{
private:
	std::wstring name;

public:
	DR_ResourceNodeNamedData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	inline virtual const std::wstring& GetName() const override { return this->name; }
	inline virtual DR_ResourceType GetNodeType() const override { return this->nodeType; }
	inline virtual bool IsInit() const override { return this->isInit; }
	inline virtual DWORD GetDataRVA() const override { return this->dataOffset; }
	inline virtual std::uint32_t GetDataSize() const override { return this->dataSize; }
	inline virtual const std::vector<BYTE>& GetData() const override { return this->data; }
	inline virtual DWORD GetCodePage() const override { return this->codePage; }
};
class DR_ResourceNodeIdData : public DR_IResourceNodeIdData, public DR_BaseResourceNodeData
{
private:
	std::uint32_t id;
	
public:
	DR_ResourceNodeIdData(const LPVOID EntryNodeAddress, const LPVOID ResourceBase, const LPVOID ModuleBase);
	inline virtual std::uint32_t GetId() const override { return this->id; }
	inline virtual DR_ResourceType GetNodeType() const override { return this->nodeType; }
	inline virtual bool IsInit() const override { return this->isInit; }
	inline virtual DWORD GetDataRVA() const override { return this->dataOffset; }
	inline virtual std::uint32_t GetDataSize() const override { return this->dataSize; }
	inline virtual const std::vector<BYTE>& GetData() const override { return this->data; }
	inline virtual DWORD GetCodePage() const override { return this->codePage; }
};

class DR_ResourceNTHeaderInfo : public DR_IResourceNTHeaderInfo
{
private:
	DWORD headerRVA;
	std::int32_t headerSize;
	bool isHeaderPresent;
	DR_NTHeader headerType;
	std::vector<DR_ShPtrResourceNode> mainNodeData;
	
public:
	DR_ResourceNTHeaderInfo(const LPVOID ModuleAddress);
	inline virtual DWORD GetHeaderRVA() const override { return this->headerRVA; }
	inline virtual std::int32_t GetHeaderSize() const override { return this->headerSize; }
	inline virtual bool IsHeaderPresent() const override { return this->isHeaderPresent; }
	inline virtual DR_NTHeader GetHeaderType() const override { return this->headerType; }
	inline virtual const std::vector<DR_ShPtrResourceNode>& GetMainNodeData() const override { return this->mainNodeData; }
};
