#pragma once
#include "GStatic.h"
#include "LoginDlg.h"
#include "svcdef.h"
#include "bomforeservice.h"

/**
@brief      ��¼Service
*/

class CLoginSvc :
    public CBOMForeService
{
   /* DECLARE_MESSAGE_MAP()*/

public:

	enum _tagLOGINOUT_RESULT
	{
		LOGIN_SUCCESS = 0x00,		//�����¼��
		LOGIN_USER_PWD_ERROR = 0x01, //�û��������ڻ��������
		LOGIN_REFUSE	   = 0x02,  //�ܾ���¼���ظ���¼��֤����
		LOGIN_AND_PWD_WILL_OVERDUE	   = 0x03,  //�����¼�����뼴��ʧЧ��
		LOGIN_AND_MODIFIED = 0x04,	//�����¼��ǿ���޸����룻
		LOGOUT_SUCCESS = 0x05,		//0x05������ǳ�
		LOGOUT_FAILURE = 0x06,		//0x06���ܾ��ǳ���
		LOGIN_ID_OVERDUE = 0x07,	//0x07���ܾ���¼���˻���������
		LOGIN_PWD_OVERDUE = 0x08,	//0x08���ܾ���¼��������ֹ��
		LOGIN_ID_EXPIRED = 0x09,	//0x09���ܾ���¼���˻�ͣ�ã�
		LOGIN_SETITEM_EMPTY,		//�û��������벻��Ϊ�ա�
		LOGIN_NOT_ACCESS, //û�з���Ȩ�ޣ�
		LOGIN_OVER_MAX_COUNT, //��������½������
		LOGIN_LOCKED,		//��ID�����������ܽ��е�½��
		LOGIN_CANNOT_USE_SUPEROPERATOR, //�����û�Ȩ��û�зſ���
		LOGIN_FAILURE,		//δ֪����
		LOGIN_HAS_EXCEPTION,          // �쳣�У���ֹ��½
		LOGIN_REQUEST_SERVER_FAILED, //�������ʧ��
	} LOGINOUT_RESULT;

    CLoginSvc(void);                                                        // ���캯��
    ~CLoginSvc(void);                                                       // ��������

    virtual void OnStart();                                                 // Service��������
    virtual bool OnStop();                                                  // Serviceֹͣǰ����
    PLABEL_GROUP GetLoginAreaInfo();                                        // ��ȡ��¼����ʾ����
    void OnOK(int nOperatorID,CString password);                           // ȷ�ϴ���
    void SetUserAuthority(STAFF_AUTHORITY authority);                       // ���õ�¼����ԱȨ�ޣ������»��棩
    STAFF_AUTHORITY GetOperatorAllAccess(int nOperatorID);
	bool IsSuperOperatorLogin(int nOperatorID);

public:
    const static int LOGIN_INFO_ROWS = 2;                                   // ��¼��Ϣ���������
    USER_INFO m_userInfo;                                                   // ��¼����Ա��Ϣ
	CString m_userId;                                                       // ����Ĳ���ԱID
    
private:

    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;
	pair<int,int> m_UserLoginCount;
    LABEL_GROUP m_loginInfoGroup[LOGIN_INFO_ROWS];                          // ��¼��Ϣ�������ʾ����
    int m_passwordErrCount;                                                 // ��������������
   
	void InitLoginAreaInfo();                                               // ���ڻ���¼����ʾ����
	long Login(int nOperatorID,CString sPwd);
	long LocalValidate(int nOperatorID,CString sPwd);
	long LockedOperatorID(int nOperatorID,int nPosInfo);
	void SaveOperationLog(bool success,bool isLogin,int nOperationID);
	OPERATION_MSG GetGuidCode(int loginResult);                  // ��ȡ��½ʧ�����Դ���
}; 
