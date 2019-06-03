#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccServiceFeeParam��Ψһ�ӿ�
#define theACC_SERVICEFEE CAccServiceFeeParam::GetInstance()        

/**
@brief   �������������Ѳ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccServiceFeeParam :public CAccParamMaster
{
public: 

	// �������������Ѽ�¼��
	typedef struct _service_Fee {
		BYTE    processingType;			// �����������
		WORD	serviceFee;				// �����ѽ��
		_service_Fee(){
			processingType =0;					
			serviceFee =0;					    		   	
		}
	}SERVICE_FEE;



	// ���нӿ�
	const vector<SERVICE_FEE>&  GetServiceFeeListParam();									// ��ȡ���������ѽ��
	int  GetServiceFeeInfo(DWORD processingType,WORD& serviceFee);							// ��ȡָ�������������������ѽ��
	static CAccServiceFeeParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<SERVICE_FEE>  m_vec_serviceFeeList_param;			// ���г�Ʊ����
private:

	SECTION_INFO m_section[1];	//�ֶ���Ϣ

	// ˽�г�Ա����


	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccServiceFeeParam(void);
	~CAccServiceFeeParam();
	CAccServiceFeeParam(const CAccServiceFeeParam&);
	CAccServiceFeeParam& operator=(const CAccServiceFeeParam&);

	static  CAccServiceFeeParam theInstance;
};

