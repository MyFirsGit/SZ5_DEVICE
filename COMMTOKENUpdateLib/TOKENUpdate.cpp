#include "stdafx.h"
#include "TOKENUpdate.h"
#include "TOKENCommBase.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief		TOKEN�̼�����

@param      (i)int port �˿ں�
@param      (i)int speed ������
@param      (i)char *data�ļ�����
@param      (i)long bufferLength�ļ����ݳ���

@retval     int 0:ִ�гɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int TOKENDownload(int port,int speed,char* data,long bufferLength)
{
	CTOKENCommBase comBase;
	return comBase.TOKENUpdate(port,speed,data,bufferLength);
}
