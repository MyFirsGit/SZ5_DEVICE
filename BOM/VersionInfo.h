#pragma once
#include "delegate.h";

#define theVERSION_INI CVersionInfo::GetInstance()       // CVersionInfo的唯一接口

/**
@brief Versioninfo.INI的信息管理
*/

class CVersionInfo
{
public:
	// 版本类型
	typedef enum _VER_TYPE{
		CURRENT = 0,       // 当前版本
		FUTURE  = 1        // 将来版本
	}VER_TYPE;

	// 程序种类
	typedef enum _PROG_TYPE{
		MC = 1,
 		TH_MAIN = 2,
 		TH_APP = 4,
		RW_MAIN = 8,
		RW_APP = 16,
 		TW_MAIN = 32,
 		TW_APP = 64
	}PROG_TYPE;

    static CVersionInfo&  GetInstance()
    {
        return theInstance;
    }
	
	static CVersionInfo* GetInstance(CString fileName);
	static void DeleteInstance(CVersionInfo* cVersionInfo);

    int Initialize(CString fileName);                                // 初期化
	
	delegate<void(VER_TYPE,PROG_TYPE,DWORD)> VersionNoChanged;		//版本号改变事件
    DWORD GetVersionNo(VER_TYPE,PROG_TYPE);				// 获取版本号
	void SetVersionNo(VER_TYPE,PROG_TYPE,DWORD);        // 设置版本号

	int GetBatchNo(VER_TYPE,PROG_TYPE);					// 获取批次号
	void SetBatchNo(VER_TYPE,PROG_TYPE,int);			// 设置批次号

	int GetCompanyID(VER_TYPE,PROG_TYPE);				// 获取厂商ID
	void SetCompanyID(VER_TYPE,PROG_TYPE,int);			// 设置厂商ID

	_DATE GetStartDate(VER_TYPE,PROG_TYPE);				// 获取生效日期
	void SetStartDate(VER_TYPE,PROG_TYPE,_DATE);		// 设置生效日期

	int GetVersionType(VER_TYPE,PROG_TYPE);				// 获取版本类型
	void SetVersionType(VER_TYPE,PROG_TYPE,int);		// 设置版本类型

	bool SwitchVersion(__in PROG_TYPE progType,__in VER_TYPE fromVerType,__in VER_TYPE toVerType,__in bool removeFromVer = true);

	bool RemoveVersion(__in PROG_TYPE progType,__in VER_TYPE verType);

private:
    static CVersionInfo theInstance;
    CGetIni* m_versionIni;

	__inline CString GetSectionName(__in PROG_TYPE progTye);

    // 隐藏构造函数
    CVersionInfo();
    ~CVersionInfo();
    CVersionInfo(const CVersionInfo&);
    CVersionInfo& operator=(const CVersionInfo&);

    CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠
};