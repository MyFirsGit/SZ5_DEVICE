#include "stdafx.h"
#include "CHCommand.h"


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
CCHReadCoinNum::CCHReadCoinNum()
{
	memset(&m_res, 0x00, sizeof(m_res));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHReadCoinNum::~CCHReadCoinNum()
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
long CCHReadCoinNum::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReadCoinNumFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取命令反馈

 @param		LPCH_SYSTEM_STATUS_RSP pRes 指向获取反馈信息的对象

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadCoinNum::GetResponse(tCHACoinValueInfo* pRes)
{
	memcpy(pRes,&m_res, sizeof(m_res));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		关闭串口

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadCoinNum::ReadCoinNumFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("Read_Coin"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iReadCoinNum: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	//tDevReturn sDevReturn[8];
    tCHADevReturn DevStatus  ={0};
	errCode = CHA_GetReceiveNumber(&m_res, &DevStatus);
	//errCode = gOCAS002Dev.iReadCoinNum(m_res,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}
	
	for(int i=0;i<2;i++){
		sLogString.Format(_T("iReadCoinNum: 接收到硬币信息（面额：%d，枚数：%d）"), m_res.stCoinBoxValueInfo[i].uiCoinValue,m_res.stCoinBoxValueInfo[i].uiCoinNum);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("iReadCoinNum: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
