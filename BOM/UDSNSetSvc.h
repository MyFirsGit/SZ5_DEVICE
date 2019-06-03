#pragma once
#include "bomforeservice.h"

/**
@brief    ����ҵ�� - UDSN����ҵ��
*/
class CUDSNSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CUDSNSetSvc();
	~CUDSNSetSvc();
	void SetAccUDSN(ULONG);		// ����ACC��ˮ��
	void SetAfcUDSN(ULONG);		// ����AFC��ˮ��
	void SetEctUDSN(ULONG);		// ����ECT��ˮ��

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_UDSN_SET           =  1,                       
	} DIALOG_GROUP;

	//UDSN�趨���
	ULONG m_sAccUDSN;				// ACC��ˮ��
	ULONG m_sAfcUDSN;				// AFC��ˮ��
	ULONG m_sEctUDSN;				// ECT��ˮ��
};