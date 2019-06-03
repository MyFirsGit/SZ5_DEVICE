#pragma once
#include "Stdafx.h"
#include "CParameterCommandExp.h"
/**
@brief   ECT����������
*/
class PARAMETERCOMMAND_DECLSPEC CParameterMaster
{
public:
	 virtual bool Internalize(bool readFlag = true,CString dataPath=_T(""),CString backupPath=_T(""))=0;         			// ���ڻ�

	virtual DWORD GetCurMasterVer()=0;                                    // �õ���ǰ�汾��Ϣ
	virtual DWORD GetFutMasterVer()=0;                                    // �õ�δ���汾��Ϣ
	virtual _DATE GetCurStartDate()=0;                                    // �õ���ǰ��Ч������Ϣ
	virtual _DATE GetFutStartDate()=0;                                    // �õ�δ����Ч������Ϣ
	virtual PARAM_VERSION_TYPE_CODE GetCurVerType()=0;					// ȡ�õ�ǰ�����汾����
	virtual PARAM_VERSION_TYPE_CODE GetFutVerType()=0;					// ȡ��δ�������汾����

};
