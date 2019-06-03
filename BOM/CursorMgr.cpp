#include "StdAfx.h"
#include "CursorMgr.h"
#include "bominfo.h"

/**
/@brief ������
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ʵ����CCursorMgr��Ψһ����
CCursorMgr CCursorMgr::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr::CCursorMgr(void)
    :m_displayCount(0)
    ,m_isWaitCursor(false)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr::~CCursorMgr(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ķ���

@param      ��

@retval     CCursorMgr&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr& CCursorMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�ȴ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::BeginWaitCursor()
{
    // ��ʾ�ȴ����
    AfxGetApp()->DoWaitCursor(1);

    // ��ʾ���
    while (m_displayCount < 0) {
        m_displayCount = ShowCursor(TRUE);
    }

    m_isWaitCursor = true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ȴ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::EndWaitCursor()
{
    // ���ع��
    if (! theBOM_INFO.CanShowCursor()) {
        while (m_displayCount >= 0) {
            m_displayCount = ShowCursor(FALSE);
        }
    }

    // ������ʾ�ȴ����
    AfxGetApp()->DoWaitCursor(-1);

    m_isWaitCursor = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ָ��ȴ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::RestoreWaitCursor()
{
    if (m_isWaitCursor) {
        AfxGetApp()->DoWaitCursor(0);
    }
}
