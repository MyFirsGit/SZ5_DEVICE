#pragma once

/************************************************************************/
/* Des 运算 
/************************************************************************/

// 8字节数据的DES运算
void des0(unsigned char bEncrypt, unsigned char* lpSrc,unsigned char* lpKey,unsigned char* lpResult);

// 8字节数据的3DES运算
void trides0(unsigned char bEncrypt, unsigned char *lpSrc, unsigned char *lpKey, unsigned char *lpResult);

