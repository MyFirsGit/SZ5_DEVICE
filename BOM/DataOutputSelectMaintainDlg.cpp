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

static const char* const DATATYPE_BUSINESS_BUTTON_NAME			= "ҵ������";
static const char* const DATATYPE_BUSINESS_BUTTON_MSG			= "DATATYPE_BUSINESS";
static const char* const DATATYPE_TRANSACTION_BUTTON_NAME		= "��������";
static const char* const DATATYPE_TRANSACTION_BUTTON_MSG		= "DATATYPE_TRANSATION";
static const char* const DATATYPE_LOG_BUTTON_NAME				= "��־����";
static const char* const DATATYPE_LOG_BUTTON_MSG				= "DATATYPE_LOG";

IMPLEMENT_DYNAMIC(CDataOutputSelectMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDataOutputSelectMaintainDlg,COperationDlg)
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
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::~CDataOutputSelectMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      OnCreate

@param      (i)LPCREATESTRUCT lpCreateStruct ���崴��������

@retval     int		\n
0 �ɹ�����0 ʧ��

@exception  ��
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
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataOutputSelectMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CDataOutputSvc* m_DataOutputSvc = (CDataOutputSvc*)GetService();

	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// �ж��Ƿ��ѻ�״̬
		if (!theAPP_SESSION.IsSCConnected()){
			// �ⲿý����֤
			if(theBOM_INFO.GetUSBAuthFlag() == 1){
				if(!m_DataOutputSvc->AuthUSB()){
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
					theSERVICE_MGR.SetForeServiceBusy(false);
					return 0;
				}
			}

			// ��ȡѡ���������ͣ����洢��vector��
			const vector<BTN_INFO>& selectBtnInfos= m_Right_1_Group->GetSelectedBtnInfo();

			// ���û��ѡ���κ��������ѡ���������
			if (0 == selectBtnInfos.size()){
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_SELECT));
				theSERVICE_MGR.SetForeServiceBusy(false);
				return 0;	

			}

			// ���ѡ�����������ѡ���������Ŀ¼
			CString sUsbDrive = theBOM_INFO.GetBOMUSBDrive();
			CString sDesPath = sUsbDrive + theBOM_INFO.GetBomExportPath() + m_DataOutputSvc->m_DeviceID + _T("\\");
			if (!ComCheckAndCreateDirectory(sDesPath)) { 		
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
				HideCompoment(BTN_OK);
				theSERVICE_MGR.SetForeServiceBusy(false);
				return 0;	
			}

			// ���ѡ����������ѡ��ĸ���
			size_t selectButtonsNumber = selectBtnInfos.size();
			// ˳�ε���ѡ���������
			for (size_t i = 0;i < selectButtonsNumber;i++){
				// ��ȡ��������
				CString strExportType = selectBtnInfos.at(i).btnMsg;
				// ����ҵ������
				if (DATATYPE_BUSINESS_BUTTON_MSG == strExportType){
					m_DataOutputSvc->ExportParameter(BUS_DATA);
				}
				// ������������
				else if (DATATYPE_TRANSACTION_BUTTON_MSG == strExportType){
					m_DataOutputSvc->ExportParameter(ACC_TRANS);
					m_DataOutputSvc->ExportParameter(AFC_TRANS);
					m_DataOutputSvc->ExportParameter(ECT_TRANS);
				}
				// ������־����
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
			CMessageBox::Show(_T("��ʾ"),_T("����״̬������������ݵ���"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
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