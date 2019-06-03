#pragma once
#include "CAccParamMaster.h"

// CAccPriceLevelParam��Ψһ�ӿ�
#define theACC_PRICELEVEL CAccPriceLevelParam::GetInstance()        

/**
@brief   ACCƱ�ۼ���������
*/

class PARAMETERCOMMAND_DECLSPEC CAccPriceLevelParam :public CAccParamMaster
{
public: 

	// ACCƱ�ۼ���
	typedef struct _price_Level {
		BYTE priceLevel;				        //Ʊ�ۼ���
		DWORD price;							//Ʊ��

		_price_Level(){
			priceLevel=0;				
			price=0;			
		}
	}PRICE_LEVEL;


	static CAccPriceLevelParam&  GetInstance()
	{
		return theInstance;
	}

protected:
	vector<PRICE_LEVEL>  m_vec_priceLecvel_param;			                                            // ����ACCƱ�ۼ���
public: 
	const int GetPriceLevelParam(vector<DWORD>&);										            // ��ȡ����ACCƱ�ۼ���
	// ˽�г�Ա����
private:

	SECTION_INFO m_section[1];	//�ֶ���Ϣ

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccPriceLevelParam(void);
	~CAccPriceLevelParam();
	CAccPriceLevelParam(const CAccPriceLevelParam&);
	CAccPriceLevelParam& operator=(const CAccPriceLevelParam&);

	static  CAccPriceLevelParam theInstance;
};
