#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcOperatorCodeTableParam��Ψһ�ӿ�
#define theAFC_OPERATOR_CODE CAfcOperatorCodeTableParam::GetInstance()        

/**
 @brief   ��Ӫ�̴���ӳ������
*/

class PARAMETERCOMMAND_DECLSPEC CAfcOperatorCodeTableParam :public CAfcParamMaster
{
public: 

	// ���нӿ�
    //void GetOperationTimeParam(OPRATION_TIME&);		// ��Ӫ�̴���ӳ���
	
    static CAfcOperatorCodeTableParam&  GetInstance()
    {
        return theInstance;
    }

private:

	//SECTION_INFO m_section;	//�ֶ���Ϣ

    // ˽�г�Ա����
    //OPRATION_TIME	m_operation_param;			// ��Ӫʱ�����
	
    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcOperatorCodeTableParam(void);
    ~CAfcOperatorCodeTableParam();
	CAfcOperatorCodeTableParam(const CAfcOperatorCodeTableParam&);
	CAfcOperatorCodeTableParam& operator=(const CAfcOperatorCodeTableParam&);

    static  CAfcOperatorCodeTableParam theInstance;
};
