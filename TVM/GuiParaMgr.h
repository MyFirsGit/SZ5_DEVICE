/*
 *  �˿���·���������
 */
#pragma once
#include "stdafx.h"
#define theGUIMAP_MGR CGuiParaMgr::GetInstance()


/*
 *  ���ݽṹ����
 */
/*  ��·������ͼ��Ϣ  */
typedef struct _line_map_layout_{
	DWORD	dwLineId;
	CPoint	startPoint;
	CSize	mapSize;
	CImage*	pImageBackupground;
	bool	bEnable;
	CString szBackupground;//��ͼ
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

/*  ��·������ͼ��ť��Ϣ */
typedef struct _line_map_btn_info_{
	DWORD dwBtnId;						// ��ťֵ
	CPoint startBntPiont;				// ��ť��ʼλ��
	CSize	sizeOfBtn;					// ��ť��С
	CImage* pBtnImage;					// ��ť����ͼ
	bool bEnable;						// ��ť�Ƿ����

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

/*  ��ͼ  */
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

	// ��ȡĳ����·�ĵ�ͼ(��ʼλ�á���С������ͼ�����ñ�ʶ)
	void GetLineMapInfo(IN DWORD dwLineId,OUT MAP_LAYOUT& layout);
	// ��ȡָ����·�İ�ť��Ϣ����ʼλ�á���С������ͼ���Ƿ���á���ťֵ��
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