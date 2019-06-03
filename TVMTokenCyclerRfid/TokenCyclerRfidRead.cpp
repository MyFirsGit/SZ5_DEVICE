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

 @param		void

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidRead::CTCRfidRead()
{
	memset(m_rfidData, 0, sizeof(m_rfidData));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidRead::~CTCRfidRead()
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
long CTCRfidRead::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReadFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidRead::ReadFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("READ"));

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("zlg500B_read: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	/*
		addr:块号(0x00--0x3f) 
		data:块数据 长度 16 个字节 
		函数功能 此函数功能是读取卡的一个块的数据 返回值 0 表示成功 否则返回错误码
	*/
	long errCode = E_NO_CS_ERROR;
	unsigned char addr = 1*4 + 1;//读1扇区1块
	errCode = zlg500B_read(addr, m_rfidData);

	sLogString.Format(_T("zlg500B_read: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}


long CTCRfidRead::GetResponse(BYTE* rfidData)
{
	memcpy(rfidData, m_rfidData, 8);
	return 0;
}