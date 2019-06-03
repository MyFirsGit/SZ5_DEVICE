#ifndef _TOKENUPDATE_INTERFACE_H_
#define _TOKENUPDATE_INTERFACE_H_

#ifndef TOKENUPDATE_API
#define TOKENUPDATE_API  extern "C"  _declspec(dllexport)
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief		TOKEN固件下载

@param      (i)int port 端口号
@param      (i)int speed 波特率
@param      (i)char *data文件内容
@param      (i)long bufferLength文件内容长度

@retval     int 0:执行成功，非0:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TOKENUPDATE_API int TOKENDownload(int port,int speed,char* data,long bufferLength);

#endif  // 
