#include "stdafx.h"
#include "ResuceOperation.h"
#include "Util.h"

bool CResuceOperation::ResuceFile(CString mainFilePath, CString backupFilePath,Func<bool(LPBYTE,UINT)>* funcValidator)
{
	bool isMainFileOK = false;//���ļ��Ƿ���ȷ
	if(ComCheckFileExist(mainFilePath))//���ļ�����
	{
		isMainFileOK = true;
		if(funcValidator!=NULL)//�ṩ���Զ���������֤��
		{
			VARIABLE_DATA fileData;
			ReadBinaryFile(mainFilePath,fileData);
			isMainFileOK = funcValidator->Invoke(fileData.lpData,fileData.nLen);//�Զ�����֤���
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
	if(ComCheckFileExist(backupFilePath))//�����ļ�����
	{
		isBackupFileOK = true;
		if(funcValidator!=NULL)
		{
			VARIABLE_DATA fileData;
			ReadBinaryFile(backupFilePath,fileData);
			isBackupFileOK = funcValidator->Invoke(fileData.lpData,fileData.nLen);//�Զ�����֤���
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
		for(int i=0;i<2;i++)//��������
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
		for(int i=0;i<2;i++)//��������
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
@brief      ����ļ���CRC32ѭ������У����
@param     
@retval     bool				
@exception  ��
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