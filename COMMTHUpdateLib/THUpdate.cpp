#include "stdafx.h"
#include "THUpdate.h"
#include "THCommBase.h"

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
int THDownload(int port,int speed,int updateSpeed,char* pFilePath)
{
	CTHCommBase comBase;
	return comBase.THUpdate(port,speed,updateSpeed,pFilePath);
}
