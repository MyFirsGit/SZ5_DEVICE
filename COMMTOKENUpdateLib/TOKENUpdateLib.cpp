//*****************************************************************************/
//* FILE NAME     : TOKENLib.cpp                                             */
//* PROGRAM NAME  : Implement file for functions of the transaction management*/
//*                  library                                                  */
//* FUNCTION      : 1.                                                        */
//*                                                                           */
//*****************************************************************************/
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////
///**
//@brief      DLL��ں���
//
//@param      (i)HANDLE hModule				DLL���
//@param      (i)DWORD ul_reason_for_call		����ԭ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	BOOL bRet = TRUE;
	int i = 0;
	switch (ul_reason_for_call){
	// ����DLL
	case DLL_PROCESS_ATTACH:

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	// ж��DLL
	case DLL_PROCESS_DETACH:
		break;
	}
    return bRet;
}
