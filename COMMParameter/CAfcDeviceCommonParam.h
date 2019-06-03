#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcDeviceCommonParam的唯一接口
#define theAFC_COMMON CAfcDeviceCommonParam::GetInstance()        

/**
 @brief   设备公共参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAfcDeviceCommonParam :public CAfcParamMaster
{
public: 

	// 运营时间参数记录体
	 typedef struct _operation_time {
		DWORD delayTime24H;			// 24小时运营时票卡延长使用时间
		WORD  connectCheckInterval;	// SLE连接检测时间间隔
	
        _operation_time(){
            delayTime24H = 0;
            connectCheckInterval = 0;
        }
    }OPRATION_TIME;	

	// 通讯参数记录体
    typedef struct _communication{
		WORD txnInterval;				// 交易数据传送间隔		2	BIN	单位分钟，默认为1分钟
		WORD auditInterval;				// 审计时间间隔			2	BIN	单位分钟，默认值为15分钟
		WORD businessInterval;			// 业务数据传送间隔		2	BIN	单位分钟，默认值为15分钟
		WORD magazineInterval;			// 钱票箱数据产生间隔	2	BIN	单位分钟，默认值为30分钟
		WORD ntpInterval;				// 时钟同步时间间隔		2	BIN	单位分钟，开机后两次时钟同步间隔，默认15分钟
		WORD ntpAlarmValue;				// 同步时间报警差值		2	BIN	单位：秒。默认60秒。
		WORD ntpMalfunctionValue;		// 同步时间故障差值		2	BIN	单位：秒。默认300秒。
        _communication(){
            txnInterval = 0;
            auditInterval   = 0;
            businessInterval = 0;
            magazineInterval = 0;
			ntpInterval = 0;
            ntpAlarmValue   = 0;
            ntpMalfunctionValue = 0;
        }   
    } COMMUNICATION;

	// 共有接口
    void GetOperationTimeParam(OPRATION_TIME&);		// 获得运营时间参数
	void GetCommunication(COMMUNICATION&);			// 获得通讯参数
	
    static CAfcDeviceCommonParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[3];	//分段信息

    // 私有成员变量
    OPRATION_TIME	m_operation_param;		// 运营时间参数
	COMMUNICATION	m_communication_param;		// 范围通讯参数

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcDeviceCommonParam(void);
    ~CAfcDeviceCommonParam();
	CAfcDeviceCommonParam(const CAfcDeviceCommonParam&);
	CAfcDeviceCommonParam& operator=(const CAfcDeviceCommonParam&);

    static  CAfcDeviceCommonParam theInstance;
};
