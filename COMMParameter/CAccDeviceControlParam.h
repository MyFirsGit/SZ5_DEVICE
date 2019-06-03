#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccDeviceControlParam的唯一接口
#define theACC_DEVICECONTROL CAccDeviceControlParam::GetInstance()        

/**
 @brief   ACC设备控制参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccDeviceControlParam :public CAccParamMaster
{
public: 
	

    static CAccDeviceControlParam&  GetInstance()
    {
        return theInstance;
    }

protected:
	
private:
	
	//SECTION_INFO m_section[1];	//分段信息

    // 私有成员变量
	

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccDeviceControlParam(void);
    ~CAccDeviceControlParam();
	CAccDeviceControlParam(const CAccDeviceControlParam&);
	CAccDeviceControlParam& operator=(const CAccDeviceControlParam&);

    static  CAccDeviceControlParam theInstance;
};
