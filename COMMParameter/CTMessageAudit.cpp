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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTMessageAudit::CTMessageAudit(void)
{
	m_pekgAuditFile = NULL;
	m_pekgList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTMessageAudit::~CTMessageAudit(void)
{
	try{
		delete m_pekgAuditFile;
		m_pekgAuditFile = NULL;
	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}        
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化文件

@param      (i) CString dataPath    数据路径
@param      (i) CString backupPath  备份路径

@retval     bool

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::Internalize(CString dataPath, CString backupPath)
{
	// 取得两重化的文件
	CString strFileName = dataPath + DEVICE_MASTER_DIR +PEKG_AUDIT_FILE_NAME;
	CString strBackupFilename = backupPath + DEVICE_MASTER_DIR + PEKG_AUDIT_FILE_NAME;

	try{
		if (NULL == m_pekgAuditFile) {
			m_pekgAuditFile = new CDXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;

		// 读取预制票文件失败
		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// 检查文件长度
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

		// 解析数据包信息
		LPBYTE lpData = binfileData.lpData;		

		// 解析审计开始时间
		m_StartDateTime.wYear = BCD2int((char*)lpData,2);     // 年
		lpData += 2;
		m_StartDateTime.biMonth = BCD2int((char*)lpData++,1); // 月
		m_StartDateTime.biDay = BCD2int((char*)lpData++,1);   // 日
		m_StartDateTime.biHour = BCD2int((char*)lpData++,1);   // 时
		m_StartDateTime.biMinute= BCD2int((char*)lpData++,1);   // 分
		m_StartDateTime.biSecond = BCD2int((char*)lpData++,1);   // 秒

		// 解析审计开始时间
		m_EndDateTime.wYear = BCD2int((char*)lpData,2);     // 年
		lpData += 2;
		m_EndDateTime.biMonth = BCD2int((char*)lpData++,1); // 月
		m_EndDateTime.biDay = BCD2int((char*)lpData++,1);   // 日
		m_EndDateTime.biHour = BCD2int((char*)lpData++,1);   // 时
		m_EndDateTime.biMinute= BCD2int((char*)lpData++,1);   // 分
		m_EndDateTime.biSecond = BCD2int((char*)lpData++,1);   // 秒

		// 数据包总数
		m_PekgCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;

		// 解析数据包信息
		m_pekgList.clear();
		PEKG_INFO pekgInfo;
		for (ULONG i=0;i<m_PekgCnt && binfileData.nLen != MIN_FILE_LENGTH ;i++) {
			// 数据包ID
			memcpy(pekgInfo.sPekgId,lpData,14);
			lpData += 14;
			// 包中记录数
			pekgInfo.iRecordCnt = ComMakeWORD(*lpData,*(lpData+1));
			lpData += 2;
			// 将包信息放置vector中
			m_pekgList.push_back(pekgInfo);
		}

		if(binfileData.lpData != NULL){
			delete binfileData.lpData;
			binfileData.lpData = NULL;
		}

		return true;
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取包传输审计电文

@param      ULONG& len     电文长度

@retval      LPBYTE  电文内容

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTMessageAudit::GetPekgAuditMessage(VARIABLE_DATA& binfileData)
{
	try{
		// 读取文件
		m_criticalSection.Lock();

		// 设置传输审计结束时间
		_DATE_TIME CurDateTime = ComGetCurTime();
		SetEndDateTime(CurDateTime);

		if (m_pekgAuditFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		// 清除数据，重新审计
		ClearAuditData();
		SetStartDateTime(CurDateTime);

		m_criticalSection.Unlock();
		// 检查文件长度
		if ((binfileData.nLen <= MIN_FILE_LENGTH)) {
			return false;
		}
		return true;
	}
	// 异常捕获
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
@brief      追加文件

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
	// 异常捕获
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
@brief      设置审计开始时间

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
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置审计结束时间

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
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清空审计文件数据

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
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否已经设置开始审计时间

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
@brief      是否已经设置结束审计时间

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
