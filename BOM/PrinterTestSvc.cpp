#include "stdafx.h"
#include "PrinterTestSvc.h"
#include "bominfo.h"
#include "printertestmaintaindlg.h"
#include "guideinfo.h"
#include "TemplateHelper.h"

BEGIN_MESSAGE_MAP(CPrinterTestSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::CPrinterTestSvc()
:CBOMForeService(PRINTER_TEST_SVC)
{
	GetDialogFlow()->AddDialog(IDD_20531_PRINTER_TEST_DLG,new CPrinterTestMaintainDlg(this));
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_PRINT, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_PRINT,IDD_20531_PRINTER_TEST_DLG);
	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_PRINT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::~CPrinterTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::PrinterPrint()
{
	try{
		CString templateName= _T("PRINTER_TEST.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("PRINTER_TEST"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			int i = 0;
			while(i < printTexts.GetCount()){
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{PRINT_TIME}"),ComGetCurTime().ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				CStringArray printLine;
				ComPrintLineFormatForTemplate(line,printLine);
				printArr.Append(printLine);
				i++;
			}
		}
		m_pPRTHelper->PrinterPrint(&printArr,false);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}