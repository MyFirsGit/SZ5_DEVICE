#include "StdAfx.h"
#include "CTCardCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const CARD_COUNT_INFO_FILE_NAME		= _T("CardCountInfo.mst");          // 票卡计数信息文件名

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无
 
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CTCardCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, CARD_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::~CTCardCountInfo(void)
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

@retval     BOOL  TRUE: 成功      FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCardCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + CARD_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + CARD_COUNT_INFO_FILE_NAME;

    try{
        m_file = new CDXFile( file, fileBack);
        // 读出二进制文件
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// 文件长度异常
        if ( CARD_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// 是否是首次创建文件
			if(0 == binfileData.nLen){               
				// 将信息写入文件
				SetCardboxAInfo(m_CardboxAInfo);
				SetCardboxBInfo(m_CardboxBInfo);
				SetCardboxWastedInfo(m_CardboxWastedInfo);
			}
			// 文件长度非法
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// 文件正常
		else{
			memcpy(m_fileData, binfileData.lpData, CARD_COUNT_INFO_FILE_LEN);
			// 解析文件
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

@param   none

@retval     CARD_BOX_INFO& 票箱1的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxAInfo()
{
    return m_CardboxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取票箱2的计数信息

@param   none

@retval     CARD_BOX_INFO& 票箱2的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxBInfo()
{
  return m_CardboxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取废票箱的计数信息

@param   none

@retval     CARD_BOX_INFO& 废票箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCardCountInfo::CARD_BOX_INFO & CTCardCountInfo::GetCardboxWastedInfo()
{
	return m_CardboxWastedInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置票箱1的计数信息

@param    (i)CARD_BOX_INFO& ticketBoxInfo  票箱1计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
			//当前是卸载操作
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// 当前是安装操作
			theTVM_SETTING.SetCardBoxLastAction(1);
		}

		ticketBoxInfo.Serialize((LPBYTE)m_fileData);
        // 更新并保存文件
        UpdateMasterFile();
        m_CardboxAInfo = ticketBoxInfo;
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
@brief      设置票箱2的计数信息

@param    (i)CARD_BOX_INFO& ticketBoxInfo   票箱2计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
			//当前是卸载操作
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// 当前是安装操作
			theTVM_SETTING.SetCardBoxLastAction(1);
		}

		ticketBoxInfo.Serialize((LPBYTE)m_fileData+16);
		// 更新并保存文件
		UpdateMasterFile();
		m_CardboxBInfo = ticketBoxInfo;
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
@brief      设置票箱2的计数信息

@param    (i)CARD_BOX_INFO& ticketBoxInfo   票箱2计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
			//当前是卸载操作
			theTVM_SETTING.SetCardBoxLastAction(0);
		}else{
			// 当前是安装操作
			theTVM_SETTING.SetCardBoxLastAction(1);
		}
		ticketBoxInfo.Serialize((LPBYTE)m_fileData+32);
		// 更新并保存文件
		UpdateMasterFile();
		m_CardboxWastedInfo = ticketBoxInfo;
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
@brief      更新文件并保存

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTCardCountInfo::UpdateMasterFile()
{
    try {
        // 开启文件锁
        m_lock.Lock();
		// 写文件
        if(m_file->WriteBinFile(&m_fileData[0], CARD_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTCardCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// 开启文件锁
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], CARD_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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

CTCardCountInfo CTCardCountInfo::theInstance;