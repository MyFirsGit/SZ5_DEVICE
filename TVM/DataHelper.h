#pragma once
#include "stdafx.h"
#include <vector>
#include "ServiceHelper.h"
#include "TransactionInfo.h"
#include "CoinAddOperationRecord.h"
#include "CoinCollectOperationRecord.h"
#include "CoinCollectBoxRemoveOperationRecord.h"
#include "CoinBoxCashDetail.h"
#include "CoinChangeBoxOperationRecord.h"
#include "BanknoteOperationRecord.h"
#include "BanknoteBoxInstallOperationRecord.h"
#include "BanknoteBoxRemoveOperationRecord.h"
#include "BanknoteBoxCashDetail.h"
#include "TokenBoxAddOperationRecord.h"
#include "TokenBoxInstallOperationRecord.h"
#include "TokenBoxRemoveOperationRecord.h"
#include "TokenCollectBoxRemoveOperationRecord.h"
#include "TokenCollectOperationRecord.h"
#include "CardBoxInstallOperationRecord.h"
#include "CardBoxRemoveOperationRecord.h"
#include "CardCollectBoxRemoveOperationRecord.h"

#define  TASK_ID_TASK_TYPE_BEGIN_NO			8		// 任务编号中任务类型的起始位
#define  TASK_ID_TASK_TYPE_LENGTH			2		// 任务编号中任务类型的长度
#define  TEMAPLE_ID_TEMAPLE_TYPE_BEGIN_NO	8		// 任务编号中任务类型的起始位
#define  TEMAPLE_ID_TEMAPLE_TYPE_LENGTH		2		// 任务编号中任务类型的长度
#define  TIME_BEGIN_NO						0		// 任务编号或模板编号中时间的起始位
#define  TIME_LENGTH						8		// 任务编号或模板编号中时间的长度
#define  DATABASE_SQL_FILE_NAME		    _T("TVM.sql")// SQL脚本文件名称

using namespace Data;
class CDataHelper
{
public:
	// ----------------------------------乘客业务数据库操作接口定义-----------------------------------
	// 保存交易信息
	long DbInsertTransactionInfo(TransactionRecord &transactionInfo);
	// 查询指定时间段的交易信息
	long DbSelectTransactionInfo(CString beginTime,CString endTime, vector<TransactionRecord>& vec_transactionInfo,bool bSelectDetail = true);
	// 查询最近的N条交易记录（根据交易时间）
	long DbSelectTopNumTransInfo(vector<TransactionRecord>& vec_transactionInfo,long lNum=100);
	// 查询指定时间段的硬币收纳记录
	long DbSelectCoinAcceptRecord(CString beginTime,CString endTime, vector<CoinAcceptRecord>& vec_CoinAcceptRecord);
	// 查询指定时间段的硬币找零记录
	long DbSelectCoinChangeRecord(CString beginTime,CString endTime, vector<CoinChangeRecord>& vec_CoinChangeRecord);
	// 查询指定时间段的纸币收纳记录
	long DbSelectBanknoteAcceptRecord(CString beginTime,CString endTime, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord);
	// 查询指定时间段的纸币找零记录
	long DbSelectBanknoteChangeRecord(CString beginTime,CString endTime, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord);
	// 查询指定时间段的故障记录
	long DbSelectTransFailureRecord(CString beginTime,CString endTime, vector<ExceptionRecord>& vec_TransFailureRecord);
	// 删除N天前的交易信息

	// 删除N天前的交易信息
	long DbDeleteTransactionInfoBeforeNdays(WORD dwDays);
	// 删除指定时间段的交易信息（预留，暂不使用）
	long DbDeleteTransactionInfo(CString beginTime,CString endTime);

    // ----------------------------------站员业务数据库操作接口定义-----------------------------------
	// 保存硬币找零箱补充操作记录
	long DbInsertCoinAddOperationRecord(CoinAddOperationRecord &coinAddOperationRecord);
	// 保存硬币回收操作记录
	long DbInsertCoinCollectOperationRecord(CoinCollectOperationRecord &coinCollectOperationRecord);
	// 保存硬币回收箱卸载操作记录
	long DbInsertCoinCollectBoxRemoveOperationRecord(CoinCollectBoxRemoveOperationRecord &coinCollectBoxRemoveOperationRecord);

	// 查询指定时间段的硬币投入信息
	long DbSelectCoinInsertRecord(CString beginTime,CString endTime,vector<CoinInsertRecord>& vec_CoinInsertRecord);

	// 查询指定时间段的硬币找零箱补充操作记录
	long DbSelectCoinAddOperationRecord(CString beginTime,CString endTime, vector<CoinAddOperationRecord>& vec_CoinAddOperationRecord);
	// 查询指定时间段的硬币回收操作记录
	long DbSelectCoinCollectOperationRecord(CString beginTime,CString endTime, vector<CoinCollectOperationRecord>& vec_CoinCollectOperationRecord);
	// 查询指定时间段的硬币回收箱卸载操作记录
	long DbSelectCoinCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CoinCollectBoxRemoveOperationRecord>& vec_CoinCollectBoxRemoveOperationRecord);
	// 删除N天数以前的所有硬币操作记录
	long DbDeleteCoinOperationRecordBeforeNdays(WORD dwDays);

	// 查询指定时间段硬币补充箱操作记录
	long DbSelectCoinChangeBoxOperationRecord(CString beginTime,CString endTime,vector<CoinChangeBoxOperationRecord>& vec_coinChangeboxOperationRecord);
	// 保存硬币找零箱操作记录
	long DbInsertCoinChangeBoxOperationRecord(CoinChangeBoxOperationRecord& coinChangeBoxOperationRecord);

	// 保存纸币找零箱补充操作记录
	long DbInsertBanknoteOperationRecord(BanknoteOperationRecord &banknoteAddOperationRecord);
	// 保存纸币回收箱安装操作记录
	long DbInsertBanknoteBoxInstallOperationRecord(BanknoteBoxInstallOperationRecord &banknoteBoxInstallOperationRecord);
	// 保存纸币卸载回收操作记录
	long DbInsertBanknoteBoxRemoveOperationRecord(BanknoteBoxRemoveOperationRecord &banknoteBoxRemoveOperationRecord);
	// 查询指定时间段的纸币找零箱补充操作记录
	long DbSelectBanknoteOperationRecord(CString beginTime,CString endTime, vector<BanknoteOperationRecord>& vec_BanknoteAddOperationRecord);
	// 查询指定时间段的纸币回收箱安装操作记录
	long DbSelectBanknoteBoxInstallOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxInstallOperationRecord>& vec_BanknoteBoxInstallOperationRecord);
	// 查询指定时间段的纸币卸载回收操作记录
	long DbSelectBanknoteBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxRemoveOperationRecord>& vec_BanknoteBoxRemoveOperationRecord);
	// 删除N天数以前的所有纸币操作记录
	long DbDeleteBanknoteOperationRecordBeforeNdays(WORD dwDays);

	// 保存Token箱安装操作记录(预留)
	long DbInsertTokenBoxInstallOperationRecord(TokenBoxInstallOperationRecord &tokenBoxInstallOperationRecord);
	// 保存Token箱卸载操作记录(预留)
	long DbInsertTokenBoxRemoveOperationRecord(TokenBoxRemoveOperationRecord &tokenBoxRemoveOperationRecord);
	// 保存Token箱补充操作记录
	long DbInsertTokenBoxAddOperationRecord(TokenBoxAddOperationRecord &tokenBoxAddOperationRecord);
	// 保存Token回收操作记录
	long DbInsertTokenCollectOperationRecord(TokenCollectOperationRecord &tokenBoxAddOperationRecord);
	// 保存Token废票箱卸载操作记录
	long DbInsertTokenCollectBoxRemoveOperationRecord(TokenCollectBoxRemoveOperationRecord &tokenCollectBoxRemoveOperationRecord);
	// 查询指定时间段的Token箱安装操作记录(预留)
	long DbSelectTokenBoxInstallOperationRecord(CString beginTime,CString endTime, vector<TokenBoxInstallOperationRecord>& vec_TokenBoxInstallOperationRecord);
	// 查询指定时间段的Token箱卸载操作记录(预留)
	long DbSelectTokenBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenBoxRemoveOperationRecord>& vec_TokenBoxRemoveOperationRecord);
	// 查询指定时间段的Token箱补充操作记录
	long DbSelectTokenBoxAddOperationRecord(CString beginTime,CString endTime, vector<TokenBoxAddOperationRecord>& vec_TokenBoxAddOperationRecord);
	// 查询指定时间段的Token回收操作记录
	long DbSelectTokenCollectOperationRecord(CString beginTime,CString endTime, vector<TokenCollectOperationRecord>& vec_TokenCollectOperationRecord);
	// 查询指定时间段的Token废票箱卸载操作记录
	long DbSelectTokenCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenCollectBoxRemoveOperationRecord>& vec_TokenCollectBoxRemoveOperationRecord);
	// 删除N天数以前的所有Token操作记录
	long DbDeleteTokenOperationRecordBeforeNdays(WORD dwDays);


	// 保存Card箱安装操作记录
	long DbInsertCardBoxInstallOperationRecord(CardBoxInstallOperationRecord &cardBoxInstallOperationRecord);
	// 保存Card箱卸载操作记录
	long DbInsertCardBoxRemoveOperationRecord(CardBoxRemoveOperationRecord &cardBoxRemoveOperationRecord);
	// 保存Card废票箱卸载操作记录
	long DbInsertCardCollectBoxRemoveOperationRecord(CardCollectBoxRemoveOperationRecord &cardCollectBoxRemoveOperationRecord);
	// 查询指定时间段的Card箱安装操作记录
	long DbSelectCardBoxInstallOperationRecord(CString beginTime,CString endTime, vector<CardBoxInstallOperationRecord>& vec_CardBoxInstallOperationRecord);
	// 查询指定时间段的Card箱卸载操作记录
	long DbSelectCardBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardBoxRemoveOperationRecord>& vec_CardBoxRemoveOperationRecord);
	// 查询指定时间段的Card废票箱卸载操作记录
	long DbSelectCardCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardCollectBoxRemoveOperationRecord>& vec_CardCollectBoxRemoveOperationRecord);
	
	// 删除N天数以前的所有Card操作记录
	long DbDeleteCardOperationRecordBeforeNdays(WORD dwDays);


public:
	CDataHelper();                                 // 构造函数
	~CDataHelper();                                                 // 析构函数
	long DbInitialize();											// 该函数执行，数据库文件的二重化，如果主数据库文件和备份数据库文件都不存在，则需要执行创建数据库并初始化数据。二重化成功后，需要打开数据库的连接。
	//没有调用
	//long DbClose();												// 关闭数据库的连接。


private:
	// ----------------------------------乘客业务数据库操作接口定义-----------------------------------
	// 查询指定交易的硬币投入操作明细
	long DbSelectCoinInsertRecord(int  iTransID, vector<CoinInsertRecord>& vec_CoinInsertRecord);
	// 查询指定交易的硬币找零操作明细
	long DbSelectCoinChangeRecord(int iTransID, vector<CoinChangeRecord>& vec_CoinChangeRecord);
	// 查询指定交易的硬币收纳操作明细
	long DbSelectCoinAcceptRecord(int iTransID, vector<CoinAcceptRecord>& vec_CoinAcceptRecord);
	// 查询指定交易的纸币投入操作明细
	long DbSelectBanknoteInsertRecord(int iTransID, vector<BanknoteInsertRecord>& vec_BanknoteInsertRecord);
	// 查询指定交易的纸币找零操作明细（预留）
	long DbSelectBanknoteChangeRecord(int iTransID, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord);
	// 查询指定交易的纸币收纳操作明细
	long DbSelectBanknoteAcceptRecord(int iTransID, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord);
	// 查询指定交易的纸币废币操作明细（预留）
	long DbSelectBanknoteDiscardRecord(int iTransID, vector<BanknoteDiscardRecord>& vec_BanknoteDiscardRecord);
	// 查询指定交易的故障明细
	long DbSelectExceptionRecord(int iTransID, vector<ExceptionRecord>& vec_ExceptionRecord);
	// 查询指定交易的票卡操作明细
	long DbSelectTicketOperationRecord(int iTransID, vector<TicketOperationRecord>& vec_TicketOperationRecord);

	public:
    // ----------------------------------站员业务数据库操作接口定义-----------------------------------
	// 查询硬币回收箱现金明细
	long DbSelectCoinBoxCashDetail(int iRecordID, vector<CoinBoxCashDetail>& vec_CoinBoxCashDetail);
	// 查询纸币回收箱现金明细
	long DbSelectBanknoteBoxCashDetail(int iRecordID, vector<BanknoteBoxCashDetail>& vec_BanknoteBoxCashDetail);
};