#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccDiscountParam��Ψһ�ӿ�
#define theACC_DISCOUNT CAccDiscountParam::GetInstance()        

/**
@brief   ACC�ۿ��ʲ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccDiscountParam :public CAccParamMaster
{
public: 

	static CAccDiscountParam&  GetInstance()
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

	CAccDiscountParam(void);
	~CAccDiscountParam();
	CAccDiscountParam(const CAccDiscountParam&);
	CAccDiscountParam& operator=(const CAccDiscountParam&);

	static  CAccDiscountParam theInstance;
};
