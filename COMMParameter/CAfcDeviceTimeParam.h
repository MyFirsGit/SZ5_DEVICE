#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcDeviceTimeParam��Ψһ�ӿ�
#define theAFC_DEV_TIME CAfcDeviceTimeParam::GetInstance()        

/**
 @brief   �豸����ʱ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAfcDeviceTimeParam :public CAfcParamMaster
{
public: 

	// ��Ӫʱ�������¼��
	typedef struct _operation_time {
		_TIME oprationBeginTime;			// ��ʼ��Ӫʱ��		2	BCD	HH24MI���豸�Զ���ʼ��Ӫ����ʱ��
		_TIME oprationEndTime;			// ������Ӫʱ��		2	BCD	HH24MI���豸�Զ�������Ӫ����ʱ��
		_TIME oprationDateChangeTime;	// ��Ӫ���л�ʱ��	2	BCD	HH24MI���豸�л���Ӫ��ʱ�䡣����ʱ����ͣ�ò�������Ӫ���л���
    }OPRATION_TIME;	

	// ���нӿ�
    void GetOperationTimeParam(OPRATION_TIME&);		// �����Ӫʱ�����
	
    static CAfcDeviceTimeParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[2];	//�ֶ���Ϣ

    // ˽�г�Ա����
    OPRATION_TIME	m_operation_param;			// ��Ӫʱ�����
	
    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcDeviceTimeParam(void);
    ~CAfcDeviceTimeParam();
	CAfcDeviceTimeParam(const CAfcDeviceTimeParam&);
	CAfcDeviceTimeParam& operator=(const CAfcDeviceTimeParam&);

    static  CAfcDeviceTimeParam theInstance;
};
