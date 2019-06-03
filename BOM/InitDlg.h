#pragma once
#include "OperationDlg.h"

#include "afxwin.h"
#include "InitSvc.h"

/**
@brief      ������������
*/
class CInitDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CInitDlg)
    DECLARE_MESSAGE_MAP()

public:
	CInitDlg(CService* pService);      // ���캯��
	~CInitDlg();                       // ��������

	enum { IDD = IDD_00001_INITIAL_DLG };       // �Ի���ID

    void ShowInitRessult(int index);            // ˢ�³��ڻ����

private:
    void OnDraw(CDC* pDC);                                      // ������ƺ���
    const TCHAR* GetResultText(CInitSvc::INIT_RESULT result);    // ��ȡ���ڻ��������
    COLORREF GetResultColor(CInitSvc::INIT_RESULT result);      // ��ȡ���ڻ������ɫ
};
