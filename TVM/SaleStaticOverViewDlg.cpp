#include "stdafx.h"
#include "SaleStaticSvc.h"
#include "SaleStaticOverViewDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSaleStaticOverViewDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CSaleStaticOverViewDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

#define UD_MOP		_T(" 元")
#define UD_LEFT		_T("(")
#define UD_RIGHT	_T(")")
#define STR_TXN_UNIT	_T(" ") + _opl(TXN_UNIT)
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticOverViewDlg::CSaleStaticOverViewDlg(CService* pService)
	: COperationDlg(CSaleStaticOverViewDlg::IDD,pService)
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
CSaleStaticOverViewDlg::~CSaleStaticOverViewDlg()
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
int CSaleStaticOverViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
        return -1;
    }

	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_labelGroup);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 128);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 128);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 128);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 128);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 128);		// 设置第四列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);

	
    return 0;
}

void CSaleStaticOverViewDlg::showData()
{
	CSaleStaticSvc* pSvc = (CSaleStaticSvc*)GetService();
	if(NULL!=pSvc)
	{
		CSaleStaticSvc::Model& model = pSvc->GetDataModel();
		LABEL_GROUP_FIVE notice[15];
		int lenIndex = 0;

		CString sztemp;

		// 售票时间
		m_baseInfoArea->SetLableWidth(lenIndex,1,500);
		notice[lenIndex].name1=_opl(TXT_SALEDETAIL_SALEPERIOD) + _opl(TXT_COLON);
		sztemp+=model.strDateFrom;
		sztemp+=_T("—");
		sztemp+=model.strDateTo;
		notice[lenIndex++].name2 = sztemp;

		// 故障金额
		sztemp=_opl(TXT_SALEDETAIL_FAILURERETURN) + _opl(TXT_COLON);
		notice[lenIndex].name1 = sztemp;

		sztemp  = ComFormatAmount(model.dwFailedAmount);
		sztemp += _opl(MONEY_UNIT);
		notice[lenIndex].name2 = sztemp;

		sztemp  = ComFormatCount(model.dwFailedCount);
		sztemp += _T(" 个")/*STR_TXN_UNIT*/;
		notice[lenIndex].name3 = sztemp;

		sztemp	= _opl(TXT_SALESTATIC_FALIED_TOTAL_INCOME) + _opl(TXT_COLON);
		notice[lenIndex].name4 = sztemp;

		sztemp = ComFormatAmount(model.dwFailedTotalAmount);
		sztemp+=_opl(MONEY_UNIT);
		notice[lenIndex++].name5=sztemp;

		lenIndex++;
		// 售票
		sztemp=_opl(TXT_SALEGUIDE_ISSUE);
		notice[lenIndex++].name1=sztemp;

		
		sztemp=_opl(TXT_SALEDETAIL_SJT);
		notice[lenIndex].name1  = sztemp;

		sztemp= ComFormatAmount(model.dwSingleTicketAmount) + _opl(MONEY_UNIT);
		notice[lenIndex].name2  = sztemp;

		sztemp= ComFormatCount(model.dwSingleTicketTrsCount) + STR_TXN_UNIT/*UD_COUNT*/;
		notice[lenIndex].name3  = sztemp;

		m_baseInfoArea->SetLableWidth(lenIndex,3,220);
		sztemp= ComFormatCount(model.dwSingleTicketCount) + _opl(TXT_SALEGUIDE_PIECES);
		notice[lenIndex++].name4 =sztemp;

		// 售票小计
		sztemp =_opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON);
		notice[lenIndex].name1 = sztemp;

		sztemp = ComFormatAmount(model.dwSaleTotalAmount) + _opl(MONEY_UNIT);
		notice[lenIndex].name2=sztemp;

		sztemp = ComFormatCount(model.dwSaleTotalTrsCount) + STR_TXN_UNIT/*UD_COUNT*/;
		notice[lenIndex].name3 = sztemp;

		m_baseInfoArea->SetLableWidth(lenIndex,3,220);
		sztemp = ComFormatCount(model.dwSaleTotalCount) + _opl(TXT_SALEGUIDE_PIECES);
		notice[lenIndex++].name4=sztemp;

		lenIndex++;
		// 充值
		sztemp=_opl(TXT_SALEGUIDE_CHARGE);
		notice[lenIndex++].name1=sztemp;
	
		sztemp=_T("储值卡") + _opl(TXT_COLON);
		notice[lenIndex].name1=sztemp;

		sztemp = ComFormatAmount(model.dwRechargeAmount) + _opl(MONEY_UNIT);
		notice[lenIndex].name2=sztemp;

		sztemp=ComFormatCount(model.dwRechargeTrsCount) + STR_TXN_UNIT;
		notice[lenIndex++].name3 = sztemp;

		// 充值小计
		sztemp =_opl(TXT_SALEGUIDE_SUBTOTAL) + _opl(TXT_COLON);
		notice[lenIndex].name1 = sztemp;

		sztemp = ComFormatAmount(model.dwRechargeTotalAmount) + _opl(MONEY_UNIT);
		notice[lenIndex].name2=sztemp;

		sztemp = ComFormatCount(model.dwRechargeTotalTraCount) + STR_TXN_UNIT/*UD_COUNT*/;
		notice[lenIndex++].name3 = sztemp;

		lenIndex++;

		// 净收入
		notice[lenIndex].name1	 = _opl(TXT_SALEGUIDE_NET_INCOME) + _opl(TXT_COLON);
		notice[lenIndex++].name2 = ComFormatAmount(model.dwNetIncomAmount) + _opl(MONEY_UNIT);


		setlabelgroup(notice,15);

		theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_F6_ESC);
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
void CSaleStaticOverViewDlg::UpdateUI()
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
void CSaleStaticOverViewDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
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
void CSaleStaticOverViewDlg::setlabelgroup(const LABEL_GROUP_FIVE* plabelgroup,const UINT uigroupnum)
{
	UINT uiloop=0;
	for(;uiloop<UD_MAX_ROW;uiloop++)
	{
		if(uiloop<uigroupnum)
		{			
			const LABEL_GROUP_FIVE* ptemp=plabelgroup+uiloop;
			m_labelGroup[uiloop].name1 = ptemp->name1;
			m_labelGroup[uiloop].name2= ptemp->name2;
			m_labelGroup[uiloop].name3=ptemp->name3;
			m_labelGroup[uiloop].name4=ptemp->name4;
			m_labelGroup[uiloop].name5=ptemp->name5;
		}else
		{
			m_labelGroup[uiloop].name1= _T("");
			m_labelGroup[uiloop].name2= _T("");
			m_labelGroup[uiloop].name3=_T("");
			m_labelGroup[uiloop].name4=_T("");
			m_labelGroup[uiloop].name5=_T("");
		}
	}
}
