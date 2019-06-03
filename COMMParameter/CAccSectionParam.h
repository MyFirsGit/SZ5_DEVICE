#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccSectionParam��Ψһ�ӿ�
#define theACC_SECTION CAccSectionParam::GetInstance()        

/**
 @brief   ACC����Ʊ�۱������
*/

class PARAMETERCOMMAND_DECLSPEC CAccSectionParam :public CAccParamMaster
{
public: 

    static CAccSectionParam&  GetInstance()
    {
        return theInstance;
    }

protected:

private:

//	SECTION_INFO m_section[4];	//�ֶ���Ϣ

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���


	CAccSectionParam(void);
    ~CAccSectionParam();
	CAccSectionParam(const CAccSectionParam&);
	CAccSectionParam& operator=(const CAccSectionParam&);

    static  CAccSectionParam theInstance;
};
