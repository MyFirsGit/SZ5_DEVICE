////////////////////////////////////////////////
//                                            //
// Some Verify utilities macros               //
// Written by N. Nanbara            //
//                                            //
////////////////////////////////////////////////

#ifndef __CVERIFY_H_INCLUDED_
#define __CVERIFY_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <crtdbg.h>

#ifdef _DEBUG

#define ASSERT_ISREADPOINTER(p, l) \
{ \
    if ((NULL == p) || IsBadReadPtr(p, l)) \
    { \
         _RPTF2(_CRT_ASSERT, "Parameter " #p " 0x%08X is NULL or does not have " \
                "read access to the specified range " #l " = %d bytes of memory.", p, l); \
    } \
}

#define ASSERT_ISWRITEPOINTER(p, l) \
{ \
    if ((NULL == p) || IsBadWritePtr(p, l)) \
    { \
         _RPTF2(_CRT_ASSERT, "Parameter " #p " 0x%08X is NULL or does not have " \
                "write access to the specified range " #l " = %d bytes of memory.", p, l); \
    } \
}

#define ASSERT_ISVALIDSTRING(p) \
{ \
    if ((NULL == p) || IsBadStringPtr(p, (UINT) -1)) \
    { \
        _RPTF1(_CRT_ASSERT, "String pointer " #p " 0x%08X is NULL or is a invalid zero terminating string.", p); \
    } \
}

#define WIN32CHECK(x) \
{ \
    BOOL __bReturn__ = (x); \
    if (FALSE == __bReturn__) \
    { \
        DWORD __dwErrCode__ = GetLastError(); \
        _RPTF1(_CRT_ASSERT, "Call the Win32 API function " #x " failed. GetLastError() return %d.", __dwErrCode__); \
        SetLastError(__dwErrCode__); \
    } \
}

#define VERIFY_ISREADPOINTER(p,l)   ASSERT_ISREADPOINTER(p, l)
#define VERIFY_ISWRITEPOINTER(p,l)  ASSERT_ISWRITEPOINTER(p, l)
#define VERIFY_ISVALIDSTRING(p)     ASSERT_ISVALIDSTRING(p)

#else   // _DEBUG

#define ASSERT_ISREADPOINTER(p, l)  ((void) 0)
#define VERIFY_ISREADPOINTER(p, l)  ((void) (p, l))

#define ASSERT_ISWRITEPOINTER(p, l) ((void) 0)
#define VERIFY_ISWRITEPOINTER(p, l) ((void) (p, l))

#define ASSERT_ISVALIDSTRING(p)     ((void) 0)
#define VERIFY_ISVALIDSTRING(p)     ((void) (p))

#define WIN32CHECK(x)               ((void) (x))

#endif  // !_DEBUG

#endif  // __CVERIFY_H_INCLUDED_
