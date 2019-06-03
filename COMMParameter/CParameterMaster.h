#pragma once
#include "Stdafx.h"
#include "CParameterCommandExp.h"
/**
@brief   ECT参数管理父类
*/
class PARAMETERCOMMAND_DECLSPEC CParameterMaster
{
public:
	 virtual bool Internalize(bool readFlag = true,CString dataPath=_T(""),CString backupPath=_T(""))=0;         			// 初期化

	virtual DWORD GetCurMasterVer()=0;                                    // 得到当前版本信息
	virtual DWORD GetFutMasterVer()=0;                                    // 得到未来版本信息
	virtual _DATE GetCurStartDate()=0;                                    // 得到当前生效日期信息
	virtual _DATE GetFutStartDate()=0;                                    // 得到未来生效日期信息
	virtual PARAM_VERSION_TYPE_CODE GetCurVerType()=0;					// 取得当前参数版本类型
	virtual PARAM_VERSION_TYPE_CODE GetFutVerType()=0;					// 取得未来参数版本类型

};
