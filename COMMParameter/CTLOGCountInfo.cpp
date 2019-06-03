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
@brief      构造函数

@param      无
 
@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::~CCTLOGCountInfo(void)
{
    try{
        delete m_file;
        m_file = NULL;
    }
      
    // 异常捕获
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期处理

@param      (i)CString dataPath         数据主目录
@param      (i)CString backupPath       备份主目录

@retval     BOOL \n
            TRUE: 成功      FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCTLOGCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + DEVICE_MASTER_DIR + COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + DEVICE_MASTER_DIR + COUNT_INFO_FILE_NAME;

    try{
        m_file = new CDXFile( file, fileBack);

        // 读出二进制文件
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
        if ( COUNT_INFO_LEN != binfileData.nLen){
			if(0 == binfileData.nLen){
				// 取得当前时间
				CTime curDate = CTime::GetCurrentTime();  

				// 将当前时间计入内存
				m_auditEndTime.wYear = curDate.GetYear();
				m_auditEndTime.biMonth = curDate.GetMonth();
				m_auditEndTime.biDay = curDate.GetDay();
				m_auditEndTime.biHour = curDate.GetHour();
				m_auditEndTime.biMinute = curDate.GetMinute();
				m_auditEndTime.biSecond = curDate.GetSecond();
                
				// 初始化UDSN为-1 
				m_ulUDSN = 0;
				m_transactionNum = 0;
				m_eventNum = 0;
				m_maintenceNum = -1;
				m_ECTNum = 0;

				// 将信息写入文件
				SetTicketboxAInfo(m_TicketboxAInfo);
				SetTicketboxBInfo(m_TicketboxBInfo);
				SetTicketboxCInfo(m_TicketboxCInfo);
				//SetAuditEndTime(m_auditEndTime);
				SetUDSN(m_ulUDSN);
				SetTransactionNum(m_transactionNum);
				SetEventNum(m_eventNum);
				SetMaintenceNum(m_maintenceNum);
				SetYKTNum(m_ECTNum);

				//操作员计数写入文件
				m_loginCount = 0;
				SetLoginCount(m_loginCount);
				// 将日志日期写入mastar
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

			// 解析文件
			LPBYTE point = binfileData.lpData;
			m_TicketboxAInfo.Deserialize(point);
			point+=16;

			m_TicketboxBInfo.Deserialize(point);
			point+=16;

			m_TicketboxCInfo.Deserialize(point);
			point+=16;

			// 审计结束时间
			m_auditEndTime = BCD2Time((char*)point);
			point += 7;

			// acc计数
			m_ulUDSN = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// afc计数
			m_transactionNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// event计数
			m_eventNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// maintence计数
			m_maintenceNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;
			// 一卡通计数
			m_ECTNum = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;

			//操作员计数
			m_loginCount = ComMakeLong(*point, *(point + 1), *(point + 2), *(point + 3));
			point += 4;

			// 送信失败的日期
			m_failLogDate = BCD2int((char*)point,4);
			point += 4;

			if(binfileData.lpData != NULL){
				delete binfileData.lpData;
				binfileData.lpData = NULL;
			}
		}	
            return TRUE;
    }

    // 异常捕获
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
    }    
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取票箱1的计数信息

@param      (o) COUNT_INFO& ciData        票箱1的计数信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxAInfo()
{
    return m_TicketboxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取票箱2的计数信息

@param      (o) COUNT_INFO& ciData        票箱2的计数信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxBInfo()
{
  return m_TicketboxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取废票箱的计数信息

@param      (o) COUNT_INFO& ciData        票箱2的计数信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGCountInfo::TICKETBOX_INFO & CCTLOGCountInfo::GetTicketboxCInfo()
{
	return m_TicketboxCInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置票箱1的计数信息

@param      (i) const COUNT_INFO ciData        计数信息

@retval     BOOL \n
            TRUE: 成功  FALSE: 失败

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
        // 更新并保存文件
        UpdateMasterFile();
        m_TicketboxAInfo = ticketBoxInfo;
        return TRUE;
    }

    // 异常捕获
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置回收箱的当前张数

@param      (i)WORD collectionTicketCount 回收箱当前张数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetCollectionCurCount(WORD collectionTicketCount)
{
    m_collectionTicketCount = collectionTicketCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置回收箱的当前张数

@param      无

@retval     WORD    回收箱的当前张数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CCTLOGCountInfo::GetCollectionCurCount()
{
    return m_collectionTicketCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置票箱2的计数信息

@param      (i) const COUNT_INFO ciData        计数信息

@retval     BOOL \n
            TRUE: 成功  FALSE: 失败

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
        // 更新并保存文件
        UpdateMasterFile();
        m_TicketboxBInfo = ticketBoxInfo;
        return TRUE;
    }

    // 异常捕获
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      设置废票箱的计数信息

@param      (i) const COUNT_INFO ciData        计数信息

@retval     BOOL \n
            TRUE: 成功  FALSE: 失败

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
        // 更新并保存文件
        UpdateMasterFile();
        m_TicketboxCInfo = ticketBoxInfo;
        return TRUE;
    }

    // 异常捕获
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
        // 当udsn为最大值时写入1，返回1
        if(0xFFFFFFFF == udsn) {
			udsn = 0;
			m_ulUDSN = 0;
			ret = false;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
        }
        ULONG ulUDSN = ltoml(udsn + 1);
        memcpy(&m_fileData[39+16], &ulUDSN, 4);
        // 更新并保存文件
        UpdateMasterFile();
        // 累计m_ulUDSN
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
@brief      设置终端设备交易计数(不备份文件)

@param      (i) ULONG   udsn    交易计数

@retval     bool    成功返回true

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetUDSNNoCopy(ULONG udsn)
{
	try{
		bool bResetNum = false;
		// 当udsn为最大值时写入1，返回1
		if(0xFFFFFFFF == udsn) {
			udsn = 0;
			m_ulUDSN = 0;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
		}
		ULONG ulUDSN = ltoml(udsn + 1);
		memcpy(&m_fileData[39+16], &ulUDSN, 4);
		// 更新并保存文件
		UpdateMasterFileNoCopy();

		// 累计m_ulUDSN
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
@brief      设置终端设备交易计数

@param      (i) ULONG   udsn    交易计数

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetUDSNForMain(ULONG udsn)
{
    try{
		// 当udsn为最大值时写入1，返回1
		if(0xFFFFFFFF == udsn) {
			CounterReseted.Invoke(COUNTER_TYPE_TRANS_ACC);
			udsn = 1;
		}
        ULONG ulUDSN = ltoml(udsn);
        memcpy(&m_fileData[39+16], &ulUDSN, 4);

        // 更新并保存文件
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
@brief      得到终端设备交易计数

@param      bool &isReset 交易计数是否重置

@retval     ULONG

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetUDSN()
{
    try{
		ULONG currentUDSN = m_ulUDSN;
        // 更新文件中的交易计数器
        if(!SetUDSN(m_ulUDSN)){            
        }
		// 返回交易计数值
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
@brief      更新终端设备交易计数

@param      uchar 交易计数累加值

@retval     bool true:计数重置  false:计数不重置

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
@brief      更新一卡通设备交易计数

@param      uchar 交易计数累加值

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
@brief      得到终端设备交易计数，不累加

@param      void

@retval     ULONG

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetUDSNForDisplay()
{
    return m_ulUDSN;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      得到最后笔业务的AFC计数

@param      无

@retval     ULONG   最后笔业务的AFC计数

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedUDSN()
{
	return (m_ulUDSN - 1)==0 ? 0xFFFFFFFF:(m_ulUDSN - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
  @brief      写入transaction计数

  @param      (i)ULONG oldtransactionNum   当前m_transaction的值

  @retval     bool    \n      
              true:写入成功     false:写入失败

  @exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetTransactionNum(ULONG oldTransactionNum)
{
    try{
		bool bResetNum = false;
        if ( 0xFFFFFFFF == oldTransactionNum) {
            // 重新计数从1开始
            oldTransactionNum = 0;
            m_transactionNum = 0;
            // m_currentTraNum = 1;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
			bResetNum = true;
        }
        ULONG newTransactionNum = ltoml(oldTransactionNum+1);
        memcpy(&m_fileData[43+16], &newTransactionNum, 4);
        // 更新并保存文件
        UpdateMasterFile();
        // 累计m_transactionNum
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
@brief      写入transaction计数(不备份文件)

@param      (i)ULONG oldtransactionNum   当前m_transaction的值

@retval     bool    \n      
true:写入成功     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetTransactionNumNoCopy(ULONG oldTransactionNum)
{
	try{
		bool bResetNum = false;
		if ( 0xFFFFFFFE == oldTransactionNum) {
			// 重新计数从1开始
			oldTransactionNum = 0;
			m_transactionNum = 0;
			m_currentTraNum = 0;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
		}
		ULONG newTransactionNum = ltoml(oldTransactionNum+1);
		memcpy(&m_fileData[43+16], &newTransactionNum, 4);
		// 更新并保存文件
		UpdateMasterFileNoCopy();
		// 累计m_transactionNum
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
@brief      写入transaction计数

@param      (i)ULONG oldtransactionNum   当前m_transaction的值

@retval     bool    \n      
true:写入成功     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetTransactionNumForMain(ULONG oldTransactionNum)
{
    try{
		bool bResetNum = false;
		// 当udsn为最大值时写入1，返回1
		if(0xFFFFFFFF == oldTransactionNum) {
			oldTransactionNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_AFC);
		}
        ULONG newTransactionNum = ltoml(oldTransactionNum);
        memcpy(&m_fileData[43+16], &newTransactionNum, 4);
        // 更新并保存文件
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
@brief      得到transaction计数

@param      void

@retval     ULONG

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetTransactionNum()
{
    try{
        m_currentTraNum = m_transactionNum;
        // 更新文件中的transaction计数
        if(SetTransactionNum(m_transactionNum)){
            // 返回计数
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
@brief      得到transaction计数，不累加

@param      void

@retval     ULONG

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetTransactionNumForDsp()
{
    return m_transactionNum;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到最后笔业务的AFC计数

@param      无

@retval     ULONG   最后笔业务的AFC计数

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedTransactionNum()
{
	return (m_transactionNum - 1)==0 ? 0xFFFFFFFF:(m_transactionNum - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
  @brief      写入Event计数

  @param      (i)ULONG oldEventNum      当前计数

  @retval     bool  \n
              true:成功写入     false:写入失败

  @exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetEventNum(ULONG oldEventNum)
{
    try{
		bool bResetNum = false;
        if (0xFFFFFFFE == oldEventNum) {
            // 重新计数从1开始
            oldEventNum = 0;
            m_eventNum = 0;
           // m_currentEveNum = 1;
			bResetNum = true;
        }
        ULONG newEventNum = ltoml(oldEventNum+1);
        memcpy(&m_fileData[47+16], &newEventNum, 4);
        // 更新并保存文件
        UpdateMasterFile();
        // 累计m_eventNum
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
@brief      写入Event计数

@param      (i)ULONG oldEventNum      当前计数

@retval     bool  \n
true:成功写入     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetEventNumForMain(ULONG oldEventNum)
{
    try{
		bool bResetNum = false;
		// 当udsn为最大值时写入1，返回1
		if(0xFFFFFFFF == oldEventNum) {
			oldEventNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_EVENT);
		}
        ULONG newEventNum = ltoml(oldEventNum);
        memcpy(&m_fileData[47+16], &newEventNum, 4);

        // 更新并保存文件
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
  @brief      得到event计数

  @param      (i)

  @retval     ULONG         当前eventNum的值

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
@brief      得到event计数，不累加 

@param      无

@retval     ULONG         当前eventNum的值

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetEventNumForDsp()
{
    return m_eventNum;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      得到最后笔业务的事件计数

@param      无

@retval     ULONG   最后笔业务的事件计数

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedEventNum()
{
	return (m_eventNum - 1)==0 ? 0xFFFFFFFF:(m_eventNum - 1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      写入Maintence计数

@param      (i)ULONG oldMaintenceNum      当前计数

@retval     bool  \n
true:成功写入     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetMaintenceNum(ULONG oldMaintenceNum)
{
    try{
        if (0xFFFFFFFE == oldMaintenceNum) {
            // 重新计数从1开始
            oldMaintenceNum = 0;
            m_maintenceNum = 0;
            //m_currentMainNum = 0;
			//CounterReseted.Invoke(COUNTER_TYPE_EVENT);
        }
        ULONG newMaintenceNum = ltoml(oldMaintenceNum+1);
        memcpy(&m_fileData[51+16], &newMaintenceNum, 4);
        // 更新并保存文件
        UpdateMasterFile();
        // 累计 m_maintenceNum
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
@brief      得到event计数

@param      (i)

@retval     ULONG         当前eventNum的值

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
@brief      写入一卡通计数

@param      (i)ULONG oldYKTNum      当前计数

@retval     bool  \n
true:成功写入     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGCountInfo::SetYKTNum(ULONG oldYKTNum)
{
    try{
		bool bResetNum = false;
        if (0xFFFFFFFF == oldYKTNum) {
            // 重新计数从1开始
            oldYKTNum = 0;
            m_ECTNum = 0;
			bResetNum = true;
            //m_currentECTNum = 1;
        }
        ULONG newYKTNum = ltoml(oldYKTNum+1);
        memcpy(&m_fileData[55+16], &newYKTNum, 4);

        // 更新并保存文件
        UpdateMasterFile();
        // 累计 m_YKTNum
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
@brief      写入一卡通计数

@param      (i)ULONG oldYKTNum      当前计数

@retval     bool  \n
true:成功写入     false:写入失败

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::SetECTNumForMain(ULONG oldYKTNum)
{
    try{
		bool bResetNum = false;
		// 当udsn为最大值时写入1，返回1
		if(0xFFFFFFFF == oldYKTNum) {
			oldYKTNum = 1;
			bResetNum = true;
			//CounterReseted.Invoke(COUNTER_TYPE_TRANS_ECT);
		}
        ULONG newYKTNum = ltoml(oldYKTNum);
        memcpy(&m_fileData[55+16], &newYKTNum, 4);

        // 更新并保存文件
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
@brief      得到一卡通计数

@param      (i)

@retval     ULONG         当前一卡通的值

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
@brief      得到一卡通计数，不累加

@param      无

@retval     ULONG   当前一卡通的值

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetECTNumForDsp()
{
    return m_ECTNum;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到最后笔业务的一卡通计数

@param      无

@retval     ULONG   最后笔业务的一卡通计数

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGCountInfo::GetLastUsedECTNum()
{
	return (m_ECTNum - 1)==0 ? 0xFFFFFFFF:(m_ECTNum - 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置操作员计数

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

        // 更新并保存文件
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
@brief      累加操作员计数

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
@brief      得到操作员计数

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
@brief      更新文件并保存

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::UpdateMasterFile()
{
    try {
        // 开启文件锁
        m_lock.Lock();

        if(m_file->WriteBinFile(&m_fileData[0], COUNT_INFO_LEN) != CXFile::FILE_OPERATION_OK,false) {
            throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
        }

        // 关闭文件锁
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
@brief      更新并保存文件(不备份文件)

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// 开启文件锁
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], COUNT_INFO_LEN) != CXFile::FILE_OPERATION_OK,false) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
		// 关闭文件锁
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
@brief      把主文件复制到备份文件

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
//void CCTLOGCountInfo::CopyCountBackupFile()
//{
//	try {
//		// 开启文件锁
//		m_lock.Lock();
//
//		m_file->CopyXFile();
//
//		// 关闭文件锁
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
@brief      得到终端设备交易计数(不备份文件)

@param      void

@retval     ULONG

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//ULONG CCTLOGCountInfo::GetUDSNNoCopy()
//{
//	try{    
//		// 更新文件中的交易计数器
//		if(SetUDSNNoCopy(m_ulUDSN)){
//			// 返回交易计数值
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
@brief      得到transaction计数(不备份文件)

@param      void

@retval     ULONG

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//ULONG CCTLOGCountInfo::GetTransactionNumNoCopy()
//{
//	try{
//		m_currentTraNum = m_transactionNum;
//		// 更新文件中的transaction计数
//		if(SetTransactionNumNoCopy(m_transactionNum)){
//			// 返回计数
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