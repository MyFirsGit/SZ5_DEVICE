#pragma once
#include "bomforeservice.h"
#include "FTPCommand.h"

/**
@brief    保守业务 - 硬件自检业务
*/
class CNetworkTestSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CNetworkTestSvc();
	~CNetworkTestSvc();

	BOOL PingSC(char* pBuff);										// ping上位服务器
	static const int MAX_PING_RESULT_LLENGTH = 2000;				// 结果信息最大长度
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_NETWORK           =  1,                        // 钱箱测试选择
	} DIALOG_GROUP;
};