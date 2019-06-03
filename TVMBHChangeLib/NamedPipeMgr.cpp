#include "NamedPipeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

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
@brief      ��������

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
@brief      ���������ܵ���������

@param      (i)CString name  �ܵ��� 

@retval     int 0:�ɹ�  1:ʧ��

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
	// ���˴����ɹ�

	// ���ӿͻ���,��ʧ����رչܵ� 
	if(ConnectNamedPipe(pipe_, NULL) == 0){	
		//GetLastError() == ERROR_PIPE_CONNECTED;
		CloseHandle(pipe_); 
		return 1;
	}
	// �������ӳɹ�

	::memset(&olr_, 0, sizeof olr_);
	::memset(&olw_, 0, sizeof olw_);
	olr_.Offset = olw_.Offset = 0;
	olr_.OffsetHigh = olw_.OffsetHigh = 0;
	// �����¼�����  
	olr_.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	olw_.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������д��ܵ�

@param      (i)const char* buff  д���BUFF
@param      (i)unsigned int len  д���BUFF����

@retval     DWORD ��д����ֽ���

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD NamedPipeMgr::write(const char* buff, unsigned int len)
{
	unsigned long ret = 0;
	m_pipeStat = BHTRS_ST_SNDEXE;
	// �����쳣
	if(!pipe_){
		m_pipeStat = BHTRS_ST_SNDERR;
		ret = 0;
	}
	// ��������
	else{
		if(!WriteFile(pipe_, buff, len, &ret, &olw_)) {
			if (ERROR_IO_PENDING != ::GetLastError()) {
				// ������			
			}
			else {
				// �ȴ�д�����
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
@brief      �ӹܵ��ж�ȡ����

@param      (o)char* buff			������BUFF
@param      (o)unsigned int maxlen  ��ȡ���ֽڳ���

@retval     DWORD �������ֽ���

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD NamedPipeMgr::read(char* buff, unsigned int maxlen)
{
	unsigned long ret = 0;
	m_pipeStat = BHTRS_ST_RCVEXE;
	// �����쳣
	if(!pipe_){
		m_pipeStat = BHTRS_ST_RCVERR;
		ret = 0;
	}
	// ��������
	else{
		if(!ReadFile(pipe_, buff, maxlen, &ret, &olr_)) {
			if (ERROR_IO_PENDING != ::GetLastError()) {
				// ������
			}
			else {
				// �ȴ���ȡ���
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
@brief      �ر������ܵ�

@param      none

@retval     bool  true:�ɹ� false:ʧ��

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
@brief      ȡ�õ�ǰͨ��״̬

@param      none

@retval     long ͨ��״̬

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long NamedPipeMgr::GetStat()
{
	return m_pipeStat;
}