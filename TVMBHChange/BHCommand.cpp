#include "stdafx.h"
#include "BHChangeCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief    　构造函数

@param     无

@retval      无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeCommand::CBHChangeCommand()
{

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeCommand::~CBHChangeCommand(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断时间处理执行是否超时

@param      (i)DWORD dwTimer		超时时间间隔
@param      (i)DWORD dwBaseTime		基本时间

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
		dwCurTime = GetTickCount();		// 当前时间
		dwUpTime = dwBaseTime + dwTimer;// 结束时间
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// 超时（当前时间 > 结束时间）
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
@brief				发送命令数据并返回反馈数据
                                                                 	
@param			(i)DWORD nSendLength		发送数据长度
@param			(i)BYTE* pSendBuffer				发送数据缓存
@param			(o) BYTE* pRecvBuff				接收数据缓存	
@param			(o)DWORD& nRecvLength	接收数据长度
@param			(i)DWORD dwTimer				超时时间（单位：毫秒）

@retval				IOTRS_RESULT IOTRS_RES_OK:成功   非IOTRS_RES_OK:失败

@exception		无
*/
//////////////////////////////////////////////////////////////////////////
BHTRS_RESULT CBHChangeCommand::SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer)
{
	BHTRS_RESULT result = BHTRS_RES_OK;
	DWORD dwChk = 0;
	DWORD dwSize = 0;

	// 发送数据
	result = BHTrs_Send(m_hBH, nSendLength,pSendBuffer, &dwSize);	
	DWORD dwBaseTime= GetTickCount();
	bool istimeout = false;
	// 发送成功
	if(result == BHTRS_RES_OK){
		while(true){			
			// 获取当前通信状态
			result = BHTrs_GetStat(m_hBH, &dwChk);
			// 存在待接收数据
			if(result == BHTRS_RES_OK && (dwChk &BHTRS_ST_RCVQUE) != 0){
				// 接收数据反馈
				result = BHTrs_Recv(m_hBH,RES_MAX,pRecvBuff,&nRecvLength);
				// 接收数据成功
				if (result == BHTRS_RES_OK){
					// 检查命令序列
					//if(pRecvBuff[0] != pSendBuffer[0]){
					//	result = CBHException::ERROR_CH_SEQUNCE_ERR;
					//	break;
					//}
					//// 检查状态码
					//if(pRecvBuff[3] != 0x00){
					//	result = pRecvBuff[3];
					//	break;
					//}
					break;
				}
				// 接收数据失败
				else{
					result = CBHChangeException::ERROR_BH_RECEIVE;
					break;
				}
			}
			// 不存在待接收数据
			else{
				// 超时判断
				if(IsTimeUp(dwTimer,dwBaseTime)){			
					result = CBHChangeException::ERROR_BH_RECEIVE_TIMEOUT;
					break;
				}
				continue;
			}
		} // end while
	}
	// 发送失败
	else{
		result = CBHChangeException::ERROR_BH_SEND;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief				发送命令数据无反馈数据
                                                                 	
@param			(i)DWORD nSendLength		发送数据长度
@param			(i)BYTE* pSendBuffer				发送数据缓存
@param			(o) BYTE* pRecvBuff				接收数据缓存	
@param			(o)DWORD& nRecvLength	接收数据长度

@retval				IOTRS_RESULT IOTRS_RES_OK:成功   非IOTRS_RES_OK:失败

@exception		无
*/
//////////////////////////////////////////////////////////////////////////
BHTRS_RESULT CBHChangeCommand::SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer)
{
	BHTRS_RESULT result = BHTRS_RES_OK;
	DWORD dwChk  = 0;
	DWORD dwSize = 0;
	DWORD cnt    = 0;
	// 发送数据
	result = BHTrs_Send(m_hBH, nSendLength,pSendBuffer, &dwSize);							
	// 发送成功
	if(result == BHTRS_RES_OK){

	}
	// 发送失败
	else{
		result = CBHChangeException::ERROR_BH_SEND;
	}
	return result;
}