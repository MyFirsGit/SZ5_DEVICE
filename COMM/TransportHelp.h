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
@brief	ҵ��������
*/
static const int MAX_READ_BUFFER_SIZE       = 10*1024;
unsigned int cal_crc16(unsigned char *ptr, unsigned long len);

class CTransportHelp
{
public:
	CTransportHelp();      // ���캯��
	~CTransportHelp();     // ��������



	WORD DeleteTransHead(CString sFileName,bool isCrcChk = true); // ɾ���ļ�����Э��ͷ����(�����ļ�)

private:
	bool GetSrcDataFromFile(LPCTSTR pstrFile, BYTE **pbyData, DWORD &dwDataLen);
};
