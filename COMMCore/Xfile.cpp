#include "StdAfx.h"
#include "xfile.h"
#include "LogException.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SUM值长度
static const int SUM_LEN = 4;


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CString fileName   	文件名

@retval     无

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
@brief      构造函数

@param      (i)CString fileName   	    文件名
@param      (i)CString backupFileName	备份文件名

@retval     无

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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CXFile::~CXFile()
{
	this->Close();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计算数据的校验和(SUM)

@param      (i)const Char* pData		进行SAM的数据
@param 		(i)const int nSize			数据长度
@param 		(o)Char* pSum				保存校验和

@retval     0:成功 非0：异常

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
@brief      打开二进制文件(写文件用)

@param      (i)fileName	文件名

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenFile(CString& fileName)
{
	try	{
		// 基本文件打开
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// 判断目录是否存在, 如果不存在就要创建
		CString strPathDir = fileName.Left(fileName.ReverseFind('\\'));
		if(!ComCheckAndCreateDirectory(strPathDir))
			throw CLogException(CLogException::FILE_NOT_EXIST,_T(__FILE__),__LINE__,fileName);

		// 去掉只读属性
		DWORD fileAttributes = GetFileAttributes(fileName);
		if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
			if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
				SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
			}
		}

		// 打开文件
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
@brief      写ASCII文件

@param      (i)sData	写入的数据

@retval     0:成功 非0：异常

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
@brief      写二进制文件

@param      (i)lpData	写入的数据
@param 	  (i)nLen	写入的数据大小

@retval     0:成功 非0：异常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFile(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		char sumBuf[SUM_LEN];
		CalcSum(lpData, nLen, sumBuf);			// 计算数据的校验和

		// 清空文件
		this->SetLength(0);
		this->Write(sumBuf, SUM_LEN);				// 写入校验和
		this->Write(lpData, nLen);			// 写入数据
		this->Close();

		// 备份文件
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
@brief      读取二进制文件（无二重化）

@param      (i)CString fileName:	        文件名
@param	  (o)VARIABLE_DATA& data:		接受数据缓冲区
@param	  (i)hasSum:	                数据中是否有sum值

@retval     无

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

		// 文件打开出错
		OpenBinFileRead(fileName);

		vector<unsigned char> vec_data;
		unsigned char sHeader[SUM_LEN] = {0};
		unsigned char ch;
		ULONG newSum = 0;
		int n = 0;

		if (hasSum) {
			while (this->Read(&ch, 1)) {
				if (n < SUM_LEN) {
					sHeader[n] = ch;            // 将sum值取出保存
				}
				else {
					vec_data.push_back(ch);     // 将实际值取出保存
					newSum += ch;			    // 重新计算数据的校验和
				}
				n++;
			}

			// 如果文件不为空
			if (vec_data.size() > 0) {
				ULONG oldSum = *(ULONG*)sHeader;

				// 校验和检验
				if (oldSum != newSum) {
					this->Close();
					throw CLogException(CLogException::FILE_SAM_ERR, _T(__FILE__), __LINE__, fileName);
				}



				data.nLen = static_cast<ULONG>(vec_data.size());
				data.lpData = new BYTE[data.nLen];
				memcpy(data.lpData, (unsigned char*)&vec_data[0], data.nLen);

				vec_data.clear();
			}
			// 如果文件为空
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
@brief      读取二进制文件（无二重化）

@param      (i)CString fileName:	        文件名
@param	    (o)VARIABLE_DATA& data:		接受数据缓冲区

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::ReadBinFileWithoutSUM(VARIABLE_DATA& data)
{
	ReadBinFile(m_fileName, data, false);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      写二进制文件(不含SUM值，无备份文件)

@param      (i)lpData	写入的数据
@param 	    (i)nLen	写入的数据大小

@retval     0:成功 非0：异常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFileWithoutSUM(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		// 清空文件
		if (NULL != lpData || 0 != nLen) {      // 当数据为空不写数据
			this->SetLength(0);
			this->Write(lpData, nLen);			// 写入数据
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
@brief      写二进制文件

@param      (i)lpData	写入的数据
@param 	  (i)nLen	写入的数据大小

@retval     0:成功 非0：异常

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
			CalcSum(lpData, nLen, sumBuf);			// 计算数据的校验和
			this->Write(sumBuf, SUM_LEN);				// 写入校验和
		}

		this->Write(lpData, nLen);			// 写入数据
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
@brief   读二进制文件   

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
@brief  读取一行字符    

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
@brief      打开二进制文件(写文件用)

@param      (i)fileName	文件名

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenBinFile(CString fileName)
{
	try	{
		// 基本文件打开
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// 去掉只读属性
		DWORD fileAttributes = GetFileAttributes(fileName);
		if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
			if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
				SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
			}
		}

		// 打开文件
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
@brief      打开二进制文件(读文件用)

@param      (i)fileName	文件名

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CXFile::OpenBinFileRead(CString fileName)
{
	try	{
		// 基本文件打开
		if (fileName == "") {
			throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
		}

		// 判断是否usb
		bool isUSB = false;
		// 获取盘符
		char fileDrive = fileName.GetAt(0);
		// 比较文件盘符和usb盘符是否一致
		if (fileDrive == 'h' || fileDrive == 'H') {
			isUSB = true;
		}

		// 不是usb
		if (!isUSB) {
			// 去掉只读属性
			DWORD fileAttributes = GetFileAttributes(fileName);
			if (INVALID_FILE_ATTRIBUTES != fileAttributes) {
				if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
					SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
				}
			}

			// 打开文件
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
			// 打开文件
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
		this->Write(&crc, sizeof(unsigned long));				// 写入校验和
		this->Write(data.lpData, data.nLen);			// 写入数据
		this->Close();

		// 备份文件
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
		// 文件打开出错
		OpenBinFileRead(fileName);
		unsigned long fileLength = this->GetLength();
		if(fileLength<sizeof(unsigned long))//小于crc的长度
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
		NO_FILE = 0,        // 无文件
		WRONG_FILE,         // 文件检查错误
		RIGHT_FILE          // 文件检查通过
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
	// 主文件读取正常
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
@brief      清空二进制文件

@param      none

@retval     0:成功 非0：异常

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
@brief      删除文件

@param      void

@retval     0:成功 非0：异常

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
@brief      写二进制文件(不备份文件)

@param      (i)lpData	写入的数据
@param 	  (i)nLen	写入的数据大小

@retval     0:成功 非0：异常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::WriteFileNoCopy(const char* lpData, const ULONG nLen)
{
	try {
		OpenBinFile(m_fileName);

		char sumBuf[SUM_LEN];
		CalcSum(lpData, nLen, sumBuf);			// 计算数据的校验和

		// 清空文件
		this->SetLength(0);
		this->Write(sumBuf, SUM_LEN);				// 写入校验和
		this->Write(lpData, nLen);			// 写入数据
		this->Close();

		//// 备份文件
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
@brief      复制二进制文件

@param      none

@retval     0:成功 非0：异常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CXFile::ERR_DETAITL CXFile::CopyXFile()
{
	try{
		// 备份文件
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
