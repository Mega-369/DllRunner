#include "DR_Trace.h"
#include "DR_TraceDefault.h"

DR_SPTrace DR_TRACE_INTERNAL = nullptr;

DR_SPTrace DR_TRACE_FUNC DR_ImplementTrace()
{
    if (DR_TRACE_INTERNAL == nullptr)
    {
        DR_TRACE_INTERNAL = std::make_shared<DR_TraceDefault>();
    }

    return DR_TRACE_INTERNAL;
}
