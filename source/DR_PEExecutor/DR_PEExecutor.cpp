#include "DR_PEExecutor.h"
#include "DR_DefaultPEExecutor.h"

DR_UnPtrPEExecutor MakePEExecutorUnPtr()
{
	DR_UnPtrPEExecutor newPEExecutor = std::make_unique<DR_DefaultPEExecutor>();
	return newPEExecutor;
}

DR_ShPtrPEExecutor MakePEExecutorShPtr()
{
	DR_ShPtrPEExecutor newPEExecutor = std::make_shared<DR_DefaultPEExecutor>();
	return newPEExecutor;
}
