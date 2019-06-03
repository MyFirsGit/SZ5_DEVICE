#pragma once
#include "stdafx.h"
#include "GStatic.h"
#include "LoginDlg.h"
#include "svcdef.h"
#include "tvmforeservice.h"

/**
@brief ��¼Service
*/
class CLoginSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _dialog_group {
		DIALOG_GROUP_BASE = 1,
	} DIALOG_GROUP;

	enum _tagLOGINOUT_RESULT
	{
		LOGIN_SUCCESS					= 0x00, // �����¼��
		LOGIN_USER_PWD_ERROR			= 0x01, // �û��������ڻ��������
		LOGIN_REFUSE					= 0x02, // �ܾ���¼���ظ���¼��֤����
		LOGIN_AND_PWD_WILL_OVERDUE		= 0x03, // �����¼�����뼴��ʧЧ��
		LOGIN_AND_MODIFIED				= 0x04, // �����¼��ǿ���޸����룻
		LOGOUT_SUCCESS					= 0x05, // 0x05������ǳ�
		LOGOUT_FAILURE					= 0x06, // 0x06���ܾ��ǳ���
		LOGIN_ID_OVERDUE				= 0x07, // 0x07���ܾ���¼���˻���������
		LOGIN_PWD_OVERDUE				= 0x08, // 0x08���ܾ���¼��������ֹ��
		LOGIN_ID_EXPIRED				= 0x09, // 0x09���ܾ���¼���˻�ͣ�ã�
		LOGIN_SETITEM_EMPTY				= 0x10, // �û��������벻��Ϊ�ա�
		LOGIN_NOT_ACCESS				= 0x11, // û�з���Ȩ�ޣ�
		LOGIN_OVER_MAX_COUNT			= 0x12, // ��������½������
		LOGIN_LOCKED					= 0x13, // ��ID�����������ܽ��е�½��
		LOGIN_CANNOT_USE_SUPEROPERATOR	= 0x14, // �����û�Ȩ��û�зſ���
		LOGIN_FAILURE					= 0x15, // δ֪����
		LOGIN_HAS_EXCEPTION				= 0x16, // �쳣�У���ֹ��½
		LOGIN_REQUEST_SERVER_FAILED		= 0x17, // �������ʧ��
	} LOGINOUT_RESULT;

	CLoginSvc(void);
	~CLoginSvc(void);

	virtual void OnStart();                                                 // Service��������
	virtual bool OnStop();                                                  // Serviceֹͣǰ����

	long CheckPassword(int id, CString pwd);				// �������
	void Login(int id, CString pwd);						// ȷ�ϴ���
	void SetUserAuthority(STAFF_AUTHORITY authority);		// ���õ�¼����ԱȨ�ޣ������»��棩
	STAFF_AUTHORITY GetOperatorAllAccess(int nOperatorID);
	bool IsSuperOperatorLogin(int nOperatorID);

public:
	const static int LOGIN_INFO_ROWS = 15;                                  // ��¼��Ϣ���������
	USER_INFO m_userInfo;                                                   // ��¼����Ա��Ϣ
	CString m_userId;                                                       // ����Ĳ���ԱID

private:
	pair<int,int> m_UserLoginCount;

	int m_passwordErrCount;                                                 // ��������������

	void InitLoginAreaInfo();                                               // ���ڻ���¼����ʾ����
	long LocalValidate(int id, CString pwd);
	long LockedOperatorID(int id, int nPosInfo);
	void SaveOperationLog(bool success,bool isLogin,int id);
	LANGUAGE_MSG GetGuidCode(int loginResult);								// ��ȡ��½ʧ�����Դ���

	LRESULT OnKeyboardEnter(WPARAM wp , LPARAM lp);

	//LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);
}; 

//PLABEL_GROUP GetLoginAreaInfo();                                        // ��ȡ��¼����ʾ����
//LABEL_GROUP m_loginInfoGroup[LOGIN_INFO_ROWS];                          // ��¼��Ϣ�������ʾ����


