#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"
#include "OperationDlg.h"
#include "MenuMgr.h"

enum KEYBOARD
{
	KEY_NOBODY	= 0x0000, // ����ά������
	KEY_NUMBER	= 0x0001, // ���ּ� 0 ~ 9
	KEY_DOT		= 0x0002, // Բ��� .
	KEY_CLEAR	= 0x0004, // ɾ���� Backspace
	KEY_ENTER	= 0x0008, // ȷ�ϼ� Enter
	KEY_ESC		= 0x0010, // ���ؼ� Esc
	KEY_F1		= 0x0020, // F1 �� δʹ��
	KEY_F2		= 0x0040, // F2 �� ����ҵ���е����ձ���
	KEY_F3		= 0x0080, // F3 �� ������һ��
	KEY_F4		= 0x0100, // F4 �� δʹ��
	KEY_F5		= 0x0200, // F5 �� ��һҳ
	KEY_F6		= 0x0400, // F6 �� ��һҳ

	KEY_INPUT	= KEY_NUMBER | KEY_CLEAR | KEY_ENTER,	// ������� ��������
	KEY_RETURN	= KEY_F3 | KEY_ESC,						// ������� ���ز˵�
	KEY_PAGE	= KEY_F5 | KEY_F6,						// ������� ��ҳ
};				

/**
 @ brief ����Ա�˵��������Ի���
*/
class CNavigationDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNavigationDlg)
    DECLARE_MESSAGE_MAP()

public:
	CNavigationDlg(CService* pService);
	virtual ~CNavigationDlg();

	enum { IDD = IDD_00301_CHARGE_RECEPT_DLG };

	void SetKeyboard(DWORD keys);						// ����ά������ָ������
	void DiableKeyboard(DWORD keys);					// ����ά������ָ������
	void ShowLevel(SERVICE_ID& serviceID);				// ��ʾ�˵�����
	void AppendLevel(CString level);					// ׷�Ӳ˵�����

	void DoService(SERVICE_ID& serviceID);				// ����ҵ��
	Menu_Item* SearchService(SERVICE_ID& serviceID);	// ����id��Ӧ�Ĳ˵�

protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDraw(CDC* pDC);
	void OnMenuKindChanged(CMenuMgr::MENU_KIND&, CMenuMgr::MENU_KIND&);
	void OnMenuItemStatusChanged(Menu_Item*);
	void OnStartingService(CService* service);
	void OnStartedService(CService* service);

	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	
private:
	CRect m_navi_rect;				// ������������ʾ����
	CString m_auto_level;			// �Զ���ȡ�Ĳ˵�����
	CString m_manual_level;			// �ֶ�׷�ӵĲ˵�����

	static DWORD m_hook_keys;		// ����ָ��������ö�� KeyBoard ��λ�롣
	static HHOOK m_keyboard_hook;	// �ֲ����̹���
};
