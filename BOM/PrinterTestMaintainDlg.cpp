#include "stdafx.h"
#include "BOM.h"
#include "PrinterTestMaintainDlg.h"
#include "guideinfo.h"
#include "PrinterTestSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPrinterTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CPrinterTestMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestMaintainDlg::CPrinterTestMaintainDlg(CService* pService)
: COperationDlg(CPrinterTestMaintainDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestMaintainDlg::~CPrinterTestMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���崴������

@param      (i)LPCREATESTRUCT lpCreateStruct ���崴��������

@retval     int		\n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	return __super::OnCreate(lpCreateStruct,   GUIDE_AREA | BTN_OK) ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try
	{
		CPrinterTestSvc* pPrinterTestSvc = (CPrinterTestSvc*)GetService();

		// ���öԻ����ڷ�æ״̬
		theSERVICE_MGR.SetForeServiceBusy(true);
		// ��������Ϣ
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_TESTING));
		// �رմ�ӡ��
		pPrinterTestSvc->GetPRTHelper()->PrinterClose();
		// ���Ӵ�ӡ��
		pPrinterTestSvc->GetPRTHelper()->PrinterConnect(theBOM_INFO.GetPrinterCommPort(), theBOM_INFO.GetPrinterBaudRate());
		// ������ӡ����
		pPrinterTestSvc->PrinterPrint();
		// �Ƴ��쳣
		theEXCEPTION_MGR.RemoveException(CPrinterException::MODULE_ID);
		// ��ʾ��ӡ�����������Ϣ
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_TEST_FINISH));
		// ȡ���Ի���æ״̬
		theSERVICE_MGR.SetForeServiceBusy(false);

		HideCompoment(BTN_OK);
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	}
	return 0;
}

