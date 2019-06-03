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
	//���ڻ�
	bool Initilize();

	//���õ�����Ӫ��
	void SetCurrentBusinessDay(_DATE currentDate);
	void ClearCurrentBusinessDay();

	//ȡ�õ�����������
	TxnAuditData& GetCurrentTxnAudit();

	//���õ����������Ϊ�ѷ���������
	void SetCurrentTxnAuditSended();

	void LoadUnSendTxnAudit(vector<TxnAuditData>& unSendDayPurchase);

	void SetTxnAuditSended(_DATE& businessDate);

	//���ӵ�����������
	void AddTxnAudit(TXNAuditKey&, TXNAuditValue&);

private:

	//��������������ļ�
	bool SaveTxnAuditDataToFile(CString strFileName, CString subFolderName, TxnAuditData& purchaseData);
	//���ļ��м����������
	bool LoadTxnAuditDataFromFile(CString strFileName,CString subFolderName, TxnAuditData& purchaseData);																																																	
	// �趨SC�ռ��������ͱ�־
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


