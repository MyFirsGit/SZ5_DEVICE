#include "stdafx.h"
#include "TxnDataControl.h"
#include <algorithm>
#include <functional>
#include "sync.h"
#include "SysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////
///**
// @brief      目录是否存在
//
// @param      (i)CString  strPath    目录
//
// @retval     bool
//             true:目录存在  false:目录不否存在
//
// @exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool IsFolderExist(CString strPath)
{
	// 目录是否存在
	if (strPath ==  _T("")) {
		return false;
	}
	long lFolderAttribute = GetFileAttributes(strPath);
	if (lFolderAttribute == INVALID_FILE_ATTRIBUTES) {
		return false;
	} else {
		// 是目录
		if (lFolderAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			return true;
		// 不是目录
		} else {
			return false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
///**
// @brief      文件是否存在
//
// @param      (i)CString  strPath    文件
//
// @retval     bool
//             true:文件存在  false:文件不否存在
//
// @exception  无
//*/
//////////////////////////////////////////////////////////////////////////
bool IsFileExist(CString strFile)
{
	// 文件是否存在
	if (strFile ==  _T("")) {
		return false;
	}
	long lFileAttribute = GetFileAttributes(strFile);
	if (lFileAttribute == INVALID_FILE_ATTRIBUTES) {
		return false;
	} else {
		// 不是文件
		if (lFileAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			return false;
		// 是文件
		} else {
			return true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
/*
 @brief		获取数据包日期 

 @param     (i)CString		pekgName	数据包ID

 @retval    CString 数据包组成日期

 @exception  无
*/
////////////////////////////////////////////////////////////////////////////
CString GetPekgDate(CString pekgName)
{
	return pekgName.Mid(10,8);
}

////////////////////////////////////////////////////////////////////////////
/*
 @brief		获取今天日期 

 @param     无

 @retval    CString 今天日期

 @exception  无
*/
////////////////////////////////////////////////////////////////////////////
CString	GetTodayDate()
{
	CString ret =  _T("");
	//_DATE today = ComGetCurDate();
	//ret.Format( _T("%04d%02d%02d"),today.wYear,today.biMonth,today.biDay);
	return ret;
}

////////////////////////////////////////////////////////////////////////////
///**
// @brief      新建目录
//
// @param      (i)CString		strPath		目录
//
// @retval     bool
//             true:新建成功  false:新建失败
//
// @exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool CreatePath(CString strPath)
{
	CStringArray strArrPath;
	BOOL bSucess = FALSE;
	int iLoop = 0;
	CString strTemp =  _T("");
	
	// 目录存在
	if (IsFolderExist(strPath))	{
		return true;
	}
	strArrPath.RemoveAll();

	for(iLoop = 0; iLoop < strPath.GetLength(); iLoop++) {
		// 一层目录未结束
		if (strPath.GetAt(iLoop) != '\\') {
			strTemp += strPath.GetAt(iLoop);
		// 一层目录结束
		} else {
			strArrPath.Add(strTemp);
			strTemp += _T("\\");
		}
		// 添加最后一层目录
		if (iLoop == strPath.GetLength()-1) {
			strArrPath.Add(strTemp);
		}
	}

	for(iLoop = 1; iLoop < strArrPath.GetSize(); iLoop++) {
		strTemp = strArrPath.GetAt(iLoop);
		bSucess = CreateDirectory(strTemp, NULL);
		// 目录新建成功
		if (bSucess) {
			SetFileAttributes(strTemp, FILE_ATTRIBUTE_NORMAL);
		// 目录新建失败或者目录已经存在
		} else {
			// 目录新建失败
			if (!IsFolderExist(strTemp)) {
				return false;
			}
		}
	}
	strArrPath.RemoveAll();
	// 目录新建成功
	if (IsFolderExist(strPath)) {
		return true;
	// 目录新建失败
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////
/*
@brief      保存数据包到文件中

@param      (i)CString strFileName						数据包文件名
@param      (i)LPBYTE dataBuff							数据包内容
@param      (i)DWORD datalen			

@retval     bool  true:保存成功  false:保存失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
bool WritePekgFile(CString strFileName,LPBYTE dataBuff,DWORD datalen)
{   
	bool ret = false; 
	byte * bFileData = NULL; 
	unsigned int uiFileLength = 0; 
	CFile cfFile; 
	CFileException cfeError; 
	bool bOpen = false; 
	SYNC(FILE,strFileName);
	try { 
		// 打开文件
		if(!cfFile.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException =  _T("异常："); 
			strException += strCause; 
			throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		bOpen = true; 
		cfFile.SeekToBegin(); 
		cfFile.SetLength(0); 	
		if (datalen > 0) { 
			bFileData = (byte *)malloc(datalen); 
			if (bFileData == NULL) { 
				cfFile.Close(); 
				bOpen = false; 
				throw CTxnDataException(CTxnDataException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("分配内存失败")); 
			} 
			memcpy(bFileData,dataBuff, datalen);
			// 写入文件
			try { 
				cfFile.Write(bFileData, datalen); 
				cfFile.Flush();
			} 
			catch (...) { 
				free(bFileData); 
				bFileData = NULL; 
				cfFile.Close(); 
				bOpen = false; 
				throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("文件写入异常：" + strFileName)); 
			} 
			cfFile.Close(); 
			bOpen = false; 
			free(bFileData); 
			bFileData = NULL; 
			ret = true;
		} 
		else { 
			cfFile.Close(); 
			bOpen = false; 
			throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("文件写入异常：" + strFileName)); 
		} 
	} 
	catch(CSysException&) { 
		if (bFileData != NULL) { 
			free(bFileData); 
			bFileData = NULL; 
		} 
		if (bOpen) { 
			cfFile.Close(); 
		} 
		throw; 
	} 
	catch (CException &ex) { 
		TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
		CString strException; 
		ex.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = "异常："; 
		strException += strCause; 
		if (bFileData != NULL) { 
			free(bFileData); 
			bFileData = NULL; 
		} 
		if (bOpen) { 
			cfFile.Close(); 
		} 
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, strException); 
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////
/*
@brief      读取数据包文件

@param      (i)CString strFileName						数据包文件名
@param      (o)PEKG_DATA& pekgData						当前数据包内容

@retval     无

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
void ReadPekgFile(CString strFileName,PEKG_DATA& pekgData)
{
	// 读取文件开始		
	unsigned int uiFileLength = 0; 
	CFile cfFile; 
	CFileException cfeError; 
	bool bOpen = false; 
	SYNC(FILE,strFileName);
	try { 
		// 打开文件
		if(!cfFile.Open(strFileName, CFile::modeRead | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = strCause; 
			throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, _T("文件<")+ strFileName + _T(">打开失败:") + strException); 
		} 
		bOpen = true; 
		cfFile.SeekToBegin(); 
		uiFileLength = (unsigned int)cfFile.GetLength(); 
		if (uiFileLength > 0) { 
			pekgData.lpData = (byte *)malloc(uiFileLength); 
			if (pekgData.lpData == NULL) { 
				cfFile.Close(); 
				bOpen = false; 
				throw CTxnDataException(CTxnDataException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("分配内存失败")); 
			} 
			memset(pekgData.lpData, 0x00, uiFileLength); 
			// 读取文件
			if (cfFile.Read(pekgData.lpData, uiFileLength) != uiFileLength) { 
				cfFile.Close(); 
				bOpen = false; 				
				throw CTxnDataException(CTxnDataException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("文件读取异常：" + strFileName)); 
			} 
			pekgData.nLen = uiFileLength;
			cfFile.Close(); 
			bOpen = false;
		} 
		else { 
			cfFile.Close(); 
			bOpen = false; 
			throw CTxnDataException(CTxnDataException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("文件读取异常：" + strFileName)); 
		} 
	} 
	catch(CSysException&) { 
		if (bOpen) { 
			cfFile.Close(); 
		} 
		throw; 
	} 
	catch (...) { 
		if (bOpen) { 
			cfFile.Close(); 
		} 
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("文件读取异常：" + strFileName)); 
	}
}
 
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取指定目录下的所有数据包文件名(按时间排序)

@param       (o)所有文件名

@retval      bool true: 获取成功  false：获取失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void GetUnsendFileName(CString strDir,queue<CString>& fileNames)
{
	try{
		// 取得文件目录
		CString sPath = strDir;

		// 查找文件
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*"));

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return;
		}

		vector<CString> tempFileNames;
		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取出文件名，用于比较日期
				sFileName = finder.GetFileName(); 
				fileNames.push(sFileName);				
			}
		}
		finder.Close();
	} 
	catch(CSysException&) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获取未送文件名错误")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获取未送文件名错误")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获得当前未送数据存留的天数

@param       (i)CString strDir  未送数据目录
@param       (o)所有文件名

@retval      int 当前未送数据存留的天数

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
unsigned int GetUnsendDayCount(CString strDir)
{
	try{
		map<CString,CString> mapNotSend;
		// 取得文件目录
		CString sPath = strDir;

		// 查找文件
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*")+ _T(".*"));

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return 0;
		}

		mapNotSend.clear();
		while (bContinue) {
			bContinue = finder.FindNextFile();
			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取出文件名，用于比较日期
				sFileName = finder.GetFileName(); 
				mapNotSend[GetPekgDate(sFileName)] = sFileName;
			}
		}
		finder.Close();
		return (unsigned int)mapNotSend.size();
	} 
	catch(CSysException&) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获得未送数据存留的天数")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获得未送数据存留的天数")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      取得INI未送天数

 @param      (o)int			iUnSendDataMaxDay		未送数据最大允许天数

 @retval     无

 @exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void GetUnsendDay(unsigned int& iUnSendDataMaxDay)
//{
//	iUnSendDataMaxDay = theTVM_INFO.GetLimitedDaysOfUnSendData();
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      取得INI已送天数

 @param      (o)int			iSendedDataMaxDay		已送数据最大允许天数

 @retval     无

 @exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void GetSendedDay(unsigned int& iSendedDataMaxDay)
//{
//	iSendedDataMaxDay=theTVM_INFO.GetLimitedDaysOfSendData();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取指定目录下文件夹的数量

@param       (i)CString strDir					指定目录
@param       (o)vector<CString> tempDirNames    目录下的文件名

@retval      unsigned int: 指定目录下文件夹的数量

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
unsigned int GetDirectoryCount(CString strDir,vector<CString>& tempDirNames)
{
	try{
		unsigned int ret = 0;
		// 取得文件目录
		CString sPath = strDir;

		// 查找文件
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*")+ _T(".*"));

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return 0;
		}

		tempDirNames.clear();
		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDirectory() && finder.GetFileName() !=  _T(".") && finder.GetFileName() !=  _T("..")) {
				sFileName = finder.GetFileName(); 
				tempDirNames.push_back(sFileName);				
			}
			else {  
				continue;							
			}
		}
		finder.Close();
		ret = (unsigned int)tempDirNames.size();
		return ret;
	} 
	catch(CSysException&) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获取未送文件名错误")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("获取未送文件名错误")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       删除目录及目录下所有内容

@param       (i)CString strDir					指定目录

@retval      unsigned int: 指定目录下文件夹的数量

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void Removecate(CString strfolderPath)
{
	try{
		const TCHAR * folderPath = strfolderPath.GetBuffer(0);		
		SetFileAttributes(folderPath,FILE_ATTRIBUTE_NORMAL); 
		TCHAR fileFound[1024];
		WIN32_FIND_DATA info;
		HANDLE hp; 
	    swprintf_s(fileFound,1024, _T("%s\\*.*"), folderPath);
		hp = FindFirstFile(fileFound, &info); 
		do
		{
			if (!((_tcscmp(info.cFileName,  _T("."))==0)||(_tcscmp(info.cFileName,  _T(".."))==0)))
			{
				// 文件夹处理
				if((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{
					SYNC(FOLDER,folderPath);
					CString subFolder = folderPath;
					subFolder.Append( _T("\\"));
					subFolder.Append(info.cFileName);
					Removecate(subFolder);
					RemoveDirectoryW(subFolder);
				}
				// 文件处理
				else
				{
					SYNC(FILE,fileFound);
					swprintf_s(fileFound,1024, _T("%s\\%s"), folderPath, info.cFileName);
					// 取消文件只读属性
					SetFileAttributes(fileFound,FILE_ATTRIBUTE_NORMAL); 					
					BOOL retVal = DeleteFile(fileFound);
					if(!retVal){
						throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("已送数据删除失败")); 
					}				
				}
			}

		}while(FindNextFile(hp, &info)); 
		FindClose(hp);
		RemoveDirectoryW(folderPath);
	}
	catch(...){
		throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("已送数据删除失败")); 
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       二重化未送数据

@param      (i)DATA_TYPE_CODE dataType	数据类型
@param      (i)CString strDataPath				主数据目录
@param      (i)CString strBackupPath			备份数据目录

@retval      unsigned int: 指定目录下文件夹的数量

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void ResumeUnsendData(DATA_TYPE_CODE dataType,CString strDataPath,CString strBackupPath)
{
	
	CString m_dataPath = strDataPath;
	CString m_BackupPath = strBackupPath;

	CString strUnsendDir( _T(""));
	CString strUnsendDirBackup( _T(""));
	switch(dataType)
	{
		case ACC_TRANS:	//ACC交易数据
			strUnsendDir = m_dataPath + ACC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + ACC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case AFC_TRANS://AFC交易数据
			strUnsendDir = m_dataPath + AFC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + AFC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case ECT_TRANS://ECT交易数据
			strUnsendDir = m_dataPath + ECT_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + ECT_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case BUS_DATA://业务数据
			strUnsendDir = m_dataPath + BUS_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + BUS_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case EVEN_DATA://事件数据
			strUnsendDir = m_dataPath + EVN_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + EVN_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		default:
			return;
	}

	{
		queue<CString> queUnsendFile;
		GetUnsendFileName(strUnsendDir,queUnsendFile);
		CString strTemp =  _T("");
		while(!queUnsendFile.empty()){
			strTemp = queUnsendFile.front();
			if(!IsFileExist(strUnsendDirBackup + strTemp)){
				for(int i=0;i<3;i++)
				{
					BOOL copyFileSuccess = CopyFile(strUnsendDir + strTemp,strUnsendDirBackup + strTemp,FALSE);
					if(copyFileSuccess)
					{
						break;
					}
					::Sleep(100);
				}
			}
			queUnsendFile.pop();
		}
	}

	{
		queue<CString> queUnsendFile;
		GetUnsendFileName(strUnsendDirBackup,queUnsendFile);
		CString strTemp =  _T("");
		while(!queUnsendFile.empty()){
			strTemp = queUnsendFile.front();
			if(!IsFileExist(strUnsendDir + strTemp)){
				for(int i=0;i<3;i++)
				{
					BOOL copyFileSuccess = CopyFile(strUnsendDirBackup + strTemp,strUnsendDir + strTemp,FALSE);
					if(copyFileSuccess)
					{
						break;
					}
					::Sleep(100);
				}
			}
			queUnsendFile.pop();
		}
	}
}