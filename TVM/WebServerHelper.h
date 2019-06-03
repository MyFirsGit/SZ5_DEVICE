#pragma once
#include <afxinet.h>
#include "SCSvcDef.h"

using namespace std;

#define  theWEBSERVER_HELPER CWebServerHelper::GetInstance()
/**
@brief  webserver返回结果
*/
typedef enum _txn_code {
	TXN_CODE_OK						= 0,		// 请求成功
	TXN_CODE_PARAM_ERR				= 1,		// 请求参数错误
	TXN_CODE_TIMESPAN_ERR			= 2,		// 时间戳异常
	TXN_CODE_ENCODE_ERR				= 3,		// 加密算法未知
	TXN_CODE_REPEAT					= 5,		// 请求需重发
	TXN_CODE_EQUIPID_ERR			= 6,		// 设备编号错误
	TXN_CODE_PASSWORD_ERR			= 7,		// 设备密码错误或设备已注册
	TXN_CODE_DATABASE_ERR			= 8,		// 数据库操作错误
	TXN_CODE_TOKEN_ERR				= 9,		// 设备密钥有效期超时
	TXN_CODE_LOCK_ERR				= 10,		// 订单锁定状态调用预出票接口返回错误码10
}TXN_CODE;

typedef enum _enum_web_operator_type{
	WEB_ORDER_REQUEST				= 1,	// 请求生成订单
	WEB_ORDER_QUERY					= 2,	// 查询订单
	WEB_ORDER_CANCEL				= 3,	// 取消订单
	WEB_ORDER_CONFIRM				= 4,	// 确认订单
}WEB_OPERATOR_TYPE;

typedef struct _ticketInfo{
	CString ticketID;
	CString strWriteTicketTime;
	CString strIssueTime;	
	_ticketInfo(){
		ticketID = _T("");
		strWriteTicketTime = _T("");
		strIssueTime = _T("");
	}
}TicketInfo;

/*
* 线程参数
*/
typedef struct {
	CHttpFile* pHttpFile;
	CHAR* pSendData;
	CString strHeader;
}PARM;

/**
@brief  订单处理
*/

class CWebServerHelper 
{

public:
	static CWebServerHelper&  GetInstance(){
		return theInstance;
	}
	CWebServerHelper();                                                 // 构造函数
	~CWebServerHelper();                                                // 析构函数

public:
	RETURN_CODE Web_ConnectCheck();										// 服务器连接性检查
	RETURN_CODE Weixin_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Weixin_QueryOrder();
	RETURN_CODE Weixin_CancelOrder();
	RETURN_CODE Zhifubao_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Zhifubao_QueryOrder();
	RETURN_CODE Zhifubao_CancelOrder();
	RETURN_CODE Web_ConfirmOrder(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo);
	RETURN_CODE Web_IdleProcessOrder();

	void SetConnectFlag(bool bConnected);		// 设置WEB服务器连通标识
	bool GetConnectFlag();						// 获取WEB服务器连通标识
	void EnableIdleProcess(bool bEnabled);		// 设置是否允许本地处理保存的未处理订单
	bool IsEnableIdleProcess();					// 获取是否允许本地处理保存的未处理订单
	bool IsExistLocalOrder();					// 是否存在保存的未处理订单 
	void ClearLocalOrder();						// 清理保存的未处理订单 
	void SaveLocalOrder();						// 保存未处理订单 

private:
	// 用于实时上送订单请求
	RETURN_CODE Weixin_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Weixin_Query();
	RETURN_CODE Weixin_Cancel();
	RETURN_CODE Zhifubao_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Zhifubao_Query();
	RETURN_CODE Zhifubao_Cancel();
	RETURN_CODE Web_Confirm(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo);
	// 用于闲时上送本地保存的订单撤销或结果确认请求
	RETURN_CODE Web_IdleConfirm(CString strConfirmURL);
	RETURN_CODE Web_IdleCancel(CString strCancelURL);

private:
	// 服务器连接信息
	CString host;
	int port;
	CString strHeaders;
	CString baseURL;
	CString connectURL;
	// 订单URL信息
	CString weixin_requestOrderURL;
	CString weixin_queryOrderURL;
	CString weixin_cancelOrderURL;
	CString zhifubao_requestOrderURL;
	CString zhifubao_queryOrderURL;
	CString zhifubao_cancelOrderURL;
	CString confirmOrderURL;
	// 订单信息
	int m_nOrderRetryTimes;
	CString m_strOrderID;

	// 本地保存订单信息
	bool m_bCurConnect;						//当前WEB服务器是否可用
	bool m_bLoacalProcessEnable;			//是否允许本地处理订单
	int m_nOrderProcessType;				//本地保存的闲时处理的订单处理类型
	CString m_strOrderProcessURL;			//本地保存的闲时处理的订单处理URL

	void UTF8toANSI(CString &strUTF8);
	RETURN_CODE ServerPost(CString senddata, CString url,CString& getdata,int nTimeOut = 10000 /* 单位：ms */);
	long StartWebThread();
	DWORD WINAPI WebCommunicateFunction(LPVOID lpParameter);
	static  CWebServerHelper theInstance;   // 该类的唯一接口

public:
	// 线程相关
	HANDLE m_hPauseThread;
	HANDLE m_hWaitResult;
	HANDLE m_hExitThread;
	PARM m_Parm;
	RETURN_CODE m_EmThreadReturn;
	BOOL	m_bExitThread;
};