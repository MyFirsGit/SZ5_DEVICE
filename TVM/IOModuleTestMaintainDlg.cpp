#include "stdafx.h"
#include "IOModuleTestSvc.h"
#include "IOModuleTestMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CIOModuleTestMaintainDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CIOModuleTestMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestMaintainDlg::CIOModuleTestMaintainDlg(CService* pService) : COperationDlg(CIOModuleTestMaintainDlg::IDD,pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestMaintainDlg::~CIOModuleTestMaintainDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CIOModuleTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)) {
		return -1;
	}
	// ����GStatic
	m_baseInfo->detailInfo.row        = _MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_LableGroup;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	Clear();
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// ��ȡ�豸����
	//Update Menu status when Service input [Enter] 
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)GetService();
	if (pSvc->GetCurDialogGroupID() == CIOModuleTestSvc::DIALOG_GROUP_IO_TEST){
		theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_ENTER|KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(2);
		InitMenuUI();
	}
	else if (pSvc->GetCurDialogGroupID() == CIOModuleTestSvc::DIALOG_GROUP_IO_STATUS){
		theAPP_SESSION.ShowOfficeGuide(TXT_MSG_OPERATOR_CONFIRM_IO_STATUS);
		theAPP_SESSION.SetKeyboard(KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(0);
		InitStatusUI();
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ˢ�½���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::UpdateUI()
{
	Clear();
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)GetService();
	if (pSvc->GetCurDialogGroupID() == CIOModuleTestSvc::DIALOG_GROUP_IO_TEST){
		UpdateMenuData();
	}
	if (pSvc->GetCurDialogGroupID() == CIOModuleTestSvc::DIALOG_GROUP_IO_STATUS)	{
		UpdateStatusData();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������˵�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::InitMenuUI()
{
	//���ò˵�ѡ����
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);

	//Set aliagn
	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô�����״̬����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::InitStatusUI()
{
	//���ò˵�ѡ����
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);

	//Set aliagn
	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ò˵�����

@param      none

@retval     none

@exception  none1
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::UpdateMenuData()
{
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)GetService();
	CIOModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	// TVM
	if(DEVICE_TYPE_TVM == DeviceType){
		//m_LableGroup[0].nameLeft = _T("[1]") + _opl(IO_TEST_TEXT_IO_MODULE_STATUS);
		//m_LableGroup[1].nameLeft = _T("[2]") + _opl(IO_TEST_TEXT_OPEN_ALARM);
		//m_LableGroup[2].nameLeft = _T("[3]") + _opl(IO_TEST_TEXT_MAINTAIN_BACK_LIGHT);
		//m_LableGroup[3].nameLeft = _T("[4]") + _opl(IO_TEST_TEXT_CARD_LOCKER);
		//m_LableGroup[4].nameLeft = _T("[5]") + _opl(IO_TEST_TEXT_CARD_SENSER);
		//m_LableGroup[5].nameLeft = _T("[6]") + _opl(IO_TEST_TEXT_COIN_SHUTTER);
		//m_LableGroup[6].nameLeft = _T("[7]") + _opl(IO_TEST_TEXT_PASSENGER_SENSER);
		//m_LableGroup[7].nameLeft = _T("[8]") + _opl(IO_TEST_TEXT_TEST_CHANGE_INDICATOR);
		//m_LableGroup[8].nameLeft = _T("[9]") + _opl(IO_TEST_TEXT_TEST_CHANGE_LIGHT);
		//m_LableGroup[9].nameLeft = _T("[10]") + _opl(IO_TEST_TEXT_OPEN_ALL);
		//m_LableGroup[10].nameLeft = _T("[11]") + _opl(IO_TEST_TEXT_CLOSE_ALL);

		m_LableGroup[0].nameLeft = _T("[1]") + _opl(IO_TEST_TEXT_IO_MODULE_STATUS);
		//m_LableGroup[1].nameLeft = _T("[2]") + /*_opl(IO_TEST_TEXT_OPEN_ALARM)*/CString(_T("ȡƱ����ָʾ��"));
		//m_LableGroup[2].nameLeft = _T("[3]") + /*_opl(IO_TEST_TEXT_MAINTAIN_BACK_LIGHT)*/CString(_T("ȡƱ����������"));
		//m_LableGroup[3].nameLeft = _T("[4]") + /*_opl(IO_TEST_TEXT_CARD_LOCKER)*/CString(_T("��ֵ��ָʾ��"));
		////m_LableGroup[4].nameLeft = _T("[5]") + /*_opl(IO_TEST_TEXT_CARD_SENSER)*/CString(_T("������"));
		//m_LableGroup[4].nameLeft = _T("[5]") + /*_opl(IO_TEST_TEXT_COIN_SHUTTER)*/CString(_T("Ӳ��Ͷ�ҿ�ָʾ��"));
		//m_LableGroup[5].nameLeft = _T("[6]") + /*_opl(IO_TEST_TEXT_PASSENGER_SENSER)*/CString(_T("ƾ����ָʾ��"));
		//m_LableGroup[6].nameLeft = _T("[7]") + /*_opl(IO_TEST_TEXT_TEST_CHANGE_INDICATOR)*/CString(_T("Ӳ�ҵ����"));
		//m_LableGroup[7].nameLeft = _T("[8]") + /*_opl(IO_TEST_TEXT_TEST_CHANGE_LIGHT)*/CString(_T("��ֵ�������"));
		m_LableGroup[1].nameLeft = _T("[2]") + _opl(IO_TEST_TEXT_OPEN_ALL);
		m_LableGroup[2].nameLeft = _T("[3]") + _opl(IO_TEST_TEXT_CLOSE_ALL);

		//�˵�����
		for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu){
			m_LableGroup[nMenu].nameRight = modol.menuInfo[nMenu].strMenuStatus;
			m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1,modol.menuInfo[nMenu].strMenuStatus == FAILED ? RED : SOFTGREEN);
		}
	}
	// AVM
	else if(DEVICE_TYPE_AVM == DeviceType)
	{
		/*m_LableGroup[0].nameLeft = _T("[1]") + _opl(IO_TEST_TEXT_IO_MODULE_STATUS);
		m_LableGroup[1].nameLeft = _T("[2]") + _opl(IO_TEST_TEXT_OPEN_ALARM);
		m_LableGroup[2].nameLeft = _T("[3]") + _opl(IO_TEST_TEXT_CLOSE_ALARM);

		m_LableGroup[0].nameRight = modol.menuInfo[0].strMenuStatus;
		m_baseInfoArea->SetLabelColor(0,_MAX_COL - 1,modol.menuInfo[0].strMenuStatus == FAILED ? RED : SOFTGREEN);

		m_LableGroup[1].nameRight = modol.menuInfo[3].strMenuStatus;
		m_baseInfoArea->SetLabelColor(1,_MAX_COL - 1,modol.menuInfo[3].strMenuStatus == FAILED ? RED : SOFTGREEN);

		m_LableGroup[2].nameRight = modol.menuInfo[4].strMenuStatus;
		m_baseInfoArea->SetLabelColor(2,_MAX_COL - 1,modol.menuInfo[4].strMenuStatus == FAILED ? RED : SOFTGREEN);

		*/
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����IO���status����

@param      none

@retval     none

@exception  none1
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::UpdateStatusData()
{
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)GetService();
	CIOModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	//m_LableGroup[0].nameLeft = _opl(IO_TEST_TEXT_COINBOX1_ATTACH);
	//m_LableGroup[1].nameLeft = _opl(IO_TEST_TEXT_COINBOX2_ATTACH);
	//m_LableGroup[2].nameLeft =  _opl(IO_TEST_TEXT_COINBOX1_LID_LOCKED);
	//m_LableGroup[3].nameLeft =  _opl(IO_TEST_TEXT_COINBOX2_LID_LOCKED);
	//m_LableGroup[4].nameLeft =  _opl(IO_TEST_TEXT_BANKCARD_ACTIVE);
	//m_LableGroup[5].nameLeft =  _opl(IO_TEST_TEXT_BANKNOTE_RECYCLE_ACTIVE);
	//m_LableGroup[6].nameLeft =  _opl(IO_TEST_TEXT_BANKNOTE_RECYCLE_SAFEGATE_ACTIVE);
	//m_LableGroup[7].nameLeft = _opl(IO_TEST_TEXT_BANKNOTE_CHANGE_ACTIVE);
	//m_LableGroup[8].nameLeft = _opl(IO_TEST_TEXT_BANKNOTE_CHANGE_SAFEGATE_ACTIVE);
	//m_LableGroup[9].nameLeft =  _opl(IO_TEST_TEXT_ASSISTANT_BUTTON_PRESSED);
	//m_LableGroup[10].nameLeft =  _opl(IO_TEST_TEXT_MAINTAIN_GATE_LOCKED);
	//m_LableGroup[11].nameLeft =  _opl(IO_TEST_TEXT_PASSENGER_SENSER_ACTIVED);
	//m_LableGroup[12].nameLeft =  _T("����ģ�鵽λ");

	m_LableGroup[0].nameLeft = /*_opl(IO_TEST_TEXT_COINBOX1_ATTACH);*/_opl(IO_TEST_TEXT_FRONT_DOOR_SENSOR);
	m_LableGroup[1].nameLeft = /*_opl(IO_TEST_TEXT_COINBOX2_ATTACH);*/_opl(IO_TEST_TEXT_BACK_DOOR_SENSOR);
	m_LableGroup[2].nameLeft = /* _opl(IO_TEST_TEXT_COINBOX1_LID_LOCKED);*/_opl(IO_TEST_TEXT_PEOPLE_SENSOR);
	m_LableGroup[3].nameLeft =  /*_opl(IO_TEST_TEXT_COINBOX2_LID_LOCKED);*/_opl(IO_TEST_TEXT_BH_IN_PLACE_SENSOR);
	m_LableGroup[4].nameLeft =  /*_opl(IO_TEST_TEXT_BANKCARD_ACTIVE);*/_opl(IO_TEST_TEXT_TOKEN_INVALID_SENSOR);
	m_LableGroup[5].nameLeft = /* _opl(IO_TEST_TEXT_BANKNOTE_RECYCLE_ACTIVE);*/_opl(IO_TEST_TEXT_TOKEN_RECOVERY_SENSOR);
	m_LableGroup[6].nameLeft = /* _opl(IO_TEST_TEXT_BANKNOTE_RECYCLE_SAFEGATE_ACTIVE);*/_opl(IO_TEST_TEXT_BACK_DOOR_TU_SENSOR);
	m_LableGroup[7].nameLeft = /*_opl(IO_TEST_TEXT_BANKNOTE_CHANGE_ACTIVE);*/_opl(IO_TEST_TEXT_BACK_DOOR_BU_SENSOR);

	//�˵�����
	for (int nMenu = 0; nMenu < 8; ++nMenu){
		m_LableGroup[nMenu].nameRight = modol.IO_Status[nMenu].bIsThisActive ? _opl(IO_TEST_TEXT_ON) : _opl(IO_TEST_TEXT_OFF);
		m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1, modol.IO_Status[nMenu].bIsThisActive ? SOFTGREEN : RED);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������ʾ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestMaintainDlg::Clear()
{
	for (int nRow = 0; nRow < _MAX_ROW; ++nRow){
		m_LableGroup[nRow].nameLeft   = UNKNOW;
		m_LableGroup[nRow].nameRight  = UNKNOW;
		for(int nCol = 0; nCol < _MAX_COL; ++nCol){
			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
		}		
	} 
}
