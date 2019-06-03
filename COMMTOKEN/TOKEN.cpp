/*****************************************************************************/
/* FILE NAME     : TOKEN.cpp													 */
/*****************************************************************************/
#include "stdafx.h"
#include "TOKEN.h"
#include "crtdbg.h"

#ifdef  _TRACE_INFO_
char* m_lpszTraceBuff;
#endif

BOOL APIENTRY DllMain( HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

void TOKENTrace(const char* lpszFormat, ...)
{
	_ASSERTE(lpszFormat);
	va_list arglist;
	va_start( arglist, lpszFormat );

#ifdef _TRACE_INFO_
	int nBuf;

	nBuf = wvsprintf(m_lpszTraceBuff, lpszFormat, arglist);
	//_ASSERTE(nBuf < MAX_TRACE_LEN);
	g_Debug.print( m_lpszTraceBuff );

	//g_Debug.printf( lpszFormat, arglist );
#endif

	va_end(arglist);
}

//
/////////////////////////////////////////////////////////////////////////
//
//CTokenMemController::CTokenMemController()
//{
//	InitSlots(20);
//}
//
//CTokenMemController::CTokenMemController(int iSlotCount)
//{
//	InitSlots(iSlotCount);
//}
//
//void CTokenMemController::InitSlots(int iSlotCount)
//{
//	_ASSERTE(iSlotCount > 0);
//	//Initial a critical section
//	::InitializeCriticalSection(&m_criticalSection);
//
//	m_iSlotCount = iSlotCount;
//	m_pHashTable = new HashItem[m_iSlotCount];
//
//	_ASSERTE(NULL != m_pHashTable);
//
//	for(int i=0; i<m_iSlotCount; i++){
//		m_pHashTable[i].fUsed = 0;
//		m_pHashTable[i].Slot.wSize = 0;
//		m_pHashTable[i].Slot.lpBuffer = NULL;
//	}
//}
//
//CTokenMemController::~CTokenMemController()
//{
//	_ASSERTE(NULL != m_pHashTable);
//
//	for(int i=0; i<m_iSlotCount; i++){
//		Free(i);
//	}
//	delete []m_pHashTable;
//
//	//Destroy the created critical session
//	::DeleteCriticalSection(&this->m_criticalSection);
//}
//
//int  CTokenMemController::Alloc(int iSize)
//{
//	int iFreeSlot = -1;
//
//	if (iSize <= 0) 
//		return -1;
//
//	::EnterCriticalSection(&m_criticalSection);
//
//	iFreeSlot = FindFreeSlot();
//	if (iFreeSlot != -1){
//		m_pHashTable[iFreeSlot].Slot.lpBuffer =	(BYTE*)malloc(iSize);
//		if (NULL != m_pHashTable[iFreeSlot].Slot.lpBuffer){
//			m_pHashTable[iFreeSlot].Slot.wSize = (WORD)iSize;
//			m_pHashTable[iFreeSlot].fUsed = 1;
//		}
//	}
//	::LeaveCriticalSection(&m_criticalSection);
//
//	return iFreeSlot;
//}
//
//int CTokenMemController::Free(int iSlotNo)
//{
//	int iRet = -1;
//	::EnterCriticalSection(&m_criticalSection);
//
//	if (iSlotNo >= 0 && iSlotNo < m_iSlotCount){
//		__try{
//			if (0 != m_pHashTable[iSlotNo].fUsed &&	NULL != m_pHashTable[iSlotNo].Slot.lpBuffer){
//				free((void*)m_pHashTable[iSlotNo].Slot.lpBuffer);
//			}
//		}
//		__finally{
//			m_pHashTable[iSlotNo].Slot.wSize = 0;
//			m_pHashTable[iSlotNo].Slot.lpBuffer = NULL;
//			m_pHashTable[iSlotNo].fUsed = 0;
//		}
//	}
//
//	::LeaveCriticalSection(&m_criticalSection);
//
//	return 0;
//}
//
//LPTOKENMEMORY_SLOT CTokenMemController::GetSlot(int iSlotNo)
//{
//	if (iSlotNo >= 0 && iSlotNo < m_iSlotCount){
//		if (m_pHashTable[iSlotNo].fUsed == 1){
//			return &m_pHashTable[iSlotNo].Slot;
//		}
//	}
//	return NULL;
//}
//
//int CTokenMemController::FindFreeSlot()
//{
//	int iRet = -1;
//	for (int i=0; i<m_iSlotCount; i++){
//		if (m_pHashTable[i].fUsed == 0){
//			iRet = i;
//			break;
//		}
//	}
//	return iRet;
//}
//
