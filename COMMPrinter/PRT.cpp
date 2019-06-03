// PRT.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "PRT.h"
DWORD m_hPrinter;         //���ڴ�ӡ�����
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
//�������ӡ��
int PRT_WriteToPrint(DWORD dwTotalLen, char* buff)
{
	DWORD errCode = PRT_RES_OK;
	CString sLogString(_T(""));

	//�������ӡ��
	bool ret = sp_write(buff, dwTotalLen);

	if (!ret)
	{
		errCode = PRT_RES_FAILED;
		sLogString.Format(_T("WriteToPrint:��ӡʱ�����ӡ��д���ݴ���"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	sLogString.Format(_T("WriteToPrint:���ӡ��д�����ݽ���"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//��ȡ��ӡ������
int PRT_ReadToPrint(char& status)
{
	CString sLogString(_T(""));
	char cGetStatusResponse[2] = { 0 };
	// ��ȡ��ӡ������
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
//�ж��ַ���ʵ���ֽ���
int PRT_GetAactualCharacterMemarySize(CString text)
{
	int actualCharacterWidth = 0;
	for (int i = 0; i < text.GetLength(); i++) {
		TCHAR c = text[i];
		if (c > 0x0100) {	//��λ�����1	
			actualCharacterWidth += 2;
		}
		else {
			actualCharacterWidth += 1;
		}
	}
	return actualCharacterWidth;
}
//---------------------------------------------------�����Ǳ�׼���ӿ�-------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//��ʼ��ģ�飬���ģ���Ӳ������ģ��ָ�����ʼ״̬��,�豸��������Ҫ���ñ�����
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
		sLogString.Format(_T("PRT_Init:m_hPrinter is null,����δ��"));
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
//���豸���ϻ��ά��ģʽ�˳�ʱ������֧��ά��ģʽ���������ִ�б�����
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
		sLogString.Format(_T("PRT_Reset:m_hPrinter is null,����δ��"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_Reset:��ӡ����λ�ɹ���ʵ�������ݷ��أ�����һ����ȷ����"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}
//����˵�������������ڽ�������ַ������д�ӡ��
//����˵�����±� 
//	��248��PRT_Print����˵�� 
//	���	��������	��������	����	˵��
//	1	Char*	cPrintData	In	���ӡ�����ݡ�����/0��β��
//	2	bool	bCutPaper	In	�Ƿ���ֽ��true����ֽ��false������ֽ��
//	3	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
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
		sLogString.Format(_T("PRT_GetVersion:m_hPrinter is null,����δ��"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	DWORD  dwTotalLen = 0;
	char* buff = NULL;
	//��ӡ����ֽһ�� 
	char cCmdRowPrint = 0x0A;
	//��ӡ����ֽ 
	char cCmdPageGo[] = { 0x1B,0x4A,0x60 };
	// ��ӡ�����Ի�ָ��
	char cInit[] = { 0x1B,0x40 };
	//��ָֽ��
	char cCutPaper[] = { 0x1D,0x56,0x42,0x12 };
	//ѡ����ģʽ
	char cCmdSelectMultiByteMode[] = { 0x1C,0x26 };

	int cnt = 0;
	cnt = (int)m_sArrPrinter->GetCount();
	int iSize = cnt * 100 + 25;
	buff = new char[iSize];
	memset(buff, 0, iSize);

	// ���Ի���ӡ��
	memcpy(buff + dwTotalLen, cInit, sizeof(cInit));
	dwTotalLen += sizeof(cInit);

	// ���ô�ӡ����
	memcpy(buff + dwTotalLen, cCmdSelectMultiByteMode, sizeof(cCmdSelectMultiByteMode));
	dwTotalLen += sizeof(cCmdSelectMultiByteMode);

	//ȡ�ô�ӡ���ݼ���ӡָ��
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
		// ������ָֽ��
		memcpy(buff + dwTotalLen, cCmdPageGo, sizeof(cCmdPageGo));
		dwTotalLen += sizeof(cCmdPageGo);
		// ������ָֽ��
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
//����˵�������ô˽ӿڿɻ�ȡģ�鵱ǰ״̬��
//	����˵�����±� 
//	��250��PRT_GetDevStatus ()����˵�� 
//	���	��������	��������	����	˵��
//	1	tPRTModuleStatus *	pModuleStatus 	Out	����ģ��״̬��Ϣ 
//	2	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
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
		sLogString.Format(_T("PRT_GetDevStatus:m_hPrinter is null,����δ��"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	OVERLAPPED coverLaped = { 0 };
	//��ȡ״̬����
	//n = 1�������ӡ��״̬ 
	//n = 2�������ѻ�״̬ 
	//n = 3���������״̬ 
	//n = 4������ֽ������״̬ 
	//n = 5������ֽ������״̬
	char cCmdGetStatus[] = { 0X10,0x04,0X02 };
	errCode = PRT_WriteToPrint(sizeof(cCmdGetStatus), cCmdGetStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:WriteToPrint����"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	char cStatus = 0;
	errCode = PRT_ReadToPrint(cStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:ReadToPrint����"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	//��ֽ
	if ((cStatus & 0x20) == 0x20)
	{
		pModuleStatus->uiModuleStatus = 0;
		pModuleStatus->uiPaperStatus = 2;
		PRT_SetPRTDevReturn(errCode, pDevStatus, PRT_ERR_NO_PAPER);
		sLogString.Format(_T("PRT_GetDevStatus:��ӡ����ֽ"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	//��ȡֽ������״̬
	cCmdGetStatus[2] = 0x04;
	errCode = PRT_WriteToPrint(sizeof(cCmdGetStatus), cCmdGetStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:WriteToPrint����"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	cStatus = 0;
	errCode = PRT_ReadToPrint(cStatus);
	if (errCode == PRT_RES_FAILED)
	{
		PRT_SetPRTDevReturn(errCode, pDevStatus);
		sLogString.Format(_T("PRT_GetDevStatus:ReadToPrint����"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}

	//��ֽ
	if ((cStatus & 0x0C) == 0x0C)
	{
		pModuleStatus->uiModuleStatus = 0;
		pModuleStatus->uiPaperStatus = 1;
		PRT_SetPRTDevReturn(errCode, pDevStatus, PRT_ERR_LESS_PAPER);
		sLogString.Format(_T("PRT_GetDevStatus:��ӡ����ֽ"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_GetDevStatus:��ӡ��״̬����"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}

//����˵������ȡ�豸�����汾�š��̼��汾�š�״̬��Ϣ��
//	����˵�����±� 
//	��252��PRT_GetVersion ()����˵�� 
//	���	��������	��������	����	˵��
//	1	char	cFirmWareVersion[32]	out	���ع̼��汾��
//	2	char	cDriverVersion[32]	out	���������汾��
//	3	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
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
		sLogString.Format(_T("PRT_GetVersion:m_hPrinter is null,����δ��"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return errCode;
	}
	PRT_SetPRTDevReturn(errCode, pDevStatus);
	sLogString.Format(_T("PRT_GetVersion:��ȡ��ӡ���汾�ɹ���ʵ�������ݷ��أ�����һ����ȷ����"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return errCode;
}

int PRT_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName)
{
	//��������ֵΪ��ֵ
	if (path == _T("") || fileName == _T(""))
		return PRT_RES_FAILED;
	//��ʼ����־
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
	sLogString.Format(_T("PRT_CommOpen:�򿪴��ڳɹ�"));
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
	sLogString.Format(_T("PRT_CommClose:�رմ��ڳɹ�"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	LogApi.CloseLog();
	return errCode;
}