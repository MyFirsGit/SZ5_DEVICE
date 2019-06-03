#include "StdAfx.h"
#include "ULCardInfo.h"

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
CULCard::CULCard(void)
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
CULCard::~CULCard(void)
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
void CULCard::Initialize()
{
	// 票卡基本信息结构体
	applicationData.PublisherCode = 0;					// 发卡方代码
	applicationData.CardPhysicalType = 0;				// 票卡物理类型
	memset(applicationData.CardPhysicalNumber,0,8);		// 票卡物理编号
	applicationData.CardLogicalNumber = 0;
	applicationData.CardAppMainType = 0;				// 卡应用主类型
	applicationData.CardAppSubType = 0;					// 卡应用子类型
	applicationData.TestCard = 0;						// 测试票标识
	applicationData.BatchNumber = 0;					// 票卡批次号
	applicationData.AppVersion = 0;						// 应用版本号
	applicationData.TicketStatus = 0;					// 车票状态
	applicationData.CardActived = 0;
	applicationData.SaleStation = 0;					// 发售车站
	applicationData.Deposit = 0;						// 押金
	applicationData.Cost = 0;							// 成本
	applicationData.SalePrice = 0;						// 售票金额
	applicationData.Balance = 0;						// 票内余额

	// 旅程信息结构体
	journeyData.JourneyStatus = 0;						// 旅程状态
	journeyData.EnterStation = 0;						// 进站站点
	memset(journeyData.EnterDeviceID,0,6);				// 进站设备 
	journeyData.ExitStation = 0;						// 出站站点
	memset(journeyData.ExitDeviceID,0,6);				// 出站设备

	// 历史记录
	historyData.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      取得UL票种信息

@param      (i)BYTE* cardInfo 读卡器返回的票卡信息

@retval     bool true:成功 false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CULCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
		LPBYTE pCardinfo = cardInfo;

		// UID
		pCardinfo += 8;

		// 发行流水号
		applicationData.CardLogicalNumber = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;

		// 发行商代码
		applicationData.PublisherCode = *pCardinfo;
		pCardinfo++;

		// 主票卡类型
		pCardinfo++;

		// 卡应用标识
		applicationData.TestCard = *pCardinfo;
		pCardinfo++;

		// 密钥版本
		pCardinfo++;

		// 卡应用版本
		applicationData.AppVersion = *pCardinfo;
		pCardinfo++;

		// 发行批次
		applicationData.BatchNumber = ComMakeWORD(*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 2;

		// 卡类型
		applicationData.CardAppMainType = *pCardinfo; 
		pCardinfo++;

		// 发行日期
		applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// 卡物理子标识
		pCardinfo++;

		// 发行验证码MAC1
		pCardinfo += 4;

		// 票种类（子应用类型）
		applicationData.CardAppMainType = applicationData.CardAppMainType - (*pCardinfo);
		pCardinfo++;
		
		// 可用站点1（线路+站点）
		applicationData.SaleStation = ComMakeWORD(*(pCardinfo+1),*pCardinfo);
		pCardinfo += 2;

		// 可用站点2（线路+站点）
		pCardinfo += 2;

		// 钱包单位           	
		pCardinfo++;

		// 有效开始时间
		applicationData.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// 有效结束时间
		applicationData.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// 出站票标志     	
		pCardinfo++;

		// 是否需要激活
		applicationData.CardActived = *pCardinfo;
		pCardinfo++;

		// 售票金额
		applicationData.SalePrice = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;

		// 状态标志
		applicationData.TicketStatus = *pCardinfo;             	
		pCardinfo++;

		// 进闸标志
		journeyData.JourneyStatus = *pCardinfo;
		pCardinfo++;

		// 钱包
		memcpy(&applicationData.Balance,pCardinfo,4);
		pCardinfo += 4;

		// 卡交易序号
		pCardinfo += 2;

		// 备份标志
		pCardinfo++;

		// 进站时间
		journeyData.EnterTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// 进站站点
		journeyData.EnterStation = ComMakeWORD(*(pCardinfo+1),*pCardinfo);
		pCardinfo += 2;

		// 交易验证码MAC2
		pCardinfo += 3;
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CULCard::GetHistoryInfo(int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
		LPBYTE pHistoryinfo = historyInfo;
		STR_CARD_HISTORY_DATA history;
		historyData.clear();
	
		for(int i=0; i<count; i++)
		{
			// 电子钱包消费、充值交易序号
			history.TransNumber = MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
			pHistoryinfo += 2;

			// 预留
			pHistoryinfo += 3;

			// 交易金额
			history.TransValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),*(pHistoryinfo + 3));
			pHistoryinfo += 4;

			// 交易类型标识
			history.TransType = *pHistoryinfo;
			pHistoryinfo++;

			// 终端机编号
			pHistoryinfo++;
			history.TransStation = MAKEWORD(*(pHistoryinfo + 1),*pHistoryinfo);
			pHistoryinfo += 5;

			// 交易日期与时间
			history.TransTime.wYear = BCD2int((char*)pHistoryinfo,2);	
			pHistoryinfo += 2;
			history.TransTime.biMonth = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biDay = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biHour = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biMinute = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biSecond = BCD2int((char*)pHistoryinfo,1);
			pHistoryinfo++;

			historyData.push_back(history);
		}
	}
	
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}