#pragma once
#include "OperationDlg.h"

#include "afxwin.h"
#include "InitSvc.h"

/**
@brief      初期启动画面
*/
class CInitDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CInitDlg)
    DECLARE_MESSAGE_MAP()

public:
	CInitDlg(CService* pService);      // 构造函数
	~CInitDlg();                       // 析构函数

	enum { IDD = IDD_00001_INITIAL_DLG };       // 对话框ID

    void ShowInitRessult(int index);            // 刷新初期化结果

private:
    void OnDraw(CDC* pDC);                                      // 画面绘制函数
    const TCHAR* GetResultText(CInitSvc::INIT_RESULT result);    // 获取初期化结果文字
    COLORREF GetResultColor(CInitSvc::INIT_RESULT result);      // 获取初期化结果颜色
};
