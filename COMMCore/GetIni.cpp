#include "stdafx.h"
#include "getIni.h"
#include "GetIni.h"
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------------
/**
 * @brief Contructor, initialize data member
 */
//-------------------------------------------------------------------------
CGetIni::CGetIni()
{
	m_sFileName = "";		
}
//-------------------------------------------------------------------------
/**
 * @brief Contructor, initialize data member
 * @see none
 */
//-------------------------------------------------------------------------
CGetIni::CGetIni(CString sFileName)
{
	m_sFileName = sFileName;
}

CGetIni::~CGetIni()
{

}
//-------------------------------------------------------------------------
/**
 * @brief Read data from ini file
 * @param  pszSection: section of read data
 * @param pszKey: key of read data
 * @return string of data or empty if there is error
 * @since 2003-11-23 (Ngan Do)
 */
//-------------------------------------------------------------------------

TCHAR* CGetIni::GetData(char *pszSection, char *pszKey)
{
	DWORD dw = 0;
	TCHAR szBuf[INI_BUFF_LEN];
	dw = GetPrivateProfileString ((TCHAR *)pszSection, (TCHAR *)pszKey, NULL, (TCHAR *)szBuf, INI_BUFF_LEN, m_sFileName); 		
	if(dw !=0)
		return &szBuf[0];
	return NULL;
}

//-------------------------------------------------------------------------
/**
 * @brief Read data from ini file
 * @param lpszSection: section of read data
 * @param lpszKey: key of read data
 * @param iDefault: default value
 * @return value of the key as an integer
 * @since 2004-03-26 (Khang Luu)
 */
//-------------------------------------------------------------------------
int CGetIni::GetDataInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int iDefault)
{
    int iValue = iDefault;

    try {
        //if (PathFileExists(m_sFileName))
        //{
        iValue = GetPrivateProfileInt(lpszSection, lpszKey, iDefault, m_sFileName);
        //}
    }
    catch(...) {
    }

	return iValue;
}
	
//-------------------------------------------------------------------------
/**
 * @brief Read data from ini file
 * @param lpszSection: section of read data
 * @param lpszKey: key of read data
 * @param CString sDefault: default value
 * @return value of the key as an CString
 * @since 2004-03-26 (Khang Luu)
 */
//-------------------------------------------------------------------------
CString CGetIni::GetDataString(LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault)
{
	CString sValue = sDefault;

    try {
        DWORD dw = 0;
        TCHAR szBuf[INI_BUFF_LEN];

        dw = GetPrivateProfileString (lpszSection, lpszKey, _T(""), (TCHAR *)szBuf, INI_BUFF_LEN, m_sFileName);
        if (dw > 0)
        {
            AfxExtractSubString(sValue,szBuf,0,';');
            if (sValue.GetAt(0) == _T('|'))
            {
                sValue = sValue.Right(sValue.GetLength() - 1);
            }
        }
    }
    catch(...) {
		theDEBUG_LOG->WriteData(m_sFileName,_T(__FUNCTION__),__LINE__,_T("取配置文件失败，默认为：%s"),sDefault);
    }

	return sValue;

}

//-------------------------------------------------------------------------
/**
* @brief Write data to ini file
* @param lpszSection: section to write data
* @param lpszKey: key to write data
* @param lpszData: data to write
* @return True if write success else return false
* @since 2004-10-14 (K.Tuyen)
*/
//-------------------------------------------------------------------------
BOOL CGetIni::WriteData(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszData)
{
	int iRet = 0;

	iRet = WritePrivateProfileString(lpszSection,lpszKey,lpszData,m_sFileName);

	if( iRet != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-------------------------------------------------------------------------
/**
* @brief delete section in INI file
* @param lpszSection: section to be delete
* @return True if delete success else return false
* @since 2004-10-14 (K.Tuyen)
*/
//-------------------------------------------------------------------------
BOOL CGetIni::DeleteSection(LPCTSTR lpszSection)
{
	return WriteData(lpszSection,NULL,NULL);
}

//-------------------------------------------------------------------------
/**
* @brief delete key in INI file
* @param lpszSection: section to write data
* @param lpszKey: key will be deleted
* @return True if delete success else return false
* @since 2004-10-14 (K.Tuyen)
*/
//-------------------------------------------------------------------------
BOOL CGetIni::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	return WriteData(lpszSection,lpszKey,NULL);
}
