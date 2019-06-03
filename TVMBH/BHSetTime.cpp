#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHSetTime::CBHSetTime(_DATE_TIME current)
{
	m_current_datetime = current;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHSetTime::~CBHSetTime()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		取得执行结果

 @param		(o)_DATE_TIME* rsp	时间

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::GetResponse(_DATE_TIME* rsp)
{
	try {
		*rsp = m_bh_datetime;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = SetTimeFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		向纸币接收模块请求状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::SetTimeFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 拼接命令
	CStringA cmd;
	cmd.Format("%s%02x%02x%02x%02x%02x%02x", CMD_BH_SET_TIME, 
		m_current_datetime.wYear-2000,m_current_datetime.biMonth, m_current_datetime.biDay,
		m_current_datetime.biHour, m_current_datetime.biMinute, m_current_datetime.biSecond);
	cmd.MakeUpper();

	// 发送命令
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = { 0 };
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(cmd.GetLength(), (BYTE*)cmd.GetBuffer(cmd.GetLength()), recvbuf, recvlen);
	cmd.ReleaseBuffer();
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 验证并解析返回数据
	if (recvlen > 0){
		if (recvbuf[0] != 'M' || recvbuf[1] != 'A' || recvbuf[2] != '0')
			return CBHException::ERROR_BH_DESERIALIZE;
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// 读取时间
	recvlen = 0;
	memset(recvbuf, 0, RES_MAX);
	result = SendOneCommand(strlen(CMD_BH_GET_TIME), (BYTE*)CMD_BH_GET_TIME, recvbuf, recvlen);
	cmd.ReleaseBuffer();
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 验证并解析返回数据
	if (recvlen > 0){
		if (recvbuf[0] != 'M' || recvbuf[1] != 'A')
		{
			return CBHException::ERROR_BH_RECEIVE;
		}
		LPSTR junk = NULL;
		char waitconv[3];
		// 年
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+2, 2);
		m_bh_datetime.wYear = 2000 + strtoul(waitconv, &junk, 16);
		// 月
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+4, 2);
		m_bh_datetime.biMonth = strtoul(waitconv, &junk, 16);
		// 日
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+6, 2);
		m_bh_datetime.biDay = strtoul(waitconv, &junk, 16);
		// 时
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+8, 2);
		m_bh_datetime.biHour = strtoul(waitconv, &junk, 16);
		// 分
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+10, 2);
		m_bh_datetime.biMinute = strtoul(waitconv, &junk, 16);
		// 秒
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+12, 2);
		m_bh_datetime.biSecond = strtoul(waitconv, &junk, 16);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
