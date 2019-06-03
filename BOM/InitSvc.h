#pragma once
#include "stdafx.h"

/**
@brief      初期化Service
*/
class CInitSvc :
    public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
    enum INIT_RESULT {
        UNDOWN = 0,             // 未知状态
        SUCCESS,                // 成功
        FAIL,                   // 失败
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
                result = UNDOWN;
				errorInfo = "";
				dealInfo = "";
            }
    } INITIAL_INFO;

	// 初期化显示行号
	enum ENUM_ROW {
		ROW_DATA_CHECK = 0,
		ROW_PARAMETER,
		ROW_RW,
		ROW_TW,		
		ROW_TH,
		ROW_PRINT,
	};

    static const int MAX_INITIAL_INFO_ROW = 6;          // 初期化显示信息行数
    INITIAL_INFO m_InitialInfo[MAX_INITIAL_INFO_ROW];   // 初期化显示信息

    CInitSvc(void);                                     // 构造函数
    ~CInitSvc(void);                                    // 析构函数
    virtual void OnStart();                             // Service启动后处理
	void SetInitRessult(int index, INIT_RESULT result); // 设置显示初期化结果
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;

    bool Initialize();                                  // 初期化
    bool InitializeController();                        // 初期化Controller
    bool CheckConsistent();                             // 数据一致性检查
    bool InitializeParameter();                         // 初期化系统参数
    bool InitializeRW();                                // 初期化读写器(RW)
	bool InitializeTW();								// 初期化读写器(TW)
    bool InitializePrinter();                           // 初期化打印机(Printer)
    bool InitializeTH();								// 初期化出票机
	bool InitializeLPT();								// 初期化出票机门禁开关
	bool InitializeUPS();								// 初期化UPS
    bool InitializeCPS();								// 初期化上位(CPS)
    //INIT_RESULT ACCAccredit();                          // ACC授权（一卡通）认证
    //INIT_RESULT BMACAccredit();                         // 充值终端授权（一卡通）认证

    bool CheckCreateHomeFolder();                       // 检查创建数据主目录
    bool CheckCreateBackupFolder();                     // 检查创建备份目录
    void DeleteLog(int days);                           // 删除指定天数前的FTP上传的日志
	void DeleteChargeReceiptFile(int days);				// 删除指定天数前的充值单据文件
	void DeleteTpuLog(int days);                        //删除指定天数前的日志文件
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);    //获取指定天数的Tpulog日志文件日期
	void GetLimitLogDate(int days,vector<int>& logdate);           //获取指定天数的log日志文件日期
	//CString GetInitErrorInfo(CSysException);			// 获取异常显示信息
	//void AddUnsendAudit();								// 追加未送集计
	void DoException(CSysException& e,int index);
	void CheckSAMidChange(SAM_POS_ samPos);
private:

	void OnExceptionNotify(CSysException&,bool);
};
