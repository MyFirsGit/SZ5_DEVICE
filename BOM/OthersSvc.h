#pragma once

#include "BOMForeService.h"

// OthersSvc

/**
@brief ����ҵ��Service
*/
class COthersSvc : public CBOMForeService
{
public:
   
    COthersSvc();                                               // ���캯��
    virtual ~COthersSvc();                                      // ��������

private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE	= 1
    } DIALOG_GROUP;
};