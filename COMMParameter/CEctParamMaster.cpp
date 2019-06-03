#include "StdAfx.h"
#include "CEctParamMaster.h"
#include "LogException.h"
#include "getini.h"
#include "ExceptionLog.h"
#include "cparamexception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = _T(__FILE__);
#endif


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CEctParamMaster::CEctParamMaster(void):CParameterMaster()
{
	m_dataPath = "";
	m_backupPath = "";
	m_futVersion = 0;
	m_curVersion = 0;
	m_curFile = NULL;
	m_futFile = NULL;
	m_flag = true;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////// 
CEctParamMaster::~CEctParamMaster(void)
{
	try{
		if (NULL != m_curFile) {
			delete m_curFile;
			m_curFile = NULL;
		}
		if (NULL != m_futFile) {
			delete m_futFile;
			m_futFile = NULL;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief  ���ڻ�

@param  (i)CString fileName         ��ȡ���ļ���
@param  (i)bool readflag            ѡ���ȡ�ļ���־    true ��ǰ�汾�ļ�   false δ���汾�ļ�

@retval bool        true    �ɹ�    \n
false   ʧ��

@exception CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::Internalize(bool readflag,CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	m_curVersion = 0xFFFFFFFF;
	m_futVersion = 0xFFFFFFFF;
	m_packageHead.internalize();
	m_flag = readflag;
	GetParamFileName();	
	if ( m_curfileName == "" && m_futfileName == "" ){
		throw CParamException(CParamException::FILE_NOT_EXIST, _T(__FILE__), __LINE__);
	}
	SetCurVerType(m_curfileName);							// ���õ�ǰ�����汾����
	SetFutVerType(m_futfileName);							// ����δ�������汾����

	try{
		return ReadFile();		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}

}

/////////////////////////////////////////////////////////////////////////
/**
@brief  �õ���ǰ�汾

@param  ��

@retval DWORD version   �������汾 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
DWORD CEctParamMaster::GetCurMasterVer()
{
	return m_curVersion;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  �õ�future�汾

@param  ��

@retval DWORD version   future�������汾 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
DWORD CEctParamMaster::GetFutMasterVer()
{
	return m_futVersion;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  �õ���ǰ�汾��Ч����

@param  ��

@retval _DATE ��ǰ��Ч���� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
_DATE CEctParamMaster::GetCurStartDate()
{
	return m_curStartDate;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  �õ�δ���汾��Ч����

@param  ��

@retval _DATE δ���汾��Ч���� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
_DATE CEctParamMaster::GetFutStartDate()
{
	return m_futStartDate;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  ȡ�õ�ǰ�����汾����

@param  ��

@retval PARAM_VERSION_TYPE_CODE �����汾���� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CEctParamMaster::GetCurVerType()
{
	return m_curVersionType;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  ȡ��δ�������汾����

@param  ��

@retval PARAM_VERSION_TYPE_CODE �����汾���� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CEctParamMaster::GetFutVerType()
{
	return m_futVersionType;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  ���õ�ǰ�����汾����

@param  CString strFileName �����ļ���

@retval �� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::SetCurVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == "00"){
		m_curVersionType = CUR_VER;
	}
	else if(strTemp == "01"){
		m_curVersionType = FUT_VER;
	}
	else if(strTemp == "10"){
		m_curVersionType = CUR_TEST;
	}
	else if(strTemp == "11"){
		m_curVersionType = FUT_TEST;
	}
	else{
		m_curVersionType = CUR_VER;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  ����δ�������汾����

@param  CString strFileName �����ļ���

@retval �� 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::SetFutVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == "00"){
		m_futVersionType = CUR_VER;
	}
	else if(strTemp == "01"){
		m_futVersionType = FUT_VER;
	}
	else if(strTemp == "10"){
		m_futVersionType = CUR_TEST;
	}
	else if(strTemp == "11"){
		m_futVersionType = FUT_TEST;
	}
	else{
		m_futVersionType = FUT_VER;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  ���²�����Ϣ���ݲ�ʹ�ã�

@param  (i)const char * proBlacklistInfo        Ҫд�������
@param  (i)ULONG datalength                     Ҫд�����ݵĳ���
@param  (i)bool writeflag                       ѡ������ļ��ı�־ true ��ǰ�汾�ļ�    false δ���汾�ļ�

@retval bool  ret   true    �ɹ�
false   ʧ��

@exception CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::UpdateMasterFile(const char *lpData, ULONG nLen, bool writeflag)
{  
	try {
		if(writeflag) {   
			if (m_curFile->WriteFile(lpData, nLen) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
			lpData += 8;
			m_curVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		}
		else {
			if (m_futFile->WriteFile(lpData, nLen) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
			lpData += 8;
			m_futVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		}
		return true;
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�ļ�

@param      ��

@retval     bool        \n
true:��ȡ�ļ��ɹ�         false:��ȡ�ļ�ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::ReadFile()
{

	CString filePath = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
	CString backupPath = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
	try{
		if(m_curfileName != ""){
			// current�ļ�
			//if (NULL == m_curFile) {
			m_curFile = new CXFile(filePath, backupPath);
			//}
			VARIABLE_DATA curbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_curFile->ReadBinFile(curbinInfo) != CXFile::FILE_OPERATION_OK){
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
			// ��������ȷ��
			if (CheckDataInfo(curbinInfo)) {            
				LPBYTE lpData = curbinInfo.lpData;
				// ������Ϣͷ
				lpData += 8;
				// ��ǰ�汾��
				m_curVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData +=11;  

				// ��ǰ�汾��Ч����
				m_curStartDate =  BCD2Date((char*)lpData);
				lpData+=4;

				if(m_flag) {
					// ������ͷ
					ParseHead(curbinInfo.lpData);
					if(m_packageHead.sectionCount > 0){
						// �����ֶ���Ϣ
						ParseSectionInfo(curbinInfo.lpData+PACKAGE_HEAD_LENS);					
						// ����������
						ParseBody(curbinInfo.lpData);
					}
				}
			}
			else{  
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + filePath);
				return false;
			}
		}

		if(m_futfileName != ""){
			// future�ļ�
			filePath = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
			backupPath = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;   
			if (NULL == m_futFile) {
				m_futFile = new CXFile(filePath, backupPath);
			}
			VARIABLE_DATA futbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_futFile->ReadBinFile(futbinInfo) != CXFile::FILE_OPERATION_OK){
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
			// ��������ȷ��
			if (CheckDataInfo(futbinInfo)) {     
				// ��ȡ�ļ�������������Ϣ
				LPBYTE lpData = futbinInfo.lpData;
				// ������Ϣͷ
				lpData += 8;
				// version
				m_futVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData +=11;  
				// δ���汾��Ч����
				m_futStartDate =  BCD2Date((char*)lpData);
				lpData+=4;

				if(!m_flag) {
					// ������ͷ
					ParseHead(futbinInfo.lpData);
					if(m_packageHead.sectionCount > 0){
						// �����ֶ���Ϣ
						ParseSectionInfo(futbinInfo.lpData+PACKAGE_HEAD_LENS);					
						// ����������
						ParseBody(futbinInfo.lpData);
					}
				}               
			}
			else{           
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + backupPath);
				return false;
			}
		}
		return true;		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ����ݣ��ݲ�ʹ�ã�

@param      (o)  LPBYTE& lpData       �ļ�����
(i)  bool readFlag        true:��ȡ��ǰ�汾     false:��ȡδ���汾�ļ�
@retval     bool \n
true    ��ȡ�ɹ�           false   ��ȡʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::ReadMasterData(VARIABLE_DATA &binfileData, bool readFlag)
{
	try{
		if (readFlag) {
			// ��ȡ���ػ��ļ�ʧ��
			if (m_curFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{               
			// ��ȡ�ļ�ʧ��
			if (m_futFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
		}

		// ��ȡ�ɹ�
		return true;
	}
	catch(CSysException&) {
		throw ;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ��İ�ͷ����

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::ParseHead(LPBYTE lpData)
{
	try{
		// ����ʽ�汾
		m_packageHead.formatVersion = *lpData;
		lpData++;
		// ������Դ��
		m_packageHead.dataOrigin = *lpData;
		lpData++;
		// ���ݰ�����
		m_packageHead.packageLength = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;
		// �������ʹ���
		m_packageHead.dataTypeCode = BCD2int((char*)lpData, 2);
		lpData += 2;
		// �汾��
		m_packageHead.versionCode = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;
		// ����ʱ��
		m_packageHead.createDateTime = BCD2Time((char*)lpData);
		lpData += 7; 
		// ��Ч����
		m_packageHead.startDate = BCD2Date((char*)lpData);;
		lpData += 4;
		// ���ݷֶ�����
		m_packageHead.sectionCount = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
		lpData+=2;
		// Ԥ�� 3 Byte
		memcpy(m_packageHead.reverse,lpData,3);
		lpData += 3;        
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,m_curfileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������ȷ��

@param      (i)const VARIABLE_DATA&      ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::CheckDataInfo(const VARIABLE_DATA& binfileData)
{
	// �����ĳ���
	if (binfileData.nLen < PACKAGE_HEAD_LENS) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT���������쳣"));
		return false;
	}
	LPBYTE lpData = binfileData.lpData;

	// ����ʽ�汾��
	lpData ++;
	// ������Դ��
	lpData ++;
	// ���ݰ�����
	DWORD packageLength = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
	if (packageLength != binfileData.nLen){
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT���������쳣"));
		return false;
	}
	lpData += 4;
	// �������ʹ���
	lpData += 2;
	// �汾��
	lpData += 4;
	// ����ʱ��
	if (!ComCheckIsBCD((char*)lpData,7)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT��������ʱ���BCD��ʽ"));
		return false;
	}
	_DATE_TIME startDateTime;
	startDateTime = BCD2Time((char*)lpData);
	if (0 != ComCheckDateTimeValid(startDateTime)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT��������ʱ�䲻�Ϸ�"));
		return false;
	}
	lpData+=7;
	// ��Ч����
	if (!ComCheckIsBCD((char*)lpData,4)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT������Ч���ڷ�BCD��ʽ"));
		return false;
	}
	_DATE createDate;
	createDate =  BCD2Date((char*)lpData);
	lpData+=4;
	if (0 != ComCheckDateValid(createDate)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT������Ч���ڲ��Ϸ�"));
		return false;
	}
	// ���ݷֶ�����
	DWORD sectionCount = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
	lpData += 2;
	// Ԥ�� 3 Byte
	lpData += 3;
	if (sectionCount != 0){				
		// �ֶνṹ��
		lpData += (8*sectionCount);		
		// ������    
		if (!CheckBodyInfo(lpData)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT����������BCD�����"));
			return false;
		}		
	}
	else{
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT�����ֶ�����Ϊ0"));
		return false;
	}
	return true;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      (i)CString sPrefix      �ļ���ǰ׺

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::GetParamFileNameSub(CString sPrefix)
{
	try{
		//ȡBOM����Ŀ¼
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString szBOMINIFName = CString(szRunPath) + BOM_INI_FILE_NAME;
		CGetIni bomIni(szBOMINIFName);  
		m_dataPath = bomIni.GetDataString(BOM_INI_SECTION_APPLICATION, BOM_INI_KEY_DATAPATH);
		m_backupPath = bomIni.GetDataString(BOM_INI_SECTION_APPLICATION, BOM_INI_KEY_DATAPATH_BKP);
		//m_dataPath = theBOM_INFO.GetDataPath();		
		//m_backupPath = theBOM_INFO.GetBackupPath();

		m_curfileName = _T("");
		m_futfileName = _T("");

		CFileFind finder;
		CString sPath = _T("");
		CString sMainFile("");
		CString sBackupFile("");
		// ��ǰ����
		{
			// ��Ŀ¼����
			sPath = m_dataPath + PARAM_ECT_CURRENT_DIR;
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// ȡ���ļ���
					m_curfileName = finder.GetFileName(); 
					// ���ػ�����
					// �����ļ�û��
					sBackupFile = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// ����Ŀ¼����
			if(m_curfileName == ""){
				sPath = m_backupPath + PARAM_ECT_CURRENT_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {   
						// ȡ���ļ���
						m_curfileName = finder.GetFileName(); 						
					}
				}	
				// ���ػ�����
				// ���ļ�û�� && �����ļ���
				if(m_curfileName != ""){
					sMainFile = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					sBackupFile = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					CopyFile(sBackupFile,sMainFile,FALSE);
				}
			}
			finder.Close();
		}

		// ȡ��δ�������ļ���
		{
			sPath = m_dataPath + PARAM_ECT_FUTURE_DIR;		
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// ȡ���ļ��������ڱȽ�����
					m_futfileName = finder.GetFileName(); 		

					// ���ػ�����
					// �����ļ�û��
					sBackupFile = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// ����Ŀ¼����
			if(m_futfileName == ""){
				sPath = m_backupPath + PARAM_ECT_FUTURE_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {   
						// ȡ���ļ���
						m_futfileName = finder.GetFileName(); 						
					}
				}	
				// ���ػ�����
				// ���ļ�û�� && �����ļ���
				if(m_futfileName != ""){
					sMainFile = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					sBackupFile = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					CopyFile(sBackupFile,sMainFile,FALSE);
				}
			}
			finder.Close();
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}