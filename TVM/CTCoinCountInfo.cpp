#include "StdAfx.h"
#include "CTCoinCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const COIN_COUNT_INFO_FILE_NAME		= _T("CoinCountInfo.mst");          // Ӳ���������Ϣ�ļ���

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��
 
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::CTCoinCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, COIN_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::~CTCoinCountInfo(void)
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
BOOL CTCoinCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + COIN_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + COIN_COUNT_INFO_FILE_NAME;
	CString radixchange;
	int Num_radix10 = -1;

    try{
        m_file = new CDXFile( file, fileBack);
        // �����������ļ�
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// �ļ������쳣
        if ( COIN_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// �Ƿ����״δ����ļ�
			if(0 == binfileData.nLen){               
				// ����Ϣд���ļ�
				//SetCoinAddboxAInfo(m_CoinAddBoxAInfo);
				//SetCoinAddboxBInfo(m_CoinAddBoxBInfo);
				
				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				// ��ʼ��Ӳ��ר��������AID
				int2BCD(bLineCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bLineCode, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bLineCode));
				m_CoinHopperBoxAInfo.coinBoxID.bType = 0x09;
				int2BCD(bStationCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bNumInQu, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bNumInQu));
				bMachineCode = 2*theMAINTENANCE_INFO.GetMachineCode()-1;
				int2BCD(bMachineCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bNum, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bNum));
				m_CoinHopperBoxAInfo.ulCurCount = 0;
				SetCoinHopperboxAInfo(m_CoinHopperBoxAInfo);

				// ��ʼ��Ӳ��ר��������BID
				int2BCD(bLineCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bLineCode, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bLineCode));
				m_CoinHopperBoxBInfo.coinBoxID.bType = 0x09;
				int2BCD(bStationCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bNumInQu, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bNumInQu));
				bMachineCode = 2*theMAINTENANCE_INFO.GetMachineCode();
				int2BCD(bMachineCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bNum, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bNum));
				m_CoinHopperBoxBInfo.ulCurCount = 0;
				SetCoinHopperboxBInfo(m_CoinHopperBoxBInfo);

				// �������Ŷ�RFID
				SetCollectionCoinboxAInfo(m_CoinCollectionBoxAInfo);
				SetCollectionCoinboxBInfo(m_CoinCollectionBoxBInfo);

				SetCoinTemporary1Info(m_CoinTemporary1Info);
				SetCoinTemporary2Info(m_CoinTemporary2Info);
				// �������Ŷ�RFID
				SetChangeCoinboxAInfo(m_CoinChangeBoxAInfo);
				SetChangeCoinboxBInfo(m_CoinChangeBoxBInfo);
			}
			// �ļ����ȷǷ�
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// �ļ�����
		else{
			
			memcpy(m_fileData, binfileData.lpData, COIN_COUNT_INFO_FILE_LEN);
			// �����ļ�
			LPBYTE point = binfileData.lpData;
			//m_CoinAddBoxAInfo.Deserialize(point);
			//point+=16;
			//m_CoinAddBoxBInfo.Deserialize(point);
			//point+=16;
			m_CoinHopperBoxAInfo.Deserialize(point);
			point+=16;
			m_CoinHopperBoxBInfo.Deserialize(point);
			point+=16;
//			m_CoinHopperBoxCInfo.Deserialize(point);
//			point+=16;
			m_CoinCollectionBoxAInfo.Deserialize(point);
			point+=16;
			m_CoinCollectionBoxBInfo.Deserialize(point);
			point+=16;
			m_CoinTemporary1Info.Deserialize(point);
			point+=16;
			m_CoinTemporary2Info.Deserialize(point);
			point+=16;
			m_CoinChangeBoxAInfo.Deserialize(point);
			point+=16;
			m_CoinChangeBoxBInfo.Deserialize(point);
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
@brief      ����Ӳ�Ҳ�����A�ļ�����Ϣ

@param     (i)COIN_ADD_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CTCoinCountInfo::SetCoinAddboxAInfo(COIN_ADD_BOX_INFO& boxInfo)
//{
//	try{
//		boxInfo.CheckMinusValue();
//		boxInfo.Serialize((LPBYTE)m_fileData);
//		// ���²������ļ�
//		UpdateMasterFile();
//		m_CoinAddBoxAInfo = boxInfo;
//		return TRUE;
//	}
//	// �쳣����
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡӲ�Ҳ�����A�ļ�����Ϣ

@param		none

@retval     COIN_ADD_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//CTCoinCountInfo::COIN_ADD_BOX_INFO& CTCoinCountInfo::GetCoinAddboxAInfo()
//{
//	 return m_CoinAddBoxAInfo;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ�Ҳ�����B�ļ�����Ϣ

@param     (i)COIN_ADD_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CTCoinCountInfo::SetCoinAddboxBInfo(COIN_ADD_BOX_INFO& boxInfo)
//{
//	try{
//		boxInfo.CheckMinusValue();
//		boxInfo.Serialize((LPBYTE)m_fileData+16);
//		// ���²������ļ�
//		UpdateMasterFile();
//		m_CoinAddBoxBInfo = boxInfo;
//		return TRUE;
//	}
//	// �쳣����
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡӲ�Ҳ�����B�ļ�����Ϣ

@param      none

@retval     COIN_ADD_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//CTCoinCountInfo::COIN_ADD_BOX_INFO& CTCoinCountInfo::GetCoinAddboxBInfo()
//{
//	 return m_CoinAddBoxBInfo;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ��HOPPER��A�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinHopperboxAInfo(COIN_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCurCount==0){
			// ���ղ���
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// ��������
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinHopperBoxAInfo = boxInfo;
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
@brief      ��ȡӲ��HOPPER��A�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_BOX_INFO& CTCoinCountInfo::GetCoinHopperboxAInfo()
{
	 return m_CoinHopperBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ��HOPPER��B�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinHopperboxBInfo(COIN_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+16);
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinHopperBoxBInfo = boxInfo;
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
@brief      ��ȡӲ��HOPPER��B�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_BOX_INFO& CTCoinCountInfo::GetCoinHopperboxBInfo()
{
	 return m_CoinHopperBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ�һ�����A�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCollectionCoinboxAInfo(COIN_COLLECTION_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+32);
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinCollectionBoxAInfo = boxInfo;
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
@brief      ��ȡӲ�һ�����A�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& CTCoinCountInfo::GetCollectionCoinboxAInfo()
{
	 return m_CoinCollectionBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ�һ�����B�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCollectionCoinboxBInfo(COIN_COLLECTION_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+48);
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinCollectionBoxBInfo = boxInfo;
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
@brief      ��ȡӲ�һ�����B�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& CTCoinCountInfo::GetCollectionCoinboxBInfo()
{
	 return m_CoinCollectionBoxBInfo;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ�һ�����A�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetChangeCoinboxAInfo(COIN_CHANGE_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+96);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCount==0){
			// ���ղ���
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// ��������
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinChangeBoxAInfo = boxInfo;
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
@brief      ��ȡӲ�һ�����A�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_CHANGE_BOX_INFO& CTCoinCountInfo::GetChangeCoinboxAInfo()
{
	return m_CoinChangeBoxAInfo;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ��������B�ļ�����Ϣ

@param     (i)COIN_BOX_INFO& boxInfo  Ӳ���������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetChangeCoinboxBInfo(COIN_CHANGE_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+112);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCount==0){
			// ���ղ���
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// ��������
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinChangeBoxBInfo = boxInfo;
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
@brief      ��ȡӲ�һ�����B�ļ�����Ϣ

@param      none

@retval     COIN_BOX_INFO& Ӳ����ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_CHANGE_BOX_INFO& CTCoinCountInfo::GetChangeCoinboxBInfo()
{
	return m_CoinChangeBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ���ݴ���1�ļ�����Ϣ

@param     (i)COIN_TEMPORARY1_INFO& boxInfo  Ӳ���ݴ���1������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinTemporary1Info(COIN_TEMPORARY1_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+64);
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinTemporary1Info = boxInfo;
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
@brief      ��ȡӲ���ݴ���1�ļ�����Ϣ

@param      none

@retval     COIN_TEMPORARY1_INFO& Ӳ���ݴ���1�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_TEMPORARY1_INFO& CTCoinCountInfo::GetCoinTemporary1Info()
{
	return m_CoinTemporary1Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ���ݴ���2�ļ�����Ϣ

@param     (i)COIN_TEMPORARY2_INFO& boxInfo  Ӳ���ݴ���1������Ϣ

@retval     BOOL TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinTemporary2Info(COIN_TEMPORARY2_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+80);
		// ���²������ļ�
		UpdateMasterFile();
		m_CoinTemporary2Info = boxInfo;
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
@brief      ��ȡӲ���ݴ���2�ļ�����Ϣ

@param      none

@retval     COIN_TEMPORARY2_INFO& Ӳ���ݴ���2�ļ�����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_TEMPORARY2_INFO& CTCoinCountInfo::GetCoinTemporary2Info()
{
	return m_CoinTemporary2Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ļ�������

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTCoinCountInfo::UpdateMasterFile()
{
    try {
        // �����ļ���
        m_lock.Lock();
		// д�ļ�
        if(m_file->WriteBinFile(&m_fileData[0], COIN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTCoinCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// �����ļ���
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], COIN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
@brief      ͬ��Ӳ�����������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::UpdataBoxCountData(int nHopper1,int nHooper2,int nChangeBox1,int nChangeBox2,int nColl1,int nColl2){
	try{
		if(nHopper1 >= 0){
			m_CoinHopperBoxAInfo.ulCurCount = nHopper1;
		}
		if(nHooper2 >= 0){
			m_CoinHopperBoxBInfo.ulCurCount = nHooper2;
		}
		if(nChangeBox1 >= 0){
			m_CoinChangeBoxAInfo.ulCount = nChangeBox1;
		}

		if(nChangeBox2 >= 0){
			m_CoinChangeBoxBInfo.ulCount = nChangeBox2;
		}
		// ������A������Ϊ��
		if(nColl1 >= 0){
			m_CoinCollectionBoxAInfo.ulCount_1yuan = nColl1;
		}
		// ��ǰ������A�������
		if(nColl2 >= 0){
			m_CoinCollectionBoxAInfo.ulCount_5jiao = nColl2;
		}

		m_CoinHopperBoxAInfo.Serialize((LPBYTE)m_fileData);
		m_CoinHopperBoxBInfo.Serialize((LPBYTE)m_fileData + 16);
		m_CoinCollectionBoxAInfo.Serialize((LPBYTE)m_fileData + 32);
		m_CoinCollectionBoxBInfo.Serialize((LPBYTE)m_fileData + 48);
		m_CoinChangeBoxAInfo.Serialize((LPBYTE)m_fileData + 96);
		m_CoinChangeBoxBInfo.Serialize((LPBYTE)m_fileData + 112);

		UpdateMasterFile();

		theAPP_SESSION.SetCoinNeedSyncMasterFile(false);
	}
	catch(CSysException&){
		throw;
	}
}

CTCoinCountInfo CTCoinCountInfo::theInstance;
