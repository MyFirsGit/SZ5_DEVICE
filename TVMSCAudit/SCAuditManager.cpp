#include "stdafx.h"
#include "SCAuditManager.h"
#include "sync.h"
#include "Util.h"
#include <memory>
#include "Xfile.h"
#include "TVMInfo.h"
#include "SCAuditDef.h"
#include "CommonFunc.h"
#include "CommonDefine.h"
//#include "ServiceDefinition.h"
#include "DXFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
/*
静态实例
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager CSCAuditManager::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager::CSCAuditManager()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager::~CSCAuditManager()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   取得该类唯一实例   

@param      无

@retval   CSCAuditManager& 对象实例引用  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCAuditManager& CSCAuditManager::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化
@retval     bool		true:成功  false:失败
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::Initilize()
{
	//确认目录存在，不存在，则创建
	if(!CDXFile::CheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PURCHASE_MANAGE_DAY_FILE_PATH,theTVM_INFO.GetBackupPath() + PURCHASE_MANAGE_DAY_FILE_PATH))
	{
		throw CSCAuditException(CSCAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("新建主数据目录异常：") +CString(PURCHASE_MANAGE_DAY_FILE_PATH)); 
	}

	//删除已送数据
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	COleDateTime currentDate(sysTime);
	currentDate.SetDateTime(currentDate.GetYear(),currentDate.GetMonth(),currentDate.GetDay(),0,0,0);

	COleDateTimeSpan saveDaySpan = COleDateTimeSpan( theTVM_INFO.GetStoreDay(),0,0,0);
	COleDateTime saveFileFromDate=currentDate-saveDaySpan;

	vector<CString> sendSCFileNames;
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),_DATE(),PRINT_ALL,SEND_DATA);
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
			if(!CDXFile::DeleteResuceFile(theTVM_INFO.GetDataPath()+PURCHASE_MANAGE_DAY_FILE_PATH+sendSCFileName,theTVM_INFO.GetBackupPath()+PURCHASE_MANAGE_DAY_FILE_PATH+sendSCFileName))
			{
				throw CSCAuditException(CSCAuditException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("删除未送数据文件异常："+sendSCFileName)); 
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置当前运营日的结算文件  如果没有对应的文件则创建一个新文件

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
	// 存在多个未发送的文件说明程序逻辑错误 应该正常情况下只有一个未发送的文件
	if(fileNames.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	// 正好有一个未发送的文件的情况 就读取该文件的数据
	currentBusinessDate = currentDate;
	if(fileNames.size()==1)
	{
		LoadPurchaseDataFromFile(fileNames[0],PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
	}
	// 没有文件的时候就创建一个结算文件
	else
	{
		currentDayPurchase = PurchaseData();
		currentDayPurchase.auditTimeFrom = ComGetCurTime();
		currentDayPurchase.auditTimeTo = ComGetCurTime();
		currentDayPurchase.businessDate = ComGetBusiDate();
		//currentDayPurchase.staffID = _T("000000");
		CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
		SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      加载所有的未发送的文件

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
	for (size_t i=0;i<fileNames.size();i++)
	{
		PurchaseData purchaseData;
		LoadPurchaseDataFromFile(fileNames[i],PURCHASE_MANAGE_DAY_FILE_PATH,purchaseData);
		unSendDayPurchase.push_back(purchaseData);
	}
}

void CSCAuditManager::setSelledCardNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lSelledCardNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}
void CSCAuditManager::addSelledCardNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lSelledCardNum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setSelledCardSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lSelledCardSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addSelledCardSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lSelledCardSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardRechargeNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardRechargeNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardRechargeNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardRechargeNum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardRechargeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardRechargeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardRechargeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardRechargeSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setFailBusinessNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lFailBusinessNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addFailBusinessNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lFailBusinessNum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setFailBusinessSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lFailBusinessSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addFailBusinessSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lFailBusinessSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinSupplementSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinSupplementSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinSupplementSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinSupplementSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinRecoverSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinRecoverSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinRecoverSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinRecoverSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinChangeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinChangeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinChangeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinChangeSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinLastLeftSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinLastLeftSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinLastLeftSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinLastLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinCurrentLeftSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinCurrentLeftSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinCurrentLeftSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCoinIncomeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinIncomeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCoinIncomeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCoinIncomeSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteSupplementSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteSupplementSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteSupplementSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteSupplementSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteRecoverSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteRecoverSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteRecoverSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteRecoverSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteChangeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteChangeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteChangeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteChangeSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteLastLeftSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteLastLeftSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteLastLeftSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteLastLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteCurrentLeftSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteCurrentLeftSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setNoteIncomeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteIncomeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addNoteIncomeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setBankCardIncomeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lBankCardIncomeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addBankCardIncomeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setBankCardBusinessNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lBankCardBusinessNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addBankCardBusinessNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setWalletIncomeSum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	//currentDayPurchase.settlementData.lWalletIncomeSum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addWalletIncomeSum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setWalletBusinessNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	//currentDayPurchase.settlementData.lWalletBusinessNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addWalletBusinessNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setTokenLastLeftNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lTokenLastLeftNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addTokenLastLeftNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setTokenCurrentInvalidatedNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lTokenCurrentInvalidatedNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addTokenCurrentInvalidatedNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setTokenCurrentSupplementNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lTokenCurrentSupplementNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addTokenCurrentSupplementNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setTokenCurrentSelledNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lTokenCurrentSelledNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addTokenCurrentSelledNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setTokenCurrentRecoverNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
//	currentDayPurchase.settlementData.lTokenCurrentRecoverNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addTokenCurrentRecoverNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardLastLeftNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardLastLeftNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardLastLeftNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardCurrentInvalidateNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardCurrentInvalidateNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardCurrentInvalidateNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardCurrrntSupplementNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardCurrrntSupplementNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardCurrrntSupplementNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardCurrentSelledNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardCurrentSelledNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardCurrentSelledNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::setCardCurrentRecoverNum(long lvalue)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lCardCurrentRecoverNum=lvalue;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

void	CSCAuditManager::addCardCurrentRecoverNum(long lstep)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.settlementData.lNoteCurrentLeftSum+=lstep;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置结算文件为发送状态 实质操作是修改文件名来实现

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetDayPurchaseSended(_DATE& businessDate)
{
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),businessDate,PRINT_ALL,NOT_SEND_DATA);
	vector<CString> unsendFiles;
	SearchDayPurchaseFile(unsendFiles,searchString);
	if(unsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	if(unsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("没有未送日结文件:"+searchString)); 
	}

	SetDayPurchaseSended(unsendFiles[0]);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      得到当前正在操作的文件的数据结构

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PurchaseData& CSCAuditManager::GetCurrentDayPurchase()
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	return currentDayPurchase;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置文件的运营结束时间 和开始运营时间相对应

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentDayAuditTo(_DATE_TIME auditToTime)
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	currentDayPurchase.auditTimeTo = auditToTime;
	CString fileName = MakeDayPurchaseFileName(_DATE_TIME(),currentBusinessDate,NOT_PRINT_DATA,NOT_SEND_DATA);
	SavePurchaseDataToFile(fileName,PURCHASE_MANAGE_DAY_FILE_PATH,currentDayPurchase);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      置空当前运营时间

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
@brief      设置当天收益情况为已发给服务器
@param  
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SetCurrentDayPurchaseSended()
{
	if(currentBusinessDate == _DATE())
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未设置运营日" )); 
	}
	CString searchString = MakeDayPurchaseSearchFileString(_DATE_TIME(),currentBusinessDate,PRINT_ALL,NOT_SEND_DATA);
	vector<CString> currentDayUnsendFiles;
	SearchDayPurchaseFile(currentDayUnsendFiles,searchString);
	if(currentDayUnsendFiles.size()>1)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("未送日结文件不止一个:"+searchString)); 
	}
	if(currentDayUnsendFiles.size()==0)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T("没有未送日结文件:"+searchString)); 
	}

	SetDayPurchaseSended(currentDayUnsendFiles[0]);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      保存SC集计数据到SC集计文件
@param      CString				strFileName			SC集计二重化主数据文件
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::SavePurchaseDataToFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA purchaseBinData;
	purchaseData.Serialize(purchaseBinData);
	CDXFile purchaseFile(theTVM_INFO.GetDataPath()+subFolderName+strFileName,theTVM_INFO.GetBackupPath()+subFolderName+strFileName);
	CDXFile::ERR_DETAITL writeResult = purchaseFile.WriteBinFile((const char *)purchaseBinData.lpData,purchaseBinData.nLen,true);
	if(writeResult!=CXFile::FILE_OPERATION_OK)
	{
		throw CSCAuditException(CSCAuditException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("文件写入异常：" + strFileName)); 
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      加载SC集计数据从SC集计文件
@param      CString				strFileName			SC集计二重化主数据文件
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CSCAuditManager::LoadPurchaseDataFromFile(CString strFileName,CString subFolderName, PurchaseData& purchaseData)
{
	SYNC(FILE,strFileName);
	VARIABLE_DATA purchaseBinData;
	CDXFile purchaseFile(theTVM_INFO.GetDataPath()+subFolderName+strFileName,theTVM_INFO.GetBackupPath()+subFolderName+strFileName);
	purchaseFile.ReadBinFile(purchaseBinData,true);
	purchaseData.Deserialize(purchaseBinData);
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      拼成日结审计文件名 [SC_Audit_YYYYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINTED)_SENDED(UNSEND)]
@retval     成功或失败
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
@brief      生成查询文件名称 

@param      _DATE_TIME sendTime,	//发送时间
			_DATE businessDate,		//运营时间
			PRINT_KIND printKind,	//打印标志
			SEND_KIND sendKind		//发送标志

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
@brief      解析日结审计文件名 [SC_Audit_YYMMDDHHmmSS_YYYYMMDD_PRINTED(UNPRINT)_SENDED(UNSEND)]
@retval     成功或失败
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
	if(arrStrings->ElementAt(4).MakeUpper() == _T("PRINTED"))
	{
		printKind = PRINT_DATA;
	}
	else if(arrStrings->ElementAt(4).MakeUpper() == _T("UNPRINT"))
	{
		printKind = NOT_PRINT_DATA;
	}
	else
	{
		return false;
	}
	if(arrStrings->ElementAt(5).MakeUpper() == _T("SENDED"))
	{
		sendKind = SEND_DATA;
	}
	else if(arrStrings->ElementAt(5).MakeUpper() == _T("UNSEND"))
	{
		sendKind = NOT_SEND_DATA;
	}
	else
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得符合指定条件的指定目录的日结文件
@param    CString subFolderName 指定目录
@param    CString searchCondition
@retval     无
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SearchFile(vector<CString>& vecFileNames,CString subFolderName, CString searchCondition)
{
	if(!CDXFile::CheckAndCreateDirectory(theTVM_INFO.GetDataPath() + subFolderName,theTVM_INFO.GetBackupPath() + subFolderName))
	{
		throw CSCAuditException(CSCAuditException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("二重化目录异常：") + theTVM_INFO.GetDataPath() + subFolderName); 
	}

	vector<CString> dataPathFiles;
	::SearchFile(theTVM_INFO.GetDataPath() + subFolderName + searchCondition,dataPathFiles);

	for(vector<CString>::iterator dataPathFileIter = dataPathFiles.begin();dataPathFileIter!=dataPathFiles.end();dataPathFileIter++)
	{
		CString fileName = GetFileNameFromFullPath(*dataPathFileIter);
		CString mainFilePath = theTVM_INFO.GetDataPath()+subFolderName+fileName;
		CString backupFilePath = theTVM_INFO.GetBackupPath()+subFolderName+fileName;
		if(!CDXFile::ResuceFile(mainFilePath,backupFilePath,true))
		{
			throw CSCAuditException(CSCAuditException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("主目录文件:") + mainFilePath+_T(",备份目录文件:")+backupFilePath); 
		}
		vecFileNames.push_back(fileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得符合指定条件的SC日结文件
@param    CString searchCondition
@retval     无
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::SearchDayPurchaseFile(vector<CString>& vecFileNames,CString searchCondition)
{
	SearchFile(vecFileNames,PURCHASE_MANAGE_DAY_FILE_PATH,searchCondition);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设定SC个人小计数据已打印标志
@param		PURCHASE_SC_DATA	dtSCData				SC集计文件数据
int					iRecoverFromBakCount	文件恢复次数
@retval     无
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
		throw CSCAuditException(CSCAuditException::FILE_NAME_INVALID, _T(__FILE__), __LINE__, _T(" 目标文件：") + strFileName+_T("名字不可解析")); 
	}
	if(sendKind!=NOT_SEND_DATA)
	{
		throw CSCAuditException(CSCAuditException::LOGIC_ERR, _T(__FILE__), __LINE__, _T(" 文件：") + strFileName+_T("为已打印状态")); 
	}
	sendKind = SEND_DATA;
	CString renameToFileName = MakeDayPurchaseFileName(ComGetCurTime(),bussinessDate,printKind,sendKind);
	RenameFileName(PURCHASE_MANAGE_DAY_FILE_PATH,strFileName,renameToFileName);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      根据指定条件在当前规则下重命名文件

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCAuditManager::RenameFileName(CString subFolderName,CString strFileNameFrom,CString strFileNameTo)
{
	SYNC(FILE,strFileNameFrom);
	CString mainFilePathFrom = theTVM_INFO.GetDataPath()+subFolderName+strFileNameFrom;
	CString backupFilePathFrom = theTVM_INFO.GetBackupPath()+subFolderName+strFileNameFrom;
	CString mainFilePathTo = theTVM_INFO.GetDataPath()+subFolderName+strFileNameTo;
	CString backupFilePathTo = theTVM_INFO.GetBackupPath()+subFolderName+strFileNameTo;
	if(!ComCheckFileExist(mainFilePathFrom))
	{
		throw CSCAuditException(CSCAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" 主文件：") + mainFilePathFrom+_T("不存在")); 
	}
	if(!ComCheckFileExist(backupFilePathFrom))
	{
		throw CSCAuditException(CSCAuditException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T(" 备份文件：") + backupFilePathFrom+_T("不存在")); 
	}
	if(!CDXFile::RenameResuceFile(mainFilePathFrom,backupFilePathFrom,mainFilePathTo,backupFilePathTo))
	{
		throw CSCAuditException(CSCAuditException::RENAME_FILE_FAIL, _T(__FILE__), __LINE__, _T("重命名二重化文件：") + strFileNameFrom+_T("为")+strFileNameTo+_T("失败")); 
	}
}