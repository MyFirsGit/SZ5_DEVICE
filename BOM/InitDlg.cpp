#include "stdafx.h"
#include "InitDlg.h"
#include "InitSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TXT_UNDOWN                 = _T("--");
static const TCHAR* const TXT_SUCCESS                = _T("成功");
static const TCHAR* const TXT_FAIL                   = _T("失败");


static const int MSG_HEIGHT         =  90;                          // 文字高度
static const int MSG_TITLE_START_X  =  100 + 400;                         // 初期化项目名称起始Ｘ坐标
static const int MSG_TITLE_END_X    =  MSG_TITLE_START_X + 300;     // 初期化项目名称结束Ｘ坐标
static const int MSG_RESULT_START_X =  MSG_TITLE_END_X + 30;        // 初期化项目结果起始Ｘ坐标
static const int MSG_RESULT_END_X   =  MSG_RESULT_START_X + 500;    // 初期化项目结果结束Ｘ坐标

IMPLEMENT_DYNAMIC(CInitDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CInitDlg, COperationDlg)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInitDlg::CInitDlg(CService* pService)
: COperationDlg(CInitDlg::IDD,INIT_FRAME_RECT,LIGHTBLUE, pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInitDlg::~CInitDlg()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i)CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);


	CInitSvc* pService = (CInitSvc*)GetService();

	// 显示初始化内容
	for (int i = 0; i<pService->MAX_INITIAL_INFO_ROW; i++) {
		DrawText(pDC, CRect(MSG_TITLE_START_X, MSG_HEIGHT * i+ 22, 
			MSG_TITLE_END_X, MSG_HEIGHT * (i+1)+ 50), pService->m_InitialInfo[i].title, NORMAL_FONT, TITLE_FONT_SIZE,FW_NORMAL, BLACK, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

		CString expMsg = GetResultText(pService->m_InitialInfo[i].result);
		CString dealMsg ;
		if (pService->m_InitialInfo[i].result == CInitSvc::FAIL ) {
			expMsg = pService->m_InitialInfo[i].errorInfo;
			dealMsg = pService->m_InitialInfo[i].dealInfo;
		}
		DrawText(pDC, CRect(MSG_RESULT_START_X, MSG_HEIGHT * i + 22, 
			MSG_RESULT_END_X, MSG_HEIGHT * (i+1) + 50), expMsg, NORMAL_FONT, TITLE_FONT_SIZE, FW_BOLD,
			GetResultColor(pService->m_InitialInfo[i].result), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		DrawText(pDC, CRect(MSG_RESULT_START_X, MSG_HEIGHT * i + 22, 
			MSG_RESULT_END_X, MSG_HEIGHT * (i+1) + 95), dealMsg, NORMAL_FONT, TITLE_FONT_SIZE, FW_BOLD,
			GetResultColor(pService->m_InitialInfo[i].result), DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新初期化结果

@param      (i)int index    

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitDlg::ShowInitRessult(int index)
{
	InvalidateRect(CRect(MSG_RESULT_START_X, MSG_HEIGHT * index + 22, MSG_RESULT_END_X, MSG_HEIGHT * (index+1) + 22));
	UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取初期化结果文字

@param      (i)CInitSvc::INIT_RESULT result   

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const TCHAR* CInitDlg::GetResultText(CInitSvc::INIT_RESULT result)
{
	const TCHAR* resultText = TXT_UNDOWN;

	switch (result) {
	case CInitSvc::UNDOWN:                  // 未知状态
		resultText = TXT_UNDOWN;
		break;

	case CInitSvc::SUCCESS:                 // 成功
		resultText = TXT_SUCCESS;
		break;

	case CInitSvc::FAIL:                    // 失败
		resultText = TXT_FAIL;
		break;
	}

	return resultText;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取初期化结果颜色

@param      (i)CInitSvc::INIT_RESULT result      

@retval     COLORREF

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CInitDlg::GetResultColor(CInitSvc::INIT_RESULT result)
{
	COLORREF color = BLACK;

	switch (result) {
	case CInitSvc::UNDOWN :                 // 未知状态
		color = BLACK;
		break;               
	case CInitSvc::SUCCESS :                // 成功
		color = BLACK;
		break;                
	case CInitSvc::FAIL :                   // 失败
		color = RED;
		break;
	default:
		color = RED;
		break;
	}

	return color;
}
