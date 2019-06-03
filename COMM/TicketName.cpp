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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTicketName::CTicketName()
{
	try {
		m_DeviceIni = NULL;

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
CTicketName::~CTicketName()
{
	delete m_DeviceIni;
	m_DeviceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTicketName::Initialize() 
{
	try {
		// ���ѳ�ʼ������ֱ�ӷ���
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
//@brief      // ��ȡ��Ʊ����
//
//@param      (i)int NO   ��Ʊ����
//
//@retval     CString     ��Ʊ����
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
//@brief      // ��ȡ��Ʊ����
//
//@param      (i)int NO   ��Ʊ����
//
//@retval     CString     ��Ʊ����
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

