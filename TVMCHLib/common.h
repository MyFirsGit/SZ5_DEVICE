#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <crtdbg.h>
#include <stddef.h>
#include <windows.h>
#include <malloc.h>
#include "CHLib.h"
#include "htdl.h"
#include "csmim_def.h"
#include "CHLog.h"

// 日志开关宏定义
//#define _TRACE_SEND_DATA_
//#define _TRACE_SERVER_
//#define _TRACE_HTDL_            // HTDL日志
//#define _TRACE_MAXTRIX_
//#define _TRACE_STEP_

#define MAX_TRACE_LEN 7*MAX_SENDBUFF      // 日志数据最大长度

//extern char* m_szText;
//extern void CHTrs_Trace(const char* lpszFormat, ...);
//extern void CHTrs_Clear();
//extern void CHTrs_Initialize(const char* tracePath);

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC计算函数

@param      (i)BYTE* pData     参与计算书数据指针
@param      (i)WORD dwLength   参与计算书数据长度

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

inline BYTE BccCheck(BYTE* pData, WORD dwLength)
{
	LONG Sum = 0;
	BYTE i = 0;
	BYTE temp;

	while(dwLength-- !=0 )
	{
		temp = *pData++;
		for( i = 0 ; i< 8 ; i++ )
		{
			if(temp % 2)
			{
				Sum++;
			}
			temp /= 2;
		}
	}

	return Sum % 2;
}