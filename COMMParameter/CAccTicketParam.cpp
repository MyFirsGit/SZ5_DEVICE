#include "StdAfx.h"
#include "CAccTicketParam.h"
#include "LogException.h"
#include "CommonDefine.h"
#include "ticketname.h"
#include "BomInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccTicketParam CAccTicketParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccTicketParam::CAccTicketParam():CAccParamMaster()
{
	m_vec_ticketsTypeList_param.clear();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccTicketParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0401);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccTicketParam::~CAccTicketParam(void)
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
void CAccTicketParam::ParseBody(LPBYTE lpData)
{
	try{
		// 第1段：车票信息       
		// 偏移量设置
		m_vec_ticketsTypeList_param.clear();
		m_vec_ticketTypeList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			WORD fareTypeInfoList_len;
			memcpy(&fareTypeInfoList_len,lpData,2);
			lpData += 2;      
			for(int i = 0;i<fareTypeInfoList_len;i++){
				TICKET_TYPE tmpFareTypeInfo;
				//车票主类型代码
				tmpFareTypeInfo.ticketTypebaseCode = BCD2int((char*)lpData,1);	
				lpData ++;
				//车票子类型代码
				tmpFareTypeInfo.ticketTypeCode = BCD2int((char*)lpData,1);
				tmpFareTypeInfo.ticketTypeName_cn = theTICKET_NAME.GetTicketNameCn(tmpFareTypeInfo.ticketTypeCode);
				tmpFareTypeInfo.ticketTypeName_en = theTICKET_NAME.GetTicketNameEn(tmpFareTypeInfo.ticketTypeCode);
				lpData ++;
				//车票余额/次上限
				memcpy(&tmpFareTypeInfo.MaxBalance,lpData,4);
				lpData +=4;
				//是否允许充值
				tmpFareTypeInfo.chargeable = BCD2int((char*)lpData,1);
				lpData ++;
				//充值限制
				memcpy(&tmpFareTypeInfo.maxChargeSize,lpData,4);
				lpData +=4;	
				//更新时收费方式
				tmpFareTypeInfo.payMathodOfAdjuest = BCD2int((char*)lpData,1);
				lpData ++;
				//是否允许退款
				tmpFareTypeInfo.refoundable = BCD2int((char*)lpData,1);
				lpData ++;
				//退款时使用次数限制
				tmpFareTypeInfo.sendBackablecLimitCount = *lpData;
				lpData ++;		
				//是否限制日乘坐次数
				tmpFareTypeInfo.hasLimitOfOneday = BCD2int((char*)lpData,1);
				lpData ++; 
				//日乘坐次数限制
				tmpFareTypeInfo.maxMountOfOneDay = *lpData;//BCD2int((char*)lpData,2);
				lpData ++;  
				//是否允许预赋值
				tmpFareTypeInfo.prechargeable = BCD2int((char*)lpData,1);
				lpData ++;
				//有效期	
				memcpy(&tmpFareTypeInfo.vilidateData,lpData,2);
				lpData += 2; 
				//可延长时间	
				memcpy(&tmpFareTypeInfo.extendTime,lpData,2);
				lpData += 2; 
				//押金
				memcpy(&tmpFareTypeInfo.deposit,lpData,4);
				lpData +=4;		
				//使用前是否需激活
				tmpFareTypeInfo.needActive = BCD2int((char*)lpData,1);
				lpData ++;	
				//是否检查黑名单
				tmpFareTypeInfo.checkBlackList = BCD2int((char*)lpData,1);
				lpData ++;	
				//是否检查余额/余次
				tmpFareTypeInfo.checkBalance = BCD2int((char*)lpData,1);
				lpData ++;	
				//是否检查有效期
				tmpFareTypeInfo.checkValidate = BCD2int((char*)lpData,1);
				lpData ++;	
				//是否检查进出次序
				tmpFareTypeInfo.checkSequence = BCD2int((char*)lpData,1);
				lpData ++;		
				//是否检查超时
				tmpFareTypeInfo.checkOvertime = BCD2int((char*)lpData,1);
				lpData ++;		
				//是否检查超乘
				tmpFareTypeInfo.checkOverJourney = BCD2int((char*)lpData,1);		
				lpData ++;		
				//是否限制线路
				tmpFareTypeInfo.checkLimitLine = BCD2int((char*)lpData,1);
				lpData ++;	
				//是否只允许本站进出
				tmpFareTypeInfo.checkOnlyThis = BCD2int((char*)lpData,1);	
				lpData ++;	
				//是否限制站内停留时间
				tmpFareTypeInfo.checkStickTime = BCD2int((char*)lpData,1);	
				lpData ++;	
				//站内停留时间上限
				memcpy(&tmpFareTypeInfo.maxStickTime,lpData,2);	
				lpData +=2;	
				//可充值的设备
				memcpy(&tmpFareTypeInfo.chargeDevice,lpData,1);	
				lpData ++;
				//可使用的设备
				memcpy(&tmpFareTypeInfo.UseableDevice,lpData,1);	
				lpData ++;
				//本站进出控制标记
				tmpFareTypeInfo.checkInFlag = BCD2int((char*)lpData,1);	
				lpData ++;	
				//尾乘优惠
				tmpFareTypeInfo.favourable =  BCD2int((char*)lpData,1);
				lpData ++;		
				//回收标识
				tmpFareTypeInfo.recorverFlag = BCD2int((char*)lpData,1);
				lpData ++;	
				//计费方式1BCD计费类型
				tmpFareTypeInfo.chargeType =  BCD2int((char*)lpData,1);
				lpData += 1;
				//透支标识
				tmpFareTypeInfo.overdraftFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//储值票最小余额检查
				tmpFareTypeInfo.checkMinBalanceFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//储值票最大余额检查
				tmpFareTypeInfo.checkMaxBalanceFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//进站时间检查
				memcpy(tmpFareTypeInfo.checkEnterTime, lpData,3);
				lpData += 3;
				//退票手续费
				memcpy(&tmpFareTypeInfo.refoundFee,lpData,4);
				lpData += 4;
				//换票手续费
				memcpy(&tmpFareTypeInfo.exchangeFee,lpData,4);
				lpData += 4;
				//回收类票丢票罚款额
				memcpy(&tmpFareTypeInfo.amercementOfloseCard,lpData,4);
				lpData += 4;
				//回收类票人为损坏罚款额
				memcpy(&tmpFareTypeInfo.amercementOfBrokenCard,lpData,4);
				lpData += 4;	
				//超时罚款额
				memcpy(&tmpFareTypeInfo.overTimeFee,lpData,4);
				lpData += 4;
				m_vec_ticketsTypeList_param.push_back(tmpFareTypeInfo);
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
void CAccTicketParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAccTicketParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      车票参数

@param      none

@retval     vector<FARE_TYPE_LIST>   车票参数向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccTicketParam::TICKET_TYPE>&  CAccTicketParam::GetTicketTypeListParam()
{
	return m_vec_ticketsTypeList_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定车票参数

@param      (i)WORD ticketTypeCode   车票类型编码(低字节有效)
@param  	(o)TICKET_TYPE_LIST& ticket_type_Param      车票类型信息  

@retval     int  0 成功 		 1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketInfo(WORD ticketTypeCode,CAccTicketParam::TICKET_TYPE& ticket_type_Param)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			ticket_type_Param = *it;
			return 0;
		}
	}
	return 1;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型车票的有效日期

@param      (i)WORD ticketTypeCode			车票主类型编码(低字节有效)
@param  	(o)_DATE_TIME& validateDate     车票有效期  

@retval     int    -1   失败	
					0	永远无效
					1	一直有效
					2	售后/充值后一段日期有效
					3	首次使用后一段日期有效(激活首次使用)
					4	固定起、始有效期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketValidate(WORD ticketTypeCode,_DATE_TIME& validateDate)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			_DATE date;
			date.wYear = validateDate.wYear;
			date.biMonth = validateDate.biMonth;
			date.biDay = validateDate.biDay;
			_DATE day = GetDayAfter(date,it->vilidateData);	
			validateDate.wYear = day.wYear;
			validateDate.biMonth = day.biMonth;
			validateDate.biDay = day.biDay;
			validateDate.biHour = 23;
			validateDate.biMinute = 59;
			validateDate.biSecond = 59;			
		}
	}
	return -1;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型车票的押金信息

@param      (i)DWORD ticketTypeCode   车票类型编码(低字节有效)
@param  	(o)DWORD* deposit        车票押金

@retval     int  0 成功         1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketDeposit(WORD ticketTypeCode,LONG& deposit)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			deposit = it->deposit;
			return 0;
		}
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型车票的中文名称及英文名称

@param      (i)WORD ticketTypeCode             车票类型编码(低字节有效)
@param      (o)CString& ticketTypeName_cn       车票类型中文名
@param      (o)CString& ticketTypeName_en		车票类型英文名

@retval     int  0 成功         1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString  CAccTicketParam::GetProductName(WORD ticketTypeCode,LANGUAGETYPE_T language)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			if (language == CHINESE)
			{
				return it->ticketTypeName_cn;
			}
			else
				return it->ticketTypeName_en;
		}
	}
	return _T("");
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      根据车票子类型获取车票的主类型

@param      (i)BYTE ticketSubTypeCode     车票子类型编码

@retval     BYTE  车票主类型编码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const BYTE  CAccTicketParam::GetTicketMainType(BYTE ticketSubTypeCode)
{
	BYTE byTicketMainType = 0;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == ticketSubTypeCode){
			byTicketMainType = it->ticketTypebaseCode;
			break;
		}
	}
	return byTicketMainType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是不回收票卡

@param      (i)BYTE ticketSubTypeCode  票卡子类型

@retval     true:需要回收 false:不需要回收

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAccTicketParam::IsNotCollectedTicket(BYTE ticketSubTypeCode)
{
	bool iscollect = false;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == ticketSubTypeCode){
			// 不回收票卡
			if(it->recorverFlag == 0x01){
				iscollect = true;
				break;
			}			
		}
	}
	return iscollect;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型的车票的计费类型

@param      (i)WORD ticketTypeCode              车票类型编码(低字节有效)
@param      (o)TicketType_t& ticket_categry     车票类型

@retval     int  0 成功         1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE CAccTicketParam::GetTicketCategry(WORD ticketTypeCode)
{
	ProductCategory_t ticket_categry;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			switch(it->chargeType){			
			case 01:
				ticket_categry = PERIOD;
				break;
			case 02:
				ticket_categry = PURSE;
				break;
			case 03:
				ticket_categry = MULTIRIDE;
				break;
			default:
				return -1;
				break;
			}
		}
	}
	return ticket_categry;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型的车票的计费类型

@param      (i)WORD ticketTypeCode              车票类型编码(低字节有效)
@param      (o)TicketType_t& ticket_categry     车票类型

@retval     int  0 成功         1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const int CAccTicketParam::GetTicketCategry(WORD ticketTypeCode,ProductCategory_t& ticket_categry)
{
	if(theBOM_INFO.IsUseConfigTicketCategory())
	{
		int ticketCategory = theBOM_INFO.GetConfigTicketCategory(ticketTypeCode);
		if(ticketCategory>0)
		{
			ticket_categry = (ProductCategory_t)ticketCategory;
			return 0;
		}
	}

	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			switch(it->chargeType){			
			case 01:
				ticket_categry = PERIOD;
				break;
			case 02:
				ticket_categry = PURSE;
				break;
			case 03:
				ticket_categry = MULTIRIDE;
				break;
			default:
				return -1;
				break;
			}
			return 0;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定类型车票的中文名称及英文名称

@param      (i)WORD ticketTypeCode             车票类型编码(低字节有效)
@param      (o)CString& ticketTypeName_cn       车票类型中文名
@param      (o)CString& ticketTypeName_en		车票类型英文名

@retval     int  0 成功         1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketTypeName(WORD ticketTypeCode,CString& ticketTypeName_cn,CString& ticketTypeName_en)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			ticketTypeName_cn = it->ticketTypeName_cn;
			ticketTypeName_en = it->ticketTypeName_en;
			return 0;
		}
	}
	return 1;
}
