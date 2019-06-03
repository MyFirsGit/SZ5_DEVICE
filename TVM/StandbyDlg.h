#pragma once
#include "BaseDlg.h"
#include "StandbySvc.h"
#include "TVM.h"
#include "PictureEx.h"

/**
@brief      ������Ϣ����
*/

class CStandbyDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStandbyDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStandbyDlg(CService* pService);
	~CStandbyDlg();

	enum { IDD = IDD_20901_SYSTEM_OUT_DLG };						// �Ի�������

	enum _timer
	{
		PLAY_FONT_TIMER,		// �����������ݶ�ʱ��
		PLAY_PICTURE_TIMER		// ����ͼƬ���ݶ�ʱ��
	};

	enum _playtype
	{
		PLAY_TYPE_FONT = 1,		// ���Ź������������
		PLAY_TYPE_PICTURE		// ���Ź����ͼƬ����
	}playtype;

	enum _FontPlayMode
	{
		RTOL_ROLL = 1,			// ���ҵ��󲥷�
		DTOU_ROLL,				// ���µ��ϲ���
		LTOR_ROLL,				// �����Ҳ���
		UTOD_ROLL				// ���ϵ��²���
	}FontPlayMode;
	
	typedef struct _fontStruct
	{	 
		CString SerialNo;		// ���ֲ���˳��
		CString Font;			// ����
		CString Size;			// ��С
		CString Color;			// ��ɫ
		CString TimeLength;		// ����ʱ��
		CString Content;		// ��������
		CString DisplayMode;	// ���ŷ�ʽ
		CString Speed;			// �����ٶ�
		_fontStruct()
		{
			SerialNo = _T("");
			Font = _T("");
			Size = _T("");
			Color = _T("");
			TimeLength = _T("");
			Content = _T("");
			DisplayMode = _T("");
			Speed = _T("");
		}
	}FontStruct;

	typedef struct _pictureStruct
	{
		CString SerialNo;				// ͼƬ�����󲥷�˳��
		CString TimeLength;				// ͼƬ��沥��ʱ��
		CString FilePathName;			// ͼƬλ��
		_pictureStruct()
		{
			SerialNo = _T("");
			TimeLength = _T("");
			FilePathName = _T("");
		}
	}PictureStruct;

	typedef struct _adStruct
	{
		FontStruct  fontstruct;							// ���ֹ��
		PictureStruct picturestruct;					// ͼƬ���
	}AdStruct;

	BOOL IsTimeUp(IN DWORD dwTimer,IN DWORD dwBaseTime);																	//  �ж�ʱ�䴦��ִ���Ƿ�ʱ

	void PlayAdFont(CDC* pDC,CString Font,CString size,CString Color,CString content,CString PlayMode,CString speed);		// ����ͬ���ŷ�ʽ���Ź���е���������
	void PlayAdPicture(CString FilePathName);	

	void GetAdvertisementInfo();																							// ��ȡ�����Ϣ
	static bool sortfontbyserialno(CParseXML::FontStruct fontstruct1,CParseXML::FontStruct fontstruct2);					// ���尴SerialNo����
	static bool sortpicturebyserialno(CParseXML::PictureStruct picturestruct1,CParseXML::PictureStruct picturestruct2);		// ͼƬ��SerialNo����
	static bool sortadbyserialno(CParseXML::AdStruct ad_object1,CParseXML::AdStruct ad_object2);							// ��水SerialNo����
protected:
	void UpdateUI();
private:
	int font_right;										// ��������ֶ������ҵ��󲥷�right����
	int font_left;										// ��������ֶ��������Ҳ���left����			
	int font_bottom;									// ��������ֶ������µ��ϲ���bottom����	
	int font_top;										// ��������ֶ������ϵ��²���top����
	CDC m_memDC;
	CBitmap m_menBit;

	CImage image;
	CPictureEx pictureex;
	virtual void OnDraw(CDC* pDC);                                  // ������ƺ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);			// ��Ļ����˳���沥��
	void OnTimer(UINT nIDEvent);
private:
	AdStruct adstruct;
	vector<CParseXML::AdStruct>AdvertisementInfo;
	int Adsz;
	int AdNowNumber;
	int FontNowNumber;
	int PictureNowNumber;
	int FirstTime;
	DWORD PlayFont_OriSeconds;							// ��������ֲ��Ż���ʱ��
	DWORD PlayPicture_OriSeconds;						// �����ͼƬ���Ż���ʱ��
	bool m_bIsPlayFont;									// ����Ƿ񲥷����֣�
	bool m_bIsPlayPic;									// ����Ƿ񲥷�ͼƬ��
private:
	int PlayType;										// ��������֡�ͼƬ����Ƶ�����л���־	
	bool m_bIsShowUp;									// �V���Ƿ��ѽ��@ʾ��
	bool m_bIsRenderGif;								// GIFͼƬ�Ƿ񲥷ţ�
};

