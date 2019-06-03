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

static const int MSG_HEIGHT				=  40;							// ���ָ߶�
static const int MSG_TITLE_START_X		=  100;							// ���ڻ���Ŀ������ʼ������
static const int MSG_TITLE_END_X		=  MSG_TITLE_START_X + 200;		// ���ڻ���Ŀ���ƽ���������
static const int MSG_RESULT_START_X		=  MSG_TITLE_END_X + 5;			// ���ڻ���Ŀ�����ʼ������
static const int MSG_RESULT_END_X		=  MSG_RESULT_START_X + 495;	// ���ڻ���Ŀ�������������

IMPLEMENT_DYNAMIC(CInitDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CInitDlg, COperationDlg)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CInitDlg::CInitDlg(CService* pService)
: COperationDlg(CInitDlg::IDD, OPERATOR_INIT_RECT, OPERATOR_BASE_BK_COLOR/*LIGHTBLUE*/, pService)
, m_OnlyTime(true)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CInitDlg::~CInitDlg()
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
void CInitDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	BOOL bShowInitProcess = theFunction_INFO.IsShowInitProcess();
	int index;
	CInitSvc* pService = (CInitSvc*)GetService();
	if (bShowInitProcess){	// ��ʾ��ʼ������
		for (int i = 0; i < CInitSvc::ROW_MAX; i++)
		{
			// �ж��Ƿ���ʾ��ǰ��
			if (pService->m_order[i] == CInitSvc::ROW_INVALID) continue;
			index = pService->m_order[i];

			// ��ʾ����
			DrawText(pDC,
				CRect(MSG_TITLE_START_X, MSG_HEIGHT * index, MSG_TITLE_END_X, MSG_HEIGHT * (index + 1)),
				pService->m_InitialInfo[i].title, INI_FONT_NAME, INI_FONT_SIZE, FW_NORMAL, INI_FONT_COLOR/*BLACK*/,
				DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			// ��ʾ���
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
	}else{	// ����ʾ��ʼ������ 
		CString strResultCN = _T("ϵ�y��ʼ����");
		CString strResultEN = _T("System Initializing");
		CString strResultPN = _T("Inicializando o sistema");
		BOOL bAllSucceed = TRUE;
		BOOL bInitDone = pService->m_InitialInfo[CInitSvc::ROW_PRINT].result == CInitSvc::UNKNOWN ? FALSE : TRUE;
		if ( bInitDone ){														// ��ʼ�����
			for (int nIndex(0); nIndex < CInitSvc::ROW_MAX;nIndex ++ ){
				if (pService->m_InitialInfo[nIndex].result == CInitSvc::FAIL){	// ��һ��ʧ�ܾ���ʧ��
					bAllSucceed = FALSE;
				}
			}
			if (bAllSucceed){													// ��ʼ���ɹ�
				strResultCN = _T("ϵ�y��ʼ���ɹ�");
				strResultEN = _T("Initialize Sucesfuly");
				strResultPN = _T("Inicializar Sucesfuly");
			}
			else{																 // ��ʼ��ʧ��
				strResultCN = _T("ϵ�y��ʼ��ʧ��");
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
@brief      ˢ�³��ڻ����

@param      (i)int index    

@retval     ��

@exception  ��
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
@brief      ��ȡ���ڻ��������

@param      (i)CInitSvc::INIT_RESULT result   

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CInitDlg::GetResultText(CInitSvc::INIT_RESULT result)
{
	switch (result)
	{
	case CInitSvc::SUCCESS:                 // �ɹ�
		return _opl(INI_CHECK_SUCCESS);
	case CInitSvc::FAIL:                    // ʧ��
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
COLORREF CInitDlg::GetResultColor(CInitSvc::INIT_RESULT result)
{
	switch (result)
	{           
	case CInitSvc::SUCCESS:			// �ɹ�
		return INI_RESULT_SUCCESS_COLOR/*BLACK*/;          
	case CInitSvc::FAIL:			// ʧ��
		return INI_RESULT_FAILED_COLOR/*RED*/;
	default:						// δ֪״̬
		return INI_RESULT_UNKNOW_COLOR/*BLACK*/;
	}
}
