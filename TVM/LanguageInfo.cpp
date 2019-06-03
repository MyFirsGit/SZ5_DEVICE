#include "StdAfx.h"
#include "LanguageInfo.h"
#include "CommonDefine.h"
#include "AppSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const LANGUAGE_FILE_NAME = _T("TVM_LANGUAGE.CSV");		// 语言信息文件名

CLanguageInfo CLanguageInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取单实例对象

@param      (i)void

@retval      CLanguageInfo& CLanguageInfo类对象

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CLanguageInfo& CLanguageInfo::GetInstance()
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
CLanguageInfo::CLanguageInfo()
{
	try {
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
CLanguageInfo::~CLanguageInfo()
{
	POSITION pos = NULL;
	CString sKey;
	LPLANGUAGE_DATA pLanguageData = NULL;
	pos = m_mapLanguageInfo.GetStartPosition();
	while(pos != NULL)
	{
		m_mapLanguageInfo.GetNextAssoc(pos,sKey,pLanguageData);
		delete pLanguageData;
	}
	m_mapLanguageInfo.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无

@retval      bool	true：成功	false：失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CLanguageInfo::Initialize() 
{
	CString sAppPath;
	// 获取当前路径
	if(!GetAppPath(sAppPath)){
		return false;
	}
	// 语言信息文件路径
	CString fileName = sAppPath + LANGUAGE_FILE_NAME;
	// 读取文件信息
	CXFile *languageFile = new CXFile(fileName);
	CString strLine;
	//CStringArray *strLanguageArray = NULL;
	CStringArray strLanguageArray;
	while(!feof(languageFile->m_pStream))   
	{
		languageFile->ReadString(strLine);
		if(strLine.IsEmpty()){
			continue;
		}
		strLanguageArray.RemoveAll();
		GetSplitStrArray(strLine,_T("$"),strLanguageArray);
		//strLanguageArray = SplitCString(strLine,_T("$"));
		//int count = strLanguageArray->GetSize();
		int count = strLanguageArray.GetSize();
		if(count < 4){
			continue;
		}
		else{
			LPLANGUAGE_DATA pLanguageData = new LANGUAGE_DATA;
			// 文言代码
			pLanguageData->msgCode = strLanguageArray.GetAt(0);
			// 繁体中文
			pLanguageData->strTraditionalChinese = strLanguageArray.GetAt(1);
			// 英文
			pLanguageData->strEnglish = strLanguageArray.GetAt(2);
			// 葡萄牙文
			pLanguageData->strPortuguese = strLanguageArray.GetAt(3);

			m_mapLanguageInfo.SetAt(pLanguageData->msgCode,pLanguageData);
		}
	}
	/*if(strLanguageArray != NULL){
	delete strLanguageArray;
	strLanguageArray = NULL;
	}*/
	if(languageFile != NULL){
		delete languageFile;
		languageFile = NULL;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取语言

@param      (i)LANGUAGE_MSG msgCode	文言代码
            (i)positionType=1 乘客界面，2站员界面

@retval     CString 代码的语言信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLanguageInfo::GetLanguageInfo(int msgCode,int Mode,int positionType)
{
	CString msg;
	int languageMode = 0;
	if(Mode == -1 ){
		languageMode = positionType==1? theAPP_SESSION.GetCurrentClientLanguageType():theAPP_SESSION.GetCurrentOperatorLanguageType();
	}
	else{
		languageMode = Mode;
	}
	switch(languageMode)
	{
	case LANGUAGE_ID_CHINESE:
		msg = GetTraditionalChinese((LANGUAGE_MSG)msgCode);
		break;
	case LANGUAGE_ID_ENGLISH:
		msg = GetEnglish((LANGUAGE_MSG)msgCode);
		break;
	case LANGUAGE_ID_PORTUGUESE:
		msg = GetPortuguese((LANGUAGE_MSG)msgCode);
		break;
	default:
		msg = GetTraditionalChinese((LANGUAGE_MSG)msgCode);
		break;
	}
	return msg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取繁体中文

@param      (i)LANGUAGE_MSG msgCode 文言代码

@retval     CString 代码的繁体中文信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLanguageInfo::GetTraditionalChinese(LANGUAGE_MSG msgCode)
{
	CString msg;
	CString sKey;
	LPLANGUAGE_DATA pLanguageData = NULL;
	sKey.Format(_T("%.8x"), msgCode);
	if(m_mapLanguageInfo.Lookup(sKey,pLanguageData))
	{
		msg = pLanguageData->strTraditionalChinese;
	}
	return msg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取英文

@param      (i)LANGUAGE_MSG msgCode 文言代码

@retval     CString 代码的英文信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLanguageInfo::GetEnglish(LANGUAGE_MSG msgCode)
{
	CString msg;
	CString sKey;
	LPLANGUAGE_DATA pLanguageData = NULL;
	sKey.Format(_T("%.8x"), msgCode);
	if(m_mapLanguageInfo.Lookup(sKey,pLanguageData))
	{
		msg = pLanguageData->strEnglish;
	}
	return msg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取葡萄牙文

@param      (i)LANGUAGE_MSG msgCode 文言代码

@retval     CString 代码的葡萄牙文信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLanguageInfo::GetPortuguese(LANGUAGE_MSG msgCode)
{
	CString msg;
	CString sKey;
	LPLANGUAGE_DATA pLanguageData = NULL;
	sKey.Format(_T("%.8x"), msgCode);
	if(m_mapLanguageInfo.Lookup(sKey,pLanguageData))
	{
		msg = pLanguageData->strPortuguese;
	}
	return msg;
}