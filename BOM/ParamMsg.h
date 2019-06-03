#pragma once
#include "svcdef.h"
#include <afxtempl.h>
#include "ParameterDef.h"
#include "scapi.h"
#include "SCSvcDef.h"

typedef enum _ykt_parameter_type{
	BMAC_PARAM_CARDTYPE   = 0,
	BMAC_PARAM_COMMCONFIG = 1
} YKT_PARAM_TYPE;

#define thePARAM_MSG    CParamMsg::GetInstance()		// CParamMsg的唯一接口

/**
@brief 上位电文paramter message管理                                                                    
*/
class CParamMsg
{
private:
	CParamMsg(void);
	~CParamMsg(void);

public:
	static CParamMsg& GetInstance();

	void SetAccParameterVersion();           // 获得ACC参数版本
	void SetECTParameterVersion();           // 获得一卡通参数版本
	void SetAfcParameterVersion();           // 获得AFC系统参数 版本

	void SetOperationMode(BYTE bMode);
	BYTE GetOperationMode(void);

	void Set24HourMode(BYTE bMode);
	BYTE Get24HourMode(void);

	void SetDelayMode(BYTE wMode);
	BYTE GetDelayMode(void);
	void SaveMsg(LPBYTE lpMsg);
public:
	VERSION_ACC  m_cur_version_acc;          // ACC当前版
	VERSION_ACC  m_future_version_acc;	     // ACC将来版
	
	VERSION_ECT  m_version_ect;              // 一卡通参数版本
	VERSION_AFC  m_version_afc;              // AFC系统参数版本

	CD_VERSION  m_CurCDVersion;              // 当前CD版本

private:
	BYTE  m_bOperationMode;
	BYTE  m_b24HourMode;
	BYTE  m_wDelayMode;
// 	VERSION_ACC m_version_acc;
// 	VERSION_ECT m_version_ect;
// 	VERSION_AFC m_version_afc;
private:
	static CParamMsg theInstance;
};
