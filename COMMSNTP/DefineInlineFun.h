//
// Create Date : 2010.01.22
// FileName : DefineInlineFun.h
//

#ifndef __DEFINE_INLINE_FUN__H__
#define __DEFINE_INLINE_FUN__H__

#include <io.h>
//fun define
#define SafeDelete(pObject)\
{\
	if(pObject!=NULL){\
	delete (pObject);\
	(pObject)=NULL;\
	}\
}

#define SafeDeleteArray(pObjArray)\
{\
	if(pObjArray!=NULL){\
	delete [](pObjArray);\
	(pObjArray)=NULL;\
	}\
}

#define SafeObject(pObj)\
{\
	if (pObj != NULL){\
	pObj->DeleteObject();\
	(pObj)=NULL;\
	}\
}

//inline int _ACCESS(LPCTSTR str, int mode)
//{
//#ifdef UNICODE
//	return _waccess(str, mode);
//#else
//// 	USES_CONVERSION;
//// 	LPSTR lpStr = T2A((LPCTSTR)str);
//	return _access(str, mode);
//#endif
//}

inline  LPCSTR _T2CA(LPCTSTR pWStr)
{
#ifdef UNICODE
	USES_CONVERSION;
	return T2CA(pWStr);
#else
	return pWStr;
#endif
}

inline LPSTR _T2A(LPTSTR pWStr)
{
#ifdef UNICODE
	USES_CONVERSION;
	return T2A(pWStr);
#else
	return pWStr;
#endif
}

// #ifdef UNICODE
// #define _T2A(pWStr)\
// {\
// 	USES_CONVERSION;\
// 	T2A((pWStr));\
// }
// #else
// #define _T2A(pWStr)\
// {\
// 	(pWStr);\
// }
// #endif


inline double _ATOF(LPCTSTR pStr)
{
	return atof(_T2CA(pStr));
}

inline int _ATOI(LPCTSTR pStr)
{
	return atoi(_T2CA(pStr));
}

inline LPSTR _ITOA(int nValue, LPTSTR pStr, int radix)
{
	return _itoa(nValue, _T2A(pStr), radix);
	//return _itoa_s(nValue, _T2A(pStr), radix);
}

#endif //__DEFINE_INLINE_FUN__H__