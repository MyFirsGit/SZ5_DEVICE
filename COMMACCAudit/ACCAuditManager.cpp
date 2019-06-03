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
@brief   构造函数   

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
@brief    析构函数  

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
@brief  取得当前类的唯一实例    

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
@brief   初始化，检查文件夹，判断是否有AR文件二重化，（如果都没有则新新建），并加载已保存的AR数据

@param      (i)CString dataPath          文件存放路径
@param      (i)CString backupPath     备份文件路径

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
		throw CARException(CARException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("二重化创建文件夹失败：" + CString(AR_MANAGE_FOLDER))); 
	}
	InitARItems();
	CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_ACC_FILE_NAME;
	if(!CDXFile::ResuceFile(accARFileName,accARBackupFileName,true))
	{
		if(ComCheckFileExist(accARFileName) || ComCheckFileExist(accARBackupFileName))//文件存在，并且没有二重化成功，抛异常
		{
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("二重化文件失败：" + CString(AR_MANAGE_ACC_FILE_NAME))); 
		}
		SaveACCARToFile(m_ACCAR);//不然的话新建，并保存文件
	}
	ACCAR savedACCAR;
	LoadACCARFromFile(savedACCAR);
	AddAR(savedACCAR);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得当前AR数据 

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
@brief  添加AR数据，并保存至文件    

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
			throw CARException(CARException::KEY_ERR,_T(__FILE__),__LINE__,_T("错误的键值")+sError);
		}
		m_ACCAR[arIter->first]+=arIter->second;
	}
	SaveACCARToFile(m_ACCAR);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   从文件中加载AR数据   

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
@brief    保存AR数据到文件  

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
@brief  将AR数据序列化    

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
@brief     将二进流反序列化成AR结构 

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
@brief    初始化AR项  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CACCAuditManager::InitARItems()
{
	m_ACCAR.clear();
	//m_ACCAR[1]=0; // 使用现金购买的定期产品的数目。
	//m_ACCAR[2]=0;  // 用来购买某个定期产品的纸币和硬币的总价值。                                                                                           
	//m_ACCAR[3]=0; //   // 取消的以前购买的定期产品的数目。                                                                                 
	//m_ACCAR[4]=0; // 取消的以前用纸币和硬币购买的定期产品总价值。                                                                                   
	//m_ACCAR[5]=0;          // 使用电子转账方式购买的定期产品的数目。                                                                                         
	//m_ACCAR[6]=0;          // 使用电子转账方式购买的定期产品的价值。                                                                                         
	//m_ACCAR[7]=0;          // 取消的以前使用电子转账方式购买的定期产品的数目。                                                                               
	//m_ACCAR[8]=0;          // 取消的以前使用电子转账方式购买的定期产品的价值。                                                                               
	//m_ACCAR[9]=0;          // 使用EFT 发行计次票。                                                                                                           
	//m_ACCAR[10]=0;         // 使用EFT 发行计次票。                                                                                                           
	//m_ACCAR[11]=0;         // 取消的使用定期产品的次数。                                                                                                     
	//m_ACCAR[12]=0;         // 取消的使用定期产品的价值。                                                                                                     
	m_ACCAR[13]=0;         // 发生补票的定期产品的数量。			//lichao                                                                                                
	m_ACCAR[14]=0;         // 发生补票的定期产品的数量。         //lichao                                                                                           
	m_ACCAR[15]=0;         // 使用自动加值方式购买的定期产品的数目。                                                                                         
	m_ACCAR[16]=0;         // 使用自动加值方式购买的定期产品的价值。                                                                                         
	m_ACCAR[17]=0;         // 使用现金（纸币和/或硬币）购买的计次产品的数量。                                                                                
	m_ACCAR[18]=0;         // 使用现金（纸币和/或硬币）购买的计次产品的价值。                                                                                
	m_ACCAR[19]=0;         // 取消以前使用现金（纸币和/ 或硬币）购买的计次产品的数目。                                                                       
	m_ACCAR[20]=0;         // 取消以前使用现金（纸币和/ 或硬币）购买的计次产品的价值。                                                                       
	m_ACCAR[21]=0;         // 使用电子转账方式购买的计次产品的数目。                                                                                         
	m_ACCAR[22]=0;         // 使用电子转账方式购买的计次产品的价值。                                                                                         
	m_ACCAR[23]=0;         // 取消以前使用电子转账方式购买的计次产品的数目。                                                                                 
	m_ACCAR[24]=0;         // 取消的以前使用电子转账方式购买的计次产品的价值。                                                                               
	m_ACCAR[25]=0;         // 使用优惠券发行计次票的数量。                                                                                                   
	m_ACCAR[26]=0;         // 使用优惠券发行计次产品的总价值。                                                                                               
	m_ACCAR[27]=0;         // 取消的计次产品的次数。                                                                                                         
	m_ACCAR[28]=0;         // 取消的计次产品的价值。                                                                                                         
	m_ACCAR[29]=0;         // 发生退款的计次产品的数目。                                                                                                     
	m_ACCAR[30]=0;         // 发生退款的计次产品的价值。                                                                                                     
	m_ACCAR[31]=0;         // 使用自动加值方式购买的计次产品的数目。                                                                                         
	m_ACCAR[32]=0;         // 使用自动加值方式购买的计次产品的价值。                                                                                         
	m_ACCAR[33]=0;         // 使用现金（纸币和/或硬币）进行钱包加值的次数。                                                                                  
	m_ACCAR[34]=0;         // 使用现金（纸币和/或硬币）进行钱包加值的价值。                                                                                  
	m_ACCAR[35]=0;         // 取消使用现金（纸币和/或硬币）进行钱包加值的次数。                                                                              
	m_ACCAR[36]=0;         // 取消使用现金（纸币和/或硬币）进行钱包加值的价值。                                                                              
	m_ACCAR[37]=0;         // 使用EFT 进行钱包加值的次数。                                                                                                   
	m_ACCAR[38]=0;         // 使用电子转账方式进行钱包加值的价值。                                                                                           
	m_ACCAR[39]=0;         // 取消使用电子转账方式进行钱包加值的次数。                                                                                       
	m_ACCAR[40]=0;         // 取消使用电子转账方式进行钱包加值的价值。                                                                                       
	m_ACCAR[41]=0;         // 钱包退款的次数。                                                                                                               
	m_ACCAR[42]=0;         // 钱包退款的价值。                                                                                                               
	m_ACCAR[43]=0;         // 钱包扣款的次数。                                                                                                               
	m_ACCAR[44]=0;         // 钱包扣款的价值。                                                                                                               
	m_ACCAR[45]=0;         // 取消钱包扣款的次数。                                                                                                           
	m_ACCAR[46]=0;         // 取消钱包扣款的价值。                                                                                                           
	m_ACCAR[47]=0;         // 使用自动加值进行钱包加值的次数。                                                                                               
	m_ACCAR[48]=0;         // 使用自动加值进行钱包加值的价值。                                                                                               
	m_ACCAR[49]=0;         // 使用EST 发行的产品数量。                                                                                                       
	m_ACCAR[50]=0;         // 使用EST 发行产品的金额。                                                                                                       
	m_ACCAR[51]=0;         // 使用优惠券发行产品的数量。                                                                                                     
	m_ACCAR[52]=0;         // 使用优惠券发行产品年的金额。                                                                                                   
	m_ACCAR[53]=0;         // 产品押金的次数。                                                                                                               
	m_ACCAR[54]=0;         // 产品押金的价值。                                                                                                               
	m_ACCAR[55]=0;         // 发生补票的钱包产品的数量。                                                                                                     
	m_ACCAR[56]=0;         // 发生补票的钱包产品的数量。                                                                                                     
	m_ACCAR[57]=0;         // 发行给出站车站的计次票的数量。                                                                                                 
	m_ACCAR[58]=0;         // 发行给出站车站的计次产品的金额。                                                                                               
	m_ACCAR[59]=0;         // 取用票卡押金的次数。                                                                                                           
	m_ACCAR[60]=0;         // 取用票卡押金的金额。                                                                                                           
	m_ACCAR[61]=0;         // 被退款的卡押金的数量。                                                                                                         
	m_ACCAR[62]=0;         // 被退款的卡押金的金额。                                                                                                         
	m_ACCAR[63]=0;         // 使用EFT 发行钱包产品的数量。                                                                                                   
	m_ACCAR[64]=0;         // 使用EFT 发行钱包产品的金额。                                                                                                   
	m_ACCAR[65]=0;         // 使用优惠券为钱包产品补票的数量。                                                                                               
	m_ACCAR[66]=0;         // 使用优惠券为钱包产品补票的金额。                                                                                               
	m_ACCAR[67]=0;         // 被回退的卡押金收据的金额。                                                                                                     
	m_ACCAR[68]=0;         // 被回退的卡押金收据的数量。                                                                                                     
	m_ACCAR[69]=0;         // 被回退的卡费用的金额。                                                                                                         
	m_ACCAR[70]=0;         // 被回退的卡费用的数量。                                                                                                         
	m_ACCAR[71]=0;         // 现金购买的票卡的退款金额                                                                                                       
	m_ACCAR[72]=0;         // 现金购买的票卡的退款数量                                                                                                       
	m_ACCAR[73]=0;         // 票卡的卡费用的取用次数。                                                                                                       
	m_ACCAR[74]=0;         // 票卡的卡费用的取用金额。                                                                                                       
	m_ACCAR[75]=0;         // 使用“PurseUseOnEntry” 交易进行钱包扣款的次数。                                                                               
	m_ACCAR[76]=0;         // 使用“PurseUseOnEntry” 交易进行钱包扣款的价值。                                                                               
	m_ACCAR[77]=0;         // 使用“PurseUseOnExit” 交易进行钱包扣款的次数。                                                                                
	m_ACCAR[78]=0;         // 使用“PurseUseOnExit” 交易进行钱包扣款的价值。                                                                                
	m_ACCAR[79]=0;         // The number of multiride adds using coupons.                                                                                    
	m_ACCAR[80]=0;         // The accumulated value of multiride adds using coupons.                                                                         
	m_ACCAR[81]=0;         // 由“PassUseOnEntry” 交易产生的定期产品使用次数。                                                                              
	m_ACCAR[82]=0;         // 由“PassUseOnEntry” 交易产生的定期产品使用价值。                                                                              
	m_ACCAR[83]=0;         // 由“PassUseOnExit” 交易产生的定期产品使用次数。                                                                               
	m_ACCAR[84]=0;         // 由“PassUseOnExit” 交易产生的定期产品使用价值。                                                                               
	m_ACCAR[85]=0;         // 回退计次问题的数量。                                                                                                           
	m_ACCAR[86]=0;         // 回退计次问题的价值。                                                                                                           
	m_ACCAR[87]=0;         // 使用“MultirideUseOnEntry” 交易发生的计次产品使用次数。                                                                       
	m_ACCAR[88]=0;         // 使用“MultirideUseOnEntry” 交易发生的计次产品使用价值。                                                                       
	m_ACCAR[89]=0;         // 由“PassUseOnExitMultirideUseOnExit” 交易产生的定期产品使用次数。                                                             
	m_ACCAR[90]=0;         // 由“PassUseOnExit” 交易产生的定期产品使用价值。                                                                               
	m_ACCAR[91]=0;         // 通过EFT 购买的票卡退款的数量                                                                                                   
	m_ACCAR[92]=0;         // 通过EFT 购买的票卡退款的金额                                                                                                   
	m_ACCAR[93]=0;         // 通过优惠卷购买的票卡退款的数量                                                                                                 
	m_ACCAR[94]=0;         // 通过优惠卷购买的票卡退款的金额                                                                                                 
	m_ACCAR[95]=0;         // 通过现金购买的计次票产品的退款数量                                                                                             
	m_ACCAR[96]=0;         // 通过现金购买的计次票产品的退款金额                                                                                             
	m_ACCAR[97]=0;         // 通过EFT 购买的计次票产品的退款数量                                                                                             
	m_ACCAR[98]=0;         // 通过ERT 购买的计次票产品的退款金额                                                                                             
	m_ACCAR[99]=0;         // 通过优惠卷购买的计次票产品的退款数量                                                                                           
	m_ACCAR[100]=0;        // 通过优惠卷购买的计次票产品的退款金额                                                                                           
	m_ACCAR[101]=0;        // 通过EFT 购买的钱包产品的退款数量                                                                                               
	m_ACCAR[102]=0;        // 通过EFT 购买的钱包产品的退款金额                                                                                               
	m_ACCAR[103]=0;        // 通过优惠卷购买的钱包产品的退款数量                                                                                             
	m_ACCAR[104]=0;        // 通过优惠卷购买的钱包产品的退款金额                                                                                             
	m_ACCAR[105]=0;        // 由DelayPurseUseOnEntry 交易产生的钱包使用次数。                                                                                
	m_ACCAR[106]=0;        // 由DelayPurseUseOnEntry 交易产生的钱包使用价值。                                                                                
	m_ACCAR[107]=0;        // 由DelayPurseUseOnExit 交易产生的钱包使用次数。                                                                                 
	m_ACCAR[108]=0;        // 由DelayPurseUseOnExit 交易产生的钱包使用价值。                                                                                 
	m_ACCAR[109]=0;        // 由DelayMultirideUseOnEntry 交易产生的计次产品使用次数。                                                                        
	m_ACCAR[110]=0;        // 由DelayMultirideUseOnEntry 交易产生的计次产品使用价值。                                                                        
	m_ACCAR[111]=0;        // 由DelayMultirideUseOnExit 交易产生的计次产品使用次数。                                                                         
	m_ACCAR[112]=0;        // 由DelayMultirideUseOnExit 交易产生的计次产品使用价值。                                                                         
	m_ACCAR[113]=0;        // 由DelayPassUseOnEntry 交易产生的定期票使用次数。                                                                               
	m_ACCAR[114]=0;        // 由DelayPassUseOnEntry 交易产生的定期票使用价值。                                                                               
	m_ACCAR[115]=0;        // 由DelayPassUseOnExit 交易产生的定期票使用次数。                                                                                
	m_ACCAR[116]=0;        // 由DelayPassUseOnExit 交易产生的定期票使用价值。                                                                                
	m_ACCAR[117]=0;        // 优惠券计次问题回退次数                                                                                                         
	m_ACCAR[118]=0;        // 优惠券计次问题回退价值.                                                                                                        
	m_ACCAR[119]=0;        // EFT 计次问题回退次数                                                                                                           
	m_ACCAR[120]=0;        // EFT 计次问题回退价值                                                                                                           
	m_ACCAR[121]=0;        // 定期票使用优惠券充值次数。                                                                                                     
	m_ACCAR[122]=0;        // 定期票使用优惠券充值价值。                                                                                                     
	m_ACCAR[123]=0;        // 定期票发行回退现金的次数。                                                                                                     
	m_ACCAR[124]=0;        // 定期票发行回退现金的价值。                                                                                                     
	m_ACCAR[125]=0;        // 基于优惠券回退的定期票发行的数量.                                                                                              
	m_ACCAR[126]=0;        // 基于优惠券回退的定期票发行的价值.                                                                                              
	m_ACCAR[127]=0;        // 使用现金购买已发行的定期产品的数目。                                                                                           
	m_ACCAR[128]=0;        // 使用现金购买已发行的定期产品的价值。                                                                                           
	m_ACCAR[129]=0;        // 基于EFT 回退的定期票发行的次数。                                                                                               
	m_ACCAR[130]=0;        // 基于EFT 回退的定期票发行的价值。                                                                                               
	m_ACCAR[135]=0;        // 使用签账卡购买已发行的定期产品的数目。                                                                                         
	m_ACCAR[136]=0;        // 用现金购买的钱包的发行金额。                                                                                                   
	m_ACCAR[141]=0;        // 用优惠券给钱包充值的次数。                                                                                                     
	m_ACCAR[142]=0;        // 用优惠券给钱包充值的价值。                                                                                                     
	m_ACCAR[143]=0;        // 使用现金购买已发行的计次产品的数目。                                                                                           
	m_ACCAR[144]=0;        // 使用现金购买已发行的计次产品的价值。                                                                                           
	m_ACCAR[145]=0;        // 钱包发行回退现金的次数。                                                                                                       
	m_ACCAR[146]=0;        // 钱包发行回退现金的价值。                                                                                                       
	m_ACCAR[147]=0;        // 钱包发行回退优惠券的次数。                                                                                                     
	m_ACCAR[148]=0;        // 钱包发行回退优惠券的价值。                                                                                                     
	m_ACCAR[149]=0;        // 钱包发行回退EFT的次数。                                                                                                        
	m_ACCAR[150]=0;        // 钱包发行回退EFT的价值。                                                                                                        
	m_ACCAR[151]=0;        // 定期票发行自动充值的次数。                                                                                                     
	m_ACCAR[152]=0;        // 定期票发行自动充值的价值。                                                                                                     
	m_ACCAR[153]=0;        // 计次票发行充值的次数。                                                                                                         
	m_ACCAR[154]=0;        // 计次票发行充值的价值。                                                                                                         
	m_ACCAR[155]=0;        // 钱包产品发行自动充值的次数。                                                                                                   
	m_ACCAR[156]=0;        // 钱包产品发行自动充值的价值。                                                                                                   
	m_ACCAR[157]=0;        // 定期票自动充值退款的次数。                                                                                                     
	m_ACCAR[158]=0;        // 定期票自动充值退款的价值。                                                                                                     
	m_ACCAR[159]=0;        // 计次票自动充值退款的次数。                                                                                                     
	m_ACCAR[160]=0;        // 计次票自动充值退款的价值。                                                                                                     
	m_ACCAR[161]=0;        // 钱包自动充值退款的次数。                                                                                                       
	m_ACCAR[162]=0;        // 钱包自动充值退款的价值。                                                                                                       
	m_ACCAR[187]=0;        // 通过凭据为定期票充值的金额	                                                                                                    
	m_ACCAR[188]=0;        // 通过凭据为定期票充值的数量                                                                                                     
	m_ACCAR[195]=0;        // 计次票通过票据充值的金额                                                                                                       
	m_ACCAR[196]=0;        // 计次票通过票据充值的数量                                                                                                       
	m_ACCAR[203]=0;        // 钱包产品通过票据充值的金额                                                                                                     
	m_ACCAR[204]=0;        // 钱包产品通过票据充值的金额                                                                                                     
	m_ACCAR[211]=0;        // 在给定期票充值时，凭票据回转的金额                                                                                             
	m_ACCAR[212]=0;        // 在给定期票充值时，凭票据回转的数量                                                                                             
	m_ACCAR[219]=0;        // 在给计次票充值时，凭票据回转的金额                                                                                             
	m_ACCAR[220]=0;        // 在给计次票充值时，凭票据回转的数量                                                                                             
	m_ACCAR[227]=0;        // 在给钱包充值时，凭票据回转的金额                                                                                               
	m_ACCAR[228]=0;        // 在给钱包充值时，凭票据回转的数量                                                                                               
	m_ACCAR[235]=0;        // 用票据发行的定期票的金额                                                                                                       
	m_ACCAR[236]=0;        // 用票据发行的定期票的数量                                                                                                       
	m_ACCAR[243]=0;        // 用票据发行的计次票的金额                                                                                                       
	m_ACCAR[244]=0;        // 用票据发行的计次票的数量                                                                                                       
	m_ACCAR[251]=0;        // 用票据发行的钱包产品的金额                                                                                                     
	m_ACCAR[252]=0;        // 用票据发行的钱包产品的数量                                                                                                     
	m_ACCAR[259]=0;        // 在定期票发行时，凭票据回转的金额                                                                                               
	m_ACCAR[260]=0;        // 在定期票发行时，凭票据回转的数量                                                                                               
	m_ACCAR[267]=0;        // 在计次票发行时，凭票据回转的金额                                                                                               
	m_ACCAR[268]=0;        // 在计次票发行时，凭票据回转的数量                                                                                               
	m_ACCAR[275]=0;        // 在钱包产品发行时，凭票据回转的金额                                                                                             
	m_ACCAR[276]=0;        // 在钱包产品发行时，凭票据回转的数量                                                                                             
	m_ACCAR[289]=0;        // 卡初始化时，该值增加。                                                                                                         
	m_ACCAR[290]=0;        // 卡发行时，该值增加。                                                                                                           
	m_ACCAR[513]=0;        // 钱包产品售票数量                                                                                                               
	m_ACCAR[514]=0;        // 计次产品售票数量                                                                                                               
	m_ACCAR[515]=0;        // 定期产品售票数量                                                                                                               
	m_ACCAR[516]=0;        // 钱包产品售票金额                                                                                                               
	m_ACCAR[517]=0;        // 计次产品售票金额                                                                                                               
	m_ACCAR[518]=0;        // 定期产品售票金额                                                                                                               
	m_ACCAR[519]=0;        // 钱包产品售票押金数量                                                                                                           
	m_ACCAR[520]=0;        // 计次产品售票押金数量                                                                                                           
	m_ACCAR[521]=0;        // 定期产品售票押金数量                                                                                                           
	m_ACCAR[522]=0;        // 钱包产品售票押金金额                                                                                                           
	m_ACCAR[523]=0;        // 计次产品售票押金金额                                                                                                           
	m_ACCAR[524]=0;        // 定期产品售票押金金额                                                                                                           
	m_ACCAR[525]=0;        // 钱包产品充值数量                                                                                                               
	m_ACCAR[526]=0;        // 计次产品充值数量                                                                                                               
	m_ACCAR[527]=0;        // 定期产品充值数量                                                                                                               
	m_ACCAR[528]=0;        // 钱包产品充值金额                                                                                                               
	m_ACCAR[529]=0;        // 计次产品充值金额                                                                                                               
	m_ACCAR[530]=0;        // 定期产品充值金额                                                                                                               
	m_ACCAR[531]=0;        // 钱包产品退卡数量                                                                                                               
	m_ACCAR[532]=0;        // 计次产品退卡数量                                                                                                               
	m_ACCAR[533]=0;        // 定期产品退卡数量                                                                                                               
	m_ACCAR[534]=0;        // 钱包产品退卡金额                                                                                                               
	m_ACCAR[535]=0;        // 计次产品退卡金额                                                                                                               
	m_ACCAR[536]=0;        // 定期产品退卡金额                                                                                                               
	m_ACCAR[537]=0;        // 钱包产品退押金数量                                                                                                             
	m_ACCAR[538]=0;        // 计次产品退押金数量                                                                                                             
	m_ACCAR[539]=0;        // 定期产品退押金数量                                                                                                             
	m_ACCAR[540]=0;        // 钱包产品退押金金额                                                                                                             
	m_ACCAR[541]=0;        // 计次产品退押金金额                                                                                                             
	m_ACCAR[542]=0;        // 定期产品退押金金额                                                                                                             
	m_ACCAR[543]=0;        // 钱包产品现金补票数量                                                                                                           
	m_ACCAR[544]=0;        // 计次产品现金补票数量                                                                                                           
	m_ACCAR[545]=0;        // 定期产品现金补票数量                                                                                                           
	m_ACCAR[546]=0;        // 钱包产品现金补票金额                                                                                                           
	m_ACCAR[547]=0;        // 计次产品现金补票金额                                                                                                           
	m_ACCAR[548]=0;        // 定期产品现金补票金额                                                                                                           
	m_ACCAR[549]=0;        // 钱包产品进站数量                                                                                                               
	m_ACCAR[550]=0;        // 计次产品进站数量                                                                                                               
	m_ACCAR[551]=0;        // 定期产品进站数量                                                                                                               
	m_ACCAR[552]=0;        // 钱包产品出站数量                                                                                                               
	m_ACCAR[553]=0;        // 计次产品出站数量                                                                                                               
	m_ACCAR[554]=0;        // 定期产品出站数量                                                                                                               
	m_ACCAR[555]=0;        // 钱包产品出站金额                                                                                                               
	m_ACCAR[556]=0;        // 计次产品出站金额                                                                                                               
	m_ACCAR[557]=0;        // 定期产品出站金额                                                                                                               
	m_ACCAR[558]=0;        // 钱包产品替换数量                                                                                                               
	m_ACCAR[559]=0;        // 计次产品替换数量                                                                                                               
	m_ACCAR[560]=0;        // 定期产品替换数量                                                                                                               
	m_ACCAR[561]=0;        // 钱包产品替换金额                                                                                                               
	m_ACCAR[562]=0;        // 计次产品替换金额                                                                                                               
	m_ACCAR[563]=0;        // 定期产品替换金额                                                                                                               
	m_ACCAR[564]=0;        // 钱包产品抵消数量                                                                                                               
	m_ACCAR[565]=0;        // 计次产品抵消数量                                                                                                               
	m_ACCAR[566]=0;        // 定期产品抵消数量                                                                                                               
	m_ACCAR[567]=0;        // 钱包产品抵消金额                                                                                                               
	m_ACCAR[568]=0;        // 计次产品抵消金额                                                                                                               
	m_ACCAR[569]=0;        // 定期产品抵消金额                                                                                                               
	m_ACCAR[570]=0;        // 手续费数量                                                                                                                     
	m_ACCAR[571]=0;        // 手续费金额                                                                                                                     
	m_ACCAR[572]=0;        // 锁卡数量
	m_ACCAR[573]=0;		 //一票通当前UDSN
	m_ACCAR[641]=0;        // 钱包产品售票数量                                                                                                               
	m_ACCAR[642]=0;        // 计次产品售票数量                                                                                                               
	m_ACCAR[643]=0;        // 定期产品售票数量                                                                                                               
	m_ACCAR[644]=0;        // 钱包产品售票金额                                                                                                               
	m_ACCAR[645]=0;        // 计次产品售票金额                                                                                                               
	m_ACCAR[646]=0;        // 定期产品售票金额                                                                                                               
	m_ACCAR[647]=0;        // 钱包产品售票押金数量                                                                                                           
	m_ACCAR[648]=0;        // 计次产品售票押金数量                                                                                                           
	m_ACCAR[649]=0;        // 定期产品售票押金数量                                                                                                           
	m_ACCAR[650]=0;        // 钱包产品售票押金金额                                                                                                           
	m_ACCAR[651]=0;        // 计次产品售票押金金额                                                                                                           
	m_ACCAR[652]=0;        // 定期产品售票押金金额                                                                                                           
	m_ACCAR[653]=0;        // 钱包产品充值数量                                                                                                               
	m_ACCAR[654]=0;        // 计次产品充值数量                                                                                                               
	m_ACCAR[655]=0;        // 定期产品充值数量                                                                                                               
	m_ACCAR[656]=0;        // 钱包产品充值金额                                                                                                               
	m_ACCAR[657]=0;        // 计次产品充值金额                                                                                                               
	m_ACCAR[658]=0;        // 定期产品充值金额                                                                                                               
	m_ACCAR[659]=0;        // 钱包产品退卡数量                                                                                                               
	m_ACCAR[660]=0;        // 计次产品退卡数量                                                                                                               
	m_ACCAR[661]=0;        // 定期产品退卡数量                                                                                                               
	m_ACCAR[662]=0;        // 钱包产品退卡金额                                                                                                               
	m_ACCAR[663]=0;        // 计次产品退卡金额                                                                                                               
	m_ACCAR[664]=0;        // 定期产品退卡金额                                                                                                               
	m_ACCAR[665]=0;        // 钱包产品退押金数量                                                                                                             
	m_ACCAR[666]=0;        // 计次产品退押金数量                                                                                                             
	m_ACCAR[667]=0;        // 定期产品退押金数量                                                                                                             
	m_ACCAR[668]=0;        // 钱包产品退押金金额                                                                                                             
	m_ACCAR[669]=0;        // 计次产品退押金金额                                                                                                             
	m_ACCAR[670]=0;        // 定期产品退押金金额                                                                                                             
	m_ACCAR[671]=0;        // 钱包产品现金补票数量                                                                                                           
	m_ACCAR[672]=0;        // 计次产品现金补票数量                                                                                                           
	m_ACCAR[673]=0;        // 定期产品现金补票数量                                                                                                           
	m_ACCAR[674]=0;        // 钱包产品现金补票金额                                                                                                           
	m_ACCAR[675]=0;        // 计次产品现金补票金额                                                                                                           
	m_ACCAR[676]=0;        // 定期产品现金补票金额                                                                                                           
	m_ACCAR[677]=0;        // 钱包产品卡内补票数量                                                                                                           
	m_ACCAR[678]=0;        // 计次产品卡内补票数量                                                                                                           
	m_ACCAR[679]=0;        // 定期产品卡内补票数量                                                                                                           
	m_ACCAR[680]=0;        // 钱包产品卡内补票金额                                                                                                           
	m_ACCAR[681]=0;        // 计次产品卡内补票金额                                                                                                           
	m_ACCAR[682]=0;        // 定期产品卡内补票金额                                                                                                           
	m_ACCAR[683]=0;        // 钱包产品进站数量                                                                                                               
	m_ACCAR[684]=0;        // 计次产品进站数量                                                                                                               
	m_ACCAR[685]=0;        // 定期产品进站数量                                                                                                               
	m_ACCAR[686]=0;        // 钱包产品出站数量                                                                                                               
	m_ACCAR[687]=0;        // 计次产品出站数量                                                                                                               
	m_ACCAR[688]=0;        // 定期产品出站数量                                                                                                               
	m_ACCAR[689]=0;        // 钱包产品出站金额                                                                                                               
	m_ACCAR[690]=0;        // 计次产品出站金额                                                                                                               
	m_ACCAR[691]=0;        // 定期产品出站金额                                                                                                               
	m_ACCAR[692]=0;        // 钱包产品替换数量                                                                                                               
	m_ACCAR[693]=0;        // 计次产品替换数量                                                                                                               
	m_ACCAR[694]=0;        // 定期产品替换数量                                                                                                               
	m_ACCAR[695]=0;        // 钱包产品替换金额                                                                                                               
	m_ACCAR[696]=0;        // 计次产品替换金额                                                                                                               
	m_ACCAR[697]=0;        // 定期产品替换金额                                                                                                               
	m_ACCAR[698]=0;        // 锁卡数量                                                                                                                       
	m_ACCAR[699]=0;        // 钱包产品联程优惠出站数量                                                                                                       
	m_ACCAR[700]=0;        // 计次产品联程优惠出站数量                                                                                                       
	m_ACCAR[701]=0;        // 钱包产品联程优惠出站金额                                                                                                       
	m_ACCAR[702]=0;        // 计次产品联程优惠出站金额
	m_ACCAR[703]=0;		  // 一卡通当前UDSN
	m_ACCAR[801]=0;        // UL发票箱1累计发票数量                                                                                                          
	m_ACCAR[802]=0;        // UL发票箱1累计出票失败次数                                                                                                      
	m_ACCAR[803]=0;        // UL发票箱1累计更换次数                                                                                                          
	m_ACCAR[804]=0;        // UL发票箱2累计发票数量                                                                                                          
	m_ACCAR[805]=0;        // UL发票箱2累计出票失败次数                                                                                                      
	m_ACCAR[806]=0;        // UL发票箱2累计更换次数                                                                                                          
	m_ACCAR[825]=0;        // 废票箱1累计回收废票数量                                                                                                        
	m_ACCAR[826]=0;        // 废票箱1累计卡票次数                                                                                                            
	m_ACCAR[827]=0;        // 废票箱1累计更换次数                                                                                                            
	m_ACCAR[865]=0;        // TPU1累计读写卡次数                                                                                                             
	m_ACCAR[866]=0;        // TPU1累计无效卡次数                                                                                                             
	m_ACCAR[867]=0;        // TPU1读卡失败次数                                                                                                               
	m_ACCAR[868]=0;        // TPU2累计读写卡次数                                                                                                             
	m_ACCAR[869]=0;        // TPU2累计无效卡次数                                                                                                             
	m_ACCAR[870]=0;        // TPU2读卡失败次数                                                                                                               
	m_ACCAR[881]=0;        // 打印机使用次数                                                                                                                 
	m_ACCAR[882]=0;        // 打印机故障次数                                                                                                                 
	m_ACCAR[884]=0;        // 设备投入使用时间                                                                                                               
	m_ACCAR[886]=0;        // 设备登陆总次数                                                                                                                 
	m_ACCAR[887]=0;        // 设备登陆失败总次数                                                                                                             
	m_ACCAR[888]=0;        // 设备故障总次数                                                                                                                 
	m_ACCAR[891]=0;        // 与SC连接失败次数                                                                                                               
	m_ACCAR[892]=0;        // TPU更换次数                                                                                                                    
	m_ACCAR[895]=0;        //  一票通ISAM卡更换次数   
}

