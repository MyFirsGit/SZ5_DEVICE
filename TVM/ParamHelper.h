#pragma once
#include <wtypes.h>
#include <vector>
#include "ParameterDef.h"
#include "CAfcTVMOperationParam.h"
#include "CAfcAccessLevelParam.h"
#include "CAfcDeviceCommonParam.h"
#include "CAfcStaffPwdParam.h"
#include "CAfcDeviceTimeParam.h"
#include "CAfcOperatorCodeTableParam.h"
#include "CAccBusinessParam.h"
#include "CAccServiceFeeParam.h"
#include "CAccLineInfoParam.h"
#include "CAccTicketParam.h"
#include "CAccNotServiceStation.h"
#include "cacctollstationparam.h"
#include "caccsingleblacklistparam.h"
#include "caccsectionblacklistparam.h"
#include "caccbasepriceparam.h"
#include "caccpricelevelparam.h"
#include "caccpriceadjuest.h"
#include "cacctimetypeparam.h"
#include "cacctimedetailparam.h"
#include "cacctimefloatparam.h"
#include "CAccDiscountParam.h"
#include "caccwritelistparam.h"
#include "caccothercitycard.h"
#include "caccovertimeadjuestparam.h"
#include "CAccDeviceControlParam.h"
#include "CAccIssueCompanyParam.h"
#include "CAccSectionParam.h"
// CParamHelper的唯一接口
#define thePARAM_HELPER CParamHelper::GetInstance()              

#define CD_NO_VERSION 0xFFFF
/**
@brief  参数管理类
*/

class CParamHelper
{
public:

	// 参数类型编码
	typedef	enum _param_type
	{
		ACC_VER  = 1,//ACC
		ECT_VER	 = 2,//ECT
		AFC_VER  = 3,//AFC
		UNKNOW   = 255 //未知
	} PARAM_TYPE;

	// 版本切换记录
	typedef struct _version_change_info {
		WORD paramId;		             // 参数ID
		PARAM_VERSION_TYPE_CODE versionType;		// 版本类型
		DWORD oldVersion;		// 旧版本号
		_DATE oldStartDate;		// 旧版本生效日期
		DWORD newVersion;		// 新版本号
		_DATE newStartDate;		// 新版本生效日期
		_version_change_info(){
			paramId = PARAM_UNKNOW;
			versionType = VER_TYPE_UNKNOW;
			oldVersion = 0;
			newVersion = 0;		
		}
	}VERSION_CHANGE_INFO;
	
	typedef std::vector<VERSION_CHANGE_INFO> VEC_VERSION_INFO;		// 版本切换信息
	typedef std::vector<WORD>	VEC_PARAM_ID;									  // 参数ID数组
	bool	InitParam();
	// 公共接口函数	
	bool   InitAllParam();														// 初始化所有参数
	bool   InitAccParam(bool flag);										// 初始化Acc参数
//	bool   InitAccParamNoVersionChange(bool flag);			// 初始化ACC参数(不检查版本切换)
//	bool   InitEctParam();													// 初始化Ect参数
	bool   InitAfcParam(bool readflag);								// 初始化Afc参数

// 	void GetCurrentCDVesion(CD_VERSION&,WORD&);						     // 根据CD版本信息设置当前CD版本并返回当前版本
// 	void SetCurrentCDVesion(CD_VERSION);								 // 设置当前CD版本并更新内存

	VEC_VERSION_INFO CheckAndChangeToFutVerAcc();				         // 检查参数并切换至未来版本（ACC参数）
	//VEC_VERSION_INFO ChangeFutVerToCurVer(PARAM_TYPE);			     // 将全部未来版本切换至当前版本
	void ChangeFutVerToCurVer(PARAM_TYPE paraType,CString sPrefix);	     // 将指定未来版本切换至当前版本
	void DeleteTheParamFile(CString sParamPrefix,BYTE bVerType);		 // 删除指定参数文件
	bool DeleteAllFileFromTheDir(CString sDirPath);					     // 删除指定目录下所有文件

	void UpdateLocalVersion(VEC_VERSION_INFO&, bool bUpdate=true);	// 更新当前版本

	void RestoreCurVersion(PARAM_TYPE,VEC_PARAM_ID,bool allFlag = true); // 还原当前正式版本

	void GetCurAccParamVersion(VERSION_ACC&);	// 获取ACC当前参数版本
	void GetFutAccParamVersion(VERSION_ACC&);	// 获取ACC未来参数版本

	//void GetCurEctParamVersion(VERSION_ECT&);	// 获取ECT当前参数版本
	//void GetFutEctParamVersion(VERSION_ECT&);	// 获取ECT未来参数版本

	void GetCurAfcParamVersion(VERSION_AFC&);	// 获取AFC当前参数版本
	void GetFutAfcParamVersion(VERSION_AFC&);	// 获取AFC未来参数版本

	

	PARAM_VERSION_TYPE_CODE GetParamVersionType(CString);			// 获取参数版本类型
	CString GetParamVerFromParamName(CString sParamName);			// 获取参数版本

	CString GetTheParamPathName(WORD paraID,CString ParamPath);		// 获取指定目录下的参数文件名

	CString GetTheParamBackUpPathName(WORD paraID,CString ParamPath);		// 获取指定目录下的参数文件名

	void RemoveParamTransHead(CString sParamPath);			// 删除指定文件夹下所有参数的传输协议头

	CString GetProgBatchNoFromProgName(CString sProgName);	// 获取程序批次号
	CString GetProgVerFromProgName(CString sProgName);		// 获取程序版本号
	PARAM_VERSION_TYPE_CODE GetProgVersionType(CString);			// 获取参数版本类型
	CString GetFileNameFromFullPath(CString sPathAndName);
	static CParamHelper&  GetInstance(){
		return theInstance;
	}


	CString GetCurXorSumVersion();
	CString GetFutXorSumVersion();
	std::map<PARAMETER_ID,CParameterMaster*> GetVersionProvider(){return mapVersionProvider;};
	CString* GetParameterNames(){return parameterNames;};
private:

	void GetVesionChangeInfo(VEC_VERSION_INFO&);      // 获取ACC版本切换信息

	CString GetTheParamPathNameImp(WORD paraID,CString ParamPath,CString basePath);		// 获取指定目录下的参数文件名

	CString GetCurParamName(CString,PARAM_TYPE,CString&,bool isBackup = false);			// 获取指定当前参数路径及文件名
	CString GetFutParamName(CString,PARAM_TYPE,CString&,bool isBackup = false);			// 获取指定未来参数路径及文件名
	PARAM_TYPE GetParamType(CString);								// 获得参数类型

	void BackUpLocalCurVersion(PARAM_TYPE,CString,CString);			// 备份当前参数
	void GetVersionChangeInfo(CString,CString,VERSION_CHANGE_INFO&);// 获取版本切换信息
	bool CompareVersion(DWORD ver1, DWORD ver2);					// 参数版本比较
	
	bool CheckIsNeedChangeParam(CString);							// 检查参数是否需要切换至未来版本
	CString ConvertFutFileNameToCurrentVersion(CString);            // 把将来版本文件名修改为当前版本

	// 构造,析构函数
	CParamHelper(void);
	~CParamHelper();
	CParamHelper(const CParamHelper&);
	CParamHelper& operator=(const CParamHelper&);

	static  CParamHelper theInstance;   // 该类的唯一接口

	CString m_strCurVersion;
	CString m_strFurVersion;
	std::map<PARAMETER_ID,CParameterMaster*> mapVersionProvider;//AFC和ACC参数，不包括应用程序
	CString parameterNames[32];
};
