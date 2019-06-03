#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccDeviceControlParam��Ψһ�ӿ�
#define theACC_DEVICECONTROL CAccDeviceControlParam::GetInstance()        

/**
 @brief   ACC�豸���Ʋ�����
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
	
	//SECTION_INFO m_section[1];	//�ֶ���Ϣ

    // ˽�г�Ա����
	

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccDeviceControlParam(void);
    ~CAccDeviceControlParam();
	CAccDeviceControlParam(const CAccDeviceControlParam&);
	CAccDeviceControlParam& operator=(const CAccDeviceControlParam&);

    static  CAccDeviceControlParam theInstance;
};
