#pragma once

#include "TxnAuditDef.h"
#include "TxnAuditData.h"
#include "TxnAuditException.h"
#include "TxnAuditExp.h"
#include "..\SCAudit\SCAuditDef.h"

#define theTxnAudit_MGR CTxnAuditManager::GetInstance()

class TXNAUDIT_DECLSPEC CTxnAuditManager
{
public:
	static CTxnAuditManager& GetInstance();
	//初期化
	bool Initilize();

	//设置当天运营日
	void SetCurrentBusinessDay(_DATE currentDate);
	void ClearCurrentBusinessDay();

	//取得当天的收益情况
	TxnAuditData& GetCurrentTxnAudit();

	//设置当天收益情况为已发给服务器
	void SetCurrentTxnAuditSended();

	void LoadUnSendTxnAudit(vector<TxnAuditData>& unSendDayPurchase);

	void SetTxnAuditSended(_DATE& businessDate);

	//增加当天的收益情况
	void AddTxnAudit(TXNAuditKey&, TXNAuditValue&);

private:

	//保存收益情况到文件
	bool SaveTxnAuditDataToFile(CString strFileName, CString subFolderName, TxnAuditData& purchaseData);
	//从文件中加载收益情况
	bool LoadTxnAuditDataFromFile(CString strFileName,CString subFolderName, TxnAuditData& purchaseData);																																																	
	// 设定SC日计数据已送标志
	void SetTxnAuditSended(CString strFileName);	


 	CString MakeTxnAuditFileName(_DATE_TIME sendTime,_DATE businessDate,SEND_KIND sendKind);
 	bool TryParseTxnAuditFileName(CString& fileName,_DATE_TIME& sendTime,_DATE& businessDate,SEND_KIND& sendKind);
 	CString MakeTxnAuditSearchFileString(_DATE_TIME sendTime=_DATE_TIME(),_DATE businessDate=_DATE(),SEND_KIND sendKind=SEND_KIND::ALL_DATA);
 	void SearchTxnAuditFile(vector<CString>& vecFileNames,CString searchCondition);

	void SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition);

	void RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo);
	CTxnAuditManager();
	~CTxnAuditManager();

	static CTxnAuditManager theInstance;
	TxnAuditData currentTxnAudit;
	_DATE currentBusinessDate;
};


