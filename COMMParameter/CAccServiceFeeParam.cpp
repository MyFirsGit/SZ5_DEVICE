#include "StdAfx.h"
#include "CAccServiceFeeParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccServiceFeeParam CAccServiceFeeParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccServiceFeeParam::CAccServiceFeeParam():CAccParamMaster()
{
	m_vec_serviceFeeList_param.clear();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAccServiceFeeParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0102);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccServiceFeeParam::~CAccServiceFeeParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccServiceFeeParam::ParseBody(LPBYTE lpData)
{
	try{
		// ��1�Σ���Ʊ��Ϣ       
		// ƫ��������
		m_vec_serviceFeeList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			DWORD serviceFeeList_len = *lpData;
			lpData ++;      
			for(int i = 0;i<serviceFeeList_len;i++){
				SERVICE_FEE tmpserviceFeeInfo;
				//�����������
				tmpserviceFeeInfo.processingType = *lpData;	
				lpData ++;
				//�����ѽ��
				tmpserviceFeeInfo.serviceFee = BCD2int((char*)lpData, 2);
				lpData += 2;		
				m_vec_serviceFeeList_param.push_back(tmpserviceFeeInfo);
			}
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccServiceFeeParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ľṹ��������������ȷ��

@param      (i)LPBYTE           ������Ϣ

@retval     bool   true:��ȷ  false������ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAccServiceFeeParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����������
@param      none

@retval     vector<FARE_TYPE_LIST>   �������������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccServiceFeeParam::SERVICE_FEE>&  CAccServiceFeeParam::GetServiceFeeListParam()
{
	return m_vec_serviceFeeList_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�������������

@param      (i)DWORD ticketTypeCode   ��Ʊ���ͱ���
@param  	(o)TICKET_TYPE_LIST& ticket_type_Param      ��Ʊ������Ϣ  

@retval     int  0 �ɹ� 		 1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int  CAccServiceFeeParam::GetServiceFeeInfo(DWORD processingType,WORD& serviceFee)
{
	vector<CAccServiceFeeParam::SERVICE_FEE>::iterator  it;

	for (it=m_vec_serviceFeeList_param.begin(); it!=m_vec_serviceFeeList_param.end(); it++){
		if(it->processingType == processingType){
			serviceFee = it->serviceFee;
			return 0;
		}
	}
	return 1;
}

