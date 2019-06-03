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
// @brief      Ŀ¼�Ƿ����
//
// @param      (i)CString  strPath    Ŀ¼
//
// @retval     bool
//             true:Ŀ¼����  false:Ŀ¼�������
//
// @exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool IsFolderExist(CString strPath)
{
	// Ŀ¼�Ƿ����
	if (strPath ==  _T("")) {
		return false;
	}
	long lFolderAttribute = GetFileAttributes(strPath);
	if (lFolderAttribute == INVALID_FILE_ATTRIBUTES) {
		return false;
	} else {
		// ��Ŀ¼
		if (lFolderAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			return true;
		// ����Ŀ¼
		} else {
			return false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
///**
// @brief      �ļ��Ƿ����
//
// @param      (i)CString  strPath    �ļ�
//
// @retval     bool
//             true:�ļ�����  false:�ļ��������
//
// @exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
bool IsFileExist(CString strFile)
{
	// �ļ��Ƿ����
	if (strFile ==  _T("")) {
		return false;
	}
	long lFileAttribute = GetFileAttributes(strFile);
	if (lFileAttribute == INVALID_FILE_ATTRIBUTES) {
		return false;
	} else {
		// �����ļ�
		if (lFileAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			return false;
		// ���ļ�
		} else {
			return true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
/*
 @brief		��ȡ���ݰ����� 

 @param     (i)CString		pekgName	���ݰ�ID

 @retval    CString ���ݰ��������

 @exception  ��
*/
////////////////////////////////////////////////////////////////////////////
CString GetPekgDate(CString pekgName)
{
	return pekgName.Mid(10,8);
}

////////////////////////////////////////////////////////////////////////////
/*
 @brief		��ȡ�������� 

 @param     ��

 @retval    CString ��������

 @exception  ��
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
// @brief      �½�Ŀ¼
//
// @param      (i)CString		strPath		Ŀ¼
//
// @retval     bool
//             true:�½��ɹ�  false:�½�ʧ��
//
// @exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CreatePath(CString strPath)
{
	CStringArray strArrPath;
	BOOL bSucess = FALSE;
	int iLoop = 0;
	CString strTemp =  _T("");
	
	// Ŀ¼����
	if (IsFolderExist(strPath))	{
		return true;
	}
	strArrPath.RemoveAll();

	for(iLoop = 0; iLoop < strPath.GetLength(); iLoop++) {
		// һ��Ŀ¼δ����
		if (strPath.GetAt(iLoop) != '\\') {
			strTemp += strPath.GetAt(iLoop);
		// һ��Ŀ¼����
		} else {
			strArrPath.Add(strTemp);
			strTemp += _T("\\");
		}
		// ������һ��Ŀ¼
		if (iLoop == strPath.GetLength()-1) {
			strArrPath.Add(strTemp);
		}
	}

	for(iLoop = 1; iLoop < strArrPath.GetSize(); iLoop++) {
		strTemp = strArrPath.GetAt(iLoop);
		bSucess = CreateDirectory(strTemp, NULL);
		// Ŀ¼�½��ɹ�
		if (bSucess) {
			SetFileAttributes(strTemp, FILE_ATTRIBUTE_NORMAL);
		// Ŀ¼�½�ʧ�ܻ���Ŀ¼�Ѿ�����
		} else {
			// Ŀ¼�½�ʧ��
			if (!IsFolderExist(strTemp)) {
				return false;
			}
		}
	}
	strArrPath.RemoveAll();
	// Ŀ¼�½��ɹ�
	if (IsFolderExist(strPath)) {
		return true;
	// Ŀ¼�½�ʧ��
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////
/*
@brief      �������ݰ����ļ���

@param      (i)CString strFileName						���ݰ��ļ���
@param      (i)LPBYTE dataBuff							���ݰ�����
@param      (i)DWORD datalen			

@retval     bool  true:����ɹ�  false:����ʧ��

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
		// ���ļ�
		if(!cfFile.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException =  _T("�쳣��"); 
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
				throw CTxnDataException(CTxnDataException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("�����ڴ�ʧ��")); 
			} 
			memcpy(bFileData,dataBuff, datalen);
			// д���ļ�
			try { 
				cfFile.Write(bFileData, datalen); 
				cfFile.Flush();
			} 
			catch (...) { 
				free(bFileData); 
				bFileData = NULL; 
				cfFile.Close(); 
				bOpen = false; 
				throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("�ļ�д���쳣��" + strFileName)); 
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
			throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__, _T("�ļ�д���쳣��" + strFileName)); 
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
		strException = "�쳣��"; 
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
@brief      ��ȡ���ݰ��ļ�

@param      (i)CString strFileName						���ݰ��ļ���
@param      (o)PEKG_DATA& pekgData						��ǰ���ݰ�����

@retval     ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
void ReadPekgFile(CString strFileName,PEKG_DATA& pekgData)
{
	// ��ȡ�ļ���ʼ		
	unsigned int uiFileLength = 0; 
	CFile cfFile; 
	CFileException cfeError; 
	bool bOpen = false; 
	SYNC(FILE,strFileName);
	try { 
		// ���ļ�
		if(!cfFile.Open(strFileName, CFile::modeRead | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = strCause; 
			throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, _T("�ļ�<")+ strFileName + _T(">��ʧ��:") + strException); 
		} 
		bOpen = true; 
		cfFile.SeekToBegin(); 
		uiFileLength = (unsigned int)cfFile.GetLength(); 
		if (uiFileLength > 0) { 
			pekgData.lpData = (byte *)malloc(uiFileLength); 
			if (pekgData.lpData == NULL) { 
				cfFile.Close(); 
				bOpen = false; 
				throw CTxnDataException(CTxnDataException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("�����ڴ�ʧ��")); 
			} 
			memset(pekgData.lpData, 0x00, uiFileLength); 
			// ��ȡ�ļ�
			if (cfFile.Read(pekgData.lpData, uiFileLength) != uiFileLength) { 
				cfFile.Close(); 
				bOpen = false; 				
				throw CTxnDataException(CTxnDataException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣��" + strFileName)); 
			} 
			pekgData.nLen = uiFileLength;
			cfFile.Close(); 
			bOpen = false;
		} 
		else { 
			cfFile.Close(); 
			bOpen = false; 
			throw CTxnDataException(CTxnDataException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣��" + strFileName)); 
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
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣��" + strFileName)); 
	}
}
 
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡָ��Ŀ¼�µ��������ݰ��ļ���(��ʱ������)

@param       (o)�����ļ���

@retval      bool true: ��ȡ�ɹ�  false����ȡʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void GetUnsendFileName(CString strDir,queue<CString>& fileNames)
{
	try{
		// ȡ���ļ�Ŀ¼
		CString sPath = strDir;

		// �����ļ�
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*"));

		// ���û���ҵ��ļ����͹ر��ļ�
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
				// ȡ���ļ��������ڱȽ�����
				sFileName = finder.GetFileName(); 
				fileNames.push(sFileName);				
			}
		}
		finder.Close();
	} 
	catch(CSysException&) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ȡδ���ļ�������")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ȡδ���ļ�������")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��õ�ǰδ�����ݴ���������

@param       (i)CString strDir  δ������Ŀ¼
@param       (o)�����ļ���

@retval      int ��ǰδ�����ݴ���������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int GetUnsendDayCount(CString strDir)
{
	try{
		map<CString,CString> mapNotSend;
		// ȡ���ļ�Ŀ¼
		CString sPath = strDir;

		// �����ļ�
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*")+ _T(".*"));

		// ���û���ҵ��ļ����͹ر��ļ�
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
				// ȡ���ļ��������ڱȽ�����
				sFileName = finder.GetFileName(); 
				mapNotSend[GetPekgDate(sFileName)] = sFileName;
			}
		}
		finder.Close();
		return (unsigned int)mapNotSend.size();
	} 
	catch(CSysException&) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("���δ�����ݴ���������")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("���δ�����ݴ���������")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ȡ��INIδ������

 @param      (o)int			iUnSendDataMaxDay		δ�����������������

 @retval     ��

 @exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void GetUnsendDay(unsigned int& iUnSendDataMaxDay)
//{
//	iUnSendDataMaxDay = theTVM_INFO.GetLimitedDaysOfUnSendData();
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ȡ��INI��������

 @param      (o)int			iSendedDataMaxDay		�������������������

 @retval     ��

 @exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void GetSendedDay(unsigned int& iSendedDataMaxDay)
//{
//	iSendedDataMaxDay=theTVM_INFO.GetLimitedDaysOfSendData();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡָ��Ŀ¼���ļ��е�����

@param       (i)CString strDir					ָ��Ŀ¼
@param       (o)vector<CString> tempDirNames    Ŀ¼�µ��ļ���

@retval      unsigned int: ָ��Ŀ¼���ļ��е�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int GetDirectoryCount(CString strDir,vector<CString>& tempDirNames)
{
	try{
		unsigned int ret = 0;
		// ȡ���ļ�Ŀ¼
		CString sPath = strDir;

		// �����ļ�
		CFileFind finder;
		CString sFileName =  _T("");
		BOOL bContinue = finder.FindFile(sPath + _T("*")+ _T(".*"));

		// ���û���ҵ��ļ����͹ر��ļ�
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
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ȡδ���ļ�������")); 
	} 
	catch (...) { 	
		throw CTxnDataException(CTxnDataException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ȡδ���ļ�������")); 
	} 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ɾ��Ŀ¼��Ŀ¼����������

@param       (i)CString strDir					ָ��Ŀ¼

@retval      unsigned int: ָ��Ŀ¼���ļ��е�����

@exception   ��
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
				// �ļ��д���
				if((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{
					SYNC(FOLDER,folderPath);
					CString subFolder = folderPath;
					subFolder.Append( _T("\\"));
					subFolder.Append(info.cFileName);
					Removecate(subFolder);
					RemoveDirectoryW(subFolder);
				}
				// �ļ�����
				else
				{
					SYNC(FILE,fileFound);
					swprintf_s(fileFound,1024, _T("%s\\%s"), folderPath, info.cFileName);
					// ȡ���ļ�ֻ������
					SetFileAttributes(fileFound,FILE_ATTRIBUTE_NORMAL); 					
					BOOL retVal = DeleteFile(fileFound);
					if(!retVal){
						throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("��������ɾ��ʧ��")); 
					}				
				}
			}

		}while(FindNextFile(hp, &info)); 
		FindClose(hp);
		RemoveDirectoryW(folderPath);
	}
	catch(...){
		throw CTxnDataException(CTxnDataException::FILE_DELETE_FAIL, _T(__FILE__), __LINE__, _T("��������ɾ��ʧ��")); 
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ػ�δ������

@param      (i)DATA_TYPE_CODE dataType	��������
@param      (i)CString strDataPath				������Ŀ¼
@param      (i)CString strBackupPath			��������Ŀ¼

@retval      unsigned int: ָ��Ŀ¼���ļ��е�����

@exception   ��
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
		case ACC_TRANS:	//ACC��������
			strUnsendDir = m_dataPath + ACC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + ACC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case AFC_TRANS://AFC��������
			strUnsendDir = m_dataPath + AFC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + AFC_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case ECT_TRANS://ECT��������
			strUnsendDir = m_dataPath + ECT_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + ECT_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case BUS_DATA://ҵ������
			strUnsendDir = m_dataPath + BUS_TXN_FILE_PATH + UNSEND_FILE_PATH;
			strUnsendDirBackup = m_BackupPath + BUS_TXN_FILE_PATH + UNSEND_FILE_PATH;
			break;

		case EVEN_DATA://�¼�����
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