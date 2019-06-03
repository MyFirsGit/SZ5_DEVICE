#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define theSOFT_VERSION    CCTLOGSoftVer::GetInstance()             ///< CCTLOGSoftVer的唯一接口

/**
@brief 版本信息管理master   \n
       包括对各部分版本信息的读取与更新
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGSoftVer
{
public:	

	typedef struct _bomsoft_info{
		WORD wSerialNo;   // 批次版本号 2 BIN
		BYTE bVerType;	  // 版本类型   1 BIN	
		DWORD dwVersion;  // 版本号     4 BIN	有效值范围0x00000000~0xFFFFFFFE，无效时为0xFFFFFFFF。传输时转换成INTEL序。
		_bomsoft_info(){
			wSerialNo = 0;
			bVerType  = 0;
			dwVersion = 0;
		}
	}BOMSOFT_INFO;

	//版本信息
	typedef struct _version_info {
		BYTE ulRWSAMID1[6];               // 读写器SAM ID 1 一卡通PSAM卡号
		BYTE ulRWSAMID2[6];               // 读写器SAM ID 2 一卡通ISAM卡号
		BYTE ulRWSAMID3[6];               // 读写器SAM ID 3 地铁PSAM卡号
		BYTE ulRWSAMID4[6];               // 读写器SAM ID 4 地铁ISAM卡号
		BYTE ulOutRWSAMID1[6];            // 外部读写器SAM ID 1 一卡通PSAM卡号
		BYTE ulOutRWSAMID2[6];            // 外部读写器SAM ID 2 一卡通ISAM卡号
		BYTE ulOutRWSAMID3[6];            // 外部读写器SAM ID 3 地铁PSAM卡号
		BYTE ulOutRWSAMID4[6];            // 外部读写器SAM ID 4 地铁ISAM卡号
		char ulAppliVersion[4];           // 控制程序版本(BOM APPLI) 
		char ulTHRWVersion[4];            // TH RW版本
		char ulICRWVersion[4];            // IC RW版本
		char rwBootVer[4];                // 外部读写器boot版本
		char thRwBootVer[4];              // 内部读写器boot版本
		//char thBootVersion[2];            // TH boot 部的版本
		SC_VER_INFO thInfo;               // TH版本信息
		SC_VER_INFO thBootInfo;           // TH Boot 版本
		DWORD dwModeHistory;              // 模式履历
		BOMSOFT_INFO BomSoft_info;        // BOM版本信息
		_version_info()
		{
			memset(ulRWSAMID1,0x00,6);
			memset(ulRWSAMID2,0x00,6);
			memset(ulRWSAMID3,0x00,6);
			memset(ulRWSAMID4,0x00,6);
			memset(ulOutRWSAMID1,0x00,6);
			memset(ulOutRWSAMID2,0x00,6);
			memset(ulOutRWSAMID3,0x00,6);
			memset(ulOutRWSAMID4,0x00,6);
			memset(ulAppliVersion,0x00,4);         
			memset(ulTHRWVersion,0x00,4);
			memset(ulICRWVersion,0x00,4);
			memset(rwBootVer,0x00,4);
			memset(thRwBootVer,0x00,4);
			dwModeHistory = 0;
		}
	} VERSION_INFO;


	BOOL Internalize(CString dataPath, CString backupPath);     // 初期化

	void GetSoftVerData(VERSION_INFO &svData);                  // 读取版本信息

	void SetRWSAMID(SLOT_ID, const BYTE ulID[]);                // 设置读写器SAM编号
	void SetOutRWSAMID(SLOT_ID, const BYTE ulID[]);             // 设置外部读写器SAM编号
	BOOL SetAPIVersion(const char[]);                           // 设置控制部版本
	BOOL SetTHRWVersion(const char[]);                          // 设置TH RW版本
	BOOL SetICRWVersion(const char[]);                          // 设置IC RW版本
	void SetTHVersionInfo(SC_VER_INFO);                         // 设置TH版本信息
	void SetTHBootVersionInfo(SC_VER_INFO);                     // 设置TH Boot版本信息
	void SetRWBootVer(const char[]);                            // 设置RW Boot版本信息
	void SetTHRWBootVer(const char[]);                          // 设置THRW Boot版本信息
	void SetModeHistoryVer(DWORD ver);                          // 设置模式履历版本信息
	void SetBOMSoftInfo(WORD wSerialNo, BYTE verType, DWORD dwVersion);  // 设置BOM软件版本信息
	static CCTLOGSoftVer&  GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		SOFT_VER_LEN	= 111,									  ///< 文件长度
	};

	// 字段所在位置
	enum {
		RW_SAMID_POS        = 0,                                  // 读写器          
		OUTRW_SAMID_POS     = 24,                                 // 外部读写器
		API_VERSION_POS     = 48,                                 // 控制程序版本
		RW_BOOT_VERSION     = 52,                                 // RW Boot版本信息
		//TH_BOOT_VERSION     = 52,                                 // 出票机的BOOT部的版本
		RW_HARD_POS         = 56,                                 // RW版本(IC读写器)      
		RW_SOFT_POS         = 60,                                 // RW版本(TH读写器)
		TH_TYPE_POS         = 64,                                 // TH形式
		TH_VERSION_POS      = 74,                                 // TH版本号
		TH_SUM_POS          = 76,                                 // TH的SUM值
		TH_BOOT_TYPE_POS    = 80,                                 // TH形式(boot)
		TH_BOOT_VERSION_POS = 90,                                 // TH版本号(boot)
		TH_BOOT_SUM_POS     = 92,                                 // TH的SUM值(boot)
		TH__RW_BOOT_VERSION = 96,                                 // THRW Boot版本信息
		MODE_HISTORY        = 100,                                // 模式履历版本
		BOM_SOFT_INFO       = 104                                 // BOM软件版本信息
	};    

	VERSION_INFO m_svSoftVersion;                                   // 版本信息
	CDXFile       *m_file;                                           // 文件读取控制类
	char m_fileData[SOFT_VER_LEN];                                  // 当前文件内容

	static  CCTLOGSoftVer theInstance;
	CCTLOGSoftVer(void);
	~CCTLOGSoftVer();
	CCTLOGSoftVer(const CCTLOGSoftVer&);
	CCTLOGSoftVer& operator=(const CCTLOGSoftVer&);
};
