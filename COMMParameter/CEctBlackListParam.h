#pragma once
#include "CEctParamMaster.h"

// CEctBlackListParam��Ψһ�ӿ�
#define theECT_BLACKLIST CEctBlackListParam::GetInstance()        

/**
@brief   ECTϵͳ������
*/

class PARAMETERCOMMAND_DECLSPEC CEctBlackListParam :public CEctParamMaster
{
public: 

	static CEctBlackListParam&  GetInstance()
	{
		return theInstance;
	}

protected:
	// ˽�г�Ա����

private:

	//SECTION_INFO m_section[6];	//�ֶ���Ϣ

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CEctBlackListParam(void);
	~CEctBlackListParam();
	CEctBlackListParam(const CEctBlackListParam&);
	CEctBlackListParam& operator=(const CEctBlackListParam&);

	static  CEctBlackListParam theInstance;
};
