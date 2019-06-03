#include <io.h>
#include "DebugLog.h"
#include "CParameterCommandExp.h"

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

/**
@brief  删除文件传输协议头部分(参数文件)
*/


class PARAMETERCOMMAND_DECLSPEC CParamHelp
{
public:
	CParamHelp();      // 构造函数
	~CParamHelp();     // 析构函数



	WORD DeleteTransHead(CString sFileName,bool isCrcChk = true); // 删除文件传输协议头部分(参数文件)
	WORD AddTransHead(CString sFileName, BYTE byTranType, bool isCrcChk = true);    // 添加文件传输协议头部分(数据包文件)

private:
	bool GetSrcDataFromFile(LPCTSTR pstrFile, BYTE **pbyData, DWORD &dwDataLen); //从文件中得到信息
};
