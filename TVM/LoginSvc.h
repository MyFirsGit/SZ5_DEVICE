#pragma once
#include "stdafx.h"
#include "GStatic.h"
#include "LoginDlg.h"
#include "svcdef.h"
#include "tvmforeservice.h"

/**
@brief 登录Service
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
		LOGIN_SUCCESS					= 0x00, // 允许登录；
		LOGIN_USER_PWD_ERROR			= 0x01, // 用户名不存在或密码错误；
		LOGIN_REFUSE					= 0x02, // 拒绝登录（重复登录验证）；
		LOGIN_AND_PWD_WILL_OVERDUE		= 0x03, // 允许登录，密码即将失效；
		LOGIN_AND_MODIFIED				= 0x04, // 允许登录，强制修改密码；
		LOGOUT_SUCCESS					= 0x05, // 0x05：允许登出
		LOGOUT_FAILURE					= 0x06, // 0x06：拒绝登出；
		LOGIN_ID_OVERDUE				= 0x07, // 0x07：拒绝登录（账户锁定）；
		LOGIN_PWD_OVERDUE				= 0x08, // 0x08：拒绝登录（密码终止）
		LOGIN_ID_EXPIRED				= 0x09, // 0x09：拒绝登录（账户停用）
		LOGIN_SETITEM_EMPTY				= 0x10, // 用户名或密码不能为空。
		LOGIN_NOT_ACCESS				= 0x11, // 没有访问权限；
		LOGIN_OVER_MAX_COUNT			= 0x12, // 超过最大登陆次数；
		LOGIN_LOCKED					= 0x13, // 该ID被锁定，不能进行登陆。
		LOGIN_CANNOT_USE_SUPEROPERATOR	= 0x14, // 超级用户权限没有放开。
		LOGIN_FAILURE					= 0x15, // 未知错误。
		LOGIN_HAS_EXCEPTION				= 0x16, // 异常中，禁止登陆
		LOGIN_REQUEST_SERVER_FAILED		= 0x17, // 请求服务失败
	} LOGINOUT_RESULT;

	CLoginSvc(void);
	~CLoginSvc(void);

	virtual void OnStart();                                                 // Service启动后处理
	virtual bool OnStop();                                                  // Service停止前处理

	long CheckPassword(int id, CString pwd);				// 检查密码
	void Login(int id, CString pwd);						// 确认处理
	void SetUserAuthority(STAFF_AUTHORITY authority);		// 设置登录操作员权限（并更新画面）
	STAFF_AUTHORITY GetOperatorAllAccess(int nOperatorID);
	bool IsSuperOperatorLogin(int nOperatorID);

public:
	const static int LOGIN_INFO_ROWS = 15;                                  // 登录信息区域的行数
	USER_INFO m_userInfo;                                                   // 登录操作员信息
	CString m_userId;                                                       // 输入的操作员ID

private:
	pair<int,int> m_UserLoginCount;

	int m_passwordErrCount;                                                 // 密码连续输错次数

	void InitLoginAreaInfo();                                               // 初期化登录的显示内容
	long LocalValidate(int id, CString pwd);
	long LockedOperatorID(int id, int nPosInfo);
	void SaveOperationLog(bool success,bool isLogin,int id);
	LANGUAGE_MSG GetGuidCode(int loginResult);								// 获取登陆失败文言代码

	LRESULT OnKeyboardEnter(WPARAM wp , LPARAM lp);

	//LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);
}; 

//PLABEL_GROUP GetLoginAreaInfo();                                        // 获取登录的显示内容
//LABEL_GROUP m_loginInfoGroup[LOGIN_INFO_ROWS];                          // 登录信息区域的显示内容


