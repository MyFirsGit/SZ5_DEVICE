#include "StdAfx.h"
#include "CTTokenCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TOKEN_COUNT_INFO_FILE_NAME		= _T("TokenCountInfo.mst");          // Ʊ��������Ϣ�ļ���

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��
 
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTTokenCountInfo::CTTokenCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, TOKEN_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTTokenCountInfo::~CTTokenCountInfo(void)
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
BOOL CTTokenCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + TOKEN_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + TOKEN_COUNT_INFO_FILE_NAME;

    try{
        m_file = new CDXFile( file, fileBack);
        // �����������ļ�
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// �ļ������쳣
        if ( TOKEN_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// �Ƿ����״δ����ļ�
			if(0 == binfileData.nLen){               
				// ����Ϣд���ļ�
				SetTokenboxAInfo(m_TokenboxAInfo);
				SetTokenboxBInfo(m_TokenboxBInfo);
				SetTokenboxWastedInfo(m_TokenboxWastedInfo);
				SetTokenboxCollectedInfo(m_TokenboxCollectedInfo);

				BYTE bHiByte;
				BYTE bLwByte;
				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				int2BCD(bLineCode, (char*)&m_HopperAInfo.tokenBoxID.bLineCode, sizeof(m_HopperAInfo.tokenBoxID.bLineCode));
				m_HopperAInfo.tokenBoxID.bType = 0x00;	
				int2BCD(bStationCode, (char *)&bHiByte, sizeof(bHiByte));
				bMachineCode = ((WORD)theMAINTENANCE_INFO.GetMachineCode())*2-1;
				int2BCD(bMachineCode, (char *)&bLwByte, sizeof(bLwByte));
				m_HopperAInfo.tokenBoxID.wNum = (bHiByte << 8) | (bLwByte & 0xff);
				SetHopperAInfo(m_HopperAInfo);

				bMachineCode = ((WORD)theMAINTENANCE_INFO.GetMachineCode())*2;
				int2BCD(bMachineCode, (char *)&bLwByte, sizeof(bLwByte));
				m_HopperBInfo = m_HopperAInfo;
				m_HopperBInfo.tokenBoxID.wNum = (bHiByte << 8) | (bLwByte & 0xff);
				SetHopperBInfo(m_HopperBInfo);

				//m_TokenboxWastedInfo.tokenBoxID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
				//m_TokenboxWastedInfo.tokenBoxID.bType = 0x95;
				//wMachineCode = ((int)theMAINTENANCE_INFO.GetMachineCode())*2-1;
				////radixchange.Format(_T("%d"),Num_radix10);
				//m_TokenboxWastedInfo.tokenBoxID.wNum = (wStationCode << 8) | (wMachineCode & 0xff);//_tcstoul(radixchange,NULL,16);
				//SetTokenboxWastedInfo(m_TokenboxWastedInfo);
			}
			// �ļ����ȷǷ�
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// �ļ�����
		else{
			memcpy(m_fileData, binfileData.lpData, TOKEN_COUNT_INFO_FILE_LEN);
			// �����ļ�
			LPBYTE point = binfileData.lpData;
			m_TokenboxAInfo.Deserialize(point);
			point+=16+11;

			m_TokenboxBInfo.Deserialize(point);
			point+=16+11;

			m_HopperAInfo.Deserialize(point);
			point+=16+11;

			m_HopperBInfo.Deserialize(point);
			point+=16+11;

			m_TokenboxWastedInfo.Deserialize(point);
			point+=16+11;

			m_TokenboxCollectedInfo.Deserialize(point);
			point+=16+11;

			//UpdateHopperId();

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
@brief      ��ȡToken��1�ļ�����Ϣ

@param   none

@retval     TOKEN_BOX_INFO& Token��1�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTTokenCountInfo::TOKEN_BOX_INFO & CTTokenCountInfo::GetTokenboxAInfo()
{
    return m_TokenboxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡToken��2�ļ�����Ϣ

@param   none

@retval     TOKEN_BOX_INFO& Token��2�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTTokenCountInfo::TOKEN_BOX_INFO & CTTokenCountInfo::GetTokenboxBInfo()
{
  return m_TokenboxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Token��ļ�����Ϣ

@param   none

@retval     TOKEN_BOX_INFO& ��Token��ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTTokenCountInfo::TOKEN_BOX_INFO & CTTokenCountInfo::GetTokenboxWastedInfo()
{
	return m_TokenboxWastedInfo;
}

CTTokenCountInfo::TOKEN_BOX_INFO & CTTokenCountInfo::GetTokenboxCollectedInfo()
{
	return m_TokenboxCollectedInfo;
}

CTTokenCountInfo::TOKEN_BOX_INFO& CTTokenCountInfo::GetHopperAInfo()
{
	return m_HopperAInfo;
}

CTTokenCountInfo::TOKEN_BOX_INFO& CTTokenCountInfo::GetHopperBInfo()
{
	return m_HopperBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token��1�ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& tokenBoxInfo  Token��1������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetTokenboxAInfo(TOKEN_BOX_INFO& tokenBoxInfo)
{
    try{
		if(tokenBoxInfo.ulCurCount < 0){
			tokenBoxInfo.ulCurCount = 0;
		}
		tokenBoxInfo.Serialize((LPBYTE)m_fileData);
        // ���²������ļ�
        UpdateMasterFile();
        m_TokenboxAInfo = tokenBoxInfo;
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
@brief      ����Token��2�ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& tokenBoxInfo   Token��2������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetTokenboxBInfo(TOKEN_BOX_INFO& tokenBoxInfo)
{
	try{
		if(tokenBoxInfo.ulCurCount < 0){
			tokenBoxInfo.ulCurCount = 0;
		}
		tokenBoxInfo.Serialize((LPBYTE)m_fileData+27);
		// ���²������ļ�
		UpdateMasterFile();
		m_TokenboxBInfo = tokenBoxInfo;
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
@brief      ����Hopper��A�ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& tokenBoxInfo   Token��2������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetHopperAInfo(TOKEN_BOX_INFO& hopperInfo)
{
	try{
		if(hopperInfo.ulCurCount < 0){
			hopperInfo.ulCurCount = 0;
		}
		hopperInfo.Serialize((LPBYTE)m_fileData+54);
		// ���²������ļ�
		UpdateMasterFile();
		m_HopperAInfo = hopperInfo;
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
@brief      ����hopper��B�ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& hopperInfo   hopper��B������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetHopperBInfo(TOKEN_BOX_INFO& hopperInfo)
{
	try{
		if(hopperInfo.ulCurCount < 0){
			hopperInfo.ulCurCount = 0;
		}
		hopperInfo.Serialize((LPBYTE)m_fileData+81);
		// ���²������ļ�
		UpdateMasterFile();
		m_HopperBInfo = hopperInfo;
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
@brief      ����Token��2�ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& tokenBoxInfo   Token��2������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetTokenboxWastedInfo(TOKEN_BOX_INFO& tokenBoxInfo)
{
	try{
		if(tokenBoxInfo.ulCurCount < 0){
			tokenBoxInfo.ulCurCount = 0;
		}
		tokenBoxInfo.Serialize((LPBYTE)m_fileData+108);
		// ���²������ļ�
		UpdateMasterFile();
		m_TokenboxWastedInfo = tokenBoxInfo;
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
@brief      ����Token������ļ�����Ϣ

@param    (i)TOKEN_BOX_INFO& tokenBoxInfo   Token�����������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTTokenCountInfo::SetTokenboxCollectedInfo(TOKEN_BOX_INFO& collectedBoxInfo)
{
	try{
		if(collectedBoxInfo.ulCurCount < 0){
			collectedBoxInfo.ulCurCount = 0;
		}
		collectedBoxInfo.Serialize((LPBYTE)m_fileData+135);
		// ���²������ļ�
		UpdateMasterFile();
		m_TokenboxCollectedInfo = collectedBoxInfo;
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
void CTTokenCountInfo::UpdateMasterFile()
{
    try {
        // �����ļ���
        m_lock.Lock();
		// д�ļ�
        if(m_file->WriteBinFile(&m_fileData[0], TOKEN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTTokenCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// �����ļ���
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], TOKEN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
/*
@brief      ����HopperID

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTTokenCountInfo::UpdateHopperId(){
	//WORD wMachineCode;
	//WORD wStationCode = theMAINTENANCE_INFO.GetStationCode(); 
	//int	  nMachineCode = theMAINTENANCE_INFO.GetLineCode();

	//BYTE bHiByte;
	//int2BCD(theMAINTENANCE_INFO.GetStationCode(), (char *)&bHiByte, sizeof(bHiByte));

	//m_HopperAInfo.tokenBoxID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
	//m_HopperAInfo.tokenBoxID.bType = 0x04;

	//wMachineCode = (nMachineCode)*2-1; 
	//m_HopperAInfo.tokenBoxID.wNum = (bHiByte << 8) | (wMachineCode & 0xff);
	//SetHopperAInfo(m_HopperAInfo);

	//m_HopperBInfo.tokenBoxID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
	//m_HopperBInfo.tokenBoxID.bType = 0x04;
	//
	//wMachineCode = (nMachineCode)*2; 
	//m_HopperBInfo.tokenBoxID.wNum = (bHiByte << 8) | (wMachineCode & 0xff);
	//SetHopperBInfo(m_HopperBInfo);

	// token��ID����ͬ��
	/*m_TokenboxWastedInfo.tokenBoxID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
	m_TokenboxWastedInfo.tokenBoxID.bType = 0x95;
	wMachineCode = ((int)theMAINTENANCE_INFO.GetMachineCode())*2-1;
	m_TokenboxWastedInfo.tokenBoxID.wNum = (wStationCode << 8) | (wMachineCode & 0xff);
	SetTokenboxWastedInfo(m_TokenboxWastedInfo);*/
}

CTTokenCountInfo CTTokenCountInfo::theInstance;