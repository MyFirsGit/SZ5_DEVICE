//*****************************************************************************/
//* FILE NAME     : TOKENLib.cpp                                             */
//* PROGRAM NAME  : Implement file for functions of the transaction management*/
//*                  library                                                  */
//* FUNCTION      : 1.                                                        */
//*                                                                           */
//*****************************************************************************/
#include <windows.h>
#pragma   once  
//
////----------------- 共享数据段------------------
#pragma data_seg("Shared")
//
#pragma data_seg()
#pragma comment(linker, "/section:Shared,rws")
////-----------------------------------------------

////////////////////////////////////////////////////////////////////////////
///**
//@brief      DLL入口函数
//
//@param      (i)HANDLE hModule				DLL句柄
//@param      (i)DWORD ul_reason_for_call		调用原因
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	BOOL bRet = TRUE;
	int i = 0;
	switch (ul_reason_for_call){
	// 加载DLL
	case DLL_PROCESS_ATTACH:

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	// 卸载DLL
	case DLL_PROCESS_DETACH:
		break;
	}
    return bRet;
}
