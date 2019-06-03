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
//@brief    　构造函数
//
//@param     无
//
//@retval      无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOCommand::CIOCommand()
//{
//
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      析构函数
//
//@param      (i)无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOCommand::~CIOCommand(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief				发送命令数据并返回反馈数据
//                                                                 	
//@param			(i)DWORD nSendLength		发送数据长度
//@param			(i)BYTE* pSendBuffer				发送数据缓存
//@param			(o) BYTE* pRecvBuff				接收数据缓存	
//@param			(o)DWORD& nRecvLength	接收数据长度
//@param			(i)DWORD dwTimer				超时时间（单位：毫秒）
//
//@retval				IOTRS_RESULT IOTRS_RES_OK:成功   非IOTRS_RES_OK:失败
//
//@exception		无
//*/
////////////////////////////////////////////////////////////////////////////
//IOTRS_RESULT CIOCommand::SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer)
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	DWORD dwChk = 0;
//	DWORD dwSize = 0;
//	// 发送数据
//	result = IOTrs_Send(m_hTVMIO, nSendLength,pSendBuffer, &dwSize);
//	DWORD dwBaseTime= GetTickCount();
//	bool istimeout = false;
//	// 发送成功
//	if(result == IOTRS_RES_OK){
//		while(true){
//			// 获取当前通信状态
//			result = IOTrs_GetStat(m_hTVMIO, &dwChk);
//			// 存在待接收数据
//			if(result == IOTRS_RES_OK && (dwChk &IOTRS_ST_RCVQUE) != 0){
//				// 接收数据反馈
//				result = IOTrs_Recv(m_hTVMIO,MAX_RSP_LEN,pRecvBuff,&nRecvLength);
//				// 接收数据成功
//				if (result == IOTRS_RES_OK){
//					// 检查命令序列
//					if(pRecvBuff[0] != pSendBuffer[0]){
//						result = CIOException::ERROR_IO_SEQUNCE_ERR;
//						break;
//					}
//					// 检查状态码
//					if(pRecvBuff[1] == IO_STATE_ERROR){
//						result = pRecvBuff[2];
//						break;
//					}
//					break;
//				}
//				// 接收数据失败
//				else{
//					result = CIOException::ERROR_IO_RECEIVE;
//					break;
//				}
//			}
//			// 不存在待接收数据
//			else{
//				// 超时判断
//				if(IsTimeUp(dwTimer,dwBaseTime)){			
//					result = CIOException::ERROR_IO_RECEIVE_TIMEOUT;
//					break;
//				}
//				continue;
//			}
//		} // end while
//	}
//	// 发送失败
//	else{
//		result = CIOException::ERROR_IO_SEND;
//	}
//	return result;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断时间处理执行是否超时
//
//@param      (i)DWORD dwTimer		超时时间间隔
//@param      (i)DWORD dwBaseTime		基本时间
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
//		dwCurTime = GetTickCount();		// 当前时间
//		dwUpTime = dwBaseTime + dwTimer;// 结束时间
//		if (dwUpTime >= dwBaseTime){
//			if (dwCurTime >= dwBaseTime){
//				// 超时（当前时间 > 结束时间）
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