#include "StdAfx.h"
#include "CTMessageAudit.h"
#include "ParameterDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTMessageAudit::CTMessageAudit(void)
{
	m_pekgAuditFile = NULL;
	m_pekgList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTMessageAudit::~CTMessageAudit(void)
{
	try{
		delete m_pekgAuditFile;
		m_pekgAuditFile = NULL;
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

@param      (i) CString dataPath    ����·��
@param      (i) CString backupPath  ����·��

@retval     bool

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::Internalize(CString dataPath, CString backupPath)
{
	// ȡ�����ػ����ļ�
	CString strFileName = dataPath + DEVICE_MASTER_DIR +PEKG_AUDIT_FILE_NAME;
	CString strBackupFilename = backupPath + DEVICE_MASTER_DIR + PEKG_AUDIT_FILE_NAME;

	try{
		if (NULL == m_pekgAuditFile) {
			m_pekgAuditFile = new CDXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;

		// ��ȡԤ��Ʊ�ļ�ʧ��
		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// ����ļ�����
		if ((binfileData.nLen < MIN_FILE_LENGTH)) {
			if (0 == binfileData.nLen) {
				m_pekgList.clear();	

				_DATE_TIME ZeroTime;
				m_StartDateTime = ZeroTime;
				m_EndDateTime = ZeroTime;

				BYTE fileDate[MIN_FILE_LENGTH];
				memset(fileDate,0x00,MIN_FILE_LENGTH);

				if(m_pekgAuditFile->WriteBinFile((char*)fileDate,MIN_FILE_LENGTH) != CXFile::FILE_OPERATION_OK) {
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
				}
				return true;
			}
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}

		// �������ݰ���Ϣ
		LPBYTE lpData = binfileData.lpData;		

		// ������ƿ�ʼʱ��
		m_StartDateTime.wYear = BCD2int((char*)lpData,2);     // ��
		lpData += 2;
		m_StartDateTime.biMonth = BCD2int((char*)lpData++,1); // ��
		m_StartDateTime.biDay = BCD2int((char*)lpData++,1);   // ��
		m_StartDateTime.biHour = BCD2int((char*)lpData++,1);   // ʱ
		m_StartDateTime.biMinute= BCD2int((char*)lpData++,1);   // ��
		m_StartDateTime.biSecond = BCD2int((char*)lpData++,1);   // ��

		// ������ƿ�ʼʱ��
		m_EndDateTime.wYear = BCD2int((char*)lpData,2);     // ��
		lpData += 2;
		m_EndDateTime.biMonth = BCD2int((char*)lpData++,1); // ��
		m_EndDateTime.biDay = BCD2int((char*)lpData++,1);   // ��
		m_EndDateTime.biHour = BCD2int((char*)lpData++,1);   // ʱ
		m_EndDateTime.biMinute= BCD2int((char*)lpData++,1);   // ��
		m_EndDateTime.biSecond = BCD2int((char*)lpData++,1);   // ��

		// ���ݰ�����
		m_PekgCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;

		// �������ݰ���Ϣ
		m_pekgList.clear();
		PEKG_INFO pekgInfo;
		for (ULONG i=0;i<m_PekgCnt && binfileData.nLen != MIN_FILE_LENGTH ;i++) {
			// ���ݰ�ID
			memcpy(pekgInfo.sPekgId,lpData,14);
			lpData += 14;
			// ���м�¼��
			pekgInfo.iRecordCnt = ComMakeWORD(*lpData,*(lpData+1));
			lpData += 2;
			// ������Ϣ����vector��
			m_pekgList.push_back(pekgInfo);
		}

		if(binfileData.lpData != NULL){
			delete binfileData.lpData;
			binfileData.lpData = NULL;
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
@brief      ��ȡ��������Ƶ���

@param      ULONG& len     ���ĳ���

@retval      LPBYTE  ��������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::GetPekgAuditMessage(VARIABLE_DATA& binfileData)
{
	try{
		// ��ȡ�ļ�
		m_criticalSection.Lock();

		// ���ô�����ƽ���ʱ��
		_DATE_TIME CurDateTime = ComGetCurTime();
		SetEndDateTime(CurDateTime);

		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		// ������ݣ��������
		ClearAuditData();
		SetStartDateTime(CurDateTime);

		m_criticalSection.Unlock();
		// ����ļ�����
		if ((binfileData.nLen <= MIN_FILE_LENGTH)) {
			return false;
		}
		return true;
	}
	// �쳣����
	catch(CSysException&) {
		m_criticalSection.Unlock();
		throw;
	}
	catch (...) {
		m_criticalSection.Unlock();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ׷���ļ�

@param      PEKG_INFO pekgInfo

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTMessageAudit::AddWriteFile(PEKG_INFO pekgInfo)
{
	LPBYTE lpData = NULL;
	try{
		m_pekgList.push_back(pekgInfo);

		VARIABLE_DATA binfileData;

		m_criticalSection.Lock();
		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {			
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		LPBYTE fileData = new BYTE[binfileData.nLen+16];	
		lpData = fileData;		
		memcpy(fileData,binfileData.lpData,binfileData.nLen);

		fileData += 14;

		DWORD PekgCnt = mltol(ComMakeLong(*fileData,*(fileData+1),*(fileData+2),*(fileData+3)));

		ComRevLong(mltol(PekgCnt + 1), fileData);
		fileData += 4;

		fileData += PekgCnt*16;

		memcpy(fileData,pekgInfo.sPekgId,14);

		fileData += 14;

		ComRevWORD(mstos(pekgInfo.iRecordCnt), fileData);

		if(m_pekgAuditFile->WriteBinFile((char*)lpData,binfileData.nLen + 16) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
		m_criticalSection.Unlock();
		if(lpData != NULL){
			delete []lpData;
			lpData = NULL;
		}			
		return;
	}
	// �쳣����
	catch(CSysException&) {
		m_criticalSection.Unlock();
		if(lpData != NULL){
			delete []lpData;
			lpData = NULL;
		}	
		throw;
	}
	catch (...) {
		m_criticalSection.Unlock();
		if(lpData != NULL){
			delete []lpData;
			lpData = NULL;
		}	
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƿ�ʼʱ��

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTMessageAudit::SetStartDateTime(_DATE_TIME startDateTime)
{
	try{
		VARIABLE_DATA binfileData;

		m_StartDateTime = startDateTime;

		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		LPBYTE fileData = binfileData.lpData;		

		LPBYTE lpDate = new BYTE[7];
		lpDate = Time2BCD(startDateTime,lpDate);
		memcpy(fileData,lpDate,7);
		delete lpDate;
		lpDate = NULL;

		if(m_pekgAuditFile->WriteBinFile((char*)binfileData.lpData,binfileData.nLen) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƽ���ʱ��

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTMessageAudit::SetEndDateTime(_DATE_TIME endDateTime)
{
	try{
		VARIABLE_DATA binfileData;

		m_EndDateTime = endDateTime;

		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		LPBYTE fileData = binfileData.lpData;
		fileData +=7;

		LPBYTE lpDate = new BYTE[7];
		lpDate = Time2BCD(endDateTime,lpDate);
		memcpy(fileData,lpDate,7);
		delete lpDate;
		lpDate = NULL;

		if(m_pekgAuditFile->WriteBinFile((char*)binfileData.lpData,binfileData.nLen) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ļ�����

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTMessageAudit::ClearAuditData()
{
	try{
		m_pekgList.clear();	
		_DATE_TIME ZeroTime;
		m_StartDateTime = ZeroTime;
		m_EndDateTime = ZeroTime;
		BYTE fileDate[MIN_FILE_LENGTH];
		memset(fileDate,0x00,MIN_FILE_LENGTH);
		if(m_pekgAuditFile->WriteBinFile((char*)fileDate,MIN_FILE_LENGTH) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��Ѿ����ÿ�ʼ���ʱ��

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::HasStartDateTime()
{
	if (0 != ComCheckDateTimeValid(m_StartDateTime)) {
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��Ѿ����ý������ʱ��

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::HasEndDateTime()
{
	if (0 != ComCheckDateTimeValid(m_EndDateTime)) {
		return false;
	}
	return true;
}
CTMessageAudit CTMessageAudit::theInstance;
