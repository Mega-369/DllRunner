#include "DR_PEParser.h"
#include "DR_PEInfo.h"

DR_UnPtrPEInfo DR_MakePEInfoUnPtr(const LPVOID ModulePtr)
{
	DR_UnPtrPEInfo newPEInfo = std::make_unique<DR_PEInfo>(ModulePtr);
	return std::move(newPEInfo);
}

DR_ShPtrPEInfo DR_MakePEInfoShPtr(const LPVOID ModulePtr)
{
	DR_ShPtrPEInfo newPEInfo = std::make_shared<DR_PEInfo>(ModulePtr);
	return std::move(newPEInfo);
}
