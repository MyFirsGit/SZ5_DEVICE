#include "stdafx.h"
#include "CardReadWriteMachineTestMaintainDlg.h"
#include "RWTestSvc.h"
#include "GuideInfo.h"
#include "RWMonitorSvc.h"
#include "TWMonitorSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCardReadWriteMachineTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CCardReadWriteMachineTestMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRWTestBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnRWDetailTestBtnClick)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

static const CString STR_RW_TYPE[] = {_T("�ⲿ��д��"),_T("�ڲ���д��")};
static const CString STR_OUTER_RW_DETAIL_TYPE[] = {_T("��ȡTPU��Ϣ"),_T("��ȡʱ��"),_T("��ȡ�����汾"),_T("��������"),_T("���ò���")};
static const CString STR_INNER_RW_DETAIL_TYPE[] = {_T("��ȡTPU��Ϣ"),_T("��ȡʱ��"),_T("��ȡ�����汾"),_T("��������"),_T("���ò���")};

static const CString STR_RW_TYPE_MSG[] = {_T("�ⲿ��д��"),_T("�ڲ���д��")};
static const CString STR_OUTER_RW_DETAIL_TYPE_MSG[] = {_T("��ȡTPU��Ϣ"),_T("��ȡʱ��"),_T("��ȡ�����汾"),_T("��������"),_T("���ò���")};
static const CString STR_INNER_RW_DETAIL_TYPE_MSG[] = {_T("��ȡTPU��Ϣ"),_T("��ȡʱ��"),_T("��ȡ�����汾"),_T("��������"),_T("���ò���")};

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardReadWriteMachineTestMaintainDlg::CCardReadWriteMachineTestMaintainDlg(CService* pService)
: COperationDlg(CCardReadWriteMachineTestMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TXT_RW_TEST_SVC_CN;					// ����
	m_baseInfo->detailInfo.row = 2;											// ����
	m_baseInfo->detailInfo.labelGroup = m_CardReadWriteInfo;				// ��ǩ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardReadWriteMachineTestMaintainDlg::~CCardReadWriteMachineTestMaintainDlg()
{
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
int CCardReadWriteMachineTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ի����ʼ��

@param      ��

@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCardReadWriteMachineTestMaintainDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// ��ʾBOM���ԣ���ѡ��RW������Ŀ
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_CHOOSE_ITEM));
	
	//��ʼ��lable��Ϣ
	m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("��д�����ͣ�"),MAX_SBC_CASE_LEN);
	m_CardReadWriteInfo[1].nameLeft = add_wsp(_T("������Ŀ��"),MAX_SBC_CASE_LEN);
	
	// ��ʼ����ť
	InitialButtons();

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitialButtons()
{
	m_vec_Right_1_BTN_INFO.clear();
	// ���õ�ѡ
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	for (int i = 0; i < 2; i++){
		BTN_INFO pRWTypeInfo;
		pRWTypeInfo.btnID = i + 1;
		pRWTypeInfo.btnMsg = STR_RW_TYPE_MSG[i];
		pRWTypeInfo.text = STR_RW_TYPE[i];
		m_vec_Right_1_BTN_INFO.push_back(pRWTypeInfo);
	}
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickFirstAvailButton();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϡ����� ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n		TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetForeServiceBusy(true);

	CRWTestSvc* pRWTestSvc = (CRWTestSvc*)GetService();
	try{
		m_CardReadWriteInfo[0].nameLeft = _T("");
		m_CardReadWriteInfo[0].nameRight = _T("");
		m_CardReadWriteInfo[1].nameLeft = _T("");
		m_CardReadWriteInfo[1].nameRight = _T("");

		if(m_RWDetailType == 11 || m_RWDetailType == 21){
			m_baseInfo->titleInfo.titleName = _T("TPU��Ϣ");
			m_baseInfo->detailInfo.row = 8;
			m_CardReadWriteInfo[0].nameLeft = add_hsp(_T("TPU�豸��ţ�"),20);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[1].nameLeft = add_hsp(_T("Ӧ������汾��"),20);
			m_CardReadWriteInfo[1].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[2].nameLeft = add_hsp(_T("��������汾��"),20);
			m_CardReadWriteInfo[2].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[3].nameLeft = add_hsp(_T("Ӳ���汾��"),20);
			m_CardReadWriteInfo[3].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[4].nameLeft = add_hsp(_T("BMAC PSAM��ţ�"),20);
			m_CardReadWriteInfo[4].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[5].nameLeft = add_hsp(_T("BMAC ISAM��ţ�"),20);
			m_CardReadWriteInfo[5].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[6].nameLeft = add_hsp(_T("ACC PSAM��ţ�"),20);
			m_CardReadWriteInfo[6].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[7].nameLeft = add_hsp(_T("ACC ISAM��ţ�"),20);
			m_CardReadWriteInfo[7].nameRight = TXT_UNDEFINE;
			RW_RSP_TPU_INFO tpuInfo;
			long errorCode = -1;
			if(m_RWDetailType == 11){
				errorCode = pRWTestSvc->GetRWHelper()->GetTPUInfo(tpuInfo);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetTPUInfo(tpuInfo);
			}
			if(errorCode == 0){
				CString tmp;
				CHAR pDataMsgNo[28+2]={0x00};
				PSTR2BCD((char*)tpuInfo.DeviceCode,4,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.DeviceCode,4));
				tmp.Format(_T("    0x%s"),pDataMsgNo);

				m_CardReadWriteInfo[0].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.softwareVersionApp[1],tpuInfo.softwareVersionApp[0]));
				m_CardReadWriteInfo[1].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.softwareVersionMain[1],tpuInfo.softwareVersionMain[0]));
				m_CardReadWriteInfo[2].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.hardwareVersion[1],tpuInfo.hardwareVersion[0]));
				m_CardReadWriteInfo[3].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.bmacPSAMid,6,pDataMsgNo);	
				//tmp.Format(_T("    0x%s"),changeBuff2String((char*)tpuInfo.bmacPSAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[4].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.bmacISAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.bmacISAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[5].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.accPSAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.accPSAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[6].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.accISAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.accISAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[7].nameRight = tmp;

				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 12 || m_RWDetailType == 22){
			m_baseInfo->titleInfo.titleName = _T("ʱ����Ϣ");
			m_baseInfo->detailInfo.row = 1;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("����ʱ�䣺"),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			_DATE_TIME timeInfo;
			long errorCode = -1;
			if(m_RWDetailType == 12){
				errorCode = pRWTestSvc->GetRWHelper()->GetTime(timeInfo);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetTime(timeInfo);
			}
			if(errorCode == 0){
				//m_CardReadWriteInfo[0].nameRight = timeInfo.ToString("%.4d/%.2d/%.2d %.2d:%.2d:%.2d");
				_DATE_TIME curTime = ComGetCurTime();
				m_CardReadWriteInfo[0].nameRight = curTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 13 || m_RWDetailType == 23){
			m_baseInfo->titleInfo.titleName = _T("�����汾��Ϣ");
			m_baseInfo->detailInfo.row = 10;
			CString strLeft;
			CString strRight;
			strLeft.Format(_T("��Ӫ��--"));
			strRight.Format(_T("       �������������ѣ�--"));
			m_CardReadWriteInfo[0].nameLeft = strLeft;
			m_CardReadWriteInfo[0].nameRight = strRight;

			strLeft.Format(_T("��ʱ�������ã�--"));
			strRight.Format(_T("       �豸���ƣ�--"));
			m_CardReadWriteInfo[1].nameLeft = strLeft;
			m_CardReadWriteInfo[1].nameRight = strRight;

			strLeft.Format(_T("���Σ�--"));
			strRight.Format(_T("       ��վλ�ã�--"));
			m_CardReadWriteInfo[2].nameLeft = strLeft;
			m_CardReadWriteInfo[2].nameRight = strRight;

			strLeft.Format(_T("ʱ�両����--"));
			strRight.Format(_T("       �Ʒ�վ�㣺--"));
			m_CardReadWriteInfo[3].nameLeft = strLeft;
			m_CardReadWriteInfo[3].nameRight = strRight;

			strLeft.Format(_T("ʱ�����ͱ�--"));
			strRight.Format(_T("       Ʊ�����ԣ�--"));
			m_CardReadWriteInfo[4].nameLeft = strLeft;
			m_CardReadWriteInfo[4].nameRight = strRight;

			strLeft.Format(_T("�ۿ��ʱ�--"));
			strRight.Format(_T("       ����Ʊ�۱�--"));
			m_CardReadWriteInfo[5].nameLeft = strLeft;
			m_CardReadWriteInfo[5].nameRight = strRight;

			strLeft.Format(_T("������������--"));
			strRight.Format(_T("       Ʊ�ۼ����--"));
			m_CardReadWriteInfo[6].nameLeft = strLeft;
			m_CardReadWriteInfo[6].nameRight = strRight;

			strLeft.Format(_T("���κ�������--"));
			strRight.Format(_T("       Ʊ�۵������Ա�--"));
			m_CardReadWriteInfo[7].nameLeft = strLeft;
			m_CardReadWriteInfo[7].nameRight = strRight;

			strLeft.Format(_T("��������--"));
			strRight.Format(_T("       ʱ��������ϸ��--"));
			m_CardReadWriteInfo[8].nameLeft = strLeft;
			m_CardReadWriteInfo[8].nameRight = strRight;

			strLeft.Format(_T("���д�����գ�--"));
			strRight.Format(_T("       Ʊ�������̣�--"));
			m_CardReadWriteInfo[9].nameLeft = strLeft;
			m_CardReadWriteInfo[9].nameRight = strRight;

			VERSION_ACC accVersion;
			VERSION_ECT ectVersion;
			VERSION_AFC afcVersion;
			long errorCode = -1;
			if(m_RWDetailType == 13){
				errorCode = pRWTestSvc->GetRWHelper()->GetParameterInfo(accVersion,ectVersion,afcVersion);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetParameterInfo(accVersion,ectVersion,afcVersion);
			}
			if(errorCode == 0){
				strLeft.Format(_T("��Ӫ��%d"),accVersion.lBusinessParam);
				strRight.Format(_T("       �������������ѣ�%d"),accVersion.lServerFeeParam);
				m_CardReadWriteInfo[0].nameLeft = strLeft;
				m_CardReadWriteInfo[0].nameRight = strRight;

				strLeft.Format(_T("��ʱ�������ã�%d"),accVersion.lOverTimeAdjuestParam);
				strRight.Format(_T("       �豸���ƣ�%d"),accVersion.lDeviceControlParam);
				m_CardReadWriteInfo[1].nameLeft = strLeft;
				m_CardReadWriteInfo[1].nameRight = strRight;

				strLeft.Format(_T("���β�����%d"),accVersion.lSectionParam);
				strRight.Format(_T("       ��վλ�ò�����%d"),accVersion.lStationParam);
				m_CardReadWriteInfo[2].nameLeft = strLeft;
				m_CardReadWriteInfo[2].nameRight = strRight;

				strLeft.Format(_T("ʱ�両����%d"),accVersion.lTimeFloatParam);
				strRight.Format(_T("       �Ʒ�վ�㣺%d"),accVersion.lPayStationParam);
				m_CardReadWriteInfo[3].nameLeft = strLeft;
				m_CardReadWriteInfo[3].nameRight = strRight;

				strLeft.Format(_T("ʱ�����ͱ�%d"),accVersion.lTimeTypeParam);
				strRight.Format(_T("       Ʊ�����ԣ�%d"),accVersion.lCardAttributeParam);
				m_CardReadWriteInfo[4].nameLeft = strLeft;
				m_CardReadWriteInfo[4].nameRight = strRight;

				strLeft.Format(_T("�ۿ��ʱ�%d"),accVersion.lDiscountParam);
				strRight.Format(_T("       ����Ʊ�۱�%d"),accVersion.lBasePriceParam);
				m_CardReadWriteInfo[5].nameLeft = strLeft;
				m_CardReadWriteInfo[5].nameRight = strRight;

				strLeft.Format(_T("������������%d"),accVersion.lSingleBlackListParam);
				strRight.Format(_T("       Ʊ�ۼ����%d"),accVersion.lPrieLevelParam);
				m_CardReadWriteInfo[6].nameLeft = strLeft;
				m_CardReadWriteInfo[6].nameRight = strRight;

				strLeft.Format(_T("���κ�������%d"),accVersion.lSectionBlackListParam);
				strRight.Format(_T("       Ʊ�۵������Ա�%d"),accVersion.lPriceAdjuestParam);
				m_CardReadWriteInfo[7].nameLeft = strLeft;
				m_CardReadWriteInfo[7].nameRight = strRight;

				strLeft.Format(_T("��������%d"),accVersion.lWriteListParam);
				strRight.Format(_T("       ʱ��������ϸ��%d"),accVersion.lTimeDetailParam);
				m_CardReadWriteInfo[8].nameLeft = strLeft;
				m_CardReadWriteInfo[8].nameRight = strRight;

				strLeft.Format(_T("���д�����գ�%d"),accVersion.lOtherCityCardParam);
				strRight.Format(_T("       Ʊ�������̣�%d"),accVersion.lissueCompanyParam);
				m_CardReadWriteInfo[9].nameLeft = strLeft;
				m_CardReadWriteInfo[9].nameRight = strRight;

				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 14 || m_RWDetailType == 24){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_PUT_CARD));
			m_baseInfo->titleInfo.titleName = _T("������Ϣ");
			m_baseInfo->detailInfo.row = 2;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("Ʊ�濨�ţ�"),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[1].nameLeft = add_wsp(_T("��Ʊ���ࣺ"),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[1].nameRight = TXT_UNDEFINE;
			if(m_RWDetailType == 14){
				pRWTestSvc->GetRWHelper()->ReadCard();
				INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pRWTestSvc->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				if(ret == IDCANCEL){
					pRWTestSvc->GetRWHelper()->StopReadCard();
				}
			}
			else{
				pRWTestSvc->GetTWHelper()->ReadCard();
				INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pRWTestSvc->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				if(ret == IDCANCEL){
					pRWTestSvc->GetTWHelper()->StopReadCard();
				}
			}
		}
		else if(m_RWDetailType == 15 || m_RWDetailType == 25){
			m_baseInfo->titleInfo.titleName = _T("���ò�����Ϣ");
			m_baseInfo->detailInfo.row = 1;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("����״̬��"),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			bool result = false;
			if(m_RWDetailType == 15){
				// ���RW�쳣����
				theEXCEPTION_MGR.RemoveException(CRWHardwareException::MODULE_ID);
				theEXCEPTION_MGR.RemoveException(CRWException::MODULE_ID);
				// �޸��ⲿ������
				result = pRWTestSvc->GetRWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(true)){
					theSERVICE_MGR.GetService<CRWMonitorSvc>(MONITOR_RW_SVC)->Resume();
				}					
			}
			else{
				// ���TW�쳣����
				theEXCEPTION_MGR.RemoveException(CTWHardwareException::MODULE_ID);
				theEXCEPTION_MGR.RemoveException(CTWException::MODULE_ID);
				// �޸��ڲ�������
				result = pRWTestSvc->GetTWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(false)){
					theSERVICE_MGR.GetService<CTWMonitorSvc>(MONITOR_TW_SVC)->Resume();
				}
			}
			if(result){
				m_CardReadWriteInfo[0].nameRight = "�ɹ�";
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				m_CardReadWriteInfo[0].nameRight = "ʧ��";
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		CInnerException e = CInnerException(pRWTestSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	// ���ر༭���ȷ�ϰ�ť
	HideCompoment(BTN_OK);
	// ˢ�´���
	ShowCompoment(BASE_AREA);
	// �������ఴť
	m_vec_Right_1_BTN_INFO.clear();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_vec_Right_2_BTN_INFO.clear();
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);

	theSERVICE_MGR.SetForeServiceBusy(false);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��д�����԰�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnRWTestBtnClick(WPARAM wParam,LPARAM lParam)
{
	try	{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;

		m_CardReadWriteInfo[0].nameRight = btnInfo->btnMsg;

		m_RWType = btnInfo->btnID;
		m_vec_Right_2_BTN_INFO.clear();
		// ���õ�ѡ
		m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
		switch(m_RWType)
		{
			// �ⲿ��д��
		case 1:
			for (int i = 0; i < 5; i++){
				BTN_INFO pRWDetailTypeBtnInfo;
				pRWDetailTypeBtnInfo.btnID = i + 11;
				pRWDetailTypeBtnInfo.btnMsg = STR_OUTER_RW_DETAIL_TYPE_MSG[i];
				pRWDetailTypeBtnInfo.text = STR_OUTER_RW_DETAIL_TYPE[i];
				m_vec_Right_2_BTN_INFO.push_back(pRWDetailTypeBtnInfo);
			}
			break;
			// �ڲ���д��
		case 2:
			for (int i = 0; i < 5; i++){
				BTN_INFO pRWDetailTypeBtnInfo;
				pRWDetailTypeBtnInfo.btnID = i + 21;
				pRWDetailTypeBtnInfo.btnMsg = STR_INNER_RW_DETAIL_TYPE_MSG[i];
				pRWDetailTypeBtnInfo.text = STR_INNER_RW_DETAIL_TYPE[i];
				m_vec_Right_2_BTN_INFO.push_back(pRWDetailTypeBtnInfo);
			}
			break;
		default:
			break;
		}
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_2_Group->ClickFirstAvailButton();

		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��д����ϸ���԰�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnRWDetailTestBtnClick(WPARAM wParam,LPARAM lParam)
{
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;

		m_CardReadWriteInfo[1].nameRight = btnInfo->btnMsg;

		m_RWDetailType = btnInfo->btnID;

		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

