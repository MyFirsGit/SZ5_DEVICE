#ifndef _CGUIPARM_H
#define _CGUIPARM_H

/** 
version 1.0.0
author mxw
time stamp 20140704 09.31

time stamp 20140714 17.31	���� getNext** ϵ�к����д���,Ӧ�ö���getNextLine

time stamp 20140716 10.00	���� UE_CSV_RESULT::UEUNKNOWN���������ļ���UE_UNKNOWN��ͻ������
*/
#include <list>
#include <map>
#include <vector>
#define UD_MAX_LINEWORD	1024	//����ÿ�����1024���ַ�
#define UD_MEM_ALIGNMEMENT 16
typedef enum _UE_CSV_RESULT
{
	UE_SUCCESS=0,	//�����ɹ�
	UE_ERRPATH=1,	//ָ��·�����ļ�������
	UE_FAILOPEN,	//���ļ�ʧ��
	UE_OUTMEM,		//�����
	UE_FAILE,		//����ʧ��
	UE_EMPTYLIST,	//������
	UE_LISTEND,		//����β��
	UE_UNKNOWNCSVRET//δ֪����
}UE_CSV_RESULT;

class CCsvReader
{
public:
	CCsvReader();
	~CCsvReader();
public:
	//inner class
	class CLineData
	{
	public:
		std::map<int ,const TCHAR*> m_mapLine;	
	private:
		std::map<int ,const TCHAR* >::iterator m_iter;
	public:
		CLineData();
		~CLineData();
	public:
		TCHAR* getFirstSplit();
		TCHAR* getNextSplit();	
		TCHAR* operator[](size_t index);
		size_t getMapSize();
	};
private:
	std::list<CLineData*>			m_listLines;
	std::list<CLineData*>::iterator	m_iter;
public:
	UE_CSV_RESULT	loadCsvFile(const TCHAR* tcpath,const TCHAR* tcsplit);
	CLineData*		getFirstLine();
	CLineData*		getNextLine();
	size_t			getLineCount();
private:
	UE_CSV_RESULT	splitLine(const TCHAR* tcLineData,const TCHAR* tcsplit);
};

typedef struct _US_LAYOUT_DATA
{
	typedef enum _UE_LAYOUT_INDEX
	{
		UE_PAGECODE=0,		//ҳ����
		UE_PAGEMARK,		//ҳ��ʶ
		UE_CHBASEMAPCODE,	//���ĵ�ͼ���
		UE_ENBASEMAPCODE,	//Ӣ�ĵ�ͼ���
		UE_PUBASEMAPCODE,	//���ĵ�ͼ���
		UE_TOPLEFTX,		//���Ͻ�X����
		UE_TOPLEFTY,		//���Ͻ�Y����
		UE_PICWIDTH,		//ͼƬ���
		UE_PICHEIGHT,		//ͼƬ�߶�
		UE_ISHOMEPAGE,		//�Ƿ��Ƿ�����ҳ
		UE_ENABLEAMRK,		//���ñ�־
		UE_ELEMENTENABLEMARK//Ԫ�����ñ�־
	}UE_LAYOUT_INDEX;

	TCHAR* pPageCode;		//ҳ����
	TCHAR* pPageMark;		//ҳ��ʶ
	TCHAR* pChBaseMapCode;	//���ĵ�ͼ���
	TCHAR* pEnBaseMapCode;	//Ӣ�ĵ�ͼ���
	TCHAR* pPuBaseMapCode;	//���ĵ�ͼ���
	TCHAR* PTopLeftX;		//���Ͻ�X����
	TCHAR* PTopLeftY;		//���Ͻ�Y����
	TCHAR* pPicWidth;		//ͼƬ���
	TCHAR* pPicHeight;		//ͼƬ�߶�
	TCHAR* pIsHomePage;		//�Ƿ���ҳ
	TCHAR* PEnableMark;		//���ñ�־
	TCHAR* pElementEnableMark;//Ԫ�����ñ�־

	_US_LAYOUT_DATA()
	{
		pPageCode=NULL;
		pPageMark=NULL;
		pChBaseMapCode=NULL;
		pEnBaseMapCode=NULL;
		pPuBaseMapCode=NULL;
		PTopLeftX=NULL;
		PTopLeftY=NULL;
		pPicWidth=NULL;
		pPicHeight=NULL;
		pIsHomePage=NULL;
		PEnableMark=NULL;
		pElementEnableMark=NULL;
	}
	~_US_LAYOUT_DATA()
	{
	}
}US_LAYOUT_DATA;

typedef struct _US_PIC_DATA
{
	TCHAR* pPicCode;
	TCHAR* pPicName;
	typedef enum _UE_PIC_INDEX
	{
		UE_PICCODE=0,
		UE_PICNAME
	}UE_PIC_INDEX;

	_US_PIC_DATA()
	{
		pPicCode=NULL;
		pPicName=NULL;
	}
	~_US_PIC_DATA()
	{
	}
}US_PIC_DATA;

typedef struct _US_BUTTON_DATA
{
	TCHAR* pBtnCode;			//��ť���
	TCHAR* pBtnPage;			//����ҳ���
	TCHAR* pBtnMark;			//��ť��ʶ
	TCHAR* pBtnValue;			//��ťֵ
	TCHAR* pBtnJumpPage;		//��תҳ����
	TCHAR* pBtnXPos;			//x����
	TCHAR* pBtnYPos;			//Y����
	TCHAR* pBtnWidth;			//��ť���
	TCHAR* pBtnHeight;			//��ť�߶�
	TCHAR* pBtnNormamPicCode;	//����ͼƬ���
	TCHAR* pBtnPressedPicCode;	//������ͼƬ���
	TCHAR* pBtnHoverPicCode;	//ѡ��ͼƬ���
	TCHAR* pBtnDisablePicCode;	//��ЧͼƬ���
	TCHAR* pBtnIsEnable;		//�Ƿ���ñ�־
	TCHAR* pBtnIsShowMark;		//�Ƿ���ʾ��־
	TCHAR* pBtnIsHotStation;	//�Ƿ�����վ��

	TCHAR* pChtxtPos;			//����λ��[��]
	TCHAR* pChcontent;			//����[��]
	TCHAR* pChfontName;			//��������[��]
	TCHAR* pChfontSize;			//�����С[��]
	TCHAR* pChfontColor;		//��ɫ[��]

	TCHAR* pEntxtPos;			//����λ��[Ӣ]
	TCHAR* pEncontent;			//����[Ӣ]
	TCHAR* pEnfontName;			//��������[Ӣ]
	TCHAR* pEnfontSize;			//�����С[Ӣ]
	TCHAR* pEnfontColor;		//��ɫ[Ӣ]

	TCHAR* pPutxtPos;			//����λ��[��]
	TCHAR* pPucontent;			//����[��]
	TCHAR* pPufontName;			//��������[��]
	TCHAR* pPufontSize;			//�����С
	TCHAR* pPufontColor;		//��ɫ[��]

	typedef enum _UE_BTN_INDEX
	{
		UE_BTNCODE=0,
		UE_BTNPAGE,
		UE_BTNMARK,
		UE_BTNVALUE,
		UE_BTNJUMPPAGE,
		UE_BTNXPOS,
		UE_BTNYPOS,
		UE_BTNWIDTH,
		UE_BTNHEIGHT,
		UE_BTNNORMALPICCODE,
		UE_BTNPRESSEDPICCODE,
		UE_BTNHOVERPICCODE,
		UE_BTNDISABLEPICCODE,

		UE_BTNCHTXTPOS,
		UE_BTNCHTXTCONTENT,
		UE_BTNCHFONTNAME,
		UE_BTNCHFONTSIZE,
		UE_BTNCHFONTCOLOR,

		UE_BTNENTXTPOS,
		UE_BTNENTXTCONTENT,
		UE_BTNENFONTNAME,
		UE_BTNENFONTSIZE,
		UE_BTNENFONTCOLOR,

		UE_BTNPUTXTPOS,
		UE_BTNPUTXTCONTENT,
		UE_BTNPUFONTNAME,
		UE_BTNPUFONTSIZE,
		UE_BTNPUFONTCOLOR,

		UE_BTNISENABLE,
		UE_BTNISSHOWMARK,
		UE_BTNISHOTSTATION,
	}UE_BTN_INDEX;

	_US_BUTTON_DATA()
	{
		pBtnCode=NULL;
		pBtnPage=NULL;
		pBtnMark=NULL;
		pBtnValue=NULL;
		pBtnJumpPage=NULL;
		pBtnXPos=NULL;
		pBtnYPos=NULL;
		pBtnWidth=NULL;
		pBtnHeight=NULL;
		pBtnNormamPicCode=NULL;
		pBtnPressedPicCode=NULL;
		pBtnHoverPicCode=NULL;
		pBtnDisablePicCode=NULL;
		pBtnIsEnable=NULL;
		pBtnIsShowMark=NULL;
		pBtnIsHotStation=NULL;

		pChtxtPos=NULL;
		pChcontent=NULL;
		pChfontName=NULL;
		pChfontSize=NULL;
		pChfontColor=NULL;

		pEntxtPos=NULL;
		pEncontent=NULL;
		pEnfontName=NULL;
		pEnfontSize=NULL;
		pEnfontColor=NULL;

		pPutxtPos=NULL;
		pPucontent=NULL;
		pPufontName=NULL;
		pPufontSize=NULL;
		pPufontColor=NULL;
	}
}US_BUTTON_DATA;

typedef struct _US_SPECIALBTNDATAS
{
	US_BUTTON_DATA*	m_pDataPtr;
	int				m_idataCount;
	_US_SPECIALBTNDATAS()
	{
		m_pDataPtr=NULL;
		m_idataCount=0;
	}
	~_US_SPECIALBTNDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_idataCount=0;
		}
	}
}US_SPECIALBTNDATAS;

typedef struct _US_SPECIALPICDATAS
{
	US_PIC_DATA*	m_pDataPtr;
	int				m_idataCount;
	_US_SPECIALPICDATAS()
	{
		m_pDataPtr=NULL;
		m_idataCount=0;
	}
	~_US_SPECIALPICDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_idataCount=0;
		}
	}
}US_SPECIALPICDATAS;

typedef struct _US_SPECIALLAYOUTDATAS
{
	US_LAYOUT_DATA* m_pDataPtr;
	int				idataCount;
	_US_SPECIALLAYOUTDATAS()
	{
		m_pDataPtr=NULL;
		idataCount=0;
	}
	~_US_SPECIALLAYOUTDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_pDataPtr=NULL;
		}
	}
}US_SPECIALLAYOUTDATAS;

typedef struct _area_btn_info{
	DWORD		number;						//��ť���
	CPoint		stationBtnPoint;			//���o����
	CSize		stationBtnSize;				//���o��С
	//CString		stationNormalBitmap;		//���o�����DƬ
	//CString		stationOnBitmap;			//���o�Ҹ��DƬ
	//CString		stationDownBitmap;			//���o���DƬ
	//CString		stationDisableBitmap;		//���o�����ÈDƬ
	CImage*		pImageNormal;				//����ͼƬ //ͼƬ����ָ�� �������ں�guiparm�������
	CImage*		pImageHover;				//����ͼƬ
	CImage*		pImagePressed;				//����ͼƬ
	CImage*		pImageDisabled;				//������ͼƬ
	bool		isEnable;					//���o�Ƿ���Ø��I
	bool		isVisiable;					//���o�Ƿ��@ʾ���I
	bool		isHotStation;				//�Ƿ��ǟ��cվ
	DWORD		btnID;						// ��ťֵ	������
	DWORD		nextPage;					//��תҳ����
	BYTE		textLocationType;			//����λ��
	CString		text;						//�ı�
	CString		fontname;					//��������
	int			fontsize;					//�����С
	COLORREF	textcolor;					//������ɫ
	POINT		textoffset;					//�ı���԰�ť�����ƫ��
	_area_btn_info()
	{
		number = 0;

		stationBtnPoint.x=0;
		stationBtnPoint.y=0;
		
		stationBtnSize.cx=0;
		stationBtnSize.cy=0;

		/*stationNormalBitmap=(_T(""));
		stationOnBitmap=(_T(""));
		stationDownBitmap=_T("");
		stationDisableBitmap=_T("");*/
		pImageNormal=NULL;				//����ͼƬ //ͼƬ����ָ�� �������ں�guiparm�������
		pImageHover=NULL;				//����ͼƬ
		pImagePressed=NULL;				//����ͼƬ
		pImageDisabled=NULL;				//������ͼƬ
		isEnable=false;
		isVisiable=false;
		isHotStation=false;
		btnID=0;   // ��ťֵ
		nextPage=0;   //��תҳ����
		textLocationType=0;
		text = _T("");
		fontname = _T("����");
		fontsize = 15;
		textcolor = WHITE;
	}
}AREA_BTN_INFO;

typedef struct _area_layout{
	DWORD layOutId;				//퓾��a
	CPoint startpoint;			//���Ͻ�
	CSize areaSize;				//�DƬ��С ���� �߶�
	CImage* pImageBackground;
	//CString areaBackImage;		//�׈D (·��)
	bool isEnable;				//���Ø��I
	_area_layout()
	{
		layOutId=0;
		startpoint.x=0;
		startpoint.y=0;

		areaSize.cx=0;
		areaSize.cy=0;

		pImageBackground=NULL;
		//areaBackImage=_T("");
		isEnable=false;
	}
}AREA_LAYOUT;

class CImageMgr
{
public:
	CImageMgr();
	~CImageMgr();
private:
	std::map<UINT,CImage*> m_mapImages;
public:
	void	insertImage(TCHAR* tcpiccode, const TCHAR* pcpath);
	CImage* getImage(TCHAR* tcpiccode);
};

class CGuiParm
{
public:
	CGuiParm();
	~CGuiParm();
public:
	//�x��LayOut�����ļ�
	void initializeLayOut(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));
	//�x��Button�����ļ�
	void initializeButton(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));
	//�x��Pic�����ļ�
	void initializePic(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));

	//��ȡlayout�������
	//�@ȡ��һ�l layout ��Ϣ
	US_LAYOUT_DATA*			getFirstLayOutData();
	//�@ȡ��һ�l layout ��Ϣ
	US_LAYOUT_DATA*			getNextLayOutData();
	//�@ȡָ�� layout ��Ϣ
	US_SPECIALLAYOUTDATAS*	getSpecialLayoutDatas(US_LAYOUT_DATA::UE_LAYOUT_INDEX ueIndex,const TCHAR* tcSpecialValue);
	//��ȡpic�������
	US_PIC_DATA*			getFirstPicData();
	US_PIC_DATA*			getNextPicData();
	US_SPECIALPICDATAS*		getSpecialPicDatas(US_PIC_DATA::UE_PIC_INDEX ueIndex,const TCHAR* tcSpecialValue);
	//��ȡbutton�������
	US_BUTTON_DATA*			getFirstBtnData();
	US_BUTTON_DATA*			getNextBtnData();
	US_SPECIALBTNDATAS*		getSpecialBtnDatas(US_BUTTON_DATA::UE_BTN_INDEX ueIndex,const TCHAR* tcSpecialValue);
//ex
	long getLayOutPageCodes(vector<DWORD>& pageCodes);
	long getPicData(vector<US_PIC_DATA>& picsInfo);
	//�@ȡ id  �Z�� �@ȡbutton ��Ϣ
	long getAreaBtnInfo(DWORD areaID,LANGUAGETYPE_T languageType,vector<AREA_BTN_INFO>& btnInfos);
//ex
	//�@ȡ ��һ�l layout ��Ϣ
	long					getFirstAreaLaylout(LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut);
	//����ָ�� id �Z�� �@ȡ layout ��Ϣ
	long					getAreaLaylout(DWORD areaLayoutID,LANGUAGETYPE_T languageType,AREA_LAYOUT& layOutData);

	// ��ȡHome Page����
	long GetHomePageLayout(LANGUAGETYPE_T languageType,AREA_LAYOUT& homeLayout);
private:
	void m_getAreaInfo(US_LAYOUT_DATA* playOutData,LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut);
private:
	//�������ݵĽṹ��
	US_LAYOUT_DATA			m_layoutData;
	US_PIC_DATA				m_picData;
	US_BUTTON_DATA			m_btnData;

	CCsvReader				m_layOutCsv;	//����layout�ļ��Ķ���
	CCsvReader				m_buttonCsv;	//�洢button�ļ��Ķ���
	CCsvReader				m_picCsv;		//����pic�ļ��Ķ���

	UE_CSV_RESULT			m_ueLayOutRet;	//��ǳ�ʼ���Ƿ�ɹ�
	UE_CSV_RESULT			m_ueButtonRet;	//��ǳ�ʼ���Ƿ�ɹ�
	UE_CSV_RESULT			m_uePicRet;		//��ǳ�ʼ���Ƿ�ɹ�
	//����ָ���е�ָ�����ݲ��ҵĽ����
	US_SPECIALLAYOUTDATAS	m_usSpecialLayoutDatas;
	US_SPECIALPICDATAS		m_usSpecialPicDatas;
	US_SPECIALBTNDATAS		m_usSpecialBtnDatas;

	CImageMgr				m_imageMgr;
};

#endif