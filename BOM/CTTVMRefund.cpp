#include "StdAfx.h"
#include "CTTVMRefund.h"
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
CCTVMRefund::CCTVMRefund(void)
{
	m_presellFile = NULL;
	m_preTicketNoList.clear();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTVMRefund::~CCTVMRefund(void)
{
	try{
		delete m_presellFile;
		m_presellFile = NULL;
	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}        
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化文件

@param      (i) CString dataPath    数据路径
@param      (i) CString backupPath  备份路径

@retval     bool

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTVMRefund::Internalize(CString dataPath, CString backupPath)
{
	// 取得两重化的文件
	CString strFileName = dataPath + TVM_REFUND_FILE_NAME;
	CString strBackupFilename = backupPath + TVM_REFUND_FILE_NAME;

	try{
		if (NULL == m_presellFile) {
			m_presellFile = new CXFile(strFileName,strBackupFilename);
		}

		VARIABLE_DATA binfileData;

		// 读取预制票文件失败
		if (m_presellFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
			throw CLogException(CLogException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		// 检查文件长度
		if ((binfileData.nLen < MIN_FILE_LENGTH)) {
			if (0 == binfileData.nLen) {
				m_preTicketNoList.clear();
				// 将当前日期存入master
				SetFileDate();
				return true;
			}
			else{
				throw CLogException(CLogException::FILE_LEN_FAIL, _T(__FILE__), __LINE__);
			}
		}

		// 解析预制票信息
		LPBYTE lpData = binfileData.lpData;
		m_preTicketNoList.clear();
		//lpData += 4;
		// 解析预制票文件中的日期
		m_curDate.wYear = BCD2int((char*)lpData,2);     // 年
		lpData += 2;
		m_curDate.biMonth = BCD2int((char*)lpData++,1); // 月
		m_curDate.biDay = BCD2int((char*)lpData++,1);   // 日

		// 取出当前营业日
		_DATE curDate;
		curDate = ComGetBusiDate();

		if( 0 != CmpTwoDate(curDate,m_curDate)){
			m_preTicketNoList.clear();

			// 删除master文件内容
			if(CXFile::FILE_OPERATION_OK != m_presellFile->ClearBinFile()){
				throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}

			// 将当前日期存入master
			SetFileDate();
		}

		// 解析卡号
		else{
			TVM_ERROR_TICKET_ID cardNo;
			for (ULONG i=0;i<(binfileData.nLen-4)/16;i++) {
				char temp[18];
				memset(temp, 0, 18);
				memcpy(temp, lpData, 16);
				//cardNo.csTVMErrorSierialNo.SetString(temp);
				USES_CONVERSION;
				cardNo.csTVMErrorSierialNo= A2T(temp);
				lpData += 16;
				// 将卡号放置vector中
				m_preTicketNoList.push_back(cardNo);
			}
		}

		return true;

	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      追加文件

@param      vector<PRESELL_TICKET_ID>   v_presellIdList

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTVMRefund::AddWriteFile(vector<TVM_ERROR_TICKET_ID> v_presellIdList)
{
	try{

		// 将需要加入master的卡号加到vector中
		for(unsigned long i = 0; i < v_presellIdList.size(); i++){
			m_preTicketNoList.push_back(v_presellIdList[i]);
		}

		// 计算master文件的长度
		int len = 4 + (int)m_preTicketNoList.size()*16;
		char* fileData = new char[len];
		memset(fileData,0,len);

		// 将时间写入
		int time = m_curDate.wYear*10000 + m_curDate.biMonth*100 + m_curDate.biDay;
		int2BCD(time,&fileData[0],4);

		// 将整个vector内容写入文件
		for(int k = 0; k<(int)m_preTicketNoList.size(); k++){
			//memcpy(&fileData[4+4*k],m_preTicketNoList[k].presellId,4);
			//int2BCD(m_preTicketNoList[k].presellId,&fileData[4+4*k],4);
			char temp[18];
			memset(temp, 0, 36);
			char* lptemp = temp;
		//	lptemp = m_preTicketNoList[k].csTVMErrorSierialNo.GetBuffer();
			USES_CONVERSION;
            lptemp = T2A(m_preTicketNoList[k].csTVMErrorSierialNo);
			memcpy(&fileData[4+16*k], lptemp, 16);
		}

		// 写文件
		if (m_presellFile->WriteFileWithoutSUM(fileData,len) != CXFile::FILE_OPERATION_OK) {
			delete []fileData;
			fileData = NULL;
			throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}

		delete []fileData;
		fileData = NULL;
	}

	// 异常捕获
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新日志文件

@param      vector<PRESELL_TICKET_ID>   ticketList

@retval     bool    \n
true:成功   false:失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTVMRefund::UpdateTVMTicketInfo(vector<TVM_ERROR_TICKET_ID> ticketList)
{

	try{
		AddWriteFile(ticketList);
		return true;
	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}  
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否为TVM票

@param      CardSerialNumber_t    ticketNo

@retval     bool        \n
true：  是TVM票    false： 不是TVM票

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CCTVMRefund::CheckIsTVMTicket(CString csticketNo)
{
	for(unsigned long i = 0;i < m_preTicketNoList.size();i++){
		// 比较卡号是否相等
		if(csticketNo == m_preTicketNoList[i].csTVMErrorSierialNo){
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      设置预制票文件的日期

@param      void

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTVMRefund::SetFileDate()
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

		if(m_presellFile->WriteFileWithoutSUM(fileDate,4) != CXFile::FILE_OPERATION_OK) {
			throw CLogException(CLogException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
		}
	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

CCTVMRefund CCTVMRefund::theInstance;