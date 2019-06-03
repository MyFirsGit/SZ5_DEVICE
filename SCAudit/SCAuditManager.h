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
	//���ڻ�
	bool Initilize();
	//�Ƿ�δ�ϴ����ݹ���
	bool HasTooMoreUnsendPurchase();
	//���õ��ε�¼��Ϣ
	void SetCurrentLogin(CString userID,_DATE_TIME loginTime);
	void ClearCurrentLogin();
	void SetCurrentLoginAuditTo(_DATE_TIME auditToTime);
	//���õ�����Ӫ��
	void SetCurrentBusinessDay(_DATE currentDate);
	void ClearCurrentBusinessDay();
	void SetCurrentDayAuditTo(_DATE_TIME auditToTime);
	//ȡ�õ��ε�¼���������
	PurchaseData& GetCurrentLoginPurchase();
	//ȡ�õ�����������
	PurchaseData& GetCurrentDayPurchase();
	//���ӵ��ε�¼���������
	void AddLoginPurchase(SCAuditKey&, SCAuditValue&);

	//���õ��ε�¼���������Ϊ�ѷ���������
	void SetCurrentLoginPurchaseSended();
	//���õ����������Ϊ�ѷ���������
	void SetCurrentDayPurchaseSended();
	//ȡ�õ��쵱ǰ�û�����δ��ӡ���������
	//void GetCurrentUserUnPrintPurchase(PURCHASE_DATA&);
	//���õ��쵱ǰ�û�����δ��ӡ״̬Ϊ�Ѵ�ӡ
	void SetCurrentUserPurchasePrinted();

	//
	void LoadUnSendLoginPurchase(vector<PurchaseData>& unsendLoginPurchase);
	void LoadUnSendDayPurchase(vector<PurchaseData>& unSendDayPurchase);

	//void LoadLoginPurchase(CString userID,_DATE_TIME loginTime,PurchaseData& purchaseData);
	void SetLoginPurchaseSended(CString userID,_DATE_TIME loginTime);
	void SetDayPurchaseSended(_DATE& businessDate);
private:
	//���ӵ�����������
	void AddDayPurchase(SCAuditKey&, SCAuditValue&);
	//��������������ļ�
	bool SavePurchaseDataToFile(CString strFileName, CString subFolderName, PurchaseData& purchaseData);
	//���ļ��м����������
	bool LoadPurchaseDataFromFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData);
	// �趨SC����С���������ͱ�־
	void SetPersonalPurchaseSended(CString strFileName);																																																					
	// �趨SC�ռ��������ͱ�־
	void SetDayPurchaseSended(CString strFileName);	
	// ����SC����С�ƴ�ӡ��־
	void SetPersonalPurchasePrinted(CString strFileName);

	CString MakePersonalPurchaseFileName(CString userID,_DATE_TIME loginTime,_DATE businessDate, PRINT_KIND printKind,SEND_KIND sendKind);
	bool TryParsePersonalPurchaseFileName(CString& fileName,CString& userID,_DATE_TIME& loginTime,_DATE& businessDate,PRINT_KIND& printKind,SEND_KIND& sendKind);
	CString MakePersonalPurchaseSearchFileString(CString userID=_T(""),_DATE_TIME loginTime=_DATE_TIME(),_DATE businessDate=_DATE(),PRINT_KIND printKind = PRINT_KIND::PRINT_ALL,SEND_KIND sendKind=SEND_KIND::ALL_DATA);
	void SearchPersonalPurchaseFile(vector<CString>& vecFileNames,CString searchCondition);

	CString MakeDayPurchaseFileName(_DATE_TIME sendTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind);
	bool TryParseDayPurchaseFileName(CString& fileName,_DATE_TIME& sendTime,_DATE& businessDate,PRINT_KIND& printKind,SEND_KIND& sendKind);
	CString MakeDayPurchaseSearchFileString(_DATE_TIME sendTime=_DATE_TIME(),_DATE businessDate=_DATE(),PRINT_KIND printKind = PRINT_KIND::PRINT_ALL,SEND_KIND sendKind=SEND_KIND::ALL_DATA);
	void SearchDayPurchaseFile(vector<CString>& vecFileNames,CString searchCondition);

	void SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition);

	void RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo);
	CSCAuditManager();
	~CSCAuditManager();

	static CSCAuditManager theInstance;
	PurchaseData currentPersonalPurchase;
	PurchaseData currentDayPurchase;
	CString userID;
	_DATE_TIME loginTime;
	_DATE currentBusinessDate;
};


