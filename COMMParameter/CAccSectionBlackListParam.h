#pragma once
#include "CAccParamMaster.h"

// CAccSectionBlackListParam��Ψһ�ӿ�
#define theACC_SECTIONBLACKLIST CAccSectionBlackListParam::GetInstance()        

/**
 @brief   ACC���κ�����������
*/

class PARAMETERCOMMAND_DECLSPEC CAccSectionBlackListParam :public CAccParamMaster
{
public: 

    static CAccSectionBlackListParam&  GetInstance()
    {
        return theInstance;
    }

protected:
 // ˽�г�Ա����
   
private:

	//SECTION_INFO m_section[6];	//�ֶ���Ϣ

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccSectionBlackListParam(void);
    ~CAccSectionBlackListParam();
	CAccSectionBlackListParam(const CAccSectionBlackListParam&);
	CAccSectionBlackListParam& operator=(const CAccSectionBlackListParam&);

    static  CAccSectionBlackListParam theInstance;
};
