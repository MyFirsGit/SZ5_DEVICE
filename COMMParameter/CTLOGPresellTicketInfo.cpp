#include "StdAfx.h"
#include "CTLOGPresellTicketInfo.h"


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
CCTLOGPresellTicketInfo::CCTLOGPresellTicketInfo(void)
{
	m_presellFile = NULL;
	m_preTicketNoList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGPresellTicketInfo::~CCTLOGPresellTicketInfo(void)
{
	try{
		delete m_presellFile;
		m_presellFile = NULL;
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
bool CCTLOGPresellTicketInfo::Internalize(CString dataPath, CString backupPath)
{
	// ȡ�����ػ����ļ�
	CString strFileName = dataPath + DEVICE_MASTER_DIR +PRESELL_TICKET_FILE_NAME;
	CString strBackupFilename = backupPath + DEVICE_MASTER_DIR + PRESELL_TICKET_FILE_NAME;

	try{
		if (NULL == m_presellFile) {
			m_presellFile = new CDXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;

		// ��ȡԤ��Ʊ�ļ�ʧ��
		if (m_presellFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// ����ļ�����
		if ((binfileData.nLen < MIN_FILE_LENGTH)) {
			if (0 == binfileData.nLen) {
				m_preTicketNoList.clear();
				// ����ǰ���ڴ���master
				SetFileDate();
				return true;
			}
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}

		// ����Ԥ��Ʊ��Ϣ
		LPBYTE lpData = binfileData.lpData;
		m_preTicketNoList.clear();
		//lpData += 4;
		// ����Ԥ��Ʊ�ļ��е�����
		m_curDate.wYear = BCD2int((char*)lpData,2);     // ��
		lpData += 2;
		m_curDate.biMonth = BCD2int((char*)lpData++,1); // ��
		m_curDate.biDay = BCD2int((char*)lpData++,1);   // ��

		// ȡ����ǰӪҵ��
		_DATE curDate;
		curDate = ComGetBusiDate();

		if( 0 != CmpTwoDate(curDate,m_curDate)){
			m_preTicketNoList.clear();

			// ɾ��master�ļ�����
			if(CXFile::FILE_OPERATION_OK != m_presellFile->ClearFile()){
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}

			// ����ǰ���ڴ���master
			SetFileDate();
		}

		// ��������
		else{
			PRESELL_TICKET_ID cardNo;
			for (ULONG i=0;i<(binfileData.nLen-4)/4;i++) {
				cardNo.presellId = ComMakeLong(*lpData,*(lpData+1),*(lpData+2),*(lpData+3));
				lpData += 4;
				// �����ŷ���vector��
				m_preTicketNoList.push_back(cardNo);
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
@brief      ׷���ļ�

@param      vector<PRESELL_TICKET_ID>   v_presellIdList

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGPresellTicketInfo::AddWriteFile(vector<PRESELL_TICKET_ID> v_presellIdList)
{
	try{

		// ����Ҫ����master�Ŀ��żӵ�vector��
		for(unsigned long i = 0; i < v_presellIdList.size(); i++){
			m_preTicketNoList.push_back(v_presellIdList[i]);
		}

		// ����master�ļ��ĳ���
		int len = 4 + (int)m_preTicketNoList.size()*4;
		VARIABLE_DATA vdFileData;
		vdFileData.lpData = new BYTE[len];
		vdFileData.nLen = len;
		char* fileData = (char*)vdFileData.lpData;
		memset(fileData,0,len);

		// ��ʱ��д��
		int time = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;
		int2BCD(time,&fileData[0],4);

		// ������vector����д���ļ�
		for(int k = 0; k<(int)m_preTicketNoList.size(); k++){
			//memcpy(&fileData[4+4*k],m_preTicketNoList[k].presellId,4);
			//int2BCD(m_preTicketNoList[k].presellId,&fileData[4+4*k],4);
			ComRevLong(m_preTicketNoList[k].presellId,(LPBYTE)&fileData[4+4*k]);
		}

		// д�ļ�
		if (m_presellFile->WriteBinFile(fileData,len) != CXFile::FILE_OPERATION_OK) {
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
@brief      ������־�ļ�

@param      vector<PRESELL_TICKET_ID>   ticketList

@retval     bool    \n
true:�ɹ�   false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGPresellTicketInfo::UpdatePresellTicketInfo(vector<PRESELL_TICKET_ID> ticketList)
{

	try{
		AddWriteFile(ticketList);
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
@brief      �ж��Ƿ�ΪԤ��Ʊ

@param      CardSerialNumber_t    ticketNo

@retval     bool        \n
true��  ��Ԥ��Ʊ    false�� ����Ԥ��Ʊ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGPresellTicketInfo::CheckIsPreTicket(CardSerialNumber_t ticketNo)
{
	for(unsigned long i = 0;i < m_preTicketNoList.size();i++){
		// �ȽϿ����Ƿ����
		if(ticketNo == m_preTicketNoList[i].presellId){
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ԥ��Ʊ�ļ�������

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGPresellTicketInfo::SetFileDate()
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

		if(m_presellFile->WriteBinFile(fileDate,4) != CXFile::FILE_OPERATION_OK) {
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

CCTLOGPresellTicketInfo CCTLOGPresellTicketInfo::theInstance;