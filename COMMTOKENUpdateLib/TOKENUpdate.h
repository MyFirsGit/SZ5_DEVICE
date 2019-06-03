#ifndef _TOKENUPDATE_INTERFACE_H_
#define _TOKENUPDATE_INTERFACE_H_

#ifndef TOKENUPDATE_API
#define TOKENUPDATE_API  extern "C"  _declspec(dllexport)
#endif

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
TOKENUPDATE_API int TOKENDownload(int port,int speed,char* data,long bufferLength);

#endif  // 
