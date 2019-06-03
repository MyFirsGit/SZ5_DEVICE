
#ifndef _QRCEDE_INTERFACE_H_
#define _QRCEDE_INTERFACE_H_

#ifndef QRCODE_API
#define QRCODE_API  extern "C"  _declspec(dllexport)
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief		ͼƬ����

@param      (i)LPCSTR strFileName �ļ���
@param      (o)int &nLevel ������
@param      (o)int &nVersion�汾
@param      (o)int &nCodeSize ͼƬ��С
@param      (o)char * &szContentͼƬ����
@param      (o)int lengthͼƬ���ݳ���

@retval     int 0:����ɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int DecodeQrCodeBmp(LPCSTR strFileName,int &nLevel,int &nVersion,int &nCodeSize,char *szContent,int length);

//////////////////////////////////////////////////////////////////////////
/**
@brief		���ݱ���

@param      (i)int nLevel ������
@param      (i)int nVersion�汾
@param      (i)int nMaskingNo ��ģ
@param      (i)BOOL nSizeLevel ͼƬ�Ŵ���
@param      (i)CString strSource ��������
@param      (i)CString strFileName�ļ���(����·���Լ��ļ���׺��.bmp)

@retval     int 0:����ɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int CreateQrCodeBmp(int nLevel, int nVersion, int nMaskingNo,int nSizeLevel, LPCSTR strSource,LPCSTR strFileName);


#endif  // 
