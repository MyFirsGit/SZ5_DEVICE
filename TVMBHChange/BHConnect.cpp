#include "stdafx.h"
#include "BHChangeCommand.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		(i)int nPort 串口端口号
 @param		(i)int nBaud 串口波特率
 @param		(i)int nSize 字长

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeConnect::CBHChangeConnect(int nPort, int nBaud, int nSize)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
	this->m_nSize = nSize;
	this->m_connectParam.ComPort           = m_nPort;
	this->m_connectParam.Speed             = m_nBaud;
	this->m_connectParam.MaxRecvCnt        = MAX_SEND_ITEMS_COUNT;
	this->m_connectParam.MaxSendCnt        = MAX_RECEIVE_ITEMS_COUNT;
	this->m_connectParam.MaxRecvBuffSize   = MAX_BUFFER_SIZE;
	this->m_connectParam.MaxSendBuffSize   = MAX_BUFFER_SIZE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeConnect::~CBHChangeConnect()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeConnect::ExecuteCommand()
{
	long lRet = 1;
	lRet = ConnectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		打开串口，并设置相关参数

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeConnect::ConnectFunction()
{
	DWORD errCode = CBHChangeException::ERROR_BH_OPEN_FAILED;

	// 若打开，则需要关闭
	if (m_hBH != NULL){
		errCode = BHTrs_Close(m_hBH);
		m_hBH = NULL;
		if (errCode != BHTRS_RES_OK){
			return CBHChangeException::ERROR_BH_CLOSE_FAILED;
		}
	}

	// 打开
	errCode = BHTrs_Open(&m_hBH, &m_connectParam);
	if (errCode != BHTRS_RES_OK){
		m_hBH = NULL;
		return CBHChangeException::ERROR_BH_OPEN_FAILED;
	}

	return 0;
}
