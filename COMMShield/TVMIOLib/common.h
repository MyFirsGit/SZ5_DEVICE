#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <crtdbg.h>
#include <stddef.h>
#include <windows.h>
#include <malloc.h>
#include "IOLib.h"
#include "htdl.h"
#include "csmim_def.h"
#include "ModuleLog.h"

// ��־���غ궨��
//#define _TRACE_SEND_DATA_
//#define _TRACE_SERVER_
//#define _TRACE_HTDL_            // HTDL��־
//#define _TRACE_MAXTRIX_
//#define _TRACE_STEP_

#define MAX_TRACE_LEN 7*MAX_SENDBUFF      // ��־������󳤶�

//extern char* m_szText;
//extern void IOTrs_Trace(const char* lpszFormat, ...);
//extern void IOTrs_Clear();
//extern void IOTrs_Initialize(const char* tracePath);

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC���㺯��

@param      (i)BYTE* pData     �������������ָ��
@param      (i)WORD dwLength   ������������ݳ���

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline BYTE BccCalc(BYTE* pData, WORD dwLength)
{
	BYTE Bcc = 0;
	while (dwLength-- != 0){
		Bcc = Bcc ^ *pData++;
	}
	return Bcc;
}
