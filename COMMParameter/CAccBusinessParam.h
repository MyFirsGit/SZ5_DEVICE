#pragma once
#include "CAccParamMaster.h"

// CAccBusinessParam��Ψһ�ӿ�
#define theACC_BUSINESS CAccBusinessParam::GetInstance()        

/**
 @brief   ACC��Ӫ������
*/

class PARAMETERCOMMAND_DECLSPEC CAccBusinessParam :public CAccParamMaster
{
public: 
    static CAccBusinessParam&  GetInstance()
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

	CAccBusinessParam(void);
    ~CAccBusinessParam();
	CAccBusinessParam(const CAccBusinessParam&);
	CAccBusinessParam& operator=(const CAccBusinessParam&);

    static  CAccBusinessParam theInstance;
};
