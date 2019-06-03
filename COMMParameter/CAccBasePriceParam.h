#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccBasePriceParam��Ψһ�ӿ�
#define theACC_BASEPRICE CAccBasePriceParam::GetInstance()        

/**
 @brief   ACC����Ʊ�۱������
*/

class PARAMETERCOMMAND_DECLSPEC CAccBasePriceParam :public CAccParamMaster
{
public: 

    static CAccBasePriceParam&  GetInstance()
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


	CAccBasePriceParam(void);
    ~CAccBasePriceParam();
	CAccBasePriceParam(const CAccBasePriceParam&);
	CAccBasePriceParam& operator=(const CAccBasePriceParam&);

    static  CAccBasePriceParam theInstance;
};
