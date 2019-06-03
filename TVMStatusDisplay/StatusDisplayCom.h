#pragma once
#include "afx.h"
typedef DWORD COMM_RESULT;
typedef DWORD COMM_HANDLE;        // CH连接对象句柄

extern COMM_HANDLE g_hStatusDisplay;
extern void CALLBACK OnGetResponseCmd();
extern HANDLE g_hShowComplete;