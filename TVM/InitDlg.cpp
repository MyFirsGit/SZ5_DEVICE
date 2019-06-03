#include "stdafx.h"
#include "InitDlg.h"
#include "InitSvc.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int MSG_HEIGHT				=  40;							// 文字高度
static const int MSG_TITLE_START_X		=  100;							// 初期化项目名称起始Ｘ坐标
static const int MSG_TITLE_END_X		=  MSG_TITLE_START_X + 200;		// 初期化项目名称结束Ｘ坐标
static const int MSG_RESULT_START_X		=  MSG_TITLE_END_X + 5;			// 初期化项目结果起始Ｘ坐标
static const int MSG_RESULT_END_X		=  MSG_RESULT_START_X + 495;	// 初期化项目结果结束Ｘ坐标

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
: COperationDlg(CInitDlg::IDD, OPERATOR_INIT_RECT, OPERATOR_BASE_BK_COLOR/*LIGHTBLUE*/, pService)
, m_OnlyTime(true)
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

	BOOL bShowInitProcess = theFunction_INFO.IsShowInitProcess();
	int index;
	CInitSvc* pService = (CInitSvc*)GetService();
	if (bShowInitProcess){	// 显示初始化过程
		for (int i = 0; i < CInitSvc::ROW_MAX; i++)
		{
			// 判断是否显示当前行
			if (pService->m_order[i] == CInitSvc::ROW_INVALID) continue;
			index = pService->m_order[i];

			// 显示标题
			DrawText(pDC,
				CRect(MSG_TITLE_START_X, MSG_HEIGHT * index, MSG_TITLE_END_X, MSG_HEIGHT * (index + 1)),
				pService->m_InitialInfo[i].title, INI_FONT_NAME, INI_FONT_SIZE, FW_NORMAL, INI_FONT_COLOR/*BLACK*/,
				DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			// 显示结果
			CString expMsg = GetResultText(pService->m_InitialInfo[i].result);
			if (pService->m_InitialInfo[i].result == CInitSvc::FAIL)
			{
				expMsg = pService->m_InitialInfo[i].errorInfo;
				//expMsg += pService->m_InitialInfo[i].dealInfo;
			}
			DrawText(pDC,
				CRect(MSG_RESULT_START_X, MSG_HEIGHT * index, MSG_RESULT_END_X, MSG_HEIGHT * (index + 1)),
				expMsg, INI_FONT_NAME, INI_FONT_SIZE, FW_BOLD, GetResultColor(pService->m_InitialInfo[i].result),
				DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}else{	// 不显示初始化过程 
		CString strResultCN = _T("系統初始化中");
		CString strResultEN = _T("System Initializing");
		CString strResultPN = _T("Inicializando o sistema");
		BOOL bAllSucceed = TRUE;
		BOOL bInitDone = pService->m_InitialInfo[CInitSvc::ROW_PRINT].result == CInitSvc::UNKNOWN ? FALSE : TRUE;
		if ( bInitDone ){														// 初始化完成
			for (int nIndex(0); nIndex < CInitSvc::ROW_MAX;nIndex ++ ){
				if (pService->m_InitialInfo[nIndex].result == CInitSvc::FAIL){	// 有一项失败就算失败
					bAllSucceed = FALSE;
				}
			}
			if (bAllSucceed){													// 初始化成功
				strResultCN = _T("系統初始化成功");
				strResultEN = _T("Initialize Sucesfuly");
				strResultPN = _T("Inicializar Sucesfuly");
			}
			else{																 // 初始化失败
				strResultCN = _T("系統初始化失敗");
				strResultEN = _T("Failed to Initialize");
				strResultPN = _T("Falha ao inicializar");
			}
		}
		DrawText(pDC,CRect(0,110,640,190), strResultCN, INI_FONT_NAME, INI_FONT_SIZE*3, FW_BOLD, RGB(255,0,0), DT_CENTER);
		DrawText(pDC,CRect(0,190,640,270), strResultEN, INI_FONT_NAME, INI_FONT_SIZE*3, FW_BOLD, RGB(255,0,0), DT_CENTER);
		DrawText(pDC,CRect(0,270,640,350), strResultPN, INI_FONT_NAME, INI_FONT_SIZE*3, FW_BOLD, RGB(255,0,0), DT_CENTER);
		if (bInitDone && m_OnlyTime)
		{
			InvalidateRect(CRect(0,110, 640,350));
			m_OnlyTime = false;
		}
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
	CInitSvc* pService = (CInitSvc*)GetService();
	if (pService->m_order[index] == CInitSvc::ROW_INVALID) return;

	index = pService->m_order[index];
	InvalidateRect(CRect(MSG_RESULT_START_X, MSG_HEIGHT * index, MSG_RESULT_END_X, MSG_HEIGHT * (index + 1)));
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
CString CInitDlg::GetResultText(CInitSvc::INIT_RESULT result)
{
	switch (result)
	{
	case CInitSvc::SUCCESS:                 // 成功
		return _opl(INI_CHECK_SUCCESS);
	case CInitSvc::FAIL:                    // 失败
		return _opl(INI_CHECK_FAILURE);
	default:								// 未知状态
		return _opl(TXT_UNDEFINE);
	}
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
	switch (result)
	{           
	case CInitSvc::SUCCESS:			// 成功
		return INI_RESULT_SUCCESS_COLOR/*BLACK*/;          
	case CInitSvc::FAIL:			// 失败
		return INI_RESULT_FAILED_COLOR/*RED*/;
	default:						// 未知状态
		return INI_RESULT_UNKNOW_COLOR/*BLACK*/;
	}
}
