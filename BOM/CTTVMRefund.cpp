#include "StdAfx.h"
#include "CTTVMRefund.h"
#include "LogException.h"


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
CCTVMRefund::CCTVMRefund(void)
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
CCTVMRefund::~CCTVMRefund(void)
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
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CCTVMRefund::Internalize(CString dataPath, CString backupPath)
{
	// ȡ�����ػ����ļ�
	CString strFileName = dataPath + TVM_REFUND_FILE_NAME;
	CString strBackupFilename = backupPath + TVM_REFUND_FILE_NAME;

	try{
		if (NULL == m_presellFile) {
			m_presellFile = new CXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;

		// ��ȡԤ��Ʊ�ļ�ʧ��
		if (m_presellFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CLogException(CLogException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
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
				throw CLogException(CLogException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
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
			if(CXFile::FILE_OPERATION_OK != m_presellFile->ClearBinFile()){
				throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}

			// ����ǰ���ڴ���master
			SetFileDate();
		}

		// ��������
		else{
			TVM_ERROR_TICKET_ID cardNo;
			for (ULONG i=0;i<(binfileData.nLen-4)/16;i++) {
				char temp[18];
				memset(temp, 0, 18);
				memcpy(temp, lpData, 16);
				//cardNo.csTVMErrorSierialNo.SetString(temp);
				USES_CONVERSION;
				cardNo.csTVMErrorSierialNo= A2T(temp);
				lpData += 16;
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
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CCTVMRefund::AddWriteFile(vector<TVM_ERROR_TICKET_ID> v_presellIdList)
{
	try{

		// ����Ҫ����master�Ŀ��żӵ�vector��
		for(unsigned long i = 0; i < v_presellIdList.size(); i++){
			m_preTicketNoList.push_back(v_presellIdList[i]);
		}

		// ����master�ļ��ĳ���
		int len = 4 + (int)m_preTicketNoList.size()*16;
		char* fileData = new char[len];
		memset(fileData,0,len);

		// ��ʱ��д��
		int time = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;
		int2BCD(time,&fileData[0],4);

		// ������vector����д���ļ�
		for(int k = 0; k<(int)m_preTicketNoList.size(); k++){
			//memcpy(&fileData[4+4*k],m_preTicketNoList[k].presellId,4);
			//int2BCD(m_preTicketNoList[k].presellId,&fileData[4+4*k],4);
			char temp[18];
			memset(temp, 0, 36);
			char* lptemp = temp;
		//	lptemp = m_preTicketNoList[k].csTVMErrorSierialNo.GetBuffer();
			USES_CONVERSION;
            lptemp = T2A(m_preTicketNoList[k].csTVMErrorSierialNo);
			memcpy(&fileData[4+16*k], lptemp, 16);
		}

		// д�ļ�
		if (m_presellFile->WriteFileWithoutSUM(fileData,len) != CXFile::FILE_OPERATION_OK) {
			delete []fileData;
			fileData = NULL;
			throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}

		delete []fileData;
		fileData = NULL;
	}

	// �쳣����
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CCTVMRefund::UpdateTVMTicketInfo(vector<TVM_ERROR_TICKET_ID> ticketList)
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
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ΪTVMƱ

@param      CardSerialNumber_t    ticketNo

@retval     bool        \n
true��  ��TVMƱ    false�� ����TVMƱ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CCTVMRefund::CheckIsTVMTicket(CString csticketNo)
{
	for(unsigned long i = 0;i < m_preTicketNoList.size();i++){
		// �ȽϿ����Ƿ����
		if(csticketNo == m_preTicketNoList[i].csTVMErrorSierialNo){
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
void CCTVMRefund::SetFileDate()
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

		if(m_presellFile->WriteFileWithoutSUM(fileDate,4) != CXFile::FILE_OPERATION_OK) {
			throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
	}

	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

CCTVMRefund CCTVMRefund::theInstance;