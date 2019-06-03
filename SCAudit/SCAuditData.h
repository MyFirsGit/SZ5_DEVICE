#pragma once

#include <map>
#include <vector>
#include "Util.h"
#include "SCAuditException.h"
#include "SCAuditDef.h"


using namespace std;

typedef struct _tagSCAuditKey
{
	int serviceID;						//����ID
	short cardIssuancer;			//Ʊ��������
	short ticketType;				//��Ʊ����
	char	flag;							// Ԥ�����λ

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ���캯��

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	_tagSCAuditKey()
	{
		serviceID = 0;
		cardIssuancer = 0;
		ticketType =0;
		flag = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      �������캯��

	@param      

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	_tagSCAuditKey(const _tagSCAuditKey& another)
	{
		this->serviceID = another.serviceID;
		this->cardIssuancer = another.cardIssuancer;
		this->ticketType = another.ticketType;
		this->flag = another.flag;
	}
	_tagSCAuditKey& _tagSCAuditKey::operator=( const _tagSCAuditKey& another)
	{
		this->serviceID = another.serviceID;
		this->cardIssuancer = another.cardIssuancer;
		this->ticketType = another.ticketType;
		this->flag = another.flag;
		return *this;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ==�����������

	@param      

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	bool _tagSCAuditKey::operator==(const _tagSCAuditKey& another)  const 
	{
		if(serviceID != another.serviceID) return false;
		if(cardIssuancer!=another.cardIssuancer) return false;
		if(ticketType!=another.ticketType) return false;
		if(flag != another.flag) return false;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ���л�

	@param      

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	void Serialize(LPBYTE lpByte) const 
	{
		memcpy(lpByte,&serviceID,sizeof(serviceID));
		lpByte+=sizeof(int);
		memcpy(lpByte,&cardIssuancer,sizeof(cardIssuancer));
		lpByte+=sizeof(short);
		memcpy(lpByte,&ticketType,sizeof(ticketType));
		lpByte+=sizeof(short);
		memcpy(lpByte,&flag,sizeof(flag));
		lpByte+=sizeof(char);
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      �����л�

	@param      

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	void Deserialize(LPBYTE lpByte)
	{
		memcpy(&serviceID,lpByte,sizeof(serviceID));
		lpByte+=sizeof(serviceID);
		memcpy(&cardIssuancer,lpByte,sizeof(cardIssuancer));
		lpByte+=sizeof(cardIssuancer);
		memcpy(&ticketType,lpByte,sizeof(ticketType));
		lpByte+=sizeof(ticketType);
		memcpy(&flag,lpByte,sizeof(flag));
		lpByte+=sizeof(flag);
	}
} SCAuditKey;

//////////////////////////////////////////////////////////////////////////
/*
@brief      SCAuditKey�Ƚ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
typedef struct _tagPurchaseKeyCompare
{
	bool operator()(const SCAuditKey& one,const SCAuditKey& another) const 
	{
		if(one.serviceID<another.serviceID) return true;
		if(one.serviceID>another.serviceID) return false;
		if(one.cardIssuancer<another.cardIssuancer) return true;
		if(one.cardIssuancer>another.cardIssuancer) return false;
		if(one.ticketType<another.ticketType) return true;
		if(one.ticketType>another.ticketType) return false;
		if(one.flag<another.flag) return true;
		if(one.flag>another.flag) return false;
		return false;
	}
} PurchaseKeyCompare;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ֵ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
typedef struct _tagPurchaseValue
{
	int transCount;				// ����������
	int depositAmount;		//Ѻ���ܽ��
	int transAmount;				//�����ܽ��
	int serviceFeeAmount;	//�����ѽ��
	int cardCostAmount;		//���ɱ���
	int recyleCardNum;		//���տ�����
	_tagPurchaseValue()
	{
		transCount = 0;
		depositAmount =0;
		transAmount = 0;
		serviceFeeAmount = 0;
		cardCostAmount =0;
		recyleCardNum = 0;
	}
	_tagPurchaseValue& _tagPurchaseValue::operator+=(const _tagPurchaseValue& another)
	{
		transCount += another.transCount;
		depositAmount += another.depositAmount;
		transAmount += another.transAmount;
		serviceFeeAmount += another.serviceFeeAmount;
		cardCostAmount = another.cardCostAmount;
		recyleCardNum += another.recyleCardNum;
		return *this;
	}

	void Serialize(LPBYTE lpByte) const 
	{
		memcpy(lpByte,&transCount,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(lpByte,&depositAmount,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(lpByte,&transAmount,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(lpByte,&serviceFeeAmount,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(lpByte,&cardCostAmount,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(lpByte,&recyleCardNum,sizeof(int));
		lpByte+=sizeof(int);
	}
	void Deserialize(LPBYTE lpByte)  
	{
		memcpy(&transCount,lpByte,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(&depositAmount,lpByte,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(&transAmount,lpByte,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(&serviceFeeAmount,lpByte,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(&cardCostAmount,lpByte,sizeof(int));
		lpByte+=sizeof(int);
		memcpy(&recyleCardNum,lpByte,sizeof(int));
		lpByte+=sizeof(int);
	}
} SCAuditValue;

typedef map<SCAuditKey,SCAuditValue,PurchaseKeyCompare> PurchaseItemMap;

typedef struct _tagSCAuditData
{
	_DATE_TIME loginTime;				 // ��¼ʱ��
	_DATE businessDate;					 // ִ�м��Ƶ�ϵͳ��Ӫ��
	CString staffID;					 // Ա��ID
	_DATE_TIME auditTimeFrom;			 // ��ҵ��ʼʱ��
	_DATE_TIME auditTimeTo;				 // ��ҵ����ʱ��
	int accRefundFaultCount;					// һƱͨ�����˿�������
	int accRefundFaultAmount;					// һƱͨ�����˿��ܽ��
	int accRecyleTicketCount;				 // һƱͨ�������۳�Ʊ������
	int accRecyleTicketAmount;			// һƱͨ�������۳�Ʊ�ܽ��
	int incomeAmount;									// �����ֽ��ܽ��
	int receivableAmount;						 // Ӧ���ֽ��ܽ��
	int refundAmount;						 // �˿��ֽ��ܽ��
	int recyleTicketCount;				 // ���ճ�Ʊ������
	PurchaseItemMap purchaseItems;
	_tagSCAuditData()
	{
		staffID = "000000";
		accRecyleTicketAmount = 0;
		accRecyleTicketCount = 0;
		accRefundFaultAmount = 0;
		accRefundFaultCount = 0;
		incomeAmount = 0;
		receivableAmount = 0;
		refundAmount = 0;
		recyleTicketCount = 0;
	}
	void Serialize(VARIABLE_DATA& buffer)
	{
		buffer.nLen = sizeof(loginTime) + sizeof(businessDate) + PURCHASE_MANAGE_STAFF_ID_LEN + sizeof(auditTimeFrom)
			+ sizeof(auditTimeTo) + sizeof(accRefundFaultCount) + sizeof(accRefundFaultAmount) + sizeof(accRecyleTicketCount)
			+sizeof(accRecyleTicketAmount) + sizeof(incomeAmount) + sizeof(receivableAmount) + sizeof(refundAmount)
			+sizeof(recyleTicketCount) + sizeof(purchaseItems.size()) + purchaseItems.size() * (sizeof(SCAuditKey) + sizeof(SCAuditValue));
		buffer.lpData = new BYTE[buffer.nLen];
		memset(buffer.lpData,0,buffer.nLen);
		LPBYTE lpContent = buffer.lpData;
		loginTime.Serialize(lpContent);
		lpContent+=sizeof(loginTime);
		businessDate.Serialize(lpContent);
		lpContent+=sizeof(businessDate);
		memcpy(lpContent,staffID.GetBuffer(),PURCHASE_MANAGE_STAFF_ID_LEN);
		lpContent+=PURCHASE_MANAGE_STAFF_ID_LEN;
		auditTimeFrom.Serialize(lpContent);
		lpContent+=sizeof(auditTimeFrom);
		auditTimeTo.Serialize(lpContent);
		lpContent+=sizeof(auditTimeTo);;
		memcpy(lpContent,&accRefundFaultCount,sizeof(accRefundFaultCount));
		lpContent+=sizeof(accRefundFaultCount);
		memcpy(lpContent,&accRefundFaultAmount,sizeof(accRefundFaultAmount));
		lpContent+=sizeof(accRefundFaultAmount);
		memcpy(lpContent,&accRecyleTicketCount,sizeof(accRecyleTicketCount));
		lpContent+=sizeof(accRecyleTicketCount);
		memcpy(lpContent,&accRecyleTicketAmount,sizeof(accRecyleTicketAmount));
		lpContent+=sizeof(accRecyleTicketAmount);
		memcpy(lpContent,&incomeAmount,sizeof(incomeAmount));
		lpContent+=sizeof(incomeAmount);
		memcpy(lpContent,&receivableAmount,sizeof(receivableAmount));
		lpContent+=sizeof(receivableAmount);
		memcpy(lpContent,&refundAmount,sizeof(refundAmount));
		lpContent+=sizeof(refundAmount);
		memcpy(lpContent,&recyleTicketCount,sizeof(recyleTicketCount));
		lpContent+=sizeof(recyleTicketCount);
		size_t businessCount = purchaseItems.size();
		memcpy(lpContent,&businessCount,sizeof(businessCount));
		lpContent+=sizeof(businessCount);
		for(PurchaseItemMap::iterator it = purchaseItems.begin();it!=purchaseItems.end();it++)
		{
			 it->first.Serialize(lpContent);
			lpContent+=sizeof(SCAuditKey);
			it->second.Serialize(lpContent);
			lpContent+=sizeof(SCAuditValue);
		}
	}

	void Deserialize(VARIABLE_DATA& buffer)
	{
		LPBYTE lpContent = buffer.lpData;
		loginTime.Deserialize(lpContent);
		lpContent+=sizeof(loginTime);
		businessDate.Deserialize(lpContent);
		lpContent+=sizeof(businessDate);
		staffID.SetString((TCHAR*)lpContent,PURCHASE_MANAGE_STAFF_ID_LEN);
		lpContent+=PURCHASE_MANAGE_STAFF_ID_LEN;
		auditTimeFrom.Deserialize(lpContent);
		lpContent+=sizeof(auditTimeFrom);
		auditTimeTo.Deserialize(lpContent);
		lpContent+=sizeof(auditTimeTo);
		memcpy(&accRefundFaultCount,lpContent,sizeof(accRefundFaultCount));
		lpContent+=sizeof(accRefundFaultCount);
		memcpy(&accRefundFaultAmount,lpContent,sizeof(accRefundFaultAmount));
		lpContent+=sizeof(accRefundFaultAmount);
		memcpy(&accRecyleTicketCount,lpContent,sizeof(accRecyleTicketCount));
		lpContent+=sizeof(accRecyleTicketCount);
		memcpy(&accRecyleTicketAmount,lpContent,sizeof(accRecyleTicketAmount));
		lpContent+=sizeof(accRecyleTicketAmount);
		memcpy(&incomeAmount,lpContent,sizeof(incomeAmount));
		lpContent+=sizeof(incomeAmount);
		memcpy(&receivableAmount,lpContent,sizeof(receivableAmount));
		lpContent+=sizeof(receivableAmount);
		memcpy(&refundAmount,lpContent,sizeof(refundAmount));
		lpContent+=sizeof(refundAmount);
		memcpy(&recyleTicketCount,lpContent,sizeof(recyleTicketCount));
		lpContent+=sizeof(recyleTicketCount);
		size_t businessCount;
		memcpy(&businessCount,lpContent,sizeof(size_t));
		lpContent+=sizeof(size_t);
		purchaseItems.clear();
		for (int i=0;i<businessCount;i++)
		{
			SCAuditKey key;
			key.Deserialize(lpContent);
			lpContent+=sizeof(SCAuditKey);
			SCAuditValue value;
			value.Deserialize(lpContent);
			lpContent+=sizeof(SCAuditValue);
			purchaseItems[key] = value;
		}
	}

	_tagSCAuditData& _tagSCAuditData::operator+=( _tagSCAuditData& another)
	{
		if(another.loginTime!=_DATE_TIME())
		{
			loginTime	= another.loginTime;
		}
		if(another.businessDate!=_DATE())
		{
			businessDate= another.businessDate;
		}
		if(another.staffID!="")
		{
			staffID= another.staffID;
		}
		if (auditTimeFrom == _DATE_TIME()  || (another.auditTimeFrom<auditTimeFrom && another.auditTimeFrom!=_DATE_TIME()))
		{
			auditTimeFrom	= another.auditTimeFrom;
		}
		if (another.auditTimeTo>auditTimeTo) 
		{
			auditTimeTo = another.auditTimeTo;
		}
		accRefundFaultCount       +=  another.accRefundFaultCount;
		accRefundFaultAmount       += another.accRefundFaultAmount;
		accRecyleTicketCount +=  another.accRecyleTicketCount;     
		accRecyleTicketAmount +=  another.accRecyleTicketCount;    
		incomeAmount += another.incomeAmount;						 
		receivableAmount +=  another.receivableAmount;						 
		refundAmount +=  another.refundAmount;						 
		recyleTicketCount +=  another.recyleTicketCount;	

		for (PurchaseItemMap::iterator it  = another.purchaseItems.begin();it!=another.purchaseItems.end();it++)
		{
			purchaseItems[it->first] += it->second;
		}
		return * this;
	}
} PurchaseData;

