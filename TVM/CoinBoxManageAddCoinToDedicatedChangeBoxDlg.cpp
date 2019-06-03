#include "stdafx.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxDlg.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxSvc.h"
#include "AuditHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg, COperationDlg)
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
CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::CCoinBoxManageAddCoinToDedicatedChangeBoxDlg(CService* pService)
	: COperationDlg(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::IDD,pService)
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
CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::~CCoinBoxManageAddCoinToDedicatedChangeBoxDlg()
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
int CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	// ����GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);					// ����ָ������
	//theAPP_SESSION.SetMaxInputLength(1);											// �����������λ��



	//InitDedicatedChangeBoxInfo();

	return 0;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��

@param      (i)CWnd* pWndOther  ��ʹ��

@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);						// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);											// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 190);									// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 50);									// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 85);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 50);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 75);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 90);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 80);									// ���õ����п��

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

	InitDedicatedChangeBoxInfo();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::InitDedicatedChangeBoxInfo()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IsHasException)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
		//return;
	}

	for(int i = 0; i < 15; i++)
	{
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}

	m_baseInfoArea->SetLableWidth(6,LABEL_COL_1,RESOLUTION * 300);
	m_baseInfoArea->SetLableWidth(7,LABEL_COL_1,RESOLUTION * 300);

	m_lableGroup[0].name1 = _T("Ӳ��ѭ��������A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// Ӳ��ר��������A
	//m_lableGroup[1].name1 = _opl(TXT_COINMGR_ADDCOIN_BOX);							// Ӳ�Ҳ�����
	m_lableGroup[3].name1 = _T("Ӳ��ѭ��������B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// Ӳ��ר��������B
	//m_lableGroup[4].name1 = _opl(TXT_COINMGR_ADDCOIN_BOX);							// Ӳ�Ҳ�����
	m_lableGroup[6].name1 = _T("[1] ")+_opl(TXT_COINMGR_ADDCOIN_MENU_BOXA);			// ����Ӳ����A������
	m_lableGroup[7].name1 = _T("[2] ")+_opl(TXT_COINMGR_ADDCOIN_MENU_BOXB);			// ����Ӳ����B������

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	//m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

	m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
	m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

	m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
	m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

	m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// ���
	m_lableGroup[4].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// ���
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::UpdateUI()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowDedicatedChangeBoxData();
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);
		if (pSvc->IsShowTips())	// ��ʾ�ӱ���ʾ
		{
			ShowTips();
			theAPP_SESSION.SetKeyboard( KEY_F2 | KEY_RETURN);
		}
	}
	/*else if(2 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowAddingCoinToDedicatedChangeBox();
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER | KEY_CLEAR | KEY_ESC | KEY_F2);
	}*/
	else if(3 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowFinishAddingCoinToDedicatedChangeBox();
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	}

	
	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾר���������ʼ��ʱ��ID������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowDedicatedChangeBoxData()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxAinfo = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_A];
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxBinfo = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_B];

	m_lableGroup[0].name3 = boxAinfo.strID; /*model.dedicatedchangeboxdata.DedicatedChangeCoinAID;*/	// Ӳ��ר��������AIDData
	//m_lableGroup[1].name3 = model.addingboxdata.AddingBoxAID;						// Ӳ�Ҳ�����AIDData
	m_lableGroup[3].name3 = boxBinfo.strID;/*model.dedicatedchangeboxdata.DedicatedChangeCoinBID;*/	// Ӳ��ר��������BIDData
	//m_lableGroup[4].name3 = model.addingboxdata.AddingBoxBID;						// Ӳ�Ҳ�����BIDData

	
	m_lableGroup[0].name5 = CashValueToString(boxAinfo.value);						// Ӳ�Ҳ�����A ��ֵ 
	m_lableGroup[3].name5 = CashValueToString(boxBinfo.value);						// Ӳ�Ҳ�����B ��ֵ 

	m_lableGroup[0].name7.Format(_T("%d"),boxAinfo.CurrentCount);					// Ӳ��ר��������A ���� 
	m_lableGroup[3].name7.Format(_T("%d"),boxBinfo.CurrentCount);					// Ӳ��ר��������B ���� 

	m_lableGroup[1].name7.Format(_T("%d"),boxAinfo.AddingCount);					// Ӳ�Ҳ�����A �ӱ�����
	m_lableGroup[4].name7.Format(_T("%d"),boxBinfo.AddingCount);					// Ӳ�Ҳ�����B �ӱ�����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾר��������ӱ��н���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowAddingCoinToDedicatedChangeBox()
{
	m_baseInfoArea->SetLableWidth(LABEL_COL_1,640);									// ���õ�һ�п��
	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_DOING));
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDCOIN_1STEP);
	m_lableGroup[1].name1 = _opl(TXT_COINMGR_ADDCOIN_2STEP);
	m_lableGroup[2].name1 = _opl(TXT_COINMGR_ADDCOIN_3STEP);
	m_lableGroup[3].name1 = _opl(TXT_COINMGR_ADDCOIN_4STEP);

	m_lableGroup[4].name1 = _T("");
	m_lableGroup[6].name1 = _T("");						
	m_lableGroup[7].name1 = _T("");		
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾר��������ӱ���ɽ���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowFinishAddingCoinToDedicatedChangeBox()
{
	m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);									// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);									// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,90);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,100);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,90);									// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,80);									// ���õ����п��

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxA = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_A];
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxB = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_B];

	for ( int i = 0; i < 15; i++ ){
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
	

	//model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_FINISH));

	// name1
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDING_BEFORE_ADDING);					// �ӱ�ǰ
	m_lableGroup[1].name1 = _T("Ӳ��ѭ��������A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// Ӳ��ר��������A
	m_lableGroup[4].name1 = _T("Ӳ��ѭ��������B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// Ӳ��ר��������B

	 
	m_lableGroup[7].name1 = _opl(TXT_COINMGR_ADDING_AFTER_ADDING);					// �ӱҺ�
	m_lableGroup[8].name1 = _T("Ӳ��ѭ��������A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// Ӳ��ר��������A
	m_lableGroup[9].name1 = _T("Ӳ��ѭ��������B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// Ӳ��ר��������B

	// name2 - ���
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

	// name4 - ��ֵ
	m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
	m_lableGroup[4].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
	m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					  
	m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					

	// name6 - Ӳ��ר��������A��B����
	m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
	m_lableGroup[4].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
	m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
	m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

	// name6 - Ӳ�Ҳ�����ӱ�����
	m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// ���
	m_lableGroup[5].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// ���


	// name3-ID Data
	m_lableGroup[1].name3 = boxA.strID;	// Ӳ��ר��������AID
	m_lableGroup[4].name3 = boxB.strID;	// Ӳ��ר��������BID

	m_lableGroup[8].name3 = boxA.strID;	// Ӳ��ר��������AID
	m_lableGroup[9].name3 = boxB.strID;	// Ӳ��ר��������BID

	m_lableGroup[1].name5 = CashValueToString(boxA.value);					// Ӳ�Ҳ�����A��ֵ
	m_lableGroup[4].name5 = CashValueToString(boxB.value);					// Ӳ�Ҳ�����B��ֵ

	m_lableGroup[1].name7.Format(_T("%d"),boxA.ExCount);					// Ӳ��ר��������A����
	m_lableGroup[4].name7.Format(_T("%d"),boxB.ExCount);					// Ӳ��ר��������B����

	m_lableGroup[2].name7.Format(_T("%d"),boxA.AddingCount);
	m_lableGroup[5].name7.Format(_T("%d"),boxB.AddingCount);

	// �ӱҺ�Ӳ��ר��������A��B����ֵ������
	m_lableGroup[8].name5 = CashValueToString(boxA.value);					// Ӳ��ר��������A��ֵ
	m_lableGroup[9].name5 = CashValueToString(boxB.value);					// Ӳ��ר��������B��ֵ

	int DQuantity = _ttoi(model.dedicatedchangeboxdata.QuantityA);
	int AQuantity = _ttoi(model.addingboxdata.AddingBoxQuantityA);
	CString AfterAddingQuantity = _T("");
	AfterAddingQuantity.Format(_T("%d"),DQuantity+AQuantity);

	m_lableGroup[8].name7.Format(_T("%d"),boxA.CurrentCount);				// Ӳ��ר��������A����
	m_lableGroup[9].name7.Format(_T("%d"),boxB.CurrentCount);				// Ӳ��ר��������B����
}

CString CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::CashValueToString(BankNoteAndCoinType_t type)
{
	CString strValue;
	switch(type)
	{
	case Coin1Yuan: strValue = _T("  1 Ԫ");break;
	case CoinHalfYuan :strValue = _T("  5 ��");break;
	case Banknote1Yuan:strValue = _T("  1 Ԫ");break;
	case Banknote5Yuan:strValue = _T("  5 Ԫ");break;
	case Banknote10Yuan:strValue = _T("  50MOP");break;
	case Banknote20Yuan:strValue = _T(" 100MOP");break;
	case Banknote50Yuan:strValue = _T(" 500MOP");break;
	case Banknote100Yuan:strValue = _T("1000MOP");break;
	default: break;
	}
	return strValue;
}

void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowTips()
{
	for ( int i = 0; i < 15; i++ ){
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,640);									// ���õ�һ�п��
	theAPP_SESSION.ShowOfficeGuide(_T("�ӱҽ����У��밴������ָʾ������"));
	//m_lableGroup[0].name1 = _T("��һ������Ӳ�����������ۣ�����Ӳ�������䡣");
	//m_lableGroup[1].name1 = _T("�ڶ�������Ӳ������������ۣ�ȡ��Ӳ��������ǡ�");
	//m_lableGroup[2].name1 = _T("�����������Ӳ�ҵ�Ӳ�������䡣");
	//m_lableGroup[3].name1 = _T("���Ĳ���װ��Ӳ��������ǣ����ر�����������ۡ�");
	//m_lableGroup[4].name1 = _T("���岽������Ӳ�������䣬�ر�Ӳ�����������ۡ�");
	//m_lableGroup[5].name1 = _T("����������[F2/Alt]��ɼӱҲ�����");
	m_lableGroup[0].name1 = _T("��һ����װ��Ӳ�������䣬��Ӳ�����������ۡ�");
	m_lableGroup[1].name1 = _T("�ڶ�������Ӳ�Ҵ���������ۣ�����Ӳ�������䵲�塣");
	m_lableGroup[2].name1 = _T("������������Ӳ�������䵲�壬�ر�Ӳ�Ҵ��������ۡ�");
	m_lableGroup[3].name1 = _T("���Ĳ����ر�Ӳ�����������ۣ�ȡ��Ӳ�������䡣");
	m_lableGroup[4].name1 = _T("���岽����[F2/Alt]��ɼӱҲ�����");
}