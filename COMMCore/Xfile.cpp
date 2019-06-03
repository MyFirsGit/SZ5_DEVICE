#include "StdAfx.h"
#include "xfile.h"
#include "LogException.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SUMֵ����
static const int SUM_LEN = 4;


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CString fileName   	�ļ���

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::CXFile(CString fileName)
: m_fileName(fileName)
{
	OpenFile(fileName);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CString fileName   	    �ļ���
@param      (i)CString backupFileName	�����ļ���

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::CXFile(CString fileName, CString backupFileName)
	: m_fileName(fileName)
	, m_backfileName(backupFileName)
{
	if (backupFileName == "") {
		m_hasBackup = false;
	}
	else {
		m_hasBackup = true;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CXFile::~CXFile()
{
	this->Close();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݵ�У���(SUM)

@param      (i)const Char* pData		����SAM������
@param 		(i)const int nSize			���ݳ���
@param 		(o)Char* pSum				����У���

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::CalcSum(const char* pData, const int nSize, char* pSum)
{
	try {
		int sum = 0;

		for(int k = 0; k < nSize; k++) {
			sum += (BYTE)pData[k];
		}

		memcpy(pSum, (char*)(&sum), SUM_LEN);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪶������ļ�(д�ļ���)

@param      (i)fileName	�ļ���

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenFile(CString& fileName)
{
	try	{
		// �����ļ���
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// �ж�Ŀ¼�Ƿ����, ��������ھ�Ҫ����
		CString strPathDir = fileName.Left(fileName.ReverseFind('\\'));
		if(!ComCheckAndCreateDirectory(strPathDir))
			throw CLogException(CLogException::FILE_NOT_EXIST,_T(__FILE__),__LINE__,fileName);

		// ȥ��ֻ������
		DWORD fileAttributes = GetFileAttributes(fileName);
		if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
			if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
				SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
			}
		}

		// ���ļ�
		CFileException e;
		if (! this->Open((fileName), 
			CFile::modeCreate
			| CFile::modeNoTruncate
			| CFile::modeReadWrite
			| CFile::typeBinary
			| CFile::shareDenyNone,&e)) {
				CString errCode = _T("");
				errCode.Format(_T(" errCode = %d"),e.m_cause);
				throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName + errCode);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...)
	{
		throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      дASCII�ļ�

@param      (i)sData	д�������

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFile(CString sData)
{
	try {
#ifdef _UNICODE
		this->SeekToBegin();
		BYTE unicodeFlag[] = {0xFF,0xFE};
		this->Write(unicodeFlag,2);
		this->SeekToEnd();
#endif // _UNICODE
		sData+=_T("\r\n");
		this->Write(sData.GetBuffer(), sData.GetLength()*sizeof(TCHAR));
		sData.ReleaseBuffer();
		this->Flush();

		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      д�������ļ�

@param      (i)lpData	д�������
@param 	  (i)nLen	д������ݴ�С

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFile(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		char sumBuf[SUM_LEN];
		CalcSum(lpData, nLen, sumBuf);			// �������ݵ�У���

		// ����ļ�
		this->SetLength(0);
		this->Write(sumBuf, SUM_LEN);				// д��У���
		this->Write(lpData, nLen);			// д������
		this->Close();

		// �����ļ�
		BOOL copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
		if(!copyFileSuccess)
		{
			for(int i=0;i<2;i++)
			{
				::Sleep(100);
				copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
				if(copyFileSuccess)
				{
					break;
				}
			}
		}
		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__,m_fileName);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�������ļ����޶��ػ���

@param      (i)CString fileName:	        �ļ���
@param	  (o)VARIABLE_DATA& data:		�������ݻ�����
@param	  (i)hasSum:	                �������Ƿ���sumֵ

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::ReadBinFile(CString fileName, VARIABLE_DATA& data, bool hasSum)
{
	try {
		if (data.lpData != NULL) {
			delete []  data.lpData;
			data.lpData = NULL;
		}

		data.nLen = 0;

		// �ļ��򿪳���
		OpenBinFileRead(fileName);

		vector<unsigned char> vec_data;
		unsigned char sHeader[SUM_LEN] = {0};
		unsigned char ch;
		ULONG newSum = 0;
		int n = 0;

		if (hasSum) {
			while (this->Read(&ch, 1)) {
				if (n < SUM_LEN) {
					sHeader[n] = ch;            // ��sumֵȡ������
				}
				else {
					vec_data.push_back(ch);     // ��ʵ��ֵȡ������
					newSum += ch;			    // ���¼������ݵ�У���
				}
				n++;
			}

			// ����ļ���Ϊ��
			if (vec_data.size() > 0) {
				ULONG oldSum = *(ULONG*)sHeader;

				// У��ͼ���
				if (oldSum != newSum) {
					this->Close();
					throw CLogException(CLogException::FILE_SAM_ERR, _T(__FILE__), __LINE__, fileName);
				}



				data.nLen = static_cast<ULONG>(vec_data.size());
				data.lpData = new BYTE[data.nLen];
				memcpy(data.lpData, (unsigned char*)&vec_data[0], data.nLen);

				vec_data.clear();
			}
			// ����ļ�Ϊ��
			else {
				data.nLen = 0;
				data.lpData = NULL;
				this->Close();
				throw CLogException(CLogException::FILE_NOT_EXIST, _T(__FILE__), __LINE__, fileName);
			}
		}
		else {
			data.nLen = static_cast<ULONG>(this->GetLength());
			data.lpData = new BYTE[data.nLen];
			this->Read(data.lpData, data.nLen);
		}			

		this->Close();
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, fileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�������ļ����޶��ػ���

@param      (i)CString fileName:	        �ļ���
@param	    (o)VARIABLE_DATA& data:		�������ݻ�����

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::ReadBinFileWithoutSUM(VARIABLE_DATA& data)
{
	ReadBinFile(m_fileName, data, false);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      д�������ļ�(����SUMֵ���ޱ����ļ�)

@param      (i)lpData	д�������
@param 	    (i)nLen	д������ݴ�С

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFileWithoutSUM(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		// ����ļ�
		if (NULL != lpData || 0 != nLen) {      // ������Ϊ�ղ�д����
			this->SetLength(0);
			this->Write(lpData, nLen);			// д������
			this->Close();
		}

		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д�������ļ�

@param      (i)lpData	д�������
@param 	  (i)nLen	д������ݴ�С

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteBinFile(const char* lpData, const ULONG nLen,bool hasSum)
{
	try {
		this->SetLength(0);
		if(hasSum)
		{
			char sumBuf[SUM_LEN];
			CalcSum(lpData, nLen, sumBuf);			// �������ݵ�У���
			this->Write(sumBuf, SUM_LEN);				// д��У���
		}

		this->Write(lpData, nLen);			// д������
		this->Flush();
		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__,m_fileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���������ļ�   

@param	

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::ReadBinFile(VARIABLE_DATA& data,bool hasSum)
{
	try {
		if (data.lpData != NULL) {
			delete []  data.lpData;
			data.lpData = NULL;
		}
		data.nLen = 0;

		int atLeastLength = hasSum?SUM_LEN:0;
		int wholeLength = this->GetLength();
		int contentLength = wholeLength-atLeastLength;
		if(contentLength<0)
		{
			throw CLogException(CLogException::FILE_READ_FAIL, _T(__FILE__), __LINE__, m_fileName);
		}
		char oldSum[SUM_LEN] = {0};
		if (hasSum) 
		{
			Read(oldSum,SUM_LEN);
		}
		data.lpData = new BYTE[contentLength];
		memset(data.lpData,0,contentLength);
		Read(data.lpData,contentLength);
		data.nLen = contentLength;
		if(hasSum)
		{
			//ULONG oldSum = *(ULONG*)sHeader;
			char newSum[SUM_LEN] = {0};
			CalcSum((const char*)data.lpData,data.nLen,newSum);
			if(memcmp(oldSum,newSum,SUM_LEN)!=0)
			{
				delete data.lpData;
				data.lpData = NULL;
				data.nLen = 0;
				throw CLogException(CLogException::FILE_SAM_ERR, _T(__FILE__), __LINE__, m_fileName);
			}
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, m_fileName);
	}
	return FILE_OPERATION_OK;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ��ȡһ���ַ�    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CXFile::ReadString(CString& rString)
{
#ifdef _UNICODE
	if(m_pStream && GetPosition()==0)
	{
		char unicodeStartFlag[]={0xFF,0xFE};
		Read(unicodeStartFlag,2);
	}
#endif
	return __super::ReadString(rString);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪶������ļ�(д�ļ���)

@param      (i)fileName	�ļ���

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenBinFile(CString fileName)
{
	try	{
		// �����ļ���
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// ȥ��ֻ������
		DWORD fileAttributes = GetFileAttributes(fileName);
		if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
			if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
				SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
			}
		}

		// ���ļ�
		CFileException e;
		if (! this->Open(fileName, 
			CFile::modeCreate
			| CFile::modeNoTruncate
			| CFile::modeReadWrite
			| CFile::typeBinary
			| CFile::shareDenyNone,&e)) {
				CString errCode = _T("");
				errCode.Format(_T(" errCode = %d"),e.m_cause);
				throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName + errCode);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...)
	{
		throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪶������ļ�(���ļ���)

@param      (i)fileName	�ļ���

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenBinFileRead(CString fileName)
{
	try	{
		// �����ļ���
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// �ж��Ƿ�usb
		bool isUSB = false;
		// ��ȡ�̷�
		char fileDrive = fileName.GetAt(0);
		// �Ƚ��ļ��̷���usb�̷��Ƿ�һ��
		if (fileDrive == 'h' || fileDrive == 'H') {
			isUSB = true;
		}

		// ����usb
		if (!isUSB) {
			// ȥ��ֻ������
			DWORD fileAttributes = GetFileAttributes(fileName);
			if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
				if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
					SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
				}
			}

			// ���ļ�
			CFileException e;
			if (! this->Open(fileName, 
				CFile::modeCreate
				| CFile::modeNoTruncate
				| CFile::modeRead
				| CFile::typeBinary
				| CFile::shareDenyNone,&e)) {
					CString errCode = _T("");
					errCode.Format(_T(" errCode = %d"),e.m_cause);
					throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName + errCode);
			}
		}
		// usb
		else {
			// ���ļ�
			CFileException e;
			if (! this->Open(fileName, 
				CFile::modeNoTruncate
				| CFile::modeRead
				| CFile::typeBinary
				| CFile::shareDenyNone,&e)) {
					CString errCode = _T("");
					errCode.Format(_T(" errCode = %d"),e.m_cause);
					throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName + errCode);
			}
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...)
	{
		throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, fileName);
	}
}

CXFile::ERR_DETAITL CXFile::WriteBinFileWithCrc(VARIABLE_DATA& data)
{
	try 
	{
		OpenBinFile(m_fileName);
		unsigned long crc = Crc32(data.lpData,data.nLen,0);
		this->SetLength(0);
		this->Write(&crc, sizeof(unsigned long));				// д��У���
		this->Write(data.lpData, data.nLen);			// д������
		this->Close();

		// �����ļ�
		if(m_backfileName!="")
		{

			BOOL copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
			if(!copyFileSuccess)
			{
				for(int i=0;i<2;i++)
				{
					::Sleep(100);
					copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
					if(copyFileSuccess)
					{
						break;
					}
				}
			}
		}
		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__,m_fileName);
	}
}
void CXFile::ReadBinFileWithCrc(CString fileName,VARIABLE_DATA& data)
{
	try {
		if (data.lpData != NULL) {
			delete []  data.lpData;
			data.lpData = NULL;
		}
		data.nLen = 0;
		// �ļ��򿪳���
		OpenBinFileRead(fileName);
		unsigned long fileLength = this->GetLength();
		if(fileLength<sizeof(unsigned long))//С��crc�ĳ���
		{
			throw CLogException(CLogException::FILE_CRC_ERR, _T(__FILE__), __LINE__, fileName);
		}
		this->SeekToBegin();
		unsigned long savedCrc;
		this->Read(&savedCrc,sizeof(unsigned long));
		unsigned long contentLength = fileLength - sizeof(unsigned long);
		data.lpData = new BYTE[contentLength];
		data.nLen = contentLength;
		this->Read(data.lpData,contentLength);
		unsigned long expectedCrc = Crc32(data.lpData,contentLength,0);
		if(expectedCrc!=savedCrc)
		{
			throw CLogException(CLogException::FILE_CRC_ERR, _T(__FILE__), __LINE__, fileName);
		}
		this->Close();
	}
	catch(CSysException&) {
		this->Close();
		throw;
	}
	catch (...) {
		this->Close();
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, fileName);
	}
}

CXFile::ERR_DETAITL CXFile::ReadBinFileWithCrc(VARIABLE_DATA& data)
{

	enum FileResult{
		NO_FILE = 0,        // ���ļ�
		WRONG_FILE,         // �ļ�������
		RIGHT_FILE          // �ļ����ͨ��
	};
	FileResult fileResult = RIGHT_FILE;
	try {
		ReadBinFileWithCrc(m_fileName, data);
	}
	catch (CSysException& e) {
		fileResult =  e.GetDetailID() == CLogException::FILE_NOT_EXIST ? NO_FILE: WRONG_FILE;
	}
	catch (...) {               
		fileResult = WRONG_FILE;
	}
	// ���ļ���ȡ����
	if ( RIGHT_FILE == fileResult ) {
		BOOL copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
		if(!copyFileSuccess)
		{
			for(int i=0;i<2;i++)
			{
				::Sleep(100);
				copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
				if(copyFileSuccess)
				{
					break;
				}
			}
		}
		return FILE_OPERATION_OK;
	}
	try {
		ReadBinFileWithCrc(m_backfileName, data);
	}
	catch (CSysException& e) {
		throw;
	}
	BOOL copyFileSuccess = ::CopyFile(m_backfileName, m_fileName, FALSE);
	if(!copyFileSuccess)
	{
		for(int i=0;i<2;i++)
		{
			::Sleep(100);
			copyFileSuccess = ::CopyFile(m_backfileName, m_fileName, FALSE);
			if(copyFileSuccess)
			{
				break;
			}
		}
	}
	return FILE_OPERATION_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ն������ļ�

@param      none

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::ClearBinFile()
{
	this->OpenBinFile(m_fileName);
	this->SetLength(0);
	this->Close();

	this->OpenBinFile(m_backfileName);
	this->SetLength(0);
	this->Close();

	return FILE_OPERATION_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ���ļ�

@param      void

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::DelBinFile()
{
	if (!(DeleteFile(m_fileName) && DeleteFile(m_backfileName))) {
		throw CLogException(CLogException::FILE_DEL_FAIL, _T(__FILE__), __LINE__);
	}

	return FILE_OPERATION_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      д�������ļ�(�������ļ�)

@param      (i)lpData	д�������
@param 	  (i)nLen	д������ݴ�С

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFileNoCopy(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		char sumBuf[SUM_LEN];
		CalcSum(lpData, nLen, sumBuf);			// �������ݵ�У���

		// ����ļ�
		this->SetLength(0);
		this->Write(sumBuf, SUM_LEN);				// д��У���
		this->Write(lpData, nLen);			// д������
		this->Close();

		//// �����ļ�
		//::CopyFile(_T(m_fileName), _T(m_backfileName), FALSE);
		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ƶ������ļ�

@param      none

@retval     0:�ɹ� ��0���쳣

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::CopyXFile()
{
	try{
		// �����ļ�
		if (m_hasBackup) {
			BOOL copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
			if(!copyFileSuccess)
			{
				for(int i=0;i<2;i++)
				{
					::Sleep(100);
					copyFileSuccess = ::CopyFile(m_fileName, m_backfileName, FALSE);
					if(copyFileSuccess)
					{
						break;
					}
				}
			}
		}
		return FILE_OPERATION_OK;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
