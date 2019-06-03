#pragma once
#include "IssueSvc.h"
#include "BaseDlg.h"
#include "ImageZoom.h"
const static int DEFAULT_LAYOUT = 0; // Ĭ�ϵ�һ��layout����
class CIssueSvc;

class CIssueLocationDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CIssueLocationDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueLocationDlg(CWnd*,CService*);   // standard constructor
	virtual ~CIssueLocationDlg();

// Dialog Data
	enum { IDD = IDD_STATION_DLG };
	void ReFrush();
	void Invalidate();
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CreateButton();
	PointF    m_mouseDown; //����������ʱ�������ͼƬ�ؼ��е�λ�ã�  
	bool     m_mousepressed; //��¼�Ƿ�����������  
	bool InitializeLineGroup(DWORD layoutID = DEFAULT_LAYOUT);
	void CheckLFBtnSelect(CPoint);//���������������жϵ�ǰվ��
	BOOL RectTrans(CPoint);
private:
	CIssueSvc* pSvc;
	CLineGroup* m_lineGroupArea;
	UIINFO m_LineAreaUIInfo;
	vector<UIINFO> m_vec_BtnUIInfo;
	vector<BTN_INFO> m_vec_buttonInfos;
	vector<TEXT_INFO> m_vec_textInfos;
	vector<LocationNumber_t> m_vec_hotLocation;
private:
	void OnDraw(CDC* pDC);
	void  RecalculateBtnInfo();
	CDC* m_pDC;
	CWnd* m_pWnd;
	CPictureEx* pStatic;
	CImageZoom* m_pImageZoom;
	CWnd *pMainWnd;
	CStatic* m_btnLeft;
	CStatic* m_btnTop;
	CStatic* m_btnRight;
	CStatic* m_btnBottom;
	Image* m_leftImage;
	Image* m_topImage;
	Image* m_rightImage;
	Image* m_bottomImage;

};
