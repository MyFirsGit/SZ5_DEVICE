#include "StdAfx.h"
#include "TpuErrorInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TPUERROR_INI_FILE_NAME		    _T("TpuErrorCode.ini")

TpuErrorInfo TpuErrorInfo::theInstance;

TpuErrorInfo& TpuErrorInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TpuErrorInfo::TpuErrorInfo()
{
	try {
		m_guideIni = NULL;
		// 初期化
		Initialize();
	}
	catch (...) {
		// 忽略异常信息
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TpuErrorInfo::~TpuErrorInfo()
{
	delete m_guideIni;
	m_guideIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int TpuErrorInfo::Initialize() 
{
	try {
		// 若已初始化，则直接返回
		if (m_guideIni != NULL) {
			return 0;
		}

		TCHAR szAppPath[_MAX_PATH];

		if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
		}

		CString fileName = CString(szAppPath) + TPUERROR_INI_FILE_NAME;

		// check TPUERROR_INI_FILE_NAME.INI file exist
		if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
		}

		// 文件属性改为可写
		SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
		m_guideIni = new CGetIni(fileName);
		return 0;
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////

//@brief      取得tpu错误信息文言
//
//@param      (i)OPERATION_MSG msgCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CString TpuErrorInfo::GetTpuErrorInfo(int msgCode)
{
	CString msg;
	if(0x0005010B!=msgCode)//应用被锁定
	{
		if((msgCode&0x00050000) == 0x00050000)
		{
			msg.Format(_T("（红）读卡失败，错误代码[0x%08X]"),msgCode);
			return msg;
		}
	}
	if((msgCode&0x00060000) == 0x00060000)
	{
		msg.Format(_T("（红）写卡失败，错误代码[0x%08X]"),msgCode);
		return msg;
	}
	if (!m_mapTpuErrorInfo.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("0x%08X"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("TPUERROR_INFO"), sMessageID);
		if(resultCodeMsg == _T(""))
		{
			resultCodeMsg.Format(_T("读写器未知错误，错误代码：0x%08X"),msgCode);
		}
		resultCodeMsg = _T("（红）")+resultCodeMsg;
		m_mapTpuErrorInfo.SetAt(msgCode,resultCodeMsg);
		msg = resultCodeMsg;
	}
	return msg;
}

