#include "StdAfx.h"
#include "CAfcDeviceCommonParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcDeviceCommonParam CAfcDeviceCommonParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcDeviceCommonParam::CAfcDeviceCommonParam(void):CAfcParamMaster()
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
CAfcDeviceCommonParam::~CAfcDeviceCommonParam(void)
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
void CAfcDeviceCommonParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1005);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceCommonParam::ParseBody(LPBYTE lpData)
{
    try{
		// 偏移量设置	
		lpData += m_section[0].startPosition;
        // 分段1：线路设定参数
		if (m_section[0].recordCount != 0) { 
			// 预留字段
			lpData +=16;	
        }
		// 分段2：运营时间参数
		if (m_section[1].recordCount != 0) { 	
			// 24小时运营时票卡延长使用时间
			m_operation_param.delayTime24H = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));			
			lpData += 4;
			// SLE连接检测时间间隔
			m_operation_param.connectCheckInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 预留
			lpData += 2;
        }
		// 分段3：通讯参数 
		if (m_section[2].recordCount != 0) { 	
			// 交易数据传送间隔		2	BIN	单位分钟，默认为1分钟
			m_communication_param.txnInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 审计时间间隔			2	BIN	单位分钟，默认值为15分钟
			m_communication_param.auditInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 业务数据传送间隔		2	BIN	单位分钟，默认值为15分钟
			m_communication_param.businessInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			// 钱票箱数据产生间隔	2	BIN	单位分钟，默认值为30分钟
			m_communication_param.magazineInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			// 时钟同步时间间隔		2	BIN	单位分钟，开机后两次时钟同步间隔，默认15分钟
			m_communication_param.ntpInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 同步时间报警差值		2	BIN	单位：秒。默认60秒。
			m_communication_param.ntpAlarmValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// 同步时间故障差值		2	BIN	单位：秒。默认300秒。
			m_communication_param.ntpMalfunctionValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// 预留
			lpData += 2;
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
void CAfcDeviceCommonParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAfcDeviceCommonParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得运营时间参数

@param      (o)OPRATION_TIME& operationTm   运营时间参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceCommonParam::GetOperationTimeParam(OPRATION_TIME& operationTm)
{
	operationTm = m_operation_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得通讯参数

@param      (o)COMMUNICATION& communicationParam 通讯参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceCommonParam::GetCommunication(COMMUNICATION& communicationParam)
{
	communicationParam = m_communication_param;
}