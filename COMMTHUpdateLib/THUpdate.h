#ifndef _THUPDATE_INTERFACE_H_
#define _THUPDATE_INTERFACE_H_

#ifndef THUPDATE_API
#define THUPDATE_API  extern "C"  _declspec(dllexport)
#endif

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
THUPDATE_API int THDownload(int port,int speed,int updateSpeed,char* pFilePath);

#endif  // 
