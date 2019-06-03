#pragma once
#include "ServiceHelper.h"
#include "BRHMessage.h"
#include "BRHException.h"
#include "Sync.h"
#include "BRHHardwareException.h"

/**
@brief  纸币模块控制类
*/
class CBRHModuleHelper
{
public:
	CBRHModuleHelper();						// 构造函数
	~CBRHModuleHelper();						// 析构函数

	long Init(bool bRepareCount=true);				// 初始化(是否修复软件数）
	long Reset();									// 复位
	long Abort();									// 停止动作, 用于澳门框架中的停止各个动作的轮询

	long OpenAndValidate(BRH_STATUS& status);		// 开始接收识别纸币(未使用)
	long OpenAndValidate(tBncCashInfo& feedback); // 开始接收识别纸币(MEI)

	long StopValidation();							// 停止接收识别纸币

	long Collect();									// 压钞(同步)
	//long Collect(BRH_CMD_FEEDBACK& feedback);		// 压钞（MEI,带反馈信息）

	long ReturnAll(bool isCheckShutter = false);	// 退回全部纸币
	long ReturnAll(BRH_CMD_FEEDBACK& feedback);		// 退纸币(MEI)(未使用)

	long ReturnCache(bool isCheckShutter = false);	// 退回缓冲区的纸币	

	long GetBoxID(BRH_BOX_ID& box_id);				// 获取钱箱ID
	//long GetBoxID(MEI_BOX_ID& box_id);				// 获取MEI钱箱ID
	//long Hold();									// 压暂存 ** MEI中  纸币接收识别 与 压暂存在同一动作（work）里面
	//long Clean(BRH_CLEAN_DEST dest);				// 清除钞票
	long GetStatus(tBncDevReturn& status);		// 读取状态
	long GetSystemStatus(tBncBillBoxInfo& queryCashUnit);		// 读取详细状态
	     

	long Repare(bool bMenualRepare = false);		// 异常解除
	static bool IsHWException(long errCode);		// 是否为硬件异常
	//bool IsBankNoteBoxAttached();                   // 判断纸币箱是否装上
	bool IsCashBoxAttached();						// 判断纸币回收箱是否装上	
	bool IsCashChangeBoxAttached();					// 判断纸币找零箱是否装上

	void OpenAndValidateAsync(CService* pSvc,tBncSetInhibitList& bhTable);	// 异步：开始接收识别纸币,可接收的面值列表

	//void HoldAsync(CService* pSvc);					// 异步：压暂存

	void CollectAsync(CService* pSvc);				// 异步：压钞

	void ReturnCacheAsync(CService* pSvc);			// 异步：退回缓冲区的纸币 MEI 中只有退出暂存中的全部纸币 

	void ReturnAllAsync(CService* pSvc);			// 异步：退回全部纸币  

	long TryRecoveryBanknoteModule();				// 尝试以退出方式修复模块

	long CheckModuleStatus();						// 检查模块状态


	// 维护用

	// 回收钞票
	long Empty(char* chBoxName,tBncChangeNum* result,bool btoFloat = false);
	// 更新回收箱计数
	long ClearCollectionUnitCount();

	// 获取箱子容量信息
	map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> GetEmptyFull();


	// 需要加一个同步计数的接口


public://纸币找零
	//void SendOutBill(CService* pService,int nRecycBoxA,int nRecycBoxB,int nRecycBoxC,int nCountBoxD,int nChangeBox);

	void DispenseAsync(CService* pSvc, UINT32 dispenseAmount);	// 异步：找零
	long Dispense(UINT32 dispenseAmount);						// 找零请求
	long Present(UINT32* presentAmount);						// 开始找零
	long CancelWaitingCashTaken();								// 取消等待用户取走钞票
	long Retract(UINT32* retractAmount);						// 回收钞票

private:
	long CheckAndOpenBHComport();								// 打开纸币接收模块，已打开则关闭后再打开。
	void ProcessErrorCode(long errCode);
};
