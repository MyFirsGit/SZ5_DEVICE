#include "StdAfx.h"
#include "utillist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUtilList::CUtilList(void)
    :m_mutex(FALSE, NULL)
{
}

CUtilList::~CUtilList(void)
{
}

/******************************************************************************/
/**
*	@brief Add the Request item to the tail of list.
*	@param pItem: The pointer of CUtilObject object
*	@date 10/2004
*/
/******************************************************************************/
void CUtilList::AddTail(CUtilObject *pItem)
{
    CSingleLock sLock(&m_mutex, true);

    m_list.insert(m_list.end(), pItem);

    sLock.Unlock();
}

void CUtilList::AddHead(CUtilObject* pItem)
{
	CSingleLock sLock(&m_mutex, true);

	m_list.insert(m_list.begin(), pItem);

	sLock.Unlock();
}

/******************************************************************************/
/**	
*	@brief Check the list is empty or not
*	@return TRUE: The list is empty.
*	@date 10/2003
*/
/******************************************************************************/
BOOL CUtilList::IsEmpty()
{
	return m_list.empty();
}

/******************************************************************************/
/**
*	@brief Remove the item from the head of list. The returned value should 
*			be deleted
*	@return The pointer to object CUtilObject.
*	@date 10/2004
*/
/******************************************************************************/
CUtilObject* CUtilList::RemoveHead()
{
    CSingleLock sLock(&m_mutex, true);

	m_listiter = m_list.begin();
	CUtilObject* pItem = *m_listiter;
	m_list.erase(m_listiter);
    
    sLock.Unlock();

    return pItem;
}

/******************************************************************************/
/**	
*	@brief Remove a specific item
*	@return TRUE: If removing is successful
*	@date 10/2004
*/
/******************************************************************************/
void CUtilList::Remove(CUtilObject* pItem)
{
    CSingleLock sLock(&m_mutex, true);

	m_list.remove(pItem);

    sLock.Unlock();
}

/******************************************************************************/
/**	
*	@brief Get the first pointer of request item object.
*	@return Pointer to CUtilObject object
*	@date 10/2004
*/
/******************************************************************************/
CUtilObject* CUtilList::GetHead()
{
    //CSingleLock sLock(&m_mutex, true);

	m_listiter = m_list.begin();

    //sLock.Unlock();

    return *m_listiter;
}

/******************************************************************************/
/**
*	@brief Get number item in the list
*	@return number of item in the list
*	@date 10/2004
*/
/******************************************************************************/
int CUtilList::GetCount()
{
	return (int)m_list.size();
}

/******************************************************************************/
/**
*	@brief Move the pointer to the first item in the list. This function 
*		should called before call function CUtilList::Next() 
*	@date 10/2004
*/
/******************************************************************************/
void CUtilList::First()
{
	m_listiter = m_list.begin();
}

/******************************************************************************/
/**
*	@brief Move the pointer to the next item in the list
*	@param ppItem: reference to pointer of Request item object
*	@return 
*	- TRUE: Success to move to the next item. 
*	- FALSE: Fail to move to the next item. There is no more next item in the list
*	@date 10/2004
*/
/******************************************************************************/
BOOL CUtilList::Next(CUtilObject **ppItem)
{
    CSingleLock sLock(&m_mutex, true);

	BOOL bRet;
	if (m_listiter == m_list.end())
	{
		bRet = FALSE;
	}
	else 
	{
		*ppItem = *m_listiter;
		++m_listiter;
		bRet = TRUE;
	}
    sLock.Unlock();

	return bRet;
}

/******************************************************************************/
/**
*	@brief Remove all item in the list.
*	@date 10/2004
*	@remark The request items memory in this list should be deleted in advance 
*		before call this function.
*/
/******************************************************************************/
void CUtilList::RemoveAll()
{
    CSingleLock sLock(&m_mutex, true);

	m_list.clear();

    sLock.Unlock();
}