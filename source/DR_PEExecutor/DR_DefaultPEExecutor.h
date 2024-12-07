#pragma once
#include "stdafx.h"
#include "DR_Common.h"
#include "DR_Trace.h"
#include "DR_PEExecutor.h"
#include "DR_ExecFuncWraps.h"

class DR_DefaultPEExecutor : public DR_IPEExecutor
{
private:
	std::any retrunValueAL;// TODO сделать в отдельном классе
	bool isLastOperationSuccess;

public:
	DR_DefaultPEExecutor();
	inline virtual bool IsLastOperationSuccess() const override { return this->isLastOperationSuccess; }
	virtual void ExecuteFunc(const LPVOID DataModule, DWORD FuncRVA, DR_CallDecl CallDecl, const std::vector<std::int64_t>& Args) override;
	virtual const std::any& GetReturnValue(DR_ReturnType ReturnType) override;
};

