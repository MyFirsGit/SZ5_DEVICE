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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcAccessLevelParam::CAfcAccessLevelParam(void) : CAfcParamMaster()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcAccessLevelParam::~CAfcAccessLevelParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1007);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcAccessLevelParam::ParseBody(LPBYTE lpData)
{
    try{
		// ƫ��������	
		lpData += m_section[0].startPosition;
		// �ֶ�1������Ա��ɫ������� 
		if (m_section[0].recordCount != 0) {
			// ����Ա����
			DWORD staffCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData +=4;	
			// vector�ռ����
			m_vec_staff_role.clear();
			m_vec_staff_role.reserve(staffCnt);			

			for (unsigned int i=0; i<staffCnt; i++) {  
				STAFF_ROLE tempRecord;
				// ����Ա ID 
				tempRecord.staffID = BCD2int((char*)lpData,3);			
				lpData += 3;
				// ��ɫID����
				DWORD roleIdCnt = *lpData;				
				lpData ++;	
				// vector�ռ����
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

		// �ֶ�2����ɫ������Ϣ����
		if (m_section[1].recordCount != 0) {
			// ��ɫ��������
			DWORD roleFunctionCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData +=4;	
			// vector�ռ����
			m_vec_role_function.clear();
			m_vec_role_function.reserve(roleFunctionCnt);			

			for (unsigned int k=0; k<roleFunctionCnt; k++) {  
				ROLE_FUNCTION tempRecord2;
				// ��ɫID 
				tempRecord2.roleID = mstos(ComMakeWORD(*lpData, *(lpData + 1)));
				lpData += 2;
				// ��ɫ���ܱ��
				tempRecord2.functionID = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
				lpData +=4;				
				m_vec_role_function.push_back(tempRecord2);
			}
		}
    }
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

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
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ľṹ��������������ȷ��

@param      (i)LPBYTE           ������Ϣ

@retval     bool   true:��ȷ  false������ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcAccessLevelParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ò���ԱȨ��

@param      (i)DWORD staffId						 ����ԱID
@param      (o)STAFF_ACCESS_LEVEL& staffAccessLevel  ����ԱȨ��

@retval     none

@exception  ��
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