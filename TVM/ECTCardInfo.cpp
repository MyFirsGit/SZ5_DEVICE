#include "StdAfx.h"
#include "ECTCardInfo.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CECTCard::CECTCard(void)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CECTCard::~CECTCard(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CECTCard::Initialize()
{
	//// 卡片基本信息结构体
	//memset(cardData.TicketPhyID,0,7);				// 物理卡号	    7HEX	车票物理卡号
	//memset(cardData.TicketLogicalID,0,10);			// 逻辑卡号	    10HEX	车票逻辑编号
	//cardData.CityCode = 0;							// 城市代码		2BCD	
	//cardData.IndustryCode = 0;						// 行业代码		2BCD			
	//cardData.TicketFamily = 0;						// 车票大分类	1HEX	
	//cardData.TicketType = 0;						// 卡应用类型
	//cardData.TicketStatus = 0;						// 启用标志
	//cardData.Deposit = 0;							// 押金			2BCD    单位:分        

	//// 钱包信息结构体
	//purseData.Balance = 0;							// 余额/余次
	//purseData.TransactionTimes = 0;					// 钱包累计交易次数

	//// 充值信息结构体
	//chargeData.ChargeTotalValue = 0;                // 累计充值额 单位:分
	//chargeData.MaxChargeValue = 0;					// 允许最大余额,CPU票卡该字段为0		
	//chargeData.ChargeDeviceID = 0;					// 充值设备编号(线路、车站、设备类型、设备编号各1字节) 

	//// 卡内旅程数据的结构体
	//journeyData.JourneyStatus = 0;					// 旅程状态		41:表示进站   81:表示出站	
	//journeyData.EnterStation = 0;					// 进站站点				线路（1HEX）+车站（1HEX）
	//journeyData.EnterDeviceID = 0;					// 进站设备             设备类型（1HEX）+ 站内编号（1HEX）
	//journeyData.EnterTransValue = 0;				// 进站交易金额	2HEX	单位：分		
	//journeyData.ExitStation = 0;					// 出站站点				线路（1HEX）+车站（1HEX）
	//journeyData.ExitDeviceID = 0;					// 出站设备				设备类型（1HEX）+ 站内编号（1HEX）
	//journeyData.ExitTransValue = 0;					// 出站交易金额	2HEX	单位：分
	//journeyData.BackupFlag = 0;						// 备份标识	1	HEX	55：表示允许出站； 66：表示允许进站；	71：表示付费区更新；72：表示超时更新

	//// 优惠信息结构体     
	//discountData.IndustryCode = 0;					// 行业标识
	//discountData.TicketType = 0;					// 卡应用类型

	//// 历史记录
	//historyData.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      取得ECT票种信息

@param      (i)BYTE* cardInfo 读卡器返回的票卡信息

@retval     bool true:成功 false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CECTCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
	//	LPBYTE pCardinfo = cardInfo;
	//	// 卡片基本信息结构体
	//	// 物理卡号
	//	memcpy(cardData.TicketPhyID,(char*)pCardinfo,7); 
	//	pCardinfo += 7;

	//	// 发行流水号
	//	memcpy(cardData.TicketLogicalID,(char*)pCardinfo,10);
	//	pCardinfo += 10;

	//	// 城市代码
	//	cardData.CityCode = BCD2int((char*)pCardinfo,2);
	//	pCardinfo += 2;
	//	
	//	// 行业代码
	//	cardData.IndustryCode = BCD2int((char*)pCardinfo,2);
	//	pCardinfo += 2;
	//			
	//	// 车票大分类
	//	cardData.TicketFamily = *pCardinfo;  
	//	pCardinfo ++;

	//	// 车票产品种类
	//	cardData.TicketType = *pCardinfo;
	//	pCardinfo += 1;

	//	// 启用状态
	//	cardData.TicketStatus = *pCardinfo;				
	//	pCardinfo ++;

	//	// 押金
	//	cardData.Deposit = MAKEWORD(*pCardinfo,*(pCardinfo+1));				
	//	pCardinfo += 2;
	//	
	//	// 发行日期
	//	cardData.InitDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.InitDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.InitDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// 有效日期
	//	cardData.ValiditDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.ValiditDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.ValiditDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// 启用日期
	//	cardData.StartDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.StartDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.StartDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	
	//	// 预留
	//	pCardinfo += 6;

	//	// 预留
	//	pCardinfo += 16;
	//	
	//	// 钱包信息
	//	// 卡余额、余次
	//	purseData.Balance = MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;

	//	// 钱包累计交易次数
	//	purseData.TransactionTimes = MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;
	//	
	//	//充值时间
	//	purseData.chargeDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	purseData.chargeDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	purseData.chargeDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	//充值原额
	//	purseData.chargeAmout =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;
	//	//本次充值
	//	purseData.currentChargeAmout = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
	//	pCardinfo+=2;
	//	// 预留
	//	pCardinfo += 2;

	//	// 充值信息
	//	// 累计充值额 单位:分
	//	chargeData.ChargeTotalValue  =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	            
	//	pCardinfo += 4;

	//	// 允许最大余额,CPU票卡该字段为0
	//	chargeData.MaxChargeValue  =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;

	//	// 充值设备编号(线路、车站、设备类型、设备编号各1字节) 
	//	chargeData.ChargeDeviceID  = BCD2int((char*)pCardinfo,4);	
	//	pCardinfo += 4;

	//	// 预留
	//	pCardinfo += 8;

	//	// 旅程数据
	//	// 旅程状态		41:表示进站   81:表示出站
	//	journeyData.JourneyStatus = *pCardinfo;		
	//	pCardinfo ++;

	//	// 进站交易时间	5BCD	YYMMDDHHMM   秒为0
	//	journeyData.EnterTime.wYear = 2000 +  BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biSecond = 0;
	//	
	//	// 进站站点				线路（1HEX）+车站（1HEX）
	//	journeyData.EnterStation = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// 进站设备             设备类型（1HEX）+ 站内编号（1HEX）
	//	journeyData.EnterDeviceID = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// 进站交易金额	2HEX	单位：分
	//	journeyData.EnterTransValue = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// 预留
	//	pCardinfo ++;

	//	// 出站交易时间	5BCD	YYMMDDHHMM   秒为0
	//	journeyData.ExitTime.wYear = 2000 +  BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biHour = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biSecond = 0;
	//	
	//	// 出站站点				线路（1HEX）+车站（1HEX）
	//	journeyData.ExitStation = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// 出站设备				设备类型（1HEX）+ 站内编号（1HEX）
	//	journeyData.ExitDeviceID = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// 出站交易金额	2HEX	单位：分
	//	journeyData.ExitTransValue = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// 备份标识	1	HEX	55：表示允许出站； 66：表示允许进站；
	//	journeyData.BackupFlag =  *pCardinfo;						
	//	pCardinfo ++;

	//	// 预留
	//	pCardinfo += 8;

	//	// 优惠信息数据
	//	// 行业标识
	//	discountData.IndustryCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// 卡应用类型
	//	discountData.TicketType = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// 启用日期	    4BCD	YYYYMMDD
	//	discountData.StartDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	discountData.StartDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	discountData.StartDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// 结束日期		4BCD	YYYYMMDD
	//	discountData.EndDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	discountData.EndDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	discountData.EndDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得ECT历史交易记录信息

@param      (i)BYTE* cardInfo 读卡器返回的票卡信息

@retval     bool true:成功 false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CECTCard::GetHistoryInfo(BYTE TicketPhyType,int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
	//	LPBYTE pCardinfo = historyInfo;
	//	STR_CARD_HISTORY_DATA history;
	//	historyData.clear();
	//	 
	//	//一卡通M1卡
	//	if (TicketPhyType == MIFARE1K){
	//		for(int i=0; i<count; i++){
	//			// 交易时间 YYMMDDHHMM   秒为0
	//			history.TransTime.wYear = 0;	
	//			pCardinfo ++;
	//			history.TransTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biDay = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biHour = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biSecond = 0;
	//			pCardinfo += 4;			

	//			// 交易金额?????????????????????????????????????????????????????????????
	//			history.TransValue  = ComMakeLong(0, *(pCardinfo + 2), *(pCardinfo + 1), *pCardinfo);             
	//			pCardinfo += 3;
	//			
	//			// 交易类型
	//			BYTE type= *pCardinfo;
	//			switch (type){
	//			case LOACLPAK_CONSUMER:
	//				history.TransType = "消费";
	//				break;
	//			case UPDATE_TRAC:
	//				history.TransType = "补票";
	//				break;
	//			case REFOUND_TRAC:
	//				history.TransType = "退卡";
	//				break;
	//			case LUCK_TRAC:
	//				history.TransType = "锁卡";
	//				break;
	//			case ENTRE_TRAC:
	//				history.TransType = "进站";
	//				break;
	//			case OUT_TRAC:
	//				history.TransType = "出站";
	//				break;
	//			case CHARGE_TRAC:
	//				history.TransType = "充值";
	//				break;
	//			default :
	//				history.TransType ="--"; 
	//				break;
	//			}				
	//			pCardinfo ++;
	//			// 交易站点	
	//			history.TransStation = BCD2int((char*)pCardinfo,2);		
	//			pCardinfo += 4;
	//			historyData.push_back(history);
	//		}
	//	} 
	//	//一卡通CPU卡
	//	else if (TicketPhyType == CPU_TICKET){
	//		for(int i=0; i<count; i++){	
	//			pCardinfo += 5;
	//			// 充值金额
	//			history.TransValue  = ComMakeLong(*pCardinfo, *(pCardinfo + 1), *(pCardinfo + 2), *(pCardinfo + 3));             
	//			pCardinfo += 4;
	//			
	//			// 交易类型
	//			int type= *pCardinfo;
	//			switch (type){
	//			case MONTHPAK_STROE:
	//				history.TransType = "月票钱包 圈存";
	//				break;
	//			case PUBPAK_STROE:
	//				history.TransType = "公共钱包 圈存";
	//				break;
	//			case MONTHPAK_CONSUMER:
	//				history.TransType = "月票钱包 消费";
	//				break;
	//			case PUBPAK_CONSUMER:
	//				history.TransType = "公共钱包 消费";
	//				break;
	//			case COM_CONSUMER:
	//				history.TransType = "复合消费";
	//				break;
	//			default :
	//				history.TransType ="--"; 
	//				break;
	//			}
	//			// 交易站点 线路（1HEX）+车站（1HEX）
	//			history.TransStation = ComMakeWORD(*pCardinfo, *(pCardinfo + 1));		
	//			pCardinfo += 6;
	//		
	//			// 交易时间	  	YYMMDDHHMM   秒为0
	//			history.TransTime.wYear = BCD2int((char*)pCardinfo,2);	
	//			pCardinfo ++;
	//			history.TransTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biDay = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biHour = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biSecond = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;

	//			historyData.push_back(history);
	//		}
	//	}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}