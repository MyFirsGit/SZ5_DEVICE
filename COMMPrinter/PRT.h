// PRT.h : PRT DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "PRTApi.h"
#include "AFCLogApi.h"
#include "SerialPort.h"


#define	PRT_RES_OK				0L	// �����ɹ�
#define PRT_RES_FAILED          1L  // ����ʧ��
#define MAX_PTINT_LEN           1000




void PRT_SetPRTDevReturn(const int errCode, tPRTDevReturn* pDevStatus, int uiStdErrorCode = PRT_RES_OK);
int PRT_WriteToPrint(DWORD dwTotalLen, char* buff);
int PRT_ReadToPrint(char& status);
int PRT_GetAactualCharacterMemarySize(CString text);
