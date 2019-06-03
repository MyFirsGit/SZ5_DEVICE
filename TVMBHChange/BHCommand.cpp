#include "stdafx.h"
#include "BHChangeCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����캯��

@param     ��

@retval      ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeCommand::CBHChangeCommand()
{

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeCommand::~CBHChangeCommand(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ʱ�䴦��ִ���Ƿ�ʱ

@param      (i)DWORD dwTimer		��ʱʱ����
@param      (i)DWORD dwBaseTime		����ʱ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CBHChangeCommand::IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime)
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
//////////////////////////////////////////////////////////////////////////
/**
@brief				�����������ݲ����ط�������
                                                                 	
@param			(i)DWORD nSendLength		�������ݳ���
@param			(i)BYTE* pSendBuffer				�������ݻ���
@param			(o) BYTE* pRecvBuff				�������ݻ���	
@param			(o)DWORD& nRecvLength	�������ݳ���
@param			(i)DWORD dwTimer				��ʱʱ�䣨��λ�����룩

@retval				IOTRS_RESULT IOTRS_RES_OK:�ɹ�   ��IOTRS_RES_OK:ʧ��

@exception		��
*/
//////////////////////////////////////////////////////////////////////////
BHTRS_RESULT CBHChangeCommand::SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer)
{
	BHTRS_RESULT result = BHTRS_RES_OK;
	DWORD dwChk = 0;
	DWORD dwSize = 0;

	// ��������
	result = BHTrs_Send(m_hBH, nSendLength,pSendBuffer, &dwSize);	
	DWORD dwBaseTime= GetTickCount();
	bool istimeout = false;
	// ���ͳɹ�
	if(result == BHTRS_RES_OK){
		while(true){			
			// ��ȡ��ǰͨ��״̬
			result = BHTrs_GetStat(m_hBH, &dwChk);
			// ���ڴ���������
			if(result == BHTRS_RES_OK && (dwChk &BHTRS_ST_RCVQUE) != 0){
				// �������ݷ���
				result = BHTrs_Recv(m_hBH,RES_MAX,pRecvBuff,&nRecvLength);
				// �������ݳɹ�
				if (result == BHTRS_RES_OK){
					// �����������
					//if(pRecvBuff[0] != pSendBuffer[0]){
					//	result = CBHException::ERROR_CH_SEQUNCE_ERR;
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
					result = CBHChangeException::ERROR_BH_RECEIVE;
					break;
				}
			}
			// �����ڴ���������
			else{
				// ��ʱ�ж�
				if(IsTimeUp(dwTimer,dwBaseTime)){			
					result = CBHChangeException::ERROR_BH_RECEIVE_TIMEOUT;
					break;
				}
				continue;
			}
		} // end while
	}
	// ����ʧ��
	else{
		result = CBHChangeException::ERROR_BH_SEND;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief				�������������޷�������
                                                                 	
@param			(i)DWORD nSendLength		�������ݳ���
@param			(i)BYTE* pSendBuffer				�������ݻ���
@param			(o) BYTE* pRecvBuff				�������ݻ���	
@param			(o)DWORD& nRecvLength	�������ݳ���

@retval				IOTRS_RESULT IOTRS_RES_OK:�ɹ�   ��IOTRS_RES_OK:ʧ��

@exception		��
*/
//////////////////////////////////////////////////////////////////////////
BHTRS_RESULT CBHChangeCommand::SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer)
{
	BHTRS_RESULT result = BHTRS_RES_OK;
	DWORD dwChk  = 0;
	DWORD dwSize = 0;
	DWORD cnt    = 0;
	// ��������
	result = BHTrs_Send(m_hBH, nSendLength,pSendBuffer, &dwSize);							
	// ���ͳɹ�
	if(result == BHTRS_RES_OK){

	}
	// ����ʧ��
	else{
		result = CBHChangeException::ERROR_BH_SEND;
	}
	return result;
}