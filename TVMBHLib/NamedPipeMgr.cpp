#include "NamedPipeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none 

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
NamedPipeMgr::NamedPipeMgr():name_(""),pipe_(0)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none 

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
NamedPipeMgr::~NamedPipeMgr()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建命名管道服务器端

@param      (i)CString name  管道名 

@retval     int 0:成功  1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int NamedPipeMgr::create()
{
	name_ = "\\\\.\\pipe\\BHSIM";
	pipe_ = CreateNamedPipeA(
	name_,						 // pipe name 
    PIPE_ACCESS_DUPLEX |         // read/write access
	FILE_FLAG_OVERLAPPED,        
    PIPE_TYPE_MESSAGE |          // byte type pipe 
    PIPE_READMODE_MESSAGE |      // byte-read mode 
    PIPE_WAIT,                   // blocking mode 
    PIPE_INSTANCES,              // max. instances  
    IOBUFFERSIZE,                // output buffer size 
    IOBUFFERSIZE,                // input buffer size 
    PIPE_TIMEOUT,                // client time-out 
    (LPSECURITY_ATTRIBUTES)NULL);// no security attribute 

	if (pipe_ == INVALID_HANDLE_VALUE){
		return 1;
	}
	// 到此创建成功

	// 连接客户端,若失败则关闭管道 
	if(ConnectNamedPipe(pipe_, NULL) == 0){	
		//GetLastError() == ERROR_PIPE_CONNECTED;
		CloseHandle(pipe_); 
		return 1;
	}
	// 到此连接成功

	::memset(&olr_, 0, sizeof olr_);
	::memset(&olw_, 0, sizeof olw_);
	olr_.Offset = olw_.Offset = 0;
	olr_.OffsetHigh = olw_.OffsetHigh = 0;
	// 生成事件对象  
	olr_.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	olw_.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将数据写入管道

@param      (i)const char* buff  写入的BUFF
@param      (i)unsigned int len  写入的BUFF长度

@retval     DWORD 已写入的字节数

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD NamedPipeMgr::write(const char* buff, unsigned int len)
{
	unsigned long ret = 0;
	m_pipeStat = BHTRS_ST_SNDEXE;
	// 连接异常
	if(!pipe_){
		m_pipeStat = BHTRS_ST_SNDERR;
		ret = 0;
	}
	// 连接正常
	else{
		if(!WriteFile(pipe_, buff, len, &ret, &olw_)) {
			if (ERROR_IO_PENDING != ::GetLastError()) {
				// 不处理			
			}
			else {
				// 等待写入完成
				::WaitForSingleObject(olw_.hEvent, INFINITE);
			}
		}
		::GetOverlappedResult(pipe_, &olw_, &ret, TRUE); 
		m_pipeStat = (ret > 0)?BHTRS_ST_RCVQUE:BHTRS_ST_SNDERR;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从管道中读取数据

@param      (o)char* buff			读出的BUFF
@param      (o)unsigned int maxlen  读取的字节长度

@retval     DWORD 读到的字节数

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD NamedPipeMgr::read(char* buff, unsigned int maxlen)
{
	unsigned long ret = 0;
	m_pipeStat = BHTRS_ST_RCVEXE;
	// 连接异常
	if(!pipe_){
		m_pipeStat = BHTRS_ST_RCVERR;
		ret = 0;
	}
	// 连接正常
	else{
		if(!ReadFile(pipe_, buff, maxlen, &ret, &olr_)) {
			if (ERROR_IO_PENDING != ::GetLastError()) {
				// 不处理
			}
			else {
				// 等待读取完成
				::WaitForSingleObject(olr_.hEvent, INFINITE);
			}
		}
		::GetOverlappedResult(pipe_, &olr_, &ret, TRUE); 
		m_pipeStat = (ret > 0)?BHTRS_ST_SNDQUE:BHTRS_ST_RCVERR;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭命名管道

@param      none

@retval     bool  true:成功 false:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool NamedPipeMgr::close()
{
	bool ret(false);
	if(!pipe_){
		return ret;
	}
	if(CloseHandle(pipe_)){
		ret = true;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得当前通信状态

@param      none

@retval     long 通信状态

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long NamedPipeMgr::GetStat()
{
	return m_pipeStat;
}