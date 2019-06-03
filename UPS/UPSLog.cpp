#include "StdAfx.h"
#include "UPSLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUPSLog CUPSLog::UPSLogInstance;

/**
@brief UPS��־trace
*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSLog::CUPSLog():CLog(FILTER_UPS_LOG)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSLog::~CUPSLog(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����־�ļ�����д����

@param      (i) cmdName    ������
@param      (i) lpData     ��Ҫд������
@param      (i) iLen       ��Ҫд�ĳ���
@param      (i) iMode      ���ͻ��ǽ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUPSLog::WriteLogFile(CString cmdName,LPBYTE lpData,int iLen, SEND_MODE iMode)
{
	CString sData = _T("");

	if(MODE_SEND == iMode){
		sData = "> ";
	}
	else{
		sData = "< ";
	}

	sData = sData + cmdName + _T(" ");

	for (int i = 0; i < iLen; i++){
		CString sHex = _T("");
		sHex.Format(_T("%02x"),*(lpData + i));
		sData += sHex;
	}
	// write data to log file
	__super::WriteData(sData);
}
