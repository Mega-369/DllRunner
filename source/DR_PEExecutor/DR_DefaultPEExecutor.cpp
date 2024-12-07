#include "DR_DefaultPEExecutor.h"

DR_DefaultPEExecutor::DR_DefaultPEExecutor() :
	isLastOperationSuccess(true)
{
}

void DR_DefaultPEExecutor::ExecuteFunc(const LPVOID DataModule, DWORD FuncRVA, DR_CallDecl CallDecl, const std::vector<std::int64_t>& Args)
{
	DR_OperationResultEntry resEntry(this->isLastOperationSuccess);
	
	//auto e = DR_InvokeWithCallDeclX64(1, 0); // todo delete, just a test
	/*
	DR_CallDeclX64 = 0,
	DR_CallDeclC = 1,
	DR_CallDeclStd = 2,
	DR_CallDeclFast = 3,
	DR_CallDeclVector = 4
	*/
	switch (CallDecl)
	{
	case DR_CallDeclX64:

		break;
	case DR_CallDeclC:

		break;
	case DR_CallDeclStd:

		break;
	case DR_CallDeclFast:

		break;
	case DR_CallDeclVector:

		break;
	default:

		break;
	}
	

	resEntry.SetSuccess();
}
