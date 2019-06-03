#include "StdAfx.h"
#include "CAccPriceLevelParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccPriceLevelParam CAccPriceLevelParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccPriceLevelParam::CAccPriceLevelParam():CAccParamMaster()
{
	m_vec_priceLecvel_param.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccPriceLevelParam::~CAccPriceLevelParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAccPriceLevelParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0602);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccPriceLevelParam::ParseBody(LPBYTE lpData)
{
	try{

		// ��1�Σ�Ʊ�ۼ���       
		// ƫ��������
		m_vec_priceLecvel_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			WORD fareTypeInfoList_len;
			memcpy(&fareTypeInfoList_len,lpData,2);
			lpData += 2;      
			for(int i = 0;i<fareTypeInfoList_len;i++){
				PRICE_LEVEL tmpFareTypeInfo;	

				//Ʊ�ۼ���
				tmpFareTypeInfo.priceLevel =  BCD2int((char*)lpData,2);
				lpData += 2;  
				//Ʊ��
				memcpy(&tmpFareTypeInfo.price,lpData,4);
				lpData +=4;					
				m_vec_priceLecvel_param.push_back(tmpFareTypeInfo);
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
void CAccPriceLevelParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAccPriceLevelParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���г�Ʊ���Ͳ���

@param      
@retval     intl   0:��ȷ  1������ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccPriceLevelParam::GetPriceLevelParam(vector<DWORD>& vecFareList)
{
	vector<CAccPriceLevelParam::PRICE_LEVEL>::iterator  it;
	vecFareList.clear();
	for(it=m_vec_priceLecvel_param.begin(); it!=m_vec_priceLecvel_param.end(); it++){
		vecFareList.push_back(it->price);
	}
	return 0;
}