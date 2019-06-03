#include "stdafx.h"
#include "ExceptionInfo.h"
#include "getIni.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const    EXCEPTION_FILE_NAME    = _T("TVM_EXCEPTION.CSV");// 异常信息文件名

CExceptionInfo  CExceptionInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
 @brief      构造函数
                                                                 
 @param      (i)void

 @retval      无

 @exception   无
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
@brief      析构函数

@param      (i)void

@retval      无

@exception   无
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
@brief      释放资源

@param      无

@retval      无

@exception   无
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
@brief       文件异常恢复次数加1

@param       无

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::IncRecoverTimes()
{
    m_recoverTimes++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置票价表当前恢复次数(累加)

@param       (i) int vfareRecoverTimes 错误次数

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::UpdateVfareRecoverTimes(int vfareRecoverTimes)
{
	m_vfareRecoverTimes = m_vfareRecoverTimes + vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       得到票价表当前恢复次数

@param       无

@retval      unsigned long  票价表当前恢复次数

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionInfo::GetCurVfareRecoverTimes()
{
	return m_vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       得到文件最大允许恢复次数

@param       无

@retval      unsigned long  文件最大允许恢复次数

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionInfo::GetFileMaxRecoverTimes()
{
	return m_maxRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到文件异常恢复次数

@param      (i)void

@retval      CExceptionInfo& 　CExceptionInfo类对象
               
@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionInfo& CExceptionInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否超过最大恢复次数

@param      (i)void

@retval      bool:
               true:超过最大次数　false:未超过最大次数

@exception   无
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
@brief      判断是否超过票价表最大恢复次数

@param      (i)void

@retval      bool:		\n
				true:超过最大次数　false:未超过最大次数

@exception   无
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
@brief      设置异常信息的值

@param      (i)EXCEPTION_MAP& exMap　　异常信息容器

@retval      unsigned int: 0:成功　非0:失败

@exception   无
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

	// 初始化异常文件
	GetAppPath(sAppPath);
	filename = sAppPath + EXCEPTION_FILE_NAME;
	AFC_DEVICE_TYPE deviceType = theMAINTENANCE_INFO.GetDeviceType();
	DEVICE_SUBTYPE deviceSubType = theMAINTENANCE_INFO.GetDeviceSubType();
	long oldComponentId = 0;
	
	CXFile *expfile = new CXFile(filename);
	// 按行读取文件
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
		TCHAR tcBuffer[2000];	//暂且定为每行2000个中文字符
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
			tctemp=tctemp+1+iindex;	//+1 为了跳过之前查找到的分隔符
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
	// 用完需要释放内存zhengxianle
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
@brief      获取MTC前两字节编码

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
@brief      得到异常信息的值

@param      (i)EXCEPTION_DETAIL& exMsg 异常信息结构
@param      (i)long lId                异常信息ID
@param      (i)lDetailID               异常信息DetailID

@retval      unsigned int:
              0:没找到　非0:找到

@exception   无
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
@brief      设置当前系统是否需要设置重新启动

@param      (i)bool bNeedRestart = true   是否需要重新启动 \n
               true: 需要 ; false 不需要

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::SetbNeedRestart(bool bNeedRestart)
{
    m_bNeedRestart = bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得当前系统是否需要重新启动

@param       void

@retval      bool  \n
             true: 需要 ; false 不需要

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionInfo::GetbNeedRestart()
{
    return m_bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化文件异常恢复次数

@param      none

@retval     void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::InitRecoverTimes()
{
	m_recoverTimes = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化文件异常恢复次数

@param      none

@retval     void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionInfo::InitVfareRecoverTimes()
{
	m_vfareRecoverTimes = 0;
}

