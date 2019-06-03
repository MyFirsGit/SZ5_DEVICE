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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccPriceLevelParam::CAccPriceLevelParam():CAccParamMaster()
{
	m_vec_priceLecvel_param.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccPriceLevelParam::~CAccPriceLevelParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccPriceLevelParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0602);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccPriceLevelParam::ParseBody(LPBYTE lpData)
{
	try{

		// 第1段：票价级别       
		// 偏移量设置
		m_vec_priceLecvel_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			WORD fareTypeInfoList_len;
			memcpy(&fareTypeInfoList_len,lpData,2);
			lpData += 2;      
			for(int i = 0;i<fareTypeInfoList_len;i++){
				PRICE_LEVEL tmpFareTypeInfo;	

				//票价级别
				tmpFareTypeInfo.priceLevel =  BCD2int((char*)lpData,2);
				lpData += 2;  
				//票价
				memcpy(&tmpFareTypeInfo.price,lpData,4);
				lpData +=4;					
				m_vec_priceLecvel_param.push_back(tmpFareTypeInfo);
			}
		}

	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出分段信息

@param      (i)LPBYTE lpData       文件内容

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
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文结构调整部分内容正确性

@param      (i)LPBYTE           电文信息

@retval     bool   true:正确  false：不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAccPriceLevelParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取所有车票类型参数

@param      
@retval     intl   0:正确  1：不正确

@exception  无
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