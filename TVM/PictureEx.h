#pragma once
const RECT InitRECT = { 0, 0, 0, 0 };
class CGdiPlusIni
{
public:
	CGdiPlusIni();
	~CGdiPlusIni();
private:
	Gdiplus::GdiplusStartupInput	g_gdiplusStart;
	ULONG_PTR						g_gdiplusTokens;

	static CGdiPlusIni gdiplusInstance;
};

class CPictureEx:
	public CStatic
{
private:
	class CPicturePlus
	{
	public:
		CPicturePlus();
		~CPicturePlus();

		void loadPic(const TCHAR* tcpath,bool bAutoDel = false);

		Gdiplus::Image* getCurrentImage();
		Gdiplus::Image* getSpecialFrame(unsigned long uiSpecialFrame);
		unsigned long	getSpecialFrameDelayTime(unsigned long uiSpecialFrame);
		unsigned long	getFrameCount();
		unsigned long	getFrameWidth();
		unsigned long	getFrameHeight();
		void unload();
		unsigned long	getCurFrameIndex();
		void			setCurFrame(unsigned long uiFrameIndex);
		void			setNextFrame();
	private:
		Gdiplus::Image*			m_pImage;
		vector<Gdiplus::Image*>	m_vecImage;
		map<CString,Gdiplus::Image*> m_mapImages;
		Gdiplus::PropertyItem*	m_pItem;
		unsigned long	m_ulCurFrame;
		unsigned long	m_ulTotalFrame;
		void selectFrame(unsigned long ulFrame);
		TCHAR					m_tcPicPath[MAX_PATH];
		bool m_bAutoDel;
	public:
		static BOOL bIniGdiplus;
	};
public:
	void loadPic(const _TCHAR* tcPicPath,bool bAutoDel = false);
	BOOL createPicWnd(HWND hParent, RECT rc = InitRECT);
	void pauseRender();
	void resumeRender();
	void unloadPic();
private:
	void render();
	void stopRender();
public:
	CPictureEx();
	~CPictureEx();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg LRESULT OnRender(WPARAM,LPARAM);
public:
	CPicturePlus			m_picfile;
	Gdiplus::Image*			m_pCurrentFrame;
public:
	HANDLE	m_ExitThread;
	BOOL	m_bExitThread;
	BOOL	m_bPauseThread;
	HANDLE	m_PauseThread;
	HANDLE	m_hWaitPaused;
	HANDLE	m_hRenderThread;
	HANDLE m_hWaitFrame;
public:
	void drawFrame();
};