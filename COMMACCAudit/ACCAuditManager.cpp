#include "stdafx.h"
#include "ACCAuditManager.h"
#include "CACCAuditException.h"
#include "SysInfo.h"
#include "ACCAuditDef.h"
#include "Xfile.h"
#include "DXFile.h"

CACCAuditManager CACCAuditManager::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���캯��   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CACCAuditManager::CACCAuditManager()
{
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CACCAuditManager::~CACCAuditManager()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ȡ�õ�ǰ���Ψһʵ��    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CACCAuditManager& CACCAuditManager::GetInstance()
{
	return instance;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ��ʼ��������ļ��У��ж��Ƿ���AR�ļ����ػ����������û�������½������������ѱ����AR����

@param      (i)CString dataPath          �ļ����·��
@param      (i)CString backupPath     �����ļ�·��

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::Initialize(CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_BackupPath = backupPath;
	if(!CDXFile::CheckAndCreateDirectory(m_dataPath+AR_MANAGE_FOLDER,m_BackupPath+AR_MANAGE_FOLDER))
	{
		throw CARException(CARException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("���ػ������ļ���ʧ�ܣ�" + CString(AR_MANAGE_FOLDER))); 
	}
	InitARItems();
	CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	if(!CDXFile::ResuceFile(accARFileName,accARBackupFileName,true))
	{
		if(ComCheckFileExist(accARFileName) || ComCheckFileExist(accARBackupFileName))//�ļ����ڣ�����û�ж��ػ��ɹ������쳣
		{
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("���ػ��ļ�ʧ�ܣ�" + CString(AR_MANAGE_ACC_FILE_NAME))); 
		}
		SaveACCARToFile(m_ACCAR);//��Ȼ�Ļ��½����������ļ�
	}
	ACCAR savedACCAR;
	LoadACCARFromFile(savedACCAR);
	AddAR(savedACCAR);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ȡ�õ�ǰAR���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
ACCAR& CACCAuditManager::GetCurrentAR()
{
	return m_ACCAR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ���AR���ݣ����������ļ�    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::AddAR(ACCAR &accAR)
{
	for(ACCAR::iterator arIter = accAR.begin();arIter!=accAR.end();arIter++)
	{
		if(m_ACCAR.find(arIter->first)==m_ACCAR.end())
		{
			CString sError;
			sError.Format(_T("%d"),arIter->first);
			throw CARException(CARException::KEY_ERR,_T(__FILE__),__LINE__,_T("����ļ�ֵ")+sError);
		}
		m_ACCAR[arIter->first]+=arIter->second;
	}
	SaveACCARToFile(m_ACCAR);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���ļ��м���AR����   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::LoadACCARFromFile(ACCAR& accAR)
{
	VARIABLE_DATA buffer;
	CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CDXFile accARFile(accARFileName,accARBackupFileName);
	accARFile.ReadBinFile(buffer,true);
	DeserializeACCAR(accAR,buffer);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ����AR���ݵ��ļ�  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::SaveACCARToFile(ACCAR& accAR)
{
	VARIABLE_DATA buffer;
	SerializeACCAR(accAR,buffer);
	CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CDXFile accARFile(accARFileName,accARBackupFileName);
	accARFile.WriteBinFile((const char *)buffer.lpData,buffer.nLen,true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ��AR�������л�    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::SerializeACCAR(ACCAR& accAR,VARIABLE_DATA& buffer)
{
	if(buffer.lpData!=NULL)
	{
		delete buffer.lpData;
		buffer.lpData = NULL;
	}
	int itemCount = accAR.size();
	buffer.nLen = sizeof(int) + itemCount * (sizeof(unsigned short)+sizeof(int));
	buffer.lpData = new BYTE[buffer.nLen];
	LPBYTE lpContent = buffer.lpData;
	memcpy(lpContent,&itemCount,sizeof(int));
	lpContent+=sizeof(int);
	for (ACCAR::iterator itemIter = accAR.begin();itemIter!=accAR.end();itemIter++)
	{
		unsigned short key = itemIter->first;
		int value = itemIter->second;
		memcpy(lpContent,&key,sizeof(unsigned short));
		lpContent+=sizeof(unsigned short);
		memcpy(lpContent,&value,sizeof(int));
		lpContent+=sizeof(int);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     �������������л���AR�ṹ 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::DeserializeACCAR(ACCAR& accAR,VARIABLE_DATA& buffer)
{
	LPBYTE lpContent = buffer.lpData;
	int itemCount;
	memcpy(&itemCount,lpContent,sizeof(int));
	lpContent+=sizeof(int);
	for(int i=0;i<itemCount;i++)
	{
		unsigned short key;
		memcpy(&key,lpContent,sizeof(unsigned short));
		lpContent+=sizeof(unsigned short);
		int value;
		memcpy(&value,lpContent,sizeof(int));
		lpContent+=sizeof(int);
		accAR[key]=value;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��ʼ��AR��  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::InitARItems()
{
	m_ACCAR.clear();
	//m_ACCAR[1]=0; // ʹ���ֽ���Ķ��ڲ�Ʒ����Ŀ��
	//m_ACCAR[2]=0;  // ��������ĳ�����ڲ�Ʒ��ֽ�Һ�Ӳ�ҵ��ܼ�ֵ��                                                                                           
	//m_ACCAR[3]=0; //   // ȡ������ǰ����Ķ��ڲ�Ʒ����Ŀ��                                                                                 
	//m_ACCAR[4]=0; // ȡ������ǰ��ֽ�Һ�Ӳ�ҹ���Ķ��ڲ�Ʒ�ܼ�ֵ��                                                                                   
	//m_ACCAR[5]=0;          // ʹ�õ���ת�˷�ʽ����Ķ��ڲ�Ʒ����Ŀ��                                                                                         
	//m_ACCAR[6]=0;          // ʹ�õ���ת�˷�ʽ����Ķ��ڲ�Ʒ�ļ�ֵ��                                                                                         
	//m_ACCAR[7]=0;          // ȡ������ǰʹ�õ���ת�˷�ʽ����Ķ��ڲ�Ʒ����Ŀ��                                                                               
	//m_ACCAR[8]=0;          // ȡ������ǰʹ�õ���ת�˷�ʽ����Ķ��ڲ�Ʒ�ļ�ֵ��                                                                               
	//m_ACCAR[9]=0;          // ʹ��EFT ���мƴ�Ʊ��                                                                                                           
	//m_ACCAR[10]=0;         // ʹ��EFT ���мƴ�Ʊ��                                                                                                           
	//m_ACCAR[11]=0;         // ȡ����ʹ�ö��ڲ�Ʒ�Ĵ�����                                                                                                     
	//m_ACCAR[12]=0;         // ȡ����ʹ�ö��ڲ�Ʒ�ļ�ֵ��                                                                                                     
	m_ACCAR[13]=0;         // ������Ʊ�Ķ��ڲ�Ʒ��������			//lichao                                                                                                
	m_ACCAR[14]=0;         // ������Ʊ�Ķ��ڲ�Ʒ��������         //lichao                                                                                           
	m_ACCAR[15]=0;         // ʹ���Զ���ֵ��ʽ����Ķ��ڲ�Ʒ����Ŀ��                                                                                         
	m_ACCAR[16]=0;         // ʹ���Զ���ֵ��ʽ����Ķ��ڲ�Ʒ�ļ�ֵ��                                                                                         
	m_ACCAR[17]=0;         // ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����ļƴβ�Ʒ��������                                                                                
	m_ACCAR[18]=0;         // ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����ļƴβ�Ʒ�ļ�ֵ��                                                                                
	m_ACCAR[19]=0;         // ȡ����ǰʹ���ֽ�ֽ�Һ�/ ��Ӳ�ң�����ļƴβ�Ʒ����Ŀ��                                                                       
	m_ACCAR[20]=0;         // ȡ����ǰʹ���ֽ�ֽ�Һ�/ ��Ӳ�ң�����ļƴβ�Ʒ�ļ�ֵ��                                                                       
	m_ACCAR[21]=0;         // ʹ�õ���ת�˷�ʽ����ļƴβ�Ʒ����Ŀ��                                                                                         
	m_ACCAR[22]=0;         // ʹ�õ���ת�˷�ʽ����ļƴβ�Ʒ�ļ�ֵ��                                                                                         
	m_ACCAR[23]=0;         // ȡ����ǰʹ�õ���ת�˷�ʽ����ļƴβ�Ʒ����Ŀ��                                                                                 
	m_ACCAR[24]=0;         // ȡ������ǰʹ�õ���ת�˷�ʽ����ļƴβ�Ʒ�ļ�ֵ��                                                                               
	m_ACCAR[25]=0;         // ʹ���Ż�ȯ���мƴ�Ʊ��������                                                                                                   
	m_ACCAR[26]=0;         // ʹ���Ż�ȯ���мƴβ�Ʒ���ܼ�ֵ��                                                                                               
	m_ACCAR[27]=0;         // ȡ���ļƴβ�Ʒ�Ĵ�����                                                                                                         
	m_ACCAR[28]=0;         // ȡ���ļƴβ�Ʒ�ļ�ֵ��                                                                                                         
	m_ACCAR[29]=0;         // �����˿�ļƴβ�Ʒ����Ŀ��                                                                                                     
	m_ACCAR[30]=0;         // �����˿�ļƴβ�Ʒ�ļ�ֵ��                                                                                                     
	m_ACCAR[31]=0;         // ʹ���Զ���ֵ��ʽ����ļƴβ�Ʒ����Ŀ��                                                                                         
	m_ACCAR[32]=0;         // ʹ���Զ���ֵ��ʽ����ļƴβ�Ʒ�ļ�ֵ��                                                                                         
	m_ACCAR[33]=0;         // ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����Ǯ����ֵ�Ĵ�����                                                                                  
	m_ACCAR[34]=0;         // ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����Ǯ����ֵ�ļ�ֵ��                                                                                  
	m_ACCAR[35]=0;         // ȡ��ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����Ǯ����ֵ�Ĵ�����                                                                              
	m_ACCAR[36]=0;         // ȡ��ʹ���ֽ�ֽ�Һ�/��Ӳ�ң�����Ǯ����ֵ�ļ�ֵ��                                                                              
	m_ACCAR[37]=0;         // ʹ��EFT ����Ǯ����ֵ�Ĵ�����                                                                                                   
	m_ACCAR[38]=0;         // ʹ�õ���ת�˷�ʽ����Ǯ����ֵ�ļ�ֵ��                                                                                           
	m_ACCAR[39]=0;         // ȡ��ʹ�õ���ת�˷�ʽ����Ǯ����ֵ�Ĵ�����                                                                                       
	m_ACCAR[40]=0;         // ȡ��ʹ�õ���ת�˷�ʽ����Ǯ����ֵ�ļ�ֵ��                                                                                       
	m_ACCAR[41]=0;         // Ǯ���˿�Ĵ�����                                                                                                               
	m_ACCAR[42]=0;         // Ǯ���˿�ļ�ֵ��                                                                                                               
	m_ACCAR[43]=0;         // Ǯ���ۿ�Ĵ�����                                                                                                               
	m_ACCAR[44]=0;         // Ǯ���ۿ�ļ�ֵ��                                                                                                               
	m_ACCAR[45]=0;         // ȡ��Ǯ���ۿ�Ĵ�����                                                                                                           
	m_ACCAR[46]=0;         // ȡ��Ǯ���ۿ�ļ�ֵ��                                                                                                           
	m_ACCAR[47]=0;         // ʹ���Զ���ֵ����Ǯ����ֵ�Ĵ�����                                                                                               
	m_ACCAR[48]=0;         // ʹ���Զ���ֵ����Ǯ����ֵ�ļ�ֵ��                                                                                               
	m_ACCAR[49]=0;         // ʹ��EST ���еĲ�Ʒ������                                                                                                       
	m_ACCAR[50]=0;         // ʹ��EST ���в�Ʒ�Ľ�                                                                                                       
	m_ACCAR[51]=0;         // ʹ���Ż�ȯ���в�Ʒ��������                                                                                                     
	m_ACCAR[52]=0;         // ʹ���Ż�ȯ���в�Ʒ��Ľ�                                                                                                   
	m_ACCAR[53]=0;         // ��ƷѺ��Ĵ�����                                                                                                               
	m_ACCAR[54]=0;         // ��ƷѺ��ļ�ֵ��                                                                                                               
	m_ACCAR[55]=0;         // ������Ʊ��Ǯ����Ʒ��������                                                                                                     
	m_ACCAR[56]=0;         // ������Ʊ��Ǯ����Ʒ��������                                                                                                     
	m_ACCAR[57]=0;         // ���и���վ��վ�ļƴ�Ʊ��������                                                                                                 
	m_ACCAR[58]=0;         // ���и���վ��վ�ļƴβ�Ʒ�Ľ�                                                                                               
	m_ACCAR[59]=0;         // ȡ��Ʊ��Ѻ��Ĵ�����                                                                                                           
	m_ACCAR[60]=0;         // ȡ��Ʊ��Ѻ��Ľ�                                                                                                           
	m_ACCAR[61]=0;         // ���˿�Ŀ�Ѻ���������                                                                                                         
	m_ACCAR[62]=0;         // ���˿�Ŀ�Ѻ��Ľ�                                                                                                         
	m_ACCAR[63]=0;         // ʹ��EFT ����Ǯ����Ʒ��������                                                                                                   
	m_ACCAR[64]=0;         // ʹ��EFT ����Ǯ����Ʒ�Ľ�                                                                                                   
	m_ACCAR[65]=0;         // ʹ���Ż�ȯΪǮ����Ʒ��Ʊ��������                                                                                               
	m_ACCAR[66]=0;         // ʹ���Ż�ȯΪǮ����Ʒ��Ʊ�Ľ�                                                                                               
	m_ACCAR[67]=0;         // �����˵Ŀ�Ѻ���վݵĽ�                                                                                                     
	m_ACCAR[68]=0;         // �����˵Ŀ�Ѻ���վݵ�������                                                                                                     
	m_ACCAR[69]=0;         // �����˵Ŀ����õĽ�                                                                                                         
	m_ACCAR[70]=0;         // �����˵Ŀ����õ�������                                                                                                         
	m_ACCAR[71]=0;         // �ֽ����Ʊ�����˿���                                                                                                       
	m_ACCAR[72]=0;         // �ֽ����Ʊ�����˿�����                                                                                                       
	m_ACCAR[73]=0;         // Ʊ���Ŀ����õ�ȡ�ô�����                                                                                                       
	m_ACCAR[74]=0;         // Ʊ���Ŀ����õ�ȡ�ý�                                                                                                       
	m_ACCAR[75]=0;         // ʹ�á�PurseUseOnEntry�� ���׽���Ǯ���ۿ�Ĵ�����                                                                               
	m_ACCAR[76]=0;         // ʹ�á�PurseUseOnEntry�� ���׽���Ǯ���ۿ�ļ�ֵ��                                                                               
	m_ACCAR[77]=0;         // ʹ�á�PurseUseOnExit�� ���׽���Ǯ���ۿ�Ĵ�����                                                                                
	m_ACCAR[78]=0;         // ʹ�á�PurseUseOnExit�� ���׽���Ǯ���ۿ�ļ�ֵ��                                                                                
	m_ACCAR[79]=0;         // The number of multiride adds using coupons.                                                                                    
	m_ACCAR[80]=0;         // The accumulated value of multiride adds using coupons.                                                                         
	m_ACCAR[81]=0;         // �ɡ�PassUseOnEntry�� ���ײ����Ķ��ڲ�Ʒʹ�ô�����                                                                              
	m_ACCAR[82]=0;         // �ɡ�PassUseOnEntry�� ���ײ����Ķ��ڲ�Ʒʹ�ü�ֵ��                                                                              
	m_ACCAR[83]=0;         // �ɡ�PassUseOnExit�� ���ײ����Ķ��ڲ�Ʒʹ�ô�����                                                                               
	m_ACCAR[84]=0;         // �ɡ�PassUseOnExit�� ���ײ����Ķ��ڲ�Ʒʹ�ü�ֵ��                                                                               
	m_ACCAR[85]=0;         // ���˼ƴ������������                                                                                                           
	m_ACCAR[86]=0;         // ���˼ƴ�����ļ�ֵ��                                                                                                           
	m_ACCAR[87]=0;         // ʹ�á�MultirideUseOnEntry�� ���׷����ļƴβ�Ʒʹ�ô�����                                                                       
	m_ACCAR[88]=0;         // ʹ�á�MultirideUseOnEntry�� ���׷����ļƴβ�Ʒʹ�ü�ֵ��                                                                       
	m_ACCAR[89]=0;         // �ɡ�PassUseOnExitMultirideUseOnExit�� ���ײ����Ķ��ڲ�Ʒʹ�ô�����                                                             
	m_ACCAR[90]=0;         // �ɡ�PassUseOnExit�� ���ײ����Ķ��ڲ�Ʒʹ�ü�ֵ��                                                                               
	m_ACCAR[91]=0;         // ͨ��EFT �����Ʊ���˿������                                                                                                   
	m_ACCAR[92]=0;         // ͨ��EFT �����Ʊ���˿�Ľ��                                                                                                   
	m_ACCAR[93]=0;         // ͨ���Żݾ����Ʊ���˿������                                                                                                 
	m_ACCAR[94]=0;         // ͨ���Żݾ����Ʊ���˿�Ľ��                                                                                                 
	m_ACCAR[95]=0;         // ͨ���ֽ���ļƴ�Ʊ��Ʒ���˿�����                                                                                             
	m_ACCAR[96]=0;         // ͨ���ֽ���ļƴ�Ʊ��Ʒ���˿���                                                                                             
	m_ACCAR[97]=0;         // ͨ��EFT ����ļƴ�Ʊ��Ʒ���˿�����                                                                                             
	m_ACCAR[98]=0;         // ͨ��ERT ����ļƴ�Ʊ��Ʒ���˿���                                                                                             
	m_ACCAR[99]=0;         // ͨ���Żݾ���ļƴ�Ʊ��Ʒ���˿�����                                                                                           
	m_ACCAR[100]=0;        // ͨ���Żݾ���ļƴ�Ʊ��Ʒ���˿���                                                                                           
	m_ACCAR[101]=0;        // ͨ��EFT �����Ǯ����Ʒ���˿�����                                                                                               
	m_ACCAR[102]=0;        // ͨ��EFT �����Ǯ����Ʒ���˿���                                                                                               
	m_ACCAR[103]=0;        // ͨ���Żݾ����Ǯ����Ʒ���˿�����                                                                                             
	m_ACCAR[104]=0;        // ͨ���Żݾ����Ǯ����Ʒ���˿���                                                                                             
	m_ACCAR[105]=0;        // ��DelayPurseUseOnEntry ���ײ�����Ǯ��ʹ�ô�����                                                                                
	m_ACCAR[106]=0;        // ��DelayPurseUseOnEntry ���ײ�����Ǯ��ʹ�ü�ֵ��                                                                                
	m_ACCAR[107]=0;        // ��DelayPurseUseOnExit ���ײ�����Ǯ��ʹ�ô�����                                                                                 
	m_ACCAR[108]=0;        // ��DelayPurseUseOnExit ���ײ�����Ǯ��ʹ�ü�ֵ��                                                                                 
	m_ACCAR[109]=0;        // ��DelayMultirideUseOnEntry ���ײ����ļƴβ�Ʒʹ�ô�����                                                                        
	m_ACCAR[110]=0;        // ��DelayMultirideUseOnEntry ���ײ����ļƴβ�Ʒʹ�ü�ֵ��                                                                        
	m_ACCAR[111]=0;        // ��DelayMultirideUseOnExit ���ײ����ļƴβ�Ʒʹ�ô�����                                                                         
	m_ACCAR[112]=0;        // ��DelayMultirideUseOnExit ���ײ����ļƴβ�Ʒʹ�ü�ֵ��                                                                         
	m_ACCAR[113]=0;        // ��DelayPassUseOnEntry ���ײ����Ķ���Ʊʹ�ô�����                                                                               
	m_ACCAR[114]=0;        // ��DelayPassUseOnEntry ���ײ����Ķ���Ʊʹ�ü�ֵ��                                                                               
	m_ACCAR[115]=0;        // ��DelayPassUseOnExit ���ײ����Ķ���Ʊʹ�ô�����                                                                                
	m_ACCAR[116]=0;        // ��DelayPassUseOnExit ���ײ����Ķ���Ʊʹ�ü�ֵ��                                                                                
	m_ACCAR[117]=0;        // �Ż�ȯ�ƴ�������˴���                                                                                                         
	m_ACCAR[118]=0;        // �Ż�ȯ�ƴ�������˼�ֵ.                                                                                                        
	m_ACCAR[119]=0;        // EFT �ƴ�������˴���                                                                                                           
	m_ACCAR[120]=0;        // EFT �ƴ�������˼�ֵ                                                                                                           
	m_ACCAR[121]=0;        // ����Ʊʹ���Ż�ȯ��ֵ������                                                                                                     
	m_ACCAR[122]=0;        // ����Ʊʹ���Ż�ȯ��ֵ��ֵ��                                                                                                     
	m_ACCAR[123]=0;        // ����Ʊ���л����ֽ�Ĵ�����                                                                                                     
	m_ACCAR[124]=0;        // ����Ʊ���л����ֽ�ļ�ֵ��                                                                                                     
	m_ACCAR[125]=0;        // �����Ż�ȯ���˵Ķ���Ʊ���е�����.                                                                                              
	m_ACCAR[126]=0;        // �����Ż�ȯ���˵Ķ���Ʊ���еļ�ֵ.                                                                                              
	m_ACCAR[127]=0;        // ʹ���ֽ����ѷ��еĶ��ڲ�Ʒ����Ŀ��                                                                                           
	m_ACCAR[128]=0;        // ʹ���ֽ����ѷ��еĶ��ڲ�Ʒ�ļ�ֵ��                                                                                           
	m_ACCAR[129]=0;        // ����EFT ���˵Ķ���Ʊ���еĴ�����                                                                                               
	m_ACCAR[130]=0;        // ����EFT ���˵Ķ���Ʊ���еļ�ֵ��                                                                                               
	m_ACCAR[135]=0;        // ʹ��ǩ�˿������ѷ��еĶ��ڲ�Ʒ����Ŀ��                                                                                         
	m_ACCAR[136]=0;        // ���ֽ����Ǯ���ķ��н�                                                                                                   
	m_ACCAR[141]=0;        // ���Ż�ȯ��Ǯ����ֵ�Ĵ�����                                                                                                     
	m_ACCAR[142]=0;        // ���Ż�ȯ��Ǯ����ֵ�ļ�ֵ��                                                                                                     
	m_ACCAR[143]=0;        // ʹ���ֽ����ѷ��еļƴβ�Ʒ����Ŀ��                                                                                           
	m_ACCAR[144]=0;        // ʹ���ֽ����ѷ��еļƴβ�Ʒ�ļ�ֵ��                                                                                           
	m_ACCAR[145]=0;        // Ǯ�����л����ֽ�Ĵ�����                                                                                                       
	m_ACCAR[146]=0;        // Ǯ�����л����ֽ�ļ�ֵ��                                                                                                       
	m_ACCAR[147]=0;        // Ǯ�����л����Ż�ȯ�Ĵ�����                                                                                                     
	m_ACCAR[148]=0;        // Ǯ�����л����Ż�ȯ�ļ�ֵ��                                                                                                     
	m_ACCAR[149]=0;        // Ǯ�����л���EFT�Ĵ�����                                                                                                        
	m_ACCAR[150]=0;        // Ǯ�����л���EFT�ļ�ֵ��                                                                                                        
	m_ACCAR[151]=0;        // ����Ʊ�����Զ���ֵ�Ĵ�����                                                                                                     
	m_ACCAR[152]=0;        // ����Ʊ�����Զ���ֵ�ļ�ֵ��                                                                                                     
	m_ACCAR[153]=0;        // �ƴ�Ʊ���г�ֵ�Ĵ�����                                                                                                         
	m_ACCAR[154]=0;        // �ƴ�Ʊ���г�ֵ�ļ�ֵ��                                                                                                         
	m_ACCAR[155]=0;        // Ǯ����Ʒ�����Զ���ֵ�Ĵ�����                                                                                                   
	m_ACCAR[156]=0;        // Ǯ����Ʒ�����Զ���ֵ�ļ�ֵ��                                                                                                   
	m_ACCAR[157]=0;        // ����Ʊ�Զ���ֵ�˿�Ĵ�����                                                                                                     
	m_ACCAR[158]=0;        // ����Ʊ�Զ���ֵ�˿�ļ�ֵ��                                                                                                     
	m_ACCAR[159]=0;        // �ƴ�Ʊ�Զ���ֵ�˿�Ĵ�����                                                                                                     
	m_ACCAR[160]=0;        // �ƴ�Ʊ�Զ���ֵ�˿�ļ�ֵ��                                                                                                     
	m_ACCAR[161]=0;        // Ǯ���Զ���ֵ�˿�Ĵ�����                                                                                                       
	m_ACCAR[162]=0;        // Ǯ���Զ���ֵ�˿�ļ�ֵ��                                                                                                       
	m_ACCAR[187]=0;        // ͨ��ƾ��Ϊ����Ʊ��ֵ�Ľ��	                                                                                                    
	m_ACCAR[188]=0;        // ͨ��ƾ��Ϊ����Ʊ��ֵ������                                                                                                     
	m_ACCAR[195]=0;        // �ƴ�Ʊͨ��Ʊ�ݳ�ֵ�Ľ��                                                                                                       
	m_ACCAR[196]=0;        // �ƴ�Ʊͨ��Ʊ�ݳ�ֵ������                                                                                                       
	m_ACCAR[203]=0;        // Ǯ����Ʒͨ��Ʊ�ݳ�ֵ�Ľ��                                                                                                     
	m_ACCAR[204]=0;        // Ǯ����Ʒͨ��Ʊ�ݳ�ֵ�Ľ��                                                                                                     
	m_ACCAR[211]=0;        // �ڸ�����Ʊ��ֵʱ��ƾƱ�ݻ�ת�Ľ��                                                                                             
	m_ACCAR[212]=0;        // �ڸ�����Ʊ��ֵʱ��ƾƱ�ݻ�ת������                                                                                             
	m_ACCAR[219]=0;        // �ڸ��ƴ�Ʊ��ֵʱ��ƾƱ�ݻ�ת�Ľ��                                                                                             
	m_ACCAR[220]=0;        // �ڸ��ƴ�Ʊ��ֵʱ��ƾƱ�ݻ�ת������                                                                                             
	m_ACCAR[227]=0;        // �ڸ�Ǯ����ֵʱ��ƾƱ�ݻ�ת�Ľ��                                                                                               
	m_ACCAR[228]=0;        // �ڸ�Ǯ����ֵʱ��ƾƱ�ݻ�ת������                                                                                               
	m_ACCAR[235]=0;        // ��Ʊ�ݷ��еĶ���Ʊ�Ľ��                                                                                                       
	m_ACCAR[236]=0;        // ��Ʊ�ݷ��еĶ���Ʊ������                                                                                                       
	m_ACCAR[243]=0;        // ��Ʊ�ݷ��еļƴ�Ʊ�Ľ��                                                                                                       
	m_ACCAR[244]=0;        // ��Ʊ�ݷ��еļƴ�Ʊ������                                                                                                       
	m_ACCAR[251]=0;        // ��Ʊ�ݷ��е�Ǯ����Ʒ�Ľ��                                                                                                     
	m_ACCAR[252]=0;        // ��Ʊ�ݷ��е�Ǯ����Ʒ������                                                                                                     
	m_ACCAR[259]=0;        // �ڶ���Ʊ����ʱ��ƾƱ�ݻ�ת�Ľ��                                                                                               
	m_ACCAR[260]=0;        // �ڶ���Ʊ����ʱ��ƾƱ�ݻ�ת������                                                                                               
	m_ACCAR[267]=0;        // �ڼƴ�Ʊ����ʱ��ƾƱ�ݻ�ת�Ľ��                                                                                               
	m_ACCAR[268]=0;        // �ڼƴ�Ʊ����ʱ��ƾƱ�ݻ�ת������                                                                                               
	m_ACCAR[275]=0;        // ��Ǯ����Ʒ����ʱ��ƾƱ�ݻ�ת�Ľ��                                                                                             
	m_ACCAR[276]=0;        // ��Ǯ����Ʒ����ʱ��ƾƱ�ݻ�ת������                                                                                             
	m_ACCAR[289]=0;        // ����ʼ��ʱ����ֵ���ӡ�                                                                                                         
	m_ACCAR[290]=0;        // ������ʱ����ֵ���ӡ�                                                                                                           
	m_ACCAR[513]=0;        // Ǯ����Ʒ��Ʊ����                                                                                                               
	m_ACCAR[514]=0;        // �ƴβ�Ʒ��Ʊ����                                                                                                               
	m_ACCAR[515]=0;        // ���ڲ�Ʒ��Ʊ����                                                                                                               
	m_ACCAR[516]=0;        // Ǯ����Ʒ��Ʊ���                                                                                                               
	m_ACCAR[517]=0;        // �ƴβ�Ʒ��Ʊ���                                                                                                               
	m_ACCAR[518]=0;        // ���ڲ�Ʒ��Ʊ���                                                                                                               
	m_ACCAR[519]=0;        // Ǯ����Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[520]=0;        // �ƴβ�Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[521]=0;        // ���ڲ�Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[522]=0;        // Ǯ����Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[523]=0;        // �ƴβ�Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[524]=0;        // ���ڲ�Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[525]=0;        // Ǯ����Ʒ��ֵ����                                                                                                               
	m_ACCAR[526]=0;        // �ƴβ�Ʒ��ֵ����                                                                                                               
	m_ACCAR[527]=0;        // ���ڲ�Ʒ��ֵ����                                                                                                               
	m_ACCAR[528]=0;        // Ǯ����Ʒ��ֵ���                                                                                                               
	m_ACCAR[529]=0;        // �ƴβ�Ʒ��ֵ���                                                                                                               
	m_ACCAR[530]=0;        // ���ڲ�Ʒ��ֵ���                                                                                                               
	m_ACCAR[531]=0;        // Ǯ����Ʒ�˿�����                                                                                                               
	m_ACCAR[532]=0;        // �ƴβ�Ʒ�˿�����                                                                                                               
	m_ACCAR[533]=0;        // ���ڲ�Ʒ�˿�����                                                                                                               
	m_ACCAR[534]=0;        // Ǯ����Ʒ�˿����                                                                                                               
	m_ACCAR[535]=0;        // �ƴβ�Ʒ�˿����                                                                                                               
	m_ACCAR[536]=0;        // ���ڲ�Ʒ�˿����                                                                                                               
	m_ACCAR[537]=0;        // Ǯ����Ʒ��Ѻ������                                                                                                             
	m_ACCAR[538]=0;        // �ƴβ�Ʒ��Ѻ������                                                                                                             
	m_ACCAR[539]=0;        // ���ڲ�Ʒ��Ѻ������                                                                                                             
	m_ACCAR[540]=0;        // Ǯ����Ʒ��Ѻ����                                                                                                             
	m_ACCAR[541]=0;        // �ƴβ�Ʒ��Ѻ����                                                                                                             
	m_ACCAR[542]=0;        // ���ڲ�Ʒ��Ѻ����                                                                                                             
	m_ACCAR[543]=0;        // Ǯ����Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[544]=0;        // �ƴβ�Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[545]=0;        // ���ڲ�Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[546]=0;        // Ǯ����Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[547]=0;        // �ƴβ�Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[548]=0;        // ���ڲ�Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[549]=0;        // Ǯ����Ʒ��վ����                                                                                                               
	m_ACCAR[550]=0;        // �ƴβ�Ʒ��վ����                                                                                                               
	m_ACCAR[551]=0;        // ���ڲ�Ʒ��վ����                                                                                                               
	m_ACCAR[552]=0;        // Ǯ����Ʒ��վ����                                                                                                               
	m_ACCAR[553]=0;        // �ƴβ�Ʒ��վ����                                                                                                               
	m_ACCAR[554]=0;        // ���ڲ�Ʒ��վ����                                                                                                               
	m_ACCAR[555]=0;        // Ǯ����Ʒ��վ���                                                                                                               
	m_ACCAR[556]=0;        // �ƴβ�Ʒ��վ���                                                                                                               
	m_ACCAR[557]=0;        // ���ڲ�Ʒ��վ���                                                                                                               
	m_ACCAR[558]=0;        // Ǯ����Ʒ�滻����                                                                                                               
	m_ACCAR[559]=0;        // �ƴβ�Ʒ�滻����                                                                                                               
	m_ACCAR[560]=0;        // ���ڲ�Ʒ�滻����                                                                                                               
	m_ACCAR[561]=0;        // Ǯ����Ʒ�滻���                                                                                                               
	m_ACCAR[562]=0;        // �ƴβ�Ʒ�滻���                                                                                                               
	m_ACCAR[563]=0;        // ���ڲ�Ʒ�滻���                                                                                                               
	m_ACCAR[564]=0;        // Ǯ����Ʒ��������                                                                                                               
	m_ACCAR[565]=0;        // �ƴβ�Ʒ��������                                                                                                               
	m_ACCAR[566]=0;        // ���ڲ�Ʒ��������                                                                                                               
	m_ACCAR[567]=0;        // Ǯ����Ʒ�������                                                                                                               
	m_ACCAR[568]=0;        // �ƴβ�Ʒ�������                                                                                                               
	m_ACCAR[569]=0;        // ���ڲ�Ʒ�������                                                                                                               
	m_ACCAR[570]=0;        // ����������                                                                                                                     
	m_ACCAR[571]=0;        // �����ѽ��                                                                                                                     
	m_ACCAR[572]=0;        // ��������
	m_ACCAR[573]=0;		 //һƱͨ��ǰUDSN
	m_ACCAR[641]=0;        // Ǯ����Ʒ��Ʊ����                                                                                                               
	m_ACCAR[642]=0;        // �ƴβ�Ʒ��Ʊ����                                                                                                               
	m_ACCAR[643]=0;        // ���ڲ�Ʒ��Ʊ����                                                                                                               
	m_ACCAR[644]=0;        // Ǯ����Ʒ��Ʊ���                                                                                                               
	m_ACCAR[645]=0;        // �ƴβ�Ʒ��Ʊ���                                                                                                               
	m_ACCAR[646]=0;        // ���ڲ�Ʒ��Ʊ���                                                                                                               
	m_ACCAR[647]=0;        // Ǯ����Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[648]=0;        // �ƴβ�Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[649]=0;        // ���ڲ�Ʒ��ƱѺ������                                                                                                           
	m_ACCAR[650]=0;        // Ǯ����Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[651]=0;        // �ƴβ�Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[652]=0;        // ���ڲ�Ʒ��ƱѺ����                                                                                                           
	m_ACCAR[653]=0;        // Ǯ����Ʒ��ֵ����                                                                                                               
	m_ACCAR[654]=0;        // �ƴβ�Ʒ��ֵ����                                                                                                               
	m_ACCAR[655]=0;        // ���ڲ�Ʒ��ֵ����                                                                                                               
	m_ACCAR[656]=0;        // Ǯ����Ʒ��ֵ���                                                                                                               
	m_ACCAR[657]=0;        // �ƴβ�Ʒ��ֵ���                                                                                                               
	m_ACCAR[658]=0;        // ���ڲ�Ʒ��ֵ���                                                                                                               
	m_ACCAR[659]=0;        // Ǯ����Ʒ�˿�����                                                                                                               
	m_ACCAR[660]=0;        // �ƴβ�Ʒ�˿�����                                                                                                               
	m_ACCAR[661]=0;        // ���ڲ�Ʒ�˿�����                                                                                                               
	m_ACCAR[662]=0;        // Ǯ����Ʒ�˿����                                                                                                               
	m_ACCAR[663]=0;        // �ƴβ�Ʒ�˿����                                                                                                               
	m_ACCAR[664]=0;        // ���ڲ�Ʒ�˿����                                                                                                               
	m_ACCAR[665]=0;        // Ǯ����Ʒ��Ѻ������                                                                                                             
	m_ACCAR[666]=0;        // �ƴβ�Ʒ��Ѻ������                                                                                                             
	m_ACCAR[667]=0;        // ���ڲ�Ʒ��Ѻ������                                                                                                             
	m_ACCAR[668]=0;        // Ǯ����Ʒ��Ѻ����                                                                                                             
	m_ACCAR[669]=0;        // �ƴβ�Ʒ��Ѻ����                                                                                                             
	m_ACCAR[670]=0;        // ���ڲ�Ʒ��Ѻ����                                                                                                             
	m_ACCAR[671]=0;        // Ǯ����Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[672]=0;        // �ƴβ�Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[673]=0;        // ���ڲ�Ʒ�ֽ�Ʊ����                                                                                                           
	m_ACCAR[674]=0;        // Ǯ����Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[675]=0;        // �ƴβ�Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[676]=0;        // ���ڲ�Ʒ�ֽ�Ʊ���                                                                                                           
	m_ACCAR[677]=0;        // Ǯ����Ʒ���ڲ�Ʊ����                                                                                                           
	m_ACCAR[678]=0;        // �ƴβ�Ʒ���ڲ�Ʊ����                                                                                                           
	m_ACCAR[679]=0;        // ���ڲ�Ʒ���ڲ�Ʊ����                                                                                                           
	m_ACCAR[680]=0;        // Ǯ����Ʒ���ڲ�Ʊ���                                                                                                           
	m_ACCAR[681]=0;        // �ƴβ�Ʒ���ڲ�Ʊ���                                                                                                           
	m_ACCAR[682]=0;        // ���ڲ�Ʒ���ڲ�Ʊ���                                                                                                           
	m_ACCAR[683]=0;        // Ǯ����Ʒ��վ����                                                                                                               
	m_ACCAR[684]=0;        // �ƴβ�Ʒ��վ����                                                                                                               
	m_ACCAR[685]=0;        // ���ڲ�Ʒ��վ����                                                                                                               
	m_ACCAR[686]=0;        // Ǯ����Ʒ��վ����                                                                                                               
	m_ACCAR[687]=0;        // �ƴβ�Ʒ��վ����                                                                                                               
	m_ACCAR[688]=0;        // ���ڲ�Ʒ��վ����                                                                                                               
	m_ACCAR[689]=0;        // Ǯ����Ʒ��վ���                                                                                                               
	m_ACCAR[690]=0;        // �ƴβ�Ʒ��վ���                                                                                                               
	m_ACCAR[691]=0;        // ���ڲ�Ʒ��վ���                                                                                                               
	m_ACCAR[692]=0;        // Ǯ����Ʒ�滻����                                                                                                               
	m_ACCAR[693]=0;        // �ƴβ�Ʒ�滻����                                                                                                               
	m_ACCAR[694]=0;        // ���ڲ�Ʒ�滻����                                                                                                               
	m_ACCAR[695]=0;        // Ǯ����Ʒ�滻���                                                                                                               
	m_ACCAR[696]=0;        // �ƴβ�Ʒ�滻���                                                                                                               
	m_ACCAR[697]=0;        // ���ڲ�Ʒ�滻���                                                                                                               
	m_ACCAR[698]=0;        // ��������                                                                                                                       
	m_ACCAR[699]=0;        // Ǯ����Ʒ�����Żݳ�վ����                                                                                                       
	m_ACCAR[700]=0;        // �ƴβ�Ʒ�����Żݳ�վ����                                                                                                       
	m_ACCAR[701]=0;        // Ǯ����Ʒ�����Żݳ�վ���                                                                                                       
	m_ACCAR[702]=0;        // �ƴβ�Ʒ�����Żݳ�վ���
	m_ACCAR[703]=0;		  // һ��ͨ��ǰUDSN
	m_ACCAR[801]=0;        // UL��Ʊ��1�ۼƷ�Ʊ����                                                                                                          
	m_ACCAR[802]=0;        // UL��Ʊ��1�ۼƳ�Ʊʧ�ܴ���                                                                                                      
	m_ACCAR[803]=0;        // UL��Ʊ��1�ۼƸ�������                                                                                                          
	m_ACCAR[804]=0;        // UL��Ʊ��2�ۼƷ�Ʊ����                                                                                                          
	m_ACCAR[805]=0;        // UL��Ʊ��2�ۼƳ�Ʊʧ�ܴ���                                                                                                      
	m_ACCAR[806]=0;        // UL��Ʊ��2�ۼƸ�������                                                                                                          
	m_ACCAR[825]=0;        // ��Ʊ��1�ۼƻ��շ�Ʊ����                                                                                                        
	m_ACCAR[826]=0;        // ��Ʊ��1�ۼƿ�Ʊ����                                                                                                            
	m_ACCAR[827]=0;        // ��Ʊ��1�ۼƸ�������                                                                                                            
	m_ACCAR[865]=0;        // TPU1�ۼƶ�д������                                                                                                             
	m_ACCAR[866]=0;        // TPU1�ۼ���Ч������                                                                                                             
	m_ACCAR[867]=0;        // TPU1����ʧ�ܴ���                                                                                                               
	m_ACCAR[868]=0;        // TPU2�ۼƶ�д������                                                                                                             
	m_ACCAR[869]=0;        // TPU2�ۼ���Ч������                                                                                                             
	m_ACCAR[870]=0;        // TPU2����ʧ�ܴ���                                                                                                               
	m_ACCAR[881]=0;        // ��ӡ��ʹ�ô���                                                                                                                 
	m_ACCAR[882]=0;        // ��ӡ�����ϴ���                                                                                                                 
	m_ACCAR[884]=0;        // �豸Ͷ��ʹ��ʱ��                                                                                                               
	m_ACCAR[886]=0;        // �豸��½�ܴ���                                                                                                                 
	m_ACCAR[887]=0;        // �豸��½ʧ���ܴ���                                                                                                             
	m_ACCAR[888]=0;        // �豸�����ܴ���                                                                                                                 
	m_ACCAR[891]=0;        // ��SC����ʧ�ܴ���                                                                                                               
	m_ACCAR[892]=0;        // TPU��������                                                                                                                    
	m_ACCAR[895]=0;        //  һƱͨISAM����������   
}

