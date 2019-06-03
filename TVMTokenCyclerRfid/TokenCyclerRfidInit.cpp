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
CTCRfidInit::CTCRfidInit()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidInit::~CTCRfidInit()
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
long CTCRfidInit::ExecuteCommand()
{
	long lRet = 0;
	lRet = InitFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		此函数发送 Request 命令 检查在有效范围内是否有卡的存在

 @param		mode:请求类型 
			mode=0:请求天线范围内 IDLE 状态的卡 HALT 状态的除外 
			mode=1: 请求天线范围内的所有卡 
			tagtype:返回的目标类型

 @retval	unsigned char 返回值 0 表示成功 否则返回错误码 

 @exception	无

*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidInit::InitFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("INIT"));

	sLogString.Format(_T("CTCRfidInit: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	long errCode = E_NO_CS_ERROR;
	// 1. Request
	sLogString.Format(_T("zlg500B_request: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char mode = 1;//0:请求天线范围内 IDLE 状态的卡 HALT 状态的除外;1: 请求天线范围内的所有卡
	unsigned short tagType = 0;//返回的目标类型
	//检查在有效范围内是否有卡的存在
	errCode = zlg500B_request(mode, tagType);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 2. Anticoll
	sLogString.Format(_T("zlg500B_anticoll: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char bcnt = 0;// 为预选卡所分配的位的个数 通常 Bcnt=0
	unsigned long cardSN = 0;//返回卡的序号
	BYTE rfidData[8] = {0};
	//此函数开始防冲突操作 
	errCode = zlg500B_anticoll(0, cardSN);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 3. Select
	sLogString.Format(_T("zlg500B_select: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char cardSize = 0;// 返回卡的容量
	//此函数选择某一个序号的卡
	errCode = zlg500B_select(cardSN, cardSize);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 4. Authen
	sLogString.Format(_T("zlg500B_authentication: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char pwMode = 0;// 密钥类型,密钥 A:0x00,密钥 B:0x04
	unsigned char secnr = 1;// 要验证的卡扇区号
	//将 RC500 中的密匙与卡中的密匙进行验证 在进行对卡片的读写操作之前 必须成功执行此指令 
	//在系统初始化时 主控设备将把每个扇区的密码加载到读卡器中 
	errCode = zlg500B_authentication(pwMode, secnr);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

INIT_END:
	sLogString.Format(_T("CTCRfidInit: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
