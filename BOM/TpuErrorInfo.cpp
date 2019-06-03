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
		//throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
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
	if(0x0005010B!=msgCode)//Ӧ�ñ�����
	{
		if((msgCode&0x00050000) == 0x00050000)
		{
			msg.Format(_T("���죩����ʧ�ܣ��������[0x%08X]"),msgCode);
			return msg;
		}
	}
	if((msgCode&0x00060000) == 0x00060000)
	{
		msg.Format(_T("���죩д��ʧ�ܣ��������[0x%08X]"),msgCode);
		return msg;
	}
	if (!m_mapTpuErrorInfo.Lookup(msgCode,msg)) {
		CString sMessageID;
		sMessageID.Format(_T("0x%08X"), msgCode);
		CString resultCodeMsg = m_guideIni->GetDataString(_T("TPUERROR_INFO"), sMessageID);
		if(resultCodeMsg == _T(""))
		{
			resultCodeMsg.Format(_T("��д��δ֪���󣬴�����룺0x%08X"),msgCode);
		}
		resultCodeMsg = _T("���죩")+resultCodeMsg;
		m_mapTpuErrorInfo.SetAt(msgCode,resultCodeMsg);
		msg = resultCodeMsg;
	}
	return msg;
}

