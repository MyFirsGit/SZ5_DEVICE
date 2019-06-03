#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidWrite::CTCRfidWrite(BYTE* rfidData)
{
	memset(m_rfidData, 0, sizeof(m_rfidData));
	memcpy(m_rfidData, rfidData, 8);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidWrite::~CTCRfidWrite()
{

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidWrite::ExecuteCommand()
{
	long lRet = 1;
	lRet = WriteFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		读取状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidWrite::WriteFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("WRITE"));

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("zlg500B_write: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;

	/*
		addr:块号(0x00--0x3f) 
		data:块数据 长度 16 个字节 
		函数功能 此函数功能是对指定的块进行写操作 返回值 0 表示成功 否则返回错误码
	*/
	unsigned char addr = 1;
	errCode = zlg500B_write(addr, m_rfidData);

	sLogString.Format(_T("zlg500B_write: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
