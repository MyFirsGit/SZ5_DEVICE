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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideInfo::CGuideInfo()
{
	try {
		m_guideIni = NULL;
		//// ���ڻ�
		//Initialize();
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
CGuideInfo::~CGuideInfo()
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
int CGuideInfo::Initialize() 
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

		CString fileName = CString(szAppPath) + GUIDE_INI_FILE_NAME;

		// check BOM.INI file exist
		if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
		}

		// �ļ����Ը�Ϊ��д
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

//@brief      ȡ�ó�վģʽ����
//
//@param      (i)LANGUAGE_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetOperationMessage(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode);
	return msg;
}

//////////////////////////////////////////////////////////////////////////

//@brief      ȡ��ͨ��ҵ��˿ͻ�������
//
//@param      (i)PASSENGER_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessageCn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_CHINESE);
	return msg;
}

//////////////////////////////////////////////////////////////////////////
//
//@brief      ȡ��ͨ��ҵ��˿ͻ�������
//
//@param      (i)PASSENGER_MSG msgCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessageEn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_ENGLISH);
	return msg;
}


//////////////////////////////////////////////////////////////////////////
//
//@brief      ȡ��ͨ��ҵ��˿ͻ�������
//
//@param      (i)PASSENGER_MSG msgCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetPassengerMessagePn(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode,LANGUAGE_ID_PORTUGUESE);
	return msg;
}
//////////////////////////////////////////////////////////////////////////

//brief      ȡ��ά��ҵ�����Ա��������
//
//param      (i)MAINTENANCE_MSG msgCode   ���Դ���
//
//retval     CString  �����������Ϣ
//
//exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetMaintenanceMessage(LANGUAGE_MSG msgCode)
{
	CString msg = theLANGUAGE_INFO.GetLanguageInfo(msgCode);
	return msg;
}
