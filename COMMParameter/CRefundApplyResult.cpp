#include "StdAfx.h"
#include "CRefundApplyResult.h"
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
CRefundApplyResult::CRefundApplyResult(void)
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
CRefundApplyResult::~CRefundApplyResult(void)
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
bool CRefundApplyResult::ReadFile()
{
	// ȡ�����ػ����ļ�
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + REFUND_APPLY_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + REFUND_APPLY_INFO_FILE_NAME;
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
				REFUND_APP_RESULT recordInfo;
				// ȡ���˿������¼��
				int recordCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<recordCnt;i++){
					//֤������
					memcpy(&recordInfo.certificateType,lpData,sizeof(recordInfo.certificateType));
					lpData+=sizeof(recordInfo.certificateType);
					//֤�����
					memcpy(recordInfo.certificateID,lpData,sizeof(recordInfo.certificateID));
					lpData+=sizeof(recordInfo.certificateID);
					//Ʊ��Ӧ�ú�
					memcpy(recordInfo.ticketApplicationSerialNo,lpData,sizeof(recordInfo.ticketApplicationSerialNo));
					lpData+=sizeof(recordInfo.ticketApplicationSerialNo);
					// �վݱ��
					memcpy(recordInfo.ReceiptNum,lpData,sizeof(recordInfo.ReceiptNum));
					lpData += 10;
					// ��Ѻ����
					recordInfo.Deposit = MAKELONG(MAKEWORD(*lpData, *(lpData + 1)),MAKEWORD(*(lpData + 2),*(lpData+3)));
					lpData += 4;
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

void CRefundApplyResult::SaveToFile()
{
	try
	{
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
			//֤������
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],&m_ResultList[i].certificateType,sizeof(m_ResultList[i].certificateType));
			//֤�����
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+4],m_ResultList[i].certificateID,sizeof(m_ResultList[i].certificateID));
			//Ʊ��Ӧ�ú�
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+24],m_ResultList[i].ticketApplicationSerialNo,sizeof(m_ResultList[i].ticketApplicationSerialNo));
			// �վݱ��				
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+32],m_ResultList[i].ReceiptNum,sizeof(m_ResultList[i].ReceiptNum));
			// �������
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 42],&m_ResultList[i].Balance,sizeof(m_ResultList[i].Balance));
			// ��Ѻ����
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 46],&m_ResultList[i].Deposit,sizeof(m_ResultList[i].Deposit));
			// ���״̬
			memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 50], &m_ResultList[i].Status,1);     
		}

		// д��Ϣ
		if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
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
@brief      ���ڴ���

@param      (i)CString dataPath         ������Ŀ¼
@param      (i)CString backupPath       ������Ŀ¼

@retval     bool \n

true: �ɹ�      false: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundApplyResult::Internalize(CString dataPath, CString backupPath)
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
@brief      ����˿������¼

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::ClearAllRecord()
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
@brief      ����ָ���˿��������˼�¼(����¼�����������)

@param      (i)BYTE* receiptNum �վݱ��
@param      (i)long balance �������
@param      (i)long deposit ��Ѻ����
@param      (i)BYTE result      ��˽��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::UpdateTheApplyResult(REFUND_APP_RESULT& record)
{
	try{
		m_lock.Lock();
		// ֻ���˿������¼���ڲŸ���
		int position = -1;
		for(int i=0;i<m_ResultList.size();i++)
		{
			REFUND_APP_RESULT& iterResult = m_ResultList[i];
			//Ʊ����һ��
			if(memcmp(iterResult.ticketApplicationSerialNo,record.ticketApplicationSerialNo,sizeof(record.ticketApplicationSerialNo)) == 0)
			{
				position = i;
				break;
			}
			//֤����һ��
			if(iterResult.certificateType == record.certificateType && memcmp(iterResult.certificateID,record.certificateID,sizeof(record.certificateID)) == 0)
			{
				position = i;
				break;
			}
		}

		if(position>=0)
		{
			m_ResultList[position] = record;
		}
		else
		{
			m_ResultList.push_back(record);
		}
		SaveToFile();
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
@brief      ��ȡָ���˿��������˽��

@param      (i)BYTE* receiptNum   �վݱ��
@param      (o)BYTE result        ��˽��

@retval     bool true:����   false:������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundApplyResult::GetTheApplyResult(REFUND_APP_RESULT& refund_app_result)
{
	int position = -1;
	for(int i=0;i<m_ResultList.size();i++)
	{
		REFUND_APP_RESULT& iterResult = m_ResultList[i];
		//Ʊ����һ��
		if(memcmp(iterResult.ticketApplicationSerialNo,refund_app_result.ticketApplicationSerialNo,sizeof(refund_app_result.ticketApplicationSerialNo)) == 0)
		{
			position = i;
			break;
		}
		//֤����һ��
		if(iterResult.certificateType == refund_app_result.certificateType && memcmp(iterResult.certificateID,refund_app_result.certificateID,sizeof(refund_app_result.certificateID)) == 0)
		{
			position = i;
			break;
		}
	}

	if(position>=0)
	{
		refund_app_result = m_ResultList[position];
	}
	return position>=0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      д���ļ��洢����

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::SetFileDate()
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

CRefundApplyResult CRefundApplyResult::theInstance;