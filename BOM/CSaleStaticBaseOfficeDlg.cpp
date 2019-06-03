#include "stdafx.h"
#include "CSaleStaticBaseOfficeDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSaleStaticBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CSaleStaticBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticBaseOfficeDlg::CSaleStaticBaseOfficeDlg(CService* pService)
	: COperationDlg(CSaleStaticBaseOfficeDlg::IDD, pService)
{
	CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();
	// 信息显示区域
	m_baseInfo->titleInfo.titleName = "";                    // 标题
 	m_baseInfo->detailInfo.row = 8;           // 行数
 	m_baseInfo->detailInfo.labelGroup = 0;     // 标签文字（3列）
 	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // 编辑框数目
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticBaseOfficeDlg::~CSaleStaticBaseOfficeDlg()
{
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
int CSaleStaticBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( -1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA  )) {
		return -1;
	}
	CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();
	// 创建业务部分和其他部分显示信息区域
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo, pCSaleStaticSvc->GetAuditInfoGroup());

   
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制

@param      (i) CDC* pDC  设备上下文指针

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CSaleStaticBaseOfficeDlg::OnDraw(CDC* pDC)
{
    CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();

    try {
	    __super::OnDraw(pDC);
	}
	catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pCSaleStaticSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}
