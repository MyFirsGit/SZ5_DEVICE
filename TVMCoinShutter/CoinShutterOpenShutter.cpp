#include "stdafx.h"
#include "CoinShutterCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCSOpenShutter::CCSOpenShutter()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCSOpenShutter::~CCSOpenShutter()
{

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCSOpenShutter::ExecuteCommand()
{
	long lRet = 0;
	lRet = OpenShutterFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ��

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCSOpenShutter::OpenShutterFunction()
{
	//CString sLogString(_T(""));
	//SYNC(CS,_T("OPEN_SHUTTER"));

	//if (m_bConnected == false){
	//	return CCoinShutterException::ERROR_CS_NOT_OPEN;
	//}

	//sLogString.Format(_T("iOpen: "));
	//theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//long errCode = E_NO_CS_ERROR;;
	//tDevReturn sDevReturn[8];//�ӿڶ���˵���п��ܻ᷵�ض�ά״̬ ����

	//errCode = m_ShutterDev.iOpen(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}

	//sLogString.Format(_T("iOpen: errCode = %d"), errCode);
	//theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	//return errCode;

	CString sLogString(_T(""));
	SYNC(CS,_T("OpenShutterFunction"));

	if (m_bConnected == false){
		return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("OpenShutterFunction: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;;
	// ����
	char cCmd[]={0x1b,0x4F,0x03};
	// �������ӡ��
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = ::WriteFile(m_hCoinShutter,cCmd,sizeof(cCmd),&dwBytesWritten,NULL);  
	int iWriteResult =  0;
	// д����ʧ��
	if(bWriteState != TRUE){
		if(GetLastError() !=  ERROR_IO_PENDING){
			iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
		}
		else{
		}
	}
	if(iWriteResult!=0){
		sLogString.Format(_T("OpenShutterFunction: errCode = %d"), iWriteResult);
		theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		return iWriteResult;
	}

	char cGetStatusResponse[15] = {0};
	DWORD dwBytesRead = 0;
	// ��ȡ��ӡ������
	BOOL  bReadState = ::ReadFile(m_hCoinShutter,cGetStatusResponse,3,&dwBytesRead,NULL);
	int iReadResult = 0;
	// ������ʧ��
	if(bReadState != TRUE){
		if(GetLastError()!= ERROR_IO_PENDING){
			iReadResult = CCoinShutterException::ERROR_CS_RECEIVE;
		}
		else{
		}
	}
	if(dwBytesRead == 0){
		iReadResult =  CCoinShutterException::ERROR_CS_RECEIVE;
	}
	//��մ��ڻ�����
	if(!::PurgeComm(m_hCoinShutter, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
	{
		//iReadResult = CCoinShutterException::ERROR_CS_OTHER;
		//CloseHandle(m_hCoinShutter);
		//m_hCoinShutter = NULL;
		//return iRet;
	}

	if(cGetStatusResponse[1] != 's'){
		sLogString.Format(_T("OpenShutterFunction: errCode = %d"), (cGetStatusResponse[2]==0?iReadResult:cGetStatusResponse[2]));
		theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		return cGetStatusResponse[2];
	}
	//tDevReturn sDevReturn[8];//�ӿڶ���˵���п��ܻ᷵�ض�ά״̬ ����

	//errCode = m_ShutterDev.iInit(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	errCode = iReadResult;
	sLogString.Format(_T("OpenShutterFunction: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
