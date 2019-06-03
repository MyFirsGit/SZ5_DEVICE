#include "StdAfx.h"
#include "CPwdInputError.h"
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
CPwdInputError::CPwdInputError(void)
{
	m_StuffIDFile = NULL;
	m_stuffErrList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CPwdInputError::~CPwdInputError(void)
{
	try{
		delete m_StuffIDFile;
		m_StuffIDFile = NULL;
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
@brief      读取文件

@param      无

@retval     boole

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CPwdInputError::ReadFile()
{
	// 取得两重化的文件
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + PSD_ERROR_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + PSD_ERROR_INFO_FILE_NAME;
	try{
		if (NULL == m_StuffIDFile) {
			m_StuffIDFile = new CDXFile(strFileName, strBackupFilename);
		}
		VARIABLE_DATA binfileData;
		//读取文件
		if (m_StuffIDFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		//检查文件长度(FILE_DATE_SIZE + FILE_BASE_SIZE * N (>=0))
		if ((binfileData.nLen >FILE_DATE_SIZE) &&  (binfileData.nLen - FILE_DATE_SIZE ) %FILE_BASE_SIZE!=0) {
			throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
		}

		//解析文件信息
		LPBYTE lpData = binfileData.lpData;

		if(binfileData.nLen >= FILE_DATE_SIZE){
			// 解析密码错误文件中的日期
			m_curDate.wYear = BCD2int((char*)lpData,2);     // 年
			lpData += 2;
			m_curDate.biMonth = BCD2int((char*)lpData++,1); // 月
			m_curDate.biDay = BCD2int((char*)lpData++,1);   // 日

			// 取出当前营业日
			_DATE curDate;
			curDate = ComGetBusiDate();
			m_stuffErrList.clear();
			if( 0 != CmpTwoDate(curDate,m_curDate)){
				// 删除master文件内容
				if(CXFile::FILE_OPERATION_OK != m_StuffIDFile->ClearFile()){
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
				}
				// 将当前日期存入master
				SetFileDate();
			}
			else{
				STAFF_ERR_INFO errInfo;
				// 数据存储的员工数为文件长度的1/5
				int stuffCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<stuffCnt;i++){
					// 员工ID
					errInfo.stuffID = BCD2int((char*)lpData,4);
					lpData += 4;
					// 错误次数
					errInfo.errCnt = *lpData;
					lpData++;
					m_stuffErrList.push_back(errInfo);
				}
			}
		}
		else{
			// 将当前日期存入master
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
@brief      判断员工信息是否存在

@param      ULONG stuffID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CPwdInputError::IsStuffIDExist(ULONG stuffID)
{
	bool ret = false;
	// 看master是否已经 添加了该员工
	for(int i = 0;i<(int)m_stuffErrList.size();i++){
		if(m_stuffErrList[i].stuffID==stuffID){
			ret = true;
			break;
		}
	}
	return ret;    
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      添加员工信息

@param      ULONG stuffID 

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::AddPWDErrorStuffID(ULONG stuffID)
{
	try{
		// 只有员工不存在才添加信息
		if(!IsStuffIDExist(stuffID)){

			// 将员工信息存入vector
			STAFF_ERR_INFO stuffInfo;
			stuffInfo.stuffID = stuffID;
			m_stuffErrList.push_back(stuffInfo);

			// 得到实际存在的员工数
			int len = (int)m_stuffErrList.size();

			// 将vector内信息放入master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData = new BYTE[FILE_DATE_SIZE + len*FILE_BASE_SIZE ];
			vdWriteData.nLen = FILE_DATE_SIZE + len*FILE_BASE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// 取出营运日期
			m_curDate = ComGetBusiDate();

			// 将日期转为int型
			int dateInt;
			dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

			// 转为BCD型
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// 放员工ID
				int2BCD(m_stuffErrList[i].stuffID, (char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE], 4);
				// 错误次数
				memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE +4], &m_stuffErrList[i].errCnt,1);     
			}

			// 写信息
			if(m_StuffIDFile->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK) {
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
bool CPwdInputError::Internalize(CString dataPath, CString backupPath)
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
@brief      清除所有员工信息

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::ClearStuffInfo()
{

	try {
		// 将所有的内存变量设为默认值
		m_stuffErrList.clear();

		// 清空文件
		m_StuffIDFile->ClearFile();
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
@brief      清除指定员工信息

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::ClearTheStaffNoInfo(ULONG stuffID)
{
	try{
		// 只有员工存在才删除
		if(IsStuffIDExist(stuffID)){

			vector<STAFF_ERR_INFO>::iterator verIter;

			for (verIter=m_stuffErrList.begin(); verIter!=m_stuffErrList.end(); verIter++)
			{
				if ((*verIter).stuffID == stuffID)
				{
					m_stuffErrList.erase(verIter);
					break;
				}
			}

			// 得到实际存在的员工数
			int len = (int)m_stuffErrList.size();

			// 将vector内信息放入master
			VARIABLE_DATA vdWriteData;
			vdWriteData.lpData= new BYTE[len*FILE_BASE_SIZE + FILE_DATE_SIZE];
			vdWriteData.nLen = len*FILE_BASE_SIZE + FILE_DATE_SIZE;
			BYTE* writeData = vdWriteData.lpData;

			// 取出营运日期
			m_curDate = ComGetBusiDate();

			// 将日期转为int型
			int dateInt;
			dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

			// 转为BCD型
			char fileDate[4] = {0};
			int2BCD(dateInt,&fileDate[0],4);

			memcpy(writeData,fileDate,4);

			int i = 0;
			for(i = 0; i < len; i++){
				// 放员工ID
				int2BCD(m_stuffErrList[i].stuffID, (char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE], 4);
				// 错误次数
				memcpy(&writeData[i*FILE_BASE_SIZE + 4 + FILE_DATE_SIZE], &m_stuffErrList[i].errCnt,1);     
			}

			// 写信息
			if(m_StuffIDFile->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK) {
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
@brief      设置错误日期

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPwdInputError::SetFileDate()
{
	try{
		// 取出营运日期
		m_curDate = ComGetBusiDate();

		// 将日期转为int型
		int dateInt;
		dateInt = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;

		// 转为BCD型
		char fileDate[4] = {0};
		int2BCD(dateInt,&fileDate[0],4);

		if(m_StuffIDFile->WriteBinFile(fileDate,4) != CXFile::FILE_OPERATION_OK) {
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

CPwdInputError CPwdInputError::theInstance;