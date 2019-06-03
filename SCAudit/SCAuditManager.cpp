#include "stdafx.h"
#include "SCAuditManager.h"
#include "sync.h"
#include "Util.h"
#include <memory>
#include "Xfile.h"
#include "BomInfo.h"
#include "SCAuditDef.h"
#include "CommonFunc.h"
#include "ResuceOperation.h"
#include "CommonDefine.h"
#include "..\BOM\ServiceDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
/*
��̬ʵ��
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager CSCAuditManager::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager::CSCAuditManager()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager::~CSCAuditManager()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ȡ�ø���Ψһʵ��   

@param      ��

@retval   CSCAuditManager& ����ʵ������  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager& CSCAuditManager::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��
@retval     bool		true:�ɹ�  false:ʧ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::Initilize()
{
	//ȷ��Ŀ¼���ڣ������ڣ��򴴽�
	if(!CResuceOperation::CheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PURCHASE_MANAGE_DAY_FILE_PATH,theBOM_INFO.GetBackupPath() + PURCHASE_MANAGE_DAY_FILE_PATH))
	{
		throw CSCAuditException(CSCAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("�½�������Ŀ¼�쳣��") +CString(PURCHASE_MANAGE_DAY_FILE_PATH)); 
	}
	if(!CResuceOperation::CheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PURCHASE_MANAGE_PERSONAL_FILE_PATH,theBOM_INFO.GetBackupPath() + PURCHASE_MANAGE_PERSONAL_FILE_PATH))
	{
		throw CSCAuditException(CSCAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("�½�������Ŀ¼�쳣��") +CString(PURCHASE_MANAGE_PERSONAL_FILE_PATH)); 
	}

	//ɾ����������
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	COleDateTime currentDate(sysTime);
	currentDate.SetDateTime(currentDate.GetYear(),currentDate.GetMonth(),currentDate.GetDay(),0,0,0);

	COleDateTimeSpan saveDaySpan = COleDateTimeSpan( theBOM_INFO.GetStoreDay(),0,0,0);
	COleDateTime saveFileFromDate=currentDate-saveDaySpan;

	vector<CString> sendSCFileNames;
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),_DATE(),PRINT_ALL,SEND_KIND::SEND_DATA);
	SearchDayPurchaseFile(sendSCFileNames,searchString);
	
	for(vector<CString>::iterator sendSCFileIter = sendSCFileNames.begin();sendSCFileIter!=sendSCFileNames.end();sendSCFileIter++)
	{
		_DATE_TIME sendTime;
		_DATE businessDate;
		PRINT_KIND printKind;
		SEND_KIND sendKind;
		bool needDeleteThisFile = true;
		CString sendSCFileName = *sendSCFileIter;
		if(TryParseDayPurchaseFileName(sendSCFileName,sendTime, businessDate,printKind,sendKind))
		{
			COleDateTime businessOleDate = COleDateTime(businessDate.wYear,businessDate.biMonth,businessDate.biDay,0,0,0);
			if(businessOleDate>saveFileFromDate)
			{
				needDeleteThisFile = false;
			}
		}
		if(needDeleteThisFile)
		{
			if(!CResuceOperation::DeleteResuceFile(theBOM_INFO.GetDataPath()+PURCHASE_MANAGE_DAY_FILE_PATH+sendSCFileName,theBOM_INFO.GetBackupPath()+PURCHASE_MANAGE_DAY_FILE_PATH+sendSCFileName))
			{
				throw CSCAuditException(CSCAuditException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("ɾ��δ�������ļ��쳣��")+sendSCFileName); 
			}
		}
	}
	vector<CString> sendSCPFileNames;
	CString searchSCPString = MakePersonalPurchaseSearchFileString(_T(""),_DATE_TIME(),_DATE(),PRINT_ALL,SEND_KIND::SEND_DATA);
	SearchPersonalPurchaseFile(sendSCPFileNames,searchSCPString);

	for(vector<CString>::iterator sendSCPFileIter = sendSCPFileNames.begin();sendSCPFileIter!=sendSCPFileNames.end();sendSCPFileIter++)
	{
		_DATE_TIME sendTime;
		_DATE businessDate;
		PRINT_KIND printKind;
		SEND_KIND sendKind;
		bool needDeleteThisFile = true;
		CString userID = _T("");
		CString sendSCPFileName = *sendSCPFileIter;
		if(TryParsePersonalPurchaseFileName(sendSCPFileName,userID,sendTime, businessDate,printKind,sendKind))
		{
			COleDateTime businessOleDate = COleDateTime(businessDate.wYear,businessDate.biMonth,businessDate.biDay,0,0,0);
			if(businessOleDate>saveFileFromDate)
			{
				needDeleteThisFile = false;
			}
		}
		if(needDeleteThisFile)
		{
			if(!CResuceOperation::DeleteResuceFile(theBOM_INFO.GetDataPath()+PURCHASE_MANAGE_PERSONAL_FILE_PATH+sendSCPFileName,theBOM_INFO.GetBackupPath()+PURCHASE_MANAGE_PERSONAL_FILE_PATH+sendSCPFileName))
			{
				throw CSCAuditException(CSCAuditException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("ɾ��δ�������ļ��쳣��")+sendSCPFileName); 
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentBusinessDay(_DATE currentDate)
{
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),currentDate,PRINT_ALL,NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchDayPurchaseFile(fileNames,searchString);
	if(fileNames.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ս��ļ���ֹһ��:")+searchString); 
	}
	currentBusinessDate = currentDate;
	if(fileNames.size()==1)
	{
		LoadPurchaseDataFromFile(fileNames[0],PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
	}
	else
	{
		currentDayPurchase = PurchaseData();
		currentDayPurchase.auditTimeFrom = ComGetCurTime();
		currentDayPurchase.auditTimeTo = ComGetCurTime();
		currentDayPurchase.businessDate = ComGetBusiDate();
		currentDayPurchase.staffID = "000000";
		CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
		SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CSCAuditManager::LoadLoginPurchase(CString userID,_DATE_TIME loginTime,PurchaseData& purchaseData)
//{
//	CString searchString = MakePersonalPurchaseSearchFileString(userID,loginTime,_DATE(),PRINT_ALL,NOT_SEND_DATA);
//	vector<CString> fileNames;
//	SearchPersonalPurchaseFile(fileNames,searchString);
//	if(fileNames.size()>1)
//	{
//		throw CSCAuditException(CSCAuditException::LOGIC_ERR, __FILE__, __LINE__, _T("δ�͸���С���ļ���ֹһ��:"+searchString)); 
//	}
//	if(fileNames.size()==1)
//	{
//		LoadPurchaseDataFromFile(fileNames[0],PURCHASE_MANAGE_PERSONAL_FILE_PATH,purchaseData);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::LoadUnSendLoginPurchase(vector<PurchaseData>& unsendLoginPurchase)
{
	CString searchString = MakePersonalPurchaseSearchFileString(_T(""),_DATE_TIME(),_DATE(),PRINT_ALL,NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchPersonalPurchaseFile(fileNames,searchString);
	for (int i=0;i<fileNames.size();i++)
	{
		PurchaseData purchaseData;
		LoadPurchaseDataFromFile(fileNames[i],PURCHASE_MANAGE_PERSONAL_FILE_PATH,purchaseData);
		unsendLoginPurchase.push_back(purchaseData);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::LoadUnSendDayPurchase(vector<PurchaseData>& unSendDayPurchase)
{
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),_DATE(),PRINT_ALL,NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchDayPurchaseFile(fileNames,searchString);
	for (int i=0;i<fileNames.size();i++)
	{
		PurchaseData purchaseData;
		LoadPurchaseDataFromFile(fileNames[i],PURCHASE_MANAGE_DAY_FILE_PATH,purchaseData);
		unSendDayPurchase.push_back(purchaseData);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetLoginPurchaseSended(CString userID,_DATE_TIME loginTime)
{
	CString searchString = MakePersonalPurchaseSearchFileString(userID,loginTime,_DATE(),PRINT_KIND::PRINT_ALL,SEND_KIND::NOT_SEND_DATA);
	vector<CString> currentLoginUnsendFiles;
	SearchPersonalPurchaseFile(currentLoginUnsendFiles,searchString);
	if(currentLoginUnsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ�͸���С���ļ���ֹһ��:")+searchString); 
	}
	if(currentLoginUnsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("û��δ�͸���С���ļ�:")+searchString); 
	}
	SetPersonalPurchaseSended(currentLoginUnsendFiles[0]);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetDayPurchaseSended(_DATE& businessDate)
{
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),businessDate,PRINT_KIND::PRINT_ALL,SEND_KIND::NOT_SEND_DATA);
	vector<CString> unsendFiles;
	SearchDayPurchaseFile(unsendFiles,searchString);
	if(unsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ս��ļ���ֹһ��:")+searchString); 
	}
	if(unsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("û��δ���ս��ļ�:")+searchString); 
	}

	SetDayPurchaseSended(unsendFiles[0]);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentLogin(CString userID,_DATE_TIME loginTime)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ������Ӫ��" )); 
	}
	CString searchString = MakePersonalPurchaseSearchFileString(userID,loginTime,currentBusinessDate,PRINT_ALL,NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchPersonalPurchaseFile(fileNames,searchString);
	if(fileNames.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ�͸���С���ļ���ֹһ��:"+searchString)); 
	}
	if(fileNames.size()==1)
	{
		LoadPurchaseDataFromFile(fileNames[0],PURCHASE_MANAGE_PERSONAL_FILE_PATH,currentPersonalPurchase);
	}
	else
	{
		currentPersonalPurchase = PurchaseData();
		currentPersonalPurchase.loginTime = loginTime;
		currentPersonalPurchase.auditTimeFrom = loginTime;
		currentPersonalPurchase.auditTimeTo = loginTime;
		currentPersonalPurchase.businessDate = ComGetBusiDate();
		currentPersonalPurchase.staffID = userID;
		currentPersonalPurchase.purchaseItems.clear();
		CString fileName = MakePersonalPurchaseFileName(userID,loginTime,currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
		SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_PERSONAL_FILE_PATH,currentPersonalPurchase);
	}
	this->userID = userID;
	this->loginTime = loginTime;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PurchaseData& CSCAuditManager::GetCurrentDayPurchase()
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ������Ӫ��" )); 
	}
	return currentDayPurchase;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PurchaseData& CSCAuditManager::GetCurrentLoginPurchase()
{
	if(loginTime==_DATE_TIME())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ò���Ա����¼ʱ" )); 
	}
	return currentPersonalPurchase;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::AddLoginPurchase(SCAuditKey& purchaseKey, SCAuditValue& purchaseValue)
{
	if(loginTime==_DATE_TIME())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ò���Ա����¼ʱ" )); 
	}
	currentPersonalPurchase.auditTimeTo = ComGetCurTime();
	currentPersonalPurchase.purchaseItems[purchaseKey] += purchaseValue;

	
	if(purchaseKey.serviceID != ADJUST_SVC)
	{
		currentPersonalPurchase.incomeAmount += purchaseValue.depositAmount ;//��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentPersonalPurchase.incomeAmount += purchaseValue.cardCostAmount;
	currentPersonalPurchase.incomeAmount += purchaseValue.serviceFeeAmount ;
	currentPersonalPurchase.incomeAmount +=purchaseValue.transAmount;

	
	if(purchaseKey.serviceID!=ADJUST_SVC)
	{
		currentPersonalPurchase.receivableAmount+=purchaseValue.depositAmount>0 ? purchaseValue.depositAmount : 0; //��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentPersonalPurchase.receivableAmount+=purchaseValue.cardCostAmount>0 ? purchaseValue.cardCostAmount : 0;
	currentPersonalPurchase.receivableAmount+=purchaseValue.serviceFeeAmount>0 ? purchaseValue.serviceFeeAmount : 0;
	currentPersonalPurchase.receivableAmount+=purchaseValue.transAmount>0 ? purchaseValue.transAmount : 0;

	
	if(purchaseKey.serviceID!=ADJUST_SVC)
	{
		currentPersonalPurchase.refundAmount+=purchaseValue.depositAmount<0 ? purchaseValue.depositAmount : 0;		//��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentPersonalPurchase.refundAmount+=purchaseValue.cardCostAmount<0 ? purchaseValue.cardCostAmount : 0;
	currentPersonalPurchase.refundAmount+=purchaseValue.serviceFeeAmount<0 ? purchaseValue.serviceFeeAmount : 0;
	currentPersonalPurchase.refundAmount+=purchaseValue.transAmount<0 ? purchaseValue.transAmount : 0;

	currentPersonalPurchase.recyleTicketCount +=purchaseValue.recyleCardNum;
	
	if(purchaseKey.cardIssuancer == ACC && purchaseKey.serviceID == REFUND_SVC)
	{
		currentPersonalPurchase.accRecyleTicketCount += purchaseValue.recyleCardNum;
		currentPersonalPurchase.accRecyleTicketAmount+=purchaseValue.transAmount<0 ? purchaseValue.transAmount : 0;
		currentPersonalPurchase.accRecyleTicketAmount+=purchaseValue.depositAmount<0 ? purchaseValue.depositAmount : 0;
	}

	CString fileName = MakePersonalPurchaseFileName(userID,loginTime,currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_PERSONAL_FILE_PATH,currentPersonalPurchase);
	
	AddDayPurchase(purchaseKey,purchaseValue);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentLoginAuditTo(_DATE_TIME auditToTime)
{
	if(loginTime==_DATE_TIME())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ò���Ա����¼ʱ" )); 
	}
	currentPersonalPurchase.auditTimeTo = auditToTime;
	CString fileName = MakePersonalPurchaseFileName(userID,loginTime,currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_PERSONAL_FILE_PATH,currentPersonalPurchase);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::AddDayPurchase(SCAuditKey& purchaseKey, SCAuditValue& purchaseValue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ������Ӫ��" )); 
	}
	currentDayPurchase.auditTimeTo = ComGetCurTime();
	currentDayPurchase.purchaseItems[purchaseKey]+=purchaseValue;

	if(purchaseKey.serviceID != ADJUST_SVC)
	{
		currentDayPurchase.incomeAmount += purchaseValue.depositAmount ;//��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentDayPurchase.incomeAmount += purchaseValue.cardCostAmount;
	currentDayPurchase.incomeAmount += purchaseValue.serviceFeeAmount ;
	currentDayPurchase.incomeAmount +=purchaseValue.transAmount;


	if(purchaseKey.serviceID!=ADJUST_SVC)
	{
		currentDayPurchase.receivableAmount+=purchaseValue.depositAmount>0 ? purchaseValue.depositAmount : 0; //��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentDayPurchase.receivableAmount+=purchaseValue.cardCostAmount>0 ? purchaseValue.cardCostAmount : 0;
	currentDayPurchase.receivableAmount+=purchaseValue.serviceFeeAmount>0 ? purchaseValue.serviceFeeAmount : 0;
	currentDayPurchase.receivableAmount+=purchaseValue.transAmount>0 ? purchaseValue.transAmount : 0;


	if(purchaseKey.serviceID!=ADJUST_SVC)
	{
		currentDayPurchase.refundAmount+=purchaseValue.depositAmount<0 ? purchaseValue.depositAmount : 0;		//��Ʊ���ڿۿ�����ֶΣ����ֽ�
	}
	currentDayPurchase.refundAmount+=purchaseValue.cardCostAmount<0 ? purchaseValue.cardCostAmount : 0;
	currentDayPurchase.refundAmount+=purchaseValue.serviceFeeAmount<0 ? purchaseValue.serviceFeeAmount : 0;
	currentDayPurchase.refundAmount+=purchaseValue.transAmount<0 ? purchaseValue.transAmount : 0;

	currentDayPurchase.recyleTicketCount +=purchaseValue.recyleCardNum;

	if(purchaseKey.cardIssuancer == ACC && purchaseKey.serviceID == REFUND_SVC)
	{
		currentDayPurchase.accRecyleTicketCount += purchaseValue.recyleCardNum;
		currentDayPurchase.accRecyleTicketAmount+=purchaseValue.transAmount<0 ? purchaseValue.transAmount : 0;
		currentDayPurchase.accRecyleTicketAmount+=purchaseValue.depositAmount<0 ? purchaseValue.depositAmount : 0;
	}

	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentDayAuditTo(_DATE_TIME auditToTime)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ������Ӫ��" )); 
	}
	currentDayPurchase.auditTimeTo = auditToTime;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,PRINT_KIND::NOT_PRINT_DATA,SEND_KIND::NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::ClearCurrentLogin()
{
	loginTime = _DATE_TIME();
	userID = "";
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::ClearCurrentBusinessDay()
{
	currentBusinessDate = _DATE();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ��ε�¼���������Ϊ�ѷ���������
@param  
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentLoginPurchaseSended()
{
	if(loginTime==_DATE_TIME())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ò���Ա����¼ʱ" )); 
	}
	CString searchString = MakePersonalPurchaseSearchFileString(userID,loginTime,currentBusinessDate,PRINT_KIND::PRINT_ALL,SEND_KIND::NOT_SEND_DATA);
	vector<CString> currentLoginUnsendFiles;
	SearchPersonalPurchaseFile(currentLoginUnsendFiles,searchString);
	if(currentLoginUnsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ�͸���С���ļ���ֹһ��:"+searchString)); 
	}
	if(currentLoginUnsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("û��δ�͸���С���ļ�:"+searchString)); 
	}
	SetPersonalPurchaseSended(currentLoginUnsendFiles[0]);
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ����������Ϊ�ѷ���������
@param  
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentDayPurchaseSended()
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ������Ӫ��" )); 
	}
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),currentBusinessDate,PRINT_KIND::PRINT_ALL,SEND_KIND::NOT_SEND_DATA);
	vector<CString> currentDayUnsendFiles;
	SearchDayPurchaseFile(currentDayUnsendFiles,searchString);
	if(currentDayUnsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("δ���ս��ļ���ֹһ��:"+searchString)); 
	}
	if(currentDayUnsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("û��δ���ս��ļ�:"+searchString)); 
	}

	SetDayPurchaseSended(currentDayUnsendFiles[0]);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SC�������ݵ�SC�����ļ�
@param      CString				strFileName			SC���ƶ��ػ��������ļ�
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::SavePurchaseDataToFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA purchaseBinData;
	purchaseData.Serialize(purchaseBinData);
	CXFile purchaseFile(theBOM_INFO.GetDataPath()+subFolderName+strFileName,theBOM_INFO.GetBackupPath()+subFolderName+strFileName);
	CXFile::ERR_DETAITL writeResult = purchaseFile.WriteBinFileWithCrc(purchaseBinData);
	if(writeResult!=CXFile::FILE_OPERATION_OK)
	{
		throw CSCAuditException(CSCAuditException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("�ļ�д���쳣��") + strFileName); 
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SC�������ݴ�SC�����ļ�
@param      CString				strFileName			SC���ƶ��ػ��������ļ�
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::LoadPurchaseDataFromFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA purchaseBinData;
	CXFile purchaseFile(theBOM_INFO.GetDataPath()+subFolderName+strFileName,theBOM_INFO.GetBackupPath()+subFolderName+strFileName);
	purchaseFile.ReadBinFileWithCrc(purchaseBinData);
	purchaseData.Deserialize(purchaseBinData);
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ƴ���ս�����ļ��� [SC_Audit_YYYYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINTED)_SENDED(UNSEND)]
@retval     �ɹ���ʧ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CSCAuditManager::MakeDayPurchaseFileName(_DATE_TIME sendTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind)
{
	CString fileName = _T("SC_Audit_{SendTime}_{BusinessDate}_{PrintFlag}_{SendFlag}.dat");
	CString strSendTime =_T("");
	strSendTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),sendTime.wYear,sendTime.biMonth,sendTime.biDay,sendTime.biHour,sendTime.biMinute,sendTime.biSecond);
	fileName.Replace(_T("{SendTime}"),strSendTime);
	CString strDate = _T("");
	strDate.Format(_T("%.4d%.2d%.2d"),businessDate.wYear,businessDate.biMonth,businessDate.biDay);
	fileName.Replace(_T("{BusinessDate}"),strDate);
	CString strPrintFlag = printKind==NOT_PRINT_DATA?_T("UNPRINT"):_T("PRINTED");
	fileName.Replace(_T("{PrintFlag}"),strPrintFlag);
	CString strSendFlag = sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
	fileName.Replace(_T("{SendFlag}"),strSendFlag);
	return fileName;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CSCAuditManager::MakeDayPurchaseSearchFileString(_DATE_TIME sendTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind)
{
	CString searchFileNameString = _T("SC_Audit_{SendTime}_{BusinessDate}_{PrintFlag}_{SendFlag}.dat");
	if(sendTime == _DATE_TIME())
	{
		searchFileNameString.Replace(_T("{SendTime}"),_T("*"));
	}
	else
	{
		CString strDate = _T("");
		strDate.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),sendTime.wYear,sendTime.biMonth,sendTime.biDay,sendTime.biHour,sendTime.biMinute,sendTime.biSecond);
		searchFileNameString.Replace(_T("{SendTime}"),strDate);
	}
	if(businessDate==_DATE())
	{
		searchFileNameString.Replace(_T("{BusinessDate}"),_T("*"));
	}
	else
	{
		CString strDate = _T("");
		strDate.Format(_T("%.4d%.2d%.2d"),businessDate.wYear,businessDate.biMonth,businessDate.biDay);
		searchFileNameString.Replace(_T("{BusinessDate}"),strDate);
	}

	CString strPrintFlag = printKind== PRINT_ALL ? _T("*") : printKind==NOT_PRINT_DATA?_T("UNPRINT"):_T("PRINTED");
	searchFileNameString.Replace(_T("{PrintFlag}"),strPrintFlag);
	CString strSendFlag = sendKind == ALL_DATA ? _T("*") : sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
	searchFileNameString.Replace(_T("{SendFlag}"),strSendFlag);
	return searchFileNameString;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ս�����ļ��� [SC_Audit_YYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINT)_SENDED(UNSEND)]
@retval     �ɹ���ʧ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::TryParseDayPurchaseFileName(CString& fileName,_DATE_TIME& sendTime,_DATE& businessDate,PRINT_KIND& printKind,SEND_KIND& sendKind)
{
	CString fileNameWithExtention = GetFileNameWithoutExtentionFormFullPath(fileName);
	std::auto_ptr<CStringArray> arrStrings(SplitCString(fileNameWithExtention,_T("_")));
	if(arrStrings.get()==NULL || arrStrings->GetCount()!=6)
	{
		return false;
	}
	if(!sendTime.TryParse(arrStrings->ElementAt(2)))
	{
		return false;
	}
	if(!businessDate.TryParse(arrStrings->ElementAt(3)))
	{
		return false;
	}
	if(arrStrings->ElementAt(4).MakeUpper() == "PRINTED")
	{
		printKind = PRINT_KIND::PRINT_DATA;
	}
	else if(arrStrings->ElementAt(4).MakeUpper() == "UNPRINT")
	{
		printKind = PRINT_KIND::NOT_PRINT_DATA;
	}
	else
	{
		return false;
	}
	if(arrStrings->ElementAt(5).MakeUpper() == "SENDED")
	{
		sendKind = SEND_KIND::SEND_DATA;
	}
	else if(arrStrings->ElementAt(5).MakeUpper() == "UNSEND")
	{
		sendKind = SEND_KIND::NOT_SEND_DATA;
	}
	else
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CSCAuditManager::MakePersonalPurchaseSearchFileString(CString userID,_DATE_TIME loginTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind)
{
	CString searchFileNameString = _T("Purchase_Audit_{UserID}_{LoginTime}_{BusinessDate}_{PrintFlag}_{SendFlag}.dat");
	searchFileNameString.Replace(_T("{UserID}"),userID == _T("" )? _T("*"):userID);
	if(loginTime==_DATE_TIME())
	{
		searchFileNameString.Replace(_T("{LoginTime}"),_T("*"));
	}
	else
	{
		CString strLoginTim = _T("");
		strLoginTim.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),loginTime.wYear,loginTime.biMonth,loginTime.biDay,loginTime.biHour,loginTime.biMinute,loginTime.biSecond);
		searchFileNameString.Replace(_T("{LoginTime}"),strLoginTim);
	}
	if(businessDate==_DATE())
	{
		searchFileNameString.Replace(_T("{BusinessDate}"),_T("*"));
	}
	else
	{
		CString strDate = _T("");
		strDate.Format(_T("%.4d%.2d%.2d"),businessDate.wYear,businessDate.biMonth,businessDate.biDay);
		searchFileNameString.Replace(_T("{BusinessDate}"),strDate);
	}
	CString strPrintFlag = printKind== PRINT_ALL ? _T("*") : printKind==NOT_PRINT_DATA?_T("UNPRINT"):_T("PRINTED");
	searchFileNameString.Replace(_T("{PrintFlag}"),strPrintFlag);
	CString strSendFlag = sendKind == ALL_DATA ?_T( "*") : sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
	searchFileNameString.Replace(_T("{SendFlag}"),strSendFlag);
	return searchFileNameString;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ƴ�ɸ���С������ļ��� [Purchase_Audit_XXXXXXXX_YYYYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINTED)_SENDED(UNSEND)]
@retval     �ɹ���ʧ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CSCAuditManager::MakePersonalPurchaseFileName(CString userID,_DATE_TIME loginTime,_DATE businessDate,PRINT_KIND printKind,SEND_KIND sendKind)
{
	CString fileName = _T("Purchase_Audit_{UserID}_{LoginTime}_{BusinessDate}_{PrintFlag}_{SendFlag}.dat");
	fileName.Replace(_T("{UserID}"),userID);
	CString strLoginTime = _T("");
	strLoginTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),loginTime.wYear,loginTime.biMonth,loginTime.biDay,loginTime.biHour,loginTime.biMinute,loginTime.biSecond);
	fileName.Replace(_T("{LoginTime}"),strLoginTime);
	CString strBusinessDate=_T("");
	strBusinessDate.Format(_T("%.4d%.2d%.2d"),businessDate.wYear,businessDate.biMonth,businessDate.biDay);
	fileName.Replace(_T("{BusinessDate}"),strBusinessDate);
	CString strPrintFlag = printKind==NOT_PRINT_DATA?_T("UNPRINT"):_T("PRINTED");
	fileName.Replace(_T("{PrintFlag}"),strPrintFlag);
	CString strSendFlag = sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
	fileName.Replace(_T("{SendFlag}"),strSendFlag);
	return fileName;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������ļ��� [Purchase_Audit_XXXXXXXX_YYYYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINTED)_SENDED(UNSEND)]
@retval     �ɹ���ʧ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::TryParsePersonalPurchaseFileName(CString& fileName,CString& userID,_DATE_TIME& loginTime,_DATE& bussinessDate,PRINT_KIND& printKind,SEND_KIND& sendKind)
{
	CString fileNameWithExtention = GetFileNameWithoutExtentionFormFullPath(fileName);
	std::auto_ptr<CStringArray> arrStrings(SplitCString(fileNameWithExtention,_T("_")));
	if(arrStrings.get()==NULL||arrStrings->GetCount()!=7)
	{
		return false;
	}
	userID = arrStrings->ElementAt(2);
	if(!loginTime.TryParse(arrStrings->ElementAt(3)))
	{
		return false;
	}
	if(!bussinessDate.TryParse(arrStrings->ElementAt(4)))
	{
		return false;
	}
	if(arrStrings->ElementAt(5).MakeUpper() == "PRINTED")
	{
		printKind = PRINT_KIND::PRINT_DATA;
	}
	else if(arrStrings->ElementAt(5).MakeUpper() == "UNPRINT")
	{
		printKind = PRINT_KIND::NOT_PRINT_DATA;
	}
	else
	{
		return false;
	}
	if(arrStrings->ElementAt(6).MakeUpper() == "SENDED")
	{
		sendKind = SEND_KIND::SEND_DATA;
	}
	else if(arrStrings->ElementAt(6).MakeUpper() == "UNSEND")
	{
		sendKind = SEND_KIND::NOT_SEND_DATA;
	}
	else
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�÷���ָ��������ָ��Ŀ¼���ս��ļ�
@param    CString subFolderName ָ��Ŀ¼
@param    CString searchCondition
@retval     ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition)
{
	if(!CResuceOperation::CheckAndCreateDirectory(theBOM_INFO.GetDataPath() + subFolderName,theBOM_INFO.GetBackupPath() + subFolderName))
	{
		throw CSCAuditException(CSCAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("���ػ�Ŀ¼�쳣��") + subFolderName); 
	}

	vector<CString> dataPathFiles;
	::SearchFile(theBOM_INFO.GetDataPath() + subFolderName + searchCondition,dataPathFiles);

	for(vector<CString>::iterator dataPathFileIter = dataPathFiles.begin();dataPathFileIter!=dataPathFiles.end();dataPathFileIter++)
	{
		CString fileName = GetFileNameFromFullPath(*dataPathFileIter);
		CString mainFilePath = theBOM_INFO.GetDataPath()+subFolderName+fileName;
		CString backupFilePath = theBOM_INFO.GetBackupPath()+subFolderName+fileName;
		if(!CResuceOperation::ResuceFileWithCrc(mainFilePath,backupFilePath))
		{
			throw CSCAuditException(CSCAuditException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("��Ŀ¼�ļ�:") + mainFilePath+_T(",����Ŀ¼�ļ�:")+backupFilePath); 
		}
		vecFileNames.push_back(fileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�÷���ָ��������SC�ս��ļ�
@param    CString searchCondition
@retval     ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SearchDayPurchaseFile(vector<CString>& vecFileNames,CString searchCondition)
{
	SearchFile(vecFileNames,PURCHASE_MANAGE_DAY_FILE_PATH,searchCondition);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�÷���ָ��������SC����С���ļ�
@param    CString searchCondition
@retval     ��
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SearchPersonalPurchaseFile(vector<CString>& vecFileNames,CString searchCondition)
{
	SearchFile(vecFileNames,PURCHASE_MANAGE_PERSONAL_FILE_PATH,searchCondition);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �趨SC����С�������Ѵ�ӡ��־
@param		PURCHASE_SC_DATA	dtSCData				SC�����ļ�����
int					iRecoverFromBakCount	�ļ��ָ�����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetPersonalPurchaseSended(CString strFileName)
{
	CString userID;
	_DATE_TIME loginTime;
	_DATE businessDate;
	PRINT_KIND printKind;
	SEND_KIND sendKind;
	if(!TryParsePersonalPurchaseFileName(strFileName,userID,loginTime,businessDate,printKind,sendKind))
	{
		throw CSCAuditException(CSCAuditException::FILE_NAME_INVALID, _T(__FILE__), __LINE__, _T(" Ŀ���ļ���" )+ strFileName+_T("���ֲ��ɽ���")); 
	}
	if(sendKind!=SEND_KIND::NOT_SEND_DATA)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T(" �ļ���") + strFileName+_T("Ϊ�Ѵ�ӡ״̬")); 
	}
	sendKind = SEND_KIND::SEND_DATA;
	CString renameToFileName = MakePersonalPurchaseFileName(userID,loginTime,businessDate,printKind,sendKind);
	RenameFileName(PURCHASE_MANAGE_PERSONAL_FILE_PATH,strFileName,renameToFileName);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �趨SC����С�������Ѵ�ӡ��־
@param		PURCHASE_SC_DATA	dtSCData				SC�����ļ�����
int					iRecoverFromBakCount	�ļ��ָ�����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetDayPurchaseSended(CString strFileName)
{
	_DATE_TIME sendTime;
	CString userID;
	_DATE bussinessDate;
	PRINT_KIND printKind;
	SEND_KIND sendKind;
	if(!TryParseDayPurchaseFileName(strFileName,sendTime,bussinessDate,printKind,sendKind))
	{
		throw CSCAuditException(CSCAuditException::FILE_NAME_INVALID, _T(__FILE__), __LINE__, _T(" Ŀ���ļ���") + strFileName+_T("���ֲ��ɽ���")); 
	}
	if(sendKind!=SEND_KIND::NOT_SEND_DATA)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T(" �ļ���") + strFileName+_T("Ϊ�Ѵ�ӡ״̬")); 
	}
	sendKind = SEND_KIND::SEND_DATA;
	CString renameToFileName = MakeDayPurchaseFileName(ComGetCurTime(),bussinessDate,printKind,sendKind);
	RenameFileName(PURCHASE_MANAGE_DAY_FILE_PATH,strFileName,renameToFileName);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo)
{
	SYNC(FILE,strFileNameFrom);
	CString mainFilePathFrom = theBOM_INFO.GetDataPath()+subFolderName+strFileNameFrom;
	CString backupFilePathFrom = theBOM_INFO.GetBackupPath()+subFolderName+strFileNameFrom;
	CString mainFilePathTo = theBOM_INFO.GetDataPath()+subFolderName+strFileNameTo;
	CString backupFilePathTo = theBOM_INFO.GetBackupPath()+subFolderName+strFileNameTo;
	if(!ComCheckFileExist(mainFilePathFrom))
	{
		throw CSCAuditException(CSCAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" ���ļ���") + mainFilePathFrom+_T("������")); 
	}
	if(!ComCheckFileExist(backupFilePathFrom))
	{
		throw CSCAuditException(CSCAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" �����ļ���") + backupFilePathFrom+_T("������")); 
	}
	if(!CResuceOperation::RenameResuceFile(mainFilePathFrom,backupFilePathFrom,mainFilePathTo,backupFilePathTo))
	{
		throw CSCAuditException(CSCAuditException::RENAME_FILE_FAIL, _T(__FILE__), __LINE__, _T("���������ػ��ļ���") + strFileNameFrom+_T("Ϊ")+strFileNameTo+_T("ʧ��")); 
	}
}