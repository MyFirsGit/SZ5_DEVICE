#pragma once
#include <vector>
// CReceptionStaticInfo
#define MSG_ROUND_HEIGHT    36
#define MSG_ROUND_WIDTH     370
#define MSG_DISTANCE        55
#define MSG_TITLE_X         22
#define MSG_TITLE_Y         20
#define MSG_DETAIL_LEFT_X_LEFT   150
#define MSG_DETAIL_LEFT_X_RIGHT  350
#define MSG_DETAIL_RIGHT_X_LEFT	 400
#define MSG_DETAIL_RIGHT_X_RIGHT 950

#define MSG_WIDTH           2030
#define MSG_BASE_HEIGHT     60
#define MSG_FIRST_DETAIL_X  230
#define MSG_FIRST_DETAIL_Y  15
#define MSG_CHINESE_HEIGHT  20
#define MSG_ENGLISH_HEIGHT  15
#define MSG_MAX_ROW         12

typedef struct msg_name
{
    CString nameChinese;                    // ����
    CString nameEnglish;                    // Ӣ��
	CString namePortuguese;                 // ��������
	COLORREF color;                         // ������ɫ
        msg_name()
        {
            nameChinese = _T("");               // ����
            nameEnglish = _T("");               // Ӣ��
			namePortuguese = _T("");            // Ӣ��
			color = 0;
        }
	void SetMsgInfo(DWORD txtID,COLORREF color = BLACK) {
		nameChinese = _cn(txtID);               // ����
		nameEnglish = _en(txtID);               // Ӣ��
		namePortuguese = _po(txtID);            // Ӣ��
		this->color = color;
	}
} MSG_NAME, *PMSG_NAME;

typedef struct msg_name_group
{
    MSG_NAME nameLeft;                      // ���
    MSG_NAME nameRight;                     // �ұ�
} MSG_NAME_GROUP;

typedef struct msg_name_group_four
{
	MSG_NAME name1;                      // ���
	MSG_NAME name2;                     // �ұ�
	MSG_NAME name3;                      // ���
	MSG_NAME name4;                     // �ұ�
} MSG_NAME_GROUP_FOUR;

typedef struct msg_info
{
    //CPoint point;                           // ������ʼ��
	MSG_NAME titleName;                     // ��������
	std::vector<MSG_NAME_GROUP> detailList;
	std::vector<MSG_NAME_GROUP_FOUR> detailListfour;
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


