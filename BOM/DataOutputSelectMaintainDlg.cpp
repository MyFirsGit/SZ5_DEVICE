#include "stdafx.h"
#include "DataOutputSelectMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "DataOutputSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char* const DATATYPE_BUSINESS_BUTTON_NAME			= "业务数据";
static const char* const DATATYPE_BUSINESS_BUTTON_MSG			= "DATATYPE_BUSINESS";
static const char* const DATATYPE_TRANSACTION_BUTTON_NAME		= "交易数据";
static const char* const DATATYPE_TRANSACTION_BUTTON_MSG		= "DATATYPE_TRANSATION";
static const char* const DATATYPE_LOG_BUTTON_NAME				= "日志数据";
static const char* const DATATYPE_LOG_BUTTON_MSG				= "DATATYPE_LOG";

IMPLEMENT_DYNAMIC(CDataOutputSelectMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDataOutputSelectMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::CDataOutputSelectMaintainDlg(CService* pService) :
COperationDlg(CDataOutputSelectMaintainDlg::IDD, pService)
{
	BTN_INFO bussinessDataInfo;
	bussinessDataInfo.btnMsg = DATATYPE_BUSINESS_BUTTON_MSG;
	bussinessDataInfo.text = DATATYPE_BUSINESS_BUTTON_NAME;
	m_vec_Right_1_BTN_INFO.push_back(bussinessDataInfo);

	BTN_INFO transactionDataInfo;
	transactionDataInfo.btnMsg = DATATYPE_TRANSACTION_BUTTON_MSG;
	transactionDataInfo.text = DATATYPE_TRANSACTION_BUTTON_NAME;
	m_vec_Right_1_BTN_INFO.push_back(transactionDataInfo);

	BTN_INFO logDataInfo;
	logDataInfo.btnMsg = DATATYPE_LOG_BUTTON_MSG;
	logDataInfo.text = DATATYPE_LOG_BUTTON_NAME;
	m_vec_Right_1_BTN_INFO.push_back(logDataInfo);

	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->SetSelectType(CButtonGroup::MULTIPLE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::~CDataOutputSelectMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      OnCreate

@param      (i)LPCREATESTRUCT lpCreateStruct 窗体创建的区域

@retval     int		\n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CDataOutputSelectMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK)){
		return -1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataOutputSelectMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CDataOutputSvc* m_DataOutputSvc = (CDataOutputSvc*)GetService();

	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 判断是否脱机状态
		if (!theAPP_SESSION.IsSCConnected()){
			// 外部媒体认证
			if(theBOM_INFO.GetUSBAuthFlag() == 1){
				if(!m_DataOutputSvc->AuthUSB()){
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
					theSERVICE_MGR.SetForeServiceBusy(false);
					return 0;
				}
			}

			// 获取选择的输出类型，并存储到vector中
			const vector<BTN_INFO>& selectBtnInfos= m_Right_1_Group->GetSelectedBtnInfo();

			// 如果没有选择任何输出类型选项，结束导出
			if (0 == selectBtnInfos.size()){
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_SELECT));
				theSERVICE_MGR.SetForeServiceBusy(false);
				return 0;	

			}

			// 如果选择了输出类型选项，创建导出目录
			CString sUsbDrive = theBOM_INFO.GetBOMUSBDrive();
			CString sDesPath = sUsbDrive + theBOM_INFO.GetBomExportPath() + m_DataOutputSvc->m_DeviceID + _T("\\");
			if (!ComCheckAndCreateDirectory(sDesPath)) { 		
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
				HideCompoment(BTN_OK);
				theSERVICE_MGR.SetForeServiceBusy(false);
				return 0;	
			}

			// 获得选择的输出类型选项的个数
			size_t selectButtonsNumber = selectBtnInfos.size();
			// 顺次导出选择的数据项
			for (size_t i = 0;i < selectButtonsNumber;i++){
				// 获取导出类型
				CString strExportType = selectBtnInfos.at(i).btnMsg;
				// 导出业务数据
				if (DATATYPE_BUSINESS_BUTTON_MSG == strExportType){
					m_DataOutputSvc->ExportParameter(BUS_DATA);
				}
				// 导出交易数据
				else if (DATATYPE_TRANSACTION_BUTTON_MSG == strExportType){
					m_DataOutputSvc->ExportParameter(ACC_TRANS);
					m_DataOutputSvc->ExportParameter(AFC_TRANS);
					m_DataOutputSvc->ExportParameter(ECT_TRANS);
				}
				// 导出日志数据
				else if (DATATYPE_LOG_BUTTON_MSG == strExportType){
					m_DataOutputSvc->ExportParameter(LOG_DATA);
				}
			}
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_FINISH));
			HideCompoment(BTN_OK);
			m_vec_Right_1_BTN_INFO.clear();
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		}
		else{
			CMessageBox::Show(_T("提示"),_T("联机状态不允许进行数据导出"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(m_DataOutputSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}