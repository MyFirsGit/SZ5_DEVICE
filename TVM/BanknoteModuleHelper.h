#pragma once
#include "ServiceHelper.h"
#include "BHCommand.h"
#include "BHException.h"
#include "Sync.h"
#include "BHHardwareException.h"

/**
@brief  纸币模块控制类
*/
class CBanknoteModuleHelper
{
public:
	CBanknoteModuleHelper();						// 构造函数
	~CBanknoteModuleHelper();						// 析构函数

	long Init();									// 初始化
	long Reset();									// 复位
	long Abort();									// 停止动作, 用于澳门框架中的停止各个动作的轮询

	long OpenAndValidate(BH_STATUS& status);		// 开始接收识别纸币
	long OpenAndValidate(BH_CMD_FEEDBACK& feedback); // 开始接收识别纸币(MEI)

	long StopValidation();							// 停止接收识别纸币

	long Collect();									// 压钞
	long Collect(BH_CMD_FEEDBACK& feedback);		// 压钞（MEI,带反馈信息）

	long ReturnAll(bool isCheckShutter = false);	// 退回全部纸币
	long ReturnAll(BH_CMD_FEEDBACK& feedback);		// 退纸币(MEI)

	long ReturnCache(bool isCheckShutter = false);	// 退回缓冲区的纸币	

	long GetBoxID(BH_BOX_ID& box_id);				// 获取钱箱ID
	long GetBoxID(BRH_BOX_ID& box_id);				// 获取MEI钱箱ID

	long Hold();									// 压暂存 ** MEI中  纸币接收识别 与 压暂存在同一动作（work）里面


	long Clean(BH_CLEAN_DEST dest);					// 清除钞票
	long GetStatus(BH_STATUS& status);				// 读取状态

	long Repare(bool bMenualRepare = false);		// 异常解除
	static bool IsHWException(long errCode);		// 是否为硬件异常
	bool IsBankNoteBoxAttached();                   // 判断纸币箱是否装上
	

	void OpenAndValidateAsync(CService* pSvc,BH_FACE_ACEPTED_TABLE& bhTable);	// 异步：开始接收识别纸币,可接收的面值列表

	void HoldAsync(CService* pSvc);					// 异步：压暂存

	void CollectAsync(CService* pSvc);				// 异步：压钞

	void ReturnCacheAsync(CService* pSvc);			// 异步：退回缓冲区的纸币 MEI 中只有退出暂存中的全部纸币 

	void ReturnAllAsync(CService* pSvc);			// 异步：退回全部纸币  

	long TryRecoveryBanknoteModule();				// 尝试以退出方式修复模块

private:
	long CheckAndOpenBHComport();					// 打开纸币接收模块，已打开则关闭后再打开。
};
