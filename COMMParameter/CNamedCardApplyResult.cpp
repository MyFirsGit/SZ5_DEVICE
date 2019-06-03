#include "StdAfx.h"
#include "CNamedCardApplyResult.h"
#include <io.h>

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
CNamedCardApplyResult::CNamedCardApplyResult(void)
{
	m_File = NULL;
	m_ResultList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CNamedCardApplyResult::~CNamedCardApplyResult(void)
{
	try{
		delete m_File;
		m_File = NULL;
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
@brief      ��ȡ�ļ�

@param      ��

@retval     bool

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::ReadFile()
{
	// ȡ�����ػ����ļ�
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + NAMEDCARD_APPLY_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + NAMEDCARD_APPLY_INFO_FILE_NAME;
	try{
		if (NULL == m_File) {
			m_File = new CDXFile(strFileName, strBackupFilename);
		}
		VARIABLE_DATA binfileData;
		// ��ȡ�ļ�
		if (m_File->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// ����ļ�����(FILE_DATE_SIZE + FILE_BASE_SIZE * N (>=0))
		if((binfileData.nLen >FILE_DATE_SIZE) &&  (binfileData.nLen - FILE_DATE_SIZE ) % FILE_BASE_SIZE != 0) {
			throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
		}

		// �����ļ���Ϣ
		LPBYTE lpData = binfileData.lpData;
		// �ļ��д��ڼ�¼
		if(binfileData.nLen >= FILE_DATE_SIZE){
			// ����ڴ�
			m_ResultList.clear();

			// �洢����
			m_saveDate.wYear = BCD2int((char*)lpData,2);     // ��
			lpData += 2;
			m_saveDate.biMonth = BCD2int((char*)lpData++,1); // ��
			m_saveDate.biDay = BCD2int((char*)lpData++,1);   // ��

			// ȡ����ǰӪҵ��
			_DATE curDate = ComGetBusiDate();
			
			// ȡ�ô洢����+7�������
			_DATE dateAfter7days = GetDayAfter(m_saveDate,7);

			// ��ǰӪҵ�� > �洢����+7������� 
			if(1 == CmpTwoDate(curDate,dateAfter7days)){
				// ɾ��master�ļ�����
				if(CXFile::FILE_OPERATION_OK != m_File->ClearFile()){
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
				}
				// ����ǰ���ڴ���master
				SetFileDate();
			}
			else{
				NAMED_CARD_APP_RESULT recordInfo;
				// ȡ�ü����������¼��
				int recordCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<recordCnt;i++){
					// ֤����Ϣ
					memcpy(recordInfo.CertificatesNum,lpData,sizeof(recordInfo.CertificatesNum));
					lpData += 21;
					// ��˽��
					recordInfo.Status = *lpData;
					lpData++;
					m_ResultList.push_back(recordInfo);
				}
			}
		}
		// �ļ�Ϊ��
		else{
			// ����ǰ���ڴ����ļ�
			SetFileDate();
		}

		if(binfileData.lpData != NULL) {
			delete[] binfileData.lpData;
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
@brief      �жϼ����������¼�Ƿ����

@param      BYTE* CertificatesNum  �վݱ��

@retval     bool true:����   false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::IsRecordExist(BYTE* CertificatesNum)
{
	if(CertificatesNum == NULL){
		throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	bool ret = false;
	// �жϼ����������¼�Ƿ����
	for(int i = 0;i<(int)m_ResultList.size();i++){
		if(memcmp(m_ResultList[i].CertificatesNum,CertificatesNum,sizeof(m_ResultList[i].CertificatesNum)) == 0){
			ret = true;
			break;
		}
	}
	return ret;    
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ӽ����������¼

@param      BYTE* CertificatesNum   �վݱ�� 
@param      BYTE status        

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::AddNamedCardApplyRecord(BYTE* CertificatesNum,BYTE status)
{
	try{
		if(CertificatesNum == NULL){
			throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
		}
		// ֻ�м�����������Ϣ�����ڲ������Ϣ
		if(!IsRecordExist(CertificatesNum)){

			// ��������������Ϣ����vector
			NAMED_CARD_APP_RESULT recordInfo;
			memcpy(recordInfo.CertificatesNum,CertificatesNum,sizeof(recordInfo.CertificatesNum));
			recordInfo.Status = status;
			m_ResultList.push_back(recordInfo);

			// �õ�ʵ�ʴ��ڵļ�¼��
			int len = (int)m_ResultList.size();

			// ��vector����Ϣ����master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData = new BYTE[FILE_DATE_SIZE + len*FILE_BASE_SIZE ];
			vdWriteData.nLen = FILE_DATE_SIZE + len*FILE_BASE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// ȡ��Ӫ������
			//m_saveDate = ComGetBusiDate();

			// ������תΪint��
			int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

			// תΪBCD��
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// ֤����Ϣ			
				memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],m_ResultList[i].CertificatesNum,sizeof(m_ResultList[i].CertificatesNum));
				// ���״̬
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 21], &m_ResultList[i].Status,1);     
			}

			// д��Ϣ
			if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
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
@brief      ���ڴ���

@param      (i)CString dataPath         ������Ŀ¼
@param      (i)CString backupPath       ������Ŀ¼

@retval     bool \n

true: �ɹ�      false: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::Internalize(CString dataPath, CString backupPath)
{

	m_dataPath = dataPath;
	m_backupPath = backupPath;

	try {
		return ReadFile();
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
@brief      ��������������¼

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::ClearAllRecord()
{
	try {
		// �����е��ڴ������ΪĬ��ֵ
		m_ResultList.clear();

		// ����ļ�
		m_File->ClearFile();
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
@brief      ����ָ���������������˼�¼(����¼�����������)

@param      (i)BYTE* CertificatesNum   ֤������ + ֤������(1Byte + 20Byte)
@param      (i)BYTE result             ��˽��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::UpdateTheApplyResult(BYTE* CertificatesNum,BYTE result)
{
	try{		
		m_lock.Lock();
		if(CertificatesNum == NULL){
			throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
		}
		// ֻ�м����������¼���ڲŸ���
		if(IsRecordExist(CertificatesNum)){
			vector<NAMED_CARD_APP_RESULT>::iterator verIter;
			for(verIter=m_ResultList.begin(); verIter!=m_ResultList.end(); verIter++){
				if(memcmp(verIter->CertificatesNum,CertificatesNum,sizeof(verIter->CertificatesNum)) == 0){
					verIter->Status = result;
					break;
				}
			}

			// �õ������������¼��
			int len = (int)m_ResultList.size();

			// ��vector����Ϣ����master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData= new BYTE[len*FILE_BASE_SIZE + FILE_DATE_SIZE];
			vdWriteData.nLen = len*FILE_BASE_SIZE + FILE_DATE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// ȡ��Ӫ������
			//m_curDate = ComGetBusiDate();

			// ������תΪint��
			int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

			// תΪBCD��
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// ֤����Ϣ		
				memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],m_ResultList[i].CertificatesNum,sizeof(m_ResultList[i].CertificatesNum));
				// ���״̬
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 21], &m_ResultList[i].Status,1);     
			}

			// д��Ϣ
			if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// ������
		else{
			// ��Ӽ����������¼
			AddNamedCardApplyRecord(CertificatesNum,result);                               
		}
		m_lock.Unlock();
	}	
	catch(CSysException&) {
		m_lock.Unlock();
		throw;
	}
	catch (...) {
		m_lock.Unlock();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ���������������˽��

@param      (i)BYTE* CertificatesNum   ֤������ + ֤������(1Byte + 20Byte)
@param      (o)BYTE result             ��˽��

@retval     bool true:����   false:������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::GetTheApplyResult(BYTE* CertificatesNum,BYTE& result)
{
	if(CertificatesNum == NULL){
		throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	bool ret = false;
	// �жϼ����������¼�Ƿ����
	for(int i = 0;i<(int)m_ResultList.size();i++){
		if(memcmp(m_ResultList[i].CertificatesNum,CertificatesNum,sizeof(m_ResultList[i].CertificatesNum)) == 0){
			result = m_ResultList[i].Status;
			ret = true;
			break;
		}
	}
	return ret;   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д���ļ��洢����

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::SetFileDate()
{
	try{
		// ȡ��Ӫ������
		m_saveDate = ComGetBusiDate();

		// ������תΪint��
		int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

		// תΪBCD��
		char fileDate[4] = {0};
		int2BCD(dateInt,&fileDate[0],4);

		if(m_File->WriteBinFile(fileDate,4) != CXFile::FILE_OPERATION_OK,false) {
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

CNamedCardApplyResult CNamedCardApplyResult::theInstance;