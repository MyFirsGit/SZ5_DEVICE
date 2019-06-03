#include "stdafx.h"
#include "GlobalParameter.h"
#include "getIni.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const    EXCEPTION_FILE_NAME    = _T("EXCEPTION.CSV");// 异常信息文件名

CGlobalParameter  CGlobalParameter::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
 @brief      构造函数
                                                                 
 @param      (i)void

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
CGlobalParameter::CGlobalParameter(void)
{
	m_uHour = 0;
	m_uMinute = 0;
	m_uSecond = 0;
	m_recoverTimes = 0;
	m_vfareRecoverTimes = 0;

	//TCHAR szAppPath[_MAX_PATH];
	//ComGetAppPath(szAppPath,_MAX_PATH);
	//CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	//CGetIni GetIni(szBOMINIFName);
	//
	//theBOM_INFO.Get
    //m_maxRecoverTimes = (unsigned long)GetIni.GetDataInt("APPLICATION", "MaxRecoverTimes", 0);

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
CGlobalParameter::~CGlobalParameter(void)
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
void CGlobalParameter::Release()
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
@brief      设置营运开始时刻

@param      (i)unsigned short Hour　　小时
@param      (i)unsigned short Minute　分种

@retval      bool 
              true:成功　 false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::SetBusiDateSwitchTime(unsigned char Hour, unsigned char Minute,unsigned char Second)
{
	if (Hour > 23 || Minute > 59 || Second > 59) {
		return false;
	}
	m_uHour = Hour;
	m_uMinute = Minute;
	m_uSecond = Second;
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到营运开始时刻

@param      (i)unsigned short& Hour　　小时
@param      (i)unsigned short& Minute　分种
@param      (i)unsigned short& Second　秒

@retval      bool 
               true:成功　 false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute, unsigned char& Second)
{
	if (m_uHour > 23 || m_uMinute > 59 || m_uSecond > 59) {
		return false;
	}

	Hour = m_uHour;
	Minute = m_uMinute;
	Second = m_uSecond;
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       文件异常恢复次数加1

@param       无

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::IncRecoverTimes()
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
void CGlobalParameter::UpdateVfareRecoverTimes(int vfareRecoverTimes)
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
unsigned long CGlobalParameter::GetCurVfareRecoverTimes()
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
unsigned long CGlobalParameter::GetFileMaxRecoverTimes()
{
	return m_maxRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到文件异常恢复次数

@param      (i)void

@retval      CGlobalParameter& 　CGlobalParameter类对象
               
@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CGlobalParameter& CGlobalParameter::GetInstance()
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
bool CGlobalParameter::OverMaxRecoverTimes()
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
bool CGlobalParameter::OverMaxVfareRecoverTimes()
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
unsigned int CGlobalParameter::GetExceptionDetail()
{
	unsigned int iRet = 0;
	CString strline = _T(""); 
	CString strKey = _T(""); 
	CString strID = _T("");
	CString strDetailID = _T("");
	CString strError = _T("");
	CString strDetail = _T("");
	CStringArray *strExpArray = NULL;

	CString sAppPath;
	CString filename;
	// 初始化异常文件
	GetAppPath(sAppPath);
	filename = sAppPath + EXCEPTION_FILE_NAME;	
	CXFile *expfile = new CXFile(filename);
	// 按行读取文件
	expfile->ReadString(strline);  
	while(!feof(expfile->m_pStream))   
	{   
		expfile->ReadString(strline); 	
		if(strline.GetLength() == 0){
			continue;
		}
		strExpArray = SplitCString(strline,_T(","));
		
		int cnt = strExpArray->GetSize();
		if(cnt != 6){
			iRet = 1;
			return iRet;
		}
		else{
			LPEXCEPTION_DETAIL  pedData = new EXCEPTION_DETAIL;

			// 错误编号
			pedData->lID = _ttol(strExpArray->GetAt(0));
			strKey = strExpArray->GetAt(0);
			// 错误详细编号
			pedData->lDetail_ID = _ttol(strExpArray->GetAt(1));
			strKey += strExpArray->GetAt(1);
			// MTC编号
			pedData->lMTC = _ttol(strExpArray->GetAt(2));
			// EC编号
			pedData->lEc = _ttoi(strExpArray->GetAt(3));
			// 错误内容
			strError = strExpArray->GetAt(4);
			// 错误详细内容
			strDetail = strExpArray->GetAt(5);

			pedData->sErrLen = strError.GetLength() + 1;
			pedData->sDealLen = strDetail.GetLength() + 1;
			pedData->sErr = new char[pedData->sErrLen];
			pedData->sDeal = new char[pedData->sDealLen];
			memset(pedData->sErr, 0, pedData->sErrLen);
			memset(pedData->sDeal, 0, pedData->sDealLen);
			memcpy(pedData->sErr, strError, strError.GetLength());
			memcpy(pedData->sDeal, strDetail, strDetail.GetLength());
			strID.Format(_T("%d"), pedData->lID);
			strDetailID.Format(_T("%d"), pedData->lDetail_ID);
			strKey = strID + strDetailID;
			m_mapException.SetAt(strKey, pedData);	
		}			
	}   
	if(strExpArray != NULL){
		delete strExpArray;
		strExpArray = NULL;
	}
	return iRet;
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
unsigned int CGlobalParameter::GetExceptionByID(EXCEPTION_DETAIL& exMsg, long lId, long lDetailID)
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
void CGlobalParameter::SetbNeedRestart(bool bNeedRestart)
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
bool CGlobalParameter::GetbNeedRestart()
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
void CGlobalParameter::InitRecoverTimes()
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
void CGlobalParameter::InitVfareRecoverTimes()
{
	m_vfareRecoverTimes = 0;
}

