#pragma once
#include "CAccParamMaster.h"

// CAccPriceLevelParam的唯一接口
#define theACC_PRICELEVEL CAccPriceLevelParam::GetInstance()        

/**
@brief   ACC票价级别表参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccPriceLevelParam :public CAccParamMaster
{
public: 

	// ACC票价级别
	typedef struct _price_Level {
		BYTE priceLevel;				        //票价级别
		DWORD price;							//票价

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
	vector<PRICE_LEVEL>  m_vec_priceLecvel_param;			                                            // 所有ACC票价级别
public: 
	const int GetPriceLevelParam(vector<DWORD>&);										            // 获取所有ACC票价级别
	// 私有成员变量
private:

	SECTION_INFO m_section[1];	//分段信息

	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccPriceLevelParam(void);
	~CAccPriceLevelParam();
	CAccPriceLevelParam(const CAccPriceLevelParam&);
	CAccPriceLevelParam& operator=(const CAccPriceLevelParam&);

	static  CAccPriceLevelParam theInstance;
};
