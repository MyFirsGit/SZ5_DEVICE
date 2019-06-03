#pragma once
#include "TVMForeService.h"

/**
@brief 硬币箱管理业务Service
*/
class CCoinBoxManageAddCoinToCycleChangeBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToCycleChangeBoxSvc();
	virtual ~CCoinBoxManageAddCoinToCycleChangeBoxSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_CYCLECHANGE_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币加币基本界面					
		SecondFrame,								// 硬币加币中界面
		ThridTFrame									// 硬币加币完成界面
	}FrameFlow;

	// 设备运行状态
	typedef enum _work_status{
		WORK_STATUS_READ_AMOUNT		= 0x0001,//读取金额
		WORK_STATUS_ACCEPT_COIN		= 0x0002,//押钞
		WORK_STATUS_BEGIN_RECIVE	= 0x0004,//接收中

	}WORK_STATUS;

	typedef struct _CycleChangeBoxData
	{
		CString CycleChangeCoinBoxID;						// 硬币循环找零箱ID
		CString CycleChangeCoinBoxTye;						// 硬币循环找零箱面值
		CString CycleChangeCoinBoxQuantity;					// 硬币循环找零箱数量
		CString CycleChangeCoinBoxAddingQuantity;			// 硬币循环找零箱加币数量
		CString CycleChangeCoinBoxAfterAddingQuantity;		// 硬币循环找零箱加币后数量
		int CycleChangeCoinBoxFrameFlowTime;				// 界面流转次数
		int nBeforeAddingStockCount;						// 加币前Stock数量
		int nAddingCoinCount;								// 硬币加币数量
		_CycleChangeBoxData()
		{
			CycleChangeCoinBoxID = _T("");
			CycleChangeCoinBoxTye = _T("");
			CycleChangeCoinBoxQuantity = _T("0");
			CycleChangeCoinBoxAddingQuantity = _T("0");
			CycleChangeCoinBoxAfterAddingQuantity = _T("0");
			CycleChangeCoinBoxFrameFlowTime = -1;
			nBeforeAddingStockCount = 0;
			nAddingCoinCount = 0;
		}
	}CycleChangeBoxData;

	typedef struct _tagModel
	{
		CycleChangeBoxData cyclechangeboxdata;
		BOOL IsHasException;
		BOOL IsPrinterSuccessfull;
		_tagModel()
		{
			IsHasException = FALSE;
			IsPrinterSuccessfull = TRUE;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();

	void CycleChangeHandleData();												// 硬币循环找零箱处理ID、数量信息
	void InitializeDataModel();													// 初始化数据模型
	CString GetHopperCoinType(int hopetype);									// 硬币循环找零箱面值转换
	void DoCoinAdding();														// 开始接收硬币处理
	void StopCancleCoinAdding();												// 停止或者取消加币
	void Sendcables();															// 上传电文
	void PrintCoinBoxExchangeReceipt();											// 硬币循环找零箱加币凭证打印
	void Dataintodatabase();													// 数据导入数据库
protected:
	void OnCoinInserted(UINT res,CCHReadReceivs* chReadReceiveAmount);			// 接收到硬币回调函数
protected:
	void OnStart();																// Service启动后处理
	//bool OnStop();																// Service停止后处理
private:

	int EnterTime;																// Enter键按下次数
	int MenuCode;																// 菜单编号
	int AddingCount;															// 加币数量
	int Stop_Insert;															// 停止读取金额标志
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）

	Model m_DataModel;
	long m_work_status;
};
