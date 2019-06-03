#include "StdAfx.h"
#include "dxfile.h"
#include "Xfile.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief  ���캯��    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::CDXFile(CString fileName, CString backupFileName)
	: m_mainFileName(fileName)
	, m_backupFileName(backupFileName)
{

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::~CDXFile()
{
}


//////////////////////////////////////////////////////////////////////////
/*@brief     ���ػ�д�����ļ� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::ERR_DETAITL CDXFile::WriteBinFile(const char* lpData, const ULONG nLen,bool hasSum)
{
	{
		auto_ptr<CXFile> mainFile(new CXFile(m_mainFileName));
		mainFile->WriteBinFile(lpData,nLen,hasSum);
	}
	BOOL copyFileSuccess = FALSE;
	for(int i=0;i<3;i++)
	{
		copyFileSuccess = ::CopyFile(m_mainFileName, m_backupFileName, FALSE);
		if(copyFileSuccess)
		{
			return FILE_OPERATION_OK;
		}
		::Sleep(100);
	}
	return FILE_OPERATION_OK;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief ���ػ��������ļ�     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::ERR_DETAITL CDXFile::ReadBinFile(VARIABLE_DATA& buffer,bool hasSum)
{
	try{
		{
			auto_ptr<CXFile> mainFile(new CXFile(m_mainFileName));
			mainFile->ReadBinFile(buffer,hasSum);
		}
		BOOL copyFileSuccess = FALSE;
		for(int i=0;i<3;i++)
		{
			copyFileSuccess = ::CopyFile(m_mainFileName, m_backupFileName, FALSE);
			if(copyFileSuccess)
			{
				return FILE_OPERATION_OK;
			}
			::Sleep(100);
		}
	}
	catch(...)
	{

	}
	{
		auto_ptr<CXFile> backupFile(new CXFile(m_backupFileName));
		backupFile->ReadBinFile(buffer,hasSum);
	}
	BOOL copyFileSuccess = FALSE;
	for(int i=0;i<3;i++)
	{
		copyFileSuccess = ::CopyFile(m_backupFileName,m_mainFileName , FALSE);
		if(copyFileSuccess)
		{
			return FILE_OPERATION_OK;
		}
		::Sleep(100);
	}
	return FILE_READ_FAIL;

}


//////////////////////////////////////////////////////////////////////////
/*
@brief ���ػ��������ļ�     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::ERR_DETAITL CDXFile::ReadBinFile(VARIABLE_DATA& buffer,Func<bool(VARIABLE_DATA&)>* validator,bool hasSum)
{
	bool readMainFileSuccess = false;
	try{
		{
			auto_ptr<CXFile> mainFile(new CXFile(m_mainFileName));
			readMainFileSuccess = mainFile->ReadBinFile(buffer,hasSum) == CXFile::FILE_OPERATION_OK;
			if(readMainFileSuccess && validator!=NULL)
			{
				readMainFileSuccess = validator->Invoke(buffer);
			}
		}
		if(readMainFileSuccess)
		{
			for(int i=0;i<3;i++)
			{
				BOOL copyFileSuccess = ::CopyFile(m_mainFileName, m_backupFileName, FALSE);
				if(copyFileSuccess)
				{
					return FILE_OPERATION_OK;
				}
				::Sleep(100);
			}
			return FILE_RESEURE_FAIL_BACKUP;
		}
	}
	catch(...)
	{

	}
	bool readBackupFileSuccess = false;
	{
		auto_ptr<CXFile> backupFile(new CXFile(m_backupFileName));
		readBackupFileSuccess = backupFile->ReadBinFile(buffer,hasSum) == CXFile::FILE_OPERATION_OK;
	}
	if(readBackupFileSuccess && validator!=NULL)
	{
		readBackupFileSuccess = validator->Invoke(buffer);
	}
	if(readBackupFileSuccess)
	{
		for(int i=0;i<3;i++)
		{
			BOOL copyFileSuccess = ::CopyFile(m_backupFileName, m_mainFileName, FALSE);
			if(copyFileSuccess)
			{
				return FILE_OPERATION_OK;
			}
			::Sleep(100);
		}
		return FILE_RESEURE_FAIL_MAIN;
	}
	return FILE_READ_FAIL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ļ�

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDXFile::ERR_DETAITL CDXFile::ClearFile()
{
	{
		auto_ptr<CXFile> mainFile(new CXFile(m_mainFileName));
		mainFile->SetLength(0);
	}
	BOOL copyFileSuccess = FALSE;
	for(int i=0;i<3;i++)
	{
		copyFileSuccess = ::CopyFile(m_mainFileName, m_backupFileName, FALSE);
		if(copyFileSuccess)
		{
			return FILE_OPERATION_OK;
		}
		::Sleep(100);
	}
	return FILE_READ_FAIL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    ���ػ��ļ�  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::ResuceFile(CString mainFilePath, CString backupFilePath,Func<bool(CString)>* funcValidator)
{
	bool isMainFileOK = funcValidator!=NULL && funcValidator->Invoke(mainFilePath);//�Զ�����֤���
	if(isMainFileOK)
	{
		BOOL copyFileSuccess = FALSE;
		for(int i=0;i<3;i++)
		{
			copyFileSuccess = ::CopyFile(mainFilePath, backupFilePath, FALSE);
			if(copyFileSuccess)
			{
				return true;
			}
			::Sleep(100);
		}
		if(!copyFileSuccess)
		{
			return false;
		}
	}

	bool isBackupFileOK = funcValidator!=NULL && funcValidator->Invoke(backupFilePath);//�Զ�����֤���
	if(isBackupFileOK)
	{
		BOOL copyFileSuccess = FALSE;
		for(int i=0;i<3;i++)
		{
			copyFileSuccess = ::CopyFile(backupFilePath, mainFilePath, FALSE);
			if(copyFileSuccess)
			{
				return true;
			}
			::Sleep(100);
		}
		if(!copyFileSuccess)
		{
			return false;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���ػ��ļ� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::ResuceFile(CString mainFilePath,CString backupFilePath,bool hasSum)
{
	if(hasSum)
	{
		Func<bool(CString)> validator(CDXFile::ValidateFileWithSum);
		return ResuceFile(mainFilePath,backupFilePath,&validator);
	}
	else
	{
		Func<bool(CString)> validator(CDXFile::ValidateFileExist);
		return ResuceFile(mainFilePath,backupFilePath,&validator);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ���ػ�ɾ���ļ�    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::DeleteResuceFile(CString mainFilePath,CString backupFilePath)
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

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���ػ�ȷ��Ŀ¼���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::CheckAndCreateDirectory(CString mainFolderPath,CString backupFolderPath)
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

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���ػ��������ļ�   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::RenameResuceFile(CString mainFilePath,CString backupFilePath,CString mainNewFilePath,CString backupNewFilePath)
{

	BOOL moveBackupFileSuccess = FALSE;
	for(int i=0;i<3;i++)
	{
		moveBackupFileSuccess = ::MoveFileEx(backupFilePath,backupNewFilePath,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
		if(moveBackupFileSuccess)
		{
			break;
		}
		::Sleep(100);
	}
	if(!moveBackupFileSuccess)
	{
		return false;
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
/*
@brief    ��֤��sum���ļ�  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::ValidateFileWithSum(CString fileName)
{
	bool fileExsit = ValidateFileExist(fileName);
	if(!fileExsit)
	{
		return false;
	}
	try{
		VARIABLE_DATA data;
		CXFile file(fileName);
		return file.ReadBinFile(data,true) == CXFile::FILE_OPERATION_OK;
	}
	catch(CSysException&)
	{
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��֤�ļ��Ƿ���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDXFile::ValidateFileExist(CString fileName)
{
	return ComCheckFileExist(fileName);
}
