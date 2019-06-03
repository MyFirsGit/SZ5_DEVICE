#include "stdafx.h"
#include "TransactionHistorySvc.h"
#include "TransactionHistoryBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTransactionHistorySvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	/*ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F5,OnKeyboardF5)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F6,OnKeyboardF6)*/
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTransactionHistorySvc::CTransactionHistorySvc()
	:CTVMForeService(TRANSACTION_HISTORY_SVC)
{
    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG, new CTransactionHistoryBaseDlg(this));

    // ���ó˿ͻ���

    // ���û������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F6_ESC));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CTransactionHistoryBaseDlg::IDD);

    // ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

	m_nMaxLoadCount = theFunction_INFO.GetLoadMaxTransactionHistory();
}

void CTransactionHistorySvc::OnStart()
{
	//��ԃ���ה���
	//CString szbegintime=_T("20140701 6:15:00");
	//CString szendtime=_T("20140829 6:15:29");
	m_vec_transInfo.clear();
	try{
		long lselect=DATA_HELPER->DbSelectTopNumTransInfo(m_vec_transInfo,m_nMaxLoadCount);
		
		if(lselect == 0 && m_vec_transInfo.size() > 0){
			m_DataModel.ptransInfo = &m_vec_transInfo[0];
		}
		else{
			m_DataModel.ptransInfo = NULL;
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	
	
	long lHistoryNum=m_vec_transInfo.size();

	updateExceptionInfo();

	SetPageInfo(lHistoryNum,1,15);				// ʵ�ַ�ҳ

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

	m_pDialogFlow->ShowFirstDialogGroup();

	theAPP_SESSION.SetMenuLevel(TRANSACTION_HISTORY_SVC);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC));
}

void CTransactionHistorySvc::updateExceptionInfo()
{
	m_ExceptionList.clear();
	m_ExceptionList = theEXCEPTION_MGR.GetAllException();
	m_ExceptionNum = m_ExceptionList.size();

	if(m_ExceptionNum>=1)
	{
		CSysException &e = m_ExceptionList[0];
		CString strMTC;
		strMTC.Format(_T("%08X"),theEXCEPTION_MGR.GetActualMTC(e));
		m_DataModel.szMtc=strMTC;
		CString strEc;
		strEc.Format(_T("%04X"),theEXCEPTION_MGR.GetEC(e));
		m_DataModel.szEc=strEc;
		m_DataModel.szException=theEXCEPTION_MGR.GetDescription(e);
		m_DataModel.szSolution=theEXCEPTION_MGR.GetPrompt(e);
		COleDateTime occurTime=e.GetOccurTime();
		m_DataModel.szExceptionTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			occurTime.GetYear(),
			occurTime.GetMonth(),
			occurTime.GetDay(),
			occurTime.GetHour(),
			occurTime.GetMinute(),
			occurTime.GetSecond());
	}else
	{
		// ����Ϊ����ʾ�κ��쳣��Ϣ
		m_DataModel.szEc = _T("");
		m_DataModel.szException = _T("");
		m_DataModel.szSolution = _T("");
		m_DataModel.szExceptionTime = _T("");
		m_DataModel.szMtc = _T("");
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
CTransactionHistorySvc::~CTransactionHistorySvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTransactionHistorySvc::Model& CTransactionHistorySvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTransactionHistorySvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [0~9] ��������Ϣ

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CTransactionHistorySvc::OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/)
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTransactionHistorySvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	theEXCEPTION_MGR.RepareException();
	updateExceptionInfo();
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTransactionHistorySvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}


void CTransactionHistorySvc::turnPages(long lIndex)
{
	if(NULL != m_DataModel.ptransInfo && lIndex >= 0)
		m_DataModel.ptransInfo=&m_vec_transInfo[lIndex];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F5] ��������Ϣ

@param      (i)��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTransactionHistorySvc::pageDown(long lIndex)
{
	m_DataModel.ptransInfo=&m_vec_transInfo[lIndex];
	/*if(m_iter_transInfo!=m_vec_transInfo.begin())
	{
		if(--m_iter_transInfo!=m_vec_transInfo.begin())
		{
			m_DataModel.bfirst=FALSE;
		}else
		{
			m_DataModel.bfirst=TRUE;
		}
	}else
	{
	}
	m_DataModel.ptransInfo=&(*m_iter_transInfo);
	NotifyDataModelChanged();*/
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F6] ��������Ϣ

@param      (i)��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTransactionHistorySvc::pageUp(long lIndex)
{
	m_DataModel.ptransInfo=&m_vec_transInfo[lIndex];
	/*if(m_iter_transInfo!=m_vec_transInfo.end())
	{
		if(++m_iter_transInfo!=m_vec_transInfo.end())
		{
			m_DataModel.bend=FALSE;
		}else
		{
			m_DataModel.bend=TRUE;
			m_iter_transInfo--;
		}
		m_DataModel.ptransInfo=&(*m_iter_transInfo);
		NotifyDataModelChanged();
	}else
	{
	}*/
	return TRUE;
}
