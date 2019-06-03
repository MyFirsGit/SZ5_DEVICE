#include "StdAfx.h"
#include "CTLOGCountInfo.h"
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
CCTLOGCountInfo::CCTLOGCountInfo(void)
{
    m_file = NULL;
    memset(m_fileData, 0, COUNT_INFO_LEN);
    m_collectionTicketCount = 0;
    m_ulUDSN = 0;
    m_transactionNum = 0;
    m_eventNum = 0;
    m_maintenceNum = 0;
    m_ECTNum = 0;
    m_currentTraNum = 0;
    m_currentEveNum = 0;
    m_currentMainNum = 0;
    m_currentECTNum = 0;
    m_loginCount = 0;
    m_failLogDate = 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::~CCTLOGCountInfo(void)
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
BOOL CCTLOGCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + DEVICE_MASTER_DIR + COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + DEVICE_MASTER_DIR + COUNT_INFO_FILE_NAME;

    try{
        m_file = new CDXFile( file, fileBack);

        // �����������ļ�
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
        if ( COUNT_INFO_LEN != binfileData.nLen){
			if(0 == binfileData.nLen){
				// ȡ�õ�ǰʱ��
				CTime curDate = CTime::GetCurrentTime();  

				// ����ǰʱ������ڴ�
				m_auditEndTime.wYear = curDate.GetYear();
				m_auditEndTime.biMonth = curDate.GetMonth();
				m_auditEndTime.biDay = curDate.GetDay();
				m_auditEndTime.biHour = curDate.GetHour();
				m_auditEndTime.biMinute = curDate.GetMinute();
				m_auditEndTime.biSecond = curDate.GetSecond();
                
				// ��ʼ��UDSNΪ-1 
				m_ulUDSN = 0;
				m_transactionNum = 0;
				m_eventNum = 0;
				m_maintenceNum = -1;
				m_ECTNum = 0;

				// ����Ϣд���ļ�
				SetTicketboxAInfo(m_TicketboxAInfo);
				SetTicketboxBInfo(m_TicketboxBInfo);
				SetTicketboxCInfo(m_TicketboxCInfo);
				//SetAuditEndTime(m_auditEndTime);
				SetUDSN(m_ulUDSN);
				SetTransactionNum(m_transactionNum);
				SetEventNum(m_eventNum);
				SetMaintenceNum(m_maintenceNum);
				SetYKTNum(m_ECTNum);

				//����Ա����д���ļ�
				m_loginCount = 0;
				SetLoginCount(m_loginCount);
				// ����־����д��mastar
				//m_baseLogDate = 20060101;
				//m_failLogDate = 20060101;
				//SetLogDate(LOG_DATA_UPLOAD,m_baseLogDate);
				//SetLogDate(SEND_FAIL_MESSAGE_LOG,m_failLogDate);

				return TRUE;
			}
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{
			memcpy(m_fileData, binfileData.lpData, COUNT_INFO_LEN);

			// �����ļ�
			LPBYTE point = binfileData.lpData;
			m_TicketboxAInfo.Deserialize(point);
			point+=16;

			m_TicketboxBInfo.Deserialize(point);
			point+=16;

			m_TicketboxCInfo.Deserialize(point);
			point+=16;

			// ��ƽ���ʱ��
			m_auditEndTime = BCD2Time((char*)point);
			point += 7;

			// acc����
			m_ulUDSN = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// afc����
			m_transactionNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// event����
			m_eventNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// maintence����
			m_maintenceNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// һ��ͨ����
			m_ECTNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;

			//����Ա����
			m_loginCount = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;

			// ����ʧ�ܵ�����
			m_failLogDate = BCD2int((char*)point,4);
			point += 4;

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

@param      (o) COUNT_INFO& ciData        Ʊ��1�ļ�����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxAInfo()
{
    return m_TicketboxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��2�ļ�����Ϣ

@param      (o) COUNT_INFO& ciData        Ʊ��2�ļ�����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxBInfo()
{
  return m_TicketboxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Ʊ��ļ�����Ϣ

@param      (o) COUNT_INFO& ciData        Ʊ��2�ļ�����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxCInfo()
{
	return m_TicketboxCInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��1�ļ�����Ϣ

@param      (i) const COUNT_INFO ciData        ������Ϣ

@retval     BOOL \n
            TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGCountInfo::SetTicketboxAInfo(TICKETBOX_INFO& ticketBoxInfo)
{
    try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		ticketBoxInfo.Serialize((LPBYTE)m_fileData);
        // ���²������ļ�
        UpdateMasterFile();
        m_TicketboxAInfo = ticketBoxInfo;
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
@brief      ���û�����ĵ�ǰ����

@param      (i)WORD collectionTicketCount �����䵱ǰ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetCollectionCurCount(WORD collectionTicketCount)
{
    m_collectionTicketCount = collectionTicketCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���û�����ĵ�ǰ����

@param      ��

@retval     WORD    ������ĵ�ǰ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CCTLOGCountInfo::GetCollectionCurCount()
{
    return m_collectionTicketCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��2�ļ�����Ϣ

@param      (i) const COUNT_INFO ciData        ������Ϣ

@retval     BOOL \n
            TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGCountInfo::SetTicketboxBInfo(TICKETBOX_INFO& ticketBoxInfo)
{
    try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		ticketBoxInfo.Serialize((LPBYTE)m_fileData+16);
        // ���²������ļ�
        UpdateMasterFile();
        m_TicketboxBInfo = ticketBoxInfo;
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
@brief      ���÷�Ʊ��ļ�����Ϣ

@param      (i) const COUNT_INFO ciData        ������Ϣ

@retval     BOOL \n
            TRUE: �ɹ�  FALSE: ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGCountInfo::SetTicketboxCInfo(TICKETBOX_INFO& ticketBoxInfo)
{
    try{
		if(ticketBoxInfo.ulCurCount < 0){
			ticketBoxInfo.ulCurCount = 0;
		}
		ticketBoxInfo.Serialize((LPBYTE)m_fileData+32);
        // ���²������ļ�
        UpdateMasterFile();
        m_TicketboxCInfo = ticketBoxInfo;
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
bool CCTLOGCountInfo::SetUDSN(ULONG udsn)
{
    try{
		bool ret = true;
        // ��udsnΪ���ֵʱд��1������1
        if(0xFFFFFFFF == udsn) {
			udsn = 0;
			m_ulUDSN = 0;
			ret = false;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
        }
        ULONG ulUDSN = ltoml(udsn + 1);
        memcpy(&m_fileData[39+16], &ulUDSN, 4);
        // ���²������ļ�
        UpdateMasterFile();
        // �ۼ�m_ulUDSN
        m_ulUDSN++;
		if(!ret)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
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
@brief      �����ն��豸���׼���(�������ļ�)

@param      (i) ULONG   udsn    ���׼���

@retval     bool    �ɹ�����true

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetUDSNNoCopy(ULONG udsn)
{
	try{
		bool bResetNum = false;
		// ��udsnΪ���ֵʱд��1������1
		if(0xFFFFFFFF == udsn) {
			udsn = 0;
			m_ulUDSN = 0;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
		}
		ULONG ulUDSN = ltoml(udsn + 1);
		memcpy(&m_fileData[39+16], &ulUDSN, 4);
		// ���²������ļ�
		UpdateMasterFileNoCopy();

		// �ۼ�m_ulUDSN
		m_ulUDSN++;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
		return true;

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
@brief      �����ն��豸���׼���

@param      (i) ULONG   udsn    ���׼���

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetUDSNForMain(ULONG udsn)
{
    try{
		// ��udsnΪ���ֵʱд��1������1
		if(0xFFFFFFFF == udsn) {
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
			udsn = 1;
		}
        ULONG ulUDSN = ltoml(udsn);
        memcpy(&m_fileData[39+16], &ulUDSN, 4);

        // ���²������ļ�
        UpdateMasterFile();

        m_ulUDSN = udsn;
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
@brief      �õ��ն��豸���׼���

@param      bool &isReset ���׼����Ƿ�����

@retval     ULONG

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetUDSN()
{
    try{
		ULONG currentUDSN = m_ulUDSN;
        // �����ļ��еĽ��׼�����
        if(!SetUDSN(m_ulUDSN)){            
        }
		// ���ؽ��׼���ֵ
		return currentUDSN;
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
@brief      �����ն��豸���׼���

@param      uchar ���׼����ۼ�ֵ

@retval     bool true:��������  false:����������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::UpdateUDSN(BYTE addVal)
{
	int i = 0;
	bool ret = false;
	for(i=0;i<addVal;i++){
		GetUDSN();
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨ�豸���׼���

@param      uchar ���׼����ۼ�ֵ

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void  CCTLOGCountInfo::UpdateECTNum(BYTE addVal)
{
	int i = 0;
	for(i=0;i<addVal;i++){
		GetECTNum();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��ն��豸���׼��������ۼ�

@param      void

@retval     ULONG

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetUDSNForDisplay()
{
    return m_ulUDSN;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����ҵ���AFC����

@param      ��

@retval     ULONG   ����ҵ���AFC����

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedUDSN()
{
	return (m_ulUDSN - 1)==0 ? 0xFFFFFFFF:(m_ulUDSN - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
  @brief      д��transaction����

  @param      (i)ULONG oldtransactionNum   ��ǰm_transaction��ֵ

  @retval     bool    \n      
              true:д��ɹ�     false:д��ʧ��

  @exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetTransactionNum(ULONG oldTransactionNum)
{
    try{
		bool bResetNum = false;
        if ( 0xFFFFFFFF == oldTransactionNum) {
            // ���¼�����1��ʼ
            oldTransactionNum = 0;
            m_transactionNum = 0;
            // m_currentTraNum = 1;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
			bResetNum = true;
        }
        ULONG newTransactionNum = ltoml(oldTransactionNum+1);
        memcpy(&m_fileData[43+16], &newTransactionNum, 4);
        // ���²������ļ�
        UpdateMasterFile();
        // �ۼ�m_transactionNum
        m_transactionNum++;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
        return true;
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
@brief      д��transaction����(�������ļ�)

@param      (i)ULONG oldtransactionNum   ��ǰm_transaction��ֵ

@retval     bool    \n      
true:д��ɹ�     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetTransactionNumNoCopy(ULONG oldTransactionNum)
{
	try{
		bool bResetNum = false;
		if ( 0xFFFFFFFE == oldTransactionNum) {
			// ���¼�����1��ʼ
			oldTransactionNum = 0;
			m_transactionNum = 0;
			m_currentTraNum = 0;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
		}
		ULONG newTransactionNum = ltoml(oldTransactionNum+1);
		memcpy(&m_fileData[43+16], &newTransactionNum, 4);
		// ���²������ļ�
		UpdateMasterFileNoCopy();
		// �ۼ�m_transactionNum
		m_transactionNum++;
		if(bResetNum){
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
		}
		return true;
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
@brief      д��transaction����

@param      (i)ULONG oldtransactionNum   ��ǰm_transaction��ֵ

@retval     bool    \n      
true:д��ɹ�     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetTransactionNumForMain(ULONG oldTransactionNum)
{
    try{
		bool bResetNum = false;
		// ��udsnΪ���ֵʱд��1������1
		if(0xFFFFFFFF == oldTransactionNum) {
			oldTransactionNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
		}
        ULONG newTransactionNum = ltoml(oldTransactionNum);
        memcpy(&m_fileData[43+16], &newTransactionNum, 4);
        // ���²������ļ�
        UpdateMasterFile();

        m_transactionNum = oldTransactionNum;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
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
@brief      �õ�transaction����

@param      void

@retval     ULONG

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetTransactionNum()
{
    try{
        m_currentTraNum = m_transactionNum;
        // �����ļ��е�transaction����
        if(SetTransactionNum(m_transactionNum)){
            // ���ؼ���
            return m_currentTraNum;
        }
        else{
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
@brief      �õ�transaction���������ۼ�

@param      void

@retval     ULONG

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetTransactionNumForDsp()
{
    return m_transactionNum;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����ҵ���AFC����

@param      ��

@retval     ULONG   ����ҵ���AFC����

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedTransactionNum()
{
	return (m_transactionNum - 1)==0 ? 0xFFFFFFFF:(m_transactionNum - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
  @brief      д��Event����

  @param      (i)ULONG oldEventNum      ��ǰ����

  @retval     bool  \n
              true:�ɹ�д��     false:д��ʧ��

  @exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetEventNum(ULONG oldEventNum)
{
    try{
		bool bResetNum = false;
        if (0xFFFFFFFE == oldEventNum) {
            // ���¼�����1��ʼ
            oldEventNum = 0;
            m_eventNum = 0;
           // m_currentEveNum = 1;
			bResetNum = true;
        }
        ULONG newEventNum = ltoml(oldEventNum+1);
        memcpy(&m_fileData[47+16], &newEventNum, 4);
        // ���²������ļ�
        UpdateMasterFile();
        // �ۼ�m_eventNum
        m_eventNum++;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_EVENT);
        return true;
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
@brief      д��Event����

@param      (i)ULONG oldEventNum      ��ǰ����

@retval     bool  \n
true:�ɹ�д��     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetEventNumForMain(ULONG oldEventNum)
{
    try{
		bool bResetNum = false;
		// ��udsnΪ���ֵʱд��1������1
		if(0xFFFFFFFF == oldEventNum) {
			oldEventNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_EVENT);
		}
        ULONG newEventNum = ltoml(oldEventNum);
        memcpy(&m_fileData[47+16], &newEventNum, 4);

        // ���²������ļ�
        UpdateMasterFile();
        m_eventNum = oldEventNum;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_EVENT);
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
  @brief      �õ�event����

  @param      (i)

  @retval     ULONG         ��ǰeventNum��ֵ

  @exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetEventNum()
{
    try{
        m_currentEveNum = m_eventNum;
        if (SetEventNum(m_eventNum)) {
            return m_currentEveNum;
        }
        else{
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
@brief      �õ�event���������ۼ� 

@param      ��

@retval     ULONG         ��ǰeventNum��ֵ

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetEventNumForDsp()
{
    return m_eventNum;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����ҵ����¼�����

@param      ��

@retval     ULONG   ����ҵ����¼�����

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedEventNum()
{
	return (m_eventNum - 1)==0 ? 0xFFFFFFFF:(m_eventNum - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      д��Maintence����

@param      (i)ULONG oldMaintenceNum      ��ǰ����

@retval     bool  \n
true:�ɹ�д��     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetMaintenceNum(ULONG oldMaintenceNum)
{
    try{
        if (0xFFFFFFFE == oldMaintenceNum) {
            // ���¼�����1��ʼ
            oldMaintenceNum = 0;
            m_maintenceNum = 0;
            //m_currentMainNum = 0;
			//CounterReseted.Invoke(COUNTER_TYPE_EVENT);
        }
        ULONG newMaintenceNum = ltoml(oldMaintenceNum+1);
        memcpy(&m_fileData[51+16], &newMaintenceNum, 4);
        // ���²������ļ�
        UpdateMasterFile();
        // �ۼ� m_maintenceNum
        m_maintenceNum++;
        return true;
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
@brief      �õ�event����

@param      (i)

@retval     ULONG         ��ǰeventNum��ֵ

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetMaintenceNum()
{
    try{
        m_currentMainNum = m_maintenceNum;
        if (SetMaintenceNum(m_maintenceNum)) {
            return m_currentMainNum;
        }
        else{
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
@brief      д��һ��ͨ����

@param      (i)ULONG oldYKTNum      ��ǰ����

@retval     bool  \n
true:�ɹ�д��     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetYKTNum(ULONG oldYKTNum)
{
    try{
		bool bResetNum = false;
        if (0xFFFFFFFF == oldYKTNum) {
            // ���¼�����1��ʼ
            oldYKTNum = 0;
            m_ECTNum = 0;
			bResetNum = true;
            //m_currentECTNum = 1;
        }
        ULONG newYKTNum = ltoml(oldYKTNum+1);
        memcpy(&m_fileData[55+16], &newYKTNum, 4);

        // ���²������ļ�
        UpdateMasterFile();
        // �ۼ� m_YKTNum
        m_ECTNum++;

		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ECT);
        return true;
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
@brief      д��һ��ͨ����

@param      (i)ULONG oldYKTNum      ��ǰ����

@retval     bool  \n
true:�ɹ�д��     false:д��ʧ��

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetECTNumForMain(ULONG oldYKTNum)
{
    try{
		bool bResetNum = false;
		// ��udsnΪ���ֵʱд��1������1
		if(0xFFFFFFFF == oldYKTNum) {
			oldYKTNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ECT);
		}
        ULONG newYKTNum = ltoml(oldYKTNum);
        memcpy(&m_fileData[55+16], &newYKTNum, 4);

        // ���²������ļ�
        UpdateMasterFile();
        m_ECTNum = oldYKTNum;
		if(bResetNum)
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ECT);
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
@brief      �õ�һ��ͨ����

@param      (i)

@retval     ULONG         ��ǰһ��ͨ��ֵ

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetECTNum()
{
    try{
        m_currentECTNum = m_ECTNum;
        if (SetYKTNum(m_ECTNum)) {
            return m_currentECTNum;
        }
        else{
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
@brief      �õ�һ��ͨ���������ۼ�

@param      ��

@retval     ULONG   ��ǰһ��ͨ��ֵ

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetECTNumForDsp()
{
    return m_ECTNum;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����ҵ���һ��ͨ����

@param      ��

@retval     ULONG   ����ҵ���һ��ͨ����

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedECTNum()
{
	return (m_ECTNum - 1)==0 ? 0xFFFFFFFF:(m_ECTNum - 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ò���Ա����

@param      (i)DWORD loginCount

@retval     bool

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetLoginCount(DWORD loginCount)
{
	try{
		ULONG lCount = ltoml(loginCount);
		memcpy(&m_fileData[59+16], &lCount, 4);

        // ���²������ļ�
        UpdateMasterFile();
		m_loginCount = loginCount;
		return true;
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
@brief      �ۼӲ���Ա����

@param      none

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
DWORD CCTLOGCountInfo::AccumulateLoginCount()
{
	try{
		if(0xFFFFFFFF == m_loginCount) {
			m_loginCount = 0;
		}
		if (SetLoginCount(m_loginCount + 1)) {
			return m_loginCount;
		}
		else{
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return m_loginCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����Ա����

@param      none

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
DWORD CCTLOGCountInfo::GetLoginCount()
{
	return m_loginCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ļ�������

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::UpdateMasterFile()
{
    try {
        // �����ļ���
        m_lock.Lock();

        if(m_file->WriteBinFile(&m_fileData[0], COUNT_INFO_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CCTLOGCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// �����ļ���
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], COUNT_INFO_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
@brief      �����ļ����Ƶ������ļ�

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
//void CCTLOGCountInfo::CopyCountBackupFile()
//{
//	try {
//		// �����ļ���
//		m_lock.Lock();
//
//		m_file->CopyXFile();
//
//		// �ر��ļ���
//		m_lock.Unlock();
//
//	}
//	catch(CSysException&) {
//		m_lock.Unlock();
//		throw;
//	}
//	catch (...) {
//		m_lock.Unlock();
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��ն��豸���׼���(�������ļ�)

@param      void

@retval     ULONG

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//ULONG CCTLOGCountInfo::GetUDSNNoCopy()
//{
//	try{    
//		// �����ļ��еĽ��׼�����
//		if(SetUDSNNoCopy(m_ulUDSN)){
//			// ���ؽ��׼���ֵ
//			return m_ulUDSN;
//		}
//		else{
//			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
//		}
//
//	}
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�transaction����(�������ļ�)

@param      void

@retval     ULONG

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//ULONG CCTLOGCountInfo::GetTransactionNumNoCopy()
//{
//	try{
//		m_currentTraNum = m_transactionNum;
//		// �����ļ��е�transaction����
//		if(SetTransactionNumNoCopy(m_transactionNum)){
//			// ���ؼ���
//			return m_currentTraNum;
//		}
//		else{
//			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
//		}
//	}
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//
//}

CCTLOGCountInfo CCTLOGCountInfo::theInstance;