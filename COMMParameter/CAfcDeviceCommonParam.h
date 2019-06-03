#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcDeviceCommonParam��Ψһ�ӿ�
#define theAFC_COMMON CAfcDeviceCommonParam::GetInstance()        

/**
 @brief   �豸����������
*/

class PARAMETERCOMMAND_DECLSPEC CAfcDeviceCommonParam :public CAfcParamMaster
{
public: 

	// ��Ӫʱ�������¼��
	 typedef struct _operation_time {
		DWORD delayTime24H;			// 24Сʱ��ӪʱƱ���ӳ�ʹ��ʱ��
		WORD  connectCheckInterval;	// SLE���Ӽ��ʱ����
	
        _operation_time(){
            delayTime24H = 0;
            connectCheckInterval = 0;
        }
    }OPRATION_TIME;	

	// ͨѶ������¼��
    typedef struct _communication{
		WORD txnInterval;				// �������ݴ��ͼ��		2	BIN	��λ���ӣ�Ĭ��Ϊ1����
		WORD auditInterval;				// ���ʱ����			2	BIN	��λ���ӣ�Ĭ��ֵΪ15����
		WORD businessInterval;			// ҵ�����ݴ��ͼ��		2	BIN	��λ���ӣ�Ĭ��ֵΪ15����
		WORD magazineInterval;			// ǮƱ�����ݲ������	2	BIN	��λ���ӣ�Ĭ��ֵΪ30����
		WORD ntpInterval;				// ʱ��ͬ��ʱ����		2	BIN	��λ���ӣ�����������ʱ��ͬ�������Ĭ��15����
		WORD ntpAlarmValue;				// ͬ��ʱ�䱨����ֵ		2	BIN	��λ���롣Ĭ��60�롣
		WORD ntpMalfunctionValue;		// ͬ��ʱ����ϲ�ֵ		2	BIN	��λ���롣Ĭ��300�롣
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

	// ���нӿ�
    void GetOperationTimeParam(OPRATION_TIME&);		// �����Ӫʱ�����
	void GetCommunication(COMMUNICATION&);			// ���ͨѶ����
	
    static CAfcDeviceCommonParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[3];	//�ֶ���Ϣ

    // ˽�г�Ա����
    OPRATION_TIME	m_operation_param;		// ��Ӫʱ�����
	COMMUNICATION	m_communication_param;		// ��ΧͨѶ����

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcDeviceCommonParam(void);
    ~CAfcDeviceCommonParam();
	CAfcDeviceCommonParam(const CAfcDeviceCommonParam&);
	CAfcDeviceCommonParam& operator=(const CAfcDeviceCommonParam&);

    static  CAfcDeviceCommonParam theInstance;
};
