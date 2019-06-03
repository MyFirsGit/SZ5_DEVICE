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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
TpuErrorInfo::TpuErrorInfo()
{
	try {
		m_guideIni = NULL;
		// ���ڻ�
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
TpuErrorInfo::~TpuErrorInfo()
{
	delete m_guideIni;
	m_guideIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int TpuErrorInfo::Initialize() 
{
	try {
		// ���ѳ�ʼ������ֱ�ӷ���
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

		// �ļ����Ը�Ϊ��д
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

//@brief      ȡ��tpu������Ϣ����
//
//@param      (i)OPERATION_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString TpuErrorInfo::GetTpuErrorInfo(int msgCode)
{
	CString msg;
	// ��ȡ����ģʽ
	LANGUAGETYPE_T languageMode = theAPP_SESSION.GetCurrentClientLanguageType();//theMAINTENANCE_INFO.GetLanguageMode();;
	//if (!m_mapTpuErrorInfo.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("0x%08X"), msgCode);
		// ��ȡ�ļ����ݵȺź����������Ϣ
		CString resultCodeMsg = m_guideIni->GetDataString(_T("TPUERROR_INFO"), sMessageID);
		CStringArray strLanguageArray;
		GetSplitStrArray(resultCodeMsg,_T("$"),strLanguageArray);
		// ���ݵ�ǰ����ģʽѡ����ʾ����
		
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

