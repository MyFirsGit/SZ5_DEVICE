#pragma once
#include "afx.h"
typedef DWORD COMM_RESULT;
typedef DWORD COMM_HANDLE;        // CH���Ӷ�����

extern COMM_HANDLE g_hStatusDisplay;
extern void CALLBACK OnGetResponseCmd();
extern HANDLE g_hShowComplete;