#include "StdAfx.h"
#include "CTLOGRegister.h"


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
CCTLOGRegister::CCTLOGRegister(void)
{
	m_file = NULL;
	m_errorKindCnt = 0;
	memset(m_maintCntTotal, 0x00000000, 256);
	memset(m_fileData, 0, REGISTER_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGRegister::~CCTLOGRegister(void)
{
	try{
		delete m_file;
		m_file = NULL;
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

@retval     bool        \n
true:���ļ��ɹ������ļ�����     false:��ȡ�ļ��ɹ����ļ�Ϊ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ReadFile()
{
	bool ret = false;

	try {
		// ȡ�����ػ����ļ�
		CString strFileName = m_dataPath + DEVICE_MASTER_DIR + REGISTER_INFO_FILE_NAME;
		CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR +  REGISTER_INFO_FILE_NAME;

		if (NULL == m_file) {
			m_file = new CDXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;
		do {
			// ��ȡ�Ĵ����ļ�ʧ��
			if (m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}

			// �ļ����ȼ��
			if ((binfileData.nLen < REGISTER_LEN)) {
				if (0 == binfileData.nLen) {
					memset(&m_maintCntTotal,0,sizeof(m_maintCntTotal));
					memset(&m_errorInfo,0,sizeof(m_errorInfo));
					ret = false;
					break;
				}
				else{
					throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
				}
			}

			memcpy(m_fileData, binfileData.lpData, REGISTER_LEN);

			// ���������ļ�
			ULONG size = 0;
			LPBYTE lpData = binfileData.lpData;

			// record start date and time
			m_registerTime.wYear = BCD2int((char*)lpData,2);
			lpData +=2;
			m_registerTime.biMonth  = BCD2int((char*)lpData++,1);
			m_registerTime.biDay    = BCD2int((char*)lpData++,1);
			m_registerTime.biHour   = BCD2int((char*)lpData++,1);
			m_registerTime.biMinute = BCD2int((char*)lpData++,1);
			m_registerTime.biSecond = BCD2int((char*)lpData++,1);           

			// ά������

			// 0x00-0x1DԤ��
			lpData += REGISTER_BLOCK_SIZE * MAGAZINE_ONE_CNT;
			// Ʊ��1����Ʊ����
			m_maintCntTotal[MAGAZINE_ONE_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// Ʊ��2����Ʊ����
			m_maintCntTotal[MAGAZINE_TWO_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// ��Ʊ������
			m_maintCntTotal[INVALIDATED_TICKET_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// Ԥ��
			lpData = lpData + (255 - INVALIDATED_TICKET_CNT) * REGISTER_BLOCK_SIZE;

			// �쳣����
			m_errorKindCnt = *lpData;
			lpData++;

			// �쳣
			for(int i=0;i<m_errorKindCnt;i++){
				m_errorInfo[i].EC_code = mstos(*(WORD*)lpData);
				lpData +=2;
				m_errorInfo[i].count = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
				lpData +=4;
			}
			// �ж������Ƿ�Ϸ�
			if(0==m_registerTime.wYear){
				ret = false;
			}
			else{
				ret = true;
			}            
		} while(0);

		if(binfileData.lpData != NULL)
		{
			delete[] binfileData.lpData;
			binfileData.lpData = NULL;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
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
bool CCTLOGRegister::Internalize(CString dataPath, CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	bool iret;

	try {
		iret =  ReadFile();
		if(!iret){
			// ȡ�õ�ǰʱ��
			SYSTEMTIME dtOperationTime;
			::GetLocalTime(&dtOperationTime);

			// ����ǰʱ��ת��ΪBCD
			int time;
			time = dtOperationTime.wYear*10000 + dtOperationTime.wMonth*100 + dtOperationTime.wDay;
			int2BCD(time,&m_fileData[0],4);
			time = dtOperationTime.wHour*10000 + dtOperationTime.wMinute*100 + dtOperationTime.wSecond;
			int2BCD(time,&m_fileData[4],3);

			// ���õ�ǰʱ��
			if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {

				// �޸��ڴ�Ϊ��ǰʱ��
				memcpy(&m_registerTime.wYear,&dtOperationTime.wYear,sizeof(m_registerTime.wYear));
				memcpy(&m_registerTime.biMonth,&dtOperationTime.wMonth,sizeof(m_registerTime.biMonth));
				memcpy(&m_registerTime.biDay,&dtOperationTime.wDay,sizeof(m_registerTime.biDay));
				memcpy(&m_registerTime.biHour,&dtOperationTime.wHour,sizeof(m_registerTime.biHour));
				memcpy(&m_registerTime.biMinute,&dtOperationTime.wMinute,sizeof(m_registerTime.biMinute));
				memcpy(&m_registerTime.biSecond,&dtOperationTime.wSecond,sizeof(m_registerTime.biSecond));
			}
			else {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ü�¼ʱ��

@param      (o) _DATE_TIME&  timeData    ��¼ʱ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::GetRecordTime(_DATE_TIME& timeData)
{
	timeData = m_registerTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ü�¼ʱ��

@param      (i) _DATE_TIME   timeData    ��¼ʱ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::SetRecordTime(_DATE_TIME timeData)
{
	try{
		int time;
		time = timeData.wYear*10000 + timeData.biMonth*100 + timeData.biDay;
		int2BCD(time,&m_fileData[0],4);
		time = timeData.biHour*10000 + timeData.biMinute*100 + timeData.biSecond;
		int2BCD(time,&m_fileData[4],3);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {
			m_registerTime = timeData;
			return true;
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
@brief      �õ�ά�����ݵļ���

@param      (i) MAINTAIN_CONDITION   conditionId   ���ݴ洢��λ��
@param      (o) MAINTAIN_MODULE& module            ά������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGRegister::GetMaintDataCount(MAINTAIN_CONDITION conditionId,MAINTAIN_MODULE& module)
{
	switch(conditionId){
		case MAGAZINE_ONE_CNT:
		case MAGAZINE_TWO_CNT:
		case INVALIDATED_TICKET_CNT:
			module = MAINTAIN_TH;
			break;
		default:
			module = MAINTAIN_UNUSED;
			break;
	}
	return m_maintCntTotal[conditionId];

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ά�����ݵļ���

@param      (i) MAINTAIN_CONDITION   conditionId   Ҫ���µ�ά�����ݵ�λ��
@param      (i) ULONG Totalcount  countTotal    ���ݼ���

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::UpdateMaintDataCount(MAINTAIN_CONDITION conditionId, ULONG countTotal)
{
	try{
		// ���ø���ά������
		m_maintCntTotal[conditionId] += countTotal;
		countTotal = ltoml(m_maintCntTotal[conditionId]);
		// ����m_fileData[]
		memcpy(&m_fileData[MAINTAIN_POS + conditionId*REGISTER_BLOCK_SIZE], &countTotal, 4);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) != CXFile::FILE_OPERATION_OK) {

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
@brief      �õ��쳣��Ϣ(��λר��)

@param      (o)LPBYTE lpError

@retval     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::GetErrDataCount(LPBYTE lpError)
{
	memcpy(lpError,&m_fileData[ERROR_INFO_POS],ERROR_INFO_SIZE);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �����쳣����

@param      (i) WORD char    ECcode     �������

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::UpdateErrDataCount(WORD ECcode)
{
	try{
		// ����EC code����Ӧ��������
		int pos = 0;
		for(pos = 0; pos< m_errorKindCnt; pos++){
			// ����ҵ�ƥ��Ĵ������
			if(m_errorInfo[pos].EC_code==ECcode){
				break;
			}
		}

		// ���pos == m_errorKindCnt ����ִ���δ��master�м�¼
		if(pos == m_errorKindCnt){
			// �������࣫1
			m_errorKindCnt++;
			m_fileData[ERROR_KINDS_POS] = m_errorKindCnt;

			// �õ�EC code
			m_errorInfo[pos].EC_code = ECcode;
			ECcode = mstos(ECcode);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE],&ECcode,2);

			// �¼�һ������ ����count = 1            
			m_errorInfo[pos].count = 1;     
			ULONG errorCnt = ltoml(m_errorInfo[pos].count);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE+2],&errorCnt,REGISTER_BLOCK_SIZE);
		}
		// ���pos < m_errorKindCnt������ִ���master�Ѿ�����
		else{
			// ���ִ�����++
			m_errorInfo[pos].count ++;
			ULONG errorCnt = ltoml(m_errorInfo[pos].count);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE+2],&errorCnt,REGISTER_BLOCK_SIZE);    
		}

		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) != CXFile::FILE_OPERATION_OK) {
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
@brief      �����ļ�

@param      (i)const char*  lpData      Ҫ�������ļ�������
@param      (i)ULONG        nLen        Ҫ�������ļ������ݳ���

@retval     bool        \n
true:�����ļ��ɹ�         false:�����ļ�ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::OverWriteFile(const char* lpData, ULONG nLen)
{
	try{
		bool ret = false;

		if (m_file->WriteBinFile(lpData,nLen) == CXFile::FILE_OPERATION_OK) {
			ret = true;
		}
		else{
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
		return ret;
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
@brief      ���ָ����ά������

@param      (i)MAINTAIN_CONDITION condition    λ��

@retval     bool        \n
true:����ɹ�         false:���ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ClearMaintData(MAINTAIN_CONDITION condition)
{
	try{
		memset(&m_fileData[7+condition*REGISTER_BLOCK_SIZE],0x00,4);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {
			m_maintCntTotal[condition] = 0;
			return true;
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
@brief      ������е�ά������

@param      void

@retval     bool        \n
true:����ɹ�         false:���ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ClearAllMaintData()
{
	try{
		// ÿһ��ά��������0
		memset(&m_fileData[7],0x00,1024);

		// ȡ�õ�ǰʱ��
		SYSTEMTIME dtOperationTime;
		::GetLocalTime(&dtOperationTime);

		// ����ǰʱ��ת��ΪBCD
		int time;
		time = dtOperationTime.wYear*10000 + dtOperationTime.wMonth*100 + dtOperationTime.wDay;
		int2BCD(time,&m_fileData[0],4);
		time = dtOperationTime.wHour*10000 + dtOperationTime.wMinute*100 + dtOperationTime.wSecond;
		int2BCD(time,&m_fileData[4],3);

		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {

			// ��m_maintCntTotal��Ϊ0
			for(int i = 0;i<256;i++){
				m_maintCntTotal[i] = 0;
			}

			// �޸��ڴ�Ϊ��ǰʱ��
			memcpy(&m_registerTime.wYear,&dtOperationTime.wYear,sizeof(m_registerTime.wYear));
			memcpy(&m_registerTime.biMonth,&dtOperationTime.wMonth,sizeof(m_registerTime.biMonth));
			memcpy(&m_registerTime.biDay,&dtOperationTime.wDay,sizeof(m_registerTime.biDay));
			memcpy(&m_registerTime.biHour,&dtOperationTime.wHour,sizeof(m_registerTime.biHour));
			memcpy(&m_registerTime.biMinute,&dtOperationTime.wMinute,sizeof(m_registerTime.biMinute));
			memcpy(&m_registerTime.biSecond,&dtOperationTime.wSecond,sizeof(m_registerTime.biSecond));
			return true;
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
CCTLOGRegister CCTLOGRegister::theInstance;