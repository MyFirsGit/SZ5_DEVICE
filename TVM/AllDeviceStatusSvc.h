#pragma once
#include "TVMForeService.h"
#pragma warning(disable : 4482)

/**
@brief 硬件自检所有设备状态
*/
class CAllDeviceStatusSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:

	enum INIT_RESULT {
		UNKNOWN = 0,			// 未知状态
		SUCCESSFUL,				// 成功
		FAIL,					// 失败
	};

	// 初期化显示信息结构
	typedef struct _INITIAL_INFO {
		CString title;          // 文字
		INIT_RESULT result;     // 结果
		CString errorInfo;		// 详细错误信息
		CString dealInfo;		// 处理建议信息
		_INITIAL_INFO()
		{
			title = "";
			result = UNKNOWN;
			errorInfo = "";
			dealInfo = "";
		}
	} INITIAL_INFO;

	// 初期化显示行号
	enum ENUM_ROW {
		ROW_IO_MODULE = 0,
		//ROW_CHARGE_RW,
		ROW_CARD_RW,
		//ROW_CARD_MODULE,
		ROW_TOKEN_MODULE,
		ROW_COIN_MODULE,
		ROW_BANKNOTE_MODULE,
		//ROW_BANKNOTE_CHANGE_MODULE,
		ROW_PRINT,
		ROW_MAX,
		ROW_INVALID,
	};

	INITIAL_INFO m_InitialInfo[ROW_MAX];					// 初期化显示信息
	int m_order[ROW_MAX];									// 是否初始化，对初始化内容重新排序

	CAllDeviceStatusSvc(void);								// 构造函数
	~CAllDeviceStatusSvc(void);								// 析构函数
	virtual void OnStart();									// Service启动后处理
	void SetInitRessult(int index, INIT_RESULT result);		// 设置显示初期化结果

private:	
	typedef enum _dialog_group {
		DIALOG_GROUP_BASE = 1,
	} DIALOG_GROUP;
	void InitDataModel();
	// 初始化硬件模块
	bool InitializeStatusDisplay();						// 初始化条屏
	bool InitializeChargeRW();						    // 初始化充值读写器
	bool InitializeCardRW();							// 初始化Card读写器
	bool InitializeCardModule();						// 初始化Card模块
	bool InitializeTokenModule();						// 初始化token模块
	bool InitializeCoinModule();						// 初始化硬币模块
	bool InitializeBanknoteModule();				    // 初始化纸币接收模块
	bool InitializeBanknoteChangeModule();				// 初始化纸币找零模块
	bool InitializePrinter();							// 初期化打印机(Printer)
	bool InitializeIO();								// 初始化IO模块
private:	
	void DoException(CSysException& e,int index);
	LRESULT OnKeyboardF3(WPARAM , LPARAM );             //处理[F3/Ctrl]键按下消息（后退）
	bool m_bIsFinished;
	//void OnExceptionNotify(CSysException&,bool);
};
