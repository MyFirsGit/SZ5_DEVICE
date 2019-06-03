#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcStaffPwdParam的唯一接口
#define theAFC_STAFF CAfcStaffPwdParam::GetInstance()        

/**
 @brief   操作员参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAfcStaffPwdParam :public CAfcParamMaster
{
public: 

	// 操作员记录体
	 typedef struct _staff {
		DWORD	staffID;		// 操作员 ID 				4	BCD	按从小到大的顺序排序
		CString	staffPWD;		// 操作员密码				3	BCD	
		_DATE   pwdEndDate;		// 操作员密码有效截止日期	4	BCD	YYYYMMDD
        _staff(){
            staffID = 0;
            staffPWD = "";
        }
    }STAFF;	

	// 共有接口
    vector<STAFF> GetStaffParam();					// 获得操作员参数	
	
    static CAfcStaffPwdParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section;	//分段信息

    // 私有成员变量
    vector<STAFF>	m_vec_staff_param;			// 操作员参数参数	

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	void decrypt(const unsigned char * ciphertext, int len,char * plaintext);
	CAfcStaffPwdParam(void);
    ~CAfcStaffPwdParam();
	CAfcStaffPwdParam(const CAfcStaffPwdParam&);
	CAfcStaffPwdParam& operator=(const CAfcStaffPwdParam&);

    static  CAfcStaffPwdParam theInstance;
};
