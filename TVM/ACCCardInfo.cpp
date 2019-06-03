#include "StdAfx.h"
#include "ACCCardInfo.h"

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
CACCCard::CACCCard(void)
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
CACCCard::~CACCCard(void)
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
void CACCCard::Initialize()
{
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
bool CACCCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
		ACCCARD_PRODUCT_INFO productInfo; 
		LPBYTE pCardinfo = cardInfo;

		// 1 城市代码				2	BCD	满足CJ/T166中相关的规定要求，城市邮政编码的前4位确定
		applicationData.sCityCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1)); 
		pCardinfo += 2;
		// 2 行业代码				2	BCD	建设部标准
		applicationData.sTradeCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 3 发行商代码			2	BCD	ACC发行票卡时写入0002
		applicationData.CardIssuerCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 8 发行流水号			8	BIN	卡片发行时写入的序列号
		memcpy(applicationData.CardLogicalNumber, pCardinfo, 8);
		pCardinfo += 8;
		// 5 卡应用标识			1	BIN	初始化时写入 0 － 正式卡、1 － 测试卡
		applicationData.testFlag = (ACC_CARD_TEST_STATUS)*pCardinfo;
		pCardinfo += 1;
		// 6 卡版本				1	BIN
		pCardinfo += 1;
		// 7 卡类型				1	BIN	标识着票卡的种类编号, 范围：00-99
		productInfo.productType = *pCardinfo;  
		pCardinfo += 1;
		// 8 密钥版本				1	BIN
		pCardinfo += 1;
		// 9 发行日期				4	BCD	YYYYMMDD
		applicationData.initialDate.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.initialDate.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.initialDate.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 10 发行批次			2	BIN	3位发行批次, 范围：000到999
		applicationData.sInitialNum = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));  
		pCardinfo += 2;     
		// 12 有效开始时间		7	BCD	YYYYMMDDHHMISS
		productInfo.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		productInfo.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		// 13 有效结束时间		7	BCD	YYYYMMDDHHMISS
		productInfo.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		productInfo.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		// 14 卡子类型			1	BIN	见《08郑州ACC 线网运营管理标准-08系统编码规则.doc》
		productInfo.productSubType = *pCardinfo; 
		pCardinfo += 1;
		// 15 押金				4	BIN	售卡时收取的押金，单位：分                      
		applicationData.Deposit = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;   
		// 16 售票终端			4	BIN	标识发售票卡的设备编号
		pCardinfo += 4;
		// 17 售票金额			4	BIN	除押金外，售票实际发生金额。单位：分
		pCardinfo += 4;
		// 18 可使用站点1			2	BIN	表示可以使用的站点
		productInfo.dwStartStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;	
		// 19 可使用站点2			2	BIN	表示可以使用的站点
		productInfo.dwEndStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 20 可使用区段编号		2	BIN	可使用站点集合的编号，主要用于区段票
		productInfo.wArea = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 22 钱包余额			4	BIN              
		productInfo.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo); 
		pCardinfo += 4;   
		// 23 卡状态				1	BIN	0– 未售：待出售卡； 3 – 已售：正在使用中的卡；7 – 回收：已回收卡；9 – 黑名单锁定             
		applicationData.TicketStatus = *pCardinfo;
		pCardinfo += 1;
		// 24 卡交易序号			2	BIN	卡累计交易次数，每次写卡加1，标识卡交易的连续性
		pCardinfo += 2;
		// 25 激活状态			1	BIN	 0—未激活；1—已激活       
		productInfo.CardActived = (ACC_CARD_ACTIVE_STATUS)*pCardinfo;   
		pCardinfo += 1;  
		// 26 激活后有效开始时间	6	BCD	需要激活的票卡，激活后的有效开始日期, YYYYMMDDHHMI
		//productInfo.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		//pCardinfo += 2;
		//productInfo.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		pCardinfo+=6;
		// 27 激活后有效结束时间	6	BCD	YYYYMMDDHHMI
		//productInfo.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		//pCardinfo += 2;
		//productInfo.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		pCardinfo+=6;
		// 28 持卡人姓名			20	ASC	持卡人姓名采用GB2312-80汉字编码方式，两字节为一个汉字，最长8个汉字
		memcpy(userInfo.name,pCardinfo,20);
		pCardinfo += 20;
		// 29 持卡人证件号码		32	ASC	如身份证、学生证等证件号码
		memcpy(userInfo.certificateCardNumber,pCardinfo,32);
		pCardinfo += 32;
		// 30 持卡人证件类型		1	BCD	0—身份证 1—军官证 2—护照 3—学生证
		userInfo.certificateCardType = *pCardinfo;
		pCardinfo += 1;
		// 31 持卡人性别			1	BIN	0—男     1—女
		pCardinfo += 1;
		// 32 工作单位编码		2	BIN	标识持卡人所在单位的编码
		userInfo.companyID = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 33 持卡人职工标识		1	BCD	是否为本单位职工
		pCardinfo += 1;
		// 34 进出站标志			1	BIN	0—未进站 1—已进站
		journeyData.byTravelStatus = (ACC_CARD_JOURNEY_STATUS)*pCardinfo;
		pCardinfo += 1;
		// 35 进出站终端			4	BIN	票卡进出站的设备编号
		pCardinfo += 4;
		//    进出站站点			2	BIN	票卡进出站的站点
		journeyData.dwLastTransStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 36 进出站时间			7	BCD	票卡进出站的交易时间, YYYYMMDDHHMISS               
		journeyData.lastTransTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		journeyData.lastTransTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 37 日乘坐次数			2	BIN	每天乘坐的次数
		pCardinfo += 2;
		// 38 乘坐次数累计		4	BIN	乘坐次数累计次数
		pCardinfo += 4;
		// 39 月乘坐累计次数		2	BIN	每月乘坐次数累计
		pCardinfo += 2;
		// 40 充值终端号			4	BIN	最后一次充值的设备编号
		//	rechargeData.transStation = ComMakeLong(*pCardinfo,*(pCardinfo+1),*(pCardinfo+2),*(pCardinfo+3));
		pCardinfo += 4;
		// 41 充值交易类型		1	BIN	最后一次充值交易类型
		rechargeData.TransType = *pCardinfo;
		pCardinfo += 1;
		// 42 充值交易时间		7	BCD	最后一次充值交易的日期时间
		rechargeData.TransTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		rechargeData.TransTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 43 充值交易金额		4	BIN	最后一次充值金额，单位：分
		rechargeData.TransValue = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;
		// 44 充值交易序号		2	BIN	最后一次充值的交易序号
		pCardinfo += 2;
		// 45 充值后余额			4	BIN	最后一次充值后的余额，单位：分
		rechargeData.remaingValue = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;
		// 46 累计充值金额		4	BIN	累积充值的金额，单位：分
		pCardinfo += 4;
		// 47 上次充值单价		2	BIN	针对次票上次充次的单价，单位：分
		pCardinfo += 2;
		// 48 本次充值单价		2	BIN	针对次票本次充次的单价，单位：分
		pCardinfo += 2;
		// 49 充次到期日期		7	BCD	针对次票上次充次的到期日期
		pCardinfo += 7;
		// 50 充值操作员		3	BCD	最后一次充值的操作员编码
		rechargeData.TransOperator = BCD2int((char*)pCardinfo, 3);
		pCardinfo += 3;
		// 51 老人卡钱包余次	4	BIN	该字段仅对老人卡钱包余次有效，其余填0
		productInfo.nRemainRides = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;

		// 单独赋值字段
		applicationData.byProductsNum = 1;	//产品数量
		productInfo.productCategory = (ProductCategory_t)theACC_PRODUCT.GetTicketCategry(ComMakeWORD(productInfo.productType,productInfo.productSubType));	// 产品种类
		applicationData.byLastUsedProductCategory = productInfo.productCategory;	// 最后使用的产品种类
		applicationData.byLastUsedProductSubType = productInfo.productSubType;		// 最后使用的产品类型
		applicationData.byLastUsedProductType = productInfo.productType;			// 最后使用的产品子类型
		v_ProductInfo.push_back(productInfo);

		// 未使用字段
		//productInfo.productIssuerID = 1;				// 产品发行商ID
		//productInfo.sSamID = 1;							// 上次对产品发售/加值的SAM ID
		//applicationData.sInitialNum = 1;				// 卡重发行计数
		//applicationData.byPassengerType = 1;			// 乘客类型
		//applicationData.byTransNum= 1;					// 已换乘次数
		//// 本次旅程开始时间                 
		//journeyData.travelStartTime.wYear = 2013;
		//journeyData.travelStartTime.biMonth = 1;
		//journeyData.travelStartTime.biDay = 1;
		//journeyData.travelStartTime.biHour = 0;
		//journeyData.travelStartTime.biMinute = 0;
		//journeyData.travelStartTime.biSecond = 0;
		//// 本次旅程的起始站点  
		//journeyData.dwTravelStartStation = 1;
		//// 已付车费
		//journeyData.nPaid = 1;
		//// 个性化标示
		//applicationData.byPersonalizeStatus = 0;
		//if (applicationData.byPersonalizeStatus){
		//	// 生日
		//	userInfo.birthday.wYear = 2000;
		//	userInfo.birthday.biMonth = 1;
		//	userInfo.birthday.biDay = 1;
		//	// 乘客级别
		//	userInfo.certificateCardType = CLASSIFICATION_LEVEL_NORMOL;
		//}
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
@brief      取得ACC历史交易记录信息

@param      (i)BYTE* cardInfo 读卡器返回的票卡信息

@retval     bool true:成功 false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CACCCard::GetHistoryInfo(int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
		LPBYTE pHistoryinfo = historyInfo;
		CARD_HISTORY_DATA history;
		historyData.clear();

		for(int i=0; i<count; i++)
		{
			CARD_HISTORY_DATA history;
			// 1 交易序号						2	BIN
			pHistoryinfo += 2;
			// 2 交易金额						4	BIN	单位：分
			history.TransValue = ComMakeLong(*(pHistoryinfo+3),*(pHistoryinfo+2),*(pHistoryinfo+1),*pHistoryinfo);
			pHistoryinfo += 4;
			// 3 交易类型						1	BIN
			history.TransType = *pHistoryinfo;
			pHistoryinfo += 1;
			// 4 终端机编号					6	BIN
			//WORD high_long = ComMakeWORD(*pHistoryinfo,*(pHistoryinfo+1));
			//DWORD low_long = ComMakeLong(*(pHistoryinfo+2),*(pHistoryinfo+3),*(pHistoryinfo+4),*(pHistoryinfo+5));
			//WORD high_long = ComMakeWORD(*pHistoryinfo,*(pHistoryinfo+1),*(pHistoryinfo+2),*(pHistoryinfo+3));
			//DWORD low_long = ComMakeLong(*(pHistoryinfo+4),*(pHistoryinfo+5));
			memcpy(history.transStation,pHistoryinfo,6);
			pHistoryinfo += 6;
			// 5 交易时间						7	BCD	
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

			// 未使用字段
			//// 服务提供者  
			//history.serviceProviderId = *pHistoryinfo;
			//pHistoryinfo++;
			//// 产品类型  0:无 1:钱包 2:定期类型 3:计次类型 4-7:预留  
			//history.productType = (*pHistoryinfo)>>5;
			//// 发行商 
			//BYTE productIssuerId = *pHistoryinfo;
			//history.productIssuerId = productIssuerId%32;
			//pHistoryinfo ++;                                     
			//// 交易方式  
			//WORD paymentMethod= MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
			//paymentMethod = paymentMethod%61440;
			//paymentMethod = paymentMethod>>7;
			//history.paymentMethod = paymentMethod;
			//pHistoryinfo++;
			//// 产品类型
			//BYTE productType = *pHistoryinfo;
			//productType = productType%128;
			//productType = productType>>1;
			//history.productType = productType;                                
			//// 交易后金额                                            
			//DWORD remaingValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),0);
			//remaingValue = remaingValue&0x3FFFFFF;
			//remaingValue = remaingValue>>3;
			//history.remaingValue = remaingValue;
			//pHistoryinfo += 3;
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定的卡内产品信息

@param      (i)WORD productType 产品类型
@param      (i)WORD productSubType 产品子类型
@param      (o)ACCCARD_PRODUCT_INFO* productInfo 产品信息

@retval     bool	true:成功	false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CACCCard::GetProductInfo(WORD productType, WORD productSubType, ACCCARD_PRODUCT_INFO& productInfo)
{
	for (vector<ACCCARD_PRODUCT_INFO>::iterator iter = v_ProductInfo.begin(); iter != v_ProductInfo.end(); ++iter)
	{
		if (iter->productType == productType && iter->productSubType==productSubType)
		{
			productInfo = *iter;
			return true;
		}
	}

	return false;
}