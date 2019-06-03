#pragma once
#include <vector>
#include "coreexp.h"
#include "util.h"
using namespace std;


class CORE_API CXFile : public CStdioFile
{
public:
	CXFile(CString fileName);			
	virtual ~CXFile();
	CXFile(CString fileName , CString backupFileName);

public:
	typedef enum _err_detaitl{
		FILE_OPERATION_OK   = 0,        // 文件操作成功
		FILE_READ_FAIL	    = 102, 	    // 文件读取异常
		FILE_OPEN_FAIL	    = 104, 	    // 文件打开异常
	} ERR_DETAITL;
public:
	ERR_DETAITL WriteBinFile(const char* lpData, const ULONG nLen,bool hasSum = false);
	ERR_DETAITL ReadBinFile(VARIABLE_DATA& buffer,bool hasSum = false);
	ERR_DETAITL WriteFile(CString sData);
	ERR_DETAITL WriteFile(const char* lpData, const ULONG nLen);
	void ReadBinFileWithoutSUM(VARIABLE_DATA&);     // 读取不包含SUM的文件
	ERR_DETAITL WriteFileWithoutSUM(const char* lpData, const ULONG nLen);

	virtual BOOL ReadString(CString& rString);

	ERR_DETAITL WriteBinFileWithCrc(VARIABLE_DATA& data);
	ERR_DETAITL ReadBinFileWithCrc(VARIABLE_DATA& data);

	ERR_DETAITL WriteFileNoCopy(const char* lpData, const ULONG nLen);
	ERR_DETAITL CopyXFile();
	ERR_DETAITL ClearBinFile();
	ERR_DETAITL DelBinFile();

private:
	void CalcSum(const char* pData, const int nSize, char* pSum);
	void OpenFile(CString& fileName);
	void OpenBinFileRead(CString fileName);
	void OpenBinFile(CString fileName);
	void ReadBinFileWithCrc(CString fileName,VARIABLE_DATA& data);
	void ReadBinFile(CString fileName, VARIABLE_DATA& data, bool hasSum = false);

private:
	CString m_fileName;				// 基本文件名
	CString m_backfileName;			// 备份文件名
	bool m_hasBackup;				// 文件是否有备份还原安全机制

};