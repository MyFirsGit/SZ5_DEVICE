#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"
#include "CGuiParm.h"
#include "AllLineShowBox.h"

class CIssueSvc;

/**
@brief ��Ʊ·��ͼ���棨Ĭ�ϴ������棩
*/
class CIssueAreaMapDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueAreaMapDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueAreaMapDlg(CService*);
	~CIssueAreaMapDlg();
	
	// �Ի�������
	enum { IDD = IDD_10101_ISSUE_DLG };

	// ��Ʊ�Ҳ๦������ť�����
	typedef enum _tagIssueComponents
	{
		LINE_SELECT_BUTTON			= 0x0001,	// ��·��ť
		PRICE_BUTTON				= 0x0002,	// Ʊ�۰�ť
		NUMBER_BUTTON				= 0x0004,	// ������ť
		TOP_STATION_BUTTON			= 0x0008,	// �ȵ㳵վ��ť		
		FUNC_SELECT_BUTTON			= 0x0010,	// ѡ���ܰ�ť
	};

	enum {MAX_TICKET_INFO_ROWS = 18};

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnProductTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSubProductClick(WPARAM wParam,LPARAM lParam);
	LRESULT XLineGroupButtonClick(WPARAM wParam, LPARAM lParam);

	// ��ǰ��
	LRESULT OnLineGroupClick(WPARAM wParam,LPARAM lParam);		// ��·ѡ��
	LRESULT OnPriceTypeClick(WPARAM wParam,LPARAM lParam);		// Ʊ��ѡ��
	LRESULT OnTicketNumClick(WPARAM wParam,LPARAM lParam);		// ��Ʊ����ѡ��
	LRESULT OnTopStationClick(WPARAM wParam,LPARAM lParam);		// �ȵ㳵վѡ��
	LRESULT OnFunctionClick(WPARAM wParam,LPARAM lParam);		// ����ѡ��

	bool IsSupportIssueCard();

protected:
	void OnClientLanguageModeChanged(LANGUAGETYPE_T&);	  //���Ա仯���ƺ���
	//void changeUiLanguage(LANGUAGETYPE_T&);
private:
	void ClearLabelData();
private:
	CIssueSvc* pSvc;
	CAllLineShowBox* m_pAllLineDlg;
};
