#include "StdAfx.h"
#include "CardReadWriteMachineTestMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//ӳ����Ϣ���Ի���
IMPLEMENT_DYNAMIC(CCardReadWriteMachineTestMaintainDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CCardReadWriteMachineTestMaintainDlg,COperationDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��׼���캯��

@param      (i) CService pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCardReadWriteMachineTestMaintainDlg::CCardReadWriteMachineTestMaintainDlg(CService* pService)
	:COperationDlg(CCardReadWriteMachineTestMaintainDlg::IDD,pService)
{
	
}

CCardReadWriteMachineTestMaintainDlg::~CCardReadWriteMachineTestMaintainDlg()
{

}

int CCardReadWriteMachineTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA/* | BASE_AREA*/))
	{
		return -1;
	}

	m_baseInfoArea = new CGStatic (this,m_baseInfo,m_LabelGroupThree);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���

@param      nState     ����
@param      pWndOther  ����
@param      bMinimized ����

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();		// ��ȡ�豸����
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();

	if(pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_MAIN)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(1);
		InitMainMenuUI();
	}
	else if (pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_BASE)
	{
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(1);
		InitBaseMenuUI();
	}
	else if (pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_OPERATION)
	{
		if (modol.operation_type == CRWTestSvc::Operetion_Parameter_Ver)
		{
			theAPP_SESSION.SetKeyboard(KEY_PAGE | KEY_RETURN);	// ���÷�ҳ��
			theAPP_SESSION.SetMaxInputLength(0);
			InitParameterUI();
		}
		else if (modol.operation_type == CRWTestSvc::Operation_RW_Info)
		{
			theAPP_SESSION.SetKeyboard(KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(0);
			InitRWInfoUI();
		}
		else
		{
			InitBaseMenuUI();
		}
	}

	UpdataUI();               //��ʼ��UI��ʾ

	InitParameterBaseData();  //��ʼ��Parameter�̶���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���½�����ʾ������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::UpdataUI()
{
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();

	if (pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_MAIN)
	{
		ShowMainMenuData();
	}
	else if (pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_BASE)
	{
		ShowBaseMenuData();
	}
	else if (pSvc->GetCurDialogGroupID() == CRWTestSvc::DIALOG_GROUP_OPERATION)
	{
		if (modol.operation_type == CRWTestSvc::Operetion_Parameter_Ver)
		{
			ShowParameterData();
		}
		else if (modol.operation_type == CRWTestSvc::Operation_RW_Info)
		{
			ShowRWInfoData();
		}
		else
		{
			ShowBaseMenuData();
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ�����˵���ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitMainMenuUI()
{
	ClearData();
	m_baseInfoArea->SetLableWidth(0,400);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ�������˵���ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitBaseMenuUI()
{
	ClearData();
	m_baseInfoArea->SetLableWidth(0,400);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��Parameter��ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitParameterUI()
{
	ClearData();
	//m_baseInfoArea->SetLableWidth(0,100);
	//m_baseInfoArea->SetLableWidth(1,400);
	//m_baseInfoArea->SetLableWidth(2,100);

	//m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	//m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	//m_baseInfoArea->SetLableAlign(2,DT_LEFT);

	m_baseInfoArea->SetLableWidth(0,400);
	m_baseInfoArea->SetLableWidth(2,100);
	m_baseInfoArea->SetLableWidth(3,100);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��RW Info��ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitRWInfoUI()
{
	ClearData();
	m_baseInfoArea->SetLableWidth(0,400);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���˵�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::ShowMainMenuData()
{
	m_LabelGroupThree[0].nameLeft = _T("[1]") + _opl(RWTEST_RECHARGE);
	//if(DEVICE_TYPE_TVM == DeviceType)
	//{
		m_LabelGroupThree[1].nameLeft = _T("[2]") + _opl(RWTEST_CARD);
	//	m_LabelGroupThree[2].nameLeft = _T("[3]") + _opl(RWTEST_TOKEN);
	//}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�����˵�ѡ��˵�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::ShowBaseMenuData()
{
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();

	m_LabelGroupThree[0].nameLeft = _T("[1]") + _opl(RWTEST_RESET);
	m_LabelGroupThree[1].nameLeft = _T("[2]") + _opl(RWTEST_READ);
	if(theAPP_SESSION.IsSuperUser()){
		m_LabelGroupThree[2].nameLeft = _T("[3]") + _opl(RWTEST_PARAMETER_VERSION);
		m_LabelGroupThree[3].nameLeft = _T("[4]") + _opl(RWTEST_RW_INFOMATION);
	}	

	//��ʾ״̬,��ɫ
	m_LabelGroupThree[0].nameCenter = modol.baseMenuInfo[0].strMenuStatus;
	m_baseInfoArea->SetLabelColor(0,1,modol.baseMenuInfo[0].strMenuStatus == SUCCESS ? SOFTGREEN : RED);

	m_LabelGroupThree[1].nameCenter = modol.baseMenuInfo[1].strMenuStatus;
	m_baseInfoArea->SetLabelColor(1,1,modol.baseMenuInfo[1].strMenuStatus == SUCCESS ? SOFTGREEN : RED);
	// �����û���ʾ
	if(theAPP_SESSION.IsSuperUser()){
		m_LabelGroupThree[2].nameCenter = modol.baseMenuInfo[2].strMenuStatus;
		m_baseInfoArea->SetLabelColor(2,1,modol.baseMenuInfo[2].strMenuStatus == FAILED ? RED : SOFTGREEN);

		m_LabelGroupThree[3].nameCenter = modol.baseMenuInfo[3].strMenuStatus;
		m_baseInfoArea->SetLabelColor(3,1,modol.baseMenuInfo[3].strMenuStatus == FAILED ? RED : SOFTGREEN);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾParameter��Ϣ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::ShowParameterData()
{
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();
	for(int i = 0; i < 20; i++)
	{
		m_LabelGroupThree[i].nameLeft   = _T("");
		m_LabelGroupThree[i].nameCenter = _T("");
		m_LabelGroupThree[i].nameRight  = _T("");
	}
	//������
	//m_LabelGroupThree[0].nameLeft   = _opl(RW_TEST_TEXT_ID);
	//m_LabelGroupThree[0].nameCenter = _opl(RW_TEST_TEXT_NAME);
	//m_LabelGroupThree[0].nameRight  = _opl(RW_TEST_TEXT_VERSION);
	m_LabelGroupThree[0].nameLeft   = _opl(RW_TEST_TEXT_NAME);
	m_LabelGroupThree[0].nameCenter = _opl(RW_TEST_TEXT_VERSION);
	m_LabelGroupThree[0].nameRight  = _T("");
	//if (modol.timesstatus == 1)
	{
		//��ֵ��ʾ��������
		for (int i = 1; i <= 14; i++ )
		{
			//m_LabelGroupThree[i].nameLeft   = modol.paramterInfo[i-1].strID;
			//m_LabelGroupThree[i].nameCenter = modol.paramterInfo[i-1].strName;
			//m_LabelGroupThree[i].nameRight  = modol.paramterInfo[i-1].strVersion;
			m_LabelGroupThree[i].nameLeft   = modol.paramterInfo[i-1].strName;
			m_LabelGroupThree[i].nameCenter = modol.paramterInfo[i-1].strVersion;
			m_LabelGroupThree[i].nameRight  = modol.paramterInfo[i-1].strID;
		}
	}
	//else if (modol.timesstatus == 2)
	//{
	//	//��ֵ��ʾ��������
	//	for (int i = 1,j=15; i <= 5,j<= 19 ; i++,j++ )
	//	{
	//		//m_LabelGroupThree[i].nameLeft   = modol.paramterInfo[j-1].strID;
	//		//m_LabelGroupThree[i].nameCenter = modol.paramterInfo[j-1].strName;
	//		//m_LabelGroupThree[i].nameRight  = modol.paramterInfo[j-1].strVersion;
	//		m_LabelGroupThree[i].nameLeft   = modol.paramterInfo[j-1].strName;
	//		m_LabelGroupThree[i].nameCenter = modol.paramterInfo[j-1].strVersion;
	//		m_LabelGroupThree[i].nameRight  = modol.paramterInfo[j-1].strID;
	//	}

	//}
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC/*GUIDE_F3_F5_F6_ESC*/);
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾRW��Ϣ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::ShowRWInfoData()
{
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();

	for (int i = 0; i < 6 ;i++)
	{
		m_LabelGroupThree[i].nameLeft   = modol.rwInfo[i].strRWName;
		m_LabelGroupThree[i].nameCenter = modol.rwInfo[i].strRWValue;
	}
	if(_opl(GUIDE_RW_ISAM_NO_INSTALL) == modol.rwInfo[3].strRWValue)
	{
		m_baseInfoArea->SetLabelColor(3,1,RED);
	}
	if(_opl(GUIDE_RW_ISAM_NO_INSTALL) == modol.rwInfo[4].strRWValue)
	{
		m_baseInfoArea->SetLabelColor(4,1,RED);
	}
	if(_opl(GUIDE_RW_ISAM_NO_INSTALL) == modol.rwInfo[5].strRWValue)
	{
		m_baseInfoArea->SetLabelColor(5,1,RED);
	}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_CONFORM_READER_INFO);//!!!
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ʾParameter��Ϣ������
@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitParameterBaseData()
{
	CRWTestSvc* pSvc = (CRWTestSvc*)GetService();
	CRWTestSvc::Modol& modol = pSvc->GetDataModol();

	int index = 0;
	//��ʼ��parameter��ʾ��,(����)


	//��ʼ��RW������Ϣ��ʾ�����䣩
	modol.rwInfo[0].strRWName     = _opl(RW_TEST_TEXT_DEVICE_ID);
	modol.rwInfo[1].strRWName     = _opl(RW_TEST_TEXT_APP_VERSION);
	modol.rwInfo[2].strRWName     = _opl(RW_TEST_TEXT_HARDWARE_VERSION);
	modol.rwInfo[3].strRWName     = _T("ACC ISAM:")/*_opl(RW_TEST_TEXT_ACC_ISAM)*/;
	modol.rwInfo[4].strRWName     = _T("ACC PSAM:")/*_opl(RW_TEST_TEXT_ACC_ISAM)*/;
	modol.rwInfo[5].strRWName     = _T("YKT PSAM:")/*_opl(RW_TEST_TEXT_ACC_ISAM)*/;

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Lable����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::ClearData()
{
	for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
	{
		m_LabelGroupThree[nRow].nameLeft   = _T("");
		m_LabelGroupThree[nRow].nameCenter = _T("");
		m_LabelGroupThree[nRow].nameRight  = _T("");

		for(int nCol = 0; nCol < _MAX_COL; ++nCol)
		{
			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
		}		
	} 
}