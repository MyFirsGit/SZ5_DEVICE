#include "StdAfx.h"
#include "UPSLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUPSLog CUPSLog::UPSLogInstance;

/**
@brief UPS日志trace
*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSLog::CUPSLog():CLog(FILTER_UPS_LOG)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSLog::~CUPSLog(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      对日志文件进行写操作

@param      (i) cmdName    命令名
@param      (i) lpData     需要写的数据
@param      (i) iLen       需要写的长度
@param      (i) iMode      发送还是接收

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUPSLog::WriteLogFile(CString cmdName,LPBYTE lpData,int iLen, SEND_MODE iMode)
{
	CString sData = _T("");

	if(MODE_SEND == iMode){
		sData = "> ";
	}
	else{
		sData = "< ";
	}

	sData = sData + cmdName + _T(" ");

	for (int i = 0; i < iLen; i++){
		CString sHex = _T("");
		sHex.Format(_T("%02x"),*(lpData + i));
		sData += sHex;
	}
	// write data to log file
	__super::WriteData(sData);
}
