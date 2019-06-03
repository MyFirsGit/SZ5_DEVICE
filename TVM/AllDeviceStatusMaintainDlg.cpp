#include "stdafx.h"
#include "AllDeviceStatusSvc.h"
#include "AllDeviceStatusMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static const int MSG_HEIGHT				=  33;							// 文字高度
static const int MSG_TITLE_START_X		=  100;							// 初期化项目名称起始Ｘ坐标
static const int MSG_TITLE_END_X		=  MSG_TITLE_START_X + 200;		// 初期化项目名称结束Ｘ坐标
static const int MSG_RESULT_START_X		=  MSG_TITLE_END_X + 5;			// 初期化项目结果起始Ｘ坐标
static const int MSG_RESULT_END_X		=  MSG_RESULT_START_X + 495;	// 初期化项目结果结束Ｘ坐标

IMPLEMENT_DYNAMIC(CAllDeviceStatusMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CAllDeviceStatusMaintainDlg, COperationDlg)	
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CService* pService

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CAllDeviceStatusMaintainDlg::CAllDeviceStatusMaintainDlg(CService* pService)
		: COperationDlg(CAllDeviceStatusMaintainDlg::IDD,OPERATOR_INIT_RECT, OPERATOR_BASE_BK_COLOR,pService)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CAllDeviceStatusMaintainDlg::~CAllDeviceStatusMaintainDlg()
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
	void CAllDeviceStatusMaintainDlg::OnDraw(CDC* pDC)
	{
		__super::OnDraw(pDC);

		int index;
		CAllDeviceStatusSvc* pService = (CAllDeviceStatusSvc*)GetService();
		for (int i = 0; i < CAllDeviceStatusSvc::ROW_MAX; i++)
		{
			// 判断是否显示当前行
			if (pService->m_order[i] == CAllDeviceStatusSvc::ROW_INVALID) continue;
			index = pService->m_order[i];

			// 显示标题
			DrawText(pDC,
				CRect(MSG_TITLE_START_X, MSG_HEIGHT * index, MSG_TITLE_END_X, MSG_HEIGHT * (index + 1)),
				pService->m_InitialInfo[i].title, INI_FONT_NAME, INI_FONT_SIZE, FW_NORMAL, INI_FONT_COLOR/*BLACK*/,
				DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			// 显示结果
			CString expMsg = GetResultText(pService->m_InitialInfo[i].result);
			if (pService->m_InitialInfo[i].result == CAllDeviceStatusSvc::FAIL)
			{
				expMsg = pService->m_InitialInfo[i].errorInfo;
				//expMsg += pService->m_InitialInfo[i].dealInfo;
			}
			DrawText(pDC,
				CRect(MSG_RESULT_START_X, MSG_HEIGHT * index, MSG_RESULT_END_X, MSG_HEIGHT * (index + 1)),
				expMsg, INI_FONT_NAME, INI_FONT_SIZE, FW_BOLD, GetResultColor(pService->m_InitialInfo[i].result),
				DT_LEFT|DT_VCENTER|DT_SINGLELINE);
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
	void CAllDeviceStatusMaintainDlg::ShowInitRessult(int index)
	{
		CAllDeviceStatusSvc* pService = (CAllDeviceStatusSvc*)GetService();
		if (pService->m_order[index] == CAllDeviceStatusSvc::ROW_INVALID) return;

		index = pService->m_order[index];
		InvalidateRect(CRect(MSG_RESULT_START_X, MSG_HEIGHT * (index == 0 ? 1: index), MSG_RESULT_END_X, MSG_HEIGHT * (index + 1)));
		UpdateWindow();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      初始化标题显示

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CAllDeviceStatusMaintainDlg::InitTitle(){
		InvalidateRect(CRect(MSG_TITLE_START_X, MSG_HEIGHT, MSG_TITLE_END_X, MSG_HEIGHT * 12));
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
	CString CAllDeviceStatusMaintainDlg::GetResultText(CAllDeviceStatusSvc::INIT_RESULT result)
	{
		switch (result)
		{
		case CAllDeviceStatusSvc::SUCCESSFUL:                 // 成功
			return _opl(INI_CHECK_SUCCESS);
		case CAllDeviceStatusSvc::FAIL:                    // 失败
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
	COLORREF CAllDeviceStatusMaintainDlg::GetResultColor(CAllDeviceStatusSvc::INIT_RESULT result)
	{
		switch (result)
		{           
		case CAllDeviceStatusSvc::SUCCESSFUL:			// 成功
			return INI_RESULT_SUCCESS_COLOR/*BLACK*/;          
		case CAllDeviceStatusSvc::FAIL:			// 失败
			return INI_RESULT_FAILED_COLOR/*RED*/;
		default:						// 未知状态
			return INI_RESULT_UNKNOW_COLOR/*BLACK*/;
		}
	}
