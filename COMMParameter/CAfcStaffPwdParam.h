#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcStaffPwdParam��Ψһ�ӿ�
#define theAFC_STAFF CAfcStaffPwdParam::GetInstance()        

/**
 @brief   ����Ա������
*/

class PARAMETERCOMMAND_DECLSPEC CAfcStaffPwdParam :public CAfcParamMaster
{
public: 

	// ����Ա��¼��
	 typedef struct _staff {
		DWORD	staffID;		// ����Ա ID 				4	BCD	����С�����˳������
		CString	staffPWD;		// ����Ա����				3	BCD	
		_DATE   pwdEndDate;		// ����Ա������Ч��ֹ����	4	BCD	YYYYMMDD
        _staff(){
            staffID = 0;
            staffPWD = "";
        }
    }STAFF;	

	// ���нӿ�
    vector<STAFF> GetStaffParam();					// ��ò���Ա����	
	
    static CAfcStaffPwdParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section;	//�ֶ���Ϣ

    // ˽�г�Ա����
    vector<STAFF>	m_vec_staff_param;			// ����Ա��������	

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	void decrypt(const unsigned char * ciphertext, int len,char * plaintext);
	CAfcStaffPwdParam(void);
    ~CAfcStaffPwdParam();
	CAfcStaffPwdParam(const CAfcStaffPwdParam&);
	CAfcStaffPwdParam& operator=(const CAfcStaffPwdParam&);

    static  CAfcStaffPwdParam theInstance;
};
