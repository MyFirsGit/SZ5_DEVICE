#include "StdAfx.h"
#include "CHCommand.h"
#include "CHlib.h"
#include "CHException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHTRS_HANDLE m_hCH;	// Ӳ��ģ����
bool isCHAborted = false;


BOOL IsTimeUp1(IN DWORD dwTimer,IN DWORD	dwBaseTime)
{
	BOOL bRet = FALSE;
	DWORD dwCurTime;
	DWORD dwUpTime;
	//            dwTime
	//    |-------------------|
	// dwBaseTime          dwUpTime
	//    |----------|--------|-----
	//           dwCurTime
	// Has timer
	if (dwTimer != 0){
		dwCurTime = GetTickCount();		// ��ǰʱ��
		dwUpTime = dwBaseTime + dwTimer;// ����ʱ��
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// ��ʱ����ǰʱ�� > ����ʱ�䣩
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
			else{
				bRet = TRUE;
			}
		}
		else{
			if (dwCurTime < dwBaseTime){
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
		}
	}

	return bRet;
}

CHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer)
{
	CHTRS_RESULT result = CHTRS_RES_OK;
	DWORD dwChk = 0;
	DWORD dwSize = 0;

	// ���Դ���
	//WORD nTryCount = 0;
	// ��������
	result = CHTrs_Send(m_hCH, nSendLength,pSendBuffer, &dwSize);	
	DWORD dwBaseTime= GetTickCount();
	bool istimeout = false;
	//nTryCount++;

	// ���ͳɹ�
	if(result == CHTRS_RES_OK){
		while(true){			
			// ��ȡ��ǰͨ��״̬
			result = CHTrs_GetStat(m_hCH, &dwChk);
			// ���ڴ���������
			if(result == CHTRS_RES_OK && (dwChk &CHTRS_ST_RCVQUE) != 0){
				// �������ݷ���
				result = CHTrs_Recv(m_hCH,CH_RES_MAX,pRecvBuff,&nRecvLength);
				// �������ݳɹ�
				if (result == CHTRS_RES_OK){
					// �����������
					//if(pRecvBuff[0] != pSendBuffer[0]){
					//	result = CCHException::ERROR_CH_SEQUNCE_ERR;
					//	break;
					//}
					//// ���״̬��
					//if(pRecvBuff[3] != 0x00){
					//	result = pRecvBuff[3];
					//	break;
					//}
					break;
				}
				// ��������ʧ��
				else{
					result = -1;//CCHException::ERROR_CH_RECEIVE;
					break;
				}
			}
			// �����ڴ���������
			else{
				// ��ʱ�ж�
				if(IsTimeUp1(dwTimer,dwBaseTime)){
					// ��ʱ��Ҫ����3��(���Ի�������)
					//if(nTryCount < MAX_RETRYSEND){
					//	result = CHTrs_Send(m_hCH, nSendLength,pSendBuffer, &dwSize);
					//	
					//	// ���»�׼ʱ��
					//	dwBaseTime= GetTickCount();
					//	// ���Դ�����1
					//	nTryCount++;
					//	if(result != CHTRS_RES_OK){
					//		result = CCHException::ERROR_CH_SEND;
					//		break;
					//	}
					//}
					//else{
					result = -1;//CCHException::ERROR_CH_RECEIVE_TIMEOUT;
					break;
					//}
				}
				continue;
			}
		} // end while
	}
	// ����ʧ��
	else{
		result = -1;//CCHException::ERROR_CH_SEND;
	}
	return result;
}

//�˿ڴ�
int CHA_CommOpen(UINT uiCommPort, UINT uiBaudRate, tCHADevReturn * pDevStatus)
{
	DWORD errCode = 0;//CCHException::ERROR_CH_OPEN_FAILED;
	CHTRS_CONNECT_PARAM m_connectParam;
	memset(&m_connectParam,0,sizeof(CHTRS_CONNECT_PARAM));
	m_connectParam.ComPort=uiCommPort;
	m_connectParam.Speed =uiBaudRate;

	m_connectParam.MaxRecvCnt        = 10;
	m_connectParam.MaxSendCnt        = 10;
	m_connectParam.MaxRecvBuffSize   = MAX_BUFFER_SIZE;
	m_connectParam.MaxSendBuffSize   = MAX_BUFFER_SIZE;

	// ���򿪣�����Ҫ�ر�
	if (m_hCH != NULL){
		errCode = CHTrs_Close(m_hCH);
		m_hCH = NULL;
		if (errCode != CHTRS_RES_OK){
			return errCode;//CCHException::ERROR_CH_CLOSE_FAILED;
		}
	}

	// ��
	errCode = CHTrs_Open(&m_hCH, &m_connectParam);
	if (errCode != CHTRS_RES_OK){
		m_hCH = NULL;
		return errCode;//CCHException::ERROR_CH_OPEN_FAILED;
	}

	return 0;
}


// ��ʼ��
int CHA_Init (BYTE btXinCtl,tCHADevReturn * pDevStatus)
{
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x07;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = (btXinCtl<<4)&0xF0;
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;// CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_60000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return-1;// CCHException::ERROR_CH_RECEIVE;
	}

	return 0;
}

// ��ȡ�豸״̬
int CHA_GetDevStatus(tCHADevReturn * pDevStatus)
{
	CH_SPEC1_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x0C;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen);
	if (result != CHTRS_RES_OK){
		return result;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		//OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Common.Res[1] > 0x70){
			//BYTE errcode = m_Res.Common.Err_Info[2] >> 4;   //��ô������
			long errCode = (((m_Res.Common.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Common.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return -1;
	}

	return 0;
}

// ��ȡ������״̬(�ð汾û�� 20180703 ����)
int CHA_GetSensorStatus(tCHASensorStatus * pSensorStatus, tCHADevReturn * pDevStatus)
{
	return 1;
}

// ��ȡ����������Ϣ
int CHA_GetCoinBoxInfo(tCHADevStatus *pCoinBoxInfo,tCHADevReturn * pDevStatus)
{
	CString szLogString(_T("CHA_GetCoinBoxInfo"));
	theCH_TRACE->WriteData(__LINE__,_T("<")+szLogString,NULL,0);

	szLogString = _T("");
	if (pCoinBoxInfo == NULL || m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE)
	{
		szLogString = _T("���������Ч��");
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return -1;
	}
	BYTE m_pText[6] = {0x00};
	m_pText[0] = 0x00;
	m_pText[1] = 0x0C;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(sizeof(m_pText), m_pText, recvbuf, recvlen);
	if (result != CHTRS_RES_OK){
		szLogString.Format(_T("��������%06x ʧ�� %d"),m_pText,result);
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return result;
	}
	CH_SPEC1_RSP m_Res;
	// ����������Ϣ
	if (recvlen > 0){
		//OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Common.Res[1] > 0x70){
			//BYTE errcode = m_Res.Common.Err_Info[2] >> 4;   //��ô������
			long errCode = (((m_Res.Common.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Common.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			szLogString.Format(_T("error: %d"),errCode);
			theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);

			return errCode;//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
		else
		{
			pCoinBoxInfo->bIsChangeBoxAEmpty = m_Res.Common.bIsChangeBoxAEmpty;
			pCoinBoxInfo->bIsChangeBoxAFull = m_Res.Common.bIsChangeBoxAFull;
			pCoinBoxInfo->bIsChangeBoxAInstall = m_Res.Common.bIsChangeBoxAInstall;
			pCoinBoxInfo->bIsChangeBoxBEmpty = m_Res.Common.bIsChangeBoxBEmpty;
			pCoinBoxInfo->bIsChangeBoxBFull = m_Res.Common.bIsChangeBoxBFull;
			pCoinBoxInfo->bIsChangeBoxBInstall = m_Res.Common.bIsChangeBoxBInstall;
			pCoinBoxInfo->bIsCoinModuleStandby = m_Res.Common.bIsCoinModuleStandby;
			pCoinBoxInfo->bIsHopperAEmpty = m_Res.Common.bIsHopperAEmpty;
			pCoinBoxInfo->bIsHopperAFull = m_Res.Common.bIsHopperAFull;
			pCoinBoxInfo->bIsHopperBEmpty = m_Res.Common.bIsHopperBEmpty;
			pCoinBoxInfo->bIsHopperBFull = m_Res.Common.bIsHopperBFull;

			pCoinBoxInfo->wHopperACnt = m_Res.Common.OneY_inHopper;
			pCoinBoxInfo->wHopperBCnt = m_Res.Common.HalfY_inHopper;
			pCoinBoxInfo->wPreparationACnt = m_Res.Common.OneY_inPreparation;
			pCoinBoxInfo->wPreparationBCnt = m_Res.Common.HalfY_inPreparation;
			pCoinBoxInfo->wRecoverCnt = m_Res.Common.OneY_inRecover;
		}
	}
	else{
		szLogString.Format(_T("recvlen: %d"),recvlen);
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return -1;
	}
	szLogString = _T("CHA_GetCoinBoxInfo Success");
	theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
	return 0;
}

// ����Ӳ������
int CHA_SetCoinBoxInfo(tCoinBoxInfo *pCoinBoxInfo,tCHADevReturn * pDevStatus)
{
	CString szLogString(_T("CHA_SetCoinBoxInfo"));
	theCH_TRACE->WriteData(__LINE__,_T("<")+szLogString,NULL,0);

	szLogString = _T("");
	if (pCoinBoxInfo == NULL || m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE)
	{
		szLogString = _T("���������Ч��");
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return -1;
	}
	DatalightInfo data;
	data.OneY_inHopper = pCoinBoxInfo->OneY_inHopper;
	data.HalfY_inHopper = pCoinBoxInfo->HalfY_inHopper;
	data.OneY_preparation = pCoinBoxInfo->OneY_preparation;
	data.HalfY_preparation = pCoinBoxInfo->HalfY_preparation;
	data.OneY_inRecover = pCoinBoxInfo->OneY_inRecover;
	data.OneY_maxthreshold_inRecover = pCoinBoxInfo->OneY_maxthreshold_inRecover;
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	BYTE m_pText[60] = {0X00};
	memset(m_pText,0,sizeof(m_pText)*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x02;
	m_pText[2] = 0x00;
	m_pText[3] = 0x3A;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;

	//��x86�ĵ��ֽ���ת���ɸ��ֽ���
	char* ptemp = (char*)&data;
	BYTE length = 60 - 6;  //��Ҫת���ķ�Χ
	for(BYTE i = 6; i < length; i+=2)
	{
		m_pText[i] = ptemp[i+1-6];
		m_pText[i+1] = ptemp[i-6];
	}

	for(BYTE i = length; i < 60; ++i)
		m_pText[i] = ptemp[i-6];	

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(sizeof(m_pText), m_pText, recvbuf, recvlen);
	if (result != CHTRS_RES_OK){
		return result;
	}

	// ����������Ϣ
	if (recvlen > 0){
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			szLogString.Format(_T("error: %d"),errCode);
			theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
			return errCode;
		}
	}
	else{
		szLogString.Format(_T("recvlen: %d"),recvlen);
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);

		return -1;
	}
	szLogString = _T("CHA_SetCoinBoxInfo Success");
	theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);

	return 0;
}

// ��ʼ����Ӳ��
int CHA_StartReceiveCoin (tCHADevReturn * pDevStatus)
{ 
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x20;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;// CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_30000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return -1;//CCHException::ERROR_CH_RECEIVE;
	}

	return 0;
}
// ��������Ӳ��
int CHA_StopReceiveCoin(tCHADevReturn * pDevStatus)
{
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x21;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;//Stop accept
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;//CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_30000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		if (m_Res.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		return -1;//CCHException::ERROR_CH_RECEIVE;
	}

	return 0;
}

// ��ȡ����Ӳ����Ϣ
int CHA_GetReceiveNumber(tCHACoinValueInfo *pCoinValueInfo, tCHADevReturn * pDevStatus)
{
	CString szLogString(_T("CHA_GetReceiveNumber"));
	theCH_TRACE->WriteData(__LINE__,_T("<")+szLogString,NULL,0);

	szLogString = _T("");
	if (pCoinValueInfo == NULL || m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE)
	{
		szLogString = _T("���������Ч��");
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return -1;
	}
	CH_SPEC1_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x0C;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen);
	if (result != CHTRS_RES_OK){
		szLogString.Format(_T("��������%06x ʧ�� %d"),m_pText,result);
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		if (m_pText)
		{
			delete[] m_pText;
			m_pText = NULL;
		}
		return -1;
	}
	if (m_pText)
	{
		delete[] m_pText;
		m_pText = NULL;
	}

	// ����������Ϣ
	if (recvlen > 0){
		m_Res.Deserialize(recvbuf);
		if (m_Res.Common.Res[1] > 0x70){
			long errCode = (((m_Res.Common.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Common.Err_Info[3] & 0xFF);
			szLogString.Format(_T("error %02x"),errCode);
			theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
			return errCode;
		}
		else
		{
			pCoinValueInfo->uiEjectCoinNum = m_Res.num_Rejected;
			pCoinValueInfo->stCoinBoxValueInfo[0].uiBoxType = 2;
			pCoinValueInfo->stCoinBoxValueInfo[0].uiBoxNo = 1;
			pCoinValueInfo->stCoinBoxValueInfo[0].uiCoinNum = m_Res.OneY_inTemporary;
			pCoinValueInfo->stCoinBoxValueInfo[0].uiCoinValue = 100;
			pCoinValueInfo->stCoinBoxValueInfo[1].uiBoxType = 2;
			pCoinValueInfo->stCoinBoxValueInfo[1].uiBoxNo = 2;
			pCoinValueInfo->stCoinBoxValueInfo[1].uiCoinNum = m_Res.HalfY_inTemporary;
			pCoinValueInfo->stCoinBoxValueInfo[1].uiCoinValue = 100;

		}
	}
	else{
		szLogString.Format(_T("recvlen %d"),recvlen);
		theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
		return -1;
	}
	szLogString.Format(_T("CHA_GetReceiveNumber"));
	theCH_TRACE->WriteData(__LINE__,_T(">")+szLogString,NULL,0);
	return 0;
}

// ����Ӳ��
int CHA_AcceptCoin(tCHACoinValueInfo* rsp,tCHADevReturn * pDevStatus)
{
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int  m_Len = 6 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x30;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x01;

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;//CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_30000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);

		if (m_Res.Res[1] > 0x70)
		{
			delete [] m_pText;
			m_pText = NULL;

			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return CCHException::ERROR_CH_RECEIVE;
	}
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x30;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;
	memset(recvbuf,0x00,CH_RES_MAX);
	recvlen = 0;
	result = 0;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_30000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		rsp->stCoinBoxValueInfo[0].uiBoxType = 2;
		rsp->stCoinBoxValueInfo[0].uiCoinValue = 100;
		rsp->stCoinBoxValueInfo[0].uiCoinNum = m_Res.OneY_inHopper;
		rsp->stCoinBoxValueInfo[1].uiBoxType = 2;
		rsp->stCoinBoxValueInfo[1].uiCoinValue = 50;
		rsp->stCoinBoxValueInfo[1].uiCoinNum = m_Res.HalfY_inHopper;
		rsp->stCoinBoxValueInfo[2].uiBoxType = 5;
		rsp->stCoinBoxValueInfo[2].uiCoinValue = 100;
		rsp->stCoinBoxValueInfo[2].uiCoinNum = m_Res.OneY_inRecover;

		if (m_Res.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return CCHException::ERROR_CH_RECEIVE;
	}
	return 0;
}

// �˳�Ӳ��
int CHA_EjectCoin(UINT * uiReturnCoinNum ,tCHADevReturn * pDevStatus)
{
	CH_COMMON_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 6 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x31;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;		
	m_pText[5] = 0x00;		
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;//CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_60000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);		
		if (m_Res.Res[1] > 0x70){
			long errCode = (((m_Res.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return -1;// CCHException::ERROR_CH_RECEIVE;
	}

	return 0;
}

// ����
int CHA_ChangeCoin(tCHAChangeNum * pChangeNum,tCHADevReturn * pDevStatus)
{
	CH_SPEC4_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 14 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x32;
	m_pText[2] = 0x00;
	m_pText[3] = 0x0C;
	m_pText[4] = 0x00;		
	m_pText[5] = 0x00;
	m_pText[6] = *((BYTE*)&pChangeNum->wMHopper1Num + 1);
	m_pText[7] = *(BYTE*)&pChangeNum->wMHopper1Num;
	m_pText[8] = *((BYTE*)&pChangeNum->wMHopper2Num + 1);
	m_pText[9] = *(BYTE*)&pChangeNum->wMHopper2Num;
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;// CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_60000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		//OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		pChangeNum->wMHopper1Num = m_Res.wOneYuanCount;
		pChangeNum->wMHopper2Num = m_Res.wHalfYuanCount;
		if (m_Res.Common.Res[1] > 0x70)
		{
			delete [] m_pText;
			m_pText = NULL;

			//BYTE errcode = m_Res.Common.Err_Info[2] >> 4;   //��ô������
			long errCode = (((m_Res.Common.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Common.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		memset(pChangeNum,0,sizeof(tCHAChangeNum));
		delete [] m_pText;
		m_pText = NULL;
		return CCHException::ERROR_CH_OTHER;
	}
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x33;
	m_pText[2] = 0x00;
	m_pText[3] = 0x04;
	m_pText[4] = 0x00;		
	m_pText[5] = 0x00;
	// ��������
	memset(recvbuf,0,CH_RES_MAX); 
	recvlen = 0;
	result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_60000);
	if (result != CHTRS_RES_OK){
		return CCHException::ERROR_CH_SEND;
	}
	CH_COMMON_RSP Res;

	// ����������Ϣ
	if (recvlen > 0){
		OutputDebugString(L"\nSuccessfully!\n");
		Res.Deserialize(recvbuf);
		if (Res.Res[1] > 0x70)
		{
			long errCode = (((Res.Err_Info[2] & 0xFF)>>4) << 8) | (Res.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			//BYTE errcode = m_Res.Err_Info[2] >> 4;   //��ô������
			//throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		return CCHException::ERROR_CH_RECEIVE;
	}
	return 0;
}

// ��������
int CHA_StartReplaceBOX (UINT uiBoxType, UINT uiBoxNo,UINT uiOpType,tCHADevReturn * pDevStatus)
{
	return 1;
}
// ֹͣ��������
int CHA_StopReplaceBOX(UINT uiBoxType, UINT uiBoxNo, UINT uiOpType,tCHADevReturn * pDevStatus)
{
	return 1;
}

// ���Ӳ��
int CHA_ClearCoin (UINT uiBoxTypeOutput, UINT uiBoxNoOutput, UINT uiBoxTypeInput, UINT uiBoxNoInput, tCoinBoxInfo *pCoinBoxInfo,UINT uiCoinNum,tCHADevReturn * pDevStatus)
{
	CH_SPEC5_RSP m_Res;
	memset(&m_Res,0,sizeof(m_Res));
	int m_Len = 14 ;
	BYTE *m_pText = new BYTE[m_Len];
	memset(m_pText,0,m_Len*sizeof(BYTE));
	m_pText[0] = 0x00;
	m_pText[1] = 0x40;
	m_pText[2] = 0x00;
	m_pText[3] = 0x0C;
	m_pText[4] = 0x00;
	m_pText[5] = 0x00;
	m_pText[6] = 0xFF;
	m_pText[7] = 0xFF;
	m_pText[8] = 0xFF;
	m_pText[9] = 0xFF;
	m_pText[10] = 0xFF;
	m_pText[11] = 0xFF;
	m_pText[12] = 0xFF;
	m_pText[13] = 0xFF;

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return -1;//CCHException::ERROR_CH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[CH_RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	CHTRS_RESULT result = CHTRS_RES_OK;
	result = SendOneCommand(m_Len, m_pText, recvbuf, recvlen,TIME_OUT_300000);
	if (result != CHTRS_RES_OK){
		return result;//CCHException::ERROR_CH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		delete [] m_pText;
		m_pText = NULL;
		OutputDebugString(L"\nSuccessfully!\n");
		m_Res.Deserialize(recvbuf);
		pCoinBoxInfo->OneY_inHopper = m_Res.wOneHopperCount;
		pCoinBoxInfo->HalfY_inHopper = m_Res.wHalfHopperCount;
		pCoinBoxInfo->OneY_preparation = m_Res.wOneChangeBoxCount;
		pCoinBoxInfo->HalfY_preparation = m_Res.wHalfChangeBoxCount;
		if (m_Res.Common.Res[1] > 0x70)
		{
			long errCode = (((m_Res.Common.Err_Info[2] & 0xFF)>>4) << 8) | (m_Res.Common.Err_Info[3] & 0xFF);// ��ϸ������(Errorԭ�� + ������ʽ + Error����ϸ)
			return errCode;
			// 			BYTE errcode = m_Res.Common.Err_Info[2] >> 4;   //��ô������
			// 			throw CCHHardwareException(errcode,_T(__FILE__),__LINE__);
		}
	}
	else{
		delete [] m_pText;
		m_pText = NULL;
		return -1;// CCHException::ERROR_CH_RECEIVE;
	}

	return 0;
}

// ������RFID��Ϣ
int CHA_ReadRFIDInfo(UINT uiBoxType, UINT uiBoxNo,unsigned char RFIDInfo[36],tCHADevReturn * pDevStatus)
{
	return 1;
}

// д����RFID��Ϣ
int CHA_WriteRFIDInfo (UINT uiBoxType, UINT uiBoxNo,unsigned char RFIDInfo[36],tCHADevReturn * pDevStatus)
{
	return 1;
}
// ��ȡģ����ʰ汾
int CHA_GetVersion (char cVersion[32],tCHADevReturn * pDevStatus)
{
	return 1;
}
// ��ȡģ�����
int CHA_GetSerialNumber(char cVersion[32],tCHADevReturn * pDevStatus)
{
	return 1;
}
// ��ȡ�������
int CHA_GetAuditData (tCHACoinBoxInfo *pCoinBoxInfo,tCHADevReturn * pDevStatus)
{
	return 1;
}
// �ر�ͨѶ
int CHA_CloseComm (tCHADevReturn * pDevStatus)
{
	DWORD errCode = CHTRS_RES_OK;

	// ���򿪣�����Ҫ�ر�
	if (m_hCH != NULL){
		errCode = CHTrs_Close(m_hCH);
		m_hCH = NULL;
		if (errCode != CHTRS_RES_OK){
			return errCode;//CCHException::ERROR_CH_CLOSE_FAILED;
		}
	}

	return 0;
}