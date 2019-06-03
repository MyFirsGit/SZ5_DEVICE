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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTxnDataMgr::CTxnDataMgr()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTxnDataMgr::~CTxnDataMgr()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ʼ��

@param      (i)CString strDataPath			������Ŀ¼
@param      oCString strBackupPath			��������Ŀ¼

@retval       ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::Internalize(CString strDataPath,CString strBackupPath)
{
	m_dataPath = strDataPath;
	m_BackupPath = strBackupPath;
	// ��鲢�������������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(m_dataPath + txnFileNames.unsendedFolderPath)) { 
		return ;
	}
	if (! ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.unsendedFolderPath)) { 
		return ;
	}

	// ��鲢����ACC������ʷ�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(m_dataPath + txnFileNames.sendedFolderPath + GetTodayDate())) { 
		return ;
	}
	if (! ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.sendedFolderPath + GetTodayDate())) { 
		return ;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����������

@param     (i)bool IsValidateUnSendPackage  �Ƿ���֤δ��������Ч��
@param     (i)int iSendedDataMaxDay			  �������ݱ����������
@param     (i)int iUnSendDataMaxDay			  δ�����ݱ����������

@retval     int   0���ɹ�   1������Ŀ¼ʧ��   2��δ�����ݹ���   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTxnDataMgr::InitTxnData(bool IsValidateUnSendPackage,int iSendedDataMaxDay,int iUnSendDataMaxDay)
{
	theDEBUG_LOG->WriteData(className,_T("InitTxnData"),__LINE__,_T("<<"));
	m_iUnsendMaxDay = iUnSendDataMaxDay;
	// ���ػ�δ������
	ResumeUnsendData(txnCode,m_dataPath,m_BackupPath);
	//ProtectUnsendPackage();
	if(txnKind == ACC_PEKG && IsValidateUnSendPackage)
	{
		ValidateUnsendPackage();
	}
	
	// ������δ����ļ����ȴ��
	CString currentFileName = m_dataPath +  txnFileNames.currentFileName;
	if(IsFileExist(currentFileName)){	
		MakePekg(true);
	}

	// ���δ�������Ƿ����
	if(HasTooMoreUnSendData()){
		throw CTxnDataException(CTxnDataException::SALEDATA_NOT_SEND_TOO_MORE, _T(__FILE__), __LINE__); 
	}
	else{
		// ��ȡδ���ļ���
		CString strUnsendPath = _T("");		
		strUnsendPath = m_dataPath + txnFileNames.unsendedFolderPath;
		GetUnsendFileName(strUnsendPath,m_queUnSendPekgName);
	}

	// ������������Ƿ����
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
@brief      ���潻������

@param      WORD txnLen		UD���ݳ��ȣ�����AFCϵͳר��ͷ��10���ֽڣ�
@param      LPBYTE txnBuf   UD�����壨����AFCϵͳר��ͷ��10���ֽڣ�

@retval     ��  

@exception  ��
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
		strException = _T("�쳣��"); 
		strException += strCause; 
		throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	} 
	try{
		BYTE currentPekgID[PEKG_ID_LEN];
		LPBYTE lpPekgID = currentPekgID;

		*lpPekgID = 0x01;
		lpPekgID ++;
		// ���ݰ�ID �����ݰ�����+�豸ID+ʱ���+������к�
		memset(lpPekgID,0x00,14);
		lpPekgID += 14;

		long  pekgLen = txnLen;
		WORD  recordCount = 1;
		
		cfFile.SeekToBegin(); 
		INT64 fileLength = cfFile.GetLength();
		if(fileLength>PEKG_ID_LEN)//˵����ǰ�б��棬����°����ȺͰ��м�¼����
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
			pekgLen+=PEKG_ID_LEN;//�׸������ϰ�ID��
		}
		
		// ���ݰ�����
		ComRevLong(ltoml(pekgLen),lpPekgID);		
		lpPekgID += 4;
		// ���м�¼��
		ComRevWORD(stoms(recordCount),lpPekgID);			
		lpPekgID += 2;
		// Ԥ��
		memset(lpPekgID,0xFF,7);

		cfFile.SeekToBegin();//����ͷ��
		cfFile.Write(currentPekgID,PEKG_ID_LEN);

		cfFile.SeekToEnd();//�ƶ���ĩβ��ҵ�����ݡ�
		cfFile.Write(txnBuf,txnLen);
		cfFile.Flush();
		cfFile.Close();
	} 
	catch(...)
	{
		cfFile.Close();
		throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
	}
	// ���潻��������ݣ������߲����ã�
	//TxnDataSaved.Invoke(txnLen,txnBuf);
	theDEBUG_LOG->WriteData(className,_T("SaveTxnData"),__LINE__,_T(">>"));
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���µ�ǰ���ݰ�ID

@param      ��

@retval     CString   ���ݰ�ID

@exception  ��
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
		strException = _T("�쳣��"); 
		strException += strCause; 
		throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	} 
	try{
		BYTE savedPekgID[PEKG_ID_LEN];
		cfFile.Read(savedPekgID,PEKG_ID_LEN);
		LPBYTE lpData = savedPekgID;

		// ����ʽ�汾��		
		lpData ++;
		// ���ݰ�ID �����ݰ�����+�豸ID+ʱ���+������к�	
		// ���ݰ�����
		*lpData = txnKind;
		lpData ++;
		// �豸ID
		theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpData);
		lpData+=4;
		// ʱ���
		_DATE_TIME dtTm = ComGetCurTime();
		dtTm.Serialize(lpData);
		lpData+=7;
		// ��ô�����к�			
		WORD selNo = theSELNO_MGR.GetSelNo(dtTm);
		ComRevWORD(selNo,lpData);	
		lpData += 2;	
		cfFile.SeekToBegin();//����ͷ��
		cfFile.Write(savedPekgID,PEKG_ID_LEN);
		cfFile.Flush();
		cfFile.Close();
		// ��ȡ�ļ���
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
@brief      �������

@param      ��

@retval     ��

@exception  ��
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
			// ���µ�ǰ���ݰ�ID
			SYNC(FILE,currentFileName);
			fileName = UpdateCurrentPekgID();
			strUnsend = m_dataPath + txnFileNames.unsendedFolderPath + fileName;
			strUnsendbackup = m_BackupPath + txnFileNames.unsendedFolderPath + fileName;
			// ��ǰ���ݰ��ļ���δ��Ŀ¼
			BOOL moveFileSuccess = ::MoveFileEx(currentFileName,strUnsend,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
			if( !moveFileSuccess){
				for(int i=0;i<2;i++) //��������
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
					throw CTxnDataException(CTxnDataException::FILE_CREATE_FAIL, _T(__FILE__), __LINE__, _T("����ļ��ƶ�ʧ��:") + currentFileName + _T(">") +strUnsend);
				}
			}

			BOOL copyFileSuccess =::CopyFile(strUnsend,strUnsendbackup,FALSE); 
			if( !copyFileSuccess)
			{
				for(int i=0;i<2;i++) //��������
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
					throw CTxnDataException(CTxnDataException::FILE_CREATE_FAIL, _T(__FILE__), __LINE__, _T("����ļ��ƶ�ʧ��:") + strUnsend+ _T(">") + strUnsendbackup);
				}
			}
			
			// ��յ�ǰ�ڴ����ݰ�����
			m_currentPekg.clear();
			if(!isInit){
				// �����ݰ��ļ�������δ�Ͷ���
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
@brief      ����δ�����ݰ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::EncryptUnsendPackage(_DATE_TIME& currentTime,VARIABLE_DATA& data)
{
	CString strPlainText = _T("");
	strPlainText = strPlainText + currentTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"));

	CString strUnsendPath =  m_dataPath + txnFileNames.unsendedFolderPath;
	
	// �����ļ�
	CFileFind finder;
	CString sFileName = _T("");
	BOOL bContinue = finder.FindFile(strUnsendPath +_T("*"));

	// ���û���ҵ��ļ����͹ر��ļ�
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
			// ȡ���ļ��������ڱȽ�����
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
@brief      ��δ�����ݰ�����д��һ���ļ���

@param      ��

@retval     ��

@exception  ��
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
@brief      �����ļ��͵�ǰδ���Ƿ�һ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::ValidateUnsendPackage()
{
	bool resueFileSuccess = CDXFile::ResuceFile(m_dataPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),m_BackupPath+txnFileNames.txnFolderPath+CString(PROTECT_FILE_NAME),true);
	if(!resueFileSuccess)
	{
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("����δ������ʧ��"));
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
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("����δ������ʧ��"));
	}
	if(memcmp(lpSaveEncryptData,lpNewEncryptData,encryptUnsendPackageData.nLen)!=0)
	{
		throw CTxnDataException(CTxnDataException::VALIDATE_NOT_SEND_FAILD,_T(__FILE__),__LINE__,_T("����δ������ʧ��"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ����δ������

@param      ��

@retval     int δ�����ݰ�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CTxnDataMgr::HasUnsendData()
{
	theDEBUG_LOG->WriteData(className,_T("HasUnsendData"),__LINE__,_T("<<"));

	SYNC(QUEUE,_T("m_queUnSendPekgName"));
	unsigned long retCnt = 0;
	// ���в��գ�����δ�����ݰ�������
	if(!m_queUnSendPekgName.empty()){
		retCnt = (unsigned long)m_queUnSendPekgName.size();
	}
	// ����Ϊ�գ���δ���ļ�����ʱ�����������У�����δ�����ݰ�������
	else{
		CString strUnsendPath = _T("");		
		strUnsendPath = m_dataPath + txnFileNames.unsendedFolderPath;
		//���ļ�����ʱ������������
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
@brief      ��ȡָ�����ݰ�

@param      (i)PEKG_DATA& PekgData	 ���ݰ��ṹ��

@retval     bool true���ɹ�  false��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTxnDataMgr::GetDataPekgById(PEKG_DATA& PekgData) 
{
	theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("<<"));
	if((PekgData.strPekgID.GetLength() < PEKG_ID_LEN)){
		theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("�����쳣:���ݰ������쳣"));
		return false;
	}
	// ��ȡ�ļ�·��
	CString strUnsend = _T("");
	CString strHistory  = _T("");	
	strUnsend = m_dataPath + txnFileNames.unsendedFolderPath + PekgData.strPekgID;
	strHistory= m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(PekgData.strPekgID) + _T("\\") + PekgData.strPekgID;

	// �ļ��Ƿ����
	if(!IsFileExist(strUnsend)){
		if(!IsFileExist(strHistory)){
			theDEBUG_LOG->WriteData(className,_T("GetDataPekgById"),__LINE__,_T("ָ�����ݰ��ļ�<") + PekgData.strPekgID + _T(">������"));
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
@brief      ��ȡ�����е����ݰ�

@param      (i)PEKG_DATA& PekgData	 ���ݰ��ṹ��

@retval     bool true���ɹ�  false��ʧ��

@exception  ��
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
@brief      �������ݰ����ͱ�־

@param      ��i��CString���ݰ�ID

@retval     bool   true:�ɹ�   false��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTxnDataMgr::SetPekgSended(CString strPekgName)
{
	theDEBUG_LOG->WriteData(className,_T("SetPekgSended"),__LINE__,_T("<<"));
	if(strPekgName.GetLength() < PEKG_ID_LEN){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__,_T("SetPekgUnSended(")+strPekgName+_T(")")); 
	}

	// ��ȡ�ļ�·��
	CString strDest = _T("");
	CString strSrc = _T("");
	CString strDestbackup = _T("");
	CString strSrcbackup = _T("");
	strDest = m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\") + strPekgName;
	strSrc = m_dataPath + txnFileNames.unsendedFolderPath + strPekgName;

	strDestbackup = m_BackupPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\") + strPekgName;
	strSrcbackup = m_BackupPath + txnFileNames.unsendedFolderPath + strPekgName;

	// ��鲢����Ŀ��Ŀ¼
	ComCheckAndCreateDirectory(m_dataPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\"));

	// Դ�ļ��Ƿ����
	if(!IsFileExist(strSrc)){			
		throw CTxnDataException(CTxnDataException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T("Դ�ļ�������<") + strSrc + _T(">")); 
	}
	else{
		// �ƶ����ݰ��ļ��������ļ���
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
				throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("�ƶ��ļ�ʧ�ܣ�") + strSrc + _T("=>") + strDest);
			}	
		}
	}

	// ��鲢����Ŀ��Ŀ¼
	ComCheckAndCreateDirectory(m_BackupPath + txnFileNames.sendedFolderPath + GetPekgDate(strPekgName) + _T("\\"));
	// �����ļ�
	if(!IsFileExist(strSrcbackup)){			
		throw CTxnDataException(CTxnDataException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, _T("Դ�ļ�������<") + strSrcbackup + _T(">")); 
	}
	else{
		// �ƶ����ݰ��ļ��������ļ���
		SYNC(FILE,strSrcbackup);
		BOOL moveFileSuccess = ::MoveFileEx(strSrcbackup,strDestbackup,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
		if ( !moveFileSuccess) { 
			for(int i=0;i<2;i++)
			{
				::Sleep(100);
				// ����ļ�������ֻ�����޸�����
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
				throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("�ƶ��ļ�ʧ�ܣ�") + strSrcbackup + _T("=>") + strDestbackup);	
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
@brief      �ж�δ�������Ƿ����

@param      (i) COleDateTime  dtDateTime		ϵͳ��Ӫ��

@retval     bool
true:δ�����ݹ���  false:δ������û�й���

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
@brief      ɾ���������������

@param      ��

@retval     int  0:�ɹ�  1:ʧ��

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
		// ɾ��������Ŀ¼�¹�������
		sPath = m_dataPath + txnFileNames.sendedFolderPath + vecFname[i];
		if (PathFileExists(sPath)) {
			_tcsncpy(DelFileSource,sPath,MAX_PATH);
			fos.pFrom = DelFileSource;
			SHFileOperation( &fos);
		}

		// ɾ����������Ŀ¼�¹�������
		sPath = m_BackupPath + txnFileNames.sendedFolderPath + vecFname[i];
		if (PathFileExists(sPath)) {
			_tcsncpy(DelFileSource,sPath,MAX_PATH);
			fos.pFrom = DelFileSource;
			SHFileOperation( &fos);
		}
	}
	theDEBUG_LOG->WriteData(className,_T("DeleteTooMoreSendedData"),__LINE__,_T(">>"));
}






