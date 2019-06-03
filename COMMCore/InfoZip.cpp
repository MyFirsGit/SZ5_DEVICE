// InfoZip.cpp: implementation of the CInfoZip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoZip.h"
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


bool CInfoZip::bInfozipUnzipSuccess = false;

CInfoZip::CInfoZip()
{
	m_ZipDllHandle       = NULL;
	m_ZWZipDLLHandle     = NULL;
	m_ZipDllExec         = NULL;
	m_GetZipDllVersion   = NULL;

	m_UnzipDllHandle     = NULL;
	m_UnzipDllExec       = NULL;
	m_GetUnzipDllVersion = NULL;
}

CInfoZip::~CInfoZip()
{
	Finalize();
}

int CInfoZip::GetZipDllVersion()
{
	if (GetInitializedZip())
		return m_GetZipDllVersion();
	else
	{
		SetLastError(ZERROR_NOT_INITIALIZED);
		return 0;
	}
}

int CInfoZip::GetUnzipDllVersion()
{
	if (GetInitializedUnzip())
		return m_GetUnzipDllVersion();
	else
	{
		SetLastError(ZERROR_NOT_INITIALIZED);
		return 0;
	}
}

void CInfoZip::SetLastError(UINT uiError)
{
	m_uiLastError = uiError;
}

UINT CInfoZip::GetLastError()
{
	return m_uiLastError;
}

BOOL CInfoZip::Initialize()
{
	return InitializeZip() && InitializeUnzip();
}

BOOL CInfoZip::GetInitialized()
{
	return GetInitializedZip() && GetInitializedUnzip();
}

BOOL CInfoZip::Finalize()
{
	return FinalizeZip() && FinalizeUnzip();
}

void CInfoZip::SetDefaultValues(CZipParams * pParams)
{
	pParams->m_hwndHandle			= NULL;
	pParams->m_pCaller				= NULL;
	pParams->m_liVersion			= GetZipDllVersion();
	pParams->m_pfCallbackFunction	= DefaultZipCallback;
	pParams->m_bTraceEnabled		= FALSE;

	/*============== Begin Zip Flag section ============== */
	pParams->m_pszZipPassword		= NULL;
	pParams->m_bSuffix				= FALSE;
	pParams->m_bEncrypt				= FALSE;
	pParams->m_bSystem				= FALSE;
	pParams->m_bVolume				= FALSE;
	pParams->m_bExtra				= FALSE;
	pParams->m_bNoDirEntries		= FALSE;
	pParams->m_bDate				= FALSE;
	pParams->m_bVerboseEnabled		= FALSE;
	pParams->m_bQuiet				= FALSE;
	pParams->m_bLevel				= 9;
	pParams->m_bComprSpecial		= FALSE;
	pParams->m_bCRLF_LF				= FALSE;
	pParams->m_bJunkDir				= FALSE;
	pParams->m_bRecurse				= FALSE;
	pParams->m_bGrow				= TRUE;
	pParams->m_bForce				= FALSE;
	pParams->m_bMove				= FALSE;
	pParams->m_bDeleteEntries		= FALSE;
	pParams->m_bUpdate				= FALSE;
	pParams->m_bFreshen				= FALSE;
	pParams->m_bJunkSFX				= FALSE;
	pParams->m_bLatestTime			= FALSE;
	/*============== End Zip Flag section ============== */

	for (int j=0; j<8; j++)
		pParams->m_cDate[j] = 0;
	pParams->m_liFileCount			= 0;
	pParams->m_pszArchiveFileName	= NULL;
	pParams->m_liSeven				= 7;

//	char *PFileNames[MAX_PATH+1];
}

BOOL CInfoZip::Execute(CZipParams * pParams)
{
	if (!GetInitializedZip())
		return FALSE;

	m_ZipDllExec(pParams);
	return TRUE;
}

BOOL CInfoZip::AddFiles(const char *pszArchive, char ** paFiles, int iFileCount)
{
	CZipParams zpParams;
	SetDefaultValues(&zpParams);

	zpParams.m_bJunkDir = TRUE;//不压缩目录名称
	zpParams.m_bMove    = TRUE;//压缩完成后删除文件

	//  seting archive name
	zpParams.m_pszArchiveFileName = (char*)malloc(strlen(pszArchive)+1);
	ZeroMemory(zpParams.m_pszArchiveFileName, strlen(pszArchive)+1);
	//strcpy_s(zpParams.m_pszArchiveFileName,strlen(pszArchive)+1,pszArchive);
	strcpy(zpParams.m_pszArchiveFileName,pszArchive);

	//	seting file count
	zpParams.m_liFileCount   = iFileCount;

	//  seting file names
	for (int i=0; i<iFileCount; i++)
	{
		zpParams.m_pszFileNames[i] = (char*)malloc(MAX_PATH+1);
		ZeroMemory(zpParams.m_pszFileNames[i], MAX_PATH+1);
		//strcpy_s(zpParams.m_pszFileNames[i],MAX_FILES,paFiles[i]);
		strcpy(zpParams.m_pszFileNames[i],paFiles[i]);
	}

//  executing command
	int iProcessedCount = m_ZipDllExec(&zpParams);
	ReleaseParams(&zpParams);

	return (iProcessedCount == iFileCount);
}

BOOL CInfoZip::AddFilesEx( CString strSourceFiles, CString strDestFile )
{
	//  executing command
	BOOL bRes = m_ZWZipCpmpress(strSourceFiles,strDestFile);

	return bRes/*(iProcessedCount == iFileCount)*/;
}

BOOL __stdcall DefaultZipCallback(CZipCallbackData *pData)
{
   // CString sFileNameOrMsg = pData->m_pszFileNameOrMsg;
	char after[]={"After extraction, file"};
	char fatalerror[] = {"Fatal Error:"};
    if (pData->m_liErrorCode!=0)
    {
        CInfoZip::bInfozipUnzipSuccess = true;
    }
    if ( pData->m_liActionCode == 4  
        &&(memcmp(pData->m_pszFileNameOrMsg,after,22) ==0 || memcmp(pData->m_pszFileNameOrMsg,fatalerror,12) == 0) 
        )
    {
        CInfoZip::bInfozipUnzipSuccess = true;
    }
    return FALSE;
}

void CInfoZip::ReleaseParams(CZipParams * pParams)
{
	free(pParams->m_pszArchiveFileName);
	for (int i=0; i<pParams->m_liFileCount; i++)
		free(pParams->m_pszFileNames[i]);
}

BOOL CInfoZip::GetInitializedZip()
{
	return m_ZipDllHandle && m_GetZipDllVersion && m_ZipDllExec&& m_ZWZipDLLHandle && m_ZWZipCpmpress;
}

BOOL CInfoZip::GetInitializedUnzip()
{
	return m_UnzipDllHandle && m_GetUnzipDllVersion && m_UnzipDllExec;
}

BOOL CInfoZip::InitializeZip()
{
	BOOL bInitialized = GetInitializedZip();
	if (GetInitializedUnzip())
		return TRUE;

	m_ZipDllHandle = LoadLibraryA("ZIPDLL.DLL");
	if (!m_ZipDllHandle)
	{
		SetLastError(ZERROR_DLL_NOT_FOUND);
		return FALSE;
	}

	m_GetZipDllVersion  = (CGetZipDllVersion)GetProcAddress(m_ZipDllHandle, "GetZipDllVersion");
	if (!m_GetZipDllVersion)
	{
		SetLastError(ZERROR_DLL_FOUNCTION_NOT_FOUND);
		return FALSE;
	}

	m_ZipDllExec = (CZipDllExec)GetProcAddress(m_ZipDllHandle, "ZipDllExec");
	if (!m_ZipDllExec)
	{
		SetLastError(ZERROR_DLL_FOUNCTION_NOT_FOUND);
		return FALSE;
	}
	m_ZWZipDLLHandle = LoadLibraryA("ZLibWrap.dll");
	if (!m_ZWZipDLLHandle)
	{
		SetLastError(ZERROR_DLL_NOT_FOUND);
		return FALSE;
	}
	m_ZWZipCpmpress = (ZWZipCompress)GetProcAddress(m_ZWZipDLLHandle, "ZWZipCompress");
	if (!m_ZWZipCpmpress)
	{
		SetLastError(ZERROR_DLL_FOUNCTION_NOT_FOUND);
		return FALSE;
	}
	return TRUE;
}

BOOL CInfoZip::InitializeUnzip()
{
	BOOL bInitialized = GetInitializedUnzip();
	if (GetInitializedUnzip())
		return TRUE;

	m_UnzipDllHandle = LoadLibraryA("UNZDLL.DLL");
	if (!m_UnzipDllHandle)
	{
		SetLastError(ZERROR_DLL_NOT_FOUND);
		return FALSE;
	}

	m_GetUnzipDllVersion  = (CGetUnzipDllVersion)GetProcAddress(m_UnzipDllHandle, "GetUnzDllVersion");
	if (!m_GetUnzipDllVersion)
	{
		SetLastError(ZERROR_DLL_FOUNCTION_NOT_FOUND);
		return FALSE;
	}

	m_UnzipDllExec = (CUnzipDllExec)GetProcAddress(m_UnzipDllHandle, "UnzDllExec");
	if (!m_UnzipDllExec)
	{
		SetLastError(ZERROR_DLL_FOUNCTION_NOT_FOUND);
		return FALSE;
	}

	return TRUE;
}

BOOL CInfoZip::FinalizeZip()
{
	if (GetInitializedZip())
	{
		FreeLibrary(m_ZipDllHandle);
		m_ZipDllHandle       = NULL;
		m_ZipDllExec         = NULL;
		m_GetZipDllVersion   = NULL;

		FreeLibrary(m_ZWZipDLLHandle);
		m_ZWZipDLLHandle	 = NULL;
		m_ZWZipCpmpress		 = NULL;
	}
	return TRUE;
}

BOOL CInfoZip::FinalizeUnzip()
{
	if (GetInitializedUnzip())
	{
		FreeLibrary(m_UnzipDllHandle);
		m_UnzipDllHandle       = NULL;
		m_UnzipDllExec         = NULL;
		m_GetUnzipDllVersion   = NULL;
	}
	return TRUE;
}

BOOL CInfoZip::Execute(CUnzipParams * pParams)
{
	if (!GetInitializedUnzip())
		return FALSE;

	m_UnzipDllExec(pParams);
	return TRUE;
}

void CInfoZip::ReleaseParams(CUnzipParams * pParams)
{
	free(pParams->m_pszArchiveFileName);
	for (int i=0; i<pParams->m_liFileCount; i++)
		free(pParams->m_pszFileNames[i]);
}

void CInfoZip::SetDefaultValues(CUnzipParams * pParams)
{
	pParams->m_wndHandle          = NULL;
	pParams->m_pCaller            = NULL;
	pParams->m_liVersion          = GetUnzipDllVersion();
	pParams->m_pfCallbackFunction = DefaultZipCallback;
	pParams->m_bTraceEnabled = FALSE;

	pParams->m_bPromptToOverwrite = FALSE;
	pParams->m_pszZipPassword     = NULL;
	pParams->m_bTest              = FALSE;
	pParams->m_bComments          = FALSE;
	pParams->m_bConvert           = FALSE;

	pParams->m_bQuiet             = FALSE;
	pParams->m_bVerboseEnabled    = FALSE;
	pParams->m_bUpdate            = FALSE;
	pParams->m_bFreshen           = FALSE;
	pParams->m_bDirectories       = TRUE;
	pParams->m_bOverwrite         = TRUE;

	pParams->m_liFileCount        = 0;
	pParams->m_pszArchiveFileName = NULL;
	pParams->m_liSeven            = 7;
}

BOOL CInfoZip::AutoExtractFiles(const char * pszArchive, const char * pszTargetFolder)
{
	BOOL bRet = TRUE;
	if (FALSE == InitializeUnzip())
	{
		bRet = FALSE;
		return bRet;
	}
	if (FALSE == ExtractFiles(pszArchive,pszTargetFolder))
	{
		bRet = FALSE;
	}
	if (FALSE == FinalizeUnzip())
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CInfoZip::ExtractFiles(const char * pszArchive, const char * pszTargetFolder)
{
	char pszCurrentDir[MAX_PATH+1];
	ZeroMemory(pszCurrentDir, MAX_PATH+1);
	GetCurrentDirectoryA(MAX_PATH+1, pszCurrentDir);
	SetCurrentDirectoryA(pszTargetFolder);

	CUnzipParams uzpParams;
	SetDefaultValues(&uzpParams);

//  seting archive name
	uzpParams.m_pszArchiveFileName = (char*)malloc(strlen(pszArchive)+1);
	ZeroMemory(uzpParams.m_pszArchiveFileName, strlen(pszArchive)+1);
	//strcpy_s(uzpParams.m_pszArchiveFileName,strlen(uzpParams.m_pszArchiveFileName),pszArchive);
	strcpy(uzpParams.m_pszArchiveFileName, pszArchive);

//  seting file count
	uzpParams.m_liFileCount = 1;

//  seting file names
	uzpParams.m_pszFileNames[0] = (char*)malloc(MAX_PATH+1);
	ZeroMemory(uzpParams.m_pszFileNames[0], MAX_PATH+1);
	//strcpy_s(uzpParams.m_pszFileNames[0], MAX_FILES,"*.*");
	strcpy(uzpParams.m_pszFileNames[0], "*.*");

//  executing command
    int iProcessedCount;

    iProcessedCount = m_UnzipDllExec(&uzpParams);

    if (iProcessedCount == 0 || bInfozipUnzipSuccess == true)
    {
        ReleaseParams(&uzpParams);
        SetCurrentDirectoryA(pszCurrentDir);
        return FALSE;
    }

	ReleaseParams(&uzpParams);
	SetCurrentDirectoryA(pszCurrentDir);
	return TRUE;
}
