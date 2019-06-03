#pragma once
#include <vector>
#include "scmessagecontentdefine.h"
#include "versioninfo.h"
#include "sccommand.h"

using namespace std;

#define  theUPGRADE_HELPER CUpgradeHelper

static const char* const STR_BOM_MC_00  = "BOM_MC_00";	// BOM程序
static const char* const STR_BOM_TC_00  = "BOM_TC_00";	// TH应用程序
static const char* const STR_BOM_TC_01  = "BOM_TC_01";	// TH主控程序
static const char* const STR_BOM_RW_00  = "*4109*";		// RW应用程序
static const char* const STR_BOM_RW_01  = "*4110*";		// RW主控程序
static const char* const STR_TVM_TH_APP	= "*4111*";		// TVM 的TH应用程序

class  CUpgradeHelper
{

public:

	typedef enum  _UpgradeProcess
	{
		BEGIN = 0x0001,
		SUCCEED = 0x0002,
		FAILED = 0x0004
	} UPGRADE_PROCESS ;

	static CString m_strCurrentDirectory;

	//升级过程中通知处理函数定义。
	typedef void (*OnUpgradeProcessFunc)(CVersionInfo::PROG_TYPE,UPGRADE_PROCESS);


	//****************************************************************************
	// 
	// function  : 下载升级文件
	// input:    vector<ParameterAndSoftwareUpdate> downLoadFiles 需要下载的升级文件的列表
	// output:   无   
	// return:	  true 下载成功
	//			  false 下载失败
	//****************************************************************************
	static bool DownLoadFiles(vector<ParameterAndSoftwareUpdate> downLoadFiles);

	//****************************************************************************
	// 
	// function  : 移动下载的升级程序到升级目录
	// input:     无
	// output:   无   
	// return:	  true 升级成功
	//			  false 升级失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolder();

	//****************************************************************************
	// 
	// function  : 检查BOM及其他程序是否需要升级
	// input:     无
	// output:   无   
	// return:	  true 需要升级
	//			  false 不需要升级
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade();
	
	//****************************************************************************
	// 
	// function  : 升级程序
	// input:     升级过程通知处理函数
	// output:   无   
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(OnUpgradeProcessFunc func);


	//****************************************************************************
	// 
	// function  : 采用默认通知处理函数升级程序
	// input:     无
	// output:   无   
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade();

	//****************************************************************************
	// 
	// function  : 采用给定通知处理函数升级特定的程序。
	// input:     CVersionInfo::PROG_TYPE& progType 要升级的程序类型。
	// input:     OnUpgradeProcessFunc func 通知处理函数。
	// output:      
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(CVersionInfo::PROG_TYPE progType,OnUpgradeProcessFunc func);

	//****************************************************************************
	// 
	// function  : 采用默认通知处理函数升级特定的程序。
	// input:     CVersionInfo::PROG_TYPE& progType 要升级的程序类型。
	// output:      
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(CVersionInfo::PROG_TYPE progType);
	//****************************************************************************
	// 
	// function : 获得存放下载文件的目录。
	// input:    PARAMETER_ID& progType 程序类型
	// input:   PARAM_VERSION_TYPE_CODE& verCode 升级的版本类型。      
	// return:	  CString 升级路径
	//			  
	//****************************************************************************
	__inline static CString GetDownloadFolder(__in PARAMETER_ID& progType,__in  PARAM_VERSION_TYPE_CODE& verCode);

	//****************************************************************************
	// 
	// function : 更新Voice and Template模板文件
	// input:    
	// input:         
	// return:	  bool true：成功  false：失败
	//			  
	//****************************************************************************
	static bool UpdateVoiceAndTemplateFolder();

	static bool CheckTPUNeedUpgrade();
private:

	//****************************************************************************
	// 
	// function  : 程序在升级开始，成功或失败通过的处理函数
	// input:     CVersionInfo::PROG_TYPE& progType 程序类型
	// input:     UPGRADE_PROCESS& process 通知类型
	// output:    无       
	// return:	  无
	//			 
	//****************************************************************************
	static void OnUpgrade(CVersionInfo::PROG_TYPE progType,UPGRADE_PROCESS process);

	//****************************************************************************
	// 
	// function  : 检查给定类型程序是否需要升级
	// input:     CVersionInfo::PROG_TYPE progType 程序类型
	// output:    CVersionInfo::VER_TYPE& verType 升级的版本类型。         
	// return:	  true 需要升级，升级版本由verType返回
	//			  false 不需要升级
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade(__in CVersionInfo::PROG_TYPE progType,__out CVersionInfo::VER_TYPE& verType);

	
	//****************************************************************************
	// 
	// function  : 移动下载的BOM程序升级程序到升级目录
	// input:    无
	// output:   无         
	// return:	  true 成功
	//			  false 失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderMC();

	//****************************************************************************
	// 
	// function  : 移动下载的GUI文件到升级目录
	// input:    无
	// output:   无         
	// return:	  true 成功
	//			  false 失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderGUI();

	//****************************************************************************
	// 
	// function  : 移动下载的线路站点车站文件到升级目录
	// input:    无
	// output:   无         
	// return:	  true 成功
	//			  false 失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderStationMap();

	//****************************************************************************
	// 
	// function  : 移动下载的TPU程序升级程序到升级目录
	// input:    无
	// output:   无         
	// return:	  true 成功
	//			  false 失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderTPU();


	//****************************************************************************
	// 
	// function  : 移动下载的TH程序升级程序到升级目录
	// input:    无
	// output:   无         
	// return:	  true 成功
	//			  false 失败
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderTH();


	//****************************************************************************
	// 
	// function  : 检查当前版本文件夹和将来版本文件夹中升级文件是否能够启用
	// input:     CString& currentVersionFolder 当前版本升级文件存放的文件夹 
	//			  CString& futureVersionFolder  将来版本升级文件存放的文件夹
	// output:    CVersionInfo::VER_TYPE& verType 升级的版本类型。         
	// return:	  true 需要升级，升级版本由verType返回
	//			  false 不需要升级
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade(__in CString& currentVersionFolder,__in CString& futureVersionFolder,__out CVersionInfo::VER_TYPE& verType);

	//****************************************************************************
	// 
	// function : 检查给定的升级文件描述的生效时间是否启用。
	// input:     ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate 升级描述类。
	// output:    无       
	// return:	  true 已过生效时间，应该升级
	//			  false 未到生效时间，不应该升级
	//****************************************************************************
	static bool CheckDownloadFileUpgradeDateHasStart(__in const ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate);

	//****************************************************************************
	// 
	// function : 检查某升级目录下升级文件是否能够启用。
	// input:     CString& upgradeDataFolder 升级文件存放的目录。
	// output:    ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate 升级描述类       
	// return:	  true 已过生效时间，应该升级
	//			  false 未到生效时间，不应该升级
	//****************************************************************************
	static bool CheckDownloadFileUpgradeDateHasStart(__in const CString& upgradeDataFolder,__out ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate);

	//****************************************************************************
	/**
	@brief      解压缩程序更新包

	@param      (i)CString sProgramFileUpdateDir   压缩文件路径
	@param      (i)CString sFileName			   压缩文件名
	@param      (i)bool isAPP					   是否是BOM主程序

	@retval     bool

	@exception  无
	*/
	//****************************************************************************
	static bool ExtractUpdateFileToCurrentFolder(CString sProgramFileUpdateDir,CString sFileName,bool isAPP);

	//****************************************************************************
	/**
	@brief      解压缩程序更新包

	@param      (i)CString sProgramFileUpdateDir   压缩文件路径
	@param      (i)CString sFileName			   压缩文件名
	@param      (i)bool isAPP					   是否是BOM主程序

	@retval     bool

	@exception  无
	*/
	//****************************************************************************
	static bool ExtractUpdateFile(CString sFilePath, CString targetFolder, bool isAPP);

	//****************************************************************************
	// 
	// function : 获得存放下载程序升级文件的目录。
	// input:     CVersionInfo::PROG_TYPE progType 程序类型
	// input:    CVersionInfo::VER_TYPE& verType 升级的版本类型。      
	// return:	  CString 升级路径
	//			  
	//****************************************************************************
	__inline static CString GetDownloadFolder(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType);




	//****************************************************************************
	// 
	// function : 获得升级文件存放的目录。
	// input:     CVersionInfo::PROG_TYPE progType 程序类型 
	// output:	 vector<CString>& updateFolders 升级文件存放的目录
	// return:	  无
	//			  
	//****************************************************************************
	__inline static void GetUpgradeFolderFromDownloadProgramType(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders);


	//****************************************************************************
	// 
	// function : 获得升级文件存放的目录。
	// input:     CVersionInfo::PROG_TYPE progType 程序类型 
	// output:	 vector<CString>& updateFolders 升级文件存放的目录
	// return:	  无
	//			  
	//****************************************************************************
	__inline static void GetUpgradeFolder(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders);


	//****************************************************************************
	// 
	// function : 获得升级文件。
	// input:     CVersionInfo::PROG_TYPE progType 程序类型
	// input:    CVersionInfo::VER_TYPE& verType 升级的版本类型。 
	// output:	 vector<CString>& upgradeFiles 升级文件
	// return:	  无
	//			  
	//****************************************************************************
	__inline static void GetDownloadUpgradeFiles(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType,__out vector<CString>& upgradeFiles);

	//****************************************************************************
	// 
	// function : 将VersionCode转化为VersionType。
	// input:	 const PARAM_VERSION_TYPE_CODE& verCode
	// output:	 CVersionInfo::VER_TYPE& verType
	// return:   bool 成功转换，否则转换不成功。 
	//			  
	//****************************************************************************
	__inline static bool TryConvertVerCodeToVerType(__in const PARAM_VERSION_TYPE_CODE& verCode,__out CVersionInfo::VER_TYPE& verType);

	//****************************************************************************
	// 
	// function : 将PARAMETER_ID转化为PROG_TYPE。
	// input:	 const PARAMETER_ID& praameterID
	// output:	 CVersionInfo::PROG_TYPE& progType
	// return:   bool 成功转换，否则转换不成功。 
	//			  
	//****************************************************************************
	__inline static bool TryConvertParameterIDToProgarmType(__in const PARAMETER_ID& praameterID,__out CVersionInfo::PROG_TYPE& progType);

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      拷贝目录
	@param      none
	@retval     none
	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	static BOOL CopyDirectory(LPCTSTR lpszSourceDir, LPCTSTR lpszDestDir);
	static BOOL RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir, BOOL bIsFirst);
	static CString RenServerName2LocalName(CString sServerFileName);
};