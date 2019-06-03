#pragma once

#include "SCAuditDef.h"
#include "SCAuditData.h"
#include "SCAuditException.h"
#include "SCAuditExp.h"

#define theSCAudit_MGR CSCAuditManager::GetInstance()

class SCAUDIT_API CSCAuditManager
{
public:
	static CSCAuditManager& GetInstance();
	//初期化
	bool Initilize();
	//是否未上传数据过多
	bool HasTooMoreUnsendPurchase();
	//设置当天运营日
	void SetCurrentBusinessDay(_DATE currentDate);
	void ClearCurrentBusinessDay();
	void SetCurrentDayAuditTo(_DATE_TIME auditToTime);
	//取得当天的收益情况
	PurchaseData& GetCurrentDayPurchase();

	//设置当天收益情况为已发给服务器
	void SetCurrentDayPurchaseSended();

	void LoadUnSendDayPurchase(vector<PurchaseData>& unSendDayPurchase);

	void	setSelledCardNum(long lvalue);
	void	addSelledCardNum(long lstep);
	void	setSelledCardSum(long lvalue);
	void	addSelledCardSum(long lstep);
	void	setCardRechargeNum(long lvalue);
	void	addCardRechargeNum(long lstep);
	void	setCardRechargeSum(long lvalue);
	void	addCardRechargeSum(long lstep);
	void	setFailBusinessNum(long lvalue);
	void	addFailBusinessNum(long lstep);
	void	setFailBusinessSum(long lvalue);
	void	addFailBusinessSum(long lstep);
	void	setCoinSupplementSum(long lvalue);
	void	addCoinSupplementSum(long lstep);
	void	setCoinRecoverSum(long lvalue);
	void	addCoinRecoverSum(long lstep);
	void	setCoinChangeSum(long lvalue);
	void	addCoinChangeSum(long lstep);
	void	setCoinLastLeftSum(long lvalue);
	void	addCoinLastLeftSum(long lstep);
	void	setCoinCurrentLeftSum(long lvalue);
	void	addCoinCurrentLeftSum(long lstep);
	void	setCoinIncomeSum(long lvalue);
	void	addCoinIncomeSum(long lstep);

	void	setNoteSupplementSum(long lvalue);
	void	addNoteSupplementSum(long lstep);
	void	setNoteRecoverSum(long lvalue);
	void	addNoteRecoverSum(long lstep);
	void	setNoteChangeSum(long lvalue);
	void	addNoteChangeSum(long lstep);
	void	setNoteLastLeftSum(long lvalue);
	void	addNoteLastLeftSum(long lstep);
	void	setNoteCurrentLeftSum(long lvalue);
	void	addNoteCurrentLeftSum(long lstep);
	void	setNoteIncomeSum(long lvalue);
	void	addNoteIncomeSum(long lstep);

	void	setBankCardIncomeSum(long lvalue);
	void	addBankCardIncomeSum(long lstep);
	void	setBankCardBusinessNum(long lvalue);
	void	addBankCardBusinessNum(long lstep);
	void	setWalletIncomeSum(long lvalue);
	void	addWalletIncomeSum(long lstep);
	void	setWalletBusinessNum(long lvalue);
	void	addWalletBusinessNum(long lstep);

	void	setTokenLastLeftNum(long lvalue);
	void	addTokenLastLeftNum(long lstep);
	void	setTokenCurrentInvalidatedNum(long lvalue);
	void	addTokenCurrentInvalidatedNum(long lstep);
	void	setTokenCurrentSupplementNum(long lvalue);
	void	addTokenCurrentSupplementNum(long lstep);
	void	setTokenCurrentSelledNum(long lvalue);
	void	addTokenCurrentSelledNum(long lstep);
	void	setTokenCurrentRecoverNum(long lvalue);
	void	addTokenCurrentRecoverNum(long lstep);

	void	setCardLastLeftNum(long lvalue);
	void	addCardLastLeftNum(long lstep);
	void	setCardCurrentInvalidateNum(long lvalue);
	void	addCardCurrentInvalidateNum(long lstep);
	void	setCardCurrrntSupplementNum(long lvalue);
	void	addCardCurrrntSupplementNum(long lstep);
	void	setCardCurrentSelledNum(long lvalue);
	void	addCardCurrentSelledNum(long lstep);
	void	setCardCurrentRecoverNum(long lvalue);
	void	addCardCurrentRecoverNum(long lstep);


	void SetDayPurchaseSended(_DATE& businessDate);
private:
	//保存收益情况到文件
	bool SavePurchaseDataToFile(CString strFileName, CString subFolderName, PurchaseData& purchaseData);
	//从文件中加载收益情况
	bool LoadPurchaseDataFromFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData);
	// 设定SC日计数据已送标志
	void SetDayPurchaseSended(CString strFileName);

	CString MakeDayPurchaseFileName(_DATE_TIME sendTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind);
	bool TryParseDayPurchaseFileName(CString& fileName,_DATE_TIME& sendTime,_DATE& businessDate,PRINT_KIND& printKind,SEND_KIND& sendKind);
	CString MakeDayPurchaseSearchFileString(_DATE_TIME sendTime=_DATE_TIME(),_DATE businessDate=_DATE(),PRINT_KIND printKind = /*PRINT_KIND::*/PRINT_ALL,SEND_KIND sendKind=/*SEND_KIND::*/ALL_DATA);
	void SearchDayPurchaseFile(vector<CString>& vecFileNames,CString searchCondition);

	void SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition);

	void RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo);
	CSCAuditManager();
	~CSCAuditManager();

	static CSCAuditManager theInstance;
	PurchaseData currentDayPurchase;
	_DATE currentBusinessDate;
};


