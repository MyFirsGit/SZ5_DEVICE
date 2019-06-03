#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcAccessLevelParam的唯一接口
#define theAFC_ACCESS_LEVEL CAfcAccessLevelParam::GetInstance()        

/**
 @brief   设备权限参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAfcAccessLevelParam :public CAfcParamMaster
{
public: 

	//// 操作员权限记录体
	//typedef struct _staff_accesslevel {
	//	BYTE isCanAnlyze;					// 票卡信息查询		1：有权限  0：无权限
	//	BYTE isCanIssue_Adjust;				// 售票/补票管理	1：有权限  0：无权限
	//	BYTE isCanRefund_Reploss;			// 退卡/挂失管理	1：有权限  0：无权限
	//	BYTE isCanSamManage;				// SAM卡管理		1：有权限  0：无权限
	//	BYTE isCanHardwareTest;				// 硬件测试			1：有权限  0：无权限
	//	BYTE isCanSystemSet;				// 系统设置			1：有权限  0：无权限
	//	BYTE isCanDataManage;				// 数据管理			1：有权限  0：无权限
	//	BYTE isCanOther;					// 其他				1：有权限  0：无权限
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

	// 操作员角色记录体
	 typedef struct _staff_role {
		DWORD			staffID;			// 操作员ID 			3	BCD	按从小到大的顺序排序
		vector<WORD>	vecRoleID;		    // 操作员角色ID			2	BIN	同一类设备只有一种角色。
        _staff_role(){
            staffID = 0;
            vecRoleID.clear();
        }
    }STAFF_ROLE;	

	// 角色功能信息参数
	typedef struct _role_function {
		WORD			roleID;				// 角色ID 			按照角色ID从小到大的顺序排列
		DWORD			functionID;		    // 角色功能编号		一个角色对应多个系统功能ID
        _role_function(){
            roleID = 0;
            functionID = 0;
        }
    }ROLE_FUNCTION;	

	// 共有接口
    void GetStaffAccesslevel(DWORD,vector<SYS_FUNCTION_CODE>& functions);					// 获得操作员权限

	bool IsStaffHasRight(DWORD,SYS_FUNCTION_CODE);
	
    static CAfcAccessLevelParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[2];	//分段信息

    // 私有成员变量
    vector<STAFF_ROLE>		m_vec_staff_role;				// 操作员角色记录体
	vector<ROLE_FUNCTION>	m_vec_role_function;			// 角色功能记录体
	
    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcAccessLevelParam(void);
    ~CAfcAccessLevelParam();
	CAfcAccessLevelParam(const CAfcAccessLevelParam&);
	CAfcAccessLevelParam& operator=(const CAfcAccessLevelParam&);

    static  CAfcAccessLevelParam theInstance;
};
