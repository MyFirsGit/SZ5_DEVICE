#ifndef _THUPDATE_INTERFACE_H_
#define _THUPDATE_INTERFACE_H_

#ifndef THUPDATE_API
#define THUPDATE_API  extern "C"  _declspec(dllexport)
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief		TH固件下载

@param      (i)int port 端口号
@param      (i)int speed 波特率
@param      (i)char *pFilePath文件路径

@retval     int 0:执行成功，非0:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
THUPDATE_API int THDownload(int port,int speed,int updateSpeed,char* pFilePath);

#endif  // 
