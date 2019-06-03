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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CParamHelp::CParamHelp()
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
CParamHelp::~CParamHelp()
{	
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ���ļ�����Э��ͷ����(�����ļ�)

@param      (i)CString sFileName	��������ļ�����·��
@param      (i)bool isCrcChk		�Ƿ�Ԥ�Ƚ���CRCУ��

@retval     WORD   0:�ɹ�  1:CRCУ�����  2:�ļ���ȡʧ��  3:�ļ�д��ʧ�� 4:�ļ���ʽ����

@exception  ��
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
						theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">>%s, У��λ��֤ʧ�ܡ�"), sFileName);
						ret = 1;
					}

					file.Close();
				}
			}
			else
			{
				theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("DeleteTransHead"), __LINE__, _T(">>%s, û�����Э��ͷ��Ϣ"), sFileName);
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
@brief      ����ļ�����Э��ͷ����(���ݰ��ļ�)

@param      (i)CString sFileName	��������ļ�����·��
@param      (i)bool isCrcChk		�Ƿ����CRCУ��

@retval     WORD   0:�ɹ�  1:�ļ���ȡʧ��  2:�ļ�д��ʧ��

@exception  ��
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
			//�ж��Ƿ��Ѿ������Э��ͷ��Ϣ��
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
				//ͷ��Ϣ��
				int nMsgLen = cnHeaderLen + dwDataSize  + 1; //��Ϣ���峤�ȣ��������ֶγ��ȡ���ʼ��ǡ���β��ǣ�ȡֵ��Χ��40~ 8K
				if(isCrcChk) nMsgLen+=4;
				BYTE byHeaderInfo[39];
				memset(byHeaderInfo, 0xFF, sizeof(byHeaderInfo));

				BYTE *pTmp = byHeaderInfo;

				*pTmp++ = 0xF0; //��ʼ��� 0xF0;

				*((DWORD*)(pTmp)) = nMsgLen;
				pTmp += 4;		//��Ϣ�ܳ���

				*((DWORD*)(pTmp)) = 0x01; //Э���ʶ
				pTmp += 4;

				*pTmp++ = 0x01; //Э��汾��
				*pTmp++ = 0x01; //���ݸ�ʽ�汾��
				*pTmp++ = byTranType;//���ݴ�������

				++pTmp; //Ԥ��

				//��ϢΨһ��ʶ
				*pTmp++ = 0x03; //�����������;

				Time2BCD(ComGetCurTime(), pTmp); //��Ϣʱ���
				pTmp += 7;

				*((WORD*)(pTmp)) = 1; //	��Ϣ���к�
				pTmp += 2;

				//�豸ID 2BCD+1BIN+1BIN
				theMAINTENANCE_INFO.GetCurrentDevice().Serialize(pTmp);
				pTmp+=4;

				//�豸�������
				*pTmp++ = (BYTE)(theMAINTENANCE_INFO.GetGroupNumber());
				*pTmp++ = (BYTE)(theMAINTENANCE_INFO.GetEquNumberInGroup());

				*((WORD*)(pTmp)) = 0; //�Ự��������
				pTmp += 2;

				*pTmp++ = 0x01; //�ְ�����
				*pTmp++ = 0x01; //�ְ����

				*pTmp++ = 0;	//ǰ�ô����ʶ
				*pTmp++ = 0;	//�����㷨
				*pTmp++ = 0;	//�����㷨��λ����
				*pTmp++ = 0;	//��Կ�汾
				*pTmp++ = (isCrcChk) ? 0x01 : 0; //У�����㷨

				++pTmp; //Ԥ���ֶ�2


				//д�ŵ��ļ����ݡ�
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
			theDEBUG_LOG->WriteData(_T("CTransportHelp"), _T("AddTransHead"), __LINE__, _T("���ԭʼ����ʧ�ܡ�"));
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
@brief      ���ļ��еõ���Ϣ

@param      (i)LPCTSTR pstrFile	    ��������ļ�����·��
@param      (o)BYTE **pbyData		��ȡ������
@param      (o)BYTE **pbyData		��ȡ���ݵĳ���

@retval     bool  

@exception  ��
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

