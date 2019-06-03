#include "stdafx.h"
#include "ResuceOperation.h"
#include "Util.h"

bool CResuceOperation::ResuceFile(CString mainFilePath, CString backupFilePath,Func<bool(LPBYTE,UINT)>* funcValidator)
{
	bool isMainFileOK = false;//主文件是否正确
	if(ComCheckFileExist(mainFilePath))//主文件存在
	{
		isMainFileOK = true;
		if(funcValidator!=NULL)//提供了自定义内容验证器
		{
			VARIABLE_DATA fileData;
			ReadBinaryFile(mainFilePath,fileData);
			isMainFileOK = funcValidator->Invoke(fileData.lpData,fileData.nLen);//自定义验证结果
		}
		if(isMainFileOK)
		{
			for(int i=0;i<3;i++)
			{
				BOOL copyFileSuccess = ::CopyFile(mainFilePath, backupFilePath, FALSE);
				if(copyFileSuccess)
				{
					return true;
				}
			}
			return false;
		}
	}
	bool isBackupFileOK = false;
	if(ComCheckFileExist(backupFilePath))//备份文件存在
	{
		isBackupFileOK = true;
		if(funcValidator!=NULL)
		{
			VARIABLE_DATA fileData;
			ReadBinaryFile(backupFilePath,fileData);
			isBackupFileOK = funcValidator->Invoke(fileData.lpData,fileData.nLen);//自定义验证结果
		}
		if(isBackupFileOK)
		{
			for(int i=0;i<3;i++)
			{
				BOOL copyFileSuccess =::CopyFile(backupFilePath,mainFilePath, FALSE);
				if(copyFileSuccess)
				{
					return true;
				}
			}
			return false;
		}
	}
	return false;
}


bool CResuceOperation::ResuceFileWithCrc(CString mainFilePath,CString backupFilePath)
{
	Func<bool(LPBYTE,UINT)> crcValidator(&CResuceOperation::ValidateCrc);
	return ResuceFile(mainFilePath,backupFilePath,&crcValidator);
}


bool CResuceOperation::DeleteResuceFile(CString mainFilePath,CString backupFilePath)
{
	if(ComCheckFileExist(backupFilePath))
	{
		if(!DeleteFile(backupFilePath))
		{
			return false;
		}
	}
	if(ComCheckFileExist(mainFilePath))
	{
		if(!DeleteFile(mainFilePath))
		{
			return false;
		}
	}
	return true;
}

bool CResuceOperation::CheckAndCreateDirectory(CString mainFolderPath,CString backupFolderPath)
{
	if(!ComCheckAndCreateDirectory(mainFolderPath))
	{
		return false;
	}
	if(!ComCheckAndCreateDirectory(backupFolderPath))
	{
		return false;
	}
	return true;
}

bool CResuceOperation::RenameResuceFile(CString mainFilePath,CString backupFilePath,CString mainNewFilePath,CString backupNewFilePath)
{
	BOOL moveBackupFileSuccess = ::MoveFileEx(backupFilePath,backupNewFilePath,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
	if ( !moveBackupFileSuccess) 
	{ 
		for(int i=0;i<2;i++)//重试两次
		{
			::Sleep(100);
			moveBackupFileSuccess = ::MoveFileEx(backupFilePath,backupNewFilePath,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
			if(moveBackupFileSuccess)
			{
				break;
			}
		}
		if(!moveBackupFileSuccess)
		{
			return false;
		}
	}
	
	BOOL moveMainFileSuccess = ::MoveFileEx(mainFilePath,mainNewFilePath,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
	if ( !moveMainFileSuccess) 
	{ 
		for(int i=0;i<2;i++)//重试两次
		{
			::Sleep(100);
			moveMainFileSuccess = ::MoveFileEx(mainFilePath,mainNewFilePath,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
			if(moveMainFileSuccess)
			{
				break;
			}
		}
		if(!moveMainFileSuccess)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查文件的CRC32循环冗余校验码
@param     
@retval     bool				
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CResuceOperation::ValidateCrc(LPBYTE lpContent,UINT nLen)
{
	if(nLen<sizeof(unsigned long))
	{
		return false;
	}
	unsigned long  oldCrc = MAKELONG(
		MAKEWORD(
		*(lpContent+0),
		*(lpContent+1)
		),
		MAKEWORD(
		*(lpContent+2),
		*(lpContent+3)
		));
	unsigned long newCrc = Crc32(lpContent+sizeof(unsigned long),nLen-sizeof(unsigned long),0);
	return oldCrc == newCrc;
}