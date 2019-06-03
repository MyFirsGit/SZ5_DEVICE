#include "stdafx.h"
#include "TxnAuditManager.h"
#include "sync.h"
#include "Util.h"
#include <memory>
#include "Xfile.h"
#include "BomInfo.h"
#include "TxnAuditDef.h"
#include "CommonFunc.h"
#include "ResuceOperation.h"
#include "CommonDefine.h"
#include "SCAuditDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = _T(__FILE__);
#endif

using namespace std;
//
////////////////////////////////////////////////////////////////////////////
///*
//静态实例
//*/
////////////////////////////////////////////////////////////////////////////
CTxnAuditManager CTxnAuditManager::theInstance;

////////////////////////////////////////////////////////////////////////////
///*
//@brief      构造函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
CTxnAuditManager::CTxnAuditManager()
{
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      析构函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
CTxnAuditManager::~CTxnAuditManager()
{
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief   取得该类唯一实例   
//
//@param      无
//
//@retval   CTxnAuditManager& 对象实例引用  
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
CTxnAuditManager& CTxnAuditManager::GetInstance()
{
	return theInstance;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      初始化
//@retval     bool		true:成功  false:失败
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
bool CTxnAuditManager::Initilize()
{
	//确认目录存在，不存在，则创建
	if(!CResuceOperation::CheckAndCreateDirectory(theBOM_INFO.GetDataPath() + TXN_MANAGE_DAY_FILE_PATH,theBOM_INFO.GetBackupPath() + TXN_MANAGE_DAY_FILE_PATH))
	{
		throw CTxnAuditException(CTxnAuditException::FOLDER_CREATE_FAIL, _T(__FILE__),  __LINE__, _T("新建主数据目录异常：") +CString(TXN_MANAGE_DAY_FILE_PATH)); 
	}
	//删除已送数据
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	COleDateTime currentDate(sysTime);
	currentDate.SetDateTime(currentDate.GetYear(),currentDate.GetMonth(),currentDate.GetDay(),0,0,0);

	COleDateTimeSpan saveDaySpan = COleDateTimeSpan( theBOM_INFO.GetStoreDay(),0,0,0);
	COleDateTime saveFileFromDate=currentDate-saveDaySpan;

	vector<CString> sendAuditFileNames;
	CString searchString = MakeTxnAuditSearchFileString(_DATE_TIME(),_DATE(),SEND_KIND::SEND_DATA);
	SearchTxnAuditFile(sendAuditFileNames,searchString);

	for(vector<CString>::iterator sendFileIter = sendAuditFileNames.begin();sendFileIter!=sendAuditFileNames.end();sendFileIter++)
	{
		_DATE_TIME sendTime;
		_DATE businessDate;
		PRINT_KIND printKind;
		SEND_KIND sendKind;
		CString sendSCFileName = *sendFileIter;
		if(TryParseTxnAuditFileName(sendSCFileName,sendTime, businessDate,sendKind))
		{
			COleDateTime businessOleDate = COleDateTime(businessDate.wYear,businessDate.biMonth,businessDate.biDay,0,0,0);
			if(businessOleDate<=saveFileFromDate)
			{
				if(!CResuceOperation::DeleteResuceFile(theBOM_INFO.GetDataPath()+TXN_MANAGE_DAY_FILE_PATH+sendSCFileName,theBOM_INFO.GetBackupPath()+TXN_MANAGE_DAY_FILE_PATH+sendSCFileName))
				{
					throw CTxnAuditException(CTxnAuditException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("删除未送数据文件异常："+sendSCFileName)); 
				}
			}
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::SetCurrentBusinessDay(_DATE currentDate)
{
	CString searchString = MakeTxnAuditSearchFileString(_DATE_TIME(),currentDate,NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchTxnAuditFile(fileNames,searchString);
	if(fileNames.size()>1)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__),  __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	currentBusinessDate = currentDate;
	if(fileNames.size()==1)
	{
		LoadTxnAuditDataFromFile(fileNames[0],TXN_MANAGE_DAY_FILE_PATH,currentTxnAudit);
	}
	else
	{
		currentTxnAudit = TxnAuditData();
		currentTxnAudit.businessDate = ComGetBusiDate();		
		CString fileName = MakeTxnAuditFileName(_DATE_TIME(),currentBusinessDate,SEND_KIND::NOT_SEND_DATA);
		SaveTxnAuditDataToFile(fileName,TXN_MANAGE_DAY_FILE_PATH,currentTxnAudit);
	}
}


////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
 void CTxnAuditManager::LoadUnSendTxnAudit(vector<TxnAuditData>& unSendTxnAudit)
 {
	CString searchString = MakeTxnAuditSearchFileString(_DATE_TIME(),_DATE(),NOT_SEND_DATA);
	vector<CString> fileNames;
	SearchTxnAuditFile(fileNames,searchString);
	for (int i=0;i<fileNames.size();i++)
	{
		TxnAuditData purchaseData;
		LoadTxnAuditDataFromFile(fileNames[i],TXN_MANAGE_DAY_FILE_PATH,purchaseData);
		unSendTxnAudit.push_back(purchaseData);
	}
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::SetTxnAuditSended(_DATE& businessDate)
{
	CString searchString = MakeTxnAuditSearchFileString(_DATE_TIME(),businessDate,SEND_KIND::NOT_SEND_DATA);
	vector<CString> unsendFiles;
	SearchTxnAuditFile(unsendFiles,searchString);
	if(unsendFiles.size()>1)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__),  __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	if(unsendFiles.size()==0)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__),  __LINE__, _T("没有未送日结文件:"+searchString)); 
	}

	SetTxnAuditSended(unsendFiles[0]);
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
TxnAuditData& CTxnAuditManager::GetCurrentTxnAudit()
{
	if(currentBusinessDate == _DATE())
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__),  __LINE__, _T("未设置运营日" )); 
	}
	return currentTxnAudit;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
 void CTxnAuditManager::AddTxnAudit(TXNAuditKey& txnAuditKey, TXNAuditValue& txnAuditValue)
 {
 	if(currentBusinessDate == _DATE())
 	{
 		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__), __LINE__,  _T("未设置运营日" )); 
 	}
	currentTxnAudit.txnAuditItems[txnAuditKey]+=txnAuditValue;

	CString fileName = MakeTxnAuditFileName(_DATE_TIME(),currentBusinessDate,SEND_KIND::NOT_SEND_DATA);
	SaveTxnAuditDataToFile(fileName,TXN_MANAGE_DAY_FILE_PATH,currentTxnAudit);
 }

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::ClearCurrentBusinessDay()
{
	currentBusinessDate = _DATE();
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置当天收益情况为已发给服务器
//@param  
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::SetCurrentTxnAuditSended()
{
	if(currentBusinessDate == _DATE())
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	CString searchString = MakeTxnAuditSearchFileString(_DATE_TIME(),currentBusinessDate,SEND_KIND::NOT_SEND_DATA);
	vector<CString> currentDayUnsendFiles;
	SearchTxnAuditFile(currentDayUnsendFiles,searchString);
	if(currentDayUnsendFiles.size()>1)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	if(currentDayUnsendFiles.size()==0)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("没有未送日结文件:"+searchString)); 
	}

	SetTxnAuditSended(currentDayUnsendFiles[0]);
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      保存TxnAudit集计数据到TxnAudit集计文件
//@param      CString				strFileName			TxnAudit集计二重化主数据文件
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
bool CTxnAuditManager::SaveTxnAuditDataToFile(CString strFileName,CString subFolderName, TxnAuditData& txnAuditData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA txnAuditBinData;
	txnAuditData.Serialize(txnAuditBinData);
	CXFile txnAuditDataFile(theBOM_INFO.GetDataPath()+subFolderName+strFileName,theBOM_INFO.GetBackupPath()+subFolderName+strFileName);
	CXFile::ERR_DETAITL writeResult = txnAuditDataFile.WriteBinFileWithCrc(txnAuditBinData);
	if(writeResult!=CXFile::FILE_OPERATION_OK)
	{
		throw CTxnAuditException(CTxnAuditException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("文件写入异常：" + strFileName)); 
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      加载TxnAudit集计数据从TxnAudit集计文件
//@param      CString				strFileName			TxnAudit集计二重化主数据文件
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
bool CTxnAuditManager::LoadTxnAuditDataFromFile(CString strFileName,CString subFolderName, TxnAuditData& txnAuditData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA txnAuditBinData;
	CXFile txnAuditDataFile(theBOM_INFO.GetDataPath()+subFolderName+strFileName,theBOM_INFO.GetBackupPath()+subFolderName+strFileName);
	txnAuditDataFile.ReadBinFileWithCrc(txnAuditBinData);
	txnAuditData.Deserialize(txnAuditBinData);
	return true;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      拼成日结审计文件名 [TXN_Audit_YYYYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINTED)_SENDED(UNSEND)]
//@retval     成功或失败
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
 CString CTxnAuditManager::MakeTxnAuditFileName(_DATE_TIME sendTime,_DATE businessDate,SEND_KIND sendKind)
 {
 	CString fileName = _T("TXN_Audit_{SendTime}_{BusinessDate}_{SendFlag}.dat");
 	CString strSendTime =_T("");
 	strSendTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),sendTime.wYear,sendTime.biMonth,sendTime.biDay,sendTime.biHour,sendTime.biMinute,sendTime.biSecond);
 	fileName.Replace(_T("{SendTime}"),strSendTime);
 	CString strDate = _T("");
 	strDate.Format(_T("%.4d%.2d%.2d"),businessDate.wYear,businessDate.biMonth,businessDate.biDay);
 	fileName.Replace(_T("{BusinessDate}"),strDate);
 	CString strSendFlag = sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
 	fileName.Replace(_T("{SendFlag}"),strSendFlag);
 	return fileName;
 }

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
 CString CTxnAuditManager::MakeTxnAuditSearchFileString(_DATE_TIME sendTime,_DATE businessDate,SEND_KIND sendKind)
 {
 	CString searchFileNameString = _T("TXN_Audit_{SendTime}_{BusinessDate}_{SendFlag}.dat");
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
 	CString strSendFlag = sendKind == ALL_DATA ? _T("*") : sendKind == NOT_SEND_DATA?_T("UNSEND"):_T("SENDED");
 	searchFileNameString.Replace(_T("{SendFlag}"),strSendFlag);
 	return searchFileNameString;
 }

////////////////////////////////////////////////////////////////////////////
///**
//@brief      解析日结审计文件名 [SC_Audit_YYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINT)_SENDED(UNSEND)]
//@retval     成功或失败
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
 bool CTxnAuditManager::TryParseTxnAuditFileName(CString& fileName,_DATE_TIME& sendTime,_DATE& businessDate,SEND_KIND& sendKind)
 {
 	CString fileNameWithExtention = GetFileNameWithoutExtentionFormFullPath(fileName);
 	std::auto_ptr<CStringArray> arrStrings(SplitCString(fileNameWithExtention,_T("_")));
 	if(arrStrings.get()==NULL || arrStrings->GetCount()!=5)
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
 	if(arrStrings->ElementAt(4).MakeUpper() == "SENDED")
 	{
 		sendKind = SEND_KIND::SEND_DATA;
 	}
 	else if(arrStrings->ElementAt(4).MakeUpper() == "UNSEND")
 	{
 		sendKind = SEND_KIND::NOT_SEND_DATA;
 	}
 	else
 	{
 		return false;
 	}
 	return true;
 }

////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得符合指定条件的指定目录的审计文件
//@param    CString subFolderName 指定目录
//@param    CString searchCondition
//@retval     无
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition)
{
	if(!CResuceOperation::CheckAndCreateDirectory(theBOM_INFO.GetDataPath() + subFolderName,theBOM_INFO.GetBackupPath() + subFolderName))
	{
		throw CTxnAuditException(CTxnAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("二重化目录异常：" + subFolderName)); 
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
			throw CTxnAuditException(CTxnAuditException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("主目录文件:" + mainFilePath+_T(",备份目录文件:")+backupFilePath)); 
		}
		vecFileNames.push_back(fileName);
	}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得符合指定条件的SC日结文件
//@param    CString searchCondition
//@retval     无
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
 void CTxnAuditManager::SearchTxnAuditFile(vector<CString>& vecFileNames,CString searchCondition)
 {
 	SearchFile(vecFileNames,TXN_MANAGE_DAY_FILE_PATH,searchCondition);
 }


////////////////////////////////////////////////////////////////////////////
///**
//@brief      设定数据已发送标志
//
//@param	  CString strFileName				文件名
//
//@retval     无
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::SetTxnAuditSended(CString strFileName)
{
	_DATE_TIME sendTime;
	_DATE bussinessDate;
	SEND_KIND sendKind;
	if(!TryParseTxnAuditFileName(strFileName,sendTime,bussinessDate,sendKind))
	{
		throw CTxnAuditException(CTxnAuditException::FILE_NAME_INVALID, _T(__FILE__), __LINE__, _T(" 目标文件：" )+ strFileName+_T("名字不可解析")); 
	}
	if(sendKind!=SEND_KIND::NOT_SEND_DATA)
	{
		throw CTxnAuditException(CTxnAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T(" 文件：") + strFileName+_T("为已打印状态")); 
	}
	sendKind = SEND_KIND::SEND_DATA;
	CString renameToFileName = MakeTxnAuditFileName(ComGetCurTime(),bussinessDate,sendKind);
	RenameFileName(TXN_MANAGE_DAY_FILE_PATH,strFileName,renameToFileName);
}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTxnAuditManager::RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo)
{
	SYNC(FILE,strFileNameFrom);
	CString mainFilePathFrom = theBOM_INFO.GetDataPath()+subFolderName+strFileNameFrom;
	CString backupFilePathFrom = theBOM_INFO.GetBackupPath()+subFolderName+strFileNameFrom;
	CString mainFilePathTo = theBOM_INFO.GetDataPath()+subFolderName+strFileNameTo;
	CString backupFilePathTo = theBOM_INFO.GetBackupPath()+subFolderName+strFileNameTo;
	if(!ComCheckFileExist(mainFilePathFrom))
	{
		throw CTxnAuditException(CTxnAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" 主文件：" )+ mainFilePathFrom+_T("不存在")); 
	}
	if(!ComCheckFileExist(backupFilePathFrom))
	{
		throw CTxnAuditException(CTxnAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" 备份文件：" )+ backupFilePathFrom+_T("不存在")); 
	}
	if(!CResuceOperation::RenameResuceFile(mainFilePathFrom,backupFilePathFrom,mainFilePathTo,backupFilePathTo))
	{
		throw CTxnAuditException(CTxnAuditException::RENAME_FILE_FAIL, _T(__FILE__), __LINE__, _T("重命名二重化文件：") + strFileNameFrom+_T("为")+strFileNameTo+_T("失败")); 
	}
}