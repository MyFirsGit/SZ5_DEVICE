/******************************************************************************/
/**	
*   @class CSCSocket
*	@brief implementation file of CSCSocket class.
*	@version 2009.12.14: The first version that generated by VC .Net
*/
/******************************************************************************/

#include "stdafx.h"
#include "SCAPI.h"
#include "SCSocket.h"
#include <afxpriv.h>
#include "DebugLog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  __FILENAME__ _T("")

#define RECEIVE_DATA_LENGTH 8194    // 最大电文长度

#define  theSC_Client_Data CFileLog::GetInstance(FILTER_SC_CLIENT_DATA)
#define theSC_Client_Trace CFileLog::GetInstance(FILTER_SC_CLIENT_LOG)

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数
@param      none
@retval     none
*/
//////////////////////////////////////////////////////////////////////////
CSCSocket::CSCSocket()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数
@param      none
@retval     none
*/
//////////////////////////////////////////////////////////////////////////
CSCSocket::~CSCSocket()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      接受数据
@param      VARIABLE_DATA& data 接受的数据
@retval     bool 接受成功或失败
*/
//////////////////////////////////////////////////////////////////////////
bool  CSCSocket::TryReceiveOnePackage(VARIABLE_DATA& data)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	UINT readlen = 0;
	int nbyte = 0;
	UINT receiveNextLen = 0;

	BYTE pReceiveData[RECEIVE_DATA_LENGTH];
	BYTE* pTemp = pReceiveData;

	while((nbyte = Receive(pTemp, (LEN_START_TAG_CMD_HEADER - readlen))) > 0)
	{
		readlen += nbyte;
		pTemp += nbyte;
		if (readlen >= LEN_START_TAG_CMD_HEADER)
		{
			break;
		}
	}//接收头部

	bool receiveHeaderSuccess = nbyte>0;//接收头部数据是否成功
	//开始标记是否正确
	bool startTagIsRight = receiveHeaderSuccess && pReceiveData[0] == START_TAG;
	//电文数据长度是否超过RECEIVE_DATA_LENGTH;

	UINT length=0;
	memcpy(&length,&pReceiveData[OFFSET_PACKAGE_LENGTH],4);//消息长度
	bool messageLengthIsRight = receiveHeaderSuccess && length<=RECEIVE_DATA_LENGTH&& length>LEN_START_TAG_CMD_HEADER; 

	if (!receiveHeaderSuccess || !startTagIsRight|| !messageLengthIsRight)
	{
		// Socket 关闭
		theSC_Client_Data->WriteData(_T("<"),pReceiveData,readlen);
		theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("-> !receiveHeaderSuccess || !startTagIsRight|| !messageLengthIsRight."));
		return false;
	}

	receiveNextLen = length - LEN_START_TAG_CMD_HEADER;
	readlen = 0;
	while((nbyte = Receive(pTemp, receiveNextLen - readlen)) > 0)
	{
		readlen += nbyte;
		pTemp += nbyte;
		if (readlen >= receiveNextLen)
		{
			break;
		}
	}//接受消息体
	//接收消息体是否成功
	bool receiveBodySuccess = nbyte>0; 
	//消息结束标志是否正确
	bool endTagIsRight = receiveBodySuccess && pReceiveData[LEN_START_TAG_CMD_HEADER + receiveNextLen -LEN_END_TAG] == END_TAG;
	if(!receiveBodySuccess || !endTagIsRight)
	{
		theSC_Client_Data->WriteData(_T("<"),pReceiveData,readlen);
		theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("-> !receiveBodySuccess || !endTagIsRight."));
		return false;
	}
	if(data.lpData!=NULL)
	{
		delete data.lpData;
		data.lpData = NULL;
	}
	data.nLen = 0;
	data.lpData = new BYTE[length];
	memcpy(data.lpData,pReceiveData,length);
	data.nLen = length;
	theSC_Client_Data->WriteData(_T("<"),pReceiveData,length);
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      数据接收事件
@param      nErrorCode  错误代码
@retval     none
*/
//////////////////////////////////////////////////////////////////////////
void CSCSocket::OnReceive(int nErrorCode)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" <-"));
	Receiving.Invoke(nErrorCode);
	//Sleep(10);
	CSocket::OnReceive(nErrorCode);
	VARIABLE_DATA package;
	bool receiveImpSuccess = TryReceiveOnePackage(package);
	Received.Invoke(receiveImpSuccess,package);
	AsyncSelect();
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" -> "));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭Socket连接
@param      nErrorCode  错误代码
@retval     none
*/
//////////////////////////////////////////////////////////////////////////
void CSCSocket::OnClose(int nErrorCode)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("%d<-"),nErrorCode);
	CSocket::OnClose(nErrorCode);
	Closed.Invoke(nErrorCode);
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送消息
@param      unsigned char *lpBuf    待发送消息
@param      int nBufLen             消息数据长度
@retval     long              发送结果代码
*/
//////////////////////////////////////////////////////////////////////////
long CSCSocket::Send(const unsigned char *lpBuf, int nBufLen)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	theSC_Client_Data->WriteData(_T(">"),(LPBYTE)lpBuf,nBufLen);
	Sending.Invoke(lpBuf,nBufLen);
	long result = CSocket::Send(lpBuf, nBufLen);
	Sended.Invoke(lpBuf,nBufLen);
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}


