#include "StdAfx.h"
#include "LanguageInfo.h"
#include "CommonDefine.h"
#include "AppSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const LANGUAGE_FILE_NAME = _T("TVM_LANGUAGE.CSV");		// ������Ϣ�ļ���

CLanguageInfo CLanguageInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ʵ������

@param      (i)void

@retval      CLanguageInfo& CLanguageInfo�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CLanguageInfo& CLanguageInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLanguageInfo::CLanguageInfo()
{
	try {
		Initialize();
	}
	catch (...) {
		// �����쳣��Ϣ
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief       ���ڻ�

@param       ��

@retval      bool	true���ɹ�	false��ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CLanguageInfo::Initialize() 
{
	CString sAppPath;
	// ��ȡ��ǰ·��
	if(!GetAppPath(sAppPath)){
		return false;
	}
	// ������Ϣ�ļ�·��
	CString fileName = sAppPath + LANGUAGE_FILE_NAME;
	// ��ȡ�ļ���Ϣ
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
			// ���Դ���
			pLanguageData->msgCode = strLanguageArray.GetAt(0);
			// ��������
			pLanguageData->strTraditionalChinese = strLanguageArray.GetAt(1);
			// Ӣ��
			pLanguageData->strEnglish = strLanguageArray.GetAt(2);
			// ��������
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
@brief      ��ȡ����

@param      (i)LANGUAGE_MSG msgCode	���Դ���
            (i)positionType=1 �˿ͽ��棬2վԱ����

@retval     CString �����������Ϣ

@exception  ��
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
@brief      ��ȡ��������

@param      (i)LANGUAGE_MSG msgCode ���Դ���

@retval     CString ����ķ���������Ϣ

@exception  ��
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
@brief      ��ȡӢ��

@param      (i)LANGUAGE_MSG msgCode ���Դ���

@retval     CString �����Ӣ����Ϣ

@exception  ��
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
@brief      ��ȡ��������

@param      (i)LANGUAGE_MSG msgCode ���Դ���

@retval     CString ���������������Ϣ

@exception  ��
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