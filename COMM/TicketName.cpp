#include "StdAfx.h"
#include "TicketName.h"
#include "Util.h"
#include "SysException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TICKET_NAME_FILE_NAME		    _T("TICKETNAME.INI")

CTicketName CTicketName::theInstance;

CTicketName& CTicketName::GetInstance()
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
CTicketName::CTicketName()
{
	try {
		m_DeviceIni = NULL;

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
CTicketName::~CTicketName()
{
	delete m_DeviceIni;
	m_DeviceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTicketName::Initialize() 
{
	try {
		// 若已初始化，则直接返回
		if (m_DeviceIni != NULL) {
			return 0;
		}

		TCHAR szAppPath[_MAX_PATH];

		if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
		}

		CString fileName = CString(szAppPath) + TICKET_NAME_FILE_NAME;

		// check DEVICE.INI file exist
		if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
		}

		m_DeviceIni = new CGetIni(fileName);

		return 0;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
	}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      // 获取车票名称
//
//@param      (i)int NO   车票代码
//
//@retval     CString     车票名称
//
//@exception  none
//*/
//////////////////////////////////////////////////////////////////////////
CString CTicketName::GetTicketNameCn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TICKET_NAME"), strNo);

	CString strMsg = _T("");
	AfxExtractSubString(strMsg,resultCodeMsg,0,':');

	return strMsg;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      // 获取车票名称
//
//@param      (i)int NO   车票代码
//
//@retval     CString     车票名称
//
//@exception  none
//*/
//////////////////////////////////////////////////////////////////////////
CString CTicketName::GetTicketNameEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TICKET_NAME"), strNo);

	CString strMsg = _T("");
	AfxExtractSubString(strMsg,resultCodeMsg,1,':');

	return strMsg;
}

