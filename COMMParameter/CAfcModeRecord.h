#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcModeRecord的唯一接口
#define theAFC_MODERECORD CAfcModeRecord::GetInstance()        

/**
@brief   模式履历参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAfcModeRecord :public CAfcParamMaster
{
public: 

	static CAfcModeRecord&  GetInstance()
	{
		return theInstance;
	}

private:

	SECTION_INFO m_section;	//分段信息


	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	void decrypt(const unsigned char * ciphertext, int len,char * plaintext);
	CAfcModeRecord(void);
	~CAfcModeRecord();
	CAfcModeRecord(const CAfcModeRecord&);
	CAfcModeRecord& operator=(const CAfcModeRecord&);

	static  CAfcModeRecord theInstance;
};
