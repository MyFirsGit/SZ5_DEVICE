#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"

#define theSETTING CSetting::GetInstance()

class COMMON_API CSetting
{
public:
	static CSetting&  GetInstance();
	int Initialize();                              // 初期化
	delegate<void()> Initialized;

	// [APPLICATION]
	void SetDeviceTestMode(int);				// 设置设备测试模式
	int GetDeviceTestMode();					// 获取设备测试模式
	void SetLanguageMode(LANGUAGETYPE_T);					// 设置语言模式
	__inline LANGUAGETYPE_T GetLanguageMode();			// 获取语言模式
	delegate<void(LANGUAGETYPE_T&,LANGUAGETYPE_T&)> LanguageModeChanged;
	void SetPrintStaffID(CString);				// 设置打印员工ID
	void SetPrintLoginTime(_DATE_TIME);			// 设置打印登录时间
	CString GetPrintStaffID();					// 获取打印员工ID
	_DATE_TIME GetPrintLoginTime();				// 获取打印登录时间

	// [FTP]
	CString GetFileUploadFTPDir();				// 获取文件上传路径
	void SetFileUploadFTPDir(CString);			// 设置文件上传路径

	// [TH]
	delegate<void(int&)> THModeChanged;
	int GetTHMode();							// 获取出票机模式
	void SetTHMode(int);						// 设置出票机模式

	// [ICRW]/[ICTW]
	int GetRWUpdateFlag(bool bIsRW = true);		// 获取读写器UpdateError
	void SetRWUpdateFlag(int,bool bIsRW = true);// 设置读写器UpdateError

	// [RFID]
	int GetRFIDWriteStatus();					// 读取RFID可写状态
	void SetRFIDWriteStatus(bool isUsed);		// 写入RFID可写状态
private:
	// 隐藏构造函数
	CSetting();
	~CSetting();
	CSetting(const CSetting&);
	CSetting& operator=(const CSetting&);
	CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠

	static CSetting theInstance;
	CGetIni* m_settingIni;
	LANGUAGETYPE_T m_LanguageMode;
};