#pragma once
#include "bomforeservice.h"

/**
@brief    保守业务 - UDSN设置业务
*/
class CUDSNSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CUDSNSetSvc();
	~CUDSNSetSvc();
	void SetAccUDSN(ULONG);		// 设置ACC流水号
	void SetAfcUDSN(ULONG);		// 设置AFC流水号
	void SetEctUDSN(ULONG);		// 设置ECT流水号

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_UDSN_SET           =  1,                       
	} DIALOG_GROUP;

	//UDSN设定相关
	ULONG m_sAccUDSN;				// ACC流水号
	ULONG m_sAfcUDSN;				// AFC流水号
	ULONG m_sEctUDSN;				// ECT流水号
};