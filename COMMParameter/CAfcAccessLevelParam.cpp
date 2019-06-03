#include "StdAfx.h"
#include "CAfcAccessLevelParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcAccessLevelParam CAfcAccessLevelParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcAccessLevelParam::CAfcAccessLevelParam(void) : CAfcParamMaster()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcAccessLevelParam::~CAfcAccessLevelParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1007);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::ParseBody(LPBYTE lpData)
{
    try{
		// 偏移量设置	
		lpData += m_section[0].startPosition;
		// 分段1：操作员角色定义参数 
		if (m_section[0].recordCount != 0) {
			// 操作员数量
			DWORD staffCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData +=4;	
			// vector空间分配
			m_vec_staff_role.clear();
			m_vec_staff_role.reserve(staffCnt);			

			for (unsigned int i=0; i<staffCnt; i++) {  
				STAFF_ROLE tempRecord;
				// 操作员 ID 
				tempRecord.staffID = BCD2int((char*)lpData,3);			
				lpData += 3;
				// 角色ID数量
				DWORD roleIdCnt = *lpData;				
				lpData ++;	
				// vector空间分配
				tempRecord.vecRoleID.clear();
				tempRecord.vecRoleID.reserve(roleIdCnt);			
				for (unsigned int j=0; j<roleIdCnt; j++) { 
					WORD temp;
					temp = mstos(ComMakeWORD(*lpData, *(lpData + 1)));					
					lpData += 2;	
					tempRecord.vecRoleID.push_back(temp);
				}		
				m_vec_staff_role.push_back(tempRecord);
			}
			
		}

		// 分段2：角色功能信息参数
		if (m_section[1].recordCount != 0) {
			// 角色功能数量
			DWORD roleFunctionCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData +=4;	
			// vector空间分配
			m_vec_role_function.clear();
			m_vec_role_function.reserve(roleFunctionCnt);			

			for (unsigned int k=0; k<roleFunctionCnt; k++) {  
				ROLE_FUNCTION tempRecord2;
				// 角色ID 
				tempRecord2.roleID = mstos(ComMakeWORD(*lpData, *(lpData + 1)));
				lpData += 2;
				// 角色功能编号
				tempRecord2.functionID = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
				lpData +=4;				
				m_vec_role_function.push_back(tempRecord2);
			}
		}
    }
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出分段信息

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文结构调整部分内容正确性

@param      (i)LPBYTE           电文信息

@retval     bool   true:正确  false：不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcAccessLevelParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得操作员权限

@param      (i)DWORD staffId						 操作员ID
@param      (o)STAFF_ACCESS_LEVEL& staffAccessLevel  操作员权限

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::GetStaffAccesslevel(DWORD staffId,vector<SYS_FUNCTION_CODE>& functions) 	
{
	for(vector<STAFF_ROLE>::iterator iterStaffRole = m_vec_staff_role.begin(); iterStaffRole != m_vec_staff_role.end();iterStaffRole++){
		if(staffId!=iterStaffRole->staffID){
			continue;
		}
		vector<WORD>& vecRoles = iterStaffRole->vecRoleID;
		for(vector<WORD>::iterator iterRole = vecRoles.begin();iterRole!=vecRoles.end();iterRole++){
			for (vector<ROLE_FUNCTION>::iterator iterFunction = m_vec_role_function.begin();iterFunction!=m_vec_role_function.end();iterFunction++){
				if(iterFunction->roleID != *iterRole){
					continue;
				}
				SYS_FUNCTION_CODE  functionCode = (SYS_FUNCTION_CODE)iterFunction->functionID;
				vector<SYS_FUNCTION_CODE>::iterator pos = find(functions.begin(),functions.end(),functionCode);
				if(pos==functions.end()){
					functions.push_back(functionCode);
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcAccessLevelParam::IsStaffHasRight(DWORD staffId,SYS_FUNCTION_CODE specialFunctionCode)
{
	for(vector<STAFF_ROLE>::iterator iterStaffRole = m_vec_staff_role.begin(); iterStaffRole != m_vec_staff_role.end();iterStaffRole++){
		if(staffId!=iterStaffRole->staffID){
			continue;
		}
		vector<WORD>& vecRoles = iterStaffRole->vecRoleID;
		for(vector<WORD>::iterator iterRole = vecRoles.begin();iterRole!=vecRoles.end();iterRole++){
			for (vector<ROLE_FUNCTION>::iterator iterFunction = m_vec_role_function.begin();iterFunction!=m_vec_role_function.end();iterFunction++){
				if(iterFunction->roleID != *iterRole){
					continue;
				}
				if(iterFunction->functionID == specialFunctionCode){
					return true;
				}
			}
		}
	}
	return false;
}