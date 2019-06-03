#include "stdafx.h"
#include "CHCommand.h"


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
CCHConnect::CCHConnect(int nPort, int nBaud, int nSize)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
	this->m_nSize = nSize;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHConnect::~CCHConnect()
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
long CCHConnect::ExecuteCommand()
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
long CCHConnect::ConnectFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("CONNECT"));
	tCHADevReturn DevStatus;
	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iSetCommPara:comport %d baud %d "),m_nPort,m_nBaud);
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	//tDevReturn sDevReturn[8];//接口定义说明有可能会返回多维状态 王峰
	errCode = CHA_CommOpen (m_nPort, m_nBaud, &DevStatus);
	//errCode = gOCAS002Dev.iSetCommPara(sDevReturn);
	if (errCode != E_NO_CH_ERROR){
		m_hCH = NULL;
		//errCode = sDevReturn[0].iLogicCode;
	}
	else{
		m_hCH = 1;
	}

	sLogString.Format(_T("iSetCommPara: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
