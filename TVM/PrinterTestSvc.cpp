#include "stdafx.h"
#include "PrinterTestSvc.h"
#include "printertestmaintaindlg.h"

BEGIN_MESSAGE_MAP(CPrinterTestSvc, CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::CPrinterTestSvc()
	:CTVMForeService(RECEIPT_RINTER_TEST_SVC)
{
	//��ӻ���
	GetDialogFlow()->AddDialog(IDD_20531_PRINTER_TEST_DLG,new CPrinterTestMaintainDlg(this));

	//���û�����
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_PRINT, _opl(GUIDE_ENTER_F3_ESC));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_PRINT,IDD_20531_PRINTER_TEST_DLG);

	// ���ó�ʼ������
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_PRINT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::~CPrinterTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::OnStart(){
	__super::OnStart();
	// ���ó�ʼ������
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_PRINT);
	m_Modol = Modol();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [ENTER]����������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	int index = theAPP_SESSION.GetInputToInt();
	switch(index)
	{
	case MENU_CODE_1:
		{
			m_OperationType = OPERATION_TYPE_RESET;
		}
		break;
	case MENU_CODE_2:
		{
			m_OperationType = OPERATION_TYPE_TEST_PRINT;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return FALSE;
	}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//����ѡ�����Ӧ�Ĳ���
	DoOperation();

	//���½�����ʾ
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]����������Ϣ

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CPrinterTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����ģ������

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::Modol& CPrinterTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ݸı��֪ͨ�����������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::DoOperation()
{
	int  nMenuNum = 0;
	long errCode = 0;
	bool bIsConn = false;
	theSERVICE_MGR.SetForeServiceBusy(true);
	try
	{
		//1.����
		if ( m_OperationType == OPERATION_TYPE_RESET)
		{
			nMenuNum = 0;
			//���Թر�����
			PRINTER_HELPER->PrinterClose();	//mxw
			//�����ٴδ�����
			PRINTER_HELPER->PrinterConnect();
			PRINTER_HELPER->CheckStatus();
		}
		//2.��ӡ����
		else if (m_OperationType == OPERATION_TYPE_TEST_PRINT)
		{
			nMenuNum = 1;
			CString sztxt=_T("Ӳ���Լ�-��ӡ����:\n~!@#$%^&*()+-=_{}[]:;,./|0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\nHardware auto-test,teste for bem sucedido!\nӲ���Լ�-���Գɹ�!\n\n");
			CStringArray strlist;
			strlist.Add(sztxt);
			PRINTER_HELPER->CheckStatus();
			PRINTER_HELPER->PrintReceiptByText(&strlist,false,false);
		}
		m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		CString comment(_T("PrinterTest"));
		// ��ӡ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,true,comment);
	}
	catch (CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		CString comment(_T("PrinterTest"));
		// ��ӡ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		CString comment(_T("PrinterTest"));
		// ��ӡ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,false,comment);
	}

	theSERVICE_MGR.SetForeServiceBusy(false);
}