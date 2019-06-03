#pragma once
#include <vector>
// CReceptionStaticInfo
#define MSG_ROUND_HEIGHT    36
#define MSG_ROUND_WIDTH     370
#define MSG_DISTANCE        65
#define MSG_TITLE_X         22
#define MSG_TITLE_Y         12
#define MSG_DETAIL_LEFT_X_LEFT   150
#define MSG_DETAIL_LEFT_X_RIGHT  300
#define MSG_DETAIL_RIGHT_X_LEFT	 350
#define MSG_DETAIL_RIGHT_X_RIGHT 950

#define MSG_WIDTH           2030
#define MSG_BASE_HEIGHT     60
#define MSG_FIRST_DETAIL_X  230
#define MSG_FIRST_DETAIL_Y  15
#define MSG_CHINESE_HEIGHT  25
#define MSG_MAX_ROW         12

typedef struct msg_name
{
    CString nameChinese;                    // 中文
    CString nameEnglish;                    // 英文
	COLORREF color;                         // 标题颜色
        msg_name()
        {
            nameChinese = "";               // 中文
            nameEnglish = "";               // 英文
			color = 0;
        }
} MSG_NAME, *PMSG_NAME;

typedef struct msg_name_group
{
    MSG_NAME nameLeft;                      // 左边
    MSG_NAME nameRight;                     // 右边
} MSG_NAME_GROUP;

typedef struct msg_info
{
    //CPoint point;                           // 区域起始点
	MSG_NAME titleName;                     // 标题文字
	std::vector<MSG_NAME_GROUP> detailList;
} MSG_INFO, *PMSG_INFO;


/** 
@brief   建立圆角矩形及其中的内容，并执行相关操作 
*/
class CReceptionStaticInfo : public CStatic
{
	DECLARE_DYNAMIC(CReceptionStaticInfo)
    DECLARE_MESSAGE_MAP()

public:
    CReceptionStaticInfo(CWnd* pParentWnd, PMSG_INFO);  // 构造函数
	virtual ~CReceptionStaticInfo();                    // 析构函数

    void SetText(UINT, UINT, MSG_NAME);                   // 设置显示文字

protected:
    virtual void PreSubclassWindow();                   // 转型为当前类时调用

private:
    PMSG_INFO m_msgInfo;                                // 区域数据

private:
    afx_msg void OnPaint();                             // 画面变化时刷新画面
};


