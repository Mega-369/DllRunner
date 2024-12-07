#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"

#ifdef DR_PE_EXECUTOR_STATIC
#define DR_PE_EXECUTOR_CLASS
#define DR_PE_EXECUTOR_FUNC
#else
#ifdef DR_PE_EXECUTOR_EXPORT
#define DR_PE_EXECUTOR_CLASS __declspec(dllexport)
#define DR_PE_EXECUTOR_FUNC __declspec(dllexport)
#else
#define DR_PE_EXECUTOR_CLASS __declspec(dllimport)
#define DR_PE_EXECUTOR_FUNC __declspec(dllimport)
#endif
#endif

enum DR_PE_EXECUTOR_CLASS DR_CallDecl
{
	DR_CallDeclX64 = 0,
	DR_CallDeclC = 1,
	DR_CallDeclStd = 2,
	DR_CallDeclFast = 3,
	DR_CallDeclVector = 4
};

enum DR_PE_EXECUTOR_CLASS DR_ReturnType
{
	DR_ReturnTypeAL,
	DR_ReturnTypeBL,
	DR_ReturnTypeCL,
	DR_ReturnTypeDL,
	DR_ReturnTypeAH,
	DR_ReturnTypeBH,
	DR_ReturnTypeCH,
	DR_ReturnTypeDH,
	DR_ReturnTypeAX,
	DR_ReturnTypeBX,
	DR_ReturnTypeCX,
	DR_ReturnTypeDX,
	DR_ReturnTypeEAX,
	DR_ReturnTypeEBX,
	DR_ReturnTypeECX,
	DR_ReturnTypeEDX,
	DR_ReturnTypeRAX,
	DR_ReturnTypeRBX,
	DR_ReturnTypeRDX,
	DR_ReturnTypeRCX,
	DR_ReturnTypeST,
	DR_ReturnTypeXMM,
	DR_ReturnTypeStack
};

class DR_PE_EXECUTOR_CLASS DR_IPEExecutor
{
public:
	virtual bool IsLastOperationSuccess() const = 0;
	virtual void ExecuteFunc(const LPVOID DataModule, DWORD FuncRVA, DR_CallDecl CallDecl, const std::vector<std::int64_t>& Args) = 0;
	virtual const std::any& GetReturnValue(DR_ReturnType ReturnType) = 0;
	virtual ~DR_IPEExecutor() = default;
};
typedef std::unique_ptr<DR_IPEExecutor> DR_UnPtrPEExecutor;
typedef std::shared_ptr<DR_IPEExecutor> DR_ShPtrPEExecutor;

DR_UnPtrPEExecutor DR_PE_EXECUTOR_FUNC MakePEExecutorUnPtr();
DR_ShPtrPEExecutor DR_PE_EXECUTOR_FUNC MakePEExecutorShPtr();