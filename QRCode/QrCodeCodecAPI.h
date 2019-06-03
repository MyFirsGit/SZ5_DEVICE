
#ifndef _QRCEDE_INTERFACE_H_
#define _QRCEDE_INTERFACE_H_

#ifndef QRCODE_API
#define QRCODE_API  extern "C"  _declspec(dllexport)
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief		图片解码

@param      (i)LPCSTR strFileName 文件名
@param      (o)int &nLevel 纠错级别
@param      (o)int &nVersion版本
@param      (o)int &nCodeSize 图片大小
@param      (o)char * &szContent图片内容
@param      (o)int length图片内容长度

@retval     int 0:解码成功，非0:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int DecodeQrCodeBmp(LPCSTR strFileName,int &nLevel,int &nVersion,int &nCodeSize,char *szContent,int length);

//////////////////////////////////////////////////////////////////////////
/**
@brief		数据编码

@param      (i)int nLevel 纠错级别
@param      (i)int nVersion版本
@param      (i)int nMaskingNo 掩模
@param      (i)BOOL nSizeLevel 图片放大倍数
@param      (i)CString strSource 数据内容
@param      (i)CString strFileName文件名(包括路径以及文件后缀名.bmp)

@retval     int 0:编码成功，非0:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int CreateQrCodeBmp(int nLevel, int nVersion, int nMaskingNo,int nSizeLevel, LPCSTR strSource,LPCSTR strFileName);


#endif  // 
