#pragma once
#include <Windows.h>

/*
* zhengxianle:系统奔溃后由此生成设备运行上下文，以便调查问题发生原因
*/
VOID CreateDump(struct _EXCEPTION_POINTERS *pExceptionPointers);