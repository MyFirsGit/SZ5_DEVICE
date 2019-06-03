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
CCSGetStatus::CCSGetStatus()
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
CCSGetStatus::~CCSGetStatus()
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
long CCSGetStatus::ExecuteCommand()
{
	long lRet = 1;
	lRet = GetStatusFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ�����

 @param		LPCS_SPEC1_RSP pRes ָ���ȡ������Ϣ�Ķ���

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCSGetStatus::GetResponse(tShutterStatus* pSensorStatus)
{
	memcpy(pSensorStatus, &m_sSensorStatus, sizeof(tShutterStatus));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCSGetStatus::GetStatusFunction()
{
	//CString sLogString(_T(""));
	//SYNC(CS,_T("GET_STATUS"));

	//if (m_bConnected == false){
	//	return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	//}

	//sLogString.Format(_T("iGetDevStatus: "));
	//theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//long errCode = E_NO_CS_ERROR;

	//tDevReturn sDevReturn[8];
	//errCode = m_ShutterDev.iGetDevStatus(&m_sSensorStatus,sDevReturn);
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	//else{
	//	//Ϊ״̬�ӿڸ�ֵ

	//}

	//sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
	//theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	//return errCode;
	return 0;
	CString sLogString(_T(""));
	SYNC(CS,_T("GetStatusFunction"));

	if (m_bConnected == false){
		return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("iInit: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;;
	// ���ڴ�ӡ����OVERLAPPED �ṹ
	OVERLAPPED writeOverlapped={0};   
	writeOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (writeOverlapped.hEvent == NULL || writeOverlapped.hEvent == INVALID_HANDLE_VALUE) {
		return CCoinShutterException::SET_PARAM_ERR;
	}
	// ����
	char cCmd[]={0x1b,0x49,0x03};
	// �������ӡ��
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = ::WriteFile(m_hCoinShutter,cCmd,sizeof(cCmd),&dwBytesWritten,&writeOverlapped);  
	int iWriteResult =  0;
	// д����ʧ��
	if(bWriteState != TRUE){
		if(GetLastError() !=  ERROR_IO_PENDING){
			iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
		}
		else{
			DWORD dwWaitResult = WaitForSingleObject(writeOverlapped.hEvent,15000);  //�ȴ��ź�
			switch( dwWaitResult) {
			case WAIT_TIMEOUT: 
				iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
				break;
			case WAIT_FAILED:
			default:				
				iWriteResult = CCoinShutterException::ERROR_CS_SEND;
				break;
			}
		}
	}
	CloseHandle(writeOverlapped.hEvent);
	if(iWriteResult!=0){
		return iWriteResult;
	}

	// ���ڴ�ӡ����OVERLAPPED �ṹ
	OVERLAPPED readOverlapped={0};   
	readOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (readOverlapped.hEvent == NULL || readOverlapped.hEvent == INVALID_HANDLE_VALUE) {
		return CCoinShutterException::SET_PARAM_ERR;
	}
	char cGetStatusResponse[15] = {0};
	DWORD dwBytesRead = 0;
	// ��ȡ��ӡ������
	BOOL  bReadState = ::ReadFile(m_hCoinShutter,cGetStatusResponse,3,&dwBytesRead,&readOverlapped);
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
	CloseHandle(readOverlapped.hEvent);
	if(iReadResult != 0){
		return cGetStatusResponse[2];
	}
	//tDevReturn sDevReturn[8];//�ӿڶ���˵���п��ܻ᷵�ض�ά״̬ ����

	//errCode = m_ShutterDev.iInit(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	errCode = iReadResult;
	sLogString.Format(_T("iInit: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
