#pragma once
#include "CAfcAVMOperationParam.h"
#include "CAfcTVMOperationParam.h"
// CAfcAVMOperationParam的唯一接口
#define theAFC_AVM_OPERATION CAfcAVMOperationParam::GetInstance()        

/**
 @brief   AVM运营参数类
*/

class  CAfcAVMOperationParam :public CAfcTVMOperationParam
{
public: 

private:

    virtual void GetParamFileName();				 // 获取参数文件名
	CAfcAVMOperationParam(void);
	~CAfcAVMOperationParam();
};
