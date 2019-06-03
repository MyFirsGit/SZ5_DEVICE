#pragma once

#include "BOMForeService.h"

// OthersSvc

/**
@brief 其他业务Service
*/
class COthersSvc : public CBOMForeService
{
public:
   
    COthersSvc();                                               // 构造函数
    virtual ~COthersSvc();                                      // 析构函数

private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE	= 1
    } DIALOG_GROUP;
};