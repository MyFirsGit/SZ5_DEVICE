#include "StdAfx.h"
#include "CTLOGRegister.h"


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
CCTLOGRegister::CCTLOGRegister(void)
{
	m_file = NULL;
	m_errorKindCnt = 0;
	memset(m_maintCntTotal, 0x00000000, 256);
	memset(m_fileData, 0, REGISTER_LEN);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTLOGRegister::~CCTLOGRegister(void)
{
	try{
		delete m_file;
		m_file = NULL;
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

@retval     bool        \n
true:读文件成功并且文件存在     false:读取文件成功但文件为空

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ReadFile()
{
	bool ret = false;

	try {
		// 取得两重化的文件
		CString strFileName = m_dataPath + DEVICE_MASTER_DIR + REGISTER_INFO_FILE_NAME;
		CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR +  REGISTER_INFO_FILE_NAME;

		if (NULL == m_file) {
			m_file = new CDXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;
		do {
			// 读取寄存器文件失败
			if (m_file->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}

			// 文件长度检查
			if ((binfileData.nLen < REGISTER_LEN)) {
				if (0 == binfileData.nLen) {
					memset(&m_maintCntTotal,0,sizeof(m_maintCntTotal));
					memset(&m_errorInfo,0,sizeof(m_errorInfo));
					ret = false;
					break;
				}
				else{
					throw CParamException(CParamException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
				}
			}

			memcpy(m_fileData, binfileData.lpData, REGISTER_LEN);

			// 解析存器文件
			ULONG size = 0;
			LPBYTE lpData = binfileData.lpData;

			// record start date and time
			m_registerTime.wYear = BCD2int((char*)lpData,2);
			lpData +=2;
			m_registerTime.biMonth  = BCD2int((char*)lpData++,1);
			m_registerTime.biDay    = BCD2int((char*)lpData++,1);
			m_registerTime.biHour   = BCD2int((char*)lpData++,1);
			m_registerTime.biMinute = BCD2int((char*)lpData++,1);
			m_registerTime.biSecond = BCD2int((char*)lpData++,1);           

			// 维护数据

			// 0x00-0x1D预留
			lpData += REGISTER_BLOCK_SIZE * MAGAZINE_ONE_CNT;
			// 票箱1的售票张数
			m_maintCntTotal[MAGAZINE_ONE_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// 票箱2的售票张数
			m_maintCntTotal[MAGAZINE_TWO_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// 废票的张数
			m_maintCntTotal[INVALIDATED_TICKET_CNT] = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
			lpData += 4;
			// 预留
			lpData = lpData + (255 - INVALIDATED_TICKET_CNT) * REGISTER_BLOCK_SIZE;

			// 异常个数
			m_errorKindCnt = *lpData;
			lpData++;

			// 异常
			for(int i=0;i<m_errorKindCnt;i++){
				m_errorInfo[i].EC_code = mstos(*(WORD*)lpData);
				lpData +=2;
				m_errorInfo[i].count = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
				lpData +=4;
			}
			// 判断日期是否合法
			if(0==m_registerTime.wYear){
				ret = false;
			}
			else{
				ret = true;
			}            
		} while(0);

		if(binfileData.lpData != NULL)
		{
			delete[] binfileData.lpData;
			binfileData.lpData = NULL;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
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
bool CCTLOGRegister::Internalize(CString dataPath, CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	bool iret;

	try {
		iret =  ReadFile();
		if(!iret){
			// 取得当前时间
			SYSTEMTIME dtOperationTime;
			::GetLocalTime(&dtOperationTime);

			// 将当前时间转化为BCD
			int time;
			time = dtOperationTime.wYear*10000 + dtOperationTime.wMonth*100 + dtOperationTime.wDay;
			int2BCD(time,&m_fileData[0],4);
			time = dtOperationTime.wHour*10000 + dtOperationTime.wMinute*100 + dtOperationTime.wSecond;
			int2BCD(time,&m_fileData[4],3);

			// 设置当前时间
			if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {

				// 修改内存为当前时间
				memcpy(&m_registerTime.wYear,&dtOperationTime.wYear,sizeof(m_registerTime.wYear));
				memcpy(&m_registerTime.biMonth,&dtOperationTime.wMonth,sizeof(m_registerTime.biMonth));
				memcpy(&m_registerTime.biDay,&dtOperationTime.wDay,sizeof(m_registerTime.biDay));
				memcpy(&m_registerTime.biHour,&dtOperationTime.wHour,sizeof(m_registerTime.biHour));
				memcpy(&m_registerTime.biMinute,&dtOperationTime.wMinute,sizeof(m_registerTime.biMinute));
				memcpy(&m_registerTime.biSecond,&dtOperationTime.wSecond,sizeof(m_registerTime.biSecond));
			}
			else {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      取得记录时间

@param      (o) _DATE_TIME&  timeData    记录时间

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::GetRecordTime(_DATE_TIME& timeData)
{
	timeData = m_registerTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      设置记录时间

@param      (i) _DATE_TIME   timeData    记录时间

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::SetRecordTime(_DATE_TIME timeData)
{
	try{
		int time;
		time = timeData.wYear*10000 + timeData.biMonth*100 + timeData.biDay;
		int2BCD(time,&m_fileData[0],4);
		time = timeData.biHour*10000 + timeData.biMinute*100 + timeData.biSecond;
		int2BCD(time,&m_fileData[4],3);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {
			m_registerTime = timeData;
			return true;
		}
		else {
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
@brief      得到维护数据的计数

@param      (i) MAINTAIN_CONDITION   conditionId   数据存储的位置
@param      (o) MAINTAIN_MODULE& module            维护数据

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
ULONG CCTLOGRegister::GetMaintDataCount(MAINTAIN_CONDITION conditionId,MAINTAIN_MODULE& module)
{
	switch(conditionId){
		case MAGAZINE_ONE_CNT:
		case MAGAZINE_TWO_CNT:
		case INVALIDATED_TICKET_CNT:
			module = MAINTAIN_TH;
			break;
		default:
			module = MAINTAIN_UNUSED;
			break;
	}
	return m_maintCntTotal[conditionId];

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新维护数据的计数

@param      (i) MAINTAIN_CONDITION   conditionId   要更新的维护数据的位置
@param      (i) ULONG Totalcount  countTotal    数据计数

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::UpdateMaintDataCount(MAINTAIN_CONDITION conditionId, ULONG countTotal)
{
	try{
		// 设置更新维护数据
		m_maintCntTotal[conditionId] += countTotal;
		countTotal = ltoml(m_maintCntTotal[conditionId]);
		// 放入m_fileData[]
		memcpy(&m_fileData[MAINTAIN_POS + conditionId*REGISTER_BLOCK_SIZE], &countTotal, 4);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) != CXFile::FILE_OPERATION_OK) {

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
@brief      得到异常信息(上位专用)

@param      (o)LPBYTE lpError

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::GetErrDataCount(LPBYTE lpError)
{
	memcpy(lpError,&m_fileData[ERROR_INFO_POS],ERROR_INFO_SIZE);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新异常总数

@param      (i) WORD char    ECcode     错误代码

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTLOGRegister::UpdateErrDataCount(WORD ECcode)
{
	try{
		// 根据EC code找相应错误类型
		int pos = 0;
		for(pos = 0; pos< m_errorKindCnt; pos++){
			// 如果找到匹配的错误代码
			if(m_errorInfo[pos].EC_code==ECcode){
				break;
			}
		}

		// 如果pos == m_errorKindCnt 则该种错误未还master中记录
		if(pos == m_errorKindCnt){
			// 错误种类＋1
			m_errorKindCnt++;
			m_fileData[ERROR_KINDS_POS] = m_errorKindCnt;

			// 得到EC code
			m_errorInfo[pos].EC_code = ECcode;
			ECcode = mstos(ECcode);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE],&ECcode,2);

			// 新加一个种类 并且count = 1            
			m_errorInfo[pos].count = 1;     
			ULONG errorCnt = ltoml(m_errorInfo[pos].count);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE+2],&errorCnt,REGISTER_BLOCK_SIZE);
		}
		// 如果pos < m_errorKindCnt，则改种错误master已经记载
		else{
			// 该种错误数++
			m_errorInfo[pos].count ++;
			ULONG errorCnt = ltoml(m_errorInfo[pos].count);
			memcpy(&m_fileData[ERROR_INFO_POS+pos*ERROR_BLOCK_SIZE+2],&errorCnt,REGISTER_BLOCK_SIZE);    
		}

		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) != CXFile::FILE_OPERATION_OK) {
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
@brief      覆盖文件

@param      (i)const char*  lpData      要覆盖入文件的数据
@param      (i)ULONG        nLen        要覆盖入文件的数据长度

@retval     bool        \n
true:覆盖文件成功         false:覆盖文件失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::OverWriteFile(const char* lpData, ULONG nLen)
{
	try{
		bool ret = false;

		if (m_file->WriteBinFile(lpData,nLen) == CXFile::FILE_OPERATION_OK) {
			ret = true;
		}
		else{
			throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
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
@brief      清除指定的维护数据

@param      (i)MAINTAIN_CONDITION condition    位置

@retval     bool        \n
true:清除成功         false:清除失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ClearMaintData(MAINTAIN_CONDITION condition)
{
	try{
		memset(&m_fileData[7+condition*REGISTER_BLOCK_SIZE],0x00,4);
		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {
			m_maintCntTotal[condition] = 0;
			return true;
		}
		else {
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
@brief      清除所有的维护数据

@param      void

@retval     bool        \n
true:清除成功         false:清除失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTLOGRegister::ClearAllMaintData()
{
	try{
		// 每一项维护数据清0
		memset(&m_fileData[7],0x00,1024);

		// 取得当前时间
		SYSTEMTIME dtOperationTime;
		::GetLocalTime(&dtOperationTime);

		// 将当前时间转化为BCD
		int time;
		time = dtOperationTime.wYear*10000 + dtOperationTime.wMonth*100 + dtOperationTime.wDay;
		int2BCD(time,&m_fileData[0],4);
		time = dtOperationTime.wHour*10000 + dtOperationTime.wMinute*100 + dtOperationTime.wSecond;
		int2BCD(time,&m_fileData[4],3);

		if(m_file->WriteBinFile(&m_fileData[0], REGISTER_LEN) == CXFile::FILE_OPERATION_OK) {

			// 将m_maintCntTotal设为0
			for(int i = 0;i<256;i++){
				m_maintCntTotal[i] = 0;
			}

			// 修改内存为当前时间
			memcpy(&m_registerTime.wYear,&dtOperationTime.wYear,sizeof(m_registerTime.wYear));
			memcpy(&m_registerTime.biMonth,&dtOperationTime.wMonth,sizeof(m_registerTime.biMonth));
			memcpy(&m_registerTime.biDay,&dtOperationTime.wDay,sizeof(m_registerTime.biDay));
			memcpy(&m_registerTime.biHour,&dtOperationTime.wHour,sizeof(m_registerTime.biHour));
			memcpy(&m_registerTime.biMinute,&dtOperationTime.wMinute,sizeof(m_registerTime.biMinute));
			memcpy(&m_registerTime.biSecond,&dtOperationTime.wSecond,sizeof(m_registerTime.biSecond));
			return true;
		}
		else {
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
CCTLOGRegister CCTLOGRegister::theInstance;