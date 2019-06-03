// PRT.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PRT.h"
DWORD m_hPrinter;         //串口打印机句柄
void PRT_SetPRTDevReturn(const int errCode, tPRTDevReturn* pDevStatus, int uiStdErrorCode)
{
	pDevStatus->uiProviderId = 0x0;
	pDevStatus->uiModuleType = 0x07;
	if (errCode != PRT_RES_OK)
		pDevStatus->uiType = 0x02;
	else
		pDevStatus->uiType = errCode;
	pDevStatus->uiStdErrorCode[0] = uiStdErrorCode;
}
//输出到打印机
int PRT_WriteToPrint(DWORD dwTotalLen, char* buff)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));

	//输出到打印机
	bool ret = sp_write(buff, dwTotalLen);

	if (!ret)
	{
		errCode = PRT_RES_FAILED;
		sLogString.Format(_T("WriteToPrint:打印时，向打印机写数据错误"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	sLogString.Format(_T("WriteToPrint:向打印机写入数据结束"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//读取打印机反馈
int PRT_ReadToPrint(char& status)
{
	CString sLogString(_T(""));
	char cGetStatusResponse[2] = { 0 };
	// 读取打印机反馈
	bool ret = sp_read(cGetStatusResponse, 1);
	int iReadResult = PRT_RES_OK;

	if (!ret) {
		iReadResult = PRT_RES_FAILED;
	}
	status = cGetStatusResponse[0];
	sLogString.Format(_T("TIM_CommOpen: PRT_ReadToPrint = 0x%02x"), status);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return iReadResult;
}
//判断字符串实际字节数
int PRT_GetAactualCharacterMemarySize(CString text)
{
	int actualCharacterWidth = 0;
	for (int i = 0; i < text.GetLength(); i++) {
		TCHAR c = text[i];
		if (c > 0x0100) {	//高位如果是1	
			actualCharacterWidth += 2;
		}
		else {
			actualCharacterWidth += 1;
		}
	}
	return actualCharacterWidth;
}
//---------------------------------------------------以下是标准化接口-------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//初始化模块，检测模块各硬件、将模块恢复到初始状态等,设备开机后需要调用本函数
int PRT_Init(tPRTDevReturn * pDevStatus)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));
	if (pDevStatus == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_Init:pDevStatus is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (m_hPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_Init:m_hPrinter is null,串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	char cInit[] = { 0x1B,0x40 };
	errCode = PRT_WriteToPrint(sizeof(cInit), cInit);
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("TIM_CommOpen: ReturnCode = 0x%02x"), errCode);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//在设备故障或从维修模式退出时（对于支持维修模式的情况），执行本函数
int  PRT_Reset(tPRTDevReturn * pDevStatus)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));
	if (pDevStatus == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_Reset:pDevStatus is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (m_hPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_Reset:m_hPrinter is null,串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_Reset:打印机复位成功，实际无数据返回，仅做一个正确返回"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//功能说明：此命令用于将输入的字符串进行打印。
//参数说明见下表： 
//	表248　PRT_Print参数说明 
//	序号	参数类型	参数名称	属性	说明
//	1	Char*	cPrintData	In	需打印的数据。必须/0结尾。
//	2	bool	bCutPaper	In	是否切纸。true：切纸；false：不切纸。
//	3	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
int PRT_Print(CStringArray * m_sArrPrinter, bool m_bOpenBox, bool bCutPaper, tPRTDevReturn * pDevStatus)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));
	if (m_sArrPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetVersion:m_sArrPrinter is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (pDevStatus == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetVersion:pDevStatus is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (m_hPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetVersion:m_hPrinter is null,串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	DWORD  dwTotalLen = 0;
	char* buff = NULL;
	//打印并走纸一行 
	char cCmdRowPrint = 0x0A;
	//打印并走纸 
	char cCmdPageGo[] = { 0x1B,0x4A,0x60 };
	// 打印机初试化指令
	char cInit[] = { 0x1B,0x40 };
	//切纸指令
	char cCutPaper[] = { 0x1D,0x56,0x42,0x12 };
	//选择汉字模式
	char cCmdSelectMultiByteMode[] = { 0x1C,0x26 };

	int cnt = 0;
	cnt = (int)m_sArrPrinter->GetCount();
	int iSize = cnt * 100 + 25;
	buff = new char[iSize];
	memset(buff, 0, iSize);

	// 初试化打印机
	memcpy(buff + dwTotalLen, cInit, sizeof(cInit));
	dwTotalLen += sizeof(cInit);

	// 设置打印汉字
	memcpy(buff + dwTotalLen, cCmdSelectMultiByteMode, sizeof(cCmdSelectMultiByteMode));
	dwTotalLen += sizeof(cCmdSelectMultiByteMode);

	//取得打印内容及打印指令
	for (int i = 0; i < cnt; i++) {
		CString sOne = m_sArrPrinter->GetAt(i);
		USES_CONVERSION;
		char* pMessage = T2A(sOne);
		memcpy(buff + dwTotalLen, pMessage, PRT_GetAactualCharacterMemarySize(sOne));
		dwTotalLen += sOne.GetLength() + PRT_GetAactualCharacterMemarySize(sOne);
		if (i < cnt - 1) {
			memcpy(buff + dwTotalLen, &cCmdRowPrint, 1);
			dwTotalLen += 1;
		}
	};

	if (cnt > 0) {
		// 设置走纸指令
		memcpy(buff + dwTotalLen, cCmdPageGo, sizeof(cCmdPageGo));
		dwTotalLen += sizeof(cCmdPageGo);
		// 设置切纸指令
		if (bCutPaper)
		{
			memcpy(buff + dwTotalLen, cCutPaper, sizeof(cCutPaper));
			dwTotalLen += sizeof(cCutPaper);
		}
	}
	int nTimes = dwTotalLen / MAX_PTINT_LEN;
	int nRemainder = dwTotalLen % MAX_PTINT_LEN;

	for (int i = 0; i < nTimes; i++)
	{
		char szWriteBuff[MAX_PTINT_LEN];
		memset(szWriteBuff, 0, MAX_PTINT_LEN);
		memcpy(szWriteBuff, buff + i * MAX_PTINT_LEN, MAX_PTINT_LEN);
		errCode = PRT_WriteToPrint(MAX_PTINT_LEN, szWriteBuff);
	}
	if (nRemainder != 0)
	{
		char szWriteBuff[MAX_PTINT_LEN];
		memset(szWriteBuff, 0, MAX_PTINT_LEN);
		memcpy(szWriteBuff, buff + nTimes * MAX_PTINT_LEN, dwTotalLen - nTimes * MAX_PTINT_LEN);
		errCode = PRT_WriteToPrint(dwTotalLen - nTimes * MAX_PTINT_LEN, szWriteBuff);
	}

	if (NULL != buff) {
		delete[] buff;
		buff = NULL;
	}

	Sleep((cnt / 2) * 800);
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("TIM_CommOpen: ReturnCode = 0x%02x"), errCode);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//功能说明：调用此接口可获取模块当前状态。
//	参数说明见下表： 
//	表250　PRT_GetDevStatus ()参数说明 
//	序号	参数类型	参数名称	属性	说明
//	1	tPRTModuleStatus *	pModuleStatus 	Out	返回模块状态信息 
//	2	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
int PRT_GetDevStatus(tPRTModuleStatus * pModuleStatus, tPRTDevReturn * pDevStatus)
{
	CString sLogString(_T(""));
	DWORD errCode = PRT_RES_OK;
	if (pModuleStatus == NULL || pDevStatus == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:pModuleStatus is null or pDevStatus is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (m_hPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:m_hPrinter is null,串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	OVERLAPPED coverLaped = { 0 };
	//获取状态命令
	//n = 1：传输打印机状态 
	//n = 2：传输脱机状态 
	//n = 3：传输错误状态 
	//n = 4：传输纸传感器状态 
	//n = 5：传输纸传感器状态
	char cCmdGetStatus[] = { 0X10,0x04,0X02 };
	errCode = PRT_WriteToPrint(sizeof(cCmdGetStatus), cCmdGetStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:WriteToPrint出错"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	char cStatus = 0;
	errCode = PRT_ReadToPrint(cStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:ReadToPrint出错"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	//无纸
	if ((cStatus & 0x20) == 0x20)
	{
		pModuleStatus->uiModuleStatus = 0;
		pModuleStatus->uiPaperStatus = 2;
		PRT_SetPRTDevReturn(errCode, pDevStatus, PRT_ERR_NO_PAPER);
		sLogString.Format(_T("PRT_GetDevStatus:打印机无纸"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	//读取纸传感器状态
	cCmdGetStatus[2] = 0x04;
	errCode = PRT_WriteToPrint(sizeof(cCmdGetStatus), cCmdGetStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:WriteToPrint出错"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	cStatus = 0;
	errCode = PRT_ReadToPrint(cStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:ReadToPrint出错"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	//少纸
	if ((cStatus & 0x0C) == 0x0C)
	{
		pModuleStatus->uiModuleStatus = 0;
		pModuleStatus->uiPaperStatus = 1;
		PRT_SetPRTDevReturn(errCode, pDevStatus, PRT_ERR_LESS_PAPER);
		sLogString.Format(_T("PRT_GetDevStatus:打印机少纸"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_GetDevStatus:打印机状态正常"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}

//功能说明：获取设备驱动版本号、固件版本号、状态信息。
//	参数说明见下表： 
//	表252　PRT_GetVersion ()参数说明 
//	序号	参数类型	参数名称	属性	说明
//	1	char	cFirmWareVersion[32]	out	返回固件版本号
//	2	char	cDriverVersion[32]	out	返回驱动版本号
//	3	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
int PRT_GetVersion(char cFirmWareVersion[32], char cDriverVersion[32], tPRTDevReturn * pDevStatus)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));
	if (pDevStatus == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetVersion:pDevStatus is null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	if (m_hPrinter == NULL)
	{
		errCode = PRT_RES_FAILED;
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetVersion:m_hPrinter is null,串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_GetVersion:获取打印机版本成功，实际无数据返回，仅做一个正确返回"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}

int PRT_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName)
{
	//如果传入的值为空值
	if (path == _T("") || fileName == _T(""))
		return PRT_RES_FAILED;
	//初始化日志
	LogApi.Initialize(path, fileName);

	if (m_hPrinter != NULL)
	{
		PRT_CommClose();
	}

	CString sLogString(_T(""));
	sLogString.Format(_T("PRT_CommOpen:uiCommPort= %d,uiBaudRate= %d,"), uiCommPort, uiBaudRate);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);

	DWORD errCode = PRT_RES_OK;
	bool ret = sp_open(uiCommPort, uiBaudRate, 'N', 8, 1, EV_RXCHAR);
	if (ret)
		m_hPrinter = 1;
	sLogString.Format(_T("PRT_CommOpen:打开串口成功"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
int  PRT_CommClose()
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));

	if (m_hPrinter != NULL) {
		sp_close();
		m_hPrinter = NULL;
	}
	sLogString.Format(_T("PRT_CommClose:关闭串口成功"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	LogApi.CloseLog();
	return errCode;
}