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
	//设置当次登录信息
	void SetCurrentLogin(CString userID,_DATE_TIME loginTime);
	void ClearCurrentLogin();
	void SetCurrentLoginAuditTo(_DATE_TIME auditToTime);
	//设置当天运营日
	void SetCurrentBusinessDay(_DATE currentDate);
	void ClearCurrentBusinessDay();
	void SetCurrentDayAuditTo(_DATE_TIME auditToTime);
	//取得当次登录的收益情况
	PurchaseData& GetCurrentLoginPurchase();
	//取得当天的收益情况
	PurchaseData& GetCurrentDayPurchase();
	//增加当次登录的收益情况
	void AddLoginPurchase(SCAuditKey&, SCAuditValue&);

	//设置当次登录的收益情况为已发给服务器
	void SetCurrentLoginPurchaseSended();
	//设置当天收益情况为已发给服务器
	void SetCurrentDayPurchaseSended();
	//取得当天当前用户所有未打印的收益情况
	//void GetCurrentUserUnPrintPurchase(PURCHASE_DATA&);
	//设置当天当前用户所有未打印状态为已打印
	void SetCurrentUserPurchasePrinted();

	//
	void LoadUnSendLoginPurchase(vector<PurchaseData>& unsendLoginPurchase);
	void LoadUnSendDayPurchase(vector<PurchaseData>& unSendDayPurchase);

	//void LoadLoginPurchase(CString userID,_DATE_TIME loginTime,PurchaseData& purchaseData);
	void SetLoginPurchaseSended(CString userID,_DATE_TIME loginTime);
	void SetDayPurchaseSended(_DATE& businessDate);
private:
	//增加当天的收益情况
	void AddDayPurchase(SCAuditKey&, SCAuditValue&);
	//保存收益情况到文件
	bool SavePurchaseDataToFile(CString strFileName, CString subFolderName, PurchaseData& purchaseData);
	//从文件中加载收益情况
	bool LoadPurchaseDataFromFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData);
	// 设定SC个人小计数据已送标志
	void SetPersonalPurchaseSended(CString strFileName);																																																					
	// 设定SC日计数据已送标志
	void SetDayPurchaseSended(CString strFileName);	
	// 设置SC个人小计打印标志
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


