#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEParser.h"

#ifdef DR_MANAGER_STATIC
#define DR_MANAGER_CLASS
#define DR_MANAGER_FUNC
#else
#ifdef DR_MANAGER_EXPORT
#define DR_MANAGER_CLASS __declspec(dllexport)
#define DR_MANAGER_FUNC __declspec(dllexport)
#else
#define DR_MANAGER_CLASS __declspec(dllimport)
#define DR_MANAGER_FUNC __declspec(dllimport)
#endif // DR_MANAGER_EXPORT
#endif // DR_MANAGER_STATIC

class DR_MANAGER_CLASS DR_IPEManager
{
public:
	virtual void LoadModule(const std::wstring& ModulePath) = 0;
	virtual void Unload() = 0;
	virtual bool IsLastOperationSuccess() const = 0;
	virtual bool IsPELoaded() const = 0;
	virtual const DR_ShPtrPEInfo& GetPEInfo() const = 0;
	virtual const std::wstring& GetModulePath() const = 0;
	virtual ~DR_IPEManager() = default;
};
typedef std::shared_ptr<DR_IPEManager> DR_ShPtrPEManager;

class DR_MANAGER_CLASS DR_IManager
{
public:
	virtual void AddPE(const std::wstring& ModulePath) = 0;
	virtual void RemovePE(const std::wstring& ModulePath) = 0;
	virtual const std::list<DR_ShPtrPEManager>& GetPEList() const = 0;
	virtual bool IsLastOperationSuccess() const = 0;
	virtual ~DR_IManager() = default;
};
typedef std::unique_ptr<DR_IManager> DR_UnPtrManager;
