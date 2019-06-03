#pragma once
#include "stdafx.h"

/**
@brief      初期化Service
*/
class CInitSvc :  public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
    enum INIT_RESULT {
        UNKNOWN = 0,			// 未知状态
        SUCCESS,				// 成功
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
		ROW_DISK_CHECK = 0,
		ROW_DATA_INIT,
		ROW_IO_MODULE,
		ROW_CHARGE_RW,
		//ROW_TOKEN_RW,
		ROW_CARD_RW,
		//ROW_TOKEN_MODULE,
		ROW_CARD_MODULE,
		ROW_COIN_MODULE,
		ROW_BANKNOTE_MODULE,
		//ROW_BANKNOTE_CHANGE_MODULE,
		ROW_PRINT,
		//ROW_BANKCARD_MODULE,
		//ROW_PINPAD_MODULE,
		ROW_DISPLAY,
		ROW_MAX,
		ROW_INVALID,
				
	};

    INITIAL_INFO m_InitialInfo[ROW_MAX];					// 初期化显示信息
	int m_order[ROW_MAX];											// 是否初始化，对初始化内容重新排序

    CInitSvc(void);															// 构造函数
    ~CInitSvc(void);															// 析构函数
    virtual void OnStart();												// Service启动后处理
	void SetInitRessult(int index, INIT_RESULT result);		// 设置显示初期化结果

private:	
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;

    bool Initialize();                                       // 初期化处理
    bool InitializeController();                        // 初期化Controller
	void StartBackService();                           // 启动后台服务
	
	bool CheckConsistent();                           // 数据一致性检查
	bool CheckDiskFreeSpace();							// 检查磁盘空间
	bool InitializeSystemConfig();				   // -初始化系统配置
	bool InitializeOSSetting();						   // -初始化操作系统配置
	bool InitializeSystemFolder();				       // -初始化系统目录
    
    bool InitializeParameter();                        // 初期化系统参数
	bool InitializeSystemLog();						   // -初始化系统日志
	bool InitializeSystemException();			   // -初始化系统异常
	bool InitializeSystemMaster();				   // -初始化系统Master
	bool InitializeTxnData();							   // -初始化交易数据
	bool InitializeParamData();					   // -初始化参数数据
	bool InitializeDatabase();						   // -初始化数据库
	bool InitializeRegistAudit();					   // -初始化寄存器数据
	bool InitializeSCAudit();							   // -初始化SC审计数据
	bool InitializeSystemTime();						// -初始化系统时间

	// 初始化硬件模块
	bool InitializeStatusDisplay();						//初始化条屏
	bool InitializeChargeRW();							// 初始化充值读写器
	bool InitializeCardRW();							// 初始化Card读写器
	bool InitializeTokenModule();						// 初始化token模块
	bool InitializeCardModule();						// 初始化Card模块
	bool InitializeCoinModule();						// 初始化硬币模块
	bool InitializeBanknoteModule();					// 初始化纸币接收模块
	bool InitializeBanknoteChangeModule();				// 初始化纸币找零模块
	bool InitializePrinter();							// 初期化打印机(Printer)
	bool InitializeIO();								// 初始化IO模块
	//bool InitializeUPS();								// 初期化UPS
	void StartDeviceMonitor();							// 启动设备监控线程
	
    bool InitializeCPS();									// 初期化上位(CPS)
    
	// 在线认证
	//INIT_RESULT ACCAccredit();                          // ACC授权（一卡通）认证
    //INIT_RESULT BMACAccredit();                         // 充值终端授权（一卡通）认证
private:	
    bool CheckCreateHomeFolder();													// 检查创建数据主目录
    bool CheckCreateBackupFolder();													// 检查创建备份目录
    void DeleteLog(int days);																// 删除指定天数前的FTP上传的日志
	void DeletePicture();																		// 删除临时图片
	void DeleteTpuLog(int days);															//删除指定天数前的日志文件
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);     //获取指定天数的Tpulog日志文件日期
	void GetLimitLogDate(int days,vector<int>& logdate);					//获取指定天数的log日志文件日期
	//CString GetInitErrorInfo(CSysException);										// 获取异常显示信息
	void CheckSAMidChange(SAM_POSITION samPos);

private:	
	void DoException(CSysException& e,int index);
	void OnExceptionNotify(CSysException&,bool);

	void DeletePrintHistory(int days);								// 删除打印历史记录
	void GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir = false);// 取得指定目录下指定类型应该删除天数之外的文件
};
