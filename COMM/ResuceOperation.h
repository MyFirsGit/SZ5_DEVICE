#pragma once
#include "CommonExp.h"
#include "Func.h"


class COMMON_API CResuceOperation
{
public:
	static bool ResuceFile(CString mainFilePath,CString backupFilePath,Func<bool(LPBYTE,UINT)>* funcValidator = NULL);
	static bool ResuceFileWithCrc(CString mainFilePath,CString backupFilePath);
	static bool DeleteResuceFile(CString mainFilePath,CString backupFilePath);
	static bool CheckAndCreateDirectory(CString mainFolderPath,CString backupFolderPath);
	static bool RenameResuceFile(CString mainFilePath,CString backupFilePath,CString mainNewFilePath,CString backupNewFilePath);
private:
	static bool ValidateCrc(LPBYTE lpContent,UINT nLen);
	
};
