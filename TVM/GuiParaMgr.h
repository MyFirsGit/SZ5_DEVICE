/*
 *  乘客线路界面管理类
 */
#pragma once
#include "stdafx.h"
#define theGUIMAP_MGR CGuiParaMgr::GetInstance()


/*
 *  数据结构定义
 */
/*  线路或者总图信息  */
typedef struct _line_map_layout_{
	DWORD	dwLineId;
	CPoint	startPoint;
	CSize	mapSize;
	CImage*	pImageBackupground;
	bool	bEnable;
	CString szBackupground;//总图
	_line_map_layout_(){
		dwLineId = 0;
		startPoint.x = 0;
		startPoint.y = 0;

		mapSize.cx = 0;
		mapSize.cy = 0;

		pImageBackupground = NULL;
		bEnable = false;
		szBackupground = _T("");
	}
}MAP_LAYOUT;

/*  线路或者总图按钮信息 */
typedef struct _line_map_btn_info_{
	DWORD dwBtnId;						// 按钮值
	CPoint startBntPiont;				// 按钮起始位置
	CSize	sizeOfBtn;					// 按钮大小
	CImage* pBtnImage;					// 按钮背景图
	bool bEnable;						// 按钮是否可用

	_line_map_btn_info_(){
		dwBtnId = 0;
		startBntPiont.x = 0;
		startBntPiont.y = 0;
		sizeOfBtn.cx = 0;
		sizeOfBtn.cy = 0;
		pBtnImage = NULL;
		bEnable = false;
	}

}MAP_BTN_INFO;

/*  底图  */
typedef struct _line_language_image_{
	WORD wLineId;
	LANGUAGETYPE_T language;
	CImage* pImage;
	CString szImageName;
	_line_language_image_(){
		wLineId = 0;
		language = LANGUAGETYPE_T::MODE_UNKNOWN;
		pImage = NULL;	
		szImageName = _T("");
	}

}LINE_LAUGUAGE_IMAGE;

class CGuiParaMgr
{
public:
	static CGuiParaMgr& GetInstance(){
		return instance;
	}
	void InitLoadPicture();

	// 获取某条线路的底图(起始位置、大小、背景图、启用标识)
	void GetLineMapInfo(IN DWORD dwLineId,OUT MAP_LAYOUT& layout);
	// 获取指定线路的按钮信息（起始位置、大小、背景图、是否可用、按钮值）
	void GetLayoutBtnInfo(IN DWORD layoutId,OUT vector<MAP_BTN_INFO>& vecBtnInfo);

private:
	CImage* GetMapBackgroundImage(IN DWORD dwLindId,IN LANGUAGETYPE_T languageType);
	CString GetMapBackground(IN DWORD dwLindId,IN LANGUAGETYPE_T languageType);
	map<CString,LINE_LAUGUAGE_IMAGE> m_map_pic_info;
	void GetFileFromPath(IN CString strPath,IN CString strFormat,OUT map<CString,CString>& mapPathFiles);

	CImage* m_pNormalBtn;
private:
	CGuiParaMgr();
	~CGuiParaMgr();
	
	static CGuiParaMgr instance;

};