#include "StdAfx.h"
#include "CTCardCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const CARD_COUNT_INFO_FILE_NAME		= _T("CardCountInfo.mst");          // Ʊ��������Ϣ�ļ���

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��
 
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CTCardCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, CARD_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::~CTCardCountInfo(void)
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

@retval     BOOL  TRUE: �ɹ�      FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCardCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + CARD_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + CARD_COUNT_INFO_FILE_NAME;

    try{
        m_file = new CDXFile( file, fileBack);
        // �����������ļ�
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// �ļ������쳣
        if ( CARD_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// �Ƿ����״δ����ļ�
			if(0 == binfileData.nLen){               
				// ����Ϣд���ļ�
				SetCardboxAInfo(m_CardboxAInfo);
				SetCardboxBInfo(m_CardboxBInfo);
				SetCardboxWastedInfo(m_CardboxWastedInfo);
			}
			// �ļ����ȷǷ�
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// �ļ�����
		else{
			memcpy(m_fileData, binfileData.lpData, CARD_COUNT_INFO_FILE_LEN);
			// �����ļ�
			LPBYTE point = binfileData.lpData;
			m_CardboxAInfo.Deserialize(point);
			point+=16;

			m_CardboxBInfo.Deserialize(point);
			point+=16;

			m_CardboxWastedInfo.Deserialize(point);
			point+=16;
		
			if(binfileData.lpData != NULL){
				delete binfileData.lpData;
				binfileData.lpData = NULL;
			}
		}	
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
@brief      ��ȡƱ��1�ļ�����Ϣ

@param   none

@retval     CARD_BOX_INFO& Ʊ��1�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxAInfo()
{
    return m_CardboxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��2�ļ�����Ϣ

@param   none

@retval     CARD_BOX_INFO& Ʊ��2�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxBInfo()
{
  return m_CardboxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Ʊ��ļ�����Ϣ

@param   none

@retval     CARD_BOX_INFO& ��Ʊ��ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxWastedInfo()
{
	return m_CardboxWastedInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��1�ļ�����Ϣ

@param    (i)CARD_BOX_INFO& ticketBoxInfo  Ʊ��1������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCardCountInfo::SetCardboxAInfo(CARD_BOX_INFO& ticketBoxInfo)
{
    try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		if(ticketBoxInfo.ticketBoxID.ToString() == _T("FFFFFFFF")){
			//��ǰ��ж�ز���
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// ��ǰ�ǰ�װ����
			theTVM_SETTING.SetCardBoxLastAction(1);
		}

		ticketBoxInfo.Serialize((LPBYTE)m_fileData);
        // ���²������ļ�
        UpdateMasterFile();
        m_CardboxAInfo = ticketBoxInfo;
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
@brief      ����Ʊ��2�ļ�����Ϣ

@param    (i)CARD_BOX_INFO& ticketBoxInfo   Ʊ��2������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCardCountInfo::SetCardboxBInfo(CARD_BOX_INFO& ticketBoxInfo)
{
	try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		if(ticketBoxInfo.ticketBoxID.ToString() == _T("FFFFFFFF")){
			//��ǰ��ж�ز���
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// ��ǰ�ǰ�װ����
			theTVM_SETTING.SetCardBoxLastAction(1);
		}

		ticketBoxInfo.Serialize((LPBYTE)m_fileData+16);
		// ���²������ļ�
		UpdateMasterFile();
		m_CardboxBInfo = ticketBoxInfo;
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
@brief      ����Ʊ��2�ļ�����Ϣ

@param    (i)CARD_BOX_INFO& ticketBoxInfo   Ʊ��2������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCardCountInfo::SetCardboxWastedInfo(CARD_BOX_INFO& ticketBoxInfo)
{
	try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		if(ticketBoxInfo.ticketBoxID.ToString() == _T("FFFFFFFF")){
			//��ǰ��ж�ز���
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// ��ǰ�ǰ�װ����
			theTVM_SETTING.SetCardBoxLastAction(1);
		}
		ticketBoxInfo.Serialize((LPBYTE)m_fileData+32);
		// ���²������ļ�
		UpdateMasterFile();
		m_CardboxWastedInfo = ticketBoxInfo;
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
@brief      �����ļ�������

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTCardCountInfo::UpdateMasterFile()
{
    try {
        // �����ļ���
        m_lock.Lock();
		// д�ļ�
        if(m_file->WriteBinFile(&m_fileData[0], CARD_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTCardCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// �����ļ���
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], CARD_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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

CTCardCountInfo CTCardCountInfo::theInstance;