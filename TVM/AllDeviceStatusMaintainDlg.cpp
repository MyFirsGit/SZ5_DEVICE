#include "stdafx.h"
#include "AllDeviceStatusSvc.h"
#include "AllDeviceStatusMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static const int MSG_HEIGHT				=  33;							// ���ָ߶�
static const int MSG_TITLE_START_X		=  100;							// ���ڻ���Ŀ������ʼ������
static const int MSG_TITLE_END_X		=  MSG_TITLE_START_X + 200;		// ���ڻ���Ŀ���ƽ���������
static const int MSG_RESULT_START_X		=  MSG_TITLE_END_X + 5;			// ���ڻ���Ŀ�����ʼ������
static const int MSG_RESULT_END_X		=  MSG_RESULT_START_X + 495;	// ���ڻ���Ŀ�������������

IMPLEMENT_DYNAMIC(CAllDeviceStatusMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CAllDeviceStatusMaintainDlg, COperationDlg)	
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���캯��

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
	@brief      ��������

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
	@brief      ������ƺ���

	@param      (i)CDC* pDC  �豸������

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CAllDeviceStatusMaintainDlg::OnDraw(CDC* pDC)
	{
		__super::OnDraw(pDC);

		int index;
		CAllDeviceStatusSvc* pService = (CAllDeviceStatusSvc*)GetService();
		for (int i = 0; i < CAllDeviceStatusSvc::ROW_MAX; i++)
		{
			// �ж��Ƿ���ʾ��ǰ��
			if (pService->m_order[i] == CAllDeviceStatusSvc::ROW_INVALID) continue;
			index = pService->m_order[i];

			// ��ʾ����
			DrawText(pDC,
				CRect(MSG_TITLE_START_X, MSG_HEIGHT * index, MSG_TITLE_END_X, MSG_HEIGHT * (index + 1)),
				pService->m_InitialInfo[i].title, INI_FONT_NAME, INI_FONT_SIZE, FW_NORMAL, INI_FONT_COLOR/*BLACK*/,
				DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			// ��ʾ���
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
	@brief      ˢ�³��ڻ����

	@param      (i)int index    

	@retval     ��

	@exception  ��
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
	@brief      ��ʼ��������ʾ

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CAllDeviceStatusMaintainDlg::InitTitle(){
		InvalidateRect(CRect(MSG_TITLE_START_X, MSG_HEIGHT, MSG_TITLE_END_X, MSG_HEIGHT * 12));
		UpdateWindow();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ȡ���ڻ��������

	@param      (i)CInitSvc::INIT_RESULT result   

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	CString CAllDeviceStatusMaintainDlg::GetResultText(CAllDeviceStatusSvc::INIT_RESULT result)
	{
		switch (result)
		{
		case CAllDeviceStatusSvc::SUCCESSFUL:                 // �ɹ�
			return _opl(INI_CHECK_SUCCESS);
		case CAllDeviceStatusSvc::FAIL:                    // ʧ��
			return _opl(INI_CHECK_FAILURE);
		default:								// δ֪״̬
			return _opl(TXT_UNDEFINE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ȡ���ڻ������ɫ

	@param      (i)CInitSvc::INIT_RESULT result      

	@retval     COLORREF

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	COLORREF CAllDeviceStatusMaintainDlg::GetResultColor(CAllDeviceStatusSvc::INIT_RESULT result)
	{
		switch (result)
		{           
		case CAllDeviceStatusSvc::SUCCESSFUL:			// �ɹ�
			return INI_RESULT_SUCCESS_COLOR/*BLACK*/;          
		case CAllDeviceStatusSvc::FAIL:			// ʧ��
			return INI_RESULT_FAILED_COLOR/*RED*/;
		default:						// δ֪״̬
			return INI_RESULT_UNKNOW_COLOR/*BLACK*/;
		}
	}
