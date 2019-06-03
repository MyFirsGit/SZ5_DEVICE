#pragma once
// #include "RWTool.h"
// #include "PRTTool.h"
// #include "YKT_QUERY.h"
// #include "SCMessage.h"
// #include "CardProperty.h"
// #include "InfoZip.h"
#include <io.h>

inline int _ACCESS(LPCTSTR str, int mode)
{
#ifdef UNICODE
	return _waccess(str, mode);
#else
	// 	USES_CONVERSION;
	// 	LPSTR lpStr = T2A((LPCTSTR)str);
	return _access(str, mode);
#endif
}

#define SafeDeleteArray(pObjArray)\
{\
	if(pObjArray!=NULL){\
	delete [](pObjArray);\
	(pObjArray)=NULL;\
	}\
}

/**
@brief	业务服务基类
*/
static const int MAX_READ_BUFFER_SIZE       = 10*1024;
unsigned int cal_crc16(unsigned char *ptr, unsigned long len);

class CTransportHelp
{
public:
	CTransportHelp();      // 构造函数
	~CTransportHelp();     // 析构函数



	WORD DeleteTransHead(CString sFileName,bool isCrcChk = true); // 删除文件传输协议头部分(参数文件)

private:
	bool GetSrcDataFromFile(LPCTSTR pstrFile, BYTE **pbyData, DWORD &dwDataLen);
};
