#include "stdafx.h"
#include "InitDlg.h"
#include "InitSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TXT_UNDOWN                 = _T("--");
static const TCHAR* const TXT_SUCCESS                = _T("�ɹ�");
static const TCHAR* const TXT_FAIL                   = _T("ʧ��");


static const int MSG_HEIGHT         =  90;                          // ���ָ߶�
static const int MSG_TITLE_START_X  =  100 + 400;                         // ���ڻ���Ŀ������ʼ������
static const int MSG_TITLE_END_X    =  MSG_TITLE_START_X + 300;     // ���ڻ���Ŀ���ƽ���������
static const int MSG_RESULT_START_X =  MSG_TITLE_END_X + 30;        // ���ڻ���Ŀ�����ʼ������
static const int MSG_RESULT_END_X   =  MSG_RESULT_START_X + 500;    // ���ڻ���Ŀ�������������

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
: COperationDlg(CInitDlg::IDD,INIT_FRAME_RECT,LIGHTBLUE, pService)
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


	CInitSvc* pService = (CInitSvc*)GetService();

	// ��ʾ��ʼ������
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
@brief      ˢ�³��ڻ����

@param      (i)int index    

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitDlg::ShowInitRessult(int index)
{
	InvalidateRect(CRect(MSG_RESULT_START_X, MSG_HEIGHT * index + 22, MSG_RESULT_END_X, MSG_HEIGHT * (index+1) + 22));
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
const TCHAR* CInitDlg::GetResultText(CInitSvc::INIT_RESULT result)
{
	const TCHAR* resultText = TXT_UNDOWN;

	switch (result) {
	case CInitSvc::UNDOWN:                  // δ֪״̬
		resultText = TXT_UNDOWN;
		break;

	case CInitSvc::SUCCESS:                 // �ɹ�
		resultText = TXT_SUCCESS;
		break;

	case CInitSvc::FAIL:                    // ʧ��
		resultText = TXT_FAIL;
		break;
	}

	return resultText;
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
	COLORREF color = BLACK;

	switch (result) {
	case CInitSvc::UNDOWN :                 // δ֪״̬
		color = BLACK;
		break;               
	case CInitSvc::SUCCESS :                // �ɹ�
		color = BLACK;
		break;                
	case CInitSvc::FAIL :                   // ʧ��
		color = RED;
		break;
	default:
		color = RED;
		break;
	}

	return color;
}
