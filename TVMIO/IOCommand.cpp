//#include "stdafx.h"
//#include "IOCommand.h"
//#include "IOException.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief    �����캯��
//
//@param     ��
//
//@retval      ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOCommand::CIOCommand()
//{
//
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      (i)��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOCommand::~CIOCommand(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief				�����������ݲ����ط�������
//                                                                 	
//@param			(i)DWORD nSendLength		�������ݳ���
//@param			(i)BYTE* pSendBuffer				�������ݻ���
//@param			(o) BYTE* pRecvBuff				�������ݻ���	
//@param			(o)DWORD& nRecvLength	�������ݳ���
//@param			(i)DWORD dwTimer				��ʱʱ�䣨��λ�����룩
//
//@retval				IOTRS_RESULT IOTRS_RES_OK:�ɹ�   ��IOTRS_RES_OK:ʧ��
//
//@exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//IOTRS_RESULT CIOCommand::SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer)
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	DWORD dwChk = 0;
//	DWORD dwSize = 0;
//	// ��������
//	result = IOTrs_Send(m_hTVMIO, nSendLength,pSendBuffer, &dwSize);
//	DWORD dwBaseTime= GetTickCount();
//	bool istimeout = false;
//	// ���ͳɹ�
//	if(result == IOTRS_RES_OK){
//		while(true){
//			// ��ȡ��ǰͨ��״̬
//			result = IOTrs_GetStat(m_hTVMIO, &dwChk);
//			// ���ڴ���������
//			if(result == IOTRS_RES_OK && (dwChk &IOTRS_ST_RCVQUE) != 0){
//				// �������ݷ���
//				result = IOTrs_Recv(m_hTVMIO,MAX_RSP_LEN,pRecvBuff,&nRecvLength);
//				// �������ݳɹ�
//				if (result == IOTRS_RES_OK){
//					// �����������
//					if(pRecvBuff[0] != pSendBuffer[0]){
//						result = CIOException::ERROR_IO_SEQUNCE_ERR;
//						break;
//					}
//					// ���״̬��
//					if(pRecvBuff[1] == IO_STATE_ERROR){
//						result = pRecvBuff[2];
//						break;
//					}
//					break;
//				}
//				// ��������ʧ��
//				else{
//					result = CIOException::ERROR_IO_RECEIVE;
//					break;
//				}
//			}
//			// �����ڴ���������
//			else{
//				// ��ʱ�ж�
//				if(IsTimeUp(dwTimer,dwBaseTime)){			
//					result = CIOException::ERROR_IO_RECEIVE_TIMEOUT;
//					break;
//				}
//				continue;
//			}
//		} // end while
//	}
//	// ����ʧ��
//	else{
//		result = CIOException::ERROR_IO_SEND;
//	}
//	return result;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж�ʱ�䴦��ִ���Ƿ�ʱ
//
//@param      (i)DWORD dwTimer		��ʱʱ����
//@param      (i)DWORD dwBaseTime		����ʱ��
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//BOOL CIOCommand::IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime)
//{
//	BOOL bRet = FALSE;
//	DWORD dwCurTime;
//	DWORD dwUpTime;
//	//            dwTime
//	//    |-------------------|
//	// dwBaseTime          dwUpTime
//	//    |----------|--------|-----
//	//           dwCurTime
//	// Has timer
//	if (dwTimer != 0){
//		dwCurTime = GetTickCount();		// ��ǰʱ��
//		dwUpTime = dwBaseTime + dwTimer;// ����ʱ��
//		if (dwUpTime >= dwBaseTime){
//			if (dwCurTime >= dwBaseTime){
//				// ��ʱ����ǰʱ�� > ����ʱ�䣩
//				if (dwCurTime >= dwUpTime){
//					bRet = TRUE;
//				}
//			}
//			else{
//				bRet = TRUE;
//			}
//		}
//		else{
//			if (dwCurTime < dwBaseTime){
//				if (dwCurTime >= dwUpTime){
//					bRet = TRUE;
//				}
//			}
//		}
//	}
//
//	return bRet;
//}