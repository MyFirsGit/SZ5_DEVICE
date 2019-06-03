#pragma once
#include "TVMForeService.h"
#include "SwitchOffSetMaintainDlg.h"

#define MAX_SWITCHOFF_NUM  6		//切离设置界面菜单的最大编号

/**
@brief    通信参数设定
*/
class CSwitchOffSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSwitchOffSetSvc();
	~CSwitchOffSetSvc();

	typedef enum _dialog_group{
		DIALOG_GROUP_SWITCHOFF_SET = 1,
	}DIALOG_GROUP;

	typedef struct _tagModel
	{
		BOOL UsedUnUsed;						// 启用/停用
		CString MenuRechargeRW;					// 充值读写器
		CString MenuSmartcardRW;				// Smart-card读写器
		//CString MenuTokenRW;					// Token读写器
		CString MenuBanknoteModule;				// 纸币模块
		CString MenuBanknoteChangeModule;		// 纸币找零模块
		CString MenuSmartcardModule;			// Smart-card模块
		//CString MenuTokenModule;				// Token模块
		CString MenuCoinModule;					// 硬币模块
		CString MenuReceiptPrint;				// 凭证打印机
		//CString MenuPinPadModule;				// 密码键盘
		//CString MenuBankCardModule;			// 银行卡模块
		int MenuNumber;							// 菜单号
		BOOL MenuChange;						// 菜单变化标志(菜单号由1变到2，这种变化置MenuChange为TRUE)
		BOOL CodeChange;						// 代码变化标志(用户改变了启/停用状态，置CodeChange为TRUE)
		BOOL CodeLegal;						    // 输入内容是否非法标志
		_tagModel()
		{
			UsedUnUsed = FALSE;
			MenuRechargeRW = _T("");			
			MenuSmartcardRW = _T("");			
			//MenuTokenRW = _T("");				
			MenuBanknoteModule = _T("");
			MenuBanknoteChangeModule = _T("");
			MenuSmartcardModule = _T("");		
			//MenuTokenModule = _T("");			
			MenuCoinModule = _T("");			
			MenuReceiptPrint = _T("");			
			//MenuPinPadModule = _T("");			
			//MenuBankCardModule = _T("");		
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeLegal = TRUE;
		}
	}Model;

	typedef struct _USSAVE						// 启用、停用状态保存
	{
		CString saveRechargeRW;					// 充值读写器
		CString saveSmartcardRW;				// Smart-card读写器
		//CString saveTokenRW;					// Token读写器
		CString saveBanknoteModule;				// 纸币模块
		CString saveBanknoteChangeModule;		// 纸币找零模块
		CString saveSmartcardModule;			// Smart-card模块
		//CString saveTokenModule;				// Token模块
		CString saveCoinModule;					// 硬币模块
		CString saveReceiptPrint;				// 凭证打印机
		//CString savePinPadModule;				// 密码键盘
		//CString saveBankCardModule;			// 银行卡模块
		_USSAVE()
		{
			saveRechargeRW = _T("");
			saveSmartcardRW = _T("");
			//saveTokenRW = _T("");
			saveBanknoteModule = _T("");
			saveBanknoteChangeModule = _T("");
			saveSmartcardModule = _T("");
			//saveTokenModule = _T("");
			saveCoinModule = _T("");
			saveReceiptPrint = _T("");
			//savePinPadModule = _T("");
			//saveBankCardModule = _T("");
		}
	}USSAVE;

	Model& GetDataModel();
	void ReadTvmIni();	    // 读取TVM.INI文件
	CString ReadUSEING(BOOL UsedUnUsed);
	void CheckMenuNumber();
	void InputGuideMsg();	// 对GuideMsg分别赋值
	void DoCheckInput();	// 对输入数据是否合理做出相应项的实际判断
	CString ChangeToStr(int UsedUnUsed);
	CString SaveDataChange(BOOL SaveData);												
	virtual void NotifyDataModelChanged();
protected:
	void OnStart();		// Service启动后处理
private:
	int EnterTime;
	CString GuideMsg;	// 向导栏提示
	Model m_DataModel;
	USSAVE UsSave;
	int DeviceType;		// 设备类型

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
};