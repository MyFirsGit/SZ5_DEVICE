#include "StdAfx.h"
#include "CTLOGDeviceInfo.h"

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
CCTLOGDeviceInfo::CCTLOGDeviceInfo(void)
{
	m_file = NULL;
	memset(m_fileData, 0, DEVICE_INFO_LEN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGDeviceInfo::~CCTLOGDeviceInfo(void)
{
	try{
		delete m_file;
		m_file = NULL;
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
@brief      初期处理

@param      (i)CString dataPath         数据主目录
@param      (i)CString backupPath       备份主目录

@retval     BOOL \n
TRUE: 成功      FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGDeviceInfo::Internalize(CString dataPath, CString backupPath)
{
	try {
		CString file = dataPath + DEVICE_MASTER_DIR + DEVICE_INFO_FILE_NAME;
		CString fileBack = backupPath + DEVICE_MASTER_DIR + DEVICE_INFO_FILE_NAME;

		if(NULL == m_file){
			m_file = new CDXFile(file,fileBack);
		}

		// 读出二进制文件
		VARIABLE_DATA binfileData;
		if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		if (DEVICE_INFO_LEN != binfileData.nLen){
			if(0 == binfileData.nLen){
				memset(&m_fileData[0],0x00,DEVICE_INFO_LEN);
				if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
					memset(&m_deviceInfo,0x00,sizeof(m_deviceInfo));
					return TRUE;
				}
			}
			else{
				// 抛出异常
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{
			memcpy(m_fileData, binfileData.lpData, DEVICE_INFO_LEN);

			// 解析文件
			LPBYTE lpData = binfileData.lpData;
			// RW1的ACC ISAM卡号
			memcpy(m_deviceInfo.ACC_RW1_ISAM,lpData,6);
			lpData += 6;
			// RW1的ACC PSAM卡号
			memcpy(m_deviceInfo.ACC_RW1_PSAM,lpData,6);
			lpData += 6;		
			// RW1的ETC ISAM卡号 
			memcpy(m_deviceInfo.ECT_RW1_ISAM,lpData,6);
			lpData += 6;
			// RW1的ETC PSAM卡号
			memcpy(m_deviceInfo.ECT_RW1_PSAM,lpData,6);
			lpData += 6;

			// RW2的ACC ISAM卡号
			memcpy(m_deviceInfo.ACC_RW2_ISAM,lpData,6);
			lpData += 6;
			// RW2的ACC PSAM卡号
			memcpy(m_deviceInfo.ACC_RW2_PSAM,lpData,6);
			lpData += 6;
			// RW2的ETC ISAM卡号 
			memcpy(m_deviceInfo.ECT_RW2_ISAM,lpData,6);
			lpData += 6;
			// RW2的ETC PSAM卡号
			memcpy(m_deviceInfo.ECT_RW2_PSAM,lpData,6);
			lpData += 6;

			// RW3的ACC ISAM卡号
			memcpy(m_deviceInfo.ACC_RW3_ISAM,lpData,6);
			lpData += 6;
			// RW3的ACC PSAM卡号
			memcpy(m_deviceInfo.ACC_RW3_PSAM,lpData,6);
			lpData += 6;
			// RW3的ETC ISAM卡号
			memcpy(m_deviceInfo.ECT_RW3_ISAM,lpData,6);
			lpData += 6;
			// RW3的ETC PSAM卡号
			memcpy(m_deviceInfo.ECT_RW3_PSAM,lpData,6);
			lpData += 6;lpData += 6;

			// 外部读写器设备ID
			memcpy(m_deviceInfo.RW_DEVICE_ID,lpData,6);
			lpData += 6;
			// 内部读写器设备ID
			memcpy(m_deviceInfo.TW_DEVICE_ID,lpData,6);
			lpData += 6;
			// 打印机设备ID
			memcpy(m_deviceInfo.PRT_DEVICE_ID,lpData,6);
			lpData += 6;
			// 出票机设备ID
			memcpy(m_deviceInfo.TH_DEVICE_ID,lpData,6);
			lpData += 6;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取设备信息

@param      (o) DEVICE_INFO &svData        得到此Master的数据

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::GetDeviceInfo(DEVICE_INFO &svData)
{
	svData = m_deviceInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置RW1的ACC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW1_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW1_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW1_ISAM,samid,6);
		}
		else {
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
@brief      设置RW1的ACC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW1_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW1_PSAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW1_PSAM,samid,6);
		}
		else {
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
@brief      设置RW1的ETC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW1_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW1_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW1_ISAM,samid,6);
		}
		else {
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
@brief      设置RW1的ETC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW1_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW1_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW1_ISAM,samid,6);
		}
		else {
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
@brief      设置RW2的ACC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW2_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW2_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW2_ISAM,samid,6);
		}
		else {
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
@brief      设置RW2的ACC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW2_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW2_PSAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW2_PSAM,samid,6);
		}
		else {
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
@brief      设置RW2的ETC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW2_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW2_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW2_ISAM,samid,6);
		}
		else {
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
@brief      设置RW2的ETC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW2_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW2_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW2_ISAM,samid,6);
		}
		else {
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
@brief      设置RW3的ACC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW3_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW3_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW3_ISAM,samid,6);
		}
		else {
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
@brief      设置RW3的ACC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetACC_RW3_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ACC_RW3_PSAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ACC_RW3_PSAM,samid,6);
		}
		else {
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
@brief      设置RW3的ETC ISAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW3_ISAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW3_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW3_ISAM,samid,6);
		}
		else {
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
@brief      设置RW3的ETC PSAM卡号

@param      (i)LPBYTE samid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetETC_RW3_PSAM(LPBYTE samid)
{
	try{
		memcpy(&m_fileData[ECT_RW3_ISAM_POS],samid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.ECT_RW3_ISAM,samid,6);
		}
		else {
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
@brief      设置外部读写器设备ID

@param      (i)LPBYTE deviceid

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetRWId(LPBYTE deviceid)
{
	try{
		memcpy(&m_fileData[RW_DEVICE_ID_POS],&deviceid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.RW_DEVICE_ID,deviceid,6);
		}
		else {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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
@brief      设置内部读写器设备ID

@param      (i)LPBYTE deviceid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetTWId(LPBYTE deviceid)
{
	try{
		memcpy(&m_fileData[TW_DEVICE_ID_POS],deviceid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.TW_DEVICE_ID,deviceid,6);
		}
		else {
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
@brief      设置打印机设备ID

@param      (i)LPBYTE deviceid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetPRTId(LPBYTE deviceid)
{
	try{
		memcpy(&m_fileData[PRT_DEVICE_ID_POS],deviceid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.PRT_DEVICE_ID,deviceid,6);
		}
		else {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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
@brief      设置出票机设备ID

@param      (i)LPBYTE deviceid 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::SetTHId(LPBYTE deviceid)
{
	try{
		memcpy(&m_fileData[TH_DEVICE_ID_POS],deviceid,6);
		if(m_file->WriteBinFile(&m_fileData[0], DEVICE_INFO_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_deviceInfo.PRT_DEVICE_ID,deviceid,6);
		}
		else {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

CCTLOGDeviceInfo CCTLOGDeviceInfo::theInstance;
