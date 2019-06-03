#include "stdafx.h"
#include "SaleStaticSvc.h"
#include "SaleStaticDetailDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSaleStaticDetailDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CSaleStaticDetailDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticDetailDlg::CSaleStaticDetailDlg(CService* pService)
	: COperationDlg(CSaleStaticDetailDlg::IDD,pService)
{
	//m_baseInfo->detailInfo.row = UD_MAX_ROW;           // 行数
	//m_baseInfo->detailInfo.labelGroup = m_labelgroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticDetailDlg::~CSaleStaticDetailDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CSaleStaticDetailDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
        return -1;
    }

	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_labelGroup);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,100);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,125);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,125);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,100);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,125);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,125);		// 设置第四列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);

    return 0;
}

void CSaleStaticDetailDlg::showData()
{
	CSaleStaticSvc* pSvc = (CSaleStaticSvc*)GetService();
	if(NULL!=pSvc)
	{
		CSaleStaticSvc::Model& model = pSvc->GetDataModel();
		LABEL_GROUP_SIX notice[15];
		int lenIndex = 0;
		CString sztemp;

		// 接收纸币
		sztemp=_opl(TXT_SALEDETAIL_ACCEPTEDBANKNOTE);
		m_baseInfoArea->SetLableAlign(0,0,DT_LEFT| DT_VCENTER);
		notice[lenIndex].name1=sztemp;

		sztemp = _opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON);
		sztemp += ComFormatCount(model.dwRecvBHTotalCount) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name5 = sztemp;

		notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvBHTotalAmount) + _opl(MONEY_UNIT);

		// 纸币接收详细
		notice[lenIndex].name1= _T("1") + _opl(MONEY_UNIT) ;
		notice[lenIndex].name2 = ComFormatCount(model.dwRecvBHFace1Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name3 = ComFormatAmount(model.dwRecvBHFace1Count * 100) + _opl(MONEY_UNIT);

		notice[lenIndex].name4 = _T("5") + _opl(MONEY_UNIT);
		notice[lenIndex].name5 = ComFormatCount(model.dwRecvBHFace5Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvBHFace5Count * 500) + _opl(MONEY_UNIT);

		notice[lenIndex].name1= _T("10") + _opl(MONEY_UNIT) ;
		notice[lenIndex].name2 = ComFormatCount(model.dwRecvBHFace10Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name3 = ComFormatAmount(model.dwRecvBHFace10Count * 1000) + _opl(MONEY_UNIT);

		notice[lenIndex].name4 = _T("20") + _opl(MONEY_UNIT);
		notice[lenIndex].name5 = ComFormatCount(model.dwRecvBHFace20Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvBHFace20Count * 2000) + _opl(MONEY_UNIT);

		notice[lenIndex].name1= _T("50") + _opl(MONEY_UNIT) ;
		notice[lenIndex].name2 = ComFormatCount(model.dwRecvBHFace50Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name3 = ComFormatAmount(model.dwRecvBHFace50Count * 5000) + _opl(MONEY_UNIT);

		notice[lenIndex].name4 = _T("100") + _opl(MONEY_UNIT);
		notice[lenIndex].name5 = ComFormatCount(model.dwRecvBHFace100Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvBHFace100Count * 10000) + _opl(MONEY_UNIT);

		lenIndex++;

		// 接收硬币
		m_baseInfoArea->SetLableAlign(lenIndex,0,DT_LEFT| DT_VCENTER);
		notice[lenIndex].name1 = _opl(TXT_SALEDETAIL_ACCEPTEDCOIN);

		notice[lenIndex].name5 = _opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON) + ComFormatCount(model.dwRecvCHTotalCount) + _opl(TXT_COIN_UNIT);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvCHTotalAmount) + _opl(MONEY_UNIT);

		//notice[lenIndex].name1= _T("0.5") + _opl(MONEY_UNIT) ;
		//notice[lenIndex].name2 = ComFormatCount(model.dwRecvCHFace05Count) + _opl(TXT_COIN_UNIT);
		//notice[lenIndex].name3 = ComFormatAmount(model.dwRecvCHFace05Count * 50) + _opl(MONEY_UNIT);

		//notice[lenIndex].name4 = _T("1") + _opl(MONEY_UNIT);
		//notice[lenIndex].name5 = ComFormatCount(model.dwRecvCHFace1Count) + _opl(TXT_COIN_UNIT);
		//notice[lenIndex++].name6 = ComFormatAmount(model.dwRecvCHFace1Count * 100) + _opl(MONEY_UNIT);

		notice[lenIndex].name1 = _T("1") + _opl(MONEY_UNIT);
		notice[lenIndex].name2 = ComFormatCount(model.dwRecvCHFace1Count + model.dwRecvCHFace05Count) + _opl(TXT_COIN_UNIT);
		notice[lenIndex++].name3 = ComFormatAmount((model.dwRecvCHFace1Count + model.dwRecvCHFace05Count) * 100) + _opl(MONEY_UNIT);
		lenIndex++;

		// 硬币找零
		m_baseInfoArea->SetLableAlign(lenIndex,0,DT_LEFT| DT_VCENTER);
		notice[lenIndex].name1 = _opl(TXT_SALEDETAIL_RELEASECOIN);

		notice[lenIndex].name5 = _opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON) + ComFormatCount(model.dwChangeCHTotalCount) + _opl(TXT_COIN_UNIT);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeCHTotalAmount) + _opl(MONEY_UNIT);

		//notice[lenIndex].name1= _T("0.5") + _opl(MONEY_UNIT) ;
		//notice[lenIndex].name2 = ComFormatCount(model.dwChangeCHFace05Count) + _opl(TXT_COIN_UNIT);
		//notice[lenIndex].name3 = ComFormatAmount(model.dwChangeCHFace05Count * 50) + _opl(MONEY_UNIT);

		//notice[lenIndex].name4 = _T("1") + _opl(MONEY_UNIT);
		//notice[lenIndex].name5 = ComFormatCount(model.dwChangeCHFace1Count) + _opl(TXT_COIN_UNIT);
		//notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeCHFace1Count * 100) + _opl(MONEY_UNIT);

		notice[lenIndex].name1= _T("1") + _opl(MONEY_UNIT) ;
		notice[lenIndex].name2 = ComFormatCount(model.dwChangeCHFace05Count + model.dwChangeCHFace1Count) + _opl(TXT_COIN_UNIT);
		notice[lenIndex].name3 = ComFormatAmount((model.dwChangeCHFace05Count+model.dwChangeCHFace1Count) * 100) + _opl(MONEY_UNIT);
		lenIndex++;


		// 纸币找零
		sztemp=_opl(TXT_SALEDETAIL_BANKNOTERELEASE);
		m_baseInfoArea->SetLableAlign(lenIndex,0,DT_LEFT| DT_VCENTER);
		notice[lenIndex].name1=sztemp;

		sztemp = _opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON);
		sztemp += ComFormatCount(model.dwChangeBHTotalCount) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name5 = sztemp;

		notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeBHTotalAmount) + _opl(MONEY_UNIT);

		// 纸币找零详细
		notice[lenIndex].name1= _T("5") + _opl(MONEY_UNIT) ;
		notice[lenIndex].name2 = ComFormatCount(model.dwChangeBHFace5Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex].name3 = ComFormatAmount(model.dwChangeBHFace5Count * 500) + _opl(MONEY_UNIT);

		notice[lenIndex].name4 = _T("10") + _opl(MONEY_UNIT);
		notice[lenIndex].name5 = ComFormatCount(model.dwChangeBHFace10Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeBHFace10Count * 1000) + _opl(MONEY_UNIT);

		//notice[lenIndex].name1= _T("10") + _opl(MONEY_UNIT) ;
		//notice[lenIndex].name2 = ComFormatCount(model.dwChangeBHFace10Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		//notice[lenIndex].name3 = ComFormatAmount(model.dwChangeBHFace10Count * 1000) + _opl(MONEY_UNIT);

		//notice[lenIndex].name4 = _T("20") + _opl(MONEY_UNIT);
		//notice[lenIndex].name5 = ComFormatCount(model.dwChangeBHFace20Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		//notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeBHFace20Count * 2000) + _opl(MONEY_UNIT);

		//notice[lenIndex].name1= _T("50") + _opl(MONEY_UNIT) ;
		//notice[lenIndex].name2 = ComFormatCount(model.dwChangeBHFace50Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		//notice[lenIndex].name3 = ComFormatAmount(model.dwChangeBHFace50Count * 5000) + _opl(MONEY_UNIT);

		//notice[lenIndex].name4 = _T("100") + _opl(MONEY_UNIT);
		//notice[lenIndex].name5 = ComFormatCount(model.dwChangeBHFace100Count) + _opl(TXT_SALEGUIDE_MONEY_PIECES);
		//notice[lenIndex++].name6 = ComFormatAmount(model.dwChangeBHFace100Count * 10000) + _opl(MONEY_UNIT);

		setlabelgroup(notice,15);
		theAPP_SESSION.ShowOfficeGuide(GUIDE_F5_ESC);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSaleStaticDetailDlg::UpdateUI()
{
	showData();
	m_baseInfoArea->Invalidate();
	return ;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面激活后的处理
//
//@param      (i)UINT nState      不使用
//@param      (i)CWnd* pWndOther  不使用
//@param      (i)BOOL bMinimized  不使用
//
//@retval     void
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
void CSaleStaticDetailDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	theAPP_SESSION.SetKeyboard(KEY_PAGE|KEY_RETURN);
	
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置基础数据区的显示内容
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
void CSaleStaticDetailDlg::setlabelgroup(const LABEL_GROUP_SIX* plabelgroup,const UINT uigroupnum)
{
	UINT uiloop=0;
	for(;uiloop<UD_MAX_ROW;uiloop++)
	{
		if(uiloop<uigroupnum)
		{			
			const LABEL_GROUP_SIX* ptemp=plabelgroup+uiloop;
			m_labelGroup[uiloop].name1= ptemp->name1;
			m_labelGroup[uiloop].name2= ptemp->name2;
			m_labelGroup[uiloop].name3=	ptemp->name3;
			m_labelGroup[uiloop].name4=	ptemp->name4;
			m_labelGroup[uiloop].name5=	ptemp->name5;
			m_labelGroup[uiloop].name6=	ptemp->name6;
		}else
		{
			m_labelGroup[uiloop].name1= _T("");
			m_labelGroup[uiloop].name2= _T("");
			m_labelGroup[uiloop].name3= _T("");
			m_labelGroup[uiloop].name4= _T("");
			m_labelGroup[uiloop].name5= _T("");
			m_labelGroup[uiloop].name6= _T("");
		}
	}
}