#include "stdafx.h"
#include "THUpdate.h"
#include "THCommBase.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief		TH�̼�����

@param      (i)int port �˿ں�
@param      (i)int speed ������
@param      (i)char *pFilePath�ļ�·��

@retval     int 0:ִ�гɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int THDownload(int port,int speed,int updateSpeed,char* pFilePath)
{
	CTHCommBase comBase;
	return comBase.THUpdate(port,speed,updateSpeed,pFilePath);
}
