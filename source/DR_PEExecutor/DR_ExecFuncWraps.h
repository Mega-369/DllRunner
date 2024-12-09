#pragma once
#include "stdafx.h"

#ifndef DR_X86_HOST
extern "C" struct DR_InvokeWithCallDeclX64Args
{
	std::int64_t ArgCount;
	const std::int64_t* Args;
};

extern "C" struct DR_InvokeWithCallDeclX64RetSnap
{
	std::int64_t RAXValue;
	std::int64_t RDXValue;
	std::int64_t EAXValue;
	std::int64_t AXValue;
	std::int64_t ALValue;
	std::int64_t XMMValue;
	std::int64_t OutBufferSize;
	void* OutBuffer;
};

extern "C" void DR_InvokeWithCallDeclX64(std::int64_t FuncAddress, DR_InvokeWithCallDeclX64Args FuncArgs, DR_InvokeWithCallDeclX64RetSnap& OutRetSnapshot);
extern "C" void DR_InvokeWithCallDeclX64OutRCXBuffer(std::int64_t FuncAddress, DR_InvokeWithCallDeclX64Args FuncArgs, DR_InvokeWithCallDeclX64RetSnap& OutRetSnapshot);
#else

extern "C" struct DR_InvokeWithCallDeclX64Args
{
	std::int64_t ArgCount;
	const std::int64_t* Args;
};

extern "C" struct DR_InvokeWithCallDeclX64RetSnap
{
	std::int64_t RAXValue;
	std::int64_t RDXValue;
	std::int64_t EAXValue;
	std::int64_t AXValue;
	std::int64_t ALValue;
	std::int64_t XMMValue;
	std::int64_t OutBufferSize;
	void* OutBuffer;
};

extern "C" void DR_InvokeWithCallDeclC(std::int32_t FuncAddress, std::int32_t ArgCount, const std::int32_t* Args);
extern "C" void DR_InvokeWithCallDeclStd(std::int32_t FuncAddress, std::int32_t ArgCount, const std::int32_t* Args);
extern "C" void DR_InvokeWithCallDeclFast(std::int32_t FuncAddress, std::int32_t ArgCount, const std::int32_t* Args);
extern "C" void DR_InvokeWithCallDeclVector(std::int32_t FuncAddress, std::int32_t ArgCount, const std::int32_t* Args);
#endif