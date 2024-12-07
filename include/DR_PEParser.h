#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"

#ifdef DR_PE_PARSER_STATIC
#define DR_PE_PARSER_CLASS
#define DR_PE_PARSER_FUNC
#else
#ifdef DR_PE_PARSER_EXPORT
#define DR_PE_PARSER_CLASS __declspec(dllexport)
#define DR_PE_PARSER_FUNC __declspec(dllexport)
#else
#define DR_PE_PARSER_CLASS __declspec(dllimport)
#define DR_PE_PARSER_FUNC __declspec(dllimport)
#endif
#endif

enum DR_PE_PARSER_CLASS DR_NTHeader
{
	DR_NTHeaderExport = 0		,
	DR_NTHeaderImport = 1		,
	DR_NTHeaderResource = 2		,
	DR_NTHeaderException = 3	,
	DR_NTHeaderSecurity = 4		,
	DR_NTHeaderBaseReloc = 5	,
	DR_NTHeaderDebug = 6		,
	/*DR_NTHeaderCopyright = 7,
	DR_NTHeaderArchitecture = 7,*/
	DR_NTHeaderGlobalPtr = 8	,
	DR_NTHeaderTLS = 9			,
	DR_NTHeaderLoadConfig = 10
};

enum DR_PE_PARSER_CLASS DR_Machine
{
	DR_MachineUnknown = 0x0000		,
	DR_MachineTargetHost = 0x0001	,
	DR_MachineI386 = 0x014C			,
	DR_MachineR3000 = 0x0162		,
	DR_MachineR4000 = 0x0166		,
	DR_MachineR10000 = 0x0168		,
	DR_MachineWCEMIPSV2 = 0x0169	,
	DR_MachineALPHA = 0x0184		,
	DR_MachineSH3 = 0x01A2			,
	DR_MachineSH3DSP = 0x01A3		,
	DR_MachineSH3E = 0x01A4			,
	DR_MachineSH4 = 0x01A8			,
	DR_MachineSH5 = 0x01A8			,
	DR_MachineARM = 0x01C0			,
	DR_MachineTHUMB = 0x01C2		,
	DR_MachineARMNT = 0x01C4		,
	DR_MachineAM33 = 0x01D3			,
	DR_MachineCEE = 0xC0EE			,
	DR_MachineARM64 = 0xAA64		,
	DR_MachineM32R = 0x9041			,
	DR_MachineAMD64 = 0x8664		,
	DR_MachineEBC = 0x0EBC			,
	DR_MachineCEF = 0x0CEF			,
	DR_MachineTRICORE = 0x0520		,
	DR_MachineMIPSFPU16 = 0x0466	,
	DR_MachineMIPSFPU = 0x0366		,
	DR_MachineALPHA64 = 0x0284		,
	DR_MachineMIPS16 = 0x0266		,
	DR_MachineIA64 = 0x0200			,
	DR_MachinePOWERPCFP = 0x01f1	,
	DR_MachinePOWERPC = 0x01f0
};

enum DR_PE_PARSER_CLASS DR_SectionCharacteristic
{
	DR_SectionCharacteristicCode = 32				,
	DR_SectionCharacteristicInitializedData = 64	,
	DR_SectionCharacteristicUninitializedData = 128	,
	DR_SectionCharacteristicLnkInfo = 512			,
	DR_SectionCharacteristicLnkRemove = 2048		,
	DR_SectionCharacteristicLnkComdat = 4096		,
	DR_SectionCharacteristicNoDeferSpecExc = 16384	,
	DR_SectionCharacteristicFarData = 32768			,
	DR_SectionCharacteristicMemPurgeable = 131072	,
	DR_SectionCharacteristicMemLocked = 262144		,
	DR_SectionCharacteristicMemPreload = 524288
};

enum DR_PE_PARSER_CLASS DR_DllCharacteristic
{
	DR_DllCharacteristicHighEntropyVa = 32			,
	DR_DllCharacteristicDynamicBase = 64			,
	DR_DllCharacteristicForceIntegrity = 128		,
	DR_DllCharacteristicNXCompat = 256				,
	DR_DllCharacteristicNoIsolation = 512			,
	DR_DllCharacteristicNoSEH = 1024				,
	DR_DllCharacteristicNoBind = 2048				,
	DR_DllCharacteristicAppcontainer = 4096			,
	DR_DllCharacteristicWDMDriver = 8192			,
	DR_DllCharacteristicGuardCF = 16384				,
	DR_DllCharacteristicTerminalServerAware = 32768
};

enum DR_PE_PARSER_CLASS DR_FileCharacteristic
{
	DR_FileCharacteristicRelocsStripped = 1			,
	DR_FileCharacteristicExecutableIamage = 2		,
	DR_FileCharacteristicLineNumsStripped = 4		,
	DR_FileCharacteristicLocalSymsStripped = 8		,
	DR_FileCharacteristicAggresiveWsTrim = 16		,
	DR_FileCharacteristicLargeAddressAware = 32		,
	DR_FileCharacteristicBytesReversedLo = 128		,
	DR_FileCharacteristic32BitMachine = 256			,
	DR_FileCharacteristicDebugStripped = 512		,
	DR_FileCharacteristicRemovableRunFromSwap = 1024,
	DR_FileCharacteristicNetRunFromSwap = 2048		,
	DR_FileCharacteristicSystem = 4096				,
	DR_FileCharacteristicDll = 8192					,
	DR_FileCharacteristicUpSystemOnly = 16384		,
	DR_FileCharacteristicBytesReversedHI = 32768
};

enum DR_PE_PARSER_CLASS DR_Subsystem
{
	DR_SubsystemUnknown = 0					,
	DR_SubsystemNative = 1					,
	DR_SubsystemWindowsGUI = 2				,
	DR_SubsystemWindowsCUI = 3				,
	DR_SubsystemOS2CUI = 5					,
	DR_SubsystemPOSIXCUI = 7				,
	DR_SubsystemNativeWindows = 8			,
	DR_SubsystemWindowsCEGUI = 9			,
	DR_SubsystemEFIApplication = 10			,
	DR_SubsystemEFIBootServiceDriver = 11	,
	DR_SubsystemRuntimeDriver = 12			,
	DR_SubsystemEFIROM = 13					,
	DR_SubsystemXBOX = 14					,
	DR_SubsystemWindowsBootApplication = 16	,
	DR_SubsystemXBOXCodeCatalog = 17
};

enum DR_PE_PARSER_CLASS DR_ResourceType
{
	DR_ResourceTypeUnknownEntry = 0			,
	DR_ResourceTypeNamedDataEntry = 1		,
	DR_ResourceTypeNamedDirectoryEntry = 2	,
	DR_ResourceTypeIdDataEntry = 3			,
	DR_ResourceTypeIdDirectoryEntry = 4
};

class DR_PE_PARSER_CLASS DR_INTHeaderInfo
{
public:
	virtual DWORD GetHeaderRVA() const = 0;
	virtual std::int32_t GetHeaderSize() const = 0;
	virtual bool IsHeaderPresent() const = 0;
	virtual DR_NTHeader GetHeaderType() const = 0;
	virtual ~DR_INTHeaderInfo() = default;
};
typedef std::unique_ptr<DR_INTHeaderInfo> DR_UnPtrNTHeaderInfo;
typedef std::shared_ptr<DR_INTHeaderInfo> DR_ShPtrNTHeaderInfo;

//-------------------------------------------------------------------------------
/*
	class for export NT header
*/
class DR_PE_PARSER_CLASS DR_IExportNTHeaderInfo : public DR_INTHeaderInfo
{
public:
	virtual const std::map<WORD, DWORD>& GetOrdinals() const = 0;
	virtual const std::map<std::wstring, DWORD>& GetFunctionNames() const = 0;
	virtual std::uint32_t GetOrdinalBase() const = 0;
	virtual ~DR_IExportNTHeaderInfo() = default;
};

//-------------------------------------------------------------------------------
/*
	classes for import NT header
*/
class DR_PE_PARSER_CLASS DR_IImportInfo
{
public:
	virtual const std::wstring& GetName() const = 0;
	virtual const std::vector<std::wstring>& GetFunctionNames() const = 0;
	virtual const std::vector<WORD>& GetFunctionOrdinals() const = 0;
	virtual bool IsInit() const = 0;
	virtual ~DR_IImportInfo() = default;
};
typedef std::shared_ptr<DR_IImportInfo> DR_ShPtrImportInfo;

class DR_PE_PARSER_CLASS DR_IImportNTHeaderInfo : public DR_INTHeaderInfo
{
public:
	virtual const std::vector<DR_ShPtrImportInfo>& GetImports() const = 0;
	virtual ~DR_IImportNTHeaderInfo() = default;
};

//-------------------------------------------------------------------------------
/*
	Classes for resource NT header 
*/
class DR_PE_PARSER_CLASS DR_IResourceNode
{
public:
	virtual bool IsInit() const = 0;
	virtual DR_ResourceType GetNodeType() const = 0;
	virtual ~DR_IResourceNode() = default;
};
typedef std::shared_ptr<DR_IResourceNode> DR_ShPtrResourceNode;

class DR_PE_PARSER_CLASS DR_IResourceNodeNamedDirectory : public DR_IResourceNode
{
public:
	virtual const std::vector<DR_ShPtrResourceNode>& GetNodes() const = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual ~DR_IResourceNodeNamedDirectory() = default;
};
class DR_PE_PARSER_CLASS DR_IResourceNodeIdDirectory : public DR_IResourceNode
{
public:
	virtual const std::vector<DR_ShPtrResourceNode>& GetNodes() const = 0;
	virtual std::uint32_t GetId() const = 0;
	virtual ~DR_IResourceNodeIdDirectory() = default;
};
class DR_PE_PARSER_CLASS DR_IResourceNodeNamedData : public DR_IResourceNode
{
public:
	virtual DWORD GetDataRVA() const = 0;
	virtual std::uint32_t GetDataSize() const = 0;
	virtual const std::vector<BYTE>& GetData() const = 0;
	virtual DWORD GetCodePage() const = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual ~DR_IResourceNodeNamedData() = default;
};
class DR_PE_PARSER_CLASS DR_IResourceNodeIdData : public DR_IResourceNode
{
public:
	virtual DWORD GetDataRVA() const = 0;
	virtual std::uint32_t GetDataSize() const = 0;
	virtual const std::vector<BYTE>& GetData() const = 0;
	virtual DWORD GetCodePage() const = 0;
	virtual std::uint32_t GetId() const = 0;
	virtual ~DR_IResourceNodeIdData() = default;
};

class DR_PE_PARSER_CLASS DR_IResourceNTHeaderInfo : public DR_INTHeaderInfo
{
public:
	virtual const std::vector<DR_ShPtrResourceNode>& GetMainNodeData() const = 0;
	virtual ~DR_IResourceNTHeaderInfo() = default;
};

//-------------------------------------------------------------------------------
/*
	Section info class, for example .data, .text, etc
*/
class DR_PE_PARSER_CLASS DR_ISectionInfo
{
public:
	virtual const std::vector<DR_SectionCharacteristic>& GetSectionCharacteristics() const = 0;
	virtual const std::wstring& GetSectionName() const = 0;
	virtual std::int32_t GetSectionRVA() const = 0;
	virtual std::int32_t GetSectionSize() const = 0;
	virtual bool IsInit() const = 0;
	virtual ~DR_ISectionInfo() = default;
};
typedef std::shared_ptr<DR_ISectionInfo> DR_ShPtrSectionInfo;
typedef std::unique_ptr<DR_ISectionInfo> DR_UnPtrSectionInfo;

//-------------------------------------------------------------------------------
/*
	Common PE info class
*/
class DR_PE_PARSER_CLASS DR_IPEInfo
{
public:
	virtual bool IsInit() const = 0;
	virtual bool IsSignaturesCorrect() const = 0;
	virtual bool IsDOSSignatureCorrect() const = 0;
	virtual bool IsNTSignatureCorrect() const = 0;
	virtual const std::vector<DR_FileCharacteristic>& GetFileCharacteristics() const = 0;
	virtual const std::vector<DR_DllCharacteristic>& GetDllCharacteristics() const = 0;
	virtual const std::vector<DR_ShPtrSectionInfo>& GetSectionsInfo() const = 0;
	virtual DR_Subsystem GetSubsystem() const = 0;
	virtual DR_Machine GetTargetMachine() const = 0;
	virtual std::int32_t GetImageSize() const = 0;
	virtual std::chrono::system_clock::time_point GetTimeStamp() const = 0;
	virtual DWORD GetEntryPointRVA() const = 0;
	virtual DWORD GetNTHeaderRVA() const = 0;
	virtual const DR_ShPtrNTHeaderInfo& GetNTHeader(DR_NTHeader HeaderId) const = 0;
	virtual ~DR_IPEInfo() = default;
};
typedef std::shared_ptr<DR_IPEInfo> DR_ShPtrPEInfo;
typedef std::unique_ptr<DR_IPEInfo> DR_UnPtrPEInfo;

DR_UnPtrPEInfo DR_PE_PARSER_FUNC DR_MakePEInfoUnPtr(const LPVOID ModulePtr);
DR_ShPtrPEInfo DR_PE_PARSER_FUNC DR_MakePEInfoShPtr(const LPVOID ModulePtr);