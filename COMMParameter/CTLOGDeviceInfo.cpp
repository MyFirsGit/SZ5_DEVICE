#include "StdAfx.h"
#include "CTLOGDeviceInfo.h"

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
CCTLOGDeviceInfo::CCTLOGDeviceInfo(void)
{
	m_file = NULL;
	memset(m_fileData, 0, DEVICE_INFO_LEN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGDeviceInfo::~CCTLOGDeviceInfo(void)
{
	try{
		delete m_file;
		m_file = NULL;
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
@brief      ���ڴ���

@param      (i)CString dataPath         ������Ŀ¼
@param      (i)CString backupPath       ������Ŀ¼

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

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

		// �����������ļ�
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
				// �׳��쳣
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{
			memcpy(m_fileData, binfileData.lpData, DEVICE_INFO_LEN);

			// �����ļ�
			LPBYTE lpData = binfileData.lpData;
			// RW1��ACC ISAM����
			memcpy(m_deviceInfo.ACC_RW1_ISAM,lpData,6);
			lpData += 6;
			// RW1��ACC PSAM����
			memcpy(m_deviceInfo.ACC_RW1_PSAM,lpData,6);
			lpData += 6;		
			// RW1��ETC ISAM���� 
			memcpy(m_deviceInfo.ECT_RW1_ISAM,lpData,6);
			lpData += 6;
			// RW1��ETC PSAM����
			memcpy(m_deviceInfo.ECT_RW1_PSAM,lpData,6);
			lpData += 6;

			// RW2��ACC ISAM����
			memcpy(m_deviceInfo.ACC_RW2_ISAM,lpData,6);
			lpData += 6;
			// RW2��ACC PSAM����
			memcpy(m_deviceInfo.ACC_RW2_PSAM,lpData,6);
			lpData += 6;
			// RW2��ETC ISAM���� 
			memcpy(m_deviceInfo.ECT_RW2_ISAM,lpData,6);
			lpData += 6;
			// RW2��ETC PSAM����
			memcpy(m_deviceInfo.ECT_RW2_PSAM,lpData,6);
			lpData += 6;

			// RW3��ACC ISAM����
			memcpy(m_deviceInfo.ACC_RW3_ISAM,lpData,6);
			lpData += 6;
			// RW3��ACC PSAM����
			memcpy(m_deviceInfo.ACC_RW3_PSAM,lpData,6);
			lpData += 6;
			// RW3��ETC ISAM����
			memcpy(m_deviceInfo.ECT_RW3_ISAM,lpData,6);
			lpData += 6;
			// RW3��ETC PSAM����
			memcpy(m_deviceInfo.ECT_RW3_PSAM,lpData,6);
			lpData += 6;lpData += 6;

			// �ⲿ��д���豸ID
			memcpy(m_deviceInfo.RW_DEVICE_ID,lpData,6);
			lpData += 6;
			// �ڲ���д���豸ID
			memcpy(m_deviceInfo.TW_DEVICE_ID,lpData,6);
			lpData += 6;
			// ��ӡ���豸ID
			memcpy(m_deviceInfo.PRT_DEVICE_ID,lpData,6);
			lpData += 6;
			// ��Ʊ���豸ID
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
@brief      ��ȡ�豸��Ϣ

@param      (o) DEVICE_INFO &svData        �õ���Master������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGDeviceInfo::GetDeviceInfo(DEVICE_INFO &svData)
{
	svData = m_deviceInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����RW1��ACC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW1��ACC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW1��ETC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW1��ETC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW2��ACC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW2��ACC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW2��ETC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW2��ETC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW3��ACC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW3��ACC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW3��ETC ISAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      ����RW3��ETC PSAM����

@param      (i)LPBYTE samid 

@retval     ��

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
@brief      �����ⲿ��д���豸ID

@param      (i)LPBYTE deviceid

@retval     ��

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
@brief      �����ڲ���д���豸ID

@param      (i)LPBYTE deviceid 

@retval     ��

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
@brief      ���ô�ӡ���豸ID

@param      (i)LPBYTE deviceid 

@retval     ��

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
@brief      ���ó�Ʊ���豸ID

@param      (i)LPBYTE deviceid 

@retval     ��

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
