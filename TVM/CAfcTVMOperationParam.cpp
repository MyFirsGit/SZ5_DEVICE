#include "StdAfx.h"
#include "CAfcTVMOperationParam.h"
#include "ParameterDef.h"
#include "CParamException.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALID_VALUE		0xFF

CAfcTVMOperationParam CAfcTVMOperationParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcTVMOperationParam::CAfcTVMOperationParam(void):CAfcParamMaster()
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
CAfcTVMOperationParam::~CAfcTVMOperationParam(void)
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
void CAfcTVMOperationParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1011);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::ParseBody(LPBYTE lpData)
{
	try{
		//m_tpu_param.vecPaymentID.clear();
		// 偏移量设置	
		lpData += m_section[0].startPosition;
		// 分段1：TVM票卡读写参数
		for(int iloop=0;iloop<m_section[0].recordCount;iloop++)
		{
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
		// 分段2：TVM维修登录相关参数
		for(int iloop=0;iloop<m_section[1].recordCount ;iloop++)
		{
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
		// 分段3：TVM钱箱参数
		for(int iloop=0;iloop<m_section[2].recordCount ;iloop++)
		{
			//********************售票Ticket***********************//
			//售票接收的纸币面额1 1	BIN
			m_billbox_param.acceptSalesTicketBillValue1 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额2 1	BIN
			m_billbox_param.acceptSalesTicketBillValue2 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额3 1	BIN
			m_billbox_param.acceptSalesTicketBillValue3 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额4 1	BIN
			m_billbox_param.acceptSalesTicketBillValue4 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));
			lpData += 1;
			//售票接收的纸币面额5 1	BIN
			m_billbox_param.acceptSalesTicketBillValue5 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额6 1	BIN
			m_billbox_param.acceptSalesTicketBillValue6 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额7 1	BIN
			m_billbox_param.acceptSalesTicketBillValue7 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额8 1	BIN
			m_billbox_param.acceptSalesTicketBillValue8 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额9 1	BIN
			m_billbox_param.acceptSalesTicketBillValue9 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的纸币面额10 1 BIN
			m_billbox_param.acceptSalesTicketBillValue10= *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//售票接收的最大纸币数量 2	BIN

			m_billbox_param.acceptSalesTicketMaxBillCnt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//售票接收的最大纸币总额 2	BIN
			m_billbox_param.acceptSalesTicketMaxBillAmt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;

			// 售票接收的硬币面额 1 单位：角
			m_billbox_param.acceptSalesCoinValue1 = *lpData;
			lpData++;
			// 售票接收的硬币面额 2 单位：元
			m_billbox_param.acceptSalesCoinValue2 = *lpData;
			lpData++;
			//预留
			lpData += 2;
			//********************充值***********************//
			//充值接收的纸币面额1 1	BIN
			m_billbox_param.acceptChrgrBillValue1 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//充值接收的纸币面额2 1	BIN
			m_billbox_param.acceptChrgrBillValue2 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额3 1	BIN
			m_billbox_param.acceptChrgrBillValue3 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额4 1	BIN
			m_billbox_param.acceptChrgrBillValue4 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 1;
			//充值接收的纸币面额5 1	BIN
			m_billbox_param.acceptChrgrBillValue5 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额6 1	BIN
			m_billbox_param.acceptChrgrBillValue6 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额7 1	BIN
			m_billbox_param.acceptChrgrBillValue7 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额8 1	BIN
			m_billbox_param.acceptChrgrBillValue8 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额9 1	BIN
			m_billbox_param.acceptChrgrBillValue9 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的纸币面额10 1	BIN
			m_billbox_param.acceptChrgrBillValue10= *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//充值接收的最大纸币数量 2	BIN
			m_billbox_param.acceptChrgrMaxBillCnt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//充值接收的最大纸币总额 2	BIN
			m_billbox_param.acceptChrgrMaxBillAmt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// 预留
			lpData += 4;
			//******************************************************************//
			//无硬币找零时是否允许售票 1	BIN
			m_billbox_param.salesNoCoin =*lpData;	
			lpData += 1;
			//接受硬币的最大数量       1    BIN
			m_billbox_param.acceptCoinMaxCnt =*lpData;
			lpData += 1;
			//硬币找零的最大数量       1    BIN
			m_billbox_param.chargeCoinMaxCnt =*lpData;
			lpData += 1;
			//售票是否允许纸币找零     1    BIN
			m_billbox_param.chargeBill =*lpData;
			lpData += 1;
			//纸币找零最大纸币数量     1    BIN
			m_billbox_param.chargeBillMaxCnt =*lpData;
			lpData += 1;
			//纸币找零最大纸币总金额   1    BIN
			m_billbox_param.chargeBillMaxAmt =*lpData;
			lpData += 1;
			// 预留						9 BIN
			lpData += 9;
			// 招援按钮启用标识			1 BIN
			m_billbox_param.IsEnableAssistanceBtn = *lpData;
			lpData += 1;
			//******************************************************************//
			//交易取消时间	2	BIN
			m_billbox_param.cancelDealTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//是否允许显示待机画面	1	BIN
			m_billbox_param.showWaitScreen = *lpData;		
			lpData += 1;
			//待机画面切换时间	2	BIN
			m_billbox_param.changeWaitScreenTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//纸币钱箱将满张数	2	BIN
			m_billbox_param.willFullBillCnt     = mstos (ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//纸币钱箱已满张数	2	BIN
			m_billbox_param.alreadyFullBillCnt  = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//硬币回收箱A将满数量	2	BIN  
			m_billbox_param.coinCollectionWillFullCoinCnt     = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//硬币回收箱A已满数量	2	BIN
			m_billbox_param.coinCollectionAlreadyFullCoinCnt  = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			//硬币找零箱A将空数量	2	BIN
			m_billbox_param.coinChangeWillEmptyCoinCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//TVM最少车票数量	2	BIN(预留)
			m_billbox_param.minTicketCnt        = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//TVM纸币找零箱将空数量	2	BIN(预留)
			m_billbox_param.willEmptyBillBoxCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//预留字段
			lpData += 3;
			//***************************************************************************//
		}
		// 分段4：TVM票箱参数
		for(int iloop=0;iloop<m_section[3].recordCount ;iloop++)
		{
			// Card票箱将空张数阀值
			m_magazine_param.TicketNearEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Card票箱已空张数阀值
			m_magazine_param.TicketEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Card票箱将满张数阀值
			m_magazine_param.TicketNearFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Card票箱已满张数阀值
			m_magazine_param.TicketFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Card废票箱将满张数阀值
			m_magazine_param.TicketInvalidNearFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// Card废票箱已满张数阀值
			m_magazine_param.TicketInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// 预留
			lpData += 4;
		}
		// 分段5：TVM最近可查最小交易限制
		for(int iloop=0;iloop<m_section[4].recordCount ;iloop++)
		{
			//可查询交易数量下限 
			m_txn_lookup_param.minCount = *lpData;			
			lpData ++;
			//可查询交易时间下限
			m_txn_lookup_param.minTime = *lpData;			
			lpData ++;

			// 日志保留天数
			m_txn_lookup_param.logSaveDay = *lpData;
			lpData++;

			// 脱机最大天数
			m_txn_lookup_param.maxOffLineDay = *lpData;
			lpData++;

			// 已上传交易保留天数
			m_txn_lookup_param.expiredTransKeepDay = *lpData;
			lpData++;
		}

		m_vecDiskWaringSet.clear();
		// 分段6：磁盘空间报警参数
		for(int iloop = 0;iloop < m_section[5].recordCount;iloop++){
			// 磁盘类别数量
			WORD wDiskCount = *lpData;
			lpData++;

			// 遍历磁盘配置值
			for(int nCount = 0;nCount < wDiskCount;nCount++){
				DISK_WARN_SETTING diskWarnSet;
				// 磁盘类别ID
				diskWarnSet.diskType = (DISK_TYPE)*lpData;
				lpData++;

				// 磁盘将满报警阀值（百分比）
				diskWarnSet.wDiskNearFullPersent = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// 磁盘将满报警阀值（单位：MB）
				diskWarnSet.wDiskNearFullValue	 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// 磁盘已满故障阀值（百分比）
				diskWarnSet.wDiskFullPersent	 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// 磁盘已满故障阀值（单位：MB）
				diskWarnSet.wDiskFullValue		 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				m_vecDiskWaringSet.push_back(diskWarnSet);
			}
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		//throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CAfcTVMOperationParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount && i < 6/* MaxCount */;i++){
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
		//throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CAfcTVMOperationParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得TVM票卡读写参数

@param      (o)TPU_PARAM& temParam   TVM票卡读写参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetTpuParam(TPU_PARAM& temParam)
{
	temParam = m_tpu_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得TVM维修登录相关参数

@param      (o)MAINTAIN_PARAM& temParam   TVM维修登录相关参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetMaintainParam(MAINTAIN_PARAM& temParam)
{
	temParam = m_maintain_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得TVM钱箱参数

@param      (o)MAGAZINE_PARAM& operationTm   TVM票箱参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetBillboxParam(BILLBOX_PARAM& temParam)			
{
	temParam = m_billbox_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得TVM票箱参数

@param      (o)MAGAZINE_PARAM& operationTm   TVM票箱参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetMagazineParam(MAGAZINE_PARAM& temParam)			
{
	temParam = m_magazine_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获得自动登出等待时间参数

@param      none

@retval     无操作登出单位秒

@exception  int   自动登出等待时间参数
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAutoLogoffTime()
{
	return m_maintain_param.autoLogoffTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获得密码输入时间参数

@param      none

@retval     密码输入时间			单位秒

@exception  int   自动登出等待时间参数
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetPwdInputTime()
{
	return m_maintain_param.pwdInputTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得密码登录尝试次数

@param      none

@retval     WORD 次数

@exception  int   自动登出等待时间参数
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetPwdInputMaxTimes(){
	return m_maintain_param.loginRetryTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      维修门未关闭报警时间

@param      none

@retval     单位秒

@exception  int   自动登出等待时间参数
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDoorUnlockAlarmTime()
{
	return m_maintain_param.doorUnlockAlarmTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获得所有支付方式ID(已废)

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
const vector<BYTE>& CAfcTVMOperationParam::GetAllPaymentID()
{
	vector<BYTE> vecPaymentID;	// 支付方式ID
	return vecPaymentID;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获得VM销售额上限（已废）

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CAfcTVMOperationParam::GetMaxSellValue()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得合并支付启用标志（已废）

@param      none

@retval     1：true  0：false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CAfcTVMOperationParam::GetCombinePayFlag()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售Card时可接收的纸币面值（以元为单位）

@param      vector<WORD>& vecFace 面值列表

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptBanknoteFaceOfIssueCard(vector<WORD>& vecFace)
{
	vecFace.clear();
	if (m_billbox_param.acceptSalesTicketBillValue1 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue1);
	if (m_billbox_param.acceptSalesTicketBillValue2 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue2);
	if (m_billbox_param.acceptSalesTicketBillValue3 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue3);
	if (m_billbox_param.acceptSalesTicketBillValue4 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue4);
	if (m_billbox_param.acceptSalesTicketBillValue5 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue5);
	if (m_billbox_param.acceptSalesTicketBillValue6 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue6);
	if (m_billbox_param.acceptSalesTicketBillValue7 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue7);
	if (m_billbox_param.acceptSalesTicketBillValue8 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue8);
	if (m_billbox_param.acceptSalesTicketBillValue9 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue9);
	if (m_billbox_param.acceptSalesTicketBillValue10 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售Card时可接收的纸币最大张数

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknoteMaxPieceOfIssueCard()
{
	return m_billbox_param.acceptSalesTicketMaxBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售单程票时可接收的纸币最大总额(单位为元)

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknoteMaxAmoutOfIssueCard()
{
	return m_billbox_param.acceptSalesTicketMaxBillAmt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将参数中的硬币面值代码转为以分为单位的面值

@param      BYTE code 面值代码

@retval     以分为单位的面值

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::ConvertCoinFace(BYTE code)
{
	switch (code)
	{
	case 5://五角
		return 50;
	case 10://一元
		return 100;
	default:
		return 0;
		/*case 3:
		return 200;
		case 4:
		return 500;*/
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否支持指定面值的硬币（以AVOS为单位）

@param      (i)WORD face 面值

@retval     bool

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportCoinFaceOfIssue(WORD face)
{
	vector<WORD> vecFace;
	GetAcceptCoinFaceOfIssue(vecFace);
	for (vector<WORD>::iterator iter = vecFace.begin(); iter != vecFace.end(); ++iter)
	{
		if (face == *iter) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售票时可接收的硬币面值（以分为单位）

@param      vector<WORD>& vecFace 面值列表

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptCoinFaceOfIssue(vector<WORD>& vecFace)
{
	vecFace.clear();

	WORD face = 0;
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue1);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue2);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue3);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue4);
	if (face != 0)
		vecFace.push_back(face);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售票时可接收的硬币最大枚数

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptCoinMaxPieceOfIssue()
{
	return m_billbox_param.acceptCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取售票时可找零的硬币最大枚数

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChageCoinMaxPieceOfIssue()
{
	return m_billbox_param.chargeCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptBanknotesFaceOfRecharge(vector<WORD>& vecFace){
	vecFace.clear();
	if (m_billbox_param.acceptChrgrBillValue1 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue1);
	}
	if (m_billbox_param.acceptChrgrBillValue2 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue2);
	}
	if (m_billbox_param.acceptChrgrBillValue3 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue3);
	}
	if(m_billbox_param.acceptChrgrBillValue4 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue4);
	}
	if(m_billbox_param.acceptChrgrBillValue5 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue5);
	}
	if(m_billbox_param.acceptChrgrBillValue6 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue6);
	}
	if (m_billbox_param.acceptChrgrBillValue7 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue7);
	}
	if (m_billbox_param.acceptChrgrBillValue8 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue8);
	}
	if (m_billbox_param.acceptChrgrBillValue9 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue9);
	}
	if(m_billbox_param.acceptChrgrBillValue10 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue10);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      充值时接收纸币最大数量

@param      none

@retval     WORD 接收纸币数量

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknotesMaxPieceOfRecharge(){
	return m_billbox_param.acceptChrgrMaxBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      充值时接收纸币最大总额(单位为元)

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknotesMaxAmountOfRecharge(){
	return m_billbox_param.acceptChrgrMaxBillAmt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      充值接收硬币面额

@param      (o)vector<WORD>& vecFace

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptCoinFaceOfRecharge(vector<WORD>& vecFace){
	vecFace.clear();
	WORD face = 0;
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue1);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue2);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue3);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue4);
	if (face != 0)
		vecFace.push_back(face);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取充值接收硬币的最大枚数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptCoinMaxPieceOfRecharge(){
	return m_billbox_param.acceptCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获取充值时找零的最大枚数 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChageCoinMaxPieceOfRecharge(){
	return m_billbox_param.chargeCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		是否显示打印按钮  

@param      none

@retval     bool:  true显示  false 不显示 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportPrintSwitch(){
	return true;//m_function_param.PrintSwitch;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取找零箱A 将空枚数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChangeCoinBoxNearEmptyCount(){
	return m_billbox_param.coinChangeWillEmptyCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取回收箱A 将满枚数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetRecycleBoxNearFullCount(){
	return m_billbox_param.coinCollectionWillFullCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取回收箱A 已满枚数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetRecycleBoxFullCount(){
	return m_billbox_param.coinCollectionAlreadyFullCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		纸币箱将满值  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknotesNearFullCount(){
	return m_billbox_param.willFullBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		纸币箱已满值  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknotesFullConut(){
	return m_billbox_param.alreadyFullBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取Ticket将空张数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketNearEmptyCount(){
	return m_magazine_param.TicketNearEmptyValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取Ticket已空张数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketEmptyCount(){
	return m_magazine_param.TicketEmptyValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取Ticket废票箱将满张数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketInvalidNearFullCount(){
	return m_magazine_param.TicketInvalidNearFullValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取Ticket废票箱已满张数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketInvalidFullCount(){
	return m_magazine_param.TicketInvalidFullValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取交易超时时间 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetCancelDealTime(){
	return m_billbox_param.cancelDealTime > 0 ? m_billbox_param.cancelDealTime : 60;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取等待中到休息中状态时间  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetReadyToRestTime(){
	return m_billbox_param.changeWaitScreenTime;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      无硬币找零是否允许售票

@param      无

@retval     bool true（允许）；false（不允许）

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportSaleWithoutCoinChange(){
	return m_billbox_param.salesNoCoin;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      售票是否允许纸币找零

@param      无

@retval     bool true允许； false不允许。

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportBHChangeWithSale(){
	return m_billbox_param.chargeBill;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零最大总金额（单位：元）

@param      无

@retval     WORD 允许纸币找零的最大金额

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBHChangeMaxAmount(){
	return m_billbox_param.chargeBillMaxAmt;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      是否启用招援按钮

@param      无

@retval     bool true：启用招援按钮
				 false:不启用

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::IsEnableAssistanceBtn()
{
	if (0 == m_billbox_param.IsEnableAssistanceBtn)
		return false;
	else
		return true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零最大张数

@param      无

@retval     WORD：张数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBHChangeMaxPiece(){
	return m_billbox_param.chargeBillMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取纸币找零箱将空数量

@param      无

@retval     WORD ：数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknoteChangeNearEmptyCount(){
	return m_billbox_param.willEmptyBillBoxCnt;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      等待中到待机中是否使用？

@param      无

@retval     bool true：使用  false：不使用

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::IsReadyToRestUsed(){
	return m_billbox_param.showWaitScreen;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取系统盘将满百分比数

@param      无

@retval     WORD (将满百分比%)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取系统盘将空值（MB）

@param      无

@retval     DWORD wDiskNearFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取系统盘已满百分比（%）

@param      无

@retval     WORD wDiskFullPersent;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取系统盘已满故障值（MB）

@param      无

@retval     WORD wDiskFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取数据盘将满百分比（%）

@param      无

@retval     WORD wDiskNearFullPersent;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取数据盘将空值（MB）

@param      无

@retval     WORD wDiskNearFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取数据盘已满百分比（%）

@param      无

@retval     WORD wDiskFullPersent;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取数据盘已满值（MB）

@param      无

@retval     WORD wDiskFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取日志盘将满百分比（%）

@param      无

@retval     WORD wDiskNearFullPersent;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取日志盘将满值（MB）

@param      无

@retval     WORD wDiskNearFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取日志盘已满故障值百分比（%）

@param      无

@retval     WORD wDiskFullPersent；

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取日志盘已满故障值（MB）

@param      无

@retval     WORD wDiskFullValue;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      日志保留天数

@param      无

@retval     WORD 天

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetMaxLogSaveDays(){
	return m_txn_lookup_param.logSaveDay;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      脱机运营最大天数

@param      无

@retval     WORD 天

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetOfflineWorkDays(){
	return m_txn_lookup_param.maxOffLineDay;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      已上传交易保留天数

@param      无

@retval     WORD 天数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSendedMsgSaveDays(){
	return m_txn_lookup_param.expiredTransKeepDay;
}