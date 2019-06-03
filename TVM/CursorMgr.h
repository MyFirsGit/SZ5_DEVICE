#pragma once

#define theCURSOR_MGR CCursorMgr::GetInstance()

/**
/@brief 光标管理
*/

class CCursorMgr
{
public:
    static CCursorMgr& GetInstance();           // 获取该类的对象
    void BeginWaitCursor();                     // 显示等待光标
    void EndWaitCursor();                       // 结束等待光标
    void RestoreWaitCursor();                   // 恢复等待光标

private:
    CCursorMgr(void);                           // 构造函数
    ~CCursorMgr(void);                          // 析构函数

private:
    static CCursorMgr theInstance;              // 该类的唯一接口
    int m_displayCount;                         // 光标显示次数
    bool m_isWaitCursor;                        // 是否是等待光标
};
