#include "StdAfx.h"
#include "CTCoinCountInfo.h"
#include "LogException.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const COIN_COUNT_INFO_FILE_NAME		= _T("CoinCountInfo.mst");          // 硬币箱计数信息文件名

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无
 
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::CTCoinCountInfo(void)
{
    m_file = NULL;
	memset(m_fileData, 0, COIN_COUNT_INFO_FILE_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::~CTCoinCountInfo(void)
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
BOOL CTCoinCountInfo::Internalize(CString dataPath, CString backupPath)
{
    CString file = dataPath + MASTER_DIR + COIN_COUNT_INFO_FILE_NAME;
    CString fileBack = backupPath + MASTER_DIR + COIN_COUNT_INFO_FILE_NAME;
	CString radixchange;
	int Num_radix10 = -1;

    try{
        m_file = new CDXFile( file, fileBack);
        // 读出二进制文件
        VARIABLE_DATA binfileData;
        if(m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
            throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
        }
		// 文件长度异常
        if ( COIN_COUNT_INFO_FILE_LEN != binfileData.nLen){
			// 是否是首次创建文件
			if(0 == binfileData.nLen){               
				// 将信息写入文件
				//SetCoinAddboxAInfo(m_CoinAddBoxAInfo);
				//SetCoinAddboxBInfo(m_CoinAddBoxBInfo);
				
				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				// 初始化硬币专用找零箱AID
				int2BCD(bLineCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bLineCode, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bLineCode));
				m_CoinHopperBoxAInfo.coinBoxID.bType = 0x09;
				int2BCD(bStationCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bNumInQu, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bNumInQu));
				bMachineCode = 2*theMAINTENANCE_INFO.GetMachineCode()-1;
				int2BCD(bMachineCode, (char*)&m_CoinHopperBoxAInfo.coinBoxID.bNum, sizeof(m_CoinHopperBoxAInfo.coinBoxID.bNum));
				m_CoinHopperBoxAInfo.ulCurCount = 0;
				SetCoinHopperboxAInfo(m_CoinHopperBoxAInfo);

				// 初始化硬币专用找零箱BID
				int2BCD(bLineCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bLineCode, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bLineCode));
				m_CoinHopperBoxBInfo.coinBoxID.bType = 0x09;
				int2BCD(bStationCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bNumInQu, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bNumInQu));
				bMachineCode = 2*theMAINTENANCE_INFO.GetMachineCode();
				int2BCD(bMachineCode, (char*)&m_CoinHopperBoxBInfo.coinBoxID.bNum, sizeof(m_CoinHopperBoxBInfo.coinBoxID.bNum));
				m_CoinHopperBoxBInfo.ulCurCount = 0;
				SetCoinHopperboxBInfo(m_CoinHopperBoxBInfo);

				// 回收箱编号读RFID
				SetCollectionCoinboxAInfo(m_CoinCollectionBoxAInfo);
				SetCollectionCoinboxBInfo(m_CoinCollectionBoxBInfo);

				SetCoinTemporary1Info(m_CoinTemporary1Info);
				SetCoinTemporary2Info(m_CoinTemporary2Info);
				// 找零箱编号读RFID
				SetChangeCoinboxAInfo(m_CoinChangeBoxAInfo);
				SetChangeCoinboxBInfo(m_CoinChangeBoxBInfo);
			}
			// 文件长度非法
			else{
				throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// 文件正常
		else{
			
			memcpy(m_fileData, binfileData.lpData, COIN_COUNT_INFO_FILE_LEN);
			// 解析文件
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
@brief      设置硬币补充箱A的计数信息

@param     (i)COIN_ADD_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CTCoinCountInfo::SetCoinAddboxAInfo(COIN_ADD_BOX_INFO& boxInfo)
//{
//	try{
//		boxInfo.CheckMinusValue();
//		boxInfo.Serialize((LPBYTE)m_fileData);
//		// 更新并保存文件
//		UpdateMasterFile();
//		m_CoinAddBoxAInfo = boxInfo;
//		return TRUE;
//	}
//	// 异常捕获
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取硬币补充箱A的计数信息

@param		none

@retval     COIN_ADD_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//CTCoinCountInfo::COIN_ADD_BOX_INFO& CTCoinCountInfo::GetCoinAddboxAInfo()
//{
//	 return m_CoinAddBoxAInfo;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币补充箱B的计数信息

@param     (i)COIN_ADD_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CTCoinCountInfo::SetCoinAddboxBInfo(COIN_ADD_BOX_INFO& boxInfo)
//{
//	try{
//		boxInfo.CheckMinusValue();
//		boxInfo.Serialize((LPBYTE)m_fileData+16);
//		// 更新并保存文件
//		UpdateMasterFile();
//		m_CoinAddBoxBInfo = boxInfo;
//		return TRUE;
//	}
//	// 异常捕获
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取硬币补充箱B的计数信息

@param      none

@retval     COIN_ADD_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//CTCoinCountInfo::COIN_ADD_BOX_INFO& CTCoinCountInfo::GetCoinAddboxBInfo()
//{
//	 return m_CoinAddBoxBInfo;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币HOPPER箱A的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinHopperboxAInfo(COIN_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCurCount==0){
			// 回收操作
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// 其他操作
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinHopperBoxAInfo = boxInfo;
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
@brief      读取硬币HOPPER箱A的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_BOX_INFO& CTCoinCountInfo::GetCoinHopperboxAInfo()
{
	 return m_CoinHopperBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币HOPPER箱B的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinHopperboxBInfo(COIN_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+16);
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinHopperBoxBInfo = boxInfo;
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
@brief      读取硬币HOPPER箱B的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_BOX_INFO& CTCoinCountInfo::GetCoinHopperboxBInfo()
{
	 return m_CoinHopperBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币回收箱A的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCollectionCoinboxAInfo(COIN_COLLECTION_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+32);
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinCollectionBoxAInfo = boxInfo;
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
@brief      读取硬币回收箱A的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& CTCoinCountInfo::GetCollectionCoinboxAInfo()
{
	 return m_CoinCollectionBoxAInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币回收箱B的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCollectionCoinboxBInfo(COIN_COLLECTION_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+48);
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinCollectionBoxBInfo = boxInfo;
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
@brief      读取硬币回收箱B的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& CTCoinCountInfo::GetCollectionCoinboxBInfo()
{
	 return m_CoinCollectionBoxBInfo;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币回收箱A的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetChangeCoinboxAInfo(COIN_CHANGE_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+96);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCount==0){
			// 回收操作
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// 其他操作
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinChangeBoxAInfo = boxInfo;
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
@brief      读取硬币回收箱A的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_CHANGE_BOX_INFO& CTCoinCountInfo::GetChangeCoinboxAInfo()
{
	return m_CoinChangeBoxAInfo;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币找零箱B的计数信息

@param     (i)COIN_BOX_INFO& boxInfo  硬币箱计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetChangeCoinboxBInfo(COIN_CHANGE_BOX_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+112);
		if (boxInfo.coinBoxID.ToString()!=_T("FFFFFFFF")&&boxInfo.ulCount==0){
			// 回收操作
			theTVM_SETTING.SetCoinBoxLastAction(1);
		}else{
			// 其他操作
			theTVM_SETTING.SetCoinBoxLastAction(0);
		}
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinChangeBoxBInfo = boxInfo;
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
@brief      读取硬币回收箱B的计数信息

@param      none

@retval     COIN_BOX_INFO& 硬币箱的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_CHANGE_BOX_INFO& CTCoinCountInfo::GetChangeCoinboxBInfo()
{
	return m_CoinChangeBoxBInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币暂存器1的计数信息

@param     (i)COIN_TEMPORARY1_INFO& boxInfo  硬币暂存器1计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinTemporary1Info(COIN_TEMPORARY1_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+64);
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinTemporary1Info = boxInfo;
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
@brief      读取硬币暂存器1的计数信息

@param      none

@retval     COIN_TEMPORARY1_INFO& 硬币暂存器1的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_TEMPORARY1_INFO& CTCoinCountInfo::GetCoinTemporary1Info()
{
	return m_CoinTemporary1Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币暂存器2的计数信息

@param     (i)COIN_TEMPORARY2_INFO& boxInfo  硬币暂存器1计数信息

@retval     BOOL TRUE: 成功  FALSE: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTCoinCountInfo::SetCoinTemporary2Info(COIN_TEMPORARY2_INFO& boxInfo)
{
	try{
		boxInfo.CheckMinusValue();
		boxInfo.Serialize((LPBYTE)m_fileData+80);
		// 更新并保存文件
		UpdateMasterFile();
		m_CoinTemporary2Info = boxInfo;
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
@brief      读取硬币暂存器2的计数信息

@param      none

@retval     COIN_TEMPORARY2_INFO& 硬币暂存器2的计数信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCoinCountInfo::COIN_TEMPORARY2_INFO& CTCoinCountInfo::GetCoinTemporary2Info()
{
	return m_CoinTemporary2Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新文件并保存

@param      void

@retval     void

@exception  CParamException
*/
//////////////////////////////////////////////////////////////////////////
void CTCoinCountInfo::UpdateMasterFile()
{
    try {
        // 开启文件锁
        m_lock.Lock();
		// 写文件
        if(m_file->WriteBinFile(&m_fileData[0], COIN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
void CTCoinCountInfo::UpdateMasterFileNoCopy()
{
	try {
		// 开启文件锁
		m_lock.Lock();
		if(m_file->WriteBinFile(&m_fileData[0], COIN_COUNT_INFO_FILE_LEN) != CXFile::FILE_OPERATION_OK,false) {
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
/*
@brief      同步硬计数与软计数

@param      无

@retval     无

@exception  无
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
		// 回收箱A的数量为：
		if(nColl1 >= 0){
			m_CoinCollectionBoxAInfo.ulCount_1yuan = nColl1;
		}
		// 当前回收箱A五角数量
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
