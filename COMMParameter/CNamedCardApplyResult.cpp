#include "StdAfx.h"
#include "CNamedCardApplyResult.h"
#include <io.h>

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
CNamedCardApplyResult::CNamedCardApplyResult(void)
{
	m_File = NULL;
	m_ResultList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CNamedCardApplyResult::~CNamedCardApplyResult(void)
{
	try{
		delete m_File;
		m_File = NULL;
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
@brief      读取文件

@param      无

@retval     bool

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::ReadFile()
{
	// 取得两重化的文件
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + NAMEDCARD_APPLY_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + NAMEDCARD_APPLY_INFO_FILE_NAME;
	try{
		if (NULL == m_File) {
			m_File = new CDXFile(strFileName, strBackupFilename);
		}
		VARIABLE_DATA binfileData;
		// 读取文件
		if (m_File->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// 检查文件长度(FILE_DATE_SIZE + FILE_BASE_SIZE * N (>=0))
		if((binfileData.nLen >FILE_DATE_SIZE) &&  (binfileData.nLen - FILE_DATE_SIZE ) % FILE_BASE_SIZE != 0) {
			throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
		}

		// 解析文件信息
		LPBYTE lpData = binfileData.lpData;
		// 文件中存在记录
		if(binfileData.nLen >= FILE_DATE_SIZE){
			// 清空内存
			m_ResultList.clear();

			// 存储日期
			m_saveDate.wYear = BCD2int((char*)lpData,2);     // 年
			lpData += 2;
			m_saveDate.biMonth = BCD2int((char*)lpData++,1); // 月
			m_saveDate.biDay = BCD2int((char*)lpData++,1);   // 日

			// 取出当前营业日
			_DATE curDate = ComGetBusiDate();
			
			// 取得存储日期+7天的日期
			_DATE dateAfter7days = GetDayAfter(m_saveDate,7);

			// 当前营业日 > 存储日期+7天的日期 
			if(1 == CmpTwoDate(curDate,dateAfter7days)){
				// 删除master文件内容
				if(CXFile::FILE_OPERATION_OK != m_File->ClearFile()){
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
				}
				// 将当前日期存入master
				SetFileDate();
			}
			else{
				NAMED_CARD_APP_RESULT recordInfo;
				// 取得记名卡申请记录数
				int recordCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<recordCnt;i++){
					// 证件信息
					memcpy(recordInfo.CertificatesNum,lpData,sizeof(recordInfo.CertificatesNum));
					lpData += 21;
					// 审核结果
					recordInfo.Status = *lpData;
					lpData++;
					m_ResultList.push_back(recordInfo);
				}
			}
		}
		// 文件为空
		else{
			// 将当前日期存入文件
			SetFileDate();
		}

		if(binfileData.lpData != NULL) {
			delete[] binfileData.lpData;
			binfileData.lpData = NULL;
		}
		return true;
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
@brief      判断记名卡申请记录是否存在

@param      BYTE* CertificatesNum  收据编号

@retval     bool true:存在   false:不存在

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::IsRecordExist(BYTE* CertificatesNum)
{
	if(CertificatesNum == NULL){
		throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	bool ret = false;
	// 判断记名卡申请记录是否存在
	for(int i = 0;i<(int)m_ResultList.size();i++){
		if(memcmp(m_ResultList[i].CertificatesNum,CertificatesNum,sizeof(m_ResultList[i].CertificatesNum)) == 0){
			ret = true;
			break;
		}
	}
	return ret;    
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加记名卡申请记录

@param      BYTE* CertificatesNum   收据编号 
@param      BYTE status        

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::AddNamedCardApplyRecord(BYTE* CertificatesNum,BYTE status)
{
	try{
		if(CertificatesNum == NULL){
			throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
		}
		// 只有记名卡申请信息不存在才添加信息
		if(!IsRecordExist(CertificatesNum)){

			// 将记名卡申请信息存入vector
			NAMED_CARD_APP_RESULT recordInfo;
			memcpy(recordInfo.CertificatesNum,CertificatesNum,sizeof(recordInfo.CertificatesNum));
			recordInfo.Status = status;
			m_ResultList.push_back(recordInfo);

			// 得到实际存在的记录数
			int len = (int)m_ResultList.size();

			// 将vector内信息放入master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData = new BYTE[FILE_DATE_SIZE + len*FILE_BASE_SIZE ];
			vdWriteData.nLen = FILE_DATE_SIZE + len*FILE_BASE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// 取出营运日期
			//m_saveDate = ComGetBusiDate();

			// 将日期转为int型
			int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

			// 转为BCD型
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// 证件信息			
				memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],m_ResultList[i].CertificatesNum,sizeof(m_ResultList[i].CertificatesNum));
				// 审核状态
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 21], &m_ResultList[i].Status,1);     
			}

			// 写信息
			if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
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
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      初期处理

@param      (i)CString dataPath         数据主目录
@param      (i)CString backupPath       备份主目录

@retval     bool \n

true: 成功      false: 失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::Internalize(CString dataPath, CString backupPath)
{

	m_dataPath = dataPath;
	m_backupPath = backupPath;

	try {
		return ReadFile();
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
@brief      清除记名卡申请记录

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::ClearAllRecord()
{
	try {
		// 将所有的内存变量设为默认值
		m_ResultList.clear();

		// 清空文件
		m_File->ClearFile();
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
@brief      更新指定记名卡申请的审核记录(若记录不存在则添加)

@param      (i)BYTE* CertificatesNum   证件类型 + 证件编码(1Byte + 20Byte)
@param      (i)BYTE result             审核结果

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::UpdateTheApplyResult(BYTE* CertificatesNum,BYTE result)
{
	try{		
		m_lock.Lock();
		if(CertificatesNum == NULL){
			throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
		}
		// 只有记名卡申请记录存在才更新
		if(IsRecordExist(CertificatesNum)){
			vector<NAMED_CARD_APP_RESULT>::iterator verIter;
			for(verIter=m_ResultList.begin(); verIter!=m_ResultList.end(); verIter++){
				if(memcmp(verIter->CertificatesNum,CertificatesNum,sizeof(verIter->CertificatesNum)) == 0){
					verIter->Status = result;
					break;
				}
			}

			// 得到记名卡申请记录数
			int len = (int)m_ResultList.size();

			// 将vector内信息放入master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData= new BYTE[len*FILE_BASE_SIZE + FILE_DATE_SIZE];
			vdWriteData.nLen = len*FILE_BASE_SIZE + FILE_DATE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// 取出营运日期
			//m_curDate = ComGetBusiDate();

			// 将日期转为int型
			int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

			// 转为BCD型
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// 证件信息		
				memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],m_ResultList[i].CertificatesNum,sizeof(m_ResultList[i].CertificatesNum));
				// 审核状态
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 21], &m_ResultList[i].Status,1);     
			}

			// 写信息
			if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
		}
		// 不存在
		else{
			// 添加记名卡申请记录
			AddNamedCardApplyRecord(CertificatesNum,result);                               
		}
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
@brief      获取指定记名卡申请的审核结果

@param      (i)BYTE* CertificatesNum   证件类型 + 证件编码(1Byte + 20Byte)
@param      (o)BYTE result             审核结果

@retval     bool true:存在   false:不存在

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CNamedCardApplyResult::GetTheApplyResult(BYTE* CertificatesNum,BYTE& result)
{
	if(CertificatesNum == NULL){
		throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	bool ret = false;
	// 判断记名卡申请记录是否存在
	for(int i = 0;i<(int)m_ResultList.size();i++){
		if(memcmp(m_ResultList[i].CertificatesNum,CertificatesNum,sizeof(m_ResultList[i].CertificatesNum)) == 0){
			result = m_ResultList[i].Status;
			ret = true;
			break;
		}
	}
	return ret;   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写入文件存储日期

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNamedCardApplyResult::SetFileDate()
{
	try{
		// 取出营运日期
		m_saveDate = ComGetBusiDate();

		// 将日期转为int型
		int dateInt = m_saveDate.wYear*10000 + m_saveDate.biMonth*100 + m_saveDate.biDay;

		// 转为BCD型
		char fileDate[4] = {0};
		int2BCD(dateInt,&fileDate[0],4);

		if(m_File->WriteBinFile(fileDate,4) != CXFile::FILE_OPERATION_OK,false) {
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

CNamedCardApplyResult CNamedCardApplyResult::theInstance;