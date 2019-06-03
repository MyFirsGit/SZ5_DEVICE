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
CBHGetVersion::CBHGetVersion()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHGetVersion::~CBHGetVersion()
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
long CBHGetVersion::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetVersionFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		取得执行结果

 @param		(o)BH_VERSION* rsp	版本信息

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetVersion::GetResponse(BH_VERSION* rsp)
{
	try {
		*rsp = m_bhVer;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		向纸币接收模块获取版本

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetVersion::GetVersionFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}
	
	// RS0 : 获取版本：上部传动系统版本
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = {0}; 
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_TOP), (BYTE*)CMD_BH_VER_TRANS_TOP, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.trans_top);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}
	
	// RS1 : 获取版本：识别器OS版本
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_TOP), (BYTE*)CMD_BH_VER_TRANS_TOP, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.os);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS2 : 获取版本：识别器算法版本
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_ALGORITHM), (BYTE*)CMD_BH_VER_ALGORITHM, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.algorithm);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS3 : 获取版本：下部传动系统版本
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_BOTTOM), (BYTE*)CMD_BH_VER_TRANS_BOTTOM, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.trans_bottom);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS5 : 获取版本：ODS固件版本
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_ODS), (BYTE*)CMD_BH_VER_ODS, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.ods);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
