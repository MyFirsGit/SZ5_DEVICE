#include "StdAfx.h"
#include "CRefundApplyResult.h"
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
CRefundApplyResult::CRefundApplyResult(void)
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
CRefundApplyResult::~CRefundApplyResult(void)
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
bool CRefundApplyResult::ReadFile()
{
	// 取得两重化的文件
	CString strFileName = m_dataPath + DEVICE_MASTER_DIR + REFUND_APPLY_INFO_FILE_NAME;
	CString strBackupFilename = m_backupPath + DEVICE_MASTER_DIR + REFUND_APPLY_INFO_FILE_NAME;
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
				REFUND_APP_RESULT recordInfo;
				// 取得退款申请记录数
				int recordCnt = (binfileData.nLen - FILE_DATE_SIZE ) /FILE_BASE_SIZE;
				for(int i = 0;i<recordCnt;i++){
					//证件类型
					memcpy(&recordInfo.certificateType,lpData,sizeof(recordInfo.certificateType));
					lpData+=sizeof(recordInfo.certificateType);
					//证件编号
					memcpy(recordInfo.certificateID,lpData,sizeof(recordInfo.certificateID));
					lpData+=sizeof(recordInfo.certificateID);
					//票卡应用号
					memcpy(recordInfo.ticketApplicationSerialNo,lpData,sizeof(recordInfo.ticketApplicationSerialNo));
					lpData+=sizeof(recordInfo.ticketApplicationSerialNo);
					// 收据编号
					memcpy(recordInfo.ReceiptNum,lpData,sizeof(recordInfo.ReceiptNum));
					lpData += 10;
					// 退押金金额
					recordInfo.Deposit = MAKELONG(MAKEWORD(*lpData, *(lpData + 1)),MAKEWORD(*(lpData + 2),*(lpData+3)));
					lpData += 4;
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

void CRefundApplyResult::SaveToFile()
{
	try
	{
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
			//证件类型
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE],&m_ResultList[i].certificateType,sizeof(m_ResultList[i].certificateType));
			//证件编号
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+4],m_ResultList[i].certificateID,sizeof(m_ResultList[i].certificateID));
			//票卡应用号
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+24],m_ResultList[i].ticketApplicationSerialNo,sizeof(m_ResultList[i].ticketApplicationSerialNo));
			// 收据编号				
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE+32],m_ResultList[i].ReceiptNum,sizeof(m_ResultList[i].ReceiptNum));
			// 卡内余额
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 42],&m_ResultList[i].Balance,sizeof(m_ResultList[i].Balance));
			// 退押金金额
			memcpy((char*)&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 46],&m_ResultList[i].Deposit,sizeof(m_ResultList[i].Deposit));
			// 审核状态
			memcpy(&writeData[i*FILE_BASE_SIZE + FILE_DATE_SIZE + 50], &m_ResultList[i].Status,1);     
		}

		// 写信息
		if(m_File->WriteBinFile((char*)writeData, i*FILE_BASE_SIZE + FILE_DATE_SIZE) != CXFile::FILE_OPERATION_OK,false) {
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
bool CRefundApplyResult::Internalize(CString dataPath, CString backupPath)
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
@brief      清除退款申请记录

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::ClearAllRecord()
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
@brief      更新指定退款申请的审核记录(若记录不存在则添加)

@param      (i)BYTE* receiptNum 收据编号
@param      (i)long balance 卡内余额
@param      (i)long deposit 退押金金额
@param      (i)BYTE result      审核结果

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::UpdateTheApplyResult(REFUND_APP_RESULT& record)
{
	try{
		m_lock.Lock();
		// 只有退款申请记录存在才更新
		int position = -1;
		for(int i=0;i<m_ResultList.size();i++)
		{
			REFUND_APP_RESULT& iterResult = m_ResultList[i];
			//票卡号一样
			if(memcmp(iterResult.ticketApplicationSerialNo,record.ticketApplicationSerialNo,sizeof(record.ticketApplicationSerialNo)) == 0)
			{
				position = i;
				break;
			}
			//证件号一样
			if(iterResult.certificateType == record.certificateType && memcmp(iterResult.certificateID,record.certificateID,sizeof(record.certificateID)) == 0)
			{
				position = i;
				break;
			}
		}

		if(position>=0)
		{
			m_ResultList[position] = record;
		}
		else
		{
			m_ResultList.push_back(record);
		}
		SaveToFile();
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
@brief      获取指定退款申请的审核结果

@param      (i)BYTE* receiptNum   收据编号
@param      (o)BYTE result        审核结果

@retval     bool true:存在   false:不存在

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundApplyResult::GetTheApplyResult(REFUND_APP_RESULT& refund_app_result)
{
	int position = -1;
	for(int i=0;i<m_ResultList.size();i++)
	{
		REFUND_APP_RESULT& iterResult = m_ResultList[i];
		//票卡号一样
		if(memcmp(iterResult.ticketApplicationSerialNo,refund_app_result.ticketApplicationSerialNo,sizeof(refund_app_result.ticketApplicationSerialNo)) == 0)
		{
			position = i;
			break;
		}
		//证件号一样
		if(iterResult.certificateType == refund_app_result.certificateType && memcmp(iterResult.certificateID,refund_app_result.certificateID,sizeof(refund_app_result.certificateID)) == 0)
		{
			position = i;
			break;
		}
	}

	if(position>=0)
	{
		refund_app_result = m_ResultList[position];
	}
	return position>=0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      写入文件存储日期

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyResult::SetFileDate()
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

CRefundApplyResult CRefundApplyResult::theInstance;