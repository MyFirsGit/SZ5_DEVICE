#ifndef __TOKEN_H_2011_06_02__
#define __TOKEN_H_2011_06_02__

#include "TOKENLib.h"
#include "TOKEN_def.h"

/******************************************************************************/
/**	@brief Class support allocate and delete memory.\n
/******************************************************************************/

//typedef struct TokenMemorySlot
//{
//	WORD wReserve;
//	WORD wSize;
//	LPBYTE lpBuffer;
//	TokenMemorySlot()
//	{
//		wReserve = 0;
//		wSize    = 0;
//		lpBuffer = NULL;
//	}
//} TOKEN_MEMORY_SLOT, *LPTOKENMEMORY_SLOT;
//
//class TOKEN_API CTokenMemController
//{
//public:
//	CTokenMemController();
//	CTokenMemController(int iSlotCount);
//	virtual ~CTokenMemController();
//	int Alloc(int iSize);
//	int Free(int iSlotNo);
//	LPTOKENMEMORY_SLOT GetSlot(int iSlotNo);
//private:
//	void InitSlots(int iSlotCount);
//	int FindFreeSlot();
//
//	typedef struct HashItem
//	{
//		WORD wReserve1;
//		WORD wReserve2;
//		WORD wReserve3;
//		WORD fUsed;
//		TokenMemorySlot Slot;
//		HashItem()
//		{   
//			wReserve1 = 0;
//			wReserve2 = 0;
//			wReserve3 = 0;
//			fUsed     = 0;
//		}
//	};
//private:
//	CRITICAL_SECTION m_criticalSection; ///< Handle the "Alloc" and "Free" memory.
//	HashItem* m_pHashTable;
//	int	m_iSlotCount;
//};

#endif