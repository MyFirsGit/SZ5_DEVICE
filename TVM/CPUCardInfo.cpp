#include "StdAfx.h"
#include "CPUCardInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
///**
//@brief      构造函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CCPUCard::CCPUCard(void)
//{
//	Initialize();
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      析构函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CCPUCard::~CCPUCard(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      初始化
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CCPUCard::Initialize()
//{
//	// 票卡基本信息结构体
//	applicationData.PublisherCode = 0;					// 发卡方代码
//	applicationData.CityCode = 0;						// 城市代码
//	applicationData.IndustryCode = 0;					// 行业代码
//	applicationData.CardPhysicalType = 0;				// 票卡物理类型
//	memset(applicationData.CardPhysicalNumber,0,8);		// 票卡物理编号
//	applicationData.CardLogicalNumber = 0;				// 票卡逻辑编号
//	applicationData.CardAppMainType = 0;				// 卡应用主类型
//	applicationData.CardAppSubType = 0;					// 卡应用子类型
//	applicationData.TestCard = 0;						// 测试卡标志
//	applicationData.CardEnabled = 0;					// 启用标志
//	applicationData.TicketStatus = 0;
//	applicationData.CardLocked = 0;
//	applicationData.CardActived = 0;
//	memset(applicationData.DeviceID,0,6);				// 发行设备信息
//	applicationData.AppVersion = 0;						// 应用版本号
//	applicationData.ExchangeCard = 0;					// 互通卡标识
//	applicationData.Deposit = 0;						// 押金
//	applicationData.Cost = 0;							// 成本
//	applicationData.Balance = 0;						// 票内余额
//
//	// 持卡人信息结构体
//	personalData.Sex = 0;								// 持卡人性别
//	personalData.CredentialsType = 0;					// 持卡人证件类型
//
//	// 旅程信息结构体
//	journeyData.JourneyStatus = 0;						// 旅程状态
//	journeyData.EnterStation = 0;						// 进站站点
//	memset(journeyData.EnterDeviceID,0,6);				// 进站设备 
//	journeyData.ExitStation = 0;						// 出站站点
//	memset(journeyData.ExitDeviceID,0,6);				// 出站设备
//
//	// 发售信息结构体
//	saleData.SaleStation = 0;
//	saleData.StarSection = 0;
//	saleData.EndSection = 0;
//	saleData.SalePrice = 0;
//
//	// 历史记录
//	historyData.clear();
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得UL票种信息
//
//@param      (i)BYTE* cardInfo 读卡器返回的票卡信息
//
//@retval     bool true:成功 false:失败
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CCPUCard::GetCardInfo(WORD len,BYTE* cardInfo)
//{
//	bool ret = true;
//	try {
//		LPBYTE pCardinfo = cardInfo;
//
//		if(len == 360){
//			// 发卡方代码
//			applicationData.PublisherCode = BCD2int((char*)pCardinfo,1);			
//			pCardinfo += 2;
//
//			// 城市代码
//			applicationData.CityCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 行业代码
//			applicationData.IndustryCode = BCD2int((char*)pCardinfo,1);
//			pCardinfo += 2;
//
//			// 预留
//			pCardinfo += 2;
//
//			// 票卡逻辑编号
//			pCardinfo += 4;
//			applicationData.CardLogicalNumber = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//			pCardinfo += 4;
//
//			// 卡应用主类型
//			pCardinfo++;
//			applicationData.CardAppMainType = *pCardinfo;
//			pCardinfo++;
//
//			// 发行日期
//			applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 发行设备信息
//			memcpy(applicationData.DeviceID,(char*)pCardinfo,6);
//			pCardinfo += 6;
//
//			// 应用版本号
//			applicationData.AppVersion = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 发行商代码
//			pCardinfo += 2;
//
//			// 城市代码
//			pCardinfo += 2;
//
//			// 行业代码
//			pCardinfo += 2;
//
//			// 预留
//			pCardinfo += 2;
//
//			// 应用类型标识（启用标志）
//			applicationData.CardEnabled = *pCardinfo;
//			pCardinfo++;
//
//			// 应用版本
//			pCardinfo++;
//
//			// 互通卡标识
//			applicationData.ExchangeCard = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 应用序列号
//			pCardinfo += 8;
//
//			// 应用启用日期
//			applicationData.EnabledDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.EnabledDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.EnabledDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 应用有效日期
//			applicationData.ValidDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.ValidDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.ValidDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 预留
//			pCardinfo += 2;
//
//			// 持卡人类型标识
//			personalData.CardHoldType = *pCardinfo;
//			pCardinfo++;
//
//			// 持卡人职工标识
//			personalData.StaffStatus = *pCardinfo;
//			pCardinfo++;
//
//			// 持卡人姓名
//			char name[21] = {0};
//			memcpy(name, pCardinfo, 20);
//			personalData.Name = name;
//			pCardinfo += 20;
//
//			// 持卡人证件号码
//			char num[33] = {0};
//			memcpy(num, pCardinfo, 32);
//			personalData.CredentialsNumber = num;
//			pCardinfo += 32;
//
//			// 持卡人证件类型
//			personalData.CredentialsType = *pCardinfo;
//			pCardinfo++;
//
//			// 押金金额（分）
//			applicationData.Deposit = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 复合消费标识
//			pCardinfo++;
//
//			// 记录长度
//			pCardinfo++;
//
//			// 复合消费锁定标志位
//			pCardinfo++;
//
//			// 本地卡
//			if(applicationData.CityCode == 0x1161 || applicationData.CityCode == 0x1100){
//					pCardinfo += 16;
//			}
//			// 异地卡
//			else{
//				// 进出站标志
//				journeyData.JourneyStatus = *pCardinfo;
//				pCardinfo++;
//
//				// 已进站
//				if(journeyData.JourneyStatus != UNENTER_STATION){
//					// 进出站终端
//					pCardinfo += 4;
//
//					// 进出站地点
//					BYTE pEnterStation[2] = {0,0};
//					WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(enterStation,(char*)pEnterStation,2);
//					journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//					pCardinfo += 4;
//
//					// 进出站时间
//					journeyData.EnterTime.wYear = BCD2int((char*)pCardinfo,2);
//					pCardinfo += 2;
//					journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biSecond = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//				}
//				// 已出站
//				else {
//					// 进出站终端
//					pCardinfo += 4;
//
//					// 进出站地点
//					BYTE pExitStation[2] = {0,0};
//					WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(exitStation,(char*)pExitStation,2);
//					journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//					pCardinfo += 4;
//
//					// 进出站时间
//					journeyData.ExitTime.wYear = BCD2int((char*)pCardinfo,2);
//					pCardinfo += 2;
//					journeyData.ExitTime.biMonth = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biDay = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biHour = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biMinute = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biSecond = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//				}
//				//else{
//				//	pCardinfo += 15;
//				//}
//			}
//
//			// 预留
//			pCardinfo += 29;
//
//			// 卡状态标志
//			applicationData.TicketStatus = *pCardinfo;
//			pCardinfo++;
//
//			// 交易类型标识
//			pCardinfo++;
//
//			// 最近使用终端的企业编号
//			pCardinfo += 2;
//
//			// 最近使用终端的线路编号
//			pCardinfo += 2;
//
//			// 公共交通应用最近使用地点
//			pCardinfo += 4;
//
//			// 公共交通应用最近使用时间
//			pCardinfo += 7;
//
//			// 透支金额
//			applicationData.dwOverdraftValue = ComMakeLong(0,*(pCardinfo + 2), *(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 3;
//
//			// 积分
//			pCardinfo += 4;
//
//			// 预留
//			pCardinfo += 42;
//
//			// 本地卡
//			if((applicationData.CityCode == 0x1161) 
//				|| (applicationData.CityCode == 0x1100)){
//					// 进出站标志
//					journeyData.JourneyStatus = *pCardinfo;
//					pCardinfo++;
//					// 已进站
//					if(journeyData.JourneyStatus != UNENTER_STATION){
//						// 进出站终端
//						pCardinfo += 4;
//						
//						BYTE emptyEnterInfo[8] = {0};
//						if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//						{
//							journeyData.EnterStation = 0;
//							journeyData.EnterTime = _DATE_TIME();
//							pCardinfo+=8;
//						}
//						else
//						{
//							// 进出站地点
//							BYTE pEnterStation[2] = {0,0};
//							WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//							int2BCD(enterStation,(char*)pEnterStation,2);
//							journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//							pCardinfo += 4;
//
//							// 起始时间(6位年、4位月、5位日、5位时、6位分、6位秒)
//							DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//							BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//							BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//							BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//							BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//							BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//							BYTE second = (BYTE)(tmpTime & 0x3f);
//							journeyData.EnterTime.wYear = 2000 + year;
//							journeyData.EnterTime.biMonth = month;
//							journeyData.EnterTime.biDay = day;
//							journeyData.EnterTime.biHour = hour;
//							journeyData.EnterTime.biMinute = minute;
//							journeyData.EnterTime.biSecond = second;
//							pCardinfo += 4;
//						}
//						
//
//					}
//					// 已出站
//					else{
//						// 进出站终端
//						pCardinfo += 4;
//						BYTE emptyEnterInfo[8] = {0};
//						if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//						{
//							journeyData.ExitStation = 0;
//							journeyData.ExitTime = _DATE_TIME();
//							pCardinfo+=8;
//						}
//						else
//						{
//							// 进出站地点
//							BYTE pExitStation[2] = {0,0};
//							WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//							int2BCD(exitStation,(char*)pExitStation,2);
//							journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//							pCardinfo += 4;
//
//							// 起始时间(6位年、4位月、5位日、5位时、6位分、6位秒)
//							DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//							BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//							BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//							BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//							BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//							BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//							BYTE second = (BYTE)(tmpTime & 0x3f);
//							journeyData.ExitTime.wYear = 2000 + year;
//							journeyData.ExitTime.biMonth = month;
//							journeyData.ExitTime.biDay = day;
//							journeyData.ExitTime.biHour = hour;
//							journeyData.ExitTime.biMinute = minute;
//							journeyData.ExitTime.biSecond = second;
//							pCardinfo += 4;
//						}
//					}
//					//else{
//					//	pCardinfo += 12;
//					//}
//			}
//			// 异地卡
//			else{
//				pCardinfo += 13;
//			}
//
//			// 日乘坐次数
//			pCardinfo += 2;
//
//			// 日乘坐计数日期
//			pCardinfo += 4;
//
//			// 乘坐次数累计
//			pCardinfo += 2;
//
//			// 本地复合记录MAC
//			pCardinfo += 4;
//
//			// 预留
//			pCardinfo += 36;
//
//			// 复合消费标识
//			pCardinfo++;
//
//			// 记录长度
//			pCardinfo++;
//
//			// 应用锁定标志位
//			pCardinfo++;
//
//			// 上下车标识
//			pCardinfo++;
//
//			// 单边趟次
//			pCardinfo++;
//
//			// 上车站点号
//			pCardinfo++;
//
//			// 上车时间
//			pCardinfo += 4;
//
//			// 公司号
//			pCardinfo++;
//
//			// 路队线路号
//			pCardinfo += 4;
//
//			// 车辆号
//			pCardinfo += 3;
//
//			// 司机卡号
//			pCardinfo += 4;
//
//			// 全程票价
//			pCardinfo += 4;
//
//			// 预留
//			pCardinfo += 38;
//
//			// 钱包余额
//			if(applicationData.dwOverdraftValue > 0){		// 已透支
//				applicationData.Balance = 0 - applicationData.dwOverdraftValue;
//			}
//			else{
//				applicationData.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			}		
//			pCardinfo += 4;
//		}
//		//ADF2卡
//		else if(len == 304){
//			// 发卡方代码
//			applicationData.PublisherCode = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//
//			// 城市代码
//			applicationData.CityCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 行业代码
//			applicationData.IndustryCode = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//
//			// 预留
//			pCardinfo += 2;
//
//			// 票卡逻辑编号
//			pCardinfo += 4;
//			applicationData.CardLogicalNumber = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//			pCardinfo += 4;
//
//			// 卡类型（不使用）
//			pCardinfo += 2;
//
//			// 发行日期
//			applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 发行设备信息
//			memcpy(applicationData.DeviceID,(char*)pCardinfo,6);
//			pCardinfo += 6;
//
//			// 应用版本号
//			applicationData.AppVersion = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// 发行商代码
//			pCardinfo += 8;
//
//			// 应用类型标识
//			applicationData.CardEnabled = *pCardinfo;
//			pCardinfo++;
//
//			// 卡类型（使用）
//			applicationData.CardAppMainType = *pCardinfo;
//			pCardinfo++;
//
//			// 国家标识
//			pCardinfo++;
//
//			// 建设部标识
//			pCardinfo++;
//
//			// 城市代码
//			pCardinfo += 2;
//
//			// 行业代码
//			pCardinfo += 2;
//
//			// 卡应用号
//			pCardinfo += 4;
//
//			// 发行日期 
//			pCardinfo += 4;
//
//			// 终止日期
//			pCardinfo += 4;
//
//			// 卡版本号
//			pCardinfo++;
//
//			// 应用版本号
//			pCardinfo++;
//
//			// 票卡主类型
//			pCardinfo++;
//
//			// 预留
//			pCardinfo++;
//
//			// 有效开始日期
//			applicationData.EnabledDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.EnabledDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.EnabledDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 有效结束日期
//			applicationData.ValidDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.ValidDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.ValidDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// 押金金额（分）
//			applicationData.Deposit = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//
//			// 卡子类型
//			pCardinfo++;
//
//			// 激活方式
//			pCardinfo++;
//
//			// 激活状态
//			pCardinfo++;
//
//			// 可使用站点1
//			saleData.SaleStation = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//
//			// 可使用站点2
//			pCardinfo += 4;
//
//			// 可使用区段
//			pCardinfo += 4;
//
//			// 售票终端机编号
//			pCardinfo += 4;
//
//			// 售票操作员代码
//			pCardinfo += 4;
//
//			// 售票时间
//			pCardinfo += 4;
//
//			// 售票金额
//			pCardinfo += 4;
//
//			// 信用交易标识
//			pCardinfo++;
//
//			// 信用充值级别
//			pCardinfo++;
//
//			// 预留
//			pCardinfo += 3;
//
//			// 应用版本
//			pCardinfo++;
//
//			// 本单位职工标识
//			personalData.StaffStatus = *pCardinfo;
//			pCardinfo++;
//
//			// 持卡人姓名
//			char name[21] = {0};
//			memcpy(name, pCardinfo, 20);
//			personalData.Name = name;
//			pCardinfo += 20;
//
//			// 持卡人证件号码
//			char num[33] = {0};
//			memcpy(num, pCardinfo, 32);
//			personalData.CredentialsNumber = num;
//			pCardinfo += 32;
//
//			// 持卡人证件类型
//			personalData.CredentialsType = *pCardinfo;
//			pCardinfo++;
//
//			// 持卡人性别
//			personalData.Sex = *pCardinfo;
//			pCardinfo++;
//
//			// 保留
//			pCardinfo += 24;
//
//			// ?
//			pCardinfo++;
//
//			// 卡状态标志
//			applicationData.TicketStatus = *pCardinfo;
//			pCardinfo++;
//
//			// 交易类型标识
//			pCardinfo++;
//
//			// 最近使用时间
//			pCardinfo += 7;
//
//			// 最近使用终端的企业编号
//			pCardinfo += 2;
//
//			// 最近使用终端的线路编号
//			pCardinfo += 2;
//
//			// 公共交通应用最近使用地点
//			pCardinfo += 4;
//
//			// 公共交通应用最近使用时间
//			pCardinfo += 7;
//
//			// 积分
//			pCardinfo += 4;
//
//			// 保留
//			pCardinfo++;
//
//			// 充值交易序号
//			pCardinfo += 2;
//
//			// 充值后余额
//			pCardinfo += 4;
//
//			// 充值交易金额
//			pCardinfo += 4;
//
//			// 交易类型标识
//			pCardinfo++;
//
//			// 终端机编号
//			pCardinfo += 6;
//
//			// 交易日期时间
//			pCardinfo += 7;
//
//			// 累计充值额
//			pCardinfo += 4;
//
//			// 充值操作员代号
//			pCardinfo += 4;
//
//			// 上次充值单价
//			pCardinfo += 2;
//
//			// 本次充值单价
//			pCardinfo += 2;
//
//			// 充次到期日期
//			pCardinfo += 4;
//
//			// 预留
//			pCardinfo += 8;
//
//			// ?
//			pCardinfo++;
//
//			// 进出站标志
//			journeyData.JourneyStatus = *pCardinfo;
//			pCardinfo++;
//			
//			// 已进站
//			if(journeyData.JourneyStatus != UNENTER_STATION){
//				// 进出站终端
//				pCardinfo += 4;
//				BYTE emptyEnterInfo[8] = {0};
//				if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//				{
//					journeyData.EnterStation = 0;
//					journeyData.EnterTime = _DATE_TIME();
//					pCardinfo+=8;
//				}
//				else
//				{
//					// 进出站地点
//					BYTE pEnterStation[2] = {0,0};
//					WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(enterStation,(char*)pEnterStation,2);
//					journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//					pCardinfo += 4;
//
//					// 起始时间(6位年、4位月、5位日、5位时、6位分、6位秒)
//					DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//					BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//					BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//					BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//					BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//					BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//					BYTE second = (BYTE)(tmpTime & 0x3f);
//					journeyData.EnterTime.wYear = 2000 + year;
//					journeyData.EnterTime.biMonth = month;
//					journeyData.EnterTime.biDay = day;
//					journeyData.EnterTime.biHour = hour;
//					journeyData.EnterTime.biMinute = minute;
//					journeyData.EnterTime.biSecond = second;
//					pCardinfo += 4;
//				}
//			}
//			// 已出站
//			else{
//				// 进出站终端
//				pCardinfo += 4;
//				BYTE emptyEnterInfo[8] = {0};
//				if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//				{
//					journeyData.ExitStation = 0;
//					journeyData.ExitTime = _DATE_TIME();
//					pCardinfo+=8;
//				}
//				else
//				{
//					// 进出站地点
//					BYTE pExitStation[2] = {0,0};
//					WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(exitStation,(char*)pExitStation,2);
//					journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//					pCardinfo += 4;
//
//					// 起始时间(6位年、4位月、5位日、5位时、6位分、6位秒)
//					DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//					BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//					BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//					BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//					BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//					BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//					BYTE second = (BYTE)(tmpTime & 0x3f);
//					journeyData.ExitTime.wYear = 2000 + year;
//					journeyData.ExitTime.biMonth = month;
//					journeyData.ExitTime.biDay = day;
//					journeyData.ExitTime.biHour = hour;
//					journeyData.ExitTime.biMinute = minute;
//					journeyData.ExitTime.biSecond = second;
//					pCardinfo += 4;
//				}
//			}
//			//else{
//			//	pCardinfo += 12;
//			//}
//
//			// 日乘坐次数
//			pCardinfo += 2;
//
//			// 日乘坐计数日期
//			pCardinfo += 4;
//
//			// 乘坐次数累计
//			pCardinfo += 2;
//
//			// 月乘坐累计次数
//			pCardinfo += 2;
//
//			// 预留
//			pCardinfo += 8;
//
//			// 钱包余额
//			applicationData.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//		}
//	}
//	// 异常捕获
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得ECT历史交易记录信息
//
//@param      (i)BYTE* cardInfo 读卡器返回的票卡信息
//
//@retval     bool true:成功 false:失败
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CCPUCard::GetHistoryInfo(int count,BYTE* historyInfo)
//{
//	bool ret = true;
//	try {
//		LPBYTE pHistoryinfo = historyInfo;
//		STR_CARD_HISTORY_DATA history;
//		historyData.clear();
//
//		for(int i=0; i<count; i++)
//		{
//			// 电子钱包消费、充值交易序号
//			history.TransNumber = MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
//			pHistoryinfo += 2;
//
//			// 预留
//			pHistoryinfo += 3;
//
//			// 交易金额
//			history.TransValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),*(pHistoryinfo + 3));
//			pHistoryinfo += 4;
//
//			// 交易类型标识
//			history.TransType = *pHistoryinfo;
//			pHistoryinfo++;
//
//			// 终端机编号
//			pHistoryinfo++;
//			history.TransStation = MAKEWORD(*(pHistoryinfo + 1),*pHistoryinfo);
//			pHistoryinfo += 5;
//
//			// 交易日期与时间
//			history.TransTime.wYear = BCD2int((char*)pHistoryinfo,2);	
//			pHistoryinfo += 2;
//			history.TransTime.biMonth = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biDay = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biHour = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biMinute = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biSecond = BCD2int((char*)pHistoryinfo,1);
//			pHistoryinfo++;
//
//			if(history.TransType != 0){
//				historyData.push_back(history);
//			}			
//		}
//	}
//	// 异常捕获
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//	return ret;
//}