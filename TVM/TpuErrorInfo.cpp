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
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
	// 获取语言模式
	LANGUAGETYPE_T languageMode = theAPP_SESSION.GetCurrentClientLanguageType();//theMAINTENANCE_INFO.GetLanguageMode();;
	//if (!m_mapTpuErrorInfo.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("0x%08X"), msgCode);
		// 获取文件内容等号后面的所有信息
		CString resultCodeMsg = m_guideIni->GetDataString(_T("TPUERROR_INFO"), sMessageID);
		CStringArray strLanguageArray;
		GetSplitStrArray(resultCodeMsg,_T("$"),strLanguageArray);
		// 根据当前语言模式选择显示内容
		
		switch(languageMode){
		case LANGUAGE_ID_CHINESE :
			resultCodeMsg = strLanguageArray.GetSize()>0?strLanguageArray.GetAt(0):_T("");		
			break;
		case LANGUAGE_ID_ENGLISH :
			resultCodeMsg = strLanguageArray.GetSize()>0?strLanguageArray.GetAt(1):_T("");		
			break;
		case LANGUAGE_ID_PORTUGUESE :
			resultCodeMsg = strLanguageArray.GetSize()>0?strLanguageArray.GetAt(2):_T("");		
			break;
		default:
			resultCodeMsg = strLanguageArray.GetSize()>0?strLanguageArray.GetAt(0):_T("");		
			break;			
		}		
		if(resultCodeMsg == _T(""))
		{			
			resultCodeMsg =_tl(TXT_MSG_GUIDE_THIS_CARD_CAN_NOT_READ);
		}
		resultCodeMsg = _tl(LANGUAGE_RED)+resultCodeMsg;
		m_mapTpuErrorInfo.SetAt(msgCode,resultCodeMsg);
		msg = resultCodeMsg;
	//}
	return msg;
}

