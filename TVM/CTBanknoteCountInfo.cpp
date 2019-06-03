#include "StdAfx.h"
#include "CTBanknoteCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const BANKNOTE_COUNT_INFO_FILE_NAME		= _T("BanknoteCountInfo.mst");          // ֽ���������Ϣ�ļ���
//static const TCHAR* const BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME = _T("BanknoteChangeCountInfo.mst");		// ֽ�������������Ϣ�ļ���

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��
 
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::CTBanknoteCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, BANKNOTE_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::~CTBanknoteCountInfo(void)
{
	try{
        delete m_file;
        m_file = NULL;

		//delete m_changefile;
		//m_changefile = NULL;
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

@retval     BOOL  TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + BANKNOTE_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + BANKNOTE_COUNT_INFO_FILE_NAME;

// 	CString changeFile = dataPath + MASTER_DIR + BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME;
// 	CString ChangeFileBack = backupPath + MASTER_DIR + BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME;
    try{
        m_file = new CDXFile( file, fileBack);

        // �����������ļ�
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// �ļ������쳣
        if ( BANKNOTE_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// �Ƿ����״δ����ļ�
			if(0 == binfileData.nLen){               
				// ����Ϣд���ļ�
				SetBanknoteboxInfo(m_BanknoteBoxInfo);

				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				// ѭ����A
				int2BCD(bLineCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bLineCode, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bLineCode));
				m_RecyclerBoxAInfo.banknoteBoxID.bType	   = 0x06;
				int2BCD(bStationCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bNumInQu, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bNumInQu));
				int2BCD(bMachineCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bNum, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bNum));
				m_RecyclerBoxAInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxAAmount()*100;
				m_RecyclerBoxAInfo.ulCurCount = 0;
				SetBanknoteRecyclerBoxAInfo(m_RecyclerBoxAInfo);
				// ѭ����B
				m_RecyclerBoxBInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxBInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxBAmount()*100;
				m_RecyclerBoxBInfo.ulCurCount = 0;
				SetBanknoteRecyclerBoxBInfo(m_RecyclerBoxBInfo);
				// ѭ����C
				m_RecyclerBoxCInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxCInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxCAmount()*100;
				m_RecyclerBoxCInfo.ulCurCount = 0;

				SetBanknoteRecyclerBoxCInfo(m_RecyclerBoxCInfo);
				// ѭ����D
				m_RecyclerBoxDInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxDInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxDAmount()*100;
				m_RecyclerBoxDInfo.ulCurCount = 0;

				SetBanknoteRecyclerBoxDInfo(m_RecyclerBoxDInfo);
				m_ChangeBoxInfo.ulCurValue = theTVM_SETTING.GetBNRChangeBoxAmount()*100;
				m_ChangeBoxInfo.ulCurCount = 0;
				SetBanknoteChangeBoxInfo(m_ChangeBoxInfo);
			}
			// �ļ����ȷǷ�
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// �ļ�����
		else{
			memcpy(m_fileData, binfileData.lpData, BANKNOTE_COUNT_INFO_FILE_LEN);
			// �����ļ�
			LPBYTE point = binfileData.lpData;
			m_BanknoteBoxInfo.Deserialize(point);
			point+=32+11;
			m_RecyclerBoxAInfo.Deserialize(point);
			point += 16+11;
			m_RecyclerBoxBInfo.Deserialize(point);
			point += 16+11;
			m_RecyclerBoxCInfo.Deserialize(point);
			point += 16+11;
			m_RecyclerBoxDInfo.Deserialize(point);
			point += 16+11;
			m_ChangeBoxInfo.Deserialize(point);
			point += 16+11;

			if(binfileData.lpData != NULL){
				delete binfileData.lpData;
				binfileData.lpData = NULL;
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

	// ֽ������mst�ļ�
	/*try{
		m_changefile = new CDXFile(changeFile,ChangeFileBack);

		// ��ȡ�������ļ�
		VARIABLE_DATA changeBinFileData;
		if(m_changefile->ReadBinFile(changeBinFileData) != CXFile::FILE_OPERATION_OK){
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		// �����쳣
		if(BANKNOTE_CHANGE_INFO_LEN != changeBinFileData.nLen){
			if(0 == changeBinFileData.nLen){
				SetBanknoteChangeBoxAInfo(m_BanknoteChangeBoxAInfo);
				SetBanknoteChangeBoxBInfo(m_BanknoteChangeBoxBInfo);
				SetBanknoteInvalidBoxInfo(m_BanknoteChangeWasteInfo);
			}
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{
			memcpy(m_changefileData,changeBinFileData.lpData,BANKNOTE_CHANGE_INFO_LEN);

			LPBYTE ptobin = changeBinFileData.lpData;
			// ����ֽ����A������
			m_BanknoteChangeBoxAInfo.Deserialize(ptobin);
			ptobin += 16;
			// ����ֽ����B������
			m_BanknoteChangeBoxBInfo.Deserialize(ptobin);
			ptobin += 16;

			// ����ֽ�ҷϱ��������
			m_BanknoteChangeWasteInfo.Deserialize(ptobin);
			ptobin += 16;

			if(NULL != changeBinFileData.lpData){
				delete changeBinFileData.lpData;
				changeBinFileData.lpData = NULL;
			}

		}
		return TRUE;
	}
	catch(CSysException&){
		throw;
	}
	catch(...){
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡֽ����ļ�����Ϣ

@param   none

@retval     BANKNOTE_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_BOX_INFO & CTBanknoteCountInfo::GetBanknoteboxInfo()
{
    return m_BanknoteBoxInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ֽ����ļ�����Ϣ

@param    (i)BANKNOTE_BOX_INFO& boxInfo  ֽ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteboxInfo(BANKNOTE_BOX_INFO& boxInfo)
{
    try{
		//boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData);
        // ���²������ļ�
        UpdateMasterFile();
        m_BanknoteBoxInfo = boxInfo;
        return TRUE;
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
@brief     ����ֽ��ѭ����A�ļ�����Ϣ

@param    ��i��BANKNOTE_RECYCLER_BOX_INFO& boxInfo  ֽ�������������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteRecyclerBoxAInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo)
{
	try{
		if(boxInfo.ulCurCount < 0){
			boxInfo.ulCurCount = 0;
		}
		boxInfo.Serialize((LPBYTE)m_fileData + 43);
		// ���²������ļ�
		UpdateMasterFile();
		m_RecyclerBoxAInfo = boxInfo;
		return TRUE;
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
@brief      ����ֽ��ѭ����B�ļ�����Ϣ

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  ֽ�������������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteRecyclerBoxBInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo)
{
	try{
		if(boxInfo.ulCurCount < 0){
			boxInfo.ulCurCount = 0;
		}
		boxInfo.Serialize((LPBYTE)m_fileData + 70);
		// ���²������ļ�
		UpdateMasterFile();
		m_RecyclerBoxBInfo = boxInfo;
		return TRUE;
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
@brief     ����ֽ��ѭ����C�ļ�����Ϣ

@param    ��i��BANKNOTE_RECYCLER_BOX_INFO& boxInfo  ֽ�������������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteRecyclerBoxCInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo)
{
	try{
		if(boxInfo.ulCurCount < 0){
			boxInfo.ulCurCount = 0;
		}
		boxInfo.Serialize((LPBYTE)m_fileData + 97);
		// ���²������ļ�
		UpdateMasterFile();
		m_RecyclerBoxCInfo = boxInfo;
		return TRUE;
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
@brief      ����ֽ��ѭ����D�ļ�����Ϣ

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  ֽ�������������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteRecyclerBoxDInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo)
{
	try{
		if(boxInfo.ulCurCount < 0){
			boxInfo.ulCurCount = 0;
		}
		boxInfo.Serialize((LPBYTE)m_fileData + 124);
		// ���²������ļ�
		UpdateMasterFile();
		m_RecyclerBoxDInfo = boxInfo;
		return TRUE;
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
@brief      ����ֽ��������ļ�����Ϣ

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  ֽ�������������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteChangeBoxInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo)
{
	try{
		if(boxInfo.ulCurCount < 0){
			boxInfo.ulCurCount = 0;
		}
		boxInfo.Serialize((LPBYTE)m_fileData + 151);
		// ���²������ļ�
		UpdateMasterFile();
		m_ChangeBoxInfo = boxInfo;
		return TRUE;
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
@brief      ��ȡֽ��ѭ����A�ļ�����Ϣ

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxAInfo()
{
	return m_RecyclerBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡֽ��ѭ����B�ļ�����Ϣ

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxBInfo()
{
	return m_RecyclerBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡֽ��ѭ����C�ļ�����Ϣ

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxCInfo()
{
	return m_RecyclerBoxCInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡֽ��ѭ����D�ļ�����Ϣ

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxDInfo()
{
	return m_RecyclerBoxDInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡֽ��������ļ�����Ϣ

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& ֽ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteChangeBoxInfo()
{
	return m_ChangeBoxInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ļ�������

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTBanknoteCountInfo::UpdateMasterFile()
{
    try {
        // �����ļ���
        m_lock.Lock();
		
		// д�ļ�
		if(m_file->WriteBinFile(&m_fileData[0], BANKNOTE_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
		
        // �ر��ļ���
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
@brief      ���²������ļ�(�������ļ�)

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTBanknoteCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// �����ļ���
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], BANKNOTE_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
		// �ر��ļ���
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

CTBanknoteCountInfo CTBanknoteCountInfo::theInstance;