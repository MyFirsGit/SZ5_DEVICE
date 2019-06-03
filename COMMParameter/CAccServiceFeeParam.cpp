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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccServiceFeeParam::CAccServiceFeeParam():CAccParamMaster()
{
	m_vec_serviceFeeList_param.clear();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccServiceFeeParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0102);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccServiceFeeParam::~CAccServiceFeeParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccServiceFeeParam::ParseBody(LPBYTE lpData)
{
	try{
		// 第1段：车票信息       
		// 偏移量设置
		m_vec_serviceFeeList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			DWORD serviceFeeList_len = *lpData;
			lpData ++;      
			for(int i = 0;i<serviceFeeList_len;i++){
				SERVICE_FEE tmpserviceFeeInfo;
				//行政处理类别
				tmpserviceFeeInfo.processingType = *lpData;	
				lpData ++;
				//手续费金额
				tmpserviceFeeInfo.serviceFee = BCD2int((char*)lpData, 2);
				lpData += 2;		
				m_vec_serviceFeeList_param.push_back(tmpserviceFeeInfo);
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
bool CAccServiceFeeParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      行政处理类别
@param      none

@retval     vector<FARE_TYPE_LIST>   行政处理类别向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccServiceFeeParam::SERVICE_FEE>&  CAccServiceFeeParam::GetServiceFeeListParam()
{
	return m_vec_serviceFeeList_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定行政处理类别

@param      (i)DWORD ticketTypeCode   车票类型编码
@param  	(o)TICKET_TYPE_LIST& ticket_type_Param      车票类型信息  

@retval     int  0 成功 		 1 失败

@exception  无
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

