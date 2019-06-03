#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccPriceAdjuest��Ψһ�ӿ�
#define theACC_PRICEADJUEST CAccPriceAdjuest::GetInstance()        

/**
@brief   ACCƱ�۵������Ա������
*/

class PARAMETERCOMMAND_DECLSPEC CAccPriceAdjuest :public CAccParamMaster
{
public:

	static CAccPriceAdjuest&  GetInstance()
	{
		return theInstance;
	}

protected:

private:

	//SECTION_INFO m_section[3];                  	//�ֶ���Ϣ

	// ˽�г�Ա����


	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccPriceAdjuest(void);
	~CAccPriceAdjuest();
	CAccPriceAdjuest(const CAccPriceAdjuest&);
	CAccPriceAdjuest& operator=(const CAccPriceAdjuest&);

	static  CAccPriceAdjuest theInstance;
};


