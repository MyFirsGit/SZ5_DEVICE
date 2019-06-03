#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccOtherCityCard��Ψһ�ӿ�
#define theACC_OTHERCITYCARD CAccOtherCityCard::GetInstance()        

/**
@brief   ACC��ؿ�/���д�����ղ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccOtherCityCard :public CAccParamMaster
{
public: 

	static CAccOtherCityCard&  GetInstance()
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

	CAccOtherCityCard(void);
	~CAccOtherCityCard();
	CAccOtherCityCard(const CAccOtherCityCard&);
	CAccOtherCityCard& operator=(const CAccOtherCityCard&);

	static  CAccOtherCityCard theInstance;
};
