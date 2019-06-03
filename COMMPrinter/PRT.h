// PRT.h : PRT DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "PRTApi.h"
#include "AFCLogApi.h"
#include "SerialPort.h"


#define	PRT_RES_OK				0L	// 操作成功
#define PRT_RES_FAILED          1L  // 操作失败
#define MAX_PTINT_LEN           1000




void PRT_SetPRTDevReturn(const int errCode, tPRTDevReturn* pDevStatus, int uiStdErrorCode = PRT_RES_OK);
int PRT_WriteToPrint(DWORD dwTotalLen, char* buff);
int PRT_ReadToPrint(char& status);
int PRT_GetAactualCharacterMemarySize(CString text);
