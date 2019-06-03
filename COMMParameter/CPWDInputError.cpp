#include "StdAfx.h"
#include "CPwdInputError.h"
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
CPwdInputError::CPwdInputError(void)
{
	m_StuffIDFile = NULL;
	m_stuffErrList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CPwdInputError::~CPwdInputError(void)
{
	try{
		delete m_StuffIDFile;
		m_StuffIDFile = NULL;
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

@retval     boole

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CPwdInputError::ReadFile()
{
	// ȡ�����ػ����ļ�
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + PSD_ERROR_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + PSD_ERROR_INFO_FILE_NAME;
	try{
		if (NULL == m_StuffIDFile) {
			m_StuffIDFile = new CDXFile(strFileName, strBackupFilename);
		}
		VARIABLE_DATA binfileData;
		//��ȡ�ļ�
		if (m_StuffIDFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		//����ļ�����(FILE_DATE_SIZE + FILE_BASE_SIZE * N (>=0))
		if ((binfileData.nLen >FILE_DATE_SIZE) &&  (binfileData.nLen - FILE_DATE_SIZE ) %FILE_BASE_SIZE!=0) {
			throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
		}

		//�����ļ���Ϣ
		LPBYTE lpData = binfileData.lpData;

		if(binfileData.nLen >= FILE_DATE_SIZE){
			// ������������ļ��е�����
			m_curDate.wYear = BCD2int((char*)lpData,2);     // ��
			lpData += 2;
			m_curDate.biMonth = BCD2int((char*)lpData++,1); // ��
			m_curDate.biDay = BCD2int((char*)lpData++,1);   // ��

			// ȡ����ǰӪҵ��
			_DATE curDate;
			curDate = ComGetBusiDate();
			m_stuffErrList.clear();
			if( 0 != CmpTwoDate(curDate,m_curDate)){
				// ɾ��master�ļ�����
				if(CXFile::FILE_OPERATION_OK != m_StuffIDFile->ClearFile()){
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
				}
				// ����ǰ���ڴ���master
				SetFileDate();
			}
			else{
				STAFF_ERR_INFO errInfo;
				// ���ݴ洢��Ա����Ϊ�ļ����ȵ�1/5
				int stuffCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<stuffCnt;i++){
					// Ա��ID
					errInfo.stuffID = BCD2int((char*)lpData,4);
					lpData += 4;
					// �������
					errInfo.errCnt = *lpData;
					lpData++;
					m_stuffErrList.push_back(errInfo);
				}
			}
		}
		else{
			// ����ǰ���ڴ���master
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
@brief      �ж�Ա����Ϣ�Ƿ����

@param      ULONG stuffID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPwdInputError::IsStuffIDExist(ULONG stuffID)
{
	bool ret = false;
	// ��master�Ƿ��Ѿ� ����˸�Ա��
	for(int i = 0;i<(int)m_stuffErrList.size();i++){
		if(m_stuffErrList[i].stuffID==stuffID){
			ret = true;
			break;
		}
	}
	return ret;    
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ա����Ϣ

@param      ULONG stuffID 

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::AddPWDErrorStuffID(ULONG stuffID)
{
	try{
		// ֻ��Ա�������ڲ������Ϣ
		if(!IsStuffIDExist(stuffID)){

			// ��Ա����Ϣ����vector
			STAFF_ERR_INFO stuffInfo;
			stuffInfo.stuffID = stuffID;
			m_stuffErrList.push_back(stuffInfo);

			// �õ�ʵ�ʴ��ڵ�Ա����
			int len = (int)m_stuffErrList.size();

			// ��vector����Ϣ����master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData = new BYTE[FILE_DATE_SIZE + len*FILE_BASE_SIZE ];
			vdWriteData.nLen = FILE_DATE_SIZE + len*FILE_BASE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// ȡ��Ӫ������
			m_curDate = ComGetBusiDate();

			// ������תΪint��
			int dateInt;
			dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

			// תΪBCD��
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// ��Ա��ID
				int2BCD(m_stuffErrList[i].stuffID, (char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE], 4);
				// �������
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE +4], &m_stuffErrList[i].errCnt,1);     
			}

			// д��Ϣ
			if(m_StuffIDFile->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK) {
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
bool CPwdInputError::Internalize(CString dataPath, CString backupPath)
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
@brief      �������Ա����Ϣ

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::ClearStuffInfo()
{

	try {
		// �����е��ڴ������ΪĬ��ֵ
		m_stuffErrList.clear();

		// ����ļ�
		m_StuffIDFile->ClearFile();
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
@brief      ���ָ��Ա����Ϣ

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::ClearTheStaffNoInfo(ULONG stuffID)
{
	try{
		// ֻ��Ա�����ڲ�ɾ��
		if(IsStuffIDExist(stuffID)){

			vector<STAFF_ERR_INFO>::iterator verIter;

			for (verIter=m_stuffErrList.begin(); verIter!=m_stuffErrList.end(); verIter++)
			{
				if ((*verIter).stuffID == stuffID)
				{
					m_stuffErrList.erase(verIter);
					break;
				}
			}

			// �õ�ʵ�ʴ��ڵ�Ա����
			int len = (int)m_stuffErrList.size();

			// ��vector����Ϣ����master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData= new BYTE[len*FILE_BASE_SIZE + FILE_DATE_SIZE];
			vdWriteData.nLen = len*FILE_BASE_SIZE + FILE_DATE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// ȡ��Ӫ������
			m_curDate = ComGetBusiDate();

			// ������תΪint��
			int dateInt;
			dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

			// תΪBCD��
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// ��Ա��ID
				int2BCD(m_stuffErrList[i].stuffID, (char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE], 4);
				// �������
				memcpy(&writeData[i*FILE_BASE_SIZE + 4 + FILE_DATE_SIZE], &m_stuffErrList[i].errCnt,1);     
			}

			// д��Ϣ
			if(m_StuffIDFile->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK) {
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
@brief      ���ô�������

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::SetFileDate()
{
	try{
		// ȡ��Ӫ������
		m_curDate = ComGetBusiDate();

		// ������תΪint��
		int dateInt;
		dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

		// תΪBCD��
		char fileDate[4] = {0};
		int2BCD(dateInt,&fileDate[0],4);

		if(m_StuffIDFile->WriteBinFile(fileDate,4) != CXFile::FILE_OPERATION_OK) {
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

CPwdInputError CPwdInputError::theInstance;