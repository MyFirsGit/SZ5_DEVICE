#pragma once
#include "Panel.h"
#include <Func.h>

/** 
@brief   封装文字显示
*/
class CLable:public CPanel
{
	DECLARE_DYNAMIC(CLable)

	DECLARE_MESSAGE_MAP()

public:
	
	CLable(UIINFO& uIInfo,CString text);

	virtual ~CLable();																					//析构函数


	CString GetText();																			//取得显示内容											
	void SetText(CString text);																	//设置显示内容

	void SetPaintFunc(Func<void (CDC*,CLable*)> * pPaintFunc);							//设置绘图时用的函数

	virtual void SetUIInfo(UIINFO& uIInfo);

	virtual BOOL Create();

private:

	CString m_Text;				//内容
	Func<void (CDC*,CLable*)> * m_OnPaintFunc;//绘制函数
	CSize GetTextSize();
	afx_msg void OnPaint();



};



