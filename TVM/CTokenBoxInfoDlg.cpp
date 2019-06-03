//#include "StdAfx.h"
//#include "CTokenBoxInfoDlg.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
//IMPLEMENT_DYNAMIC(CTokenBoxInfoDlg,COperationDlg)
//BEGIN_MESSAGE_MAP(CTokenBoxInfoDlg,COperationDlg)
//	ON_WM_CREATE()
//	ON_WM_ACTIVATE()
//END_MESSAGE_MAP()
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ���캯��
//
//@param      CService* pService;Serviceָ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CTokenBoxInfoDlg::CTokenBoxInfoDlg(CService* pService)
//	: COperationDlg(CTokenBoxInfoDlg::IDD,pService){
//
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CTokenBoxInfoDlg::~CTokenBoxInfoDlg(){
//
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ���洴��
//
//@param      (i)LPCREATESTRUCT lpCreateStruct
//
//@retval     int 0:�����ɹ�  -1������ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//int CTokenBoxInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct){
//	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA)){
//		return -1;
//	}
//
//	// ����GStatic
//	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_labelGroupFive);
//	
//	return 0;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ���漤���Ĵ���
//
//@param      (i)UINT nState      ��ʹ��
//@param      (i)CWnd* pWndOther  ��ʹ��
//@param      (i)BOOL bMinimized  ��ʹ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
//	__super::OnActivate(nState,pWndOther,bMinimized);
//
//	// ȡ�õ�ǰ�����Ӧ������ָ�����
//	pService = (CTokenManageSvc*)GetService();
//	if(NULL != pService)	
//	{		
//		model	 = pService->GetDataModel();
//		UINT dlg = pService->GetCurDialogGroupID();
//		if(dlg == CTokenManageSvc::DIALOG_GROUP_MAIN){
//			InitMainDlg();
//			theAPP_SESSION.SetMaxInputLength(1);
//			theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_INSTALL){
//			InitInstallDlg();
//			theAPP_SESSION.SetMaxInputLength(1);
//			theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_INPUT | KEY_F2);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_UNINSTALL){
//			InitUninstallDlg();
//			theAPP_SESSION.SetMaxInputLength(1);
//			theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_RECYCLE){
//			InitRecycleDlg();
//			//theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_F2);
//			theAPP_SESSION.SetMaxInputLength(1);
//			theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_INSTALLED){
//			InitInstalledDlg();
//			theAPP_SESSION.SetKeyboard(KEY_RETURN);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_UNINSTALLED){
//			InitUninstalledDlg();
//			theAPP_SESSION.SetKeyboard(KEY_RETURN);
//		}
//		else if(dlg == CTokenManageSvc::DIALOG_GROUP_RECYCLED){
//			InitRecycledDlg();
//			theAPP_SESSION.SetKeyboard(KEY_RETURN);
//		}
//
//	}	
//	ClearLabelData();
//	// ��ʾ����
//	UpdateUI();
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      �������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::UpdateUI(){
//	model	 = pService->GetDataModel();
//	UINT dlg = pService->GetCurDialogGroupID();
//	
//	if(dlg == CTokenManageSvc::DIALOG_GROUP_MAIN){
//		ShowUpMainDlg();
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_INSTALL){
//		ShowUpInstallDlg();
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_UNINSTALL){
//		ShowUpUninstallDlg();
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_RECYCLE){
//		ShowUpRecycleDlg();
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_INSTALLED){
//		ShowUpInstalledDlg();
//		// ��װ���
//		if(model.bPrintSuccess && model.bInsertDataBaseSuccess) 
//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_INSTALLCOMPLETE));
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_UNINSTALLED){
//		ShowUpUninstalledDlg();
//		// ж�����
//		if(model.bPrintSuccess && model.bInsertDataBaseSuccess) 
//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_UNINSTALLCOMPLETE));
//	}
//	else if(dlg == CTokenManageSvc::DIALOG_GROUP_RECYCLED){
//		ShowUpRecycledDlg();
//		// �������
//		if(model.bPrintSuccess && model.bInsertDataBaseSuccess) 
//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_RECOVERYCOMPLETE));
//	}
//
//	if(!model.bInsertDataBaseSuccess){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_INSERT_DATABASE_FALIED));
//		return;
//	}
//
//	if(!model.bPrintSuccess){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_PRINT_FALIED));
//		return;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ��Main����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitMainDlg(){
//	m_baseInfoArea->SetLableWidth(0,600);
//	m_baseInfoArea->SetLableWidth(1,0);
//	m_baseInfoArea->SetLableWidth(2,0);
//	m_baseInfoArea->SetLableWidth(3,0);
//	m_baseInfoArea->SetLableWidth(4,0);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ����װ����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitInstallDlg(){
//	m_baseInfoArea->SetLableWidth(0,200);
//	m_baseInfoArea->SetLableWidth(1,100);
//	m_baseInfoArea->SetLableWidth(2,200);
//	m_baseInfoArea->SetLableWidth(3,0);
//	m_baseInfoArea->SetLableWidth(4,0);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
//	m_baseInfoArea->SetLableAlign(2,DT_CENTER);
//
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ����װ��ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitInstalledDlg(){
//	m_baseInfoArea->SetLableWidth(0,50);
//	m_baseInfoArea->SetLableWidth(1,150);
//	m_baseInfoArea->SetLableWidth(2,100);
//	m_baseInfoArea->SetLableWidth(3,100);
//	m_baseInfoArea->SetLableWidth(4,100);
//
//	m_baseInfoArea->SetLableWidth(0,0,150);
//	m_baseInfoArea->SetLableWidth(8,0,150);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
//	m_baseInfoArea->SetLableAlign(4,DT_LEFT);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ��ж�ؽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitUninstallDlg(){
//	m_baseInfoArea->SetLableWidth(0,150);
//	m_baseInfoArea->SetLableWidth(1,100);
//	m_baseInfoArea->SetLableWidth(2,80);
//	m_baseInfoArea->SetLableWidth(3,80);
//	m_baseInfoArea->SetLableWidth(4,150);
//
//	// ���������еĿ��
//	m_baseInfoArea->SetLableWidth(7,0,400);
//	m_baseInfoArea->SetLableWidth(8,0,400);
//	m_baseInfoArea->SetLableWidth(9,0,400);
//	m_baseInfoArea->SetLableWidth(10,0,400);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(3,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(4,DT_RIGHT);
//
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ��ж����ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitUninstalledDlg(){
//	m_baseInfoArea->SetLableWidth(0,50);
//	m_baseInfoArea->SetLableWidth(1,150);
//	m_baseInfoArea->SetLableWidth(2,100);
//	m_baseInfoArea->SetLableWidth(3,100);
//	m_baseInfoArea->SetLableWidth(4,100);
//
//	m_baseInfoArea->SetLableWidth(0,0,150);
//	m_baseInfoArea->SetLableWidth(8,0,150);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
//	m_baseInfoArea->SetLableAlign(4,DT_LEFT);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ�����ս���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitRecycleDlg(){
//	// ����
//	//m_baseInfoArea->SetLableWidth(0,120);
//	//m_baseInfoArea->SetLableWidth(1,60);
//	//m_baseInfoArea->SetLableWidth(2,60);
//	//m_baseInfoArea->SetLableWidth(3,100);
//	//m_baseInfoArea->SetLableWidth(4,100);
//	// Ӣ��
//	//m_baseInfoArea->SetLableWidth(0,140);
//	//m_baseInfoArea->SetLableWidth(1,80);
//	//m_baseInfoArea->SetLableWidth(2,60);
//	//m_baseInfoArea->SetLableWidth(3,100);
//	//m_baseInfoArea->SetLableWidth(4,100);
//
//	m_baseInfoArea->SetLableWidth(0,180);
//	m_baseInfoArea->SetLableWidth(1,80);
//	m_baseInfoArea->SetLableWidth(2,80);
//	m_baseInfoArea->SetLableWidth(3,100);
//	m_baseInfoArea->SetLableWidth(4,100);
//
//
//	m_baseInfoArea->SetLableWidth(0,0,150);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(2,DT_RIGHT);
//	m_baseInfoArea->SetLableAlign(3,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(4,DT_LEFT);
//
//	//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_SAVEGUIDE));
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʼ��������ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::InitRecycledDlg(){
//	m_baseInfoArea->SetLableWidth(0,50);
//	m_baseInfoArea->SetLableWidth(1,150);
//	m_baseInfoArea->SetLableWidth(2,100);
//	m_baseInfoArea->SetLableWidth(3,100);
//	m_baseInfoArea->SetLableWidth(4,100);
//
//	m_baseInfoArea->SetLableWidth(0,0,150);
//	m_baseInfoArea->SetLableWidth(6,0,150);
//
//	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
//	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
//	m_baseInfoArea->SetLableAlign(4,DT_LEFT);
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾMain����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpMainDlg(){
//	m_labelGroupFive[0].name1	= _T("[1]") + _opl(TXT_TOKENMGR_ATTACH);
//	m_labelGroupFive[1].name1	= _T("[2]") + _opl(TXT_TOKENMGR_DETACH);
//	m_labelGroupFive[2].name1	= _T("[3]") + _opl(TXT_TOKENMGR_RECYCLE_TOKEN);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾ��װ����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpInstallDlg(){
//	m_labelGroupFive[0].name1	= _T("[1]") + _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[0].name2	= TransforBoxID(model.AfterTokenBoxA.strBoxID);
//	m_labelGroupFive[0].name3	= TransforStatus(model.AfterTokenBoxA.emStatus);
//	m_baseInfoArea->SetLabelColor(0,2,model.AfterTokenBoxA.GetColorForState());
//
//	m_labelGroupFive[1].name1	= _T("[2]") + _opl(TXT_TOKENMGR_TOKENA_QUANTITY) + _T(":");
//	m_labelGroupFive[1].name2	= TransforCount(model.AfterTokenBoxA.nQuantity);
//
//	m_labelGroupFive[2].name1	= _T("[3]") + _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[2].name2	= TransforBoxID(model.AfterTokenBoxB.strBoxID);
//	m_labelGroupFive[2].name3	= TransforStatus(model.AfterTokenBoxB.emStatus);
//	m_baseInfoArea->SetLabelColor(2,2,model.AfterTokenBoxB.GetColorForState());
//
//	m_labelGroupFive[3].name1	= _T("[4]") + _opl(TXT_TOKENMGR_TOKENB_QUANTITY) + _T(":");
//	m_labelGroupFive[3].name2	= TransforCount(model.AfterTokenBoxB.nQuantity);
//
//	m_labelGroupFive[4].name1	= _T("[5]") + _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[4].name2	= TransforBoxID(model.AfterInvalidTokenBox.strBoxID);
//	m_labelGroupFive[4].name3	= TransforStatus(model.AfterInvalidTokenBox.emStatus);
//	m_baseInfoArea->SetLabelColor(4,2,model.AfterInvalidTokenBox.GetColorForState());
//
//	m_labelGroupFive[5].name1	= _T("[6]") + CString(_T("������ı��")) + _T(":");
//	m_labelGroupFive[5].name2	= TransforBoxID(model.AfterCollectedTokenBox.strBoxID);
//	m_labelGroupFive[5].name3	= TransforStatus(model.AfterCollectedTokenBox.emStatus);
//	m_baseInfoArea->SetLabelColor(5,2,model.AfterCollectedTokenBox.GetColorForState());
//
//	// Hopper ��Ϣ
//	m_labelGroupFive[6].name1	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[6].name2	= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//
//	m_labelGroupFive[7].name1	= _opl(TXT_TOKENMGR_HOPPER_A_NUMBER) + _T(":");
//	m_labelGroupFive[7].name2	= TransforCount(model.AfterTokenHopperA.nQuantity);
//
//	m_labelGroupFive[8].name1	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[8].name2	= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//
//	m_labelGroupFive[9].name1	= _opl(TXT_TOKENMGR_HOPPER_B_NUMBER) + _T(":");
//	m_labelGroupFive[9].name2	= TransforCount(model.AfterTokenHopperB.nQuantity);
//	// ��ɫ
//	/*if(model.AfterTokenBoxA.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//	m_baseInfoArea->SetLabelColor(0,2,GREEN);
//	}
//	else{
//	m_baseInfoArea->SetLabelColor(0,2,RED);
//	}
//
//	if(model.AfterTokenBoxB.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//	m_baseInfoArea->SetLabelColor(2,2,GREEN);
//	}
//	else{
//	m_baseInfoArea->SetLabelColor(2,2,RED);
//	}
//
//	if(model.AfterInvalidTokenBox.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//	m_baseInfoArea->SetLabelColor(4,2,GREEN);
//	}
//	else{
//	m_baseInfoArea->SetLabelColor(4,2,RED);
//	}*/
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾ��װ��ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpInstalledDlg(){
//	/********************************��װǰ******************************************************/
//	m_labelGroupFive[0].name1	= _opl(TXT_TOKENMGR_BEFORE_ATTACH);
//	
//	m_labelGroupFive[1].name2	= _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[1].name3	= TransforBoxID(model.BeforeTokenBoxA.strBoxID);
//	m_labelGroupFive[1].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[1].name5	= TransforCount(model.BeforeTokenBoxA.nQuantity);
//
//	m_labelGroupFive[2].name2	= _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[2].name3	= TransforBoxID(model.BeforeTokenBoxB.strBoxID);
//	m_labelGroupFive[2].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[2].name5	= TransforCount(model.BeforeTokenBoxB.nQuantity);
//
//	m_labelGroupFive[3].name2	= _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[3].name3	= TransforBoxID(model.BeforeInvalidTokenBox.strBoxID);
//	m_labelGroupFive[3].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[3].name5	= TransforCount(model.BeforeInvalidTokenBox.nQuantity);
//
//	m_labelGroupFive[4].name2	= _T("������ı��:");
//	m_labelGroupFive[4].name3	= TransforBoxID(model.BeforeCollectedTokenBox.strBoxID);
//	m_labelGroupFive[4].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[4].name5	= TransforCount(model.BeforeCollectedTokenBox.nQuantity);
//
//	m_labelGroupFive[5].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[5].name3	= TransforBoxID(model.BeforeTokenHopperA.strBoxID);
//	m_labelGroupFive[5].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[5].name5	= TransforCount(model.BeforeTokenHopperA.nQuantity);
//
//	m_labelGroupFive[6].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[6].name3	= TransforBoxID(model.BeforeTokenHopperB.strBoxID);
//	m_labelGroupFive[6].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[6].name5	= TransforCount(model.BeforeTokenHopperB.nQuantity);
//
//	/********************************��װ��******************************************************/
//	m_labelGroupFive[8].name1	= _opl(TXT_TOKENMGR_AFTER_ATTACH);
//
//	m_labelGroupFive[9].name2	= _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[9].name3	= TransforBoxID(model.AfterTokenBoxA.strBoxID);
//	m_labelGroupFive[9].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[9].name5	= TransforCount(model.AfterTokenBoxA.nQuantity);
//
//	m_labelGroupFive[10].name2	= _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[10].name3	= TransforBoxID(model.AfterTokenBoxB.strBoxID);
//	m_labelGroupFive[10].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[10].name5	= TransforCount(model.AfterTokenBoxB.nQuantity);
//
//	m_labelGroupFive[11].name2	= _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[11].name3	= TransforBoxID(model.AfterInvalidTokenBox.strBoxID);
//	m_labelGroupFive[11].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[11].name5	= TransforCount(model.AfterInvalidTokenBox.nQuantity);
//
//	m_labelGroupFive[12].name2	= _T("������ı��:");
//	m_labelGroupFive[12].name3	= TransforBoxID(model.AfterCollectedTokenBox.strBoxID);
//	m_labelGroupFive[12].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[12].name5	= TransforCount(model.AfterCollectedTokenBox.nQuantity);
//
//	m_labelGroupFive[13].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[13].name3	= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//	m_labelGroupFive[13].name4	= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[13].name5	= TransforCount(model.AfterTokenHopperA.nQuantity);
//
//	m_labelGroupFive[14].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[14].name3	= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//	m_labelGroupFive[14].name4	= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");	
//	m_labelGroupFive[14].name5	= TransforCount(model.AfterTokenHopperB.nQuantity);
//
//	// ��ʾ��ɫ
//	if(model.AfterTokenBoxA.bIsSelected && model.AfterTokenBoxA.emStatus != CTokenManageSvc::STATUS_INSTALL_ERROR){
//		m_baseInfoArea->SetLabelColor(9,1,RED);
//		m_baseInfoArea->SetLabelColor(9,2,RED);
//		m_baseInfoArea->SetLabelColor(9,3,RED);
//		m_baseInfoArea->SetLabelColor(9,4,RED);
//	}
//
//	if(model.AfterTokenBoxB.bIsSelected && model.AfterTokenBoxB.emStatus != CTokenManageSvc::STATUS_INSTALL_ERROR){
//		m_baseInfoArea->SetLabelColor(10,1,RED);
//		m_baseInfoArea->SetLabelColor(10,2,RED);
//		m_baseInfoArea->SetLabelColor(10,3,RED);
//		m_baseInfoArea->SetLabelColor(10,4,RED);
//	}
//
//	if(model.AfterInvalidTokenBox.bIsSelected){
//		m_baseInfoArea->SetLabelColor(11,1,RED);
//		m_baseInfoArea->SetLabelColor(11,2,RED);
//		m_baseInfoArea->SetLabelColor(11,3,RED);
//		m_baseInfoArea->SetLabelColor(11,4,RED);
//	}
//
//	if(model.AfterCollectedTokenBox.bIsSelected){
//		m_baseInfoArea->SetLabelColor(12,1,RED);
//		m_baseInfoArea->SetLabelColor(12,2,RED);
//		m_baseInfoArea->SetLabelColor(12,3,RED);
//		m_baseInfoArea->SetLabelColor(12,4,RED);
//	}
//
//	if(model.AfterTokenHopperA.bIsSelected){
//		m_baseInfoArea->SetLabelColor(13,1,RED);
//		m_baseInfoArea->SetLabelColor(13,2,RED);
//		m_baseInfoArea->SetLabelColor(13,3,RED);
//		m_baseInfoArea->SetLabelColor(13,4,RED);;
//	}
//
//	if(model.AfterTokenHopperB.bIsSelected){
//		m_baseInfoArea->SetLabelColor(14,1,RED);
//		m_baseInfoArea->SetLabelColor(14,2,RED);
//		m_baseInfoArea->SetLabelColor(14,3,RED);
//		m_baseInfoArea->SetLabelColor(14,4,RED);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾж�ؽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpUninstallDlg(){
//	m_labelGroupFive[0].name1		= _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[0].name2		= TransforBoxID(model.AfterTokenBoxA.strBoxID);
//	m_labelGroupFive[0].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[0].name4		= TransforCount(model.AfterTokenBoxA.nQuantity);
//	m_labelGroupFive[0].name5		= TransforStatus(model.AfterTokenBoxA.emStatus);
//
//	m_labelGroupFive[1].name1		= _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[1].name2		= TransforBoxID(model.AfterTokenBoxB.strBoxID);
//	m_labelGroupFive[1].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[1].name4		= TransforCount(model.AfterTokenBoxB.nQuantity);
//	m_labelGroupFive[1].name5		= TransforStatus(model.AfterTokenBoxB.emStatus);
//
//	m_labelGroupFive[2].name1		= _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[2].name2		= TransforBoxID(model.AfterInvalidTokenBox.strBoxID);
//	m_labelGroupFive[2].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[2].name4		= TransforCount(model.AfterInvalidTokenBox.nQuantity);
//	m_labelGroupFive[2].name5		= TransforStatus(model.AfterInvalidTokenBox.emStatus);
//
//	m_labelGroupFive[3].name1		= CString(_T("������ı��")) + _T(":");
//	m_labelGroupFive[3].name2		= TransforBoxID(model.AfterCollectedTokenBox.strBoxID);
//	m_labelGroupFive[3].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[3].name4		= TransforCount(model.AfterCollectedTokenBox.nQuantity);
//	m_labelGroupFive[3].name5		= TransforStatus(model.AfterCollectedTokenBox.emStatus);
//
//	m_labelGroupFive[4].name1		= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[4].name2		= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//	m_labelGroupFive[4].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[4].name4		= TransforCount(model.AfterTokenHopperA.nQuantity);
//
//	m_labelGroupFive[5].name1		= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[5].name2		= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//	m_labelGroupFive[5].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[5].name4		= TransforCount(model.AfterTokenHopperB.nQuantity);
//
//
//	m_labelGroupFive[7].name1		= _T("[1]") + _opl(TXT_TOKENMGR_DETACH_TOKENA);
//	m_labelGroupFive[8].name1		= _T("[2]") + _opl(TXT_TOKENMGR_DETACH_TOKENB);
//	m_labelGroupFive[9].name1		= _T("[3]") + _opl(TXT_TOKENMGR_DETACH_WASTED);
//	m_labelGroupFive[10].name1		= _T("[4]ж�ػ�����");
//	m_labelGroupFive[11].name1		= _T("[5]") + _opl(TXT_TOKENMGR_DETACH_ALL);
//
//	// �ı���ɫ
//	if(model.AfterTokenBoxA.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//		m_baseInfoArea->SetLabelColor(0,4,GREEN);
//	}
//	else{
//		m_baseInfoArea->SetLabelColor(0,4,RED);
//	}
//
//	if(model.AfterTokenBoxB.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//		m_baseInfoArea->SetLabelColor(1,4,GREEN);
//	}
//	else{
//		m_baseInfoArea->SetLabelColor(1,4,RED);
//	}
//
//	if(model.AfterInvalidTokenBox.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//		m_baseInfoArea->SetLabelColor(2,4,GREEN);
//	}
//	else{
//		m_baseInfoArea->SetLabelColor(2,4,RED);
//	}
//
//	if(model.AfterCollectedTokenBox.emStatus == CTokenManageSvc::STATUS_INSTALLED){
//		m_baseInfoArea->SetLabelColor(3,4,GREEN);
//	}
//	else{
//		m_baseInfoArea->SetLabelColor(3,4,RED);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾж����ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpUninstalledDlg(){
//	/********************************ж��ǰ******************************************************/
//	m_labelGroupFive[0].name1	= _opl(TXT_TOKENMGR_BEFOR_UNINSTALL);
//
//	m_labelGroupFive[1].name2	= _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[1].name3	= TransforBoxID(model.BeforeTokenBoxA.strBoxID);
//	m_labelGroupFive[1].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[1].name5	= TransforCount(model.BeforeTokenBoxA.nQuantity);
//
//	m_labelGroupFive[2].name2	= _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[2].name3	= TransforBoxID(model.BeforeTokenBoxB.strBoxID);
//	m_labelGroupFive[2].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[2].name5	= TransforCount(model.BeforeTokenBoxB.nQuantity);
//
//	m_labelGroupFive[3].name2	= _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[3].name3	= TransforBoxID(model.BeforeInvalidTokenBox.strBoxID);
//	m_labelGroupFive[3].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[3].name5	= TransforCount(model.BeforeInvalidTokenBox.nQuantity);
//
//	m_labelGroupFive[4].name2	= _T("������ı��:");
//	m_labelGroupFive[4].name3	= TransforBoxID(model.BeforeCollectedTokenBox.strBoxID);
//	m_labelGroupFive[4].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[4].name5	= TransforCount(model.BeforeCollectedTokenBox.nQuantity);
//
//	m_labelGroupFive[5].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[5].name3	= TransforBoxID(model.BeforeTokenHopperA.strBoxID);
//	m_labelGroupFive[5].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[5].name5	= TransforCount(model.BeforeTokenHopperA.nQuantity);
//
//	m_labelGroupFive[6].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[6].name3	= TransforBoxID(model.BeforeTokenHopperB.strBoxID);
//	m_labelGroupFive[6].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[6].name5	= TransforCount(model.BeforeTokenHopperB.nQuantity);
//
//	/********************************ж�غ�******************************************************/
//	m_labelGroupFive[8].name1	= _opl(TXT_TOKENMGR_AFTER_UNINSTALL);
//
//	m_labelGroupFive[9].name2	= _opl(TXT_TOKENMGR_TOKENA_ID) + _T(":");
//	m_labelGroupFive[9].name3	= TransforBoxID(model.AfterTokenBoxA.strBoxID);
//	m_labelGroupFive[9].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[9].name5	= TransforCount(model.AfterTokenBoxA.nQuantity);
//
//	m_labelGroupFive[10].name2	= _opl(TXT_TOKENMGR_TOKENB_ID) + _T(":");
//	m_labelGroupFive[10].name3	= TransforBoxID(model.AfterTokenBoxB.strBoxID);
//	m_labelGroupFive[10].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[10].name5	= TransforCount(model.AfterTokenBoxB.nQuantity);
//
//	m_labelGroupFive[11].name2	= _opl(TXT_TOKENMGR_WASTED_ID) + _T(":");
//	m_labelGroupFive[11].name3	= TransforBoxID(model.AfterInvalidTokenBox.strBoxID);
//	m_labelGroupFive[11].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[11].name5	= TransforCount(model.AfterInvalidTokenBox.nQuantity);
//
//	m_labelGroupFive[12].name2	= _T("������ı��:");
//	m_labelGroupFive[12].name3	= TransforBoxID(model.AfterCollectedTokenBox.strBoxID);
//	m_labelGroupFive[12].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[12].name5	= TransforCount(model.AfterCollectedTokenBox.nQuantity);
//
//	m_labelGroupFive[13].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[13].name3	= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//	m_labelGroupFive[13].name4	= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[13].name5	= TransforCount(model.AfterTokenHopperA.nQuantity);
//
//	m_labelGroupFive[14].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[14].name3	= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//	m_labelGroupFive[14].name4	= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");	
//	m_labelGroupFive[14].name5	= TransforCount(model.AfterTokenHopperB.nQuantity);
//
//
//	// ��ʾ��ɫ
//	if(model.AfterTokenBoxA.bIsSelected && model.AfterTokenBoxA.emStatus != CTokenManageSvc::STATUS_UNINSTALL_ERROR){
//		m_baseInfoArea->SetLabelColor(9,1,RED);
//		m_baseInfoArea->SetLabelColor(9,2,RED);
//		m_baseInfoArea->SetLabelColor(9,3,RED);
//		m_baseInfoArea->SetLabelColor(9,4,RED);
//	}
//
//	if(model.AfterTokenBoxB.bIsSelected && model.AfterTokenBoxB.emStatus != CTokenManageSvc::STATUS_UNINSTALL_ERROR){
//		m_baseInfoArea->SetLabelColor(10,1,RED);
//		m_baseInfoArea->SetLabelColor(10,2,RED);
//		m_baseInfoArea->SetLabelColor(10,3,RED);
//		m_baseInfoArea->SetLabelColor(10,4,RED);
//	}
//
//	if(model.AfterInvalidTokenBox.bIsSelected){
//		m_baseInfoArea->SetLabelColor(11,1,RED);
//		m_baseInfoArea->SetLabelColor(11,2,RED);
//		m_baseInfoArea->SetLabelColor(11,3,RED);
//		m_baseInfoArea->SetLabelColor(11,4,RED);
//	}
//
//	if(model.AfterCollectedTokenBox.bIsSelected){
//		m_baseInfoArea->SetLabelColor(12,1,RED);
//		m_baseInfoArea->SetLabelColor(12,2,RED);
//		m_baseInfoArea->SetLabelColor(12,3,RED);
//		m_baseInfoArea->SetLabelColor(12,4,RED);
//	}
//
//	if(model.AfterTokenHopperA.bIsSelected){
//		m_baseInfoArea->SetLabelColor(13,1,RED);
//		m_baseInfoArea->SetLabelColor(13,2,RED);
//		m_baseInfoArea->SetLabelColor(13,3,RED);
//		m_baseInfoArea->SetLabelColor(13,4,RED);;
//	}
//
//	if(model.AfterTokenHopperB.bIsSelected){
//		m_baseInfoArea->SetLabelColor(14,1,RED);
//		m_baseInfoArea->SetLabelColor(14,2,RED);
//		m_baseInfoArea->SetLabelColor(14,3,RED);
//		m_baseInfoArea->SetLabelColor(14,4,RED);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾ���ս���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpRecycleDlg(){
//	m_labelGroupFive[0].name1		= _opl(TXT_TOKENMGR_RECYCLE_TOKEN);
//
//	CTokenManageSvc::Model tempModel = pService->GetDataModel();
//
//	m_labelGroupFive[1].name1		= _T("[1]");
//	m_labelGroupFive[1].name1		+= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[1].name2		= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//	m_labelGroupFive[1].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[1].name4		= TransforCount(model.AfterTokenHopperA.nQuantity);
//	if (tempModel.AfterTokenBoxA.bIsSelected){
//		m_labelGroupFive[1].name5	= _opl(TH_TEST_TEXT_TO_BE_RECOVERED);
//		m_baseInfoArea->SetLabelColor(1,4,RED);
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//	}
//	else{
//		m_labelGroupFive[1].name5	= _T("");
//		m_baseInfoArea->SetLabelColor(1,4,BLACK);
//	}
//
//
//	m_labelGroupFive[2].name1		= _T("[2]");
//	m_labelGroupFive[2].name1		+= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[2].name2		= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//	m_labelGroupFive[2].name3		= _opl(TXT_TOKENMGR_QUANTITY) + _T(":");
//	m_labelGroupFive[2].name4		= TransforCount(model.AfterTokenHopperB.nQuantity);
//	if (tempModel.AfterTokenBoxB.bIsSelected){
//		m_labelGroupFive[2].name5	= _opl(TH_TEST_TEXT_TO_BE_RECOVERED);
//		m_baseInfoArea->SetLabelColor(2,4,RED);
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//	}
//	else{
//		m_labelGroupFive[2].name5	= _T("");
//		m_baseInfoArea->SetLabelColor(2,4,BLACK);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��ʾ������ɽ���
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ShowUpRecycledDlg(){
//	// ����ǰ
//	m_labelGroupFive[0].name1	= _opl(TXT_TOKENMGR_BEFORE_RECYCLE);
//
//	m_labelGroupFive[1].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[1].name3	= TransforBoxID(model.BeforeTokenHopperA.strBoxID);
//	m_labelGroupFive[1].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[1].name5	= TransforCount(model.BeforeTokenHopperA.nQuantity);
//
//	m_labelGroupFive[2].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[2].name3	= TransforBoxID(model.BeforeTokenHopperB.strBoxID);
//	m_labelGroupFive[2].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[2].name5	= TransforCount(model.BeforeTokenHopperB.nQuantity);
//
//	m_labelGroupFive[3].name2	= _T("������ı��:");
//	m_labelGroupFive[3].name3	= TransforBoxID(model.BeforeCollectedTokenBox.strBoxID);
//	m_labelGroupFive[3].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[3].name5	= TransforCount(model.BeforeCollectedTokenBox.nQuantity);
//
//	// ���պ�
//	m_labelGroupFive[6].name1	= _opl(TXT_TOKENMGR_AFTER_RECYCLE);
//
//	m_labelGroupFive[7].name2	= _opl(TXT_TOKENMGR_HOPPER_A_ID) + _T(":");
//	m_labelGroupFive[7].name3	= TransforBoxID(model.AfterTokenHopperA.strBoxID);
//	m_labelGroupFive[7].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[7].name5	= TransforCount(model.AfterTokenHopperA.nQuantity);
//
//	m_labelGroupFive[8].name2	= _opl(TXT_TOKENMGR_HOPPER_B_ID) + _T(":");
//	m_labelGroupFive[8].name3	= TransforBoxID(model.AfterTokenHopperB.strBoxID);
//	m_labelGroupFive[8].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[8].name5	= TransforCount(model.AfterTokenHopperB.nQuantity);
//
//	m_labelGroupFive[9].name2	= _T("������ı��:");
//	m_labelGroupFive[9].name3	= TransforBoxID(model.AfterCollectedTokenBox.strBoxID);
//	m_labelGroupFive[9].name4	= _opl(TXT_TOKENMGR_QUANTITY)+ _T(":");	
//	m_labelGroupFive[9].name5	= TransforCount(model.AfterCollectedTokenBox.nQuantity);
//
//	// �ı���ɫ
//	if(model.AfterTokenHopperA.bIsSelected && model.AfterTokenHopperA.emStatus != CTokenManageSvc::STATUS_RECYCLE_ERROR){
//		m_baseInfoArea->SetLabelColor(7,1,RED);
//		m_baseInfoArea->SetLabelColor(7,2,RED);
//		m_baseInfoArea->SetLabelColor(7,3,RED);
//		m_baseInfoArea->SetLabelColor(7,4,RED);
//	}
//
//	if(model.AfterTokenHopperB.bIsSelected && model.AfterTokenHopperB.emStatus != CTokenManageSvc::STATUS_RECYCLE_ERROR){
//		m_baseInfoArea->SetLabelColor(8,1,RED);
//		m_baseInfoArea->SetLabelColor(8,2,RED);
//		m_baseInfoArea->SetLabelColor(8,3,RED);
//		m_baseInfoArea->SetLabelColor(8,4,RED);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      �����ʾ��������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenBoxInfoDlg::ClearLabelData(){
//	for (int nRow = 0; nRow < MAX_ROW; ++nRow)
//	{
//		m_labelGroupFive[nRow].name1	= _T("");
//		m_labelGroupFive[nRow].name2	= _T("");
//		m_labelGroupFive[nRow].name3	= _T("");
//		m_labelGroupFive[nRow].name4	= _T("");
//		m_labelGroupFive[nRow].name5	= _T("");
//
//		for(int nCol = 0 ; nCol < MAX_COL; ++nCol){
//			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
//		}
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ת������IDΪ��ʾ����
//
//@param      const CString strID;��Ҫת����ID
//
//@retval     CString ת�����ID
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTokenBoxInfoDlg::TransforBoxID(const CString strID){
//	CString strReturn;
//	strReturn = PRINT_ID;
//	if(strID.CompareNoCase(INVALID_ID) != 0)
//		strReturn	= strID;
//	return strReturn;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ת�����ӵ�״̬
//
//@param      const CTokenManageSvc::TOKEN_BOX_STATUS status;���ӵ�״̬
//
//@retval     CString ����ʾ�����ӵ�״̬
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTokenBoxInfoDlg::TransforStatus(const CTokenManageSvc::TOKEN_BOX_STATUS status){
//	switch(status){
//	case CTokenManageSvc::STATUS_INSTALLED:
//		return _opl(TXT_TOKENMGR_ATTACHED);
//	case CTokenManageSvc::STATUS_UNINSTALL:
//		return _opl(TXT_TOKENMGR_DETACHED);
//	case CTokenManageSvc::STATUS_READY_INSTALL:
//		return _opl(TXT_TOKENMGR_WAITTOATTACH);
//	case CTokenManageSvc::STATUS_READY_UNINSTALL:
//		return _opl(TXT_TOKENMGR_WAITTODETACH);
//	default:
//		return _T("");
//	}
//}