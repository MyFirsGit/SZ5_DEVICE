#pragma once

#include "TxnAuditDef.h"
#include "TxnAuditException.h"
#include "Util.h"
#include "TxnAuditExp.h"
#include <map>
#include <vector>

using namespace std;

typedef struct _tagTxnAuditKey
{
	BYTE txnType;		//�豸�Ϸ����Ľ�������
	BYTE txnSubType;	//�豸�Ϸ����Ľ���������
	BYTE ticketType;	//Ʊ��������
	char	flag;		// Ԥ�����λ

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ���캯��

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	_tagTxnAuditKey()
	{
		txnType = 0;
		txnSubType = 0;
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
	_tagTxnAuditKey(const _tagTxnAuditKey& another)
	{
		this->txnType = another.txnType;
		this->txnSubType = another.txnSubType;
		this->ticketType = another.ticketType;
		this->flag = another.flag;
	}
	_tagTxnAuditKey& _tagTxnAuditKey::operator=( const _tagTxnAuditKey& another)
	{
		this->txnType = another.txnType;
		this->txnSubType = another.txnSubType;
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
	bool _tagTxnAuditKey::operator==(const _tagTxnAuditKey& another)  const 
	{
		if(txnType != another.txnType) return false;
		if(txnSubType!=another.txnSubType) return false;
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
		memcpy(lpByte,&txnType,sizeof(txnType));
		lpByte+=sizeof(txnType);
		memcpy(lpByte,&txnSubType,sizeof(txnSubType));
		lpByte+=sizeof(txnSubType);
		memcpy(lpByte,&ticketType,sizeof(ticketType));
		lpByte+=sizeof(ticketType);
		memcpy(lpByte,&flag,sizeof(flag));
		lpByte+=sizeof(flag);
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
		memcpy(&txnType,lpByte,sizeof(txnType));
		lpByte+=sizeof(txnType);
		memcpy(&txnSubType,lpByte,sizeof(txnSubType));
		lpByte+=sizeof(txnSubType);
		memcpy(&ticketType,lpByte,sizeof(ticketType));
		lpByte+=sizeof(ticketType);
		memcpy(&flag,lpByte,sizeof(flag));
		lpByte+=sizeof(flag);
	}
} TXNAuditKey;

//////////////////////////////////////////////////////////////////////////
/*
@brief      TXNAuditKey�Ƚ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
typedef struct _tagTXNAuditKeyCompare
{
	bool operator()(const TXNAuditKey& one,const TXNAuditKey& another) const 
	{
		if(one.txnType<another.txnType) return true;
		if(one.txnType>another.txnType) return false;
		if(one.txnSubType<another.txnSubType) return true;
		if(one.txnSubType>another.txnSubType) return false;
		if(one.ticketType<another.ticketType) return true;
		if(one.ticketType>another.ticketType) return false;
		if(one.flag<another.flag) return true;
		if(one.flag>another.flag) return false;
		return false;
	}
} TXNAuditKeyCompare;

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����������ֵ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
typedef struct _tagTXNAuditValue
{
	int transCount;				// ����������
	int transAmount;			//�����ܽ��
	int reserved;				//Ԥ��
	_tagTXNAuditValue()
	{
		transCount = 0;
		transAmount = 0;
		reserved = 0;
	}
	_tagTXNAuditValue& _tagTXNAuditValue::operator+=(const _tagTXNAuditValue& another)
	{
		transCount += another.transCount;
		transAmount += another.transAmount;
		reserved +=  another.reserved;
		return *this;
	}

	void Serialize(LPBYTE lpByte) const 
	{
		memcpy(lpByte,&transCount,sizeof(transCount));
		lpByte+=sizeof(int);
		memcpy(lpByte,&transAmount,sizeof(transAmount));
		lpByte+=sizeof(transAmount);
		memcpy(lpByte,&reserved,sizeof(reserved));
		lpByte+=sizeof(reserved);
	}
	void Deserialize(LPBYTE lpByte)  
	{
		memcpy(&transCount,lpByte,sizeof(transCount));
		lpByte+=sizeof(transCount);
		memcpy(&transAmount,lpByte,sizeof(transAmount));
		lpByte+=sizeof(transAmount);
		memcpy(&reserved,lpByte,sizeof(reserved));
		lpByte+=sizeof(reserved);
	}
} TXNAuditValue;

typedef map<TXNAuditKey,TXNAuditValue,TXNAuditKeyCompare> TXNAuditItemMap;

typedef struct _tagTXNAuditData
{
	_DATE businessDate;
	TXNAuditItemMap txnAuditItems;
	_tagTXNAuditData()
	{

	}
	void Serialize(VARIABLE_DATA& buffer)
	{
		buffer.nLen = sizeof(businessDate)+sizeof(txnAuditItems.size()) + txnAuditItems.size() * (sizeof(TXNAuditKey) + sizeof(TXNAuditValue));
		buffer.lpData = new BYTE[buffer.nLen];
		memset(buffer.lpData,0,buffer.nLen);
		LPBYTE lpContent = buffer.lpData;
		businessDate.Serialize(lpContent);
		lpContent+=sizeof(businessDate);
		int businessCount = txnAuditItems.size();
		memcpy(lpContent,&businessCount,sizeof(txnAuditItems.size()));
		lpContent+=sizeof(txnAuditItems.size());
		for(TXNAuditItemMap::iterator it = txnAuditItems.begin();it!=txnAuditItems.end();it++)
		{
 			it->first.Serialize(lpContent);
 			lpContent+=sizeof(TXNAuditKey);
 			it->second.Serialize(lpContent);
 			lpContent+=sizeof(TXNAuditValue);
		}
	}

	void Deserialize(VARIABLE_DATA& buffer)
	{
		LPBYTE lpContent = buffer.lpData;
		businessDate.Deserialize(lpContent);
		lpContent+=sizeof(businessDate);
		int businessCount=0;
		memcpy(&businessCount,lpContent,sizeof(businessCount));
		lpContent+=sizeof(businessCount);
		txnAuditItems.clear();
		for (int i=0;i<businessCount;i++)
		{
			TXNAuditKey key;
			key.Deserialize(lpContent);
			lpContent+=sizeof(TXNAuditKey);
			TXNAuditValue value;
			value.Deserialize(lpContent);
			lpContent+=sizeof(TXNAuditValue);
			txnAuditItems[key] = value;
		}
	}

	_tagTXNAuditData& _tagTXNAuditData::operator+=( _tagTXNAuditData& another)
	{
		if(another.businessDate!=_DATE())
		{
			businessDate= another.businessDate;
		}
		for (TXNAuditItemMap::iterator it  = another.txnAuditItems.begin();it!=another.txnAuditItems.end();it++)
		{
			txnAuditItems[it->first] += it->second;
		}
		return * this;
	}
} TxnAuditData;

