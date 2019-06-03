#pragma once
#include <vector>
#include <atlimage.h>
#include "PictureEx.h"

#define REC_GROUP1_BMP1 CRect(CPoint(50,135),CSize(300,210))
#define REC_GROUP1_TIP1 CRect(CPoint(10,10+210+100+70),CSize(380,120))

#define REC_GROUP2_BMP1 CRect(CPoint(50,10+20),CSize(300,210))
#define REC_GROUP2_BMP2 CRect(CPoint(50,10+210+30),CSize(300,210))
#define REC_GROUP2_TIP2 CRect(CPoint(10,10+210+210+70),CSize(380,60))

#define REC_GROUP3_BMP1 CRect(10,10,280,210)
#define REC_GROUP3_TIP1 CRect(10,210,280,250)
#define REC_GROUP3_BMP2 CRect(10,250,280,450)
#define REC_GROUP3_TIP2 CRect(10,450,280,490)
#define REC_GROUP3_BMP3 CRect(10,490,280,690)
#define REC_GROUP3_TIP3 CRect(10,690,280,730)

#define REC_GROUP4_BMP1 CRect(10,10,280,160)
#define REC_GROUP4_TIP1 CRect(10,160,280,190)
#define REC_GROUP4_BMP2 CRect(10,190,280,340)
#define REC_GROUP4_TIP2 CRect(10,340,280,370)
#define REC_GROUP4_BMP3 CRect(10,370,280,520)
#define REC_GROUP4_TIP3 CRect(10,520,280,550)
#define REC_GROUP4_BMP4 CRect(10,550,280,700)
#define REC_GROUP4_TIP4 CRect(10,700,280,730)

//static CRect const REC_BITMAP1_RECT			   = CRect(10,50,290,290);			// 乘客界面提示图片区域1
//static CRect const REC_TIP1_RECT			   = CRect(10,580,290,680);			// 乘客界面提示文字区域1
//static CRect const REC_BITMAP2_RECT			   = CRect(10,300,290,540);			// 乘客界面提示图片区域2
//static CRect const REC_TIP2_RECT			   = CRect(10,300,290,375);			// 乘客界面提示文字区域2
//static CRect const REC_BITMAP3_RECT			   = CRect(10,550,290,790);			// 乘客界面提示图片区域3
//static CRect const REC_TIP3_RECT			   = CRect(10,665,290,740);			// 乘客界面提示文字区域3


typedef struct image_group
{	
	CString strfileName;             // 图片名称
	COLORREF color;
	CString bitInfo;                 // 文本
	image_group(){
		strfileName = _T("");
		color = BLACK;
		bitInfo = _T("");
	}
} IMAGE_GROUP, *PIMAGE_GROUP;


typedef struct image_group_info
{
	UINT tipsCount;                   // 提示信息组数量
	IMAGE_GROUP detailList[4];
} IMAGE_GROUP_INFO, *PIMAGE_GROUP_INFO;


/** 
@brief   建立圆角矩形及其中的内容，并执行相关操作 
*/
class CReceptionGuideInfo : public CStatic
{
	DECLARE_DYNAMIC(CReceptionGuideInfo)
	DECLARE_MESSAGE_MAP()

public:
	CReceptionGuideInfo(CWnd* pParentWnd, PIMAGE_GROUP_INFO,UINT type = 0);  // 构造函数,type = 0:售票 type=1：充值
	CReceptionGuideInfo();
	virtual ~CReceptionGuideInfo();                       // 析构函数

	UINT	getTipCount();			//获取当前tip总数 tip数=图片数
	void	stopGif(UINT uiindex);	//停止播放指定的gif图片
	void	setGuideInfo(PIMAGE_GROUP_INFO&);
	void renderGuideInfo();
	BOOL	createWnd(CWnd* pParentWnd);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();                      // 转型为当前类时调用
	void Convert(CImage* image);                           // 透明处理
private:
	PIMAGE_GROUP_INFO m_msgInfo;                           // 区域数据
	UINT m_type;
	CPictureEx	m_gif[4];

	CRect		m_rectBmp[4];
	CRect		m_rectTip[4];

	UINT		m_uiTipsCount;

	BOOL		m_bCreateGif1;
	BOOL		m_bCreateGif2;
	BOOL		m_bCreateGif3;
private:
	afx_msg void OnPaint();                                // 画面变化时刷新画面
};


