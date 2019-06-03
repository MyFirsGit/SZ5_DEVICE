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

#define thePARAM_MSG    CParamMsg::GetInstance()		// CParamMsg��Ψһ�ӿ�

/**
@brief ��λ����paramter message����                                                                    
*/
class CParamMsg
{
private:
	CParamMsg(void);
	~CParamMsg(void);

public:
	static CParamMsg& GetInstance();

	void SetAccParameterVersion();           // ���ACC�����汾
	void SetECTParameterVersion();           // ���һ��ͨ�����汾
	void SetAfcParameterVersion();           // ���AFCϵͳ���� �汾

	void SetOperationMode(BYTE bMode);
	BYTE GetOperationMode(void);

	void Set24HourMode(BYTE bMode);
	BYTE Get24HourMode(void);

	void SetDelayMode(BYTE wMode);
	BYTE GetDelayMode(void);
	void SaveMsg(LPBYTE lpMsg);
public:
	VERSION_ACC  m_cur_version_acc;          // ACC��ǰ��
	VERSION_ACC  m_future_version_acc;	     // ACC������
	
	VERSION_ECT  m_version_ect;              // һ��ͨ�����汾
	VERSION_AFC  m_version_afc;              // AFCϵͳ�����汾

	CD_VERSION  m_CurCDVersion;              // ��ǰCD�汾

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
