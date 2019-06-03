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
@brief  ɾ���ļ�����Э��ͷ����(�����ļ�)
*/


class PARAMETERCOMMAND_DECLSPEC CParamHelp
{
public:
	CParamHelp();      // ���캯��
	~CParamHelp();     // ��������



	WORD DeleteTransHead(CString sFileName,bool isCrcChk = true); // ɾ���ļ�����Э��ͷ����(�����ļ�)
	WORD AddTransHead(CString sFileName, BYTE byTranType, bool isCrcChk = true);    // ����ļ�����Э��ͷ����(���ݰ��ļ�)

private:
	bool GetSrcDataFromFile(LPCTSTR pstrFile, BYTE **pbyData, DWORD &dwDataLen); //���ļ��еõ���Ϣ
};
