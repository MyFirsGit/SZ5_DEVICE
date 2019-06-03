#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcAccessLevelParam��Ψһ�ӿ�
#define theAFC_ACCESS_LEVEL CAfcAccessLevelParam::GetInstance()        

/**
 @brief   �豸Ȩ�޲�����
*/

class PARAMETERCOMMAND_DECLSPEC CAfcAccessLevelParam :public CAfcParamMaster
{
public: 

	//// ����ԱȨ�޼�¼��
	//typedef struct _staff_accesslevel {
	//	BYTE isCanAnlyze;					// Ʊ����Ϣ��ѯ		1����Ȩ��  0����Ȩ��
	//	BYTE isCanIssue_Adjust;				// ��Ʊ/��Ʊ����	1����Ȩ��  0����Ȩ��
	//	BYTE isCanRefund_Reploss;			// �˿�/��ʧ����	1����Ȩ��  0����Ȩ��
	//	BYTE isCanSamManage;				// SAM������		1����Ȩ��  0����Ȩ��
	//	BYTE isCanHardwareTest;				// Ӳ������			1����Ȩ��  0����Ȩ��
	//	BYTE isCanSystemSet;				// ϵͳ����			1����Ȩ��  0����Ȩ��
	//	BYTE isCanDataManage;				// ���ݹ���			1����Ȩ��  0����Ȩ��
	//	BYTE isCanOther;					// ����				1����Ȩ��  0����Ȩ��
 //       _staff_accesslevel(){
 //           isCanAnlyze = 0;
 //           isCanIssue_Adjust = 0;
	//		isCanRefund_Reploss = 0;
 //           isCanSamManage = 0;
	//		isCanHardwareTest = 0;
 //           isCanSystemSet = 0;
	//		isCanDataManage = 0;
 //           isCanOther = 0;
 //       }
 //   }STAFF_ACCESS_LEVEL;	

	// ����Ա��ɫ��¼��
	 typedef struct _staff_role {
		DWORD			staffID;			// ����ԱID 			3	BCD	����С�����˳������
		vector<WORD>	vecRoleID;		    // ����Ա��ɫID			2	BIN	ͬһ���豸ֻ��һ�ֽ�ɫ��
        _staff_role(){
            staffID = 0;
            vecRoleID.clear();
        }
    }STAFF_ROLE;	

	// ��ɫ������Ϣ����
	typedef struct _role_function {
		WORD			roleID;				// ��ɫID 			���ս�ɫID��С�����˳������
		DWORD			functionID;		    // ��ɫ���ܱ��		һ����ɫ��Ӧ���ϵͳ����ID
        _role_function(){
            roleID = 0;
            functionID = 0;
        }
    }ROLE_FUNCTION;	

	// ���нӿ�
    void GetStaffAccesslevel(DWORD,vector<SYS_FUNCTION_CODE>& functions);					// ��ò���ԱȨ��

	bool IsStaffHasRight(DWORD,SYS_FUNCTION_CODE);
	
    static CAfcAccessLevelParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[2];	//�ֶ���Ϣ

    // ˽�г�Ա����
    vector<STAFF_ROLE>		m_vec_staff_role;				// ����Ա��ɫ��¼��
	vector<ROLE_FUNCTION>	m_vec_role_function;			// ��ɫ���ܼ�¼��
	
    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcAccessLevelParam(void);
    ~CAfcAccessLevelParam();
	CAfcAccessLevelParam(const CAfcAccessLevelParam&);
	CAfcAccessLevelParam& operator=(const CAfcAccessLevelParam&);

    static  CAfcAccessLevelParam theInstance;
};
