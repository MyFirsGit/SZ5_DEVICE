LANGUAGE_DATA
LANGUAGE_DATA
// CBaseM1InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TVM.h"
#include "CBaseM1InfoDlg.h"
#include "afxdialogex.h"
#include "AnalyzeSvc.h"
#include "ACCCardInfo.h"
#include "TicketHelper.h"
#include "RechargeRWMonitorSvc.h"

// CBaseM1InfoDlg dialog
const static TCHAR* const MSG_ANALYZE_BASEINFO_DATA = _T("MSG_ANALYZE_BASEINFO_DATA");
const static TCHAR* const MSG_ANALYZE_HISTORY_DATA = _T("MSG_ANAYLIZE_HISTORY_DATA");
const static TCHAR* const MSG_ANALYZE_PERSONAL_DATA = _T("MSG_ANALYZE_PERSONAL_DATA");
const static TCHAR* const MSG_PRODUCT_TYPE_SVT = _T("MSG_PRODUCT_TYPE_SVT");
const static TCHAR* const MSG_PRODUCT_TYPE_PST = _T("MSG_PRODUCT_TYPE_PST");
const static TCHAR* const MSG_PRODUCT_TYPE_MST = _T("MSG_PRODUCT_TYPE_MST");

#define MAX_HISTORY_RECORD_COUNT   11  // ��ʷ��¼����
#define MAX_HISTORY_TYPE_COUNT		4/*5*/   // ��ʷ��¼����
#define TABLE_ITEM_HEIGHT			60  // ���ÿ�и߶�
#define TABLE_ITEM_WIDTH			180/*150 */// ���ÿ����

IMPLEMENT_DYNAMIC(CBaseM1InfoDlg, CReceptionDlg)
BEGIN_MESSAGE_MAP(CBaseM1InfoDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID, OnServiceSelect)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID, OnProductSelect)
	ON_BTN_BACK_CLICK(OnCancel)
	ON_BTN_OK_CLICK(OnCancel)

END_MESSAGE_MAP()

CBaseM1InfoDlg::CBaseM1InfoDlg(CService* pService)
	: CReceptionDlg(CBaseM1InfoDlg::IDD, pService)
{
	m_pSvc = (CAnalyzeSvc*)pService;
	m_baseInfo->point = CPoint(5,0);
	m_baseInfo->stytle = ANYLE_INFO_STYLE;
	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_BASE_IONFO);
	m_baseInfo->detailInfo.labelGroup = m_BaseInfoGroup;
	m_baseInfo->detailInfo.row = UD_MAX_WIDEROWS-2;

	HistoryInfo = NULL;
	m_historyInfoArea = NULL;
	HistoryInfo = new GSTATIC_INFO;
	HistoryInfo->point = CPoint(955,0);
	HistoryInfo->stytle = ANYLE_INFO_STYLE;
	HistoryInfo->titleInfo.titleName = _tl(TXT_ANALYZE_HISTORY_RECORD);
	HistoryInfo->detailInfo.labelGroup = m_HistoryInfoGroup;
	HistoryInfo->detailInfo.row = UD_MAX_WIDEROWS-2;
	m_pTable =  new CGridCtrl();
	isHistoryBtnClicked = true;
}

CBaseM1InfoDlg::~CBaseM1InfoDlg()
{
	delete m_pTable;
	m_pTable =  NULL;
	if (HistoryInfo != NULL)
	{
		delete HistoryInfo;
		HistoryInfo = NULL;
	}
	if (m_historyInfoArea != NULL)
	{
		delete m_historyInfoArea;
		m_historyInfoArea = NULL;
	}
}

void CBaseM1InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//             0:�����ɹ�    -1:����ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
int CBaseM1InfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

    CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
    try{    		
		/*InitButtonGroup();*/
		isHistoryBtnClicked = false;
		if (__super::OnCreate(lpCreateStruct, BASE_AREA|BTN_OK) == -1) {
			return -1;
		}
		
		CRect rectOK = CRect(850,830,1070,900);
		m_btnOK->MoveWindow(&rectOK, FALSE);
    }
    // �쳣����
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
         theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      ���»��溯��
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::UpdateUI()
{
	CMessagePictureBox::End();
	m_baseInfoArea->Invalidate();
	ShowCompoment(BTN_OK);

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::preShow()
{
	__super::preShow();
	m_btnOK->SetSelect(false);
	HideCompoment(BTN_OK);
	CAnalyzeSvc::DataModel model = m_pSvc->GetDataModel();

	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_BASE_IONFO);
	HistoryInfo->titleInfo.titleName = _tl(TXT_ANALYZE_HISTORY_RECORD);
	if (model.accCard->v_ProductInfo.size() > 0)
	{
		ShowBaseInfo( (CardType_t)model.rwReadResponse.CardPHType , model.accCard->v_ProductInfo[0]);
	}
	//CMessagePictureBox::Show(	theAPP_SESSION.GetServiceName(SERVICE_ID::ANALYZE_SVC),
	//	theAPP_SESSION.GetCurrentClientLanguageType() == LANGUAGETYPE_T::CHINESE?_T("��ѯ�У����Ժ�..."):_T("Analyzing,Please wait..."),
	//	TVM_TIP_WARINGS,
	//	CMessagePictureBox::BTN_OK,DT_LEFT,
	//	30);

}
////////////////////////////////////////////////////////////////////////////
///**
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
void CBaseM1InfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
	isHistoryBtnClicked = false;
	//InitButtonGroup();
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʼ���Ҳఴť������
//
//@param     
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::InitButtonGroup()
{
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();

	//m_vec_Right_1_BTN_INFO.clear();
	//BTN_INFO btnInfo;
	//btnInfo.text=_tl(TXT_ANALYZE_BASE_IONFO);    //  ������Ϣ 
	//btnInfo.btnMsg = MSG_ANALYZE_BASEINFO_DATA;
	//btnInfo.isSelected = true;
	//m_vec_Right_1_BTN_INFO.push_back(btnInfo);

	//if ( data_model.rwReadResponse.CardPHType == CARD_TYPE_ACC_UL )
	//{
	//	//�����UL�� �Ҳ�ֻ��ʾ һ����ť�飬��ֻ��ʾ��������Ϣ��һ����ť
	//	m_Right_1_Group->ShowWindow(SW_HIDE);
	//	m_Right_2_Group->ShowWindow(SW_HIDE);

	//	return;
	//}
	//m_Right_1_Group->ShowWindow(SW_SHOW);
	//m_Right_2_Group->ShowWindow(SW_SHOW);
	//btnInfo.text=_tl(TXT_ANALYZE_HISTORY_RECORD);//  ��ʷ��¼
	//btnInfo.isSelected = false;
	//btnInfo.btnMsg = MSG_ANALYZE_HISTORY_DATA;
	//m_vec_Right_1_BTN_INFO.push_back(btnInfo);
	//if(data_model.accCard->applicationData.byPersonalizeStatus == 1)
	//{
	//	btnInfo.text=_tl(TXT_ANALYZE_PRIVATE_INFO); //  ������Ϣ
	//	btnInfo.btnMsg = MSG_ANALYZE_PERSONAL_DATA;
	//	btnInfo.isSelected = false;
	//	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
	//}
	//m_Right_1_Group->SetButtonTitle(_tl(TXT_ANALYZE_SELECT_QUERY));
	//m_Right_1_Group->ShowButtons(true);
	//m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);


	//m_vec_Right_2_BTN_INFO.clear();             //�Ҳ�ڶ�����ť��
	//int  nCols = data_model.accCard->applicationData.byProductsNum;
	//int  nRows = nCols % 3 == 0 ? nCols / 3  : nCols / 3 + 1;
	//int nIndex = 0;
	//for( vector<CACCCard::ACCCARD_PRODUCT_INFO>::iterator iter = data_model.accCard->v_ProductInfo.begin();
	//	iter != data_model.accCard->v_ProductInfo.end(); ++iter)
	//{
	//	LANGUAGETYPE_T tp = theMAINTENANCE_INFO.GetLanguageMode();
	//	btnInfo.text = theACC_PRODUCT.GetProductName((*iter).productType,theAPP_SESSION.GetCurrentClientLanguageType());
	//	btnInfo.btnID = nIndex;
	//	if (nIndex == 0)
	//	{
	//		btnInfo.isSelected = true;
	//	}
	//	else
	//		btnInfo.isSelected = false;
	//	m_vec_Right_2_BTN_INFO.push_back(btnInfo);

	//	nIndex ++;
	//}  
	//m_Right_2_Group->SetButtonTitle(_tl(TXT_ANALYZE_SELECT_PRODUCT));
	//m_Right_2_Group->ShowButtons(true);
	//m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ѡ����񡱵İ�ť��Ӧ����
//
//@param      (i)WPARAM wParam    ��ʹ��
//@param      (i)LPARAM lParam    ��ʹ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CBaseM1InfoDlg::OnServiceSelect(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO *btn_info = (BTN_INFO*)lParam;
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();
	m_baseInfoArea->Invalidate();
	if(btn_info->btnMsg == MSG_ANALYZE_BASEINFO_DATA) //������Ϣ����
	{
		isHistoryBtnClicked = false;
		//if(m_Right_2_Group !=  NULL && (m_Right_2_Group->GetSelectedBtnInfo().size() > 0))
		//{
		//	//�����Ӳ�Ʒ������£�ֱ�ӷ��Ͱ�ť�������Ϣ
		//	m_Right_2_Group->ClickButton(m_Right_2_Group->GetSelectedBtnInfo()[0].btnID);
		//}
		//else if(data_model.accCard->v_ProductInfo.size() > 0)
		//	ShowBaseInfo((CardType_t)data_model.rwReadResponse.CardPHType/*CARD_TYPE_ACC_UL*/, data_model.accCard->v_ProductInfo[0]);
	}
	if(btn_info->btnMsg == MSG_ANALYZE_PERSONAL_DATA) //������Ϣ���� 
	{
		isHistoryBtnClicked = false;
		UpdatePrivateInfo();
	}
	if(btn_info->btnMsg == MSG_ANALYZE_HISTORY_DATA) //��ʷ��¼
	{
		UpdateHistoryInfo();
	}
	//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
	//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// ������ֵ��д�����
	//}	
	return 1;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����Ʒѡ�񡱰�ť�����Ӧ����
//
//@param      (i)WPARAM wParam      ��ʹ��
//@param      (i)LPARAM lParam      ��ʹ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CBaseM1InfoDlg::OnProductSelect(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO* pBtn = (BTN_INFO*) lParam;
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();
	if(data_model.accCard->v_ProductInfo.size() > 0)
		ShowBaseInfo((CardType_t)data_model.rwReadResponse.CardPHType/*CARD_TYPE_ACC_M1*/, m_pSvc->GetDataModel().accCard->v_ProductInfo[pBtn->btnID]);
	//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
	//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// ������ֵ��д�����
	//}	
	return 1;

}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���¸�����Ϣ���漰����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::UpdatePrivateInfo()
{
	
	ShowCompoment(m_baseInfoArea);
	//HideCompoment(m_Right_2_Group);
	HideCompoment(m_pTable);
	ClearBaseInfo();
	m_pSvc->GetM1PrivateInfo();
	m_baseInfoArea->Invalidate();

	CAnalyzeSvc::DataModel data = m_pSvc->GetDataModel();

	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_PRIVATE_INFO); //������Ϣ
	m_baseInfo->detailInfo.row = UD_MAX_ANAROWS;
	int i = 0;
	m_BaseInfoGroup[i].nameLeft = _tl(TXT_ANALYZE_PERSONAL_NAME);    // ����

	m_BaseInfoGroup[i].nameRight = data.userInfo.strName;
	m_BaseInfoGroup[++i].nameLeft = _tl(TXT_ANALYZE_IDCard_TYPE);    //�C�����

	m_BaseInfoGroup[i].nameRight = data.userInfo.strIdentityCardType;
	m_BaseInfoGroup[++i].nameLeft = _tl(TXT_ANALYZE_ID);             // �C����̖

	m_BaseInfoGroup[i].nameRight = data.userInfo.strIdentityCardNum;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ������ʷ��¼���漰����
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::UpdateHistoryInfo()
{
	if (isHistoryBtnClicked)
	{
		return;// �����ǰ������ʷ��¼���棬��������Create
	}
	m_historyInfoArea = new CGStatic(this,HistoryInfo);

	//ClearBaseInfo();
	//m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_HISTORY_RECORD);//������ʷ��¼
	CRect rect;
	m_historyInfoArea->GetClientRect(&rect);//ȡ��m_baseInfoArea�ؼ������Rectangle
	//ScreenToClient(&rect); 
	//ShowCompoment(m_baseInfoArea);
	//HideCompoment(m_Right_2_Group);                                //����Right_2_button
	
	CRect recGrid;                                                 //����Grid������ڵ�����
	recGrid.left   = (rect.right - (MAX_HISTORY_TYPE_COUNT) * TABLE_ITEM_WIDTH) / 2 + rect.left;
	recGrid.right  = recGrid.left + (MAX_HISTORY_TYPE_COUNT) * TABLE_ITEM_WIDTH + 2*BORDER_MAX_SIZE;
	recGrid.top    = rect.top + GSTATIC_STYLE_1_TITLE_HEIGHT+20;       //����߶�
	recGrid.bottom = recGrid.top + MAX_HISTORY_RECORD_COUNT * TABLE_ITEM_HEIGHT + 2*BORDER_MAX_SIZE;

	m_pTable->Create(recGrid, m_historyInfoArea, IDC_SELECTED_NO_GRID,WS_CHILD  | WS_TABSTOP | WS_VISIBLE);
	m_pTable->SetBkColor(GRAY);                                    //����ɫ��theGUI_INFO.GetBaseBakColor()  
	m_pTable->SetFixedBkColor(GRAY);
	m_pTable->SetEditable(FALSE);
	//m_pTable->SetItemFont()

	m_pTable->SetFixedRowCount(MAX_HISTORY_RECORD_COUNT);          //10��   ���10����¼
	m_pTable->SetColumnCount(MAX_HISTORY_TYPE_COUNT);              // 6��   6�����͵ļ�¼

	m_pTable->EnableColumnResize(FALSE);
	m_pTable->EnableRowResize(FALSE);
	
	m_pTable->SetItemText(0,0,_tl(TXT_ANALYZE_TRANS_DATETIME));    //       ����ʱ��
	m_pTable->SetColumnWidth(0,200); 
	//m_pTable->SetItemText(0,1,_tl(TXT_ANALYZE_TRANS_LOCATION));    //       ���׵ص�
	//m_pTable->SetColumnWidth(1,220); 
	m_pTable->SetItemText(0,1,_tl(TXT_ANALYZE_TRANS_TYPE));        //			����   
	m_pTable->SetColumnWidth(1,170/*130*/); 
	m_pTable->SetItemText(0,2,_tl(TXT_ANALYZE_TRANS_PRODUCT));     //       ��Ʒ����  
	m_pTable->SetColumnWidth(2,192/*170*/);
	m_pTable->SetItemText(0,3,_tl(TXT_ANALYZE_TRANS_VALUE));       //       ���׽��  
	m_pTable->SetColumnWidth(3,160/*120*/); 
	//m_pTable->SetItemText(0,4,_tl(TXT_ANALYZE_BALANCE));           //       Ʊ�����   
	//m_pTable->SetColumnWidth(4,130); 

	for (int nIndex(0); nIndex < 6; nIndex++)
	{
		m_pTable->SetItemBkColour(0,nIndex,OPERATOR_STATUS_BK_COLOR);	//       ��ͷ��ɫ
	}

	TCHAR fontName[LF_FACESIZE]={0};									//		 ��������
	CString strFontName = theGUI_INFO.GetHistoryGridTxtFontName();
	memcpy(fontName,strFontName.GetBuffer(),strFontName.GetLength()>LF_FACESIZE* sizeof(TCHAR)?LF_FACESIZE:strFontName.GetLength()* sizeof(TCHAR));
	strFontName.ReleaseBuffer();
	LOGFONT defFont = {theGUI_INFO.GetHistoryGridTxtFontSize(),
						0,0,0,FW_NORMAL,
						0,0,0,CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES,PROOF_QUALITY,
						DEFAULT_PITCH|FF_SWISS,_T("")};
	memcpy(defFont.lfFaceName,fontName,LF_FACESIZE);
	const LOGFONT * lf = &defFont;

	for(int i(0); i< 11; i++)
	{
			m_pTable->SetRowHeight(i,60);									//       ÿ�и߶�
		for (int j(0);j< 6; j++)
		{
			m_pTable->SetItemFont(i,j, lf);								//		 ��������
			m_pTable->SetTextColor(theGUI_INFO.GetHistoryGridTxtFontColor());
			if (i>0)// ��ͷ�ѹ̶� ���ٳ�ʼ��
			{
				m_pTable->SetItemText(i,j, _T(""));
			}
		}
	}		

	int nRow = 1;
	CString  strData;
	_DATE_TIME Time0;
	BYTE bProductCategory = theACC_PRODUCT.GetTicketCategry(ComMakeWORD(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productType,m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productSubType));
	for(vector<CACCCard::CARD_HISTORY_DATA>::iterator iter = m_pSvc->GetDataModel().accCard->historyData.begin();iter != m_pSvc->GetDataModel().accCard->historyData.end(); ++iter){
		int nCol = 0;
		// ����ʱ��
		m_pTable->SetItemText(nRow,nCol++, (*iter).TransTime.ToString(_T("%04d.%02d.%02d %02d:%02d:%02d")));	                                                                             
		// ���׵ص�
		//LocationNumber_t locationNumber = theACC_LOCATION.GetStationLocationNumber((*iter).transStation);
		//m_pTable->SetItemText(nRow,nCol++,theACC_LOCATION.GetLocationName(locationNumber,theAPP_SESSION.GetCurrentClientLanguageType()));	
		// ��������
		m_pTable->SetItemText(nRow,nCol++,theTICKET_HELPER.GetTransactionType((TRANSACTION_TYPE)(*iter).TransType,m_pSvc->GetDataModel().rwReadResponse.CardPHType,ACC));							
		// ��Ʒ����
		//m_pTable->SetItemText(nRow,nCol++,theTICKET_HELPER.GetCategoryName((ProductCategory_t)(*iter).category));	
		// ��Ʒ����
		m_pTable->SetItemText(nRow,nCol++,theACC_PRODUCT.GetProductName(ComMakeWORD(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productType,m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productSubType),theAPP_SESSION.GetCurrentClientLanguageType()));
		// ���׽��
		// Ǯ����Ʒ
		if( bProductCategory == PURSE){
			m_pTable->SetItemText(nRow,nCol++,ComFormatAmount((*iter).TransValue)+_tl(MONEY_UNIT));		
		}
		// �ƴβ�Ʒ
		else if (bProductCategory == MULTIRIDE){
			m_pTable->SetItemText(nRow,nCol++,ComFormatCount((*iter).TransValue)+_tl(TIMES_UNIT));
		}
		else if(bProductCategory == PURSE_FIRST || bProductCategory == MULTIRIDE_FIRST){
			if((*iter).TransValue<=100){//С�ڵ���100�������ƴΣ�������Ǯ��
				m_pTable->SetItemText(nRow,nCol++,ComFormatCount((*iter).TransValue)+_tl(TIMES_UNIT));
			}
			else{
				m_pTable->SetItemText(nRow,nCol++,ComFormatAmount((*iter).TransValue)+_tl(MONEY_UNIT));		
			}
		}
		else{// ���ڻ�����
			m_pTable->SetItemText(nRow,nCol++,_T("--"));	
		}		

		//// ���׺����
		//if(productcategory == PURSE){
		//	m_pTable->SetItemText(nRow,nCol++, ComFormatAmount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(MONEY_UNIT));
		//}
		//else if(productcategory == MULTIRIDE){	// �ƴ� -- ���
		//	m_pTable->SetItemText(nRow,nCol++, ComFormatCount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(TIMES_UNIT));										
		//}
		//else if(productcategory == PURSE_FIRST || productcategory == MULTIRIDE_FIRST){
		//	if(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance<=100){//С�ڵ���100�������ƴΣ�������Ǯ��
		//		m_pTable->SetItemText(nRow,nCol++, ComFormatCount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(TIMES_UNIT));		
		//	}
		//	else{
		//		m_pTable->SetItemText(nRow,nCol++, ComFormatAmount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(MONEY_UNIT));
		//	}
		//}
		//else{// ���ڻ�����
		//	m_pTable->SetItemText(nRow,nCol++, _T("--"));
		//}
		nRow++;
	}

	lf = NULL;
	isHistoryBtnClicked = true;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief     ���BaseInfo������ʾ������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::ClearBaseInfo()
{
	for (int nIndex = 0; nIndex < UD_MAX_WIDEROWS; nIndex++)
	{
		m_BaseInfoGroup[nIndex].nameLeft = _T("");
		m_BaseInfoGroup[nIndex].nameRight= m_BaseInfoGroup[nIndex].nameLeft;
	}
	
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʾ������Ϣ
//
//@param      (i)CardType_t cardType   Ʊ������
//@param      (i)ACCCARD_PRODUCT_INFO& productInfo ��Ʒ��Ϣ
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::ShowBaseInfo(CardType_t cardType, CACCCard::ACCCARD_PRODUCT_INFO& productInfo)
{

	//��ʾ��Ʒ��������
	// ��ǰ����ģʽ
	LANGUAGETYPE_T lgg = theAPP_SESSION.GetCurrentClientLanguageType();
	ShowCompoment(m_baseInfoArea);  //��ʾ������Ϣ����
	// UL����ԃ���[���҂Ȱ��o�M
	//if (cardType == CARD_TYPE_ACC_UL)	{
	//	HideCompoment(m_Right_2_Group);
	//}
	//else{ // M1K �� ��ʾ�Ҳఴť��
	//	ShowCompoment(m_Right_2_Group); //��ʾ�Ҳ�2 ��ť��
	//}
	// ���Grid�Ѿ�����������
	HideCompoment(m_pTable); 
	// BaseInfo�����������
	ClearBaseInfo();
	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_BASE_IONFO);
	// ȡ����ģ��
	CAnalyzeSvc::DataModel data = m_pSvc->GetDataModel();
	m_baseInfoArea->Invalidate();

	/*ClearBaseInfo();*/
	CAnalyzeSvc::DataModel model = m_pSvc->GetDataModel();
	//���ñ���
	int nRows = 0;//�к�

	//��Ʊ��� �߼�����
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_ID) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight = ComConvToHexStr(model.accCard->applicationData.CardLogicalNumber, sizeof(model.accCard->applicationData.CardLogicalNumber));
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_ID);

	//��Ʒ����
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_TYPE) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theACC_PRODUCT.GetProductName(ComMakeWORD(productInfo.productType,productInfo.productSubType), LANGUAGE_ID_CHINESE);
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_TYPE) + _en(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theACC_PRODUCT.GetProductName(ComMakeWORD(productInfo.productType,productInfo.productSubType), LANGUAGE_ID_ENGLISH);

	//��������
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_EXPIRE_DATE) + _cn(TXT_COLON);
	//��Ч�ھ�ȷ����
	m_BaseInfoGroup[nRows].nameRight = productInfo.ValidEndTime.ToString(_T("%04d-%02d-%02d"));
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_EXPIRE_DATE);

	//�˿�����
	//m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_ANALYZE_PASSENGER) + _tl(TXT_COLON);
	// ���� �����޸ò��� �����ʾ��ȷ��20170227
	//m_BaseInfoGroup[nRows].nameRight  = theACC_BUSINESS.GetPassengerTypeName(model.accCard->applicationData.byPassengerType, theAPP_SESSION.GetCurrentClientLanguageType());

	//Ѻ��
	//if (cardType == CARD_TYPE_ACC_M1)	{
	//	m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_ANALYZE_DEPOSIT) + _tl(TXT_COLON);
	//	m_BaseInfoGroup[nRows].nameRight = ComFormatAmount(model.accCard->applicationData.Deposit) + _tl(MONEY_UNIT);
	//}

	if (productInfo.productType==0x0B && productInfo.productSubType==0x06){//��������˿�
		// ���
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_REMAIN_VALUE);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
		nRows++;
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_REMAIN_VALUE);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
		// ���
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_REMAINING_RIDES);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.nRemainRides) + _cn(TIMES_UNIT);
		nRows++;
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_REMAINING_RIDES);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.nRemainRides) + _en(TIMES_UNIT);	
	}
	else{
		// ��ȡƱ�����
		CString strBalanceCN(_T("")), strBalanceEN(_T(""));
		CString strBalanceLableCN(_T("")), strBalanceLableEN(_T(""));
		BYTE bProductCategory = theACC_PRODUCT.GetTicketCategry(ComMakeWORD(productInfo.productType,productInfo.productSubType));
		// Ǯ����Ʒ
		if( bProductCategory == PURSE)
		{
			// ���
			strBalanceLableCN = _cn(TXT_REMAIN_VALUE);
			strBalanceLableEN = _en(TXT_REMAIN_VALUE);
			strBalanceCN = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
			strBalanceEN = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
		}
		// �ƴβ�Ʒ
		else if (bProductCategory == MULTIRIDE){
			// ���
			strBalanceLableCN = _cn(TXT_REMAINING_RIDES);
			strBalanceLableEN = _en(TXT_REMAINING_RIDES);
			strBalanceCN = ComFormatCount(productInfo.Balance) + _cn(TIMES_UNIT);
			strBalanceEN = ComFormatCount(productInfo.Balance) + _en(TIMES_UNIT);
		}
		else if(bProductCategory == PURSE_FIRST || bProductCategory == MULTIRIDE_FIRST){
			if(productInfo.Balance<=100){//С�ڵ���100�������ƴΣ�������Ǯ��
				// ���
				strBalanceLableCN = _cn(TXT_REMAINING_RIDES);
				strBalanceLableEN = _en(TXT_REMAINING_RIDES);
				strBalanceCN = ComFormatCount(productInfo.Balance) + _cn(TIMES_UNIT);
				strBalanceEN = ComFormatCount(productInfo.Balance) + _en(TIMES_UNIT);	
			}
			else{
				// ���
				strBalanceLableCN = _cn(TXT_REMAIN_VALUE);
				strBalanceLableEN = _en(TXT_REMAIN_VALUE);
				strBalanceCN = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
				strBalanceEN = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
			}
		}
		else{// ���ڻ�����
			// ����ʾ���
		}

		if(strBalanceLableCN != _T("") && strBalanceCN != _T("") ){
			m_BaseInfoGroup[++nRows].nameLeft = strBalanceLableCN;
			m_BaseInfoGroup[nRows].nameRight  = strBalanceCN;
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = strBalanceLableEN;
			m_BaseInfoGroup[nRows].nameRight  = strBalanceEN;
		}
	}
	// M1K Ʊ��������ʼĿ�����δ���
	if ( cardType == CARD_TYPE_ACC_M1 || cardType == CARD_TYPE_ACC_M4 || cardType == CARD_TYPE_ACC_CPU){
		// ��ʼ����
		if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){			
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ORIGIN_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ORIGIN_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_ENGLISH);

		}
		// Ŀ������
		if(productInfo.dwEndStation != 0 && productInfo.dwEndStation != LOCATION_NUMBER_UNSPECIFIED){
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_ENGLISH);
		}
	}
	// ULƱ����ʼĿ��վ�㴦��
	else if (cardType == CARD_TYPE_SY_EPT_UL){
		if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){			
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ORIGIN_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ORIGIN_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_ENGLISH);
		}

		//  Ŀ��վ����Ч
		if (productInfo.dwEndStation != 0 && productInfo.dwEndStation != LOCATION_NUMBER_UNSPECIFIED ){
			if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){
				m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
				m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_CHINESE);
				nRows++;
				m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);
				m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_ENGLISH);
			}
		}
		else{// BUG 2399 Ŀ��վ����Ч��ֱ����ʾ��--��
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight = _T("--");
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);

		}
	}
	

	//Ʊ��״̬,��Ʒ״̬ �߼���M1�����ڲ�Ʒ����
	if(cardType == CARD_TYPE_ACC_M1 || cardType == CARD_TYPE_ACC_M4) //M1 ��
	{
		//Ʊ��״̬
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_STATUS) + _cn(TXT_COLON);
		m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCM1KCardBlockStatusName((CardStatusCode_t_M1)model.accCard->applicationData.TicketStatus, LANGUAGE_ID_CHINESE);
		nRows++;	
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_STATUS) + _en(TXT_COLON);
		m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCM1KCardBlockStatusName((CardStatusCode_t_M1)model.accCard->applicationData.TicketStatus, LANGUAGE_ID_ENGLISH);
	}
	//else if(cardType == 0) //UL�� 
	//{
	//	//Ʊ��״̬
	//	m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_TOKEN_STATUS/*TXT_ANALYZE_TICKET_STATUS*/) + _tl(TXT_COLON);
	//	//m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCULCardBlockStatusName((CardStatusCode_t_UL)model.accCard->applicationData.TicketStatus);
	//}
	// ��һ��ʹ�õص�
	CString szLastUseDate(_T("")),szLastUseStationCN(_T("")),szLastUseStationEN(_T(""));
	szLastUseDate = model.accCard->journeyData.lastTransTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
	m_BaseInfoGroup[++nRows].nameLeft = _T("���ʹ��ʱ��:");
	m_BaseInfoGroup[nRows].nameRight = szLastUseDate;
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _T("Last Trans Time");

	szLastUseStationCN = theACC_LOCATION.GetLocationName(model.accCard->journeyData.dwLastTransStation, LANGUAGE_ID_CHINESE);
	szLastUseStationEN = theACC_LOCATION.GetLocationName(model.accCard->journeyData.dwLastTransStation, LANGUAGE_ID_ENGLISH);
	m_BaseInfoGroup[++nRows].nameLeft = _T("���ʹ��վ��:");
	m_BaseInfoGroup[nRows].nameRight = szLastUseStationCN;
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _T("Last Trans Location");
	m_BaseInfoGroup[nRows].nameRight = szLastUseStationEN;

	//�ó�״̬
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_CARD_STATUS) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCCardJourneyStatusName((ACC_CARD_JOURNEY_STATUS)model.accCard->journeyData.byTravelStatus, LANGUAGE_ID_CHINESE);
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_CARD_STATUS) + _en(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCCardJourneyStatusName((ACC_CARD_JOURNEY_STATUS)model.accCard->journeyData.byTravelStatus, LANGUAGE_ID_ENGLISH);

	UpdateHistoryInfo();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief		ȡ��  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBaseM1InfoDlg::OnCancel(WPARAM wParam,LPARAM lParam){
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();
	return TRUE;
}

