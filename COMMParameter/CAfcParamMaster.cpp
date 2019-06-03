#include "StdAfx.h"
#include "CAfcParamMaster.h"
#include "LogException.h"
#include "ExceptionLog.h"
#include "DebugLog.h"
#include "tvmInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CAfcParamMaster::m_dataPath = _T("");           // ���ļ�·��
CString CAfcParamMaster::m_backupPath = _T("");         // �����ļ�·��

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CString fileName    �ļ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcParamMaster::CAfcParamMaster(void):CParameterMaster()
{    
    m_curVersion = 0xFFFFFFFF;
	m_futVersion = 0xFFFFFFFF;
    m_curFile = NULL;
    m_futFile = NULL;
    m_flag = true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcParamMaster::~CAfcParamMaster(void)
{
    if (NULL != m_curFile) {
        delete m_curFile;
        m_curFile = NULL;
    }
    if (NULL != m_futFile) {
        delete m_futFile;
        m_futFile = NULL;
    }
}


bool CAfcParamMaster::readVersionDataInfo(CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	m_curVersion = 0xFFFFFFFF;
	m_futVersion = 0xFFFFFFFF;
	m_packageHead.internalize();
	GetParamFileName();
	if ( m_curfileName == _T("") && m_futfileName == _T("") ){
		throw CParamException(CParamException::FILE_NOT_EXIST, _T(__FILE__), __LINE__);
	}
	SetCurVerType(m_curfileName);							// ���õ�ǰ�����汾����
	SetFutVerType(m_futfileName);							// ����δ�������汾����
	try{
		if(m_curfileName != _T("")){
			CString filePath = m_dataPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
			CString backupPath = m_backupPath + PARAM_AFC_CURRENT_DIR + m_curfileName;

			// current�ļ�
			//if (NULL == m_curFile) {
			m_curFile = new CDXFile(filePath, backupPath);
			//}
			VARIABLE_DATA curbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_curFile->ReadBinFile(curbinInfo) != CXFile::FILE_OPERATION_OK){

				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
			// ��������ȷ��
			if (CheckDataInfo(curbinInfo)) 
			{            
				LPBYTE lpData = curbinInfo.lpData;
				// ������Ϣͷ
				lpData += 8;
				// ��ǰ�汾��
				m_curVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData +=11;  

				// ��ǰ�汾��Ч����
				m_curStartDate =  BCD2Date((char*)lpData);
				lpData+=4;

				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
			}
			else{

				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + filePath);
				return false;
			}
		}

		if(m_futfileName != _T("")){
			CString filePath = m_dataPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
			CString backupPath = m_backupPath + PARAM_AFC_FUTURE_DIR + m_futfileName;   

			//if (NULL == m_futFile) {
			m_futFile = new CDXFile(filePath, backupPath);
			//}
			VARIABLE_DATA futbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_futFile->ReadBinFile(futbinInfo) != CXFile::FILE_OPERATION_OK){
				
				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}

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

				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}
			}
			else{     
				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + backupPath);
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
				return false;
			}
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
@brief      ���ڻ��ļ�

@param      (i)CString dataPath       �ļ����·��
@param      (i)Cstring backupPath     �����ļ�·��

@retval     bool  \n
                true �ɹ�     false ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcParamMaster::Internalize(bool readflag,CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	m_curVersion = 0xFFFFFFFF;
	m_futVersion = 0xFFFFFFFF;
	m_packageHead.internalize();
    m_flag = readflag;
	GetParamFileName();
	if ( m_curfileName == _T("") && m_futfileName == _T("") ){
		throw CParamException(CParamException::FILE_NOT_EXIST, _T(__FILE__), __LINE__);
	}
	SetCurVerType(m_curfileName);							// ���õ�ǰ�����汾����
	SetFutVerType(m_futfileName);							// ����δ�������汾����
	try{
		if(m_curfileName != _T("")){
			CString filePath = m_dataPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
			CString backupPath = m_backupPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
	   
			// current�ļ�
			//if (NULL == m_curFile) {
				m_curFile = new CDXFile(filePath, backupPath);
			//}
			VARIABLE_DATA curbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_curFile->ReadBinFile(curbinInfo) != CXFile::FILE_OPERATION_OK){
				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
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

				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
			}
			else{    
				if(NULL != m_curFile){
					delete m_curFile;
					m_curFile = NULL;
				}
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + filePath);
				return false;
			}
		}

        // future�ļ�
		//if(m_futfileName == _T("")){			
		//	m_futStartDate.wYear = 0;
  //          m_futStartDate.biMonth = 0;
  //          m_futStartDate.biDay = 0;
		//	return true;
		//}	
		if(m_futfileName != _T("")){
			CString filePath = m_dataPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
			CString backupPath = m_backupPath + PARAM_AFC_FUTURE_DIR + m_futfileName;   

			//if (NULL == m_futFile) {
				m_futFile = new CDXFile(filePath, backupPath);
			//}
			VARIABLE_DATA futbinInfo;
			// ��ȡ�ļ���Ϣ��binInfo��
			if (m_futFile->ReadBinFile(futbinInfo) != CXFile::FILE_OPERATION_OK){
				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}
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

				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}
			}
			else{
				if(NULL != m_futFile){
					delete m_futFile;
					m_futFile = NULL;
				}
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("������Ч�Լ��ʧ�ܣ�") + backupPath);
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
				return false;
			}
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
@brief      ������ǰ�ļ�����

@param      (o)  LPBYTE& lpData       �ļ�����

@retval     bool \n
                true    ��ȡ�ɹ�           false   ��ȡʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcParamMaster::ReadMasterData(VARIABLE_DATA& binfileData,bool readFlag)
{
    try {
		if(readFlag)
		{
			// ��ȡ�ļ�
			if (m_curFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__,m_curfileName);
			}
		}
		else
		{
			// ��ȡ�ļ�
			if (m_futFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__,m_futfileName);
			}
		}

        return true;
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
@brief      ������־�ļ����ڴ�

@param      (i)const char*  lpData      Ҫ�������ļ�������
@param      (i)ULONG        nLen        Ҫ�������ļ������ݳ���

@retval     bool \n
                true: �ɹ�  false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcParamMaster::UpdateMasterFile(const char* lpData, ULONG nLen)
{
    try {
        // д�ļ�
        if (m_curFile->WriteBinFile(lpData,nLen) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__,m_curfileName);  
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
@brief      ������ǰ�ļ��İ�ͷ����

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcParamMaster::ParseHead(LPBYTE lpData)
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
		m_packageHead.startDate = BCD2Date((char*)lpData);		
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
DWORD CAfcParamMaster::GetCurMasterVer()
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
DWORD CAfcParamMaster::GetFutMasterVer()
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
_DATE CAfcParamMaster::GetCurStartDate()
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
_DATE CAfcParamMaster::GetFutStartDate()
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
PARAM_VERSION_TYPE_CODE CAfcParamMaster::GetCurVerType()
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
PARAM_VERSION_TYPE_CODE CAfcParamMaster::GetFutVerType()
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
void CAfcParamMaster::SetCurVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == _T("00")){
		m_curVersionType = CUR_VER;
	}
	else if(strTemp == _T("01")){
		m_curVersionType = FUT_VER;
	}
	else if(strTemp == _T("10")){
		m_curVersionType = CUR_TEST;
	}
	else if(strTemp == _T("11")){
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
void CAfcParamMaster::SetFutVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == _T("00")){
		m_futVersionType = CUR_VER;
	}
	else if(strTemp == _T("01")){
		m_futVersionType = FUT_VER;
	}
	else if(strTemp == _T("10")){
		m_futVersionType = CUR_TEST;
	}
	else if(strTemp == _T("11")){
		m_futVersionType = FUT_TEST;
	}
	else{
		m_futVersionType = FUT_VER;
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
bool CAfcParamMaster::CheckDataInfo(const VARIABLE_DATA& binfileData)
{
    // �����ĳ���
    if (binfileData.nLen < PACKAGE_HEAD_LENS) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC���������쳣"));
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
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC���������쳣"));
		return false;
    }
	lpData += 4;
    // �������ʹ���
    lpData += 2;
    // �汾��
    lpData += 4;
	// ����ʱ��
    if (!ComCheckIsBCD((char*)lpData,7)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC��������ʱ���BCD��ʽ"));
		return false;
    }
	_DATE_TIME startDateTime;
    startDateTime = BCD2Time((char*)lpData);
    if (0 != ComCheckDateTimeValid(startDateTime)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC��������ʱ�䲻�Ϸ�"));
        return false;
    }
	lpData+=7;
	// ��Ч����
    if (!ComCheckIsBCD((char*)lpData,4)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC������Ч���ڷ�BCD��ʽ"));
        return false;
    }
	_DATE createDate;
    createDate =  BCD2Date((char*)lpData);
    lpData+=4;
    if (0 != ComCheckDateValid(createDate)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC������Ч���ڲ��Ϸ�"));
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
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC����������BCD�����"));
			return false;
		}		
    }	
	else{
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("AFC�����ֶ�����Ϊ0"));
		return false;
	}
    return true;  
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      (i)const VARIABLE_DATA&      ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcParamMaster::GetParamFileNameSub(const TCHAR* sPrefix)
{
	try{
		m_dataPath = theTVM_INFO.GetDataPath();		
		m_backupPath = theTVM_INFO.GetBackupPath();

		m_curfileName = _T("");
		m_futfileName = _T("");

		CFileFind finder;
		CString sPath = _T("");
		CString sMainFile(_T(""));
		CString sBackupFile(_T(""));
		// ��ǰ����
		{
			// ��Ŀ¼����
			sPath = m_dataPath + PARAM_AFC_CURRENT_DIR;
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {
					// ɾ������������汾����Ϊ�յĲ���
					if(!m_curfileName.IsEmpty() || finder.GetLength() == 0){
						DeleteFile(finder.GetFilePath());
						continue;
					}

					// ȡ���ļ���
					m_curfileName = finder.GetFileName(); 
					// ���ػ�����
					// �����ļ�û��
					sBackupFile = m_backupPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// ����Ŀ¼����
			if(m_curfileName == _T("")){
				sPath = m_backupPath + PARAM_AFC_CURRENT_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {
						// ɾ������������汾�Ĳ���
						if(!m_curfileName.IsEmpty()|| finder.GetLength() == 0){
							DeleteFile(finder.GetFilePath());
							continue;
						}
						// ȡ���ļ���
						m_curfileName = finder.GetFileName(); 						
					}
				}	
				// ���ػ�����
				// ���ļ�û�� && �����ļ���
				if(m_curfileName != _T("")){
					sMainFile = m_dataPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
					sBackupFile = m_backupPath + PARAM_AFC_CURRENT_DIR + m_curfileName;
					CopyFile(sBackupFile,sMainFile,FALSE);
				}
			}
			finder.Close();
		}

		// ȡ��δ�������ļ���
		{
			sPath = m_dataPath + PARAM_AFC_FUTURE_DIR;		
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else { 
					// ɾ������������汾�Ĳ���
					if(!m_futfileName.IsEmpty()|| finder.GetLength() == 0){
						DeleteFile(finder.GetFilePath());
						continue;
					}

					// ȡ���ļ��������ڱȽ�����
					m_futfileName = finder.GetFileName(); 		

					// ���ػ�����
					// �����ļ�û��
					sBackupFile = m_backupPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// ����Ŀ¼����
			if(m_futfileName == _T("")){
				sPath = m_backupPath + PARAM_AFC_FUTURE_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {
						// ɾ������������汾�Ĳ���
						if(!m_futfileName.IsEmpty()|| finder.GetLength() == 0){
							DeleteFile(finder.GetFilePath());
							continue;
						}

						// ȡ���ļ���
						m_futfileName = finder.GetFileName(); 						
					}
				}	
				// ���ػ�����
				// ���ļ�û�� && �����ļ���
				if(m_futfileName != _T("")){
					sMainFile = m_dataPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
					sBackupFile = m_backupPath + PARAM_AFC_FUTURE_DIR + m_futfileName;
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