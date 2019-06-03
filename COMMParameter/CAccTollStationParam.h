#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTollStationParam��Ψһ�ӿ�
#define theACC_TOLLSTATION CAccTollStationParam::GetInstance()        

/**
 @brief   ACC�Ʒ�վ�������
*/

class PARAMETERCOMMAND_DECLSPEC CAccTollStationParam :public CAccParamMaster
{
public: 

    static CAccTollStationParam&  GetInstance()
    {
        return theInstance;
    }

protected:

private:

	//SECTION_INFO m_section[2];	//�ֶ���Ϣ

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccTollStationParam(void);
    ~CAccTollStationParam();
	CAccTollStationParam(const CAccTollStationParam&);
	CAccTollStationParam& operator=(const CAccTollStationParam&);

    static  CAccTollStationParam theInstance;
};
