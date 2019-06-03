#include "stdafx.h"
#include "PictureEx.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

CGdiPlusIni CGdiPlusIni::gdiplusInstance;

CGdiPlusIni::CGdiPlusIni()
{
	// Initialize GDI+.
	Gdiplus::Status startupStatus = Gdiplus::GdiplusStartup(&g_gdiplusTokens, &g_gdiplusStart, NULL);
}


CGdiPlusIni::~CGdiPlusIni()
{
	Gdiplus::GdiplusShutdown(g_gdiplusTokens);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_GUID(FrameDimensionTime, 0x6aedbd6d, 0x3fb5, 0x418a, 0x83, 0xa6, 0x7f, 0x45, 0x22, 0x9d, 0xc8, 0x72);

CPictureEx::CPicturePlus::CPicturePlus()
{
	m_pImage = NULL;
	m_pItem = NULL;
	m_bAutoDel = false;
}

void CPictureEx::CPicturePlus::unload()
{
	if (NULL != m_pImage)
	{
		//m_pOldImage = m_pImage;
		//delete m_pImage;
		if(m_bAutoDel)
			m_vecImage.push_back(m_pImage);
		m_pImage = NULL;
	}

	if (NULL != m_pItem)
	{
		free(m_pItem);
		m_pItem = NULL;
	}
	m_ulCurFrame = 0;
	m_ulTotalFrame = 0;
}

CPictureEx::CPicturePlus::~CPicturePlus()
{
	unload();
}

void CPictureEx::CPicturePlus::loadPic(const TCHAR* tcpath,bool bAutoDel)
{
	if (PathFileExists(tcpath))
	{
		// 判断这些文件是否需要自动删除
		m_bAutoDel = bAutoDel;

		if(!m_bAutoDel){
			CString strFileName;
			strFileName.Format(_T("%s"),tcpath);

			// Get file name
			strFileName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);

			// Check map has this file
			auto it = m_mapImages.find(strFileName);

			if(it != m_mapImages.end()){
				m_pImage = it->second;
			}
			else{
				m_pImage = Gdiplus::Image::FromFile(tcpath);

				if(NULL != m_pImage){
					m_mapImages[strFileName] = m_pImage;
				}
			}
		}
		else{
			m_pImage = Gdiplus::Image::FromFile(tcpath);
		}
		
		
		if (NULL != m_pImage)
		{
			_tcscpy_s(m_tcPicPath,MAX_PATH,tcpath);
			GUID guid;
			WCHAR strGuid[39];

			// Get the GUID that identifies the image format.
			Gdiplus::Status statusFormat = m_pImage->GetRawFormat(&guid);
			if (statusFormat==Gdiplus::Ok)
			{
				// Convert the GUID to a string and display it.
				StringFromGUID2(guid, strGuid, 39);
				wprintf(L"The GUID is %s.\n", strGuid);
				// Was the GUID that identifies the JPEG format retrieved?
				if (guid == Gdiplus::ImageFormatGIF)
				{
					//gif 圖片
					UINT uiFramCount = 0;
					uiFramCount = m_pImage->GetFrameDimensionsCount();
					GUID *pDimensionIDs = (GUID*)new GUID[uiFramCount];
					if (NULL != pDimensionIDs)
					{
						Gdiplus::Status getStatus = m_pImage->GetFrameDimensionsList(pDimensionIDs, uiFramCount);
						if (getStatus == Gdiplus::Ok)
						{
							WCHAR strGuid[39];
							StringFromGUID2(pDimensionIDs[0], strGuid, 39);
							m_ulTotalFrame = m_pImage->GetFrameCount(&pDimensionIDs[0]);

							int size = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
							m_pItem = (Gdiplus::PropertyItem*)malloc(size);
							if (NULL != m_pItem)
							{
								m_pItem->id = 0;
								m_pItem->length = 0;
								m_pItem->type = 0;
								m_pItem->value = NULL;
							}
							m_pImage->GetPropertyItem(PropertyTagFrameDelay, size, m_pItem);
						}
						delete[]pDimensionIDs;
						pDimensionIDs = NULL;
					}
				}
				else
				{
					//不是Gif圖片
				}
			}

		}

		// 释放资源
		if(m_bAutoDel){
			if(m_vecImage.size() >= 6){
				for(vector<Gdiplus::Image*>::iterator it = m_vecImage.begin(); it != m_vecImage.end();it++){
					if(NULL != *it){
						delete *it;
						*it = NULL;
					}
				}
				m_vecImage.clear();
			}
		}
	}
}

unsigned long	CPictureEx::CPicturePlus::getFrameWidth()
{
	ULONG ulret = 0;
	if (NULL != m_pImage)
	{
		ulret = m_pImage->GetWidth();
	}
	return ulret;
}

unsigned long	CPictureEx::CPicturePlus::getFrameHeight()
{
	ULONG ulret = 0;
	if (NULL != m_pImage)
	{
		ulret = m_pImage->GetHeight();
	}
	return ulret;
}

unsigned long	CPictureEx::CPicturePlus::getSpecialFrameDelayTime(unsigned long uiSpecialFrame)
{
	unsigned long ulret = INFINITE;
	if (NULL != m_pItem && NULL != m_pImage)
	{
		if (NULL != m_pItem->value)
		{
			ASSERT(uiSpecialFrame < m_ulTotalFrame);
			if (NULL!=m_pItem->value)
			{
				ulret = ((long*)m_pItem->value)[uiSpecialFrame] * 10;
			}
			TRACE(_T("Frame is %d,Delay Time is %d"), uiSpecialFrame, ulret);
		}
	}
	return ulret;
}

unsigned long CPictureEx::CPicturePlus::getFrameCount()
{
	return m_ulTotalFrame;
}

void CPictureEx::CPicturePlus::selectFrame(unsigned long ulFrame)
{
	GUID    Guid = FrameDimensionTime;
	if (NULL != m_pImage)
	{
		ASSERT(m_ulCurFrame < m_ulTotalFrame);
		try
		{
			Gdiplus::Status selectStatus = m_pImage->SelectActiveFrame(&Guid, m_ulCurFrame);
			if (selectStatus == Gdiplus::Ok)
			{
				TRACE(_T("Select Frame Success\n"));
			}
		}
		catch(...)
		{
		}
	}
}

Gdiplus::Image* CPictureEx::CPicturePlus::getCurrentImage()
{
	return m_pImage;
}

unsigned long	CPictureEx::CPicturePlus::getCurFrameIndex()
{
	return m_ulCurFrame;
}

void CPictureEx::CPicturePlus::setCurFrame(unsigned long uiFrameIndex)
{
	m_ulCurFrame=uiFrameIndex;
}

void CPictureEx::CPicturePlus::setNextFrame()
{
	if(m_ulCurFrame<(m_ulTotalFrame-1))
	{
		m_ulCurFrame++;
	}else
	{
		m_ulCurFrame=0;
	}
}

Gdiplus::Image* CPictureEx::CPicturePlus::getSpecialFrame(unsigned long uiSpecialFrame)
{
	if (NULL != m_pImage){
		ASSERT(uiSpecialFrame< m_ulTotalFrame);
		selectFrame(uiSpecialFrame);
	}
	return m_pImage;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define UD_WM_RENDER (WM_USER+103)

CPictureEx::CPictureEx()
{
	m_ExitThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_PauseThread=CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hWaitFrame=CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hWaitPaused=CreateEvent(NULL, TRUE, FALSE, NULL);

	m_pCurrentFrame = NULL;
	m_bExitThread = TRUE;
}


CPictureEx::~CPictureEx()
{
	stopRender();
	WaitForSingleObject(m_ExitThread, INFINITE);

	if (NULL != m_ExitThread && INVALID_HANDLE_VALUE != m_ExitThread)
	{
		CloseHandle(m_ExitThread);
		m_ExitThread  = NULL;
	}

	if (NULL != m_PauseThread && INVALID_HANDLE_VALUE != m_PauseThread)
	{
		CloseHandle(m_PauseThread);
		m_PauseThread = NULL;
	}

	if (NULL != m_hWaitFrame && INVALID_HANDLE_VALUE != m_hWaitFrame)
	{
		CloseHandle(m_hWaitFrame);
		m_hWaitFrame = NULL;
	}

	if (NULL != m_hWaitPaused && INVALID_HANDLE_VALUE != m_hWaitPaused)
	{
		CloseHandle(m_hWaitPaused);
		m_hWaitPaused = NULL;
	}

	if (IsWindow(this->m_hWnd))
	{
		this->DestroyWindow();
	}
}

BEGIN_MESSAGE_MAP(CPictureEx, CStatic)
	ON_WM_PAINT()
	ON_MESSAGE(UD_WM_RENDER,OnRender)
END_MESSAGE_MAP()


void CPictureEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	
	if(!m_bPauseThread)
	{
		if(NULL != m_pCurrentFrame)
		{
			CRect rcClient;
				GetClientRect(rcClient);
			Gdiplus::Graphics graph(dc.m_hDC);
			graph.DrawImage(m_pCurrentFrame,rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
			graph.ReleaseHDC(dc.m_hDC);
		}
	}
}

void CPictureEx::stopRender()
{
	if(m_bExitThread==FALSE)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_ExitThread, INFINITE);
	}
}

void CPictureEx::pauseRender()
{
	m_bPauseThread=TRUE;
	SetEvent(m_hWaitFrame);		//優化加速
	DWORD dwret=WaitForSingleObject(m_hWaitPaused,1000);	//等待暫停成功
	ResetEvent(m_hWaitPaused);		//等待結束
}

void CPictureEx::resumeRender()
{
	m_bPauseThread=FALSE;
	ResetEvent(m_hWaitFrame);	//回復等待 不優化加速
	m_pCurrentFrame = m_picfile.getCurrentImage();
	SetEvent(m_PauseThread);		//恢復運行信號
	return;
}	

LRESULT CPictureEx::OnRender(WPARAM,LPARAM)
{
	CRect rcWndRect;
	this->GetWindowRect(rcWndRect);
	CWnd* parent = this->GetParent();
	/*parent->ScreenToClient(rcWndRect);*/
	CWnd* pParent=parent->GetParent();
	pParent->ScreenToClient(rcWndRect);
	//pParent->InvalidateRect(rcWndRect,TRUE);
	//::InvalidateRect(parent->m_hWnd, rcWndRect, TRUE);
	::InvalidateRect(this->m_hWnd, NULL, FALSE);
	return 0;
}

void CPictureEx::drawFrame()
{
	CDC* pDc=GetDC();
	if(NULL!=pDc && NULL!=m_pCurrentFrame)
	{
		HDC hWndDc=pDc->m_hDC;
		CRect clientRect;
		GetClientRect(clientRect);
		//RedrawWindow(NULL,NULL,RDW_ERASENOW);
		Gdiplus::Graphics graph(hWndDc);
		graph.DrawImage(m_pCurrentFrame,clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height());
		graph.ReleaseHDC(hWndDc);
	}
}

UINT _stdcall renderPic(void* lParam)
{
	CPictureEx *picCtrl = reinterpret_cast<CPictureEx*> (lParam);
	if (NULL != picCtrl)
	{
		//循環顯示每一幀
		while(WAIT_OBJECT_0==WaitForSingleObject(picCtrl->m_PauseThread,INFINITE))
		{
			if (picCtrl->m_bExitThread)
			{
				break;
			}
			// 暂停显示
			if(picCtrl->m_bPauseThread || !IsWindowVisible(picCtrl->GetSafeHwnd()))
			{
				ResetEvent(picCtrl->m_PauseThread);
				SetEvent(picCtrl->m_hWaitPaused);
			}
			ULONG ulframeCount = picCtrl->m_picfile.getFrameCount();
			ULONG ulDelayTime = INFINITE;
			if(ulframeCount == 1 ){
				picCtrl->m_pCurrentFrame = picCtrl->m_picfile.getCurrentImage();
			}
			else if(ulframeCount > 1){
				unsigned long ulCurFrame=picCtrl->m_picfile.getCurFrameIndex();
				picCtrl->m_pCurrentFrame = picCtrl->m_picfile.getSpecialFrame(ulCurFrame);
				ulDelayTime = picCtrl->m_picfile.getSpecialFrameDelayTime(ulCurFrame);
				picCtrl->m_picfile.setNextFrame();
			}
			picCtrl->Invalidate(TRUE);
			//picCtrl->UpdateWindow();

			WaitForSingleObject(picCtrl->m_hWaitFrame, ulDelayTime);
		}
		SetEvent(picCtrl->m_ExitThread);
	}
	return 0;
}

BOOL CPictureEx::createPicWnd(HWND hParent, RECT rc)
{
	//需要創建
	BOOL bCreate=FALSE;
	CWnd* pWndParent = CWnd::FromHandlePermanent(hParent);
	if (NULL != pWndParent)
	{
		if (IsRectEmpty(&rc))
		{ 
			RECT rcParent;
			::GetClientRect(hParent, &rcParent);
			ULONG ulParentWndHeight = rcParent.bottom - rcParent.top;
			ULONG ulParentWndWidht = rcParent.right - rcParent.left;
			ULONG ulPicHeight = rc.bottom-rc.top;
			ULONG ulPicWidht = rc.right-rc.left;
			if(m_picfile.getCurrentImage()!=NULL)
			{
				ulPicHeight = m_picfile.getFrameHeight();
				ulPicWidht = m_picfile.getFrameWidth();
			}

			ULONG ulRectHeight = (ulPicHeight < ulParentWndHeight) ? ulPicHeight : ulParentWndHeight;
			ULONG ulRectWidht = (ulPicWidht < ulParentWndWidht) ? ulPicWidht : ulParentWndWidht;
				
			rc.left = 0; rc.top = 0;
			rc.right = rc.left + ulRectWidht;
			rc.bottom = rc.top + ulRectHeight;
		}
		bCreate = this->Create(_T("PicturePlusWnd"), WS_VISIBLE | WS_CHILD, rc, pWndParent, (unsigned int )this);

		ResetEvent(m_PauseThread);	//先挂起綫程 不讓其執行
		SetEvent(m_hWaitPaused);		//設置當前已經暫停成功
		render();
	}
	return bCreate;
}

void CPictureEx::render()
{
	unsigned int uithreadID = 0;
	m_bExitThread=FALSE;
	m_hRenderThread=(HANDLE)_beginthreadex(NULL, 0, renderPic, this, 0, &uithreadID);
	if (NULL != m_hRenderThread && INVALID_HANDLE_VALUE != m_hRenderThread)
	{
		CloseHandle(m_hRenderThread);
		m_hRenderThread = NULL;
	}
}

void CPictureEx::loadPic(const _TCHAR* tcPicPath,bool bAutoDel)
{
	pauseRender();
	m_picfile.unload();
	m_picfile.loadPic(tcPicPath,bAutoDel);
	m_pCurrentFrame=m_picfile.getSpecialFrame(0);
}

void CPictureEx::unloadPic()
{
	m_picfile.unload();
}