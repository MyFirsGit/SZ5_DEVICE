#include "StdAfx.h"
#include "CursorMgr.h"
#include "bominfo.h"

/**
/@brief 光标管理
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 实例化CCursorMgr的唯一对象
CCursorMgr CCursorMgr::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr::CCursorMgr(void)
    :m_displayCount(0)
    ,m_isWaitCursor(false)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr::~CCursorMgr(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取该类的对象

@param      无

@retval     CCursorMgr&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCursorMgr& CCursorMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示等待光标

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::BeginWaitCursor()
{
    // 显示等待光标
    AfxGetApp()->DoWaitCursor(1);

    // 显示光标
    while (m_displayCount < 0) {
        m_displayCount = ShowCursor(TRUE);
    }

    m_isWaitCursor = true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      结束等待光标

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::EndWaitCursor()
{
    // 隐藏光标
    if (! theBOM_INFO.CanShowCursor()) {
        while (m_displayCount >= 0) {
            m_displayCount = ShowCursor(FALSE);
        }
    }

    // 结束显示等待光标
    AfxGetApp()->DoWaitCursor(-1);

    m_isWaitCursor = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      恢复等待光标

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCursorMgr::RestoreWaitCursor()
{
    if (m_isWaitCursor) {
        AfxGetApp()->DoWaitCursor(0);
    }
}
