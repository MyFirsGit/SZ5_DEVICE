#include "stdafx.h"
#include "DeviceStatusDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDeviceStatusDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDeviceStatusDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusDlg::CDeviceStatusDlg(CService* pService)
	:COperationDlg(CDeviceStatusDlg::IDD,pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusDlg::~CDeviceStatusDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CDeviceStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	//����GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_PAGE | KEY_RETURN);	// ���÷�ҳ��

	//m_baseInfoArea->SetLableWidth(LABEL_COL_1,220);		// ���õ�һ�п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_2,120);		// ���õڶ��п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_3,80);		// ���õ����п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_4,100);		// ���õ����п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_5,80);		// ���õ����п��

	//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	//DeviceType = theMAINTENANCE_INFO.GetDeviceType();	// ��ȡ�豸����
	//if(DEVICE_TYPE_TVM == DeviceType)
	//{
	//	InitlableDeviceStatusInfo();
	//}
	//else if(DEVICE_TYPE_AVM == DeviceType)
	//{
	//	InitlableDeviceStatusInfoforAVM();
	//}
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
void CDeviceStatusDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	theAPP_SESSION.SetKeyboard(KEY_PAGE | KEY_RETURN);	// ���÷�ҳ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);		// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,120);		// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,125);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,100);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,115);		// ���õ����п��
	//m_baseInfoArea->SetLableWidth(0,LABEL_COL_1,300);

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);


	DeviceType = theMAINTENANCE_INFO.GetDeviceType();	// ��ȡ�豸����
	//if(DEVICE_TYPE_TVM == DeviceType)
	//{
		InitlableDeviceStatusInfo();
	//}
	/*else if(DEVICE_TYPE_AVM == DeviceType)
	{
		InitlableDeviceStatusInfoforAVM();
	}*/
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::UpdateUI()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();
	model.cashboxstatus.timesstatus = pSvc->GetCurrentPage();
    bool bIsShowNearFullStatus = theFunction_INFO.IsShowNearlyFullEmptyParaValue();
	if(Main_Page == model.cashboxstatus.timesstatus)				// ��ʾ���������ա��������ѿա�����״̬����
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F6_ESC));
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_lableGroup[i].name2 = _T("");
			m_lableGroup[i].name4 = _T("");
			m_lableGroup[i].name5 = _T("");
		}
		ShowStatus();
	}
	else if(Remain_page == model.cashboxstatus.timesstatus)			// ��ʾ��š�ʣ��ö/��������
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F5_F6_ESC));
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_REMAIN_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_baseInfoArea->SetLabelColor(i,LABEL_COL_3,BLACK);
		}
		ShowCashBoxData();
	}
	else if(Empty_Full_page == model.cashboxstatus.timesstatus)		// ��ʾ���ա��������ѿա�����ö/����
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_F5_ESC);
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_EMPTYFULL_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_baseInfoArea->SetLabelColor(i,LABEL_COL_3,BLACK);
		}
		ShowEmptyFullData();
	}
	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL����name1������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::InitlableDeviceStatusInfo()
{
	int index = 0;
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DISPLAY_SET);
	m_lableGroup[index++].name1 = _T("Ӳ��ѭ��������A")/*_opl(DEVICESTATUS_CYCLE_A_COIN_SET)*/;			// Ӳ��ѭ��������A
	m_lableGroup[index++].name1 = _T("Ӳ��ѭ��������B")/*_opl(DEVICESTATUS_CYCLE_B_COIN_SET)*/;			// Ӳ��ѭ��������B
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);		// Ӳ��ר��������A
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);		// Ӳ��ר��������B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);		// Ӳ�һ�����A
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);		// Ӳ�һ�����B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);	// ֽ�һ�����
	m_lableGroup[index++].name1 = _opl(TXT_BANKNOTEMGR_BOX);								// ֽ��������
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_A_SET);							// ֽ��ѭ����A
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_B_SET);							// ֽ��ѭ����B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_C_SET);							// ֽ��ѭ����C
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_D_SET);							// ֽ��ѭ����D
	m_lableGroup[index++].name1	= _T("Ʊ��A");									// Ʊ��A
	m_lableGroup[index++].name1	= _T("Ʊ��B");									// Ʊ��B
	m_lableGroup[index++].name1	= _T("��Ʊ��");									// ��Ʊ��
	////m_lableGroup[index++].name1 = _opl(DEVICESTATUS_TOKENA_SET);					// Token��A
	////m_lableGroup[index++].name1 = _opl(DEVICESTATUS_TOKENB_SET);					// Token��B
	//m_lableGroup[index++].name1 = _opl(TH_TEST_TEXT_TOKEN_HOPPER_BOX_1);				// Token Hopper ��A
	//m_lableGroup[index++].name1 = _opl(TH_TEST_TEXT_TOKEN_HOPPER_BOX_2);				// Token Hopper ��B
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_INVALID_TOKEN_SET);					// Token��Ʊ��
	//m_lableGroup[index++].name1 = _T("Token������");									// Token������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL����name2��name3��name4��name5������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowCashBoxData()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();

	int index = 0;
	SetColor(index++,model.cashboxdata.CycleChangeCoinAID);
	SetColor(index++,model.cashboxdata.CycleChangeCoinBID);
	//SetColor(index++,model.cashboxdata.DedicatedChangeCoinAID);
	//SetColor(index++,model.cashboxdata.DedicatedChangeCoinBID);
	SetColor(index++,model.cashboxdata.CoinCollectionAID);
	//SetColor(index++,model.cashboxdata.CoinCollectionBID);
	SetColor(index++,model.cashboxdata.BanknoteCollectionID);
 	SetColor(index++,model.cashboxdata.BanknoteChangeBoxID);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxA);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxB);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxC);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxD);
	SetColor(index++,model.cashboxdata.CardBoxAID);
	SetColor(index++,model.cashboxdata.CardBoxBID);
	SetColor(index++,model.cashboxdata.INVALIDCardID);
	//SetColor(index++,model.cashboxdata.TokenBoxAID);
	//SetColor(index++,model.cashboxdata.TokenBoxBID);
	//SetColor(index++,model.cashboxdata.TokenHopperAID);
	//SetColor(index++,model.cashboxdata.TokenHopperBID);
	//SetColor(index++,model.cashboxdata.InvalidTokenBoxID);
	//SetColor(index++,model.cashboxdata.CollectedTokenBoxID);
	
	// ���
	index = 0;
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);											
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	// ʣ������
	index = 0;
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);

	// ���Data	
	index = 0;
	m_lableGroup[index++].name3 = model.cashboxdata.CycleChangeCoinAID;
	m_lableGroup[index++].name3 = model.cashboxdata.CycleChangeCoinBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.DedicatedChangeCoinAID;
	//m_lableGroup[index++].name3 = model.cashboxdata.DedicatedChangeCoinBID;
	m_lableGroup[index++].name3 = model.cashboxdata.CoinCollectionAID;
	//m_lableGroup[index++].name3 = model.cashboxdata.CoinCollectionBID;
	m_lableGroup[index++].name3 = model.cashboxdata.BanknoteCollectionID;
	m_lableGroup[index++].name3 = model.cashboxdata.BanknoteChangeBoxID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_A_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_B_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_C_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_D_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.CardBoxAID;	
	m_lableGroup[index++].name3 = model.cashboxdata.CardBoxBID;
	m_lableGroup[index++].name3 = model.cashboxdata.INVALIDCardID;
	////m_lableGroup[index++].name3 = model.cashboxdata.TokenBoxAID;	
	////m_lableGroup[index++].name3 = model.cashboxdata.TokenBoxBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.TokenHopperAID;	
	//m_lableGroup[index++].name3 = model.cashboxdata.TokenHopperBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.InvalidTokenBoxID;
	//m_lableGroup[index++].name3 = model.cashboxdata.CollectedTokenBoxID;

	index = 0;
	// ʣ��ö��/����Data
	m_lableGroup[index++].name5 = model.cashboxdata.CycleChangeCoinAData;	
	m_lableGroup[index++].name5 = model.cashboxdata.CycleChangeCoinBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.DedicatedChangeCoinAData;
	//m_lableGroup[index++].name5 = model.cashboxdata.DedicatedChangeCoinBData;
	m_lableGroup[index++].name5 = model.cashboxdata.CoinCollectionAData;
	//m_lableGroup[index++].name5 = model.cashboxdata.CoinCollectionBData;
	m_lableGroup[index++].name5 = model.cashboxdata.BanknoteCollectionData;
	m_lableGroup[index++].name5 = model.cashboxdata.BanknoteChangeData;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxA;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxB;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxC;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxD;
	m_lableGroup[index++].name5 = model.cashboxdata.CardBoxAData;
	m_lableGroup[index++].name5 = model.cashboxdata.CardBoxBData;
	m_lableGroup[index++].name5 = model.cashboxdata.InvalidCardData;
	////m_lableGroup[index++].name5 = model.cashboxdata.TokenBoxAData;	
	////m_lableGroup[index++].name5 = model.cashboxdata.TokenBoxBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.TokenHopperAData;	
	//m_lableGroup[index++].name5 = model.cashboxdata.TokenHopperBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.InvalidTokenData;
	//m_lableGroup[index++].name5 = model.cashboxdata.CollectedTokenData;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL����name2��name3��name4��name5�Ľ��ա���������������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowEmptyFullData()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();

	int index = 0;
	// ���ա���
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// ����ö��
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// ����ö��
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// ����ö��
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// ����ö��
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// ����ö��
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// ����ö��

	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// ����ö��
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������

	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// ��������
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// ��������
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// ��������

	// �ѿա���
	index = 0;
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// �ѿ�ö��
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// �ѿ�ö��
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// �ѿ�ö��
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// �ѿ�ö��
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ����ö��
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ����ö��

	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// ����ö��
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// �ѿ�����
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ��������
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ��������
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ��������
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// ��������

	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// �ѿ�����
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// �ѿ�����
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// �ѿ�����
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// �ѿ�����
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// ��������
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// ��������

	index = 0;
	m_lableGroup[index++].name3 = model.emptyfull.CycleChangeCoinANearlyEmpty;		// Ӳ��ѭ��������A	����ö��
	m_lableGroup[index++].name3 = model.emptyfull.CycleChangeCoinBNearlyEmpty;		// Ӳ��ѭ��������B	����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.DedicatedChangeCoinANearlyEmpty;	// Ӳ��ר��������A	����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.DedicatedChangeCoinBNearlyEmpty;	// Ӳ��ר��������B	����ö��
	m_lableGroup[index++].name3 = model.emptyfull.CoinCollectionANearlyFull;		// Ӳ��ר�û�����A	����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.CoinCollectionBNearlyFull;		// Ӳ��ר�û�����B	����ö��
	m_lableGroup[index++].name3 = model.emptyfull.BanknoteCollectionNearlyFulll;	// ֽ�һ�����		��������
	m_lableGroup[index++].name3 = model.emptyfull.BanknoteChangeNearlyEmpty;		// ֽ��������		����ö��
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull;	// ѭ����A			��������
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull;	// ѭ����B			��������
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull;	// ѭ����C			��������
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull;	// ѭ����D			��������
	m_lableGroup[index++].name3 = model.emptyfull.CardMNearlyEmpty;					// Smart-card��		��������
	m_lableGroup[index++].name3 = model.emptyfull.CardMNearlyEmpty;					// Smart-card��		��������
	m_lableGroup[index++].name3 = model.emptyfull.INVALIDCardMNearlyFull;			// Smart-card��Ʊ��	��������
	////m_lableGroup[index++].name3 = model.emptyfull.TokenANearlyEmpty;						// Token��A			����ö��
	////m_lableGroup[index++].name3 = model.emptyfull.TokenBNearlyEmpty;						// Token��B			����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.TokenANearlyEmpty;						// Token Hopper��A	����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.TokenBNearlyEmpty;						// Token Hopper��B	����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.InvalidTokenNearlyFull;					// Token��Ʊ��		����ö��
	//m_lableGroup[index++].name3 = model.emptyfull.CollectedTokenNearlyFull;					// Token������		����ö��

	index = 0;
	m_lableGroup[index++].name5 = model.emptyfull.CycleChangeCoinAEmpty;		// Ӳ��ѭ��������	�ѿ�ö��
	m_lableGroup[index++].name5 = model.emptyfull.CycleChangeCoinBEmpty;		// Ӳ��ѭ��������	�ѿ�ö��
	//m_lableGroup[index++].name5 = model.emptyfull.DedicatedChangeCoinAEmpty;	// Ӳ��ר��������A	�ѿ�ö��
	//m_lableGroup[index++].name5 = model.emptyfull.DedicatedChangeCoinBEmpty;	// Ӳ��ר��������B	�ѿ�ö��
	m_lableGroup[index++].name5 = model.emptyfull.CoinCollectionAFull;			// Ӳ��ר�û�����A	����ö��
	//m_lableGroup[index++].name5 = model.emptyfull.CoinCollectionBFull;			// Ӳ��ר�û�����B	����ö��
	m_lableGroup[index++].name5 = model.emptyfull.BanknoteCollectionFull;		// ֽ�һ�����		��������
	m_lableGroup[index++].name5 = model.emptyfull.BanknoteChangeEmpty;			// ֽ��������		�ѿ�ö��
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_A_Full;		// ѭ����A			��������
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_B_Full;		// ѭ����B			��������
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_C_Full;		// ѭ����C			��������
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_D_Full;		// ѭ����D			��������
	m_lableGroup[index++].name5 = model.emptyfull.CardMEmpty;					// Smart-card��		�ѿ�����
	m_lableGroup[index++].name5 = model.emptyfull.CardMEmpty;					// Smart-card��		�ѿ�����
	m_lableGroup[index++].name5 = model.emptyfull.INVALIDCardMFull;				// Smart-card��Ʊ��	��������
	////m_lableGroup[index++].name5 = model.emptyfull.TokenAEmpty;								// Token��A			�ѿ�ö��
	////m_lableGroup[index++].name5 = model.emptyfull.TokenBEmpty;								// Token��B			�ѿ�ö��
	//m_lableGroup[index++].name5 = model.emptyfull.TokenAEmpty;					// Token Hopper��A	�ѿ�ö��
	//m_lableGroup[index++].name5 = model.emptyfull.TokenBEmpty;					// Token Hopper��B	�ѿ�ö��
	//m_lableGroup[index++].name5 = model.emptyfull.InvalidTokenFull;				// Token��Ʊ��		����ö��
	//m_lableGroup[index++].name5 = model.emptyfull.CollectedTokenFull;			// Token������		����ö��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL����Status

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowStatus()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();
	// ����״̬��ɫ
	int index = 0;
	SetColor(index++,model.cashboxstatus.CCyleChangeAStatus);
	SetColor(index++,model.cashboxstatus.CCyleChangeBStatus);
	//SetColor(index++,model.cashboxstatus.CDChangeAStatus);
	//SetColor(index++,model.cashboxstatus.CDChangeBStatus);
	SetColor(index++,model.cashboxstatus.CCollectionAStatus);
	//SetColor(index++,model.cashboxstatus.CCollectionBStatus);
	SetColor(index++,model.cashboxstatus.BanknoteStatus);
	SetColor(index++,model.cashboxstatus.BanknoteChangeStatus);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS);
	SetColor(index++,model.cashboxstatus.CardBoxAStatus);
	SetColor(index++,model.cashboxstatus.CardBoxBStatus);
	SetColor(index++,model.cashboxstatus.WastedSmartcardStatus);
	////SetColor(index++,model.cashboxstatus.TokenAStatus);
	////SetColor(index++,model.cashboxstatus.TokenBStatus);
	//SetColor(index++,model.cashboxstatus.TokenHopperAStatus);
	//SetColor(index++,model.cashboxstatus.TokenHopperBStatus);
	//SetColor(index++,model.cashboxstatus.WastedTokenStatus);
	//SetColor(index++,model.cashboxstatus.CollectedTokenStatus);

	// ����״index++̬
	index = 0;
	m_lableGroup[index++].name3 = model.cashboxstatus.CCyleChangeAStatus;						
	m_lableGroup[index++].name3 = model.cashboxstatus.CCyleChangeBStatus;						
	//m_lableGroup[index++].name3 = model.cashboxstatus.CDChangeAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CDChangeBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.CCollectionAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CCollectionBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BanknoteStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BanknoteChangeStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.CardBoxAStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.CardBoxBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.WastedSmartcardStatus;
	////m_lableGroup[index++].name3 = model.cashboxstatus.TokenAStatus;
	////m_lableGroup[index++].name3 = model.cashboxstatus.TokenBStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.TokenHopperAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.TokenHopperBStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.WastedTokenStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CollectedTokenStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����״̬��ʾ����ɫ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::SetColor(int row,CString status)
{
	if(_opl(DEVICESTATUS_NORMAL_SET) == status)						// ����-��ɫ
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,GREEN);
	}
	else if(_opl(DEVICESTATUS_NEARLY_EMPTY_TXT) == status || _opl(DEVICESTATUS_NEARLY_FULL_TXT) == status)		// ����-��ɫ������-��ɫ
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,YELLOW);
	}
	else if(_opl(DEVICESTATUS_EMPTY_TXT) == status || _opl(DEVICESTATUS_FULL_TXT) == status || _opl(DEVICESTATUS_ILLEGAL_DETACH) == status || _opl(DEVICESTATUS_ILLEGAL_INSTALLATION) == status || _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION) == status)					// �ѿ�-��ɫ������-��ɫ
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,RED);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL����name1������forAVM

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CDeviceStatusDlg::InitlableDeviceStatusInfoforAVM()
//{
//	CString BanknoteCollectionID = _T("");
//	long errCode = 0;
//
//	m_baseInfoArea->SetLableWidth(LABEL_COL_2,80);													// ���õڶ��п��
//	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
//
//
//	CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
//	theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);
//
//	m_lableGroup[0].name1 = _opl(DEVICESTATUS_BANKNOTE_INFO);										// ֽ������Ϣ
//	m_lableGroup[1].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// ֽ�һ�����
//	m_lableGroup[3].name1 = _opl(DEVICESTATUS_REMAIN_QTY);											// ֽ��ʣ������
//	m_lableGroup[4].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// ֽ�һ�����
//	m_lableGroup[6].name1 = _opl(DEVICESTATUS_NEARLY_FULL_OR_FULL_CASH);							// ֽ���佫������������
//	m_lableGroup[7].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// ֽ�һ�����
//
//	m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);								// ֽ�һ�����ID
//
//	try
//	{
//		//		CBanknoteModuleHelper* BANKNOTE_HELPER;
//		BH_STATUS bhstatus;
//
//		errCode = BANKNOTE_HELPER->GetStatus(bhstatus);
//
//		BanknoteCollectionID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();		// ֽ�һ�����
//		bool IsBanknoteAttached = BANKNOTE_HELPER->IsBankNoteBoxAttached();
//		if(_T("FFFFFFFF") == BanknoteCollectionID)
//		{
//			if(true == IsBanknoteAttached)
//			{
//				BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
//			}
//			else
//			{
//				BanknoteCollectionID = _T("--------");
//			}
//
//		}
//		else
//		{
//			if(false == IsBanknoteAttached)
//			{
//				BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
//			}
//		}
//		m_lableGroup[4].name3 = BanknoteCollectionID;													// ֽ�һ�����IDData
//	}
//	// �쳣����
//	catch (CSysException& e) {
//		BanknoteCollectionID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
//		m_lableGroup[4].name3 = BanknoteCollectionID;
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		BanknoteCollectionID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
//		m_lableGroup[4].name3 = BanknoteCollectionID;
//		theEXCEPTION_MGR.ProcessException(CInnerException(DEVICE_STATUS_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//	m_lableGroup[4].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);							// ֽ�һ�����ʣ������
//	int count = 0;
//	count = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_10										
//		+	theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_20
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_50
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_100
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_5
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_1;
//	CString Banknote_Count = _T("");
//	Banknote_Count.Format(_T("%ld"),count);
//	m_lableGroup[4].name5 = Banknote_Count;															// ֽ�һ�����ʣ������Data
//	m_lableGroup[7].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);						// ֽ�һ����佫������
//	Banknote_Count.Format(_T("%d"),billbox_param.willFullBillCnt);
//	m_lableGroup[7].name3 =	Banknote_Count;															// ֽ�һ����佫������Data
//	m_lableGroup[7].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);							// ֽ�һ�������������
//	Banknote_Count.Format(_T("%d"),billbox_param.alreadyFullBillCnt);
//	m_lableGroup[7].name5 = Banknote_Count;															// ֽ�һ�������������Data
//
//	CString DeviceStatus = _T("");
//
//	// ֽ�һ�����ֽ����������
//	if(count < billbox_param.willFullBillCnt)
//	{
//		DeviceStatus = _opl(DEVICESTATUS_NORMAL_SET);												// ���� 
//	}
//	else if(count >= billbox_param.alreadyFullBillCnt)
//	{
//		DeviceStatus = _opl(DEVICESTATUS_FULL_TXT);													// ����
//	}
//	else
//	{
//		DeviceStatus = _opl(DEVICESTATUS_NEARLY_FULL_TXT);											// ����
//	}
//	m_lableGroup[1].name3 = DeviceStatus;
//	SetColor(1,DeviceStatus);
//	SetColor(4,BanknoteCollectionID);
//}
