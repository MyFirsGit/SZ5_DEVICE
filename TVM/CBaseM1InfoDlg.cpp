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

#define MAX_HISTORY_RECORD_COUNT   11  // 历史记录条数
#define MAX_HISTORY_TYPE_COUNT		4/*5*/   // 历史记录类型
#define TABLE_ITEM_HEIGHT			60  // 表格每行高度
#define TABLE_ITEM_WIDTH			180/*150 */// 表格每项宽度

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
//@brief      画面创建函数
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//             0:创建成功    -1:创建失败
//
//@exception  无
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
    // 异常处理
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
//@brief      更新画面函数
//
//@param      无
//
//@retval     无
//
//@exception  无
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
@brief      界面显示之前

@param      无

@retval     无

@exception  无
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
	//	theAPP_SESSION.GetCurrentClientLanguageType() == LANGUAGETYPE_T::CHINESE?_T("查询中，请稍后..."):_T("Analyzing,Please wait..."),
	//	TVM_TIP_WARINGS,
	//	CMessagePictureBox::BTN_OK,DT_LEFT,
	//	30);

}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面激活后的处理
//
//@param      (i)UINT nState      不使用
//@param      (i)CWnd* pWndOther  不使用
//@param      (i)BOOL bMinimized  不使用
//
//@retval     无
//
//@exception  无
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
//@brief      初始化右侧按钮组数据
//
//@param     
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::InitButtonGroup()
{
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();

	//m_vec_Right_1_BTN_INFO.clear();
	//BTN_INFO btnInfo;
	//btnInfo.text=_tl(TXT_ANALYZE_BASE_IONFO);    //  基本信息 
	//btnInfo.btnMsg = MSG_ANALYZE_BASEINFO_DATA;
	//btnInfo.isSelected = true;
	//m_vec_Right_1_BTN_INFO.push_back(btnInfo);

	//if ( data_model.rwReadResponse.CardPHType == CARD_TYPE_ACC_UL )
	//{
	//	//如果是UL卡 右侧只显示 一个按钮组，且只显示“基本信息”一个按钮
	//	m_Right_1_Group->ShowWindow(SW_HIDE);
	//	m_Right_2_Group->ShowWindow(SW_HIDE);

	//	return;
	//}
	//m_Right_1_Group->ShowWindow(SW_SHOW);
	//m_Right_2_Group->ShowWindow(SW_SHOW);
	//btnInfo.text=_tl(TXT_ANALYZE_HISTORY_RECORD);//  历史记录
	//btnInfo.isSelected = false;
	//btnInfo.btnMsg = MSG_ANALYZE_HISTORY_DATA;
	//m_vec_Right_1_BTN_INFO.push_back(btnInfo);
	//if(data_model.accCard->applicationData.byPersonalizeStatus == 1)
	//{
	//	btnInfo.text=_tl(TXT_ANALYZE_PRIVATE_INFO); //  个人信息
	//	btnInfo.btnMsg = MSG_ANALYZE_PERSONAL_DATA;
	//	btnInfo.isSelected = false;
	//	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
	//}
	//m_Right_1_Group->SetButtonTitle(_tl(TXT_ANALYZE_SELECT_QUERY));
	//m_Right_1_Group->ShowButtons(true);
	//m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);


	//m_vec_Right_2_BTN_INFO.clear();             //右侧第二个按钮组
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
//@brief      “选择服务”的按钮响应函数
//
//@param      (i)WPARAM wParam    不使用
//@param      (i)LPARAM lParam    不使用
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CBaseM1InfoDlg::OnServiceSelect(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO *btn_info = (BTN_INFO*)lParam;
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();
	m_baseInfoArea->Invalidate();
	if(btn_info->btnMsg == MSG_ANALYZE_BASEINFO_DATA) //基本信息服务
	{
		isHistoryBtnClicked = false;
		//if(m_Right_2_Group !=  NULL && (m_Right_2_Group->GetSelectedBtnInfo().size() > 0))
		//{
		//	//存在子产品的情况下，直接发送按钮点击的消息
		//	m_Right_2_Group->ClickButton(m_Right_2_Group->GetSelectedBtnInfo()[0].btnID);
		//}
		//else if(data_model.accCard->v_ProductInfo.size() > 0)
		//	ShowBaseInfo((CardType_t)data_model.rwReadResponse.CardPHType/*CARD_TYPE_ACC_UL*/, data_model.accCard->v_ProductInfo[0]);
	}
	if(btn_info->btnMsg == MSG_ANALYZE_PERSONAL_DATA) //个人信息服务 
	{
		isHistoryBtnClicked = false;
		UpdatePrivateInfo();
	}
	if(btn_info->btnMsg == MSG_ANALYZE_HISTORY_DATA) //历史记录
	{
		UpdateHistoryInfo();
	}
	//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
	//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// 启动充值读写器监控
	//}	
	return 1;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      “产品选择”按钮组的响应函数
//
//@param      (i)WPARAM wParam      不使用
//@param      (i)LPARAM lParam      不使用
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CBaseM1InfoDlg::OnProductSelect(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO* pBtn = (BTN_INFO*) lParam;
	CAnalyzeSvc::DataModel data_model = m_pSvc->GetDataModel();
	if(data_model.accCard->v_ProductInfo.size() > 0)
		ShowBaseInfo((CardType_t)data_model.rwReadResponse.CardPHType/*CARD_TYPE_ACC_M1*/, m_pSvc->GetDataModel().accCard->v_ProductInfo[pBtn->btnID]);
	//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
	//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// 启动充值读写器监控
	//}	
	return 1;

}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      更新个人信息界面及数据
//
//@param      无
//
//@retval     无
//
//@exception  无
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

	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_PRIVATE_INFO); //個人信息
	m_baseInfo->detailInfo.row = UD_MAX_ANAROWS;
	int i = 0;
	m_BaseInfoGroup[i].nameLeft = _tl(TXT_ANALYZE_PERSONAL_NAME);    // 姓名

	m_BaseInfoGroup[i].nameRight = data.userInfo.strName;
	m_BaseInfoGroup[++i].nameLeft = _tl(TXT_ANALYZE_IDCard_TYPE);    //證件類型

	m_BaseInfoGroup[i].nameRight = data.userInfo.strIdentityCardType;
	m_BaseInfoGroup[++i].nameLeft = _tl(TXT_ANALYZE_ID);             // 證件編號

	m_BaseInfoGroup[i].nameRight = data.userInfo.strIdentityCardNum;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      更新历史记录界面及数据
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::UpdateHistoryInfo()
{
	if (isHistoryBtnClicked)
	{
		return;// 如果当前就是历史记录界面，不再重新Create
	}
	m_historyInfoArea = new CGStatic(this,HistoryInfo);

	//ClearBaseInfo();
	//m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_HISTORY_RECORD);//交易历史记录
	CRect rect;
	m_historyInfoArea->GetClientRect(&rect);//取得m_baseInfoArea控件区域的Rectangle
	//ScreenToClient(&rect); 
	//ShowCompoment(m_baseInfoArea);
	//HideCompoment(m_Right_2_Group);                                //隐藏Right_2_button
	
	CRect recGrid;                                                 //计算Grid表格所在的区域
	recGrid.left   = (rect.right - (MAX_HISTORY_TYPE_COUNT) * TABLE_ITEM_WIDTH) / 2 + rect.left;
	recGrid.right  = recGrid.left + (MAX_HISTORY_TYPE_COUNT) * TABLE_ITEM_WIDTH + 2*BORDER_MAX_SIZE;
	recGrid.top    = rect.top + GSTATIC_STYLE_1_TITLE_HEIGHT+20;       //标题高度
	recGrid.bottom = recGrid.top + MAX_HISTORY_RECORD_COUNT * TABLE_ITEM_HEIGHT + 2*BORDER_MAX_SIZE;

	m_pTable->Create(recGrid, m_historyInfoArea, IDC_SELECTED_NO_GRID,WS_CHILD  | WS_TABSTOP | WS_VISIBLE);
	m_pTable->SetBkColor(GRAY);                                    //背景色：theGUI_INFO.GetBaseBakColor()  
	m_pTable->SetFixedBkColor(GRAY);
	m_pTable->SetEditable(FALSE);
	//m_pTable->SetItemFont()

	m_pTable->SetFixedRowCount(MAX_HISTORY_RECORD_COUNT);          //10行   最近10条记录
	m_pTable->SetColumnCount(MAX_HISTORY_TYPE_COUNT);              // 6列   6个类型的记录

	m_pTable->EnableColumnResize(FALSE);
	m_pTable->EnableRowResize(FALSE);
	
	m_pTable->SetItemText(0,0,_tl(TXT_ANALYZE_TRANS_DATETIME));    //       交易时间
	m_pTable->SetColumnWidth(0,200); 
	//m_pTable->SetItemText(0,1,_tl(TXT_ANALYZE_TRANS_LOCATION));    //       交易地点
	//m_pTable->SetColumnWidth(1,220); 
	m_pTable->SetItemText(0,1,_tl(TXT_ANALYZE_TRANS_TYPE));        //			类型   
	m_pTable->SetColumnWidth(1,170/*130*/); 
	m_pTable->SetItemText(0,2,_tl(TXT_ANALYZE_TRANS_PRODUCT));     //       产品名称  
	m_pTable->SetColumnWidth(2,192/*170*/);
	m_pTable->SetItemText(0,3,_tl(TXT_ANALYZE_TRANS_VALUE));       //       交易金额  
	m_pTable->SetColumnWidth(3,160/*120*/); 
	//m_pTable->SetItemText(0,4,_tl(TXT_ANALYZE_BALANCE));           //       票内余额   
	//m_pTable->SetColumnWidth(4,130); 

	for (int nIndex(0); nIndex < 6; nIndex++)
	{
		m_pTable->SetItemBkColour(0,nIndex,OPERATOR_STATUS_BK_COLOR);	//       表头颜色
	}

	TCHAR fontName[LF_FACESIZE]={0};									//		 创建字体
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
			m_pTable->SetRowHeight(i,60);									//       每行高度
		for (int j(0);j< 6; j++)
		{
			m_pTable->SetItemFont(i,j, lf);								//		 设置字体
			m_pTable->SetTextColor(theGUI_INFO.GetHistoryGridTxtFontColor());
			if (i>0)// 表头已固定 不再初始化
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
		// 交易时间
		m_pTable->SetItemText(nRow,nCol++, (*iter).TransTime.ToString(_T("%04d.%02d.%02d %02d:%02d:%02d")));	                                                                             
		// 交易地点
		//LocationNumber_t locationNumber = theACC_LOCATION.GetStationLocationNumber((*iter).transStation);
		//m_pTable->SetItemText(nRow,nCol++,theACC_LOCATION.GetLocationName(locationNumber,theAPP_SESSION.GetCurrentClientLanguageType()));	
		// 交易类型
		m_pTable->SetItemText(nRow,nCol++,theTICKET_HELPER.GetTransactionType((TRANSACTION_TYPE)(*iter).TransType,m_pSvc->GetDataModel().rwReadResponse.CardPHType,ACC));							
		// 产品种类
		//m_pTable->SetItemText(nRow,nCol++,theTICKET_HELPER.GetCategoryName((ProductCategory_t)(*iter).category));	
		// 产品名称
		m_pTable->SetItemText(nRow,nCol++,theACC_PRODUCT.GetProductName(ComMakeWORD(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productType,m_pSvc->GetDataModel().accCard->v_ProductInfo[0].productSubType),theAPP_SESSION.GetCurrentClientLanguageType()));
		// 交易金额
		// 钱包产品
		if( bProductCategory == PURSE){
			m_pTable->SetItemText(nRow,nCol++,ComFormatAmount((*iter).TransValue)+_tl(MONEY_UNIT));		
		}
		// 计次产品
		else if (bProductCategory == MULTIRIDE){
			m_pTable->SetItemText(nRow,nCol++,ComFormatCount((*iter).TransValue)+_tl(TIMES_UNIT));
		}
		else if(bProductCategory == PURSE_FIRST || bProductCategory == MULTIRIDE_FIRST){
			if((*iter).TransValue<=100){//小于等于100，当作计次；否则当作钱包
				m_pTable->SetItemText(nRow,nCol++,ComFormatCount((*iter).TransValue)+_tl(TIMES_UNIT));
			}
			else{
				m_pTable->SetItemText(nRow,nCol++,ComFormatAmount((*iter).TransValue)+_tl(MONEY_UNIT));		
			}
		}
		else{// 定期或其他
			m_pTable->SetItemText(nRow,nCol++,_T("--"));	
		}		

		//// 交易后余额
		//if(productcategory == PURSE){
		//	m_pTable->SetItemText(nRow,nCol++, ComFormatAmount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(MONEY_UNIT));
		//}
		//else if(productcategory == MULTIRIDE){	// 计次 -- 余次
		//	m_pTable->SetItemText(nRow,nCol++, ComFormatCount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(TIMES_UNIT));										
		//}
		//else if(productcategory == PURSE_FIRST || productcategory == MULTIRIDE_FIRST){
		//	if(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance<=100){//小于等于100，当作计次；否则当作钱包
		//		m_pTable->SetItemText(nRow,nCol++, ComFormatCount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(TIMES_UNIT));		
		//	}
		//	else{
		//		m_pTable->SetItemText(nRow,nCol++, ComFormatAmount(m_pSvc->GetDataModel().accCard->v_ProductInfo[0].Balance/*(*iter).remaingValue*/) + _tl(MONEY_UNIT));
		//	}
		//}
		//else{// 定期或其他
		//	m_pTable->SetItemText(nRow,nCol++, _T("--"));
		//}
		nRow++;
	}

	lf = NULL;
	isHistoryBtnClicked = true;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief     清空BaseInfo区域显示的数据
//
//@param      无
//
//@retval     无
//
//@exception  无
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
//@brief      显示基本信息
//
//@param      (i)CardType_t cardType   票卡类型
//@param      (i)ACCCARD_PRODUCT_INFO& productInfo 产品信息
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseM1InfoDlg::ShowBaseInfo(CardType_t cardType, CACCCard::ACCCARD_PRODUCT_INFO& productInfo)
{

	//显示产品类型区域
	// 当前语言模式
	LANGUAGETYPE_T lgg = theAPP_SESSION.GetCurrentClientLanguageType();
	ShowCompoment(m_baseInfoArea);  //显示基本信息区域
	// UL卡查詢：隱藏右側按鈕組
	//if (cardType == CARD_TYPE_ACC_UL)	{
	//	HideCompoment(m_Right_2_Group);
	//}
	//else{ // M1K 卡 显示右侧按钮组
	//	ShowCompoment(m_Right_2_Group); //显示右侧2 按钮组
	//}
	// 如果Grid已经创建，隐藏
	HideCompoment(m_pTable); 
	// BaseInfo区域数据清空
	ClearBaseInfo();
	m_baseInfo->titleInfo.titleName = _tl(TXT_ANALYZE_BASE_IONFO);
	// 取数据模型
	CAnalyzeSvc::DataModel data = m_pSvc->GetDataModel();
	m_baseInfoArea->Invalidate();

	/*ClearBaseInfo();*/
	CAnalyzeSvc::DataModel model = m_pSvc->GetDataModel();
	//公用变量
	int nRows = 0;//行号

	//车票编号 逻辑卡号
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_ID) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight = ComConvToHexStr(model.accCard->applicationData.CardLogicalNumber, sizeof(model.accCard->applicationData.CardLogicalNumber));
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_ID);

	//产品类型
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_TYPE) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theACC_PRODUCT.GetProductName(ComMakeWORD(productInfo.productType,productInfo.productSubType), LANGUAGE_ID_CHINESE);
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_TYPE) + _en(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theACC_PRODUCT.GetProductName(ComMakeWORD(productInfo.productType,productInfo.productSubType), LANGUAGE_ID_ENGLISH);

	//过期日期
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_EXPIRE_DATE) + _cn(TXT_COLON);
	//有效期精确到天
	m_BaseInfoGroup[nRows].nameRight = productInfo.ValidEndTime.ToString(_T("%04d-%02d-%02d"));
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_EXPIRE_DATE);

	//乘客类型
	//m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_ANALYZE_PASSENGER) + _tl(TXT_COLON);
	// 王峰 重启无该参数 如何显示待确认20170227
	//m_BaseInfoGroup[nRows].nameRight  = theACC_BUSINESS.GetPassengerTypeName(model.accCard->applicationData.byPassengerType, theAPP_SESSION.GetCurrentClientLanguageType());

	//押金
	//if (cardType == CARD_TYPE_ACC_M1)	{
	//	m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_ANALYZE_DEPOSIT) + _tl(TXT_COLON);
	//	m_BaseInfoGroup[nRows].nameRight = ComFormatAmount(model.accCard->applicationData.Deposit) + _tl(MONEY_UNIT);
	//}

	if (productInfo.productType==0x0B && productInfo.productSubType==0x06){//如果是老人卡
		// 余额
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_REMAIN_VALUE);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
		nRows++;
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_REMAIN_VALUE);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
		// 余次
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_REMAINING_RIDES);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.nRemainRides) + _cn(TIMES_UNIT);
		nRows++;
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_REMAINING_RIDES);
		m_BaseInfoGroup[nRows].nameRight  = ComFormatAmount(productInfo.nRemainRides) + _en(TIMES_UNIT);	
	}
	else{
		// 获取票卡余额
		CString strBalanceCN(_T("")), strBalanceEN(_T(""));
		CString strBalanceLableCN(_T("")), strBalanceLableEN(_T(""));
		BYTE bProductCategory = theACC_PRODUCT.GetTicketCategry(ComMakeWORD(productInfo.productType,productInfo.productSubType));
		// 钱包产品
		if( bProductCategory == PURSE)
		{
			// 余额
			strBalanceLableCN = _cn(TXT_REMAIN_VALUE);
			strBalanceLableEN = _en(TXT_REMAIN_VALUE);
			strBalanceCN = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
			strBalanceEN = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
		}
		// 计次产品
		else if (bProductCategory == MULTIRIDE){
			// 余次
			strBalanceLableCN = _cn(TXT_REMAINING_RIDES);
			strBalanceLableEN = _en(TXT_REMAINING_RIDES);
			strBalanceCN = ComFormatCount(productInfo.Balance) + _cn(TIMES_UNIT);
			strBalanceEN = ComFormatCount(productInfo.Balance) + _en(TIMES_UNIT);
		}
		else if(bProductCategory == PURSE_FIRST || bProductCategory == MULTIRIDE_FIRST){
			if(productInfo.Balance<=100){//小于等于100，当作计次；否则当作钱包
				// 余次
				strBalanceLableCN = _cn(TXT_REMAINING_RIDES);
				strBalanceLableEN = _en(TXT_REMAINING_RIDES);
				strBalanceCN = ComFormatCount(productInfo.Balance) + _cn(TIMES_UNIT);
				strBalanceEN = ComFormatCount(productInfo.Balance) + _en(TIMES_UNIT);	
			}
			else{
				// 余额
				strBalanceLableCN = _cn(TXT_REMAIN_VALUE);
				strBalanceLableEN = _en(TXT_REMAIN_VALUE);
				strBalanceCN = ComFormatAmount(productInfo.Balance) + _cn(MONEY_UNIT);
				strBalanceEN = ComFormatAmount(productInfo.Balance) + _en(MONEY_UNIT);
			}
		}
		else{// 定期或其他
			// 不显示余额
		}

		if(strBalanceLableCN != _T("") && strBalanceCN != _T("") ){
			m_BaseInfoGroup[++nRows].nameLeft = strBalanceLableCN;
			m_BaseInfoGroup[nRows].nameRight  = strBalanceCN;
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = strBalanceLableEN;
			m_BaseInfoGroup[nRows].nameRight  = strBalanceEN;
		}
	}
	// M1K 票卡余额和起始目的区段处理
	if ( cardType == CARD_TYPE_ACC_M1 || cardType == CARD_TYPE_ACC_M4 || cardType == CARD_TYPE_ACC_CPU){
		// 起始区段
		if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){			
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ORIGIN_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ORIGIN_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_ENGLISH);

		}
		// 目的区段
		if(productInfo.dwEndStation != 0 && productInfo.dwEndStation != LOCATION_NUMBER_UNSPECIFIED){
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_ENGLISH);
		}
	}
	// UL票卡起始目的站点处理
	else if (cardType == CARD_TYPE_SY_EPT_UL){
		if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){			
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ORIGIN_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_CHINESE);
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ORIGIN_POINT) + _en(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwStartStation, LANGUAGE_ID_ENGLISH);
		}

		//  目的站点有效
		if (productInfo.dwEndStation != 0 && productInfo.dwEndStation != LOCATION_NUMBER_UNSPECIFIED ){
			if(productInfo.dwStartStation != 0 && productInfo.dwStartStation != LOCATION_NUMBER_UNSPECIFIED){
				m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
				m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_CHINESE);
				nRows++;
				m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);
				m_BaseInfoGroup[nRows].nameRight  = theACC_LOCATION.GetLocationName(productInfo.dwEndStation, LANGUAGE_ID_ENGLISH);
			}
		}
		else{// BUG 2399 目的站点无效，直接显示“--”
			m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_DESTINATION_POINT) + _cn(TXT_COLON);
			m_BaseInfoGroup[nRows].nameRight = _T("--");
			nRows++;
			m_BaseInfoGroup[nRows].nameLeft = _en(TXT_DESTINATION_POINT) + _en(TXT_COLON);

		}
	}
	

	//票卡状态,产品状态 逻辑：M1卡存在产品类型
	if(cardType == CARD_TYPE_ACC_M1 || cardType == CARD_TYPE_ACC_M4) //M1 卡
	{
		//票卡状态
		m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_ANALYZE_TICKET_STATUS) + _cn(TXT_COLON);
		m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCM1KCardBlockStatusName((CardStatusCode_t_M1)model.accCard->applicationData.TicketStatus, LANGUAGE_ID_CHINESE);
		nRows++;	
		m_BaseInfoGroup[nRows].nameLeft = _en(TXT_ANALYZE_TICKET_STATUS) + _en(TXT_COLON);
		m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCM1KCardBlockStatusName((CardStatusCode_t_M1)model.accCard->applicationData.TicketStatus, LANGUAGE_ID_ENGLISH);
	}
	//else if(cardType == 0) //UL卡 
	//{
	//	//票卡状态
	//	m_BaseInfoGroup[++nRows].nameLeft = _tl(TXT_TOKEN_STATUS/*TXT_ANALYZE_TICKET_STATUS*/) + _tl(TXT_COLON);
	//	//m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCULCardBlockStatusName((CardStatusCode_t_UL)model.accCard->applicationData.TicketStatus);
	//}
	// 上一次使用地点
	CString szLastUseDate(_T("")),szLastUseStationCN(_T("")),szLastUseStationEN(_T(""));
	szLastUseDate = model.accCard->journeyData.lastTransTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
	m_BaseInfoGroup[++nRows].nameLeft = _T("最近使用时间:");
	m_BaseInfoGroup[nRows].nameRight = szLastUseDate;
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _T("Last Trans Time");

	szLastUseStationCN = theACC_LOCATION.GetLocationName(model.accCard->journeyData.dwLastTransStation, LANGUAGE_ID_CHINESE);
	szLastUseStationEN = theACC_LOCATION.GetLocationName(model.accCard->journeyData.dwLastTransStation, LANGUAGE_ID_ENGLISH);
	m_BaseInfoGroup[++nRows].nameLeft = _T("最近使用站点:");
	m_BaseInfoGroup[nRows].nameRight = szLastUseStationCN;
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _T("Last Trans Location");
	m_BaseInfoGroup[nRows].nameRight = szLastUseStationEN;

	//旅程状态
	m_BaseInfoGroup[++nRows].nameLeft = _cn(TXT_CARD_STATUS) + _cn(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCCardJourneyStatusName((ACC_CARD_JOURNEY_STATUS)model.accCard->journeyData.byTravelStatus, LANGUAGE_ID_CHINESE);
	nRows++;
	m_BaseInfoGroup[nRows].nameLeft = _en(TXT_CARD_STATUS) + _en(TXT_COLON);
	m_BaseInfoGroup[nRows].nameRight  = theTICKET_HELPER.GetACCCardJourneyStatusName((ACC_CARD_JOURNEY_STATUS)model.accCard->journeyData.byTravelStatus, LANGUAGE_ID_ENGLISH);

	UpdateHistoryInfo();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief		取消  

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

