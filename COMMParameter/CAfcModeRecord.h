#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcModeRecord��Ψһ�ӿ�
#define theAFC_MODERECORD CAfcModeRecord::GetInstance()        

/**
@brief   ģʽ����������
*/

class PARAMETERCOMMAND_DECLSPEC CAfcModeRecord :public CAfcParamMaster
{
public: 

	static CAfcModeRecord&  GetInstance()
	{
		return theInstance;
	}

private:

	SECTION_INFO m_section;	//�ֶ���Ϣ


	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	void decrypt(const unsigned char * ciphertext, int len,char * plaintext);
	CAfcModeRecord(void);
	~CAfcModeRecord();
	CAfcModeRecord(const CAfcModeRecord&);
	CAfcModeRecord& operator=(const CAfcModeRecord&);

	static  CAfcModeRecord theInstance;
};
