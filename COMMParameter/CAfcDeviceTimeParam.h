#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcDeviceTimeParam的唯一接口
#define theAFC_DEV_TIME CAfcDeviceTimeParam::GetInstance()        

/**
 @brief   设备运行时间参数
*/

class PARAMETERCOMMAND_DECLSPEC CAfcDeviceTimeParam :public CAfcParamMaster
{
public: 

	// 运营时间参数记录体
	typedef struct _operation_time {
		_TIME oprationBeginTime;			// 开始运营时间		2	BCD	HH24MI，设备自动开始运营服务时间
		_TIME oprationEndTime;			// 结束运营时间		2	BCD	HH24MI，设备自动结束运营服务时间
		_TIME oprationDateChangeTime;	// 运营日切换时间	2	BCD	HH24MI，设备切换运营日时间。运行时间表的停用不包含运营日切换。
    }OPRATION_TIME;	

	// 共有接口
    void GetOperationTimeParam(OPRATION_TIME&);		// 获得运营时间参数
	
    static CAfcDeviceTimeParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[2];	//分段信息

    // 私有成员变量
    OPRATION_TIME	m_operation_param;			// 运营时间参数
	
    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcDeviceTimeParam(void);
    ~CAfcDeviceTimeParam();
	CAfcDeviceTimeParam(const CAfcDeviceTimeParam&);
	CAfcDeviceTimeParam& operator=(const CAfcDeviceTimeParam&);

    static  CAfcDeviceTimeParam theInstance;
};
