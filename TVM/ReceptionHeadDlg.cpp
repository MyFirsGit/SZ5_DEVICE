// CReceptionHeadDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "TVM.h"
#include "ReceptionHeadDLG.h"
#include <sstream>
#include <iomanip>
#include "util.h"
#include "AppSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CReceptionHeadDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CReceptionHeadDlg, CBaseDlg)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()
static CRect const RECEPTION_HEAD_GIF_LOGO_RECT = CRect(CPoint(20,10+5),CSize(112,84));
static CRect const RECEPTION_HEAD_LOGO_RECT		= CRect(CPoint(20,10),CSize(320,110));			// 乘客业务界面LOGO区域
static CRect const RECEPTION_HEAD_STATUS_RECT	= CRect(1605,5,1905,35);			// 乘客业务界面状态栏区域
static CRect  STATION_NAME_CN_RECT = CRect(CPoint(815,20),CSize(300,70));
static CRect  SERVICE_STATUSE_RECT=CRect(CPoint(1470,5),CSize(400,70));
//static CRect  DATETIME_RECT = CRect(1000,60,1250,100);
static CRect  DATETIME_RECT = CRect(CPoint(1430,60),CSize(400,70));
static CRect  WEEKDAY_CN_RECT = CRect(1090,70,1275,100);
static CRect  DEVICE_ID_RECT = CRect(150,54,450,78);

// 计算乘客画面分辨率
static const CRect REC_HEAD_RECT_RATIO = CRect(REC_HEAD_RECT.left*REC_RATIOWIDTH, REC_HEAD_RECT.top*REC_RATIOHEIGHT,
											   REC_HEAD_RECT.right*REC_RATIOWIDTH, REC_HEAD_RECT.bottom*REC_RATIOHEIGHT);
static CRect STATION_NAME_CN_RECT_RATIO = CRect(STATION_NAME_CN_RECT.left*REC_RATIOWIDTH, STATION_NAME_CN_RECT.top*REC_RATIOHEIGHT,
												STATION_NAME_CN_RECT.right*REC_RATIOWIDTH, STATION_NAME_CN_RECT.bottom*REC_RATIOHEIGHT);
static CRect DEVICE_ID_RECT_RATIO = CRect(DEVICE_ID_RECT.left*REC_RATIOWIDTH, DEVICE_ID_RECT.top*REC_RATIOHEIGHT,
	DEVICE_ID_RECT.right*REC_RATIOWIDTH, DEVICE_ID_RECT.bottom*REC_RATIOHEIGHT);

static CRect SERVICE_STATUSE_RECT_RATIO = CRect(SERVICE_STATUSE_RECT.left*REC_RATIOWIDTH, SERVICE_STATUSE_RECT.top*REC_RATIOHEIGHT,
	                                            SERVICE_STATUSE_RECT.right*REC_RATIOWIDTH, SERVICE_STATUSE_RECT.bottom*REC_RATIOHEIGHT);
static CRect DATETIME_RECT_RATIO = CRect(DATETIME_RECT.left*REC_RATIOWIDTH, DATETIME_RECT.top*REC_RATIOHEIGHT,
										 DATETIME_RECT.right*REC_RATIOWIDTH, DATETIME_RECT.bottom*REC_RATIOHEIGHT);
static CRect WEEKDAY_CN_RECT_RATIO = CRect(WEEKDAY_CN_RECT.left*REC_RATIOWIDTH, WEEKDAY_CN_RECT.top*REC_RATIOHEIGHT,
										   WEEKDAY_CN_RECT.right*REC_RATIOWIDTH, WEEKDAY_CN_RECT.bottom*REC_RATIOHEIGHT);
static CRect RECEPTION_HEAD_LOGO_RECT_RATIO = CRect(RECEPTION_HEAD_LOGO_RECT.left*REC_RATIOWIDTH, RECEPTION_HEAD_LOGO_RECT.top*REC_RATIOHEIGHT,
													RECEPTION_HEAD_LOGO_RECT.right*REC_RATIOWIDTH, RECEPTION_HEAD_LOGO_RECT.bottom*REC_RATIOHEIGHT);
static CRect RECEPTION_HEAD_STATUS_RECT_RATIO = CRect(RECEPTION_HEAD_STATUS_RECT.left*REC_RATIOWIDTH, RECEPTION_HEAD_STATUS_RECT.top*REC_RATIOHEIGHT,
	                                                  RECEPTION_HEAD_STATUS_RECT.right*REC_RATIOWIDTH, RECEPTION_HEAD_STATUS_RECT.bottom*REC_RATIOHEIGHT);


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CReceptionHeadDlg::CReceptionHeadDlg(CWnd* pParent)
    :CBaseDlg(CReceptionHeadDlg::IDD, pParent, REC_HEAD_RECT_RATIO, GRAY/*RECEPTION_HEAD_BK_COLOR*/,false)
	,m_status(_T(""))
	,m_stationName(_T(""))
	,m_stationNameCN(_T(""))
	,m_stationNameEN(_T(""))
	,m_stationNamePN(_T(""))	
	,m_strWeekDay(_T(""))
	,m_strStatusDispMessage(_T(""))
{ 
	m_operationMode = OPERATION_MODE_STATUS_NORMAL_MODE;

	CString bmppath;
	CString apppath;
	GetAppPath(apppath);

	// logo图片
	bmppath = apppath + TVM_IMAGE_LOGO;
	m_Logo.Load(bmppath); 
	if(!m_Logo.IsNull()) ConvertPng(&m_Logo);

	// 背景图片
	bmppath = apppath + TVM_IMAGE_HEAD_BKG;
	m_bkg.Load(bmppath); 
	//if(!m_bkg.IsNull()) ConvertPng(&m_bkg);


	theAPP_SESSION.ClientLanguageModeChanged.AddHandler(this,&CReceptionHeadDlg::OnClientLanguageModeChanged);

	theTVM_STATUS_MGR.TVMPaymentChanged.AddHandler(this,&CReceptionHeadDlg::OnPaymentChanged);
	theTVM_STATUS_MGR.TVMChangeChanged.AddHandler(this,&CReceptionHeadDlg::OnChangeChanged);
	theTVM_STATUS_MGR.TVMStatusDispMessageChanged.AddHandler(this,&CReceptionHeadDlg::OnStatusChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CReceptionHeadDlg::~CReceptionHeadDlg()
{
	theAPP_SESSION.ClientLanguageModeChanged.RemoveHandler(this,&CReceptionHeadDlg::OnClientLanguageModeChanged);

	theTVM_STATUS_MGR.TVMPaymentChanged.RemoveHandler(this,&CReceptionHeadDlg::OnPaymentChanged);
	theTVM_STATUS_MGR.TVMChangeChanged.RemoveHandler(this,&CReceptionHeadDlg::OnChangeChanged);
	theTVM_STATUS_MGR.TVMStatusDispMessageChanged.RemoveHandler(this,&CReceptionHeadDlg::OnStatusChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
              0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CReceptionHeadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }
	//m_gif.createPicWnd(this->m_hWnd);
	// 启动时钟显示计时器
    this->SetTimer(TIMER_STS, TIMER_INTERVAL, NULL);
	//CString szPath = _T("");
	//GetAppPath(szPath);
	//m_gif.loadPic(szPath+_T(".\\res\\TVM_logo.gif"));
	//m_gif.MoveWindow(RECEPTION_HEAD_GIF_LOGO_RECT.left,RECEPTION_HEAD_GIF_LOGO_RECT.top,RECEPTION_HEAD_GIF_LOGO_RECT.right-RECEPTION_HEAD_GIF_LOGO_RECT.left,RECEPTION_HEAD_GIF_LOGO_RECT.bottom-RECEPTION_HEAD_GIF_LOGO_RECT.top,FALSE);
	//m_gif.ShowWindow(TRUE);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新状态信息

@param      

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::updataStatusData()
{
	// 判断如果当前是站员业务前面则不显示任何提示
	if(WAIT_MAINTENANCE == theSERVICE_MGR.GetState() || MAINTENANCE == theSERVICE_MGR.GetState() || 
		theTVM_STATUS_MGR.GetWorkMode() == DEVICE_CONTRL_CODE_BUSINESS || 
		theTVM_STATUS_MGR.GetWorkMode() == DEVICE_CONTRL_CODE_BUSINESS_QUERY
		//(theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CARD) != DEVICE_CONTRL_CODE_BUSINESS_CARD
		){
			if (theTVM_STATUS_MGR.GetWorkMode() == DEVICE_CONTRL_CODE_BUSINESS_QUERY)
			{
				m_status = theFunction_INFO.getModeLanguage(14,(theAPP_SESSION.GetCurrentClientLanguageType()==LANGUAGE_ID_CHINESE?1:2));
			}
			else
			{
				m_status = _T("");
			}
		return;
	}
	WORD wdChangeMode=theTVM_STATUS_MGR.GetChangeMode();
	WORD wdPaymentMode=theTVM_STATUS_MGR.GetPaymentMode();
	WORD device_service_mode  = theTVM_STATUS_MGR.GetWorkMode();
	//WORD wWebPayServiceMode = theTVM_STATUS_MGR.GetWebPayServiceMode();

	LANGUAGETYPE_T languageType=theAPP_SESSION.GetCurrentClientLanguageType();
	switch(languageType)
	{
	case LANGUAGE_ID_CHINESE:
		// 默认显示正常服务(只显示一个)
		m_status = theFunction_INFO.getModeLanguageCN(DEVICE_CONTRL_CODE_PAYMENT_ALL);
		if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM){
			if(device_service_mode == DEVICE_CONTRL_CODE_BUSINESS_ALL &&
				wdChangeMode == DEVICE_CONTRL_CODE_ALL_CHANGE &&
				wdPaymentMode == DEVICE_CONTRL_CODE_PAYMENT_ALL)
			{
				break;
			}
			else{
				if ((device_service_mode != DEVICE_CONTRL_CODE_BUSINESS_ALL)
					&& ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) != DEVICE_CONTRL_CODE_BUSINESS_CARD)
					&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE
					)//只充值
				{
					m_status = theFunction_INFO.getModeLanguageCN(device_service_mode);
				}
				else{//只有售票时才显示支付和找零模式
					// 如果找零不全找，则显示当前找零模式
					if(wdChangeMode != DEVICE_CONTRL_CODE_ALL_CHANGE && wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT){
						m_status = theFunction_INFO.getModeLanguageCN(wdChangeMode);
					}

					// 支付模式
					CString strPayMethod(_T(""));

					// 只收硬币
					if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) /*&&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)*/){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_COIN,CN_ID);
					}
					// 只收纸币
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)/* &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)*/){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_BANKNOTE,CN_ID);
					}
					// 只扫码支付
					/*else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_QR,CN_ID);
					}
					// 只收现金
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_CASH,CN_ID);
					}*/
					// 不收硬币
					//else 
					if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) 
						//&&((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)
						){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_NO_COIN,CN_ID);
					}
					// 不收纸币
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) 
						//&&((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)
						){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_NO_BANKNOTE,CN_ID);
					}

					if (device_service_mode != DEVICE_CONTRL_CODE_BUSINESS_ALL
						&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD
						&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE
						)
					{
						m_status = theFunction_INFO.getModeLanguageCN(device_service_mode);
					}
					if(!strPayMethod.IsEmpty()){
						if(wdChangeMode != DEVICE_CONTRL_CODE_ALL_CHANGE && wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT){
							m_status+=_T(" | ");
							m_status+=strPayMethod;
						}
						else{
							m_status = strPayMethod;

						}
					}
				}
				
			}
			
		}
		
		break;
	case LANGUAGE_ID_ENGLISH:
		m_status = theFunction_INFO.getModeLanguageEN(DEVICE_CONTRL_CODE_PAYMENT_ALL);
		if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM){
			if(device_service_mode == DEVICE_CONTRL_CODE_BUSINESS_ALL &&
				wdChangeMode == DEVICE_CONTRL_CODE_ALL_CHANGE &&
				wdPaymentMode == DEVICE_CONTRL_CODE_PAYMENT_ALL)
			{
				break;
			}
			else{
				if ((device_service_mode != DEVICE_CONTRL_CODE_BUSINESS_ALL)
					&& ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) != DEVICE_CONTRL_CODE_BUSINESS_CARD)
					&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE
					)
				{
					m_status = theFunction_INFO.getModeLanguageEN(device_service_mode);
				}
				else{
					if(wdChangeMode != DEVICE_CONTRL_CODE_ALL_CHANGE && wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT){
						m_status = theFunction_INFO.getModeLanguageEN(wdChangeMode);
					}	

					// 支付模式
					CString strPayMethod(_T(""));

					// 只收硬币
					if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)/* &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)*/){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_COIN,EN_ID);
					}
					// 只收纸币
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)/* &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)*/){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_BANKNOTE,EN_ID);
					}
					// 只扫码支付
					/*else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_QR,EN_ID);
					}
					// 只收现金
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) &&
						((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) != DEVICE_CONTRL_CODE_WEB_QR)){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_ONLY_CASH,EN_ID);
					}*/
					// 不收硬币
					//else 
				
					if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) 
						//&&((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)
						){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_NO_COIN,EN_ID);
					}
					// 不收纸币
					else if(((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && 
						((wdPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) 
						//&&((wWebPayServiceMode & DEVICE_CONTRL_CODE_WEB_QR) == DEVICE_CONTRL_CODE_WEB_QR)
						){
							// 中文
							strPayMethod = theFunction_INFO.getModeLanguage(MSG_NO_BANKNOTE,EN_ID);
					}
			
					if (device_service_mode != DEVICE_CONTRL_CODE_BUSINESS_ALL
						&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD
						&& (device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE
						)
					{
						m_status = theFunction_INFO.getModeLanguageEN(device_service_mode);
					}

					if(!strPayMethod.IsEmpty()){
						if(wdChangeMode != DEVICE_CONTRL_CODE_ALL_CHANGE && wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT){
							m_status+=_T(" | ");
							m_status+=strPayMethod;
						}
						else{
							m_status = strPayMethod;

						}
					}
				}

			}



			/*if(wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT_ALL){
			if(wdChangeMode == DEVICE_CONTRL_CODE_ALL_CHANGE){
			m_status = theFunction_INFO.getModeLanguageEN(wdPaymentMode);
			}
			else{
			m_status+=_T(" | ");
			m_status+=theFunction_INFO.getModeLanguageEN(wdPaymentMode);
			}
			}*/
		}
		/*else if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_AVM){
			if(wdPaymentMode != (DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE|DEVICE_CONTRL_CODE_PAYMENT_BANKCARD)){
				m_status = theFunction_INFO.getModeLanguageEN(wdPaymentMode);
			}
		}*/
		
		break;
	case LANGUAGE_ID_PORTUGUESE:
		m_status = theFunction_INFO.getModeLanguagePN(DEVICE_CONTRL_CODE_PAYMENT_ALL);
		if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM){
			if(wdChangeMode != DEVICE_CONTRL_CODE_ALL_CHANGE){
				m_status = theFunction_INFO.getModeLanguagePN(wdChangeMode);
			}	
			if(wdPaymentMode != DEVICE_CONTRL_CODE_PAYMENT_ALL){
				if(wdChangeMode == DEVICE_CONTRL_CODE_ALL_CHANGE){
					m_status = theFunction_INFO.getModeLanguagePN(wdPaymentMode);
				}
				else{
					m_status+=_T(" | ");
					m_status+=theFunction_INFO.getModeLanguagePN(wdPaymentMode);
				}
			}
		}
		/*else if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_AVM){
		if(wdPaymentMode != (DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE|DEVICE_CONTRL_CODE_PAYMENT_BANKCARD)){
		m_status = theFunction_INFO.getModeLanguagePN(wdPaymentMode);
		}
		}	*/	
		break;
	case LANGUAGE_ID_UNSPECIFIED:
		break;
	default:
		break;
	}
	m_status.Replace(_T("(R)"),_T(""));
	m_status.Replace(_T("(G)"),_T(""));
	m_status.Replace(_T("(B)"),_T(""));
	m_status.Replace(_T("(Y)"),_T(""));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      找零模式变更

@param      

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnChangeChanged()
{
	updataStatusData();
	this->InvalidateRect(SERVICE_STATUSE_RECT, TRUE);	
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     支付模式变更

@param      

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnPaymentChanged()
{
	updataStatusData();
	this->Invalidate();
	this->InvalidateRect(SERVICE_STATUSE_RECT, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      找零或者支付模式变更事件

@param      

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnChangeOrPaymentChanged()
{
	updataStatusData();
	this->InvalidateRect(SERVICE_STATUSE_RECT, FALSE);
	this->InvalidateRect(DATETIME_RECT, FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnDraw(CDC* pDc)
{
    __super::OnDraw(pDc);
	CRect  roundRect = CRect(CPoint(0,0),REC_HEAD_RECT_RATIO.Size());

	// 创建内存DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	// 创建内存位图
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDc,roundRect.Width(),roundRect.Height());
	// 内存位图与绘图DC关联
	MemDc.SelectObject(MemBitmap);
// 	// 设置背景色
// 	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),RGB(255,255,255));
	// 使用内存DC绘图
	CDC* pDC = &MemDc;

	// 绘制背景图片
	if (!m_bkg.IsNull()){
		m_bkg.Draw(pDC->m_hDC,REC_HEAD_RECT_RATIO);
	}

	COLORREF svc_color;
	if(m_operationMode == OPERATION_MODE_STATUS_NORMAL_MODE){
		svc_color = GUEST_STATE_SVC_NORMAL_FONT_COLOR;
	}
	else{
		svc_color = GUEST_STATE_SVC_UNNORMAL_FONT_COLOR;
	}
	svc_color = GUEST_STATE_STATION_FONT_COLOR;
	if (!m_Logo.IsNull()){
		m_Logo.Draw(pDC->m_hDC,RECEPTION_HEAD_LOGO_RECT);
	}

	if (theAPP_SESSION.GetCurrentClientLanguageType() != LANGUAGETYPE_T::PORTUGUESE)
	{
		// 显示当前车站
		DrawText(pDC,STATION_NAME_CN_RECT_RATIO,m_stationName,GUEST_STATE_STATION_FONT_NAME/*REC_STATION_FONT_NAME_CN*/,GUEST_STATE_STATION_FONT_SIZE/*REC_STATION_FONT_SIZE_CN*/ ,
			FW_BOLD,GUEST_STATE_STATION_FONT_COLOR/*WHITE*/,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		CRect rect = STATION_NAME_CN_RECT_RATIO;
		CStringArray* vecText = SplitCString(m_stationName,_T(" "));
		CFont vertFont;
		//字体
		vertFont.CreateFont(50, 0, -900, 900, FW_BOLD,
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH, _T("Arial"));
		CFont *pOldFont = pDC->SelectObject(&vertFont);
		COLORREF oldColor = pDC->GetTextColor();
		pDC->SetTextColor(GUEST_STATE_STATION_FONT_COLOR);  //文本颜色
		pDC->SetBkMode(TRANSPARENT);
		int nWeight = 0;
		int it = 0;
		while(it < vecText->GetSize())
		{
			CString szCnt = vecText->GetAt(it);
//  			dc.TextOut(rect.left+80+nWeight, rect.top+50, szCnt); //文本起始位置
			TextOut(pDC->m_hDC,rect.left+nWeight, rect.top, szCnt,szCnt.GetLength()); //文本起始位置

// 			DrawText(pDC,CRect(CPoint(rect.left+nWeight,rect.top),CSize(50,100)),szCnt,GUEST_STATE_STATION_FONT_NAME/*REC_STATION_FONT_NAME_CN*/,GUEST_STATE_STATION_FONT_SIZE/*REC_STATION_FONT_SIZE_CN*/ ,
// 				FW_BOLD,GUEST_STATE_STATION_FONT_COLOR/*WHITE*/,DT_TOP);
// 
			it++;
			nWeight += 50;
		}
		pDC->SetTextColor(oldColor); 
		pDC->SelectObject(pOldFont); 

	}
	//// 显示设备编号和软件版本号
	//DrawText(pDC, DEVICE_ID_RECT_RATIO, m_deviceId, _T("黑体")/*GUEST_STATE_DATE_FONT_NAME*//*REC_STATION_FONT_NAME_CN*/,20/*REC_DATE_FONT_SIZE_CN*/,
	//	FW_NORMAL, GUEST_STATE_DATE_FONT_COLOR/*WHITE*/, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	// 显示当前服務狀態
	/*if (!m_hStatusPic.IsNull()){
		m_hStatusPic.Draw(pDC->m_hDC,RECEPTION_HEAD_STATUS_RECT);
	}*/

	int nServiceStatusFontSize = GUEST_STATE_SVC_FONT_SIZE;
	//DrawText(pDC,RECEPTION_HEAD_STATUS_RECT_RATIO,m_strStatusDispMessage,GUEST_STATE_SVC_FONT_NAME,nServiceStatusFontSize,FW_NORMAL,
	//	svc_color, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	// 计算支付状态占用字符大小
	CRect rect_paystatus(SERVICE_STATUSE_RECT_RATIO);
	// 取字体占用像素大小
	CSize size;
	::GetTextExtentPoint32(pDC->GetSafeHdc(),m_status.GetBuffer(),m_status.GetLength(),&size);
	//// 计算分辨率
	//float fResolution = (float)GUEST_STATE_SVC_FONT_SIZE/(float)(size.cy == 0 ? 1 : size.cy);
	//// 计算像素差
	//long lDefX = (rect_paystatus.right - rect_paystatus.left) - (long)((float)size.cx * fResolution)/2;
	//// Reset rect
	//if(lDefX > 0){
	//	rect_paystatus.left += lDefX;// 重新计算矩形左边框距离
	//}
	// 计算分辨率
	float fResolution = (float)GUEST_STATE_SVC_FONT_SIZE/(float)(size.cy == 0 ? 1 : size.cy);
	// 计算像素差
	long lDefX = (rect_paystatus.Width() - (long)((float)size.cx * fResolution))/2;
	// Reset rect
	//if(lDefX > 0)
	{
		rect_paystatus.left += lDefX;// 重新计算矩形左边框距离
	}
	DrawText(pDC, rect_paystatus, m_status, GUEST_STATE_SVC_FONT_NAME/*REC_STATION_FONT_NAME_CN*/,nServiceStatusFontSize/*REC_DATE_FONT_SIZE_CN*/, 
			FW_NORMAL, svc_color/*WHITE*/, DT_VCENTER|DT_SINGLELINE);
	
	// 显示当前时间
	// 143是CVS文言中星期一的偏移量
	CTime curTime(CTime::GetCurrentTime());
    DrawText(pDC, DATETIME_RECT_RATIO, ComGetSysTimeText(YYYYMMDDHHMMSS_TYPE5) + _T(" ") + m_strWeekDay, GUEST_STATE_DATE_FONT_NAME/*REC_STATION_FONT_NAME_CN*/,GUEST_STATE_DATE_FONT_SIZE/*REC_DATE_FONT_SIZE_CN*/,
			FW_NORMAL, GUEST_STATE_DATE_FONT_COLOR/*WHITE*/, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
    //DrawText(pDC, WEEKDAY_CN_RECT_RATIO, m_strWeekDay, REC_STATION_FONT_NAME_CN,REC_DATE_FONT_SIZE_CN, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	// 将缓冲区的图形拷贝到绘图DC
	pDc->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pDC,0,0,SRCCOPY);
	// 绘图清理
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面销毁函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnDestroy()
{
    // 关闭时钟显示计时器
    this->KillTimer(TIMER_STS);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      时间控制

@param      (i)UINT nIDEvent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnTimer(UINT nIDEvent) 
{
    _DATE_TIME curTime = ComGetCurTime();
    // 初始或分钟改变时，刷新时间显示
    if (m_preTime.biSecond!=curTime.biSecond/*m_preTime.biMinute != curTime.biMinute*/ || m_preTime.wYear == 0) {
        this->InvalidateRect(DATETIME_RECT_RATIO, FALSE);
		//this->InvalidateRect(WEEKDAY_CN_RECT_RATIO, TRUE);
        m_preTime = curTime;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置车站名称

@param      (i)CString stationNameCN   车站名称（中文）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::SetStationName(CString stationNameCN, CString stationNameEN, CString stationNamePN)
{
	m_stationNameCN = stationNameCN;
	m_stationNameEN = stationNameEN;
	m_stationNamePN = stationNamePN;
	switch(theAPP_SESSION.GetCurrentClientLanguageType()){
	case LANGUAGE_ID_CHINESE:
		m_stationName = stationNameCN;
		m_strWeekDay  = ComGetSysTimeText(TIME_FORMAT::WEEKDAY_CN);
		break;
	case  LANGUAGE_ID_ENGLISH:
		m_stationName = stationNameEN;
		m_strWeekDay  = ComGetSysTimeText(TIME_FORMAT::WEEKDAY_EN);
		break;
	case LANGUAGE_ID_PORTUGUESE:
		m_stationName = stationNamePN;
		m_strWeekDay  = ComGetSysTimeText(TIME_FORMAT::WEEKDAY_EN);
		break;
	default:
		m_stationName = _T("--");
		m_strWeekDay  = _T("--");
	}   
	m_strWeekDay = _T("");
	CString strVer;
	m_deviceId = _T("No.") + theMAINTENANCE_INFO.GetCurrentDevice().ToString();
	strVer.Format(_T(" V%d"), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT, CVersionInfo::MC));
	m_deviceId += strVer;
	this->InvalidateRect(STATION_NAME_CN_RECT_RATIO, TRUE);
	//this->InvalidateRect(WEEKDAY_CN_RECT_RATIO,TRUE);
	//this->InvalidateRect(DEVICE_ID_RECT_RATIO, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置服務狀態

@param      (i)CString serviceNameCH   业务名称

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::SetServiceStatuse(OPERATION_MODE operationMode)
{
	/*CString bmppath;
	CString apppath;
	GetAppPath(apppath);

	m_operationMode = operationMode;
	if (m_operationMode == OPERATION_MODE_STATUS_UNKNOW){
	}
	else if (m_operationMode == OPERATION_MODE_STATUS_NORMAL_MODE){
	m_status = _tl(TXT_ID_STATUES_NORMAL_MODE);
	bmppath = apppath + TVM_IMAGE_SERVICE_NORMAL;
	m_hStatusPic.Destroy();
	m_hStatusPic.Load(bmppath);	
	}
	else{		
	m_status = _tl(TXT_ID_STATUES_DEGRADE_MODE);
	bmppath = apppath + TVM_IMAGE_SERVICE_ABNORMAL;
	m_hStatusPic.Destroy();
	m_hStatusPic.Load(bmppath);
	}
	if (!m_hStatusPic.IsNull()){
	ConvertPng(&m_hStatusPic);
	}
	this->InvalidateRect(SERVICE_STATUSE_RECT_RATIO, FALSE);
	this->InvalidateRect(RECEPTION_HEAD_STATUS_RECT_RATIO, FALSE);*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      界面语言发生变化

@param      

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnClientLanguageModeChanged(LANGUAGETYPE_T& preLanguageMode)
{
	SetServiceStatuse(m_operationMode);
	SetStationName(m_stationNameCN,m_stationNameEN,m_stationNamePN);
	OnStatusChanged();
	OnChangeOrPaymentChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      透明处理

@param      CImage* image

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::ConvertPng(CImage* image)
{
	for(int i = 0; i < image->GetWidth(); i++){  
		for(int j = 0; j < image->GetHeight(); j++){  
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(image->GetPixelAddress(i , j));  
			pucColor[0] = pucColor[0] * pucColor[3] / 255;  
			pucColor[1] = pucColor[1] * pucColor[3] / 255;  
			pucColor[2] = pucColor[2] * pucColor[3] / 255;  
		}  
	}  
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      显示给乘客的提示信息，如招援信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnStatusChanged(){
	if(STATUS_CALL_HELP_MESSAGE_SHOW_NONE == theAPP_SESSION.GetMessageShowPassenger()){
		m_strStatusDispMessage = _T("");
	}
	else{
		m_strStatusDispMessage = _tl(theAPP_SESSION.GetMessageShowPassenger());
	}	
	this->InvalidateRect(RECEPTION_HEAD_STATUS_RECT_RATIO,TRUE);
}
