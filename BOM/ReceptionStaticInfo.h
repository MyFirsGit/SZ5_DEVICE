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
    CString nameChinese;                    // ����
    CString nameEnglish;                    // Ӣ��
	COLORREF color;                         // ������ɫ
        msg_name()
        {
            nameChinese = "";               // ����
            nameEnglish = "";               // Ӣ��
			color = 0;
        }
} MSG_NAME, *PMSG_NAME;

typedef struct msg_name_group
{
    MSG_NAME nameLeft;                      // ���
    MSG_NAME nameRight;                     // �ұ�
} MSG_NAME_GROUP;

typedef struct msg_info
{
    //CPoint point;                           // ������ʼ��
	MSG_NAME titleName;                     // ��������
	std::vector<MSG_NAME_GROUP> detailList;
} MSG_INFO, *PMSG_INFO;


/** 
@brief   ����Բ�Ǿ��μ����е����ݣ���ִ����ز��� 
*/
class CReceptionStaticInfo : public CStatic
{
	DECLARE_DYNAMIC(CReceptionStaticInfo)
    DECLARE_MESSAGE_MAP()

public:
    CReceptionStaticInfo(CWnd* pParentWnd, PMSG_INFO);  // ���캯��
	virtual ~CReceptionStaticInfo();                    // ��������

    void SetText(UINT, UINT, MSG_NAME);                   // ������ʾ����

protected:
    virtual void PreSubclassWindow();                   // ת��Ϊ��ǰ��ʱ����

private:
    PMSG_INFO m_msgInfo;                                // ��������

private:
    afx_msg void OnPaint();                             // ����仯ʱˢ�»���
};


