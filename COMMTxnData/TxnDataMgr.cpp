#include "StdAfx.h"
#include "TxnDataMgr.h"
#include "sync.h"
#include "maintenanceinfo.h"
#include "DebugLog.h"
#include "CommonDefine.h"
#include "DXFile.h"
#include "SysInfo.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define theTXN_DATA_TRACE CFileLog::GetInstance(FILTER_TXN_DATA)

CTxnDataMgr CTxnDataMgr::accTxnDataMgr(_T("CAccTxnDataMgr"),TXN_FILE_NAMES(ACC_TXN_FILE_PATH,ACC_SENDED_FILE_PATH,CString(ACC_TXN_FILE_PATH)+CString(CURRENT_ACC_FILE_NAME),CString(ACC_TXN_FILE_PATH)+CString(UNSEND_FILE_PATH)),ACC_TRANS,ACC_PEKG);
CTxnDataMgr CTxnDataMgr::afcTxnDataMgr(_T("CAfcTxnDataMgr"),TXN_FILE_NAMES(AFC_TXN_FILE_PATH,AFC_SENDED_FILE_PATH,CString(AFC_TXN_FILE_PATH)+CString(CURRENT_AFC_FILE_NAME),CString(AFC_TXN_FILE_PATH)+CString(UNSEND_FILE_PATH)),AFC_TRANS,AFC_PEKG);
CTxnDataMgr CTxnDataMgr::busTxnDataMgr(_T("CBusTxnDataMgr"),TXN_FILE_NAMES(BUS_TXN_FILE_PATH,BUS_SENDED_FILE_PATH,CString(BUS_TXN_FILE_PATH)+CString(CURRENT_BUS_FILE_NAME),CString(BUS_TXN_FILE_PATH)+CString(UNSEND_FILE_PATH)),BUS_DATA,BUS_PEKG);
CTxnDataMgr CTxnDataMgr::ectTxnDataMgr(_T("CEctTxnDataMgr"),TXN_FILE_NAMES(ECT_TXN_FILE_PATH,ECT_SENDED_FILE_PATH,CString(ECT_TXN_FILE_PATH)+CString(CURRENT_ECT_FILE_NAME),CString(ECT_TXN_FILE_PATH)+CString(UNSEND_FILE_PATH)),ECT_TRANS,ECT_PEKG);
CTxnDataMgr CTxnDataMgr::evnTxnDataMgr(_T("CEvnTxnDataMgr"),TXN_FILE_NAMES(EVN_TXN_FILE_PATH,EVN_SENDED_FILE_PATH,CString(EVN_TXN_FILE_PATH)+CString(CURRENT_EVN_FILE_NAME),CString(EVN_TXN_FILE_PATH)+CString(UNSEND_FILE_PATH)),EVEN_DATA,EVN_PEKG);


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTxnDataMgr::CTxnDataMgr()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTxnDataMgr::CTxnDataMgr(CString className,TXN_FILE_NAMES txnFileNames,DATA_TYPE_CODE txnCode,DATA_PEKG_KIND pekgKind)
{

	this->className = className;
	this->txnFileNames = txnFileNames;
	this->txnCode = txnCode;
	this->txnKind = pekgKind;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTxnDataMgr::~CTxnDataMgr()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief        初始化

@param      (i)CString strDataPath			主数据目录
@param      oCString strBackupPath			备份数据目录

@retval       无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::Internalize(CString strDataPath,CString strBackupPath)
{
	m_dataPath = strDataPath;
	m_BackupPath = strBackupPath;
	// 检查并创建交易数据文件目录
	if (! ComCheckAndCreateDirectory(m_dataPath + txnFileNames.unsendedFolderPath)) { 
		return ;
	}
	if (! ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.unsendedFolderPath)) { 
		return ;
	}

	// 检查并创建ACC交易历史数据文件目录
	if (! ComCheckAndCreateDirectory(m_dataPath + txnFileNames.sendedFolderPath + GetTodayDate())) { 
		return ;
	}
	if (! ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.sendedFolderPath + GetTodayDate())) { 
		return ;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化交易数据

@param     (i)bool IsValidateUnSendPackage  是否验证未送数据有效性
@param     (i)int iSendedDataMaxDay			  已送数据保存最大天数
@param     (i)int iUnSendDataMaxDay			  未送数据保存最大天数

@retval     int   0：成功   1：创建目录失败   2：未送数据过多   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTxnDataMgr::InitTxnData(bool IsValidateUnSendPackage,int iSendedDataMaxDay,int iUnSendDataMaxDay)
{
	theDEBUG_LOG->WriteData(className,_T("InitTxnData"),__LINE__,_T("<<"));
	m_iUnsendMaxDay = iUnSendDataMaxDay;
	// 二重化未送数据
	ResumeUnsendData(txnCode,m_dataPath,m_BackupPath);
	//ProtectUnsendPackage();
	if(txnKind == ACC_PEKG && IsValidateUnSendPackage)
	{
		ValidateUnsendPackage();
	}
	
	// 若存在未打包文件，先打包
	CString currentFileName = m_dataPath +  txnFileNames.currentFileName;
	if(IsFileExist(currentFileName)){	
		MakePekg(true);
	}

	// 检查未送数据是否过多
	if(HasTooMoreUnSendData()){
		throw CTxnDataException(CTxnDataException::SALEDATA_NOT_SEND_TOO_MORE, _T(__FILE__), __LINE__); 
	}
	else{
		// 读取未送文件名
		CString strUnsendPath = _T("");		
		strUnsendPath = m_dataPath + txnFileNames.unsendedFolderPath;
		GetUnsendFileName(strUnsendPath,m_queUnSendPekgName);
	}

	// 检查已送数据是否过多
	vector<CString> tempDirNames;
	CString strSendPath = m_dataPath + txnFileNames.sendedFolderPath;

	if(GetDirectoryCount(strSendPath,tempDirNames) > iSendedDataMaxDay){
		DeleteTooMoreSendedData(tempDirNames,iSendedDataMaxDay);
	}
	tempDirNames.clear();
	CString strSendPathBackup = m_BackupPath + txnFileNames.sendedFolderPath;
	if(GetDirectoryCount(strSendPathBackup,tempDirNames) > iSendedDataMaxDay){
		DeleteTooMoreSendedData(tempDirNames,iSendedDataMaxDay);
	}
	theDEBUG_LOG->WriteData(className,_T("InitTxnData"),__LINE__,_T(">>"));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存交易数据

@param      WORD txnLen		UD数据长度（包含AFC系统专用头的10个字节）
@param      LPBYTE txnBuf   UD数据体（包含AFC系统专用头的10个字节）

@retval     无  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::SaveTxnData(WORD txnLen,LPBYTE txnBuf)
{
	theDEBUG_LOG->WriteData(className,_T("SaveTxnData"),__LINE__,_T("<<"));
	if(txnLen <= 0 || txnBuf == NULL){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	
	theTXN_DATA_TRACE->WriteData(className+_T(":"),txnBuf,txnLen);
	CString currentFileName = m_dataPath +  txnFileNames.currentFileName;	
	SYNC(FILE,currentFileName);

	CFile cfFile; 
	CFileException cfeError; 
	if(!cfFile.Open(currentFileName, CFile::modeCreate|CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
		TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
		CString strException; 
		cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = _T("异常："); 
		strException += strCause; 
		throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	} 
	try{
		BYTE currentPekgID[PEKG_ID_LEN];
		LPBYTE lpPekgID = currentPekgID;

		*lpPekgID = 0x01;
		lpPekgID ++;
		// 数据包ID ：数据包类型+设备ID+时间戳+打包序列号
		memset(lpPekgID,0x00,14);
		lpPekgID += 14;

		long  pekgLen = txnLen;
		WORD  recordCount = 1;
		
		cfFile.SeekToBegin(); 
		INT64 fileLength = cfFile.GetLength();
		if(fileLength>PEKG_ID_LEN)//说明以前有保存，则更新包长度和包中记录数。
		{
			BYTE savedPekgID[PEKG_ID_LEN];
			cfFile.Read(savedPekgID,PEKG_ID_LEN);

			long savedPekgLength = mltol( ComMakeLong(savedPekgID[15],savedPekgID[16],savedPekgID[17],savedPekgID[18]));
			pekgLen+=savedPekgLength;

			WORD savedRecordCount = mstos(ComMakeWORD(savedPekgID[19],savedPekgID[20]));
			recordCount+=savedRecordCount;
		}
		else
		{
			pekgLen+=PEKG_ID_LEN;//首个包加上包ID。
		}
		
		// 数据包长度
		ComRevLong(ltoml(pekgLen),lpPekgID);		
		lpPekgID += 4;
		// 包中记录数
		ComRevWORD(stoms(recordCount),lpPekgID);			
		lpPekgID += 2;
		// 预留
		memset(lpPekgID,0xFF,7);

		cfFile.SeekToBegin();//读包头。
		cfFile.Write(currentPekgID,PEKG_ID_LEN);

		cfFile.SeekToEnd();//移动到末尾读业务数据。
		cfFile.Write(txnBuf,txnLen);
		cfFile.Flush();
		cfFile.Close();
	} 
	catch(...)
	{
		cfFile.Close();
		throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
	}
	// 保存交易审核数据（澳门线不适用）
	//TxnDataSaved.Invoke(txnLen,txnBuf);
	theDEBUG_LOG->WriteData(className,_T("SaveTxnData"),__LINE__,_T(">>"));
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      更新当前数据包ID

@param      无

@retval     CString   数据包ID

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTxnDataMgr::UpdateCurrentPekgID()
{
	theDEBUG_LOG->WriteData(className,_T("UpdateCurrentPekgID"),__LINE__,_T("<<"));
	CString retPekgID = _T("");
	CString currentFileName = m_dataPath +  txnFileNames.currentFileName;	
	SYNC(FILE,currentFileName);
	CFile cfFile; 
	CFileException cfeError; 
	if(!cfFile.Open(currentFileName, CFile::modeCreate|CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
		TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
		CString strException; 
		cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = _T("异常："); 
		strException += strCause; 
		throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	} 
	try{
		BYTE savedPekgID[PEKG_ID_LEN];
		cfFile.Read(savedPekgID,PEKG_ID_LEN);
		LPBYTE lpData = savedPekgID;

		// 包格式版本号		
		lpData ++;
		// 数据包ID ：数据包类型+设备ID+时间戳+打包序列号	
		// 数据包类型
		*lpData = txnKind;
		lpData ++;
		// 设备ID
		theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpData);
		lpData+=4;
		// 时间戳
		_DATE_TIME dtTm = ComGetCurTime();
		dtTm.Serialize(lpData);
		lpData+=7;
		// 获得打包序列号			
		WORD selNo = theSELNO_MGR.GetSelNo(dtTm);
		ComRevWORD(selNo,lpData);	
		lpData += 2;	
		cfFile.SeekToBegin();//读包头。
		cfFile.Write(savedPekgID,PEKG_ID_LEN);
		cfFile.Flush();
		cfFile.Close();
		// 获取文件名
		retPekgID.Format(_T("%.2d%s%s%.4X"),txnKind,theMAINTENANCE_INFO.GetCurrentDevice().ToString(),dtTm.ToString(_T("%04d%02d%02d%02d%02d%02d")),selNo);
	} 
	catch(...)
	{
		cfFile.Close();
		throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
	}
	theDEBUG_LOG->WriteData(className,_T("UpdateCurrentPekgID"),__LINE__,_T(">>"));
	return retPekgID;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      组包函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::MakePekg(bool isInit)
{
	theDEBUG_LOG->WriteData(className,_T("MakePekg"),__LINE__,_T("<<"));
	try{		
		CString fileName = _T("");
		CString strUnsend = _T("");
		CString strUnsendbackup = _T("");
		CString currentFileName = _T("");
		currentFileName = m_dataPath +  txnFileNames.currentFileName;
		if (ComCheckFileExist(currentFileName) == TRUE) {
			// 更新当前数据包ID
			SYNC(FILE,currentFileName);
			fileName = UpdateCurrentPekgID();
			strUnsend = m_dataPath + txnFileNames.unsendedFolderPath + fileName;
			strUnsendbackup = m_BackupPath + txnFileNames.unsendedFolderPath + fileName;
			// 当前数据包文件到未送目录
			BOOL moveFileSuccess = ::MoveFileEx(currentFileName,strUnsend,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
			if( !moveFileSuccess){
				for(int i=0;i<2;i++) //重试两次
				{
					::Sleep(100);
					moveFileSuccess = ::MoveFileEx(currentFileName,strUnsend,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
					if(moveFileSuccess)
					{
						break;
					}
				}
				if(!moveFileSuccess)
				{
					throw CTxnDataException(CTxnDataException::FILE_CREATE_FAIL, _T(__FILE__), __LINE__, _T("打包文件移动失败:") + currentFileName + _T(">") +strUnsend);
				}
			}

			BOOL copyFileSuccess =::CopyFile(strUnsend,strUnsendbackup,FALSE); 
			if( !copyFileSuccess)
			{
				for(int i=0;i<2;i++) //重试两次
				{
					::Sleep(100);
					copyFileSuccess =::CopyFile(strUnsend,strUnsendbackup,FALSE); 
					if(copyFileSuccess)
					{
						break;
					}
				}
				if(!copyFileSuccess)
				{
					throw CTxnDataException(CTxnDataException::FILE_CREATE_FAIL, _T(__FILE__), __LINE__, _T("打包文件移动失败:") + strUnsend+ _T(">") + strUnsendbackup);
				}
			}
			
			// 清空当前内存数据包数据
			m_currentPekg.clear();
			if(!isInit){
				// 将数据包文件名加入未送队列
				SYNC(QUEUE,_T("m_queUnSendPekgName"));
				m_queUnSendPekgName.push(fileName);	
			}
			if(txnKind == ACC_PEKG)
			{
				ProtectUnsendPackage();
			}
		}
	}
	catch (CSysException&) {	
		throw;
	}
	catch (...) {	
		throw;
	}
	theDEBUG_LOG->WriteData(className,_T("MakePekg"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      加密未送数据包

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::EncryptUnsendPackage(_DATE_TIME& currentTime,VARIABLE_DATA& data)
{
	CString strPlainText = _T("");
	strPlainText = strPlainText + currentTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"));

	CString strUnsendPath =  m_dataPath + txnFileNames.unsendedFolderPath;
	
	// 查找文件
	CFileFind finder;
	CString sFileName = _T("");
	BOOL bContinue = finder.FindFile(strUnsendPath +_T("*"));

	// 如果没有找到文件，就关闭文件
	if (!bContinue) {
		finder.Close();
	}

	vector<CString> tempFileNames;
	while (bContinue) {
		bContinue = finder.FindNextFile();

		if (finder.IsDots()||finder.IsDirectory()) {
			continue;
		}
		else {   
			// 取出文件名，用于比较日期
			strPlainText = strPlainText + finder.GetFileName(); 
			CString strLength=_T("");
			strLength.Format(_T("%d"),finder.GetLength());
			strPlainText = strPlainText+strLength;		
		}
	}
	finder.Close();
	data.lpData = new BYTE[strPlainText.GetLength()+1];
	data.nLen = strPlainText.GetLength()+1;
	memset(data.lpData,0,data.nLen);
	encrypt((LPBYTE)strPlainText.GetBuffer(),strPlainText.GetLength(),( char*)data.lpData);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将未送数据包加密写在一个文件里

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::ProtectUnsendPackage()
{
	
	_DATE_TIME currentTime = ComGetCurTime();
	VARIABLE_DATA encryptData;
	EncryptUnsendPackage(currentTime,encryptData);
	VARIABLE_DATA wholeData;
	wholeData.lpData = new BYTE[encryptData.nLen+7];
	wholeData.nLen = encryptData.nLen+7;
	LPBYTE lpData = wholeData.lpData;
	currentTime.Serialize(lpData);
	lpData+=7;
	memcpy(lpData,encryptData.lpData,encryptData.nLen);
	
	CDXFile protectUnsendPackage(m_dataPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),m_BackupPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME));
	protectUnsendPackage.WriteBinFile((const char *)wholeData.lpData,wholeData.nLen,true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检验文件和当前未送是否一致

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::ValidateUnsendPackage()
{
	bool resueFileSuccess = CDXFile::ResuceFile(m_dataPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),m_BackupPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),true);
	if(!resueFileSuccess)
	{
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("检验未送数据失败"));
	}
	VARIABLE_DATA protectUnsendPackageData;
	CDXFile purchaseFile(m_dataPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),m_BackupPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME));
	purchaseFile.ReadBinFile(protectUnsendPackageData,true);
	_DATE_TIME lastSavedDateTime;
	lastSavedDateTime.Deserialize(protectUnsendPackageData.lpData);

	VARIABLE_DATA encryptUnsendPackageData;
	EncryptUnsendPackage(lastSavedDateTime,encryptUnsendPackageData);

	LPBYTE lpSaveEncryptData = protectUnsendPackageData.lpData+7;
	LPBYTE lpNewEncryptData = encryptUnsendPackageData.lpData;

	if(encryptUnsendPackageData.nLen +7 != protectUnsendPackageData.nLen)
	{
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("检验未送数据失败"));
	}
	if(memcmp(lpSaveEncryptData,lpNewEncryptData,encryptUnsendPackageData.nLen)!=0)
	{
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("检验未送数据失败"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否存在未送数据

@param      无

@retval     int 未送数据包的数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CTxnDataMgr::HasUnsendData()
{
	theDEBUG_LOG->WriteData(className,_T("HasUnsendData"),__LINE__,_T("<<"));

	SYNC(QUEUE,_T("m_queUnSendPekgName"));
	unsigned long retCnt = 0;
	// 队列不空，返回未送数据包的数量
	if(!m_queUnSendPekgName.empty()){
		retCnt = (unsigned long)m_queUnSendPekgName.size();
	}
	// 队列为空，把未送文件名按时间排序读入队列，返回未送数据包的数量
	else{
		CString strUnsendPath = _T("");		
		strUnsendPath = m_dataPath + txnFileNames.unsendedFolderPath;
		//把文件名按时间排序读入队列
		while(m_queUnSendPekgName.size()>0){
			m_queUnSendPekgName.pop();
		}
		GetUnsendFileName(strUnsendPath,m_queUnSendPekgName);
		if(!m_queUnSendPekgName.empty()){
			retCnt = (unsigned long)m_queUnSendPekgName.size();
		}
	}
	theDEBUG_LOG->WriteData(className,_T("HasUnsendData"),__LINE__,_T(">>"));
	return retCnt;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取指定数据包

@param      (i)PEKG_DATA& PekgData	 数据包结构体

@retval     bool true：成功  false：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTxnDataMgr::GetDataPekgById(PEKG_DATA& PekgData) 
{
	theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("<<"));
	if((PekgData.strPekgID.GetLength() < PEKG_ID_LEN)){
		theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("参数异常:数据包长度异常"));
		return false;
	}
	// 获取文件路径
	CString strUnsend = _T("");
	CString strHistory  = _T("");	
	strUnsend = m_dataPath + txnFileNames.unsendedFolderPath + PekgData.strPekgID;
	strHistory= m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(PekgData.strPekgID) + _T("\\") + PekgData.strPekgID;

	// 文件是否存在
	if(!IsFileExist(strUnsend)){
		if(!IsFileExist(strHistory)){
			theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("指定数据包文件<") + PekgData.strPekgID + _T(">不存在"));
			return false;
		}
		else{
			ReadPekgFile(strHistory,PekgData);			
		}		
	}
	else{
		ReadPekgFile(strUnsend,PekgData);
	}
	theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T(">>"));
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取队列中的数据包

@param      (i)PEKG_DATA& PekgData	 数据包结构体

@retval     bool true：成功  false：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTxnDataMgr::GetDataPekg(PEKG_DATA& pekgData)
{
	theDEBUG_LOG->WriteData(className,_T("GetDataPekg"),__LINE__,_T("<<"));
	bool ret = true;
	SYNC(QUEUE,_T("m_queUnSendPekgName"));
	if(!m_queUnSendPekgName.empty()){
		CString sPekgName = _T("");	
		sPekgName = m_queUnSendPekgName.front();
		pekgData.strPekgID = sPekgName;
		ret = GetDataPekgById(pekgData);
		m_queUnSendPekgName.pop();
	}
	else{
		ret = false;
	}
	theDEBUG_LOG->WriteData(className,_T("GetDataPekg"),__LINE__,_T(">>"));
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置数据包已送标志

@param      （i）CString数据包ID

@retval     bool   true:成功   false：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::SetPekgSended(CString strPekgName)
{
	theDEBUG_LOG->WriteData(className,_T("SetPekgSended"),__LINE__,_T("<<"));
	if(strPekgName.GetLength() < PEKG_ID_LEN){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__,_T("SetPekgUnSended(")+strPekgName+_T(")")); 
	}

	// 获取文件路径
	CString strDest = _T("");
	CString strSrc = _T("");
	CString strDestbackup = _T("");
	CString strSrcbackup = _T("");
	strDest = m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\") + strPekgName;
	strSrc = m_dataPath + txnFileNames.unsendedFolderPath + strPekgName;

	strDestbackup = m_BackupPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\") + strPekgName;
	strSrcbackup = m_BackupPath + txnFileNames.unsendedFolderPath + strPekgName;

	// 检查并创建目标目录
	ComCheckAndCreateDirectory(m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\"));

	// 源文件是否存在
	if(!IsFileExist(strSrc)){			
		throw CTxnDataException(CTxnDataException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T("源文件不存在<") + strSrc + _T(">")); 
	}
	else{
		// 移动数据包文件至已送文件夹
		SYNC(FILE,strSrc);
		BOOL moveFileSuccess = ::MoveFileEx(strSrc,strDest,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
		if ( !moveFileSuccess) { 
			for(int i=0;i<2;i++)
			{
				::Sleep(100);
				moveFileSuccess = ::MoveFileEx(strSrc,strDest,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
				if(moveFileSuccess)
				{
					break;
				}
			}
			if(!moveFileSuccess)
			{
				throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("移动文件失败：") + strSrc + _T("=>") + strDest);
			}	
		}
	}

	// 检查并创建目标目录
	ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\"));
	// 备份文件
	if(!IsFileExist(strSrcbackup)){			
		throw CTxnDataException(CTxnDataException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T("源文件不存在<") + strSrcbackup + _T(">")); 
	}
	else{
		// 移动数据包文件至已送文件夹
		SYNC(FILE,strSrcbackup);
		BOOL moveFileSuccess = ::MoveFileEx(strSrcbackup,strDestbackup,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
		if ( !moveFileSuccess) { 
			for(int i=0;i<2;i++)
			{
				::Sleep(100);
				// 如果文件存在且只读，修改属性
				if(PathFileExists(strDestbackup)){
					DWORD dwAttributes = ::GetFileAttributes(strDestbackup);
					if(dwAttributes & FILE_ATTRIBUTE_READONLY){
						SetFileAttributes(strDestbackup,FILE_ATTRIBUTE_NORMAL);
					}
				}
				moveFileSuccess = ::MoveFileEx(strSrcbackup,strDestbackup,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
				if(moveFileSuccess)
				{
					break;
				}
			}
			if(!moveFileSuccess)
			{
				throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("移动文件失败：") + strSrcbackup + _T("=>") + strDestbackup);	
			}
		}
	}
	if(txnKind == ACC_PEKG)
	{
		ProtectUnsendPackage();
	}
	theDEBUG_LOG->WriteData(className,_T("SetPekgSended"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断未送数据是否过多

@param      (i) COleDateTime  dtDateTime		系统运营日

@retval     bool
true:未送数据过多  false:未送数据没有过多

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTxnDataMgr::HasTooMoreUnSendData()
{
	theDEBUG_LOG->WriteData(className,_T("HasTooMoreUnSendData"),__LINE__,_T("<<"));
	bool retval = false;

	CString strUnsendPath = _T("");		
	strUnsendPath = m_dataPath + txnFileNames.unsendedFolderPath;

	if(GetUnsendDayCount(strUnsendPath) > m_iUnsendMaxDay){
		retval = true;
	}
	theDEBUG_LOG->WriteData(className,_T("HasTooMoreUnSendData"),__LINE__,_T(">>"));
	return retval;

}

////////////////////////////////////////////////////////////////////////
/**
@brief      删除过多的已送数据

@param      无

@retval     int  0:成功  1:失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::DeleteTooMoreSendedData(vector<CString> vecFname,unsigned int days)
{
	theDEBUG_LOG->WriteData(className,_T("DeleteTooMoreSendedData"),__LINE__,_T("<<"));
	unsigned int deleteCnt = (unsigned int)(vecFname.size() - days);
	CString sPath = _T("");

	SHFILEOPSTRUCT fos ;
	ZeroMemory( &fos, sizeof( fos)) ;
	fos.hwnd = NULL;
	fos.wFunc = FO_DELETE ;
	fos.fFlags = FOF_SILENT  | FOF_NOCONFIRMATION;

	TCHAR DelFileSource[MAX_PATH];
	for(unsigned int i = 0;i < deleteCnt; i++){		 
		// 删除主数据目录下过多数据
		sPath = m_dataPath + txnFileNames.sendedFolderPath + vecFname[i];
		if (PathFileExists(sPath)) {
			_tcsncpy(DelFileSource,sPath,MAX_PATH);
			fos.pFrom = DelFileSource;
			SHFileOperation( &fos);
		}

		// 删除备份数据目录下过多数据
		sPath = m_BackupPath + txnFileNames.sendedFolderPath + vecFname[i];
		if (PathFileExists(sPath)) {
			_tcsncpy(DelFileSource,sPath,MAX_PATH);
			fos.pFrom = DelFileSource;
			SHFileOperation( &fos);
		}
	}
	theDEBUG_LOG->WriteData(className,_T("DeleteTooMoreSendedData"),__LINE__,_T(">>"));
}






