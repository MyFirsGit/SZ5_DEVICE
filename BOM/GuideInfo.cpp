#include "StdAfx.h"
#include "GuideInfo.h"
#include "SysException.h"
#include "Util.h"

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
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////

//@brief      ȡ�ó�վģʽ����
//
//@param      (i)OPERATION_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString CGuideInfo::GetOperationMessage(OPERATION_MSG msgCode)
{
	CString msg;
	if (!m_mapOperationMsg.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("%d"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("OPERATION_MSG"), sMessageID);
		m_mapOperationMsg.SetAt(msgCode,resultCodeMsg);
		msg = resultCodeMsg;
	}
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
CString CGuideInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
{
	CString msg;
	if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("%d"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);

		// ����
		CString strMsgCn;
		AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
		m_mapPassengerMsgCn.SetAt(msgCode,strMsgCn);

		// ��������
		msg = strMsgCn;
	}
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
CString CGuideInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
{
	CString msg;
	if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("%d"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);

		// Ӣ��
		CString strMsgEn;
		AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
		m_mapPassengerMsgEn.SetAt(msgCode,strMsgEn);

		// ����Ӣ��
		msg = strMsgEn;
	}
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
CString CGuideInfo::GetMaintenanceMessage(MAINTENANCE_MSG msgCode)
{
	CString msg;
	if (!m_mapMaintenanceMsg.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("%d"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("MAINTENANCE_MSG"), sMessageID);
		m_mapMaintenanceMsg.SetAt(msgCode,resultCodeMsg);
		msg = resultCodeMsg;
	}
	return msg;
}
