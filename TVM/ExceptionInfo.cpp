#include "stdafx.h"
#include "ExceptionInfo.h"
#include "getIni.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const    EXCEPTION_FILE_NAME    = _T("TVM_EXCEPTION.CSV");// �쳣��Ϣ�ļ���

CExceptionInfo  CExceptionInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��
                                                                 
 @param      (i)void

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionInfo::CExceptionInfo(void)
{
	m_uHour = 0;
	m_uMinute = 0;
	m_uSecond = 0;
	m_recoverTimes = 0;
	m_vfareRecoverTimes = 0;

	//TCHAR szAppPath[_MAX_PATH];
	//ComGetAppPath(szAppPath,_MAX_PATH);
	//CString szBOMINIFName = CString(szAppPath) + DEVICE_INI_FILE_NAME;
	//CGetIni GetIni(szBOMINIFName);
	//
	//theTVM_INFO.Get
    //m_maxRecoverTimes = (unsigned long)GetIni.GetDataInt(_T("APPLICATION"), _T("MaxRecoverTimes"), 0);

    m_bNeedRestart = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)void

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionInfo::~CExceptionInfo(void)
{
    Release();
	delete m_expfile; 
	m_expfile = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͷ���Դ

@param      ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::Release()
{
    POSITION pos = NULL;
    LPEXCEPTION_DETAIL  exDetail = NULL;
    CString sKey=_T("");
    pos = m_mapException.GetStartPosition();

    while (pos != NULL) {
        m_mapException.GetNextAssoc(pos, sKey, exDetail);
        delete exDetail;
    }

    m_mapException.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ļ��쳣�ָ�������1

@param       ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::IncRecoverTimes()
{
    m_recoverTimes++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ʊ�۱�ǰ�ָ�����(�ۼ�)

@param       (i) int vfareRecoverTimes �������

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::UpdateVfareRecoverTimes(int vfareRecoverTimes)
{
	m_vfareRecoverTimes = m_vfareRecoverTimes + vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �õ�Ʊ�۱�ǰ�ָ�����

@param       ��

@retval      unsigned long  Ʊ�۱�ǰ�ָ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionInfo::GetCurVfareRecoverTimes()
{
	return m_vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �õ��ļ��������ָ�����

@param       ��

@retval      unsigned long  �ļ��������ָ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionInfo::GetFileMaxRecoverTimes()
{
	return m_maxRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��ļ��쳣�ָ�����

@param      (i)void

@retval      CExceptionInfo& ��CExceptionInfo�����
               
@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionInfo& CExceptionInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ񳬹����ָ�����

@param      (i)void

@retval      bool:
               true:������������false:δ����������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionInfo::OverMaxRecoverTimes()
{
	if (m_recoverTimes > m_maxRecoverTimes) {
		return true;
	}
	else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ񳬹�Ʊ�۱����ָ�����

@param      (i)void

@retval      bool:		\n
				true:������������false:δ����������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionInfo::OverMaxVfareRecoverTimes()
{
	bool ret = false;
	if (m_vfareRecoverTimes > m_maxRecoverTimes) {
		ret = true;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����쳣��Ϣ��ֵ

@param      (i)EXCEPTION_MAP& exMap�����쳣��Ϣ����

@retval      unsigned int: 0:�ɹ�����0:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int CExceptionInfo::GetExceptionDetail()
{
	unsigned int iRet = 0;
	CString strline = _T(""); 
	CString strKey = _T(""); 
	CString strID = _T("");
	CString strDetailID = _T("");
	CString strError = _T("");
	CString strDetail = _T("");

	CString deviceComponentCode = _T(""); 
	CString MTC = _T(""); 
	CString EC = _T(""); 
	
	CString sAppPath;
	CString filename;

	// ��ʼ���쳣�ļ�
	GetAppPath(sAppPath);
	filename = sAppPath + EXCEPTION_FILE_NAME;
	AFC_DEVICE_TYPE deviceType = theMAINTENANCE_INFO.GetDeviceType();
	DEVICE_SUBTYPE deviceSubType = theMAINTENANCE_INFO.GetDeviceSubType();
	long oldComponentId = 0;
	
	CXFile *expfile = new CXFile(filename);
	// ���ж�ȡ�ļ�
	expfile->ReadString(strline);

	DWORD dwtick1=::GetTickCount();
	while(!feof(expfile->m_pStream))   
	{
		expfile->ReadString(strline);
		if(strline.GetLength() == 0){
			continue;
		}

		LPEXCEPTION_DETAIL  pedData = new EXCEPTION_DETAIL;

		TCHAR tcSplitSymbol[]=_T("$");
		TCHAR* tctemp=(TCHAR*)strline.GetBuffer();
		const TCHAR* tcfind=_tcsstr(strline,tcSplitSymbol);
		UINT uiColCount=0;
		TCHAR tcBuffer[2000];	//���Ҷ�Ϊÿ��2000�������ַ�
		while(NULL!=tcfind)
		{
			memset(tcBuffer,'\0',sizeof(TCHAR)*2000);
			int iindex=tcfind-tctemp;
			_tcsncpy(tcBuffer,tctemp,iindex);
			switch(uiColCount)
			{
			case 0:
				pedData->lID = _ttol(tcBuffer);
				strKey = tcBuffer;
				break;
			case 1:
				pedData->lDetail_ID = _ttol(tcBuffer);
				strKey += tcBuffer;
				break;
			case 2:
				if (oldComponentId != pedData->lID)
				{
					oldComponentId = pedData->lID;
					deviceComponentCode = GetDeviceComponentCode(deviceType,deviceSubType,pedData->lID);
				}
			
				MTC = deviceComponentCode + _T("FF") + tcBuffer;
				pedData->lMTC =wcstol(MTC, NULL, 16);

				EC = deviceComponentCode + _T("FF");
				pedData->lEc = wcstol(EC, NULL, 16);
				break;
			case 3:
				break;
			case 4:
				pedData->sErr = tcBuffer;
				break;
			case 5:
				pedData->sDeal = tcBuffer;
				break;
			case 6:
				pedData->eErr = tcBuffer;
				break;
			case 7:
				pedData->eDeal = tcBuffer;
				break;
			case 8:
				pedData->pErr = tcBuffer;
				break;
			case 9:
				pedData->pDeal = tcBuffer;
				break;
			default:
				break;
			}
			tctemp=tctemp+1+iindex;	//+1 Ϊ������֮ǰ���ҵ��ķָ���
			tcfind=_tcsstr(tctemp,tcSplitSymbol);
			uiColCount++;
		}
		if(uiColCount < 10)
		{
			delete pedData;
			pedData=NULL;
			iRet = 1;
			return iRet;
		}else
		{
			strID.Format(_T("%d"), pedData->lID);
			strDetailID.Format(_T("%d"), pedData->lDetail_ID);
			strKey = strID + strDetailID;
			m_mapException.SetAt(strKey, pedData);
		}
	}   
	// ������Ҫ�ͷ��ڴ�zhengxianle
	if(expfile != NULL){
		delete expfile;
		expfile = NULL;
	}
	DWORD dwtick2=::GetTickCount();
	DWORD dwtickCount=dwtick2-dwtick1;
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡMTCǰ���ֽڱ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CExceptionInfo::GetDeviceComponentCode(AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID)
{
	CString deviceComponentCode = _T("");
	m_deviceComponentCode.find(moudleID);
	if (m_deviceComponentCode.find(moudleID) == m_deviceComponentCode.end()) {
		deviceComponentCode = theTVM_INFO.GetDeviceComponentCode(deviceType,deviceSubType,moudleID);
		m_deviceComponentCode[moudleID] = deviceComponentCode;	
	}else{
		deviceComponentCode = m_deviceComponentCode[moudleID];
	}
	return deviceComponentCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��쳣��Ϣ��ֵ

@param      (i)EXCEPTION_DETAIL& exMsg �쳣��Ϣ�ṹ
@param      (i)long lId                �쳣��ϢID
@param      (i)lDetailID               �쳣��ϢDetailID

@retval      unsigned int:
              0:û�ҵ�����0:�ҵ�

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int CExceptionInfo::GetExceptionByID(EXCEPTION_DETAIL& exMsg, long lId, long lDetailID)
{
    unsigned int iRet = 0;
	CString sKey = 0;
	CString buff1, buff2;
	LPEXCEPTION_DETAIL exMsgOne = NULL;
	
	if (lId < 10000) {
		lId = 1000;
	}
	buff1.Format(_T("%d"), lId);
	buff2.Format(_T("%d"), lDetailID);
	sKey = buff1 + buff2;
	m_mapException.Lookup(sKey, exMsgOne);
	if (NULL != exMsgOne) {
		exMsg = *exMsgOne;
		iRet = 1;
	}else{
		iRet = 0;
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�ǰϵͳ�Ƿ���Ҫ������������

@param      (i)bool bNeedRestart = true   �Ƿ���Ҫ�������� \n
               true: ��Ҫ ; false ����Ҫ

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::SetbNeedRestart(bool bNeedRestart)
{
    m_bNeedRestart = bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��õ�ǰϵͳ�Ƿ���Ҫ��������

@param       void

@retval      bool  \n
             true: ��Ҫ ; false ����Ҫ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionInfo::GetbNeedRestart()
{
    return m_bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ��ļ��쳣�ָ�����

@param      none

@retval     void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::InitRecoverTimes()
{
	m_recoverTimes = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ��ļ��쳣�ָ�����

@param      none

@retval     void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::InitVfareRecoverTimes()
{
	m_vfareRecoverTimes = 0;
}

