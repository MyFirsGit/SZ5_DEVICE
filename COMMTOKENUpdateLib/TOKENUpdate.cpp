#include "stdafx.h"
#include "TOKENUpdate.h"
#include "TOKENCommBase.h"

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
int TOKENDownload(int port,int speed,char* data,long bufferLength)
{
	CTOKENCommBase comBase;
	return comBase.TOKENUpdate(port,speed,data,bufferLength);
}
