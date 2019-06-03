#include "StdAfx.h"
#include "CParamHelp.h"
#include "CParamException.h"
#include "Util.h"
#include "MaintenanceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParamHelp::CParamHelp()
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParamHelp::~CParamHelp()
{	
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      删除文件传输协议头部分(参数文件)

@param      (i)CString sFileName	待处理的文件绝对路径
@param      (i)bool isCrcChk		是否预先进行CRC校验

@retval     WORD   0:成功  1:CRC校验错误  2:文件读取失败  3:文件写入失败 4:文件格式错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

const int cnHeaderLen = 39; 
WORD CParamHelp::DeleteTransHead(CString sFileName,bool isCrcChk)
{
	theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T("<< %s"), sFileName);
	WORD ret = 0;
	if (_ACCESS(sFileName, 0) == -1)
	{
		theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">>%s : not exist"), sFileName);
		return 2;
	}

	try
	{
		BYTE *pbyTxt = NULL;
		DWORD dwTxtLen = 0;
		if (GetSrcDataFromFile(sFileName, &pbyTxt, dwTxtLen))
		{
			if(pbyTxt[0] == 0xF0 && pbyTxt[dwTxtLen-1] == 0xFF)
			{
				CFile::Remove(sFileName);
				CFile file;
				if (file.Open(sFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
				{
					bool bHaveCrcCheck = true;
					if (pbyTxt[37] == 0)
						bHaveCrcCheck = false;

					bool bCanDel = true;
					int nOffPos =  (pbyTxt[33] == 0) ? cnHeaderLen : cnHeaderLen + 32;
					int nDataLen = dwTxtLen - nOffPos - ((bHaveCrcCheck) ? 4 : 0) - 1;
					if (isCrcChk && bHaveCrcCheck)
					{
						DWORD dwCurCrc32 = mltol(Crc32(pbyTxt+nOffPos, nDataLen,0));
						DWORD dwSrcCrc32 = ComMakeLong(pbyTxt[dwTxtLen-5],pbyTxt[dwTxtLen-4],pbyTxt[dwTxtLen-3],pbyTxt[dwTxtLen-2]);
						if (dwCurCrc32 != dwSrcCrc32)
						{
							bCanDel = false;
						}
					}

					if (bCanDel)
					{
						file.SeekToBegin();
						file.Write(pbyTxt+nOffPos, nDataLen);
					}
					else
					{
						theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">>%s, 校验位验证失败。"), sFileName);
						ret = 1;
					}

					file.Close();
				}
			}
			else
			{
				theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">>%s, 没有添加协议头信息"), sFileName);
				//ret = 0;
			}
		}
		else
		{
			ret = 1;
		}

		if(pbyTxt!=NULL){
			delete [](pbyTxt);
			(pbyTxt)=NULL;
		}
	}
	catch (CFileException e)
	{
		TCHAR szError[1024];
		e.GetErrorMessage(szError, 1024);
		CString strError = szError;
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__, strError);
	}
	catch(...)
	{
		throw;
	}
	theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">> %s"), sFileName);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      添加文件传输协议头部分(数据包文件)

@param      (i)CString sFileName	待处理的文件绝对路径
@param      (i)bool isCrcChk		是否进行CRC校验

@retval     WORD   0:成功  1:文件读取失败  2:文件写入失败

@exception  无
*/
/////////////////////////////////////////////////////////////////////////
WORD CParamHelp::AddTransHead(CString sFileName, BYTE byTranType, bool isCrcChk)
{
	theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("AddTransHead"), __LINE__, _T("<< %s"), sFileName);
	WORD ret = 0;
	if (_ACCESS(sFileName, 0) == -1)
	{
		theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("AddTransHead"), __LINE__, _T("%s : not exist"), sFileName);
		return 1;
	}
	try
	{
		BYTE *pbyDataSeg = NULL;
		DWORD dwDataSize = 0;
		if (GetSrcDataFromFile(sFileName, &pbyDataSeg, dwDataSize))
		{
			//判断是否已经添加了协议头信息。
			if (pbyDataSeg[0] == 0xF0 && pbyDataSeg[dwDataSize-1] == 0xFF)
			{
				if(pbyDataSeg!=NULL){
					delete [](pbyDataSeg);
					(pbyDataSeg)=NULL;
				}
				return 0;
			}

			CFile::Remove(sFileName);
			CFile file;
			if (file.Open(sFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				//头信息。
				int nMsgLen = cnHeaderLen + dwDataSize  + 1; //消息整体长度：包含本字段长度、开始标记、结尾标记，取值范围：40~ 8K
				if(isCrcChk) nMsgLen+=4;
				BYTE byHeaderInfo[39];
				memset(byHeaderInfo, 0xFF, sizeof(byHeaderInfo));

				BYTE *pTmp = byHeaderInfo;

				*pTmp++ = 0xF0; //起始标记 0xF0;

				*((DWORD*)(pTmp)) = nMsgLen;
				pTmp += 4;		//消息总长度

				*((DWORD*)(pTmp)) = 0x01; //协议标识
				pTmp += 4;

				*pTmp++ = 0x01; //协议版本号
				*pTmp++ = 0x01; //数据格式版本号
				*pTmp++ = byTranType;//数据传输类型

				++pTmp; //预留

				//消息唯一标识
				*pTmp++ = 0x03; //传输命令编码;

				Time2BCD(ComGetCurTime(), pTmp); //消息时间戳
				pTmp += 7;

				*((WORD*)(pTmp)) = 1; //	消息序列号
				pTmp += 2;

				//设备ID 2BCD+1BIN+1BIN
				theMAINTENANCE_INFO.GetCurrentDevice().Serialize(pTmp);
				pTmp+=4;

				//设备分组编码
				*pTmp++ = (BYTE)(theMAINTENANCE_INFO.GetGroupNumber());
				*pTmp++ = (BYTE)(theMAINTENANCE_INFO.GetEquNumberInGroup());

				*((WORD*)(pTmp)) = 0; //会话生存周期
				pTmp += 2;

				*pTmp++ = 0x01; //分包总数
				*pTmp++ = 0x01; //分包序号

				*pTmp++ = 0;	//前置处理标识
				*pTmp++ = 0;	//加密算法
				*pTmp++ = 0;	//加密算法补位长度
				*pTmp++ = 0;	//密钥版本
				*pTmp++ = (isCrcChk) ? 0x01 : 0; //校验码算法

				++pTmp; //预留字段2


				//写信的文件数据。
				file.SeekToBegin();
				file.Write(byHeaderInfo, sizeof(byHeaderInfo));

				file.Write(pbyDataSeg, dwDataSize);

				if (isCrcChk)
				{
					DWORD dwCrc32 = Crc32(pbyDataSeg, dwDataSize,0);
					file.Write((BYTE*)(&dwCrc32), sizeof(DWORD));
				}

				BYTE byEndCode = 0xFF;
				file.Write(&byEndCode, 1);
				file.Flush();

				file.Close();
			}
		}
		else
		{
			theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("AddTransHead"), __LINE__, _T("获得原始数据失败。"));
			ret = 2;
		}

		if(pbyDataSeg!=NULL){
			delete [](pbyDataSeg);
			(pbyDataSeg)=NULL;
		}
	}
	catch (CFileException e)
	{
		TCHAR szError[1024];
		e.GetErrorMessage(szError, 1024);
		CString strError = szError;
		theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("AddTransHead"), __LINE__, _T("%s->%s"), sFileName, strError);

		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__, strError);
	}
	catch (...)
	{
		throw;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从文件中得到信息

@param      (i)LPCTSTR pstrFile	    待处理的文件绝对路径
@param      (o)BYTE **pbyData		读取的数据
@param      (o)BYTE **pbyData		读取数据的长度

@retval     bool  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelp::GetSrcDataFromFile(LPCTSTR pstrFile, BYTE **pbyData, DWORD &dwDataLen)
{
	if (pstrFile == NULL || pbyData == NULL)
		return false;

	bool bRes = false;

	CFile file;
	if (file.Open(pstrFile, CFile::modeReadWrite | CFile::typeBinary))
	{
		dwDataLen = file.SeekToEnd();

		*pbyData = new BYTE[dwDataLen];
		BYTE *pbyDataSeg = *pbyData;

		if (pbyDataSeg != NULL)
		{
			file.SeekToBegin();
			{
				file.Read(pbyDataSeg, dwDataLen);
			}
			bRes = true;
		}

		file.Close();
	}

	return bRes;
}

