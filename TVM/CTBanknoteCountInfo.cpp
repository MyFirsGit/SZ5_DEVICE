#include "StdAfx.h"
#include "CTBanknoteCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const BANKNOTE_COUNT_INFO_FILE_NAME		= _T("BanknoteCountInfo.mst");          // 纸币箱计数信息文件名
//static const TCHAR* const BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME = _T("BanknoteChangeCountInfo.mst");		// 纸币找零箱计数信息文件名

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无
 
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::CTBanknoteCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, BANKNOTE_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

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
BOOL CTBanknoteCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + BANKNOTE_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + BANKNOTE_COUNT_INFO_FILE_NAME;

// 	CString changeFile = dataPath + MASTER_DIR + BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME;
// 	CString ChangeFileBack = backupPath + MASTER_DIR + BANKNOTE_CHANGE_COUNT_INFO_FILE_NAME;
    try{
        m_file = new CDXFile( file, fileBack);

        // 读出二进制文件
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// 文件长度异常
        if ( BANKNOTE_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// 是否是首次创建文件
			if(0 == binfileData.nLen){               
				// 将信息写入文件
				SetBanknoteboxInfo(m_BanknoteBoxInfo);

				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				// 循环鼓A
				int2BCD(bLineCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bLineCode, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bLineCode));
				m_RecyclerBoxAInfo.banknoteBoxID.bType	   = 0x06;
				int2BCD(bStationCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bNumInQu, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bNumInQu));
				int2BCD(bMachineCode, (char*)&m_RecyclerBoxAInfo.banknoteBoxID.bNum, sizeof(m_RecyclerBoxAInfo.banknoteBoxID.bNum));
				m_RecyclerBoxAInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxAAmount()*100;
				m_RecyclerBoxAInfo.ulCurCount = 0;
				SetBanknoteRecyclerBoxAInfo(m_RecyclerBoxAInfo);
				// 循环鼓B
				m_RecyclerBoxBInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxBInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxBAmount()*100;
				m_RecyclerBoxBInfo.ulCurCount = 0;
				SetBanknoteRecyclerBoxBInfo(m_RecyclerBoxBInfo);
				// 循环鼓C
				m_RecyclerBoxCInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxCInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxCAmount()*100;
				m_RecyclerBoxCInfo.ulCurCount = 0;

				SetBanknoteRecyclerBoxCInfo(m_RecyclerBoxCInfo);
				// 循环鼓D
				m_RecyclerBoxDInfo.banknoteBoxID = m_RecyclerBoxAInfo.banknoteBoxID;
				m_RecyclerBoxDInfo.ulCurValue = theTVM_SETTING.GetBNRRecycleBoxDAmount()*100;
				m_RecyclerBoxDInfo.ulCurCount = 0;

				SetBanknoteRecyclerBoxDInfo(m_RecyclerBoxDInfo);
				m_ChangeBoxInfo.ulCurValue = theTVM_SETTING.GetBNRChangeBoxAmount()*100;
				m_ChangeBoxInfo.ulCurCount = 0;
				SetBanknoteChangeBoxInfo(m_ChangeBoxInfo);
			}
			// 文件长度非法
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// 文件正常
		else{
			memcpy(m_fileData, binfileData.lpData, BANKNOTE_COUNT_INFO_FILE_LEN);
			// 解析文件
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
    // 异常捕获
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
    }    

	// 纸币找零mst文件
	/*try{
		m_changefile = new CDXFile(changeFile,ChangeFileBack);

		// 读取二进制文件
		VARIABLE_DATA changeBinFileData;
		if(m_changefile->ReadBinFile(changeBinFileData) != CXFile::FILE_OPERATION_OK){
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}

		// 长度异常
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
			// 解析纸币箱A的数据
			m_BanknoteChangeBoxAInfo.Deserialize(ptobin);
			ptobin += 16;
			// 解析纸币箱B的数据
			m_BanknoteChangeBoxBInfo.Deserialize(ptobin);
			ptobin += 16;

			// 解析纸币废币箱的数据
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
@brief      读取纸币箱的计数信息

@param   none

@retval     BANKNOTE_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_BOX_INFO & CTBanknoteCountInfo::GetBanknoteboxInfo()
{
    return m_BanknoteBoxInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置纸币箱的计数信息

@param    (i)BANKNOTE_BOX_INFO& boxInfo  纸币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTBanknoteCountInfo::SetBanknoteboxInfo(BANKNOTE_BOX_INFO& boxInfo)
{
    try{
		//boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData);
        // 更新并保存文件
        UpdateMasterFile();
        m_BanknoteBoxInfo = boxInfo;
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
@brief     设置纸币循环鼓A的计数信息

@param    （i）BANKNOTE_RECYCLER_BOX_INFO& boxInfo  纸币找零箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
		// 更新并保存文件
		UpdateMasterFile();
		m_RecyclerBoxAInfo = boxInfo;
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
@brief      设置纸币循环鼓B的计数信息

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  纸币找零箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
		// 更新并保存文件
		UpdateMasterFile();
		m_RecyclerBoxBInfo = boxInfo;
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
@brief     设置纸币循环鼓C的计数信息

@param    （i）BANKNOTE_RECYCLER_BOX_INFO& boxInfo  纸币找零箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
		// 更新并保存文件
		UpdateMasterFile();
		m_RecyclerBoxCInfo = boxInfo;
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
@brief      设置纸币循环鼓D的计数信息

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  纸币找零箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
		// 更新并保存文件
		UpdateMasterFile();
		m_RecyclerBoxDInfo = boxInfo;
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
@brief      设置纸币找零箱的计数信息

@param    (i)BANKNOTE_RECYCLER_BOX_INFO& boxInfo  纸币找零箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

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
		// 更新并保存文件
		UpdateMasterFile();
		m_ChangeBoxInfo = boxInfo;
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
@brief      读取纸币循环鼓A的计数信息

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxAInfo()
{
	return m_RecyclerBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取纸币循环鼓B的计数信息

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxBInfo()
{
	return m_RecyclerBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取纸币循环鼓C的计数信息

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxCInfo()
{
	return m_RecyclerBoxCInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取纸币循环鼓D的计数信息

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteRecyclerBoxDInfo()
{
	return m_RecyclerBoxDInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取纸币找零箱的计数信息

@param   none

@retval     BANKNOTE_RECYCLER_BOX_INFO& 纸币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO & CTBanknoteCountInfo::GetBanknoteChangeBoxInfo()
{
	return m_ChangeBoxInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新文件并保存

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTBanknoteCountInfo::UpdateMasterFile()
{
    try {
        // 开启文件锁
        m_lock.Lock();
		
		// 写文件
		if(m_file->WriteBinFile(&m_fileData[0], BANKNOTE_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTBanknoteCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// 开启文件锁
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], BANKNOTE_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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

CTBanknoteCountInfo CTBanknoteCountInfo::theInstance;