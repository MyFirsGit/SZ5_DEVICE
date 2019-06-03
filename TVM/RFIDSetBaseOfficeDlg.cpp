#include "stdafx.h"
#include "RFIDSetBaseOfficeDlg.h"
#include "RFIDSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ʼ�������ַ���
static const TCHAR* const STR_READONLY_MODE   =  _T("����ģʽ");
static const TCHAR* const STR_WRITABLE_MODE   =  _T("ʹ��ģʽ");

static const TCHAR* const TITLE_SELECT_RFID_MODE = _T("��ѡ��RFID��дģʽ");

static const TCHAR* const TXT_CURRENT_MODE = _T("��ǰģʽ��");
static const TCHAR* const TXT_SWITCH_MODE  = _T("ѡ��ģʽ��");

static const TCHAR* const WM_BTN_READONLY_MODE = _T("ReadOnly");
static const TCHAR* const WM_BTN_WRITABLE_MODE  = _T("Writable");


IMPLEMENT_DYNAMIC(CRFIDSetBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRFIDSetBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnModeItemClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*     pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRFIDSetBaseOfficeDlg::CRFIDSetBaseOfficeDlg(CService* pService)
: COperationDlg(CRFIDSetBaseOfficeDlg::IDD, pService)
{
	//m_baseInfo->titleInfo.titleName = TITLE_SELECT_RFID_MODE;			// ����
	//m_baseInfo->detailInfo.row = 2;									// ����
	//m_baseInfo->detailInfo.labelGroup = m_lableRFIDModeInfo;          // ��ǩ���֣�2�У�

	//// ��ʼ����ͨģʽ�������
	//BTN_INFO readOnlyMode;
	//readOnlyMode.btnMsg = WM_BTN_READONLY_MODE;
	//readOnlyMode.isVisible = true;
	//readOnlyMode.text= STR_READONLY_MODE;
	//readOnlyMode.type = BUTTON_TYPE_NORMAL;
	//m_vec_Right_1_BTN_INFO.push_back(readOnlyMode);

	//// ��ʼ������ģʽ�������
	//BTN_INFO writableMode;
	//writableMode.btnMsg = WM_BTN_WRITABLE_MODE;
	//writableMode.isVisible = true;
	//writableMode.text = STR_WRITABLE_MODE;
	//readOnlyMode.type = BUTTON_TYPE_NORMAL;
	//m_vec_Right_1_BTN_INFO.push_back(writableMode);

	//m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRFIDSetBaseOfficeDlg::~CRFIDSetBaseOfficeDlg()
{
	//m_Right_1_Group->ShowButtons(true);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CRFIDSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try{
	//	// ����������Ϣ���򡢷��ء�ȷ����ť�͹��ܰ�ť����
	//	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA | BTN_OK )){
	//		return -1;
	//	}

	//	// ��ʼ��ģʽ��Ϣ
	//	InitRFIDModeInfo();
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť���º�Ĵ���

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRFIDSetBaseOfficeDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	//CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	//try{
	//	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	//	// ��ȡ��ǰRFIDģʽ
	//	int sRFIDMode = theSETTING.GetRFIDWriteStatus();

	//	// ��ʾ������
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));

	//	if (btnInfo->btnMsg == WM_BTN_READONLY_MODE){	// ����ģʽ��ť����
	//		m_lableRFIDModeInfo[1].nameRight = STR_READONLY_MODE;

	//		if (0 == sRFIDMode){	// �����ǰģʽΪ����ģʽ
	//			m_btnOK->SetEnabled(false);
	//		}
	//		else{	// �����ǰģʽΪʹ��ģʽ				
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	else if (btnInfo->btnMsg == WM_BTN_WRITABLE_MODE){	// ʹ��ģʽ��ť����
	//		m_lableRFIDModeInfo[1].nameRight = STR_WRITABLE_MODE;

	//		if (1 == sRFIDMode){	// �����ǰģʽΪʹ��ģʽ
	//			m_btnOK->SetEnabled(false);
	//		}
	//		else{	// �����ǰģʽΪ����ģʽ
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	//��ʾ�����ͻ�����
	//	ShowCompoment(BASE_AREA);
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRFIDSetBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	//try{
	//	// ��ȡ��ǰѡ���ģʽ��ť
	//	const vector<BTN_INFO>& selectBtnInfos = m_Right_1_Group->GetSelectedBtnInfo();
	//	// ����ɹ���ȡ��ģʽ
	//	if(selectBtnInfos.size() == 1){
	//		const BTN_INFO& selectBtnInfo = selectBtnInfos.at(0);
	//		// �жϻ�ȡ����RFIDģʽ
	//		int sTHMode = selectBtnInfo.btnMsg == WM_BTN_READONLY_MODE ? 0 : 1;
	//		// ���õ�ǰģʽ
	//		pRFIDModeSetSvc->SetRFIDMode(sTHMode);
	//		// ���»�ȡ��ǰģʽ
	//		int newMode = pRFIDModeSetSvc->GetCurrentRFIDMode();
	//		// ��ʾ��ǰģʽ
	//		if (0 == newMode){
	//			m_lableRFIDModeInfo[0].nameRight= STR_READONLY_MODE;
	//		}
	//		else{
	//			m_lableRFIDModeInfo[0].nameRight= STR_WRITABLE_MODE;
	//		}

	//		// ��ʾ���óɹ������ص���ť
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_FINISH));
	//		HideCompoment(BTN_OK);
	//		m_Right_1_Group->ShowButtons(false);

	//		// ˢ������
	//		m_baseInfoArea->Invalidate();
	//	}
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      InitTHModeInfo

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CRFIDSetBaseOfficeDlg::InitRFIDModeInfo()
{
	CRFIDModeSetSvc* pRFIDModeSetSvc = (CRFIDModeSetSvc*)GetService();
	try{	
		// ��ʼ��ģʽ�л���Ϣ
		m_lableRFIDModeInfo[0].nameLeft = add_hsp(TXT_CURRENT_MODE,MAX_HALF_CASE_LEFT_LEN);
		m_lableRFIDModeInfo[1].nameLeft = add_hsp(TXT_SWITCH_MODE,MAX_HALF_CASE_LEFT_LEN);

		// ���ݵ�ǰ��RFIDģʽ�����û�����Ϣ
		int sRFIDMode = pRFIDModeSetSvc->GetCurrentRFIDMode();
		// ���sRFIDMode����0������Ϊ����ģʽ������1������Ϊʹ��ģʽ
		if (0 == sRFIDMode){
			m_lableRFIDModeInfo[0].nameRight = STR_READONLY_MODE;
		}
		else{
			m_lableRFIDModeInfo[0].nameRight = STR_WRITABLE_MODE;
		}
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pRFIDModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}