#pragma once
#include "Panel.h"
#include <Func.h>

/** 
@brief   ��װ������ʾ
*/
class CLable:public CPanel
{
	DECLARE_DYNAMIC(CLable)

	DECLARE_MESSAGE_MAP()

public:
	
	CLable(UIINFO& uIInfo,CString text);

	virtual ~CLable();																					//��������


	CString GetText();																			//ȡ����ʾ����											
	void SetText(CString text);																	//������ʾ����

	void SetPaintFunc(Func<void (CDC*,CLable*)> * pPaintFunc);							//���û�ͼʱ�õĺ���

	virtual void SetUIInfo(UIINFO& uIInfo);

	virtual BOOL Create();

private:

	CString m_Text;				//����
	Func<void (CDC*,CLable*)> * m_OnPaintFunc;//���ƺ���
	CSize GetTextSize();
	afx_msg void OnPaint();



};



