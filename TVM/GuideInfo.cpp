#include "StdAfx.h"
#include "GuideInfo.h"
#include "SysException.h"
#include "Util.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GUIDE_INI_FILE_NAME		    _T("GuideInfo.INI")

CGuideInfo CGuideInfo::theInstance;

CGuideInfo& CGuideInfo::GetInstance()
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
CGuideInfo::CGuideInfo()
{
	try {
		m_guideIni = NULL;
		//// 初期化
		//Initialize();
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
CGuideInfo::~CGuideInfo()
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
int CGuideInfo::Initialize() 
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

		CString fileName = CString(szAppPath) + GUIDE_INI_FILE_NAME;

		// check BOM.INI file exist
		if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
		}

		// 文件属性改为可写
		SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
		m_guideIni = new CGetIni(fileName);
		return 0;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////

//@brief      取得车站模式文言
//
//@param      (i)LANGUAGE_MSG msgCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetOperationMessage(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode);
	return msg;
}

//////////////////////////////////////////////////////////////////////////

//@brief      取得通常业务乘客画面文言
//
//@param      (i)PASSENGER_MSG msgCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessageCn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_CHINESE);
	return msg;
}

//////////////////////////////////////////////////////////////////////////
//
//@brief      取得通常业务乘客画面文言
//
//@param      (i)PASSENGER_MSG msgCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessageEn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_ENGLISH);
	return msg;
}


//////////////////////////////////////////////////////////////////////////
//
//@brief      取得通常业务乘客画面文言
//
//@param      (i)PASSENGER_MSG msgCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessagePn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_PORTUGUESE);
	return msg;
}
//////////////////////////////////////////////////////////////////////////

//brief      取得维护业务操作员画面文言
//
//param      (i)MAINTENANCE_MSG msgCode   文言代码
//
//retval     CString  代码的中文信息
//
//exception  无

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetMaintenanceMessage(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode);
	return msg;
}
