#include "StdAfx.h"
#include "CAfcBomOperationParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcBomOperationParam CAfcBomOperationParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcBomOperationParam::CAfcBomOperationParam(void):CAfcParamMaster()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcBomOperationParam::~CAfcBomOperationParam(void)
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
void CAfcBomOperationParam::GetParamFileName()
{
	//GetParamFileNameSub(AFC_PARAM_ID_1021);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::ParseBody(LPBYTE lpData)
{
    try{
		// 偏移量设置	
		lpData += m_section[0].startPosition;
        // 分段1：BOM票卡读写参数
		if (m_section[0].recordCount != 0) { 
			// 车票连续出错最大张数		
			m_tpu_param.maxWriteErrorCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 未写入完全的卡等待时间
            m_tpu_param.unwriteWaitTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 未写入完全的卡重试次数
			m_tpu_param.unwriteRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 预留
			lpData += 4;
        }
		// 分段2：BOM维修登录相关参数
		if (m_section[1].recordCount != 0) { 
			//密码输入时间
			m_maintain_param.pwdInputTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//登录尝试次数
			m_maintain_param.loginRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//无操作自动登出时间
			m_maintain_param.autoLogoffTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			//维修门未关闭报警时间
			m_maintain_param.doorUnlockAlarmTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 预留
			lpData += 4;
        }
		// 分段3：BOM票箱参数
		if (m_section[2].recordCount != 0) { 
			// 票箱将空张数阀值
			m_magazine_param.willEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 票箱已空张数阀值
			m_magazine_param.haveEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 票箱将满张数阀值
			m_magazine_param.willFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 票箱已满张数阀值
			m_magazine_param.haveFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 废票箱将满张数阀值
			m_magazine_param.willInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// 废票箱已满张数阀值
			m_magazine_param.haveInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// 预留
			lpData += 4;
		}
		// 分段4：BOM最近可查最小交易限制
		if (m_section[3].recordCount != 0) { 
			//可查询交易数量下限 
			m_txn_lookup_param.minCount = *lpData;			
			lpData ++;
			//可查询交易时间下限
			m_txn_lookup_param.minTime = *lpData;			
			lpData ++;					
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
void CAfcBomOperationParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAfcBomOperationParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得BOM票卡读写参数

@param      (o)TPU_PARAM& temParam   BOM票卡读写参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetTpuParam(TPU_PARAM& temParam)
{
	temParam = m_tpu_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得BOM维修登录相关参数

@param      (o)MAINTAIN_PARAM& temParam   BOM维修登录相关参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetMaintainParam(MAINTAIN_PARAM& temParam)
{
	temParam = m_maintain_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得BOM票箱参数

@param      (o)MAGAZINE_PARAM& operationTm   BOM票箱参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetMagazineParam(MAGAZINE_PARAM& temParam)			
{
	temParam = m_magazine_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获得BOM最近可查最小交易限制

@param      (o)TXN_LOOKUP_PARAM& temParam   BOM最近可查最小交易限制

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetTxnLookupParam(TXN_LOOKUP_PARAM& temParam)
{
	temParam = m_txn_lookup_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得自动登出等待时间参数

@param      none

@retval     none

@exception  int   自动登出等待时间参数
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcBomOperationParam::GetAutoLogoffTime()
{
	return m_maintain_param.autoLogoffTime;
}