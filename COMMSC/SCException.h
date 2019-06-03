#pragma once
#include "afx.h"
#include "SysException.h"
#include "scexp.h"

/**
@brief	上位通信异常类
*/

class SC_API CSCException :
    public CSysException
{
public:
    static const int MODULE_ID = 10101;     // 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR					= 1,	// 设定值：参数异常
		FUNC_PARAM_ERR					= 2,	// 函数参数异常
		SEQUENCE_ERR					= 3,	// 函数调用顺序异常
		FUNC_ERR_RETURN					= 4,	// 函数返回异常
		LOGIC_ERR						= 5,	// 逻辑异常
		OTHER_ERR						= 6,	// 异常发生
		CONNECTION_ERROR				= 101,  // 连接错误
		NO_BIND_ERROR					= 102,  // 未连接
		SEND_ERROR						= 103,  // 数据送信异常
		RECEIVE_ERROR					= 104,  // 数据受信异常
		DO_ERROR						= 105,  // 操作阻塞
		REC_ADDRESS_ERROR				= 107,  // 接收端地址没有被指定
		MES_LEN_ERROR					= 108,  // 消息长度异常
		NET_DOWN_ERROR					= 109,  // 网络故障
		NET_REACH_ERROR					= 110,  // 不能到达网络
		NO_ADDRESS_ERROR				= 111,  // 没有空的缓存区
		SOCKET_CONNECT_ERROR			= 112,  // socket不被接续
		SOCKET_SEND_ERROR				= 113,  // socket打开后不能送信
		BIND_TIMEOUT_ERROR				= 114,  // 连接超时
		USER_TOO_BIG_ERROR				= 115,  // 用户过多(BSD互換)
		WIN_VER_ERROR					= 116,  // WinSock.dll的版本不在范围内
		SERVICE_ERROR					= 117,  // SERVICE不可见
		POST_SUB_ERROR					= 118,  // POST不可见，SUB异常
		OTHER_ERROR						= 119,  // 其他WinSock ERROR        
		TIME_SYNCHRONIZE_ERROR			= 200,  // 时钟同步故障
		REFRESH_VERSION_ERROR			= 201,  // CD版本切换失败
		NOT_CONNECT_SERVER_OUT_OF_TIME	= 202,	// 最大离线时间故障
	} DETAIL_ERROR_CODE;


    CSCException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CSCException(void);

private:
};
