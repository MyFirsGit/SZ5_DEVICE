#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccServiceFeeParam的唯一接口
#define theACC_SERVICEFEE CAccServiceFeeParam::GetInstance()        

/**
@brief   行政处理手续费参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccServiceFeeParam :public CAccParamMaster
{
public: 

	// 行政处理手续费记录体
	typedef struct _service_Fee {
		BYTE    processingType;			// 行政处理类别
		WORD	serviceFee;				// 手续费金额
		_service_Fee(){
			processingType =0;					
			serviceFee =0;					    		   	
		}
	}SERVICE_FEE;



	// 共有接口
	const vector<SERVICE_FEE>&  GetServiceFeeListParam();									// 获取所有手续费金额
	int  GetServiceFeeInfo(DWORD processingType,WORD& serviceFee);							// 获取指定行政处理类别的手续费金额
	static CAccServiceFeeParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<SERVICE_FEE>  m_vec_serviceFeeList_param;			// 所有车票参数
private:

	SECTION_INFO m_section[1];	//分段信息

	// 私有成员变量


	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccServiceFeeParam(void);
	~CAccServiceFeeParam();
	CAccServiceFeeParam(const CAccServiceFeeParam&);
	CAccServiceFeeParam& operator=(const CAccServiceFeeParam&);

	static  CAccServiceFeeParam theInstance;
};

