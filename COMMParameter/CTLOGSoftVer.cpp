#include "StdAfx.h"
#include "CTLOGSoftVer.h"


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
CCTLOGSoftVer::CCTLOGSoftVer(void)
{
	m_file = NULL;
	memset(m_fileData, 0, SOFT_VER_LEN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGSoftVer::~CCTLOGSoftVer(void)
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
BOOL CCTLOGSoftVer::Internalize(CString dataPath, CString backupPath)
{
	try {
		CString file = dataPath + DEVICE_MASTER_DIR + SOFTVER_FILE_NAME;
		CString fileBack = backupPath + DEVICE_MASTER_DIR + SOFTVER_FILE_NAME;

		m_file = new CDXFile(file , fileBack);

		// �����������ļ�
		VARIABLE_DATA binfileData;
		if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		if (SOFT_VER_LEN != binfileData.nLen){
			if(0==binfileData.nLen){
				memset(&m_fileData[0],0x00,SOFT_VER_LEN);
				if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
					memset(&m_svSoftVersion,0x00,sizeof(m_svSoftVersion));
					return TRUE;
				}
			}
			else{
				// �׳��쳣
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}

		memcpy(m_fileData, binfileData.lpData, SOFT_VER_LEN);

		// �����ļ�
		LPBYTE lpData = binfileData.lpData;
		for(int i = 0; i<6; i++){                                           // ��д��SAM ID1
			m_svSoftVersion.ulRWSAMID1[i] = *lpData;     
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // ��д��SAM ID2
			m_svSoftVersion.ulRWSAMID2[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // ��д��SAM ID3
			m_svSoftVersion.ulRWSAMID3[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // ��д��SAM ID4
			m_svSoftVersion.ulRWSAMID4[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // �ⲿ��д��SAM ID1
			m_svSoftVersion.ulOutRWSAMID1[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // �ⲿ��д��SAM ID2
			m_svSoftVersion.ulOutRWSAMID2[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // �ⲿ��д��SAM ID3
			m_svSoftVersion.ulOutRWSAMID3[i] = *lpData; 
			lpData++;
		}
		for(int i = 0; i<6; i++){                                           // �ⲿ��д��SAM ID4
			m_svSoftVersion.ulOutRWSAMID4[i] = *lpData; 
			lpData++;
		}
		// ���Ƴ���
		memcpy(m_svSoftVersion.ulAppliVersion, lpData, 4);
		// m_svSoftVersion.ulAppliVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		lpData += 4;
		// RW Boot�汾��Ϣ
		memcpy(m_svSoftVersion.rwBootVer,lpData,4);
		lpData += 4;
		//memcpy(m_svSoftVersion.thBootVersion, lpData, 2);
		//lpData += 2;
		// Ԥ��
		//lpData += 2;
		// TH RW
		memcpy(m_svSoftVersion.ulTHRWVersion, lpData, 4);
		// m_svSoftVersion.ulTHRWVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		lpData += 4;
		// IC RW
		memcpy(m_svSoftVersion.ulICRWVersion, lpData, 4);
		// m_svSoftVersion.ulICRWVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));            
		lpData += 4;
		// TH��ʽ
		memcpy(m_svSoftVersion.thInfo.type,lpData,10);                      
		lpData += 10;
		// TH�汾
		memcpy(m_svSoftVersion.thInfo.version,lpData,2);                    
		lpData += 2;
		// TH��SAMֵ
		m_svSoftVersion.thInfo.sum = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)); 
		lpData += 4;

		// TH��ʽ(boot)
		memcpy(m_svSoftVersion.thBootInfo.type,lpData,10);                      
		lpData += 10;
		// TH�汾(boot)
		memcpy(m_svSoftVersion.thBootInfo.version,lpData,2);                    
		lpData += 2;
		// TH��SAMֵ(boot)
		m_svSoftVersion.thBootInfo.sum = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)); 
		lpData += 4;
		// THRW Boot�汾��Ϣ
		memcpy(m_svSoftVersion.thRwBootVer,lpData,4);
		lpData += 4;
		// ģʽ�����汾����
		memcpy(&m_svSoftVersion.dwModeHistory, lpData, 4);
		lpData += 4;

		// BOM����汾��Ϣ
		memcpy(&m_svSoftVersion.BomSoft_info.wSerialNo, lpData, 2);
		lpData += 2;
		m_svSoftVersion.BomSoft_info.bVerType = *lpData;
		lpData ++;
		memcpy(&m_svSoftVersion.BomSoft_info.dwVersion, lpData, 4);
		lpData += 4;
		lpData = NULL;
		// �ж�RW boot���汾�Ƿ�Ϊ0
		if ( m_svSoftVersion.rwBootVer[0] == 0 && m_svSoftVersion.rwBootVer[1] == 0 && m_svSoftVersion.rwBootVer[2] == 0 && m_svSoftVersion.rwBootVer[3] == 0 ) {
			char ver[4] = {0};
			ver[0] = 0x30;
			ver[1] = 0x30;
			ver[2] = m_svSoftVersion.ulICRWVersion[2];
			ver[3] = m_svSoftVersion.ulICRWVersion[3];
			SetRWBootVer(ver);
		}
		if ( m_svSoftVersion.thRwBootVer[0] == 0 && m_svSoftVersion.thRwBootVer[1] == 0 && m_svSoftVersion.thRwBootVer[2] == 0 && m_svSoftVersion.thRwBootVer[3] == 0) {
			char ver[4] = {0};
			ver[0] = 0x30;
			ver[1] = 0x30;
			ver[2] = m_svSoftVersion.ulTHRWVersion[2];
			ver[3] = m_svSoftVersion.ulTHRWVersion[3];
			SetTHRWBootVer(ver);
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
@brief      ��ȡ�汾��Ϣ

@param      (o) VERSION_INFO &svData        �õ���Master������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::GetSoftVerData(VERSION_INFO &svData)
{
	svData = m_svSoftVersion;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ö�д��SAM��ID

@param      (i) SLOT_ID slot            ID��
slot = 1                ��д��SAM ID1
slot = 2                ��д��SAM ID2
slot = 3                ��д��SAM ID3
slot = 4                ��д��SAM ID4
(i) const BYTE[6] ulID      �����Ϣ

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetRWSAMID(SLOT_ID slot, const BYTE ulID[])
{
	BOOL iret;
	try {
		memcpy(&m_fileData[RW_SAMID_POS+6*(slot)],ulID,6);

		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			switch(slot)
			{
			case SLOT1:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulRWSAMID1[i] = ulID[i];
				}
				break;
			case SLOT2:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulRWSAMID2[i] = ulID[i];
				}
				break;
			case SLOT3:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulRWSAMID3[i] = ulID[i];
				}
				break;
			case SLOT4:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulRWSAMID4[i] = ulID[i];
				}
				break;
			default:
				throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);  
			}
			iret =  TRUE;
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
@brief      �����ⲿ��д��SAM��ID

@param      (i) SLOT_ID                 ID��    \n
slot = 1                �ⲿ��д��SAM ID1   \n
slot = 2                �ⲿ��д��SAM ID2   \n
slot = 3                �ⲿ��д��SAM ID3   \n
slot = 4                �ⲿ��д��SAM ID4   \n
@param      (i) const BYTE[6] ulID      �����Ϣ

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetOutRWSAMID(SLOT_ID slot, const BYTE ulID[6])
{
	try{

		memcpy(&m_fileData[OUTRW_SAMID_POS+6*(slot)],ulID,6);

		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			switch(slot)
			{
			case SLOT1:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulOutRWSAMID1[i] = ulID[i];
				}
				break;
			case SLOT2:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulOutRWSAMID2[i] = ulID[i];
				}
				break;
			case SLOT3:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulOutRWSAMID3[i] = ulID[i];
				}
				break;
			case SLOT4:
				for(int i = 0;i<6;i++){
					m_svSoftVersion.ulOutRWSAMID4[i] = ulID[i];
				}
				break;
			default:
				throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);  
			}
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
@brief      ���ÿ��Ʋ��汾

@param      (i) const char ulVersion[]        �汾��Ϣ

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGSoftVer::SetAPIVersion(const char ulVersion[])
{
	try{
		//ULONG ulValue = ltoml(ulVersion);
		memcpy(&m_fileData[API_VERSION_POS],ulVersion,4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_svSoftVersion.ulAppliVersion, ulVersion, 4);
			//m_svSoftVersion.ulAppliVersion = ulVersion;
			return TRUE;
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
@brief      ����TH RW�汾

@param      (i) const char ulVersion[]        �汾��Ϣ

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGSoftVer::SetTHRWVersion(const char ulVersion[])
{
	try{
		//ULONG ulValue = ltoml(ulVersion);
		memcpy(&m_fileData[RW_HARD_POS], ulVersion, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_svSoftVersion.ulTHRWVersion, ulVersion, 4);
			//m_svSoftVersion.ulTHRWVersion = ulVersion;
			return TRUE;
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
@brief      ����IC RW�汾

@param      (i) const char ulVersion[]        �汾��Ϣ

@retval     BOOL \n
TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGSoftVer::SetICRWVersion(const char ulVersion[])
{
	try{

		// ULONG ulValue = ltoml(ulVersion);
		memcpy(&m_fileData[RW_SOFT_POS], ulVersion, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_svSoftVersion.ulICRWVersion, ulVersion, 4);
			// m_svSoftVersion.ulICRWVersion = ulVersion;
			return TRUE;
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
@brief      ����TH�汾��Ϣ

@param      (i)SC_VER_INFO thInfo   TH�汾��Ϣ

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetTHVersionInfo(SC_VER_INFO thInfo)
{
	try{
		// TH��ʽ
		memcpy(m_svSoftVersion.thInfo.type,thInfo.type,sizeof(thInfo.type));
		memcpy(&m_fileData[TH_TYPE_POS],thInfo.type,10);

		// TH�汾��
		memcpy(m_svSoftVersion.thInfo.version,thInfo.version,sizeof(thInfo.version));
		memcpy(&m_fileData[TH_VERSION_POS],thInfo.version,2);

		// TH SUMֵ
		m_svSoftVersion.thInfo.sum = thInfo.sum;
		ULONG ulValue = mltol(thInfo.sum);
		memcpy(&m_fileData[TH_SUM_POS],&ulValue,4);

		// д�ļ�
		if(CXFile::FILE_OPERATION_OK!=m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN)) {
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
@brief      ����TH Boot�汾��Ϣ

@param      (i)SC_VER_INFO thInfo   TH�汾��Ϣ

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetTHBootVersionInfo(SC_VER_INFO thInfo)
{
	try{
		// TH��ʽ
		memcpy(m_svSoftVersion.thBootInfo.type,thInfo.type,sizeof(thInfo.type));
		memcpy(&m_fileData[TH_BOOT_TYPE_POS],thInfo.type,10);

		// TH�汾��
		memcpy(m_svSoftVersion.thBootInfo.version,thInfo.version,sizeof(thInfo.version));
		memcpy(&m_fileData[TH_BOOT_VERSION_POS],thInfo.version,2);

		// TH SUMֵ
		m_svSoftVersion.thBootInfo.sum = thInfo.sum;
		ULONG ulValue = mltol(thInfo.sum);
		memcpy(&m_fileData[TH_BOOT_SUM_POS],&ulValue,4);

		// д�ļ�
		if(CXFile::FILE_OPERATION_OK!=m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN)) {
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
@brief      ����RW Boot�汾��Ϣ

@param      (i)const char[]      �汾��Ϣ

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetRWBootVer(const char version[])
{
	try{
		memcpy(&m_fileData[RW_BOOT_VERSION], version, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_svSoftVersion.rwBootVer,version,4);
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
@brief      ����THRW Boot�汾��Ϣ

@param      (i)const char[]     �汾��Ϣ

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetTHRWBootVer(const char version[])
{
	try{
		memcpy(&m_fileData[TH__RW_BOOT_VERSION], version, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			memcpy(m_svSoftVersion.thRwBootVer,version,4);
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
@brief      ����ģʽ�����汾��Ϣ

@param      (i)DWORD ver     �汾��Ϣ

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetModeHistoryVer(DWORD ver)
{
	try{
		memcpy(&m_fileData[MODE_HISTORY], &ver, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			//memcpy(&m_svSoftVersion.dwModeHistory,&ver,4);
			m_svSoftVersion.dwModeHistory = ver;
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
@brief      ����BOM����汾��Ϣ

@param      WORD wSerialNo, BYTE verType, DWORD dwVersion

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGSoftVer::SetBOMSoftInfo(WORD wSerialNo, BYTE verType, DWORD dwVersion)
{
	try{
		memcpy(&m_fileData[BOM_SOFT_INFO], &wSerialNo, 2);
		m_fileData[BOM_SOFT_INFO+2] = verType;
		memcpy(&m_fileData[BOM_SOFT_INFO+3], &dwVersion, 4);
		if(m_file->WriteBinFile(&m_fileData[0], SOFT_VER_LEN) == CXFile::FILE_OPERATION_OK) {
			//memcpy(&m_svSoftVersion.dwModeHistory,&ver,4);
			m_svSoftVersion.BomSoft_info.wSerialNo = wSerialNo;
			m_svSoftVersion.BomSoft_info.bVerType = verType;
			m_svSoftVersion.BomSoft_info.dwVersion = dwVersion;
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

CCTLOGSoftVer CCTLOGSoftVer::theInstance;
