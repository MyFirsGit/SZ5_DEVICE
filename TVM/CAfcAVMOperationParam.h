#pragma once
#include "CAfcAVMOperationParam.h"
#include "CAfcTVMOperationParam.h"
// CAfcAVMOperationParam��Ψһ�ӿ�
#define theAFC_AVM_OPERATION CAfcAVMOperationParam::GetInstance()        

/**
 @brief   AVM��Ӫ������
*/

class  CAfcAVMOperationParam :public CAfcTVMOperationParam
{
public: 

private:

    virtual void GetParamFileName();				 // ��ȡ�����ļ���
	CAfcAVMOperationParam(void);
	~CAfcAVMOperationParam();
};
