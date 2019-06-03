#pragma once

#include <map>
#include <vector>
#include "Util.h"
#include "SCAuditException.h"
#include "SCAuditDef.h"

typedef struct _US_SETTLEMENT_DATA
{
	long lSelledCardNum;			//销售票卡总数量
	long lSelledCardSum;			//销售票卡总金额

	long lCardRechargeNum;			//Card充值总数量
	long lCardRechargeSum;			//Card充值总金额

	long lFailBusinessNum;			//故障交易数量
	long lFailBusinessSum;			//故障交易金额

	long lSVTPayCardNum;			//储值卡购票总数量
	long lSVTPayCardSum;			//储值卡购票总金额

	long lNETPayCardNum;			//二维码购票总数量
	long lNETPayCardSum;			//储值卡购票总金额

	long lCoinSupplementSum;		//硬币补充金额
	long lCoinRecoverSum;			//硬币回收金额
	long lCoinChangeSum;			//硬币找零金额
	long lCoinLastLeftSum;			//硬币上次留存金额
	long lCoinCurrentLeftSum;		//硬币本次留存金额
	long lCoinIncomeSum;			//硬币收入金额

	long lNoteSupplementSum;		//纸币补充金额
	long lNoteRecoverSum;			//纸币回收金额
	long lNoteChangeSum;			//纸币找零金额
	long lNoteLastLeftSum;			//纸币上次留存
	long lNoteCurrentLeftSum;		//纸币本次留存
	long lNoteIncomeSum;			//纸币收入金额

	/*
	long lBankCardIncomeSum;		//银行卡收入金额
	long lBankCardBusinessNum;		//银行卡交易数量
	long lWalletIncomeSum;			//电子钱包收入金额
	long lWalletBusinessNum;		//电子钱包交易数量

	long lTokenLastLeftNum;			//Token上次留存
	long lTokenCurrentInvalidatedNum;//Token本次废票
	long lTokenCurrentSupplementNum;//Token本次补充
	long lTokenCurrentSelledNum;	//Token本次售出
	long lTokenCurrentRecoverNum;	//Token本次回收*/

	long lCardLastLeftNum;			//Card上次留存
	long lCardCurrentInvalidateNum;	//Card本次废票
	long lCardCurrrntSupplementNum;	//Card本次补充
	long lCardCurrentSelledNum;		//Card本次售出
	long lCardCurrentRecoverNum;	//Card本次回收
	long lCardCurrentLeftNum;		//Card本次留存

	// 初始化
	_US_SETTLEMENT_DATA(){
		lSelledCardNum			= 0;		//销售票卡总数量
		lSelledCardSum			= 0;		//销售票卡总金额

		lCardRechargeNum		= 0;		//Card充值总数量
		lCardRechargeSum		= 0;		//Card充值总金额

		lSVTPayCardNum		= 0;			//储值卡购票总数量
		lSVTPayCardSum		= 0;			//储值卡购票总金额

		lNETPayCardNum		= 0;			//二维码购票总数量
		lNETPayCardSum		= 0;			//二维码购票总金额

		lFailBusinessNum		= 0;		//故障交易数量
		lFailBusinessSum		= 0;		//故障交易金额

		lCoinSupplementSum		= 0;		//硬币补充金额
		lCoinRecoverSum			= 0;		//硬币回收金额
		lCoinChangeSum			= 0;		//硬币找零金额
		lCoinLastLeftSum		= 0;		//硬币上次留存金额
		lCoinCurrentLeftSum		= 0;		//硬币本次留存金额
		lCoinIncomeSum			= 0;		//硬币收入金额

		lNoteSupplementSum		= 0;		//纸币补充金额
		lNoteRecoverSum			= 0;		//纸币回收金额
		lNoteChangeSum			= 0;		//纸币找零金额
		lNoteLastLeftSum		= 0;		//纸币上次留存
		lNoteCurrentLeftSum		= 0;		//纸币本次留存
		lNoteIncomeSum			= 0;		//纸币收入金额

		lCardLastLeftNum		= 0;			//Card上次留存
		lCardCurrentInvalidateNum		= 0;	//Card本次废票
		lCardCurrrntSupplementNum		= 0;	//Card本次补充
		lCardCurrentSelledNum			= 0;	//Card本次售出
		lCardCurrentRecoverNum			= 0;	//Card本次回收
		lCardCurrentLeftNum				= 0;	//Card本次留存
	}

	void Serialize(LPBYTE lpByte) const 
	{
		// 统计
		memcpy(lpByte,&lSelledCardNum,2);
		lpByte+=2;
		memcpy(lpByte,&lSelledCardSum,sizeof(lSelledCardSum));
		lpByte+=sizeof(lSelledCardSum);

		memcpy(lpByte,&lCardRechargeNum,2);
		lpByte+=2;
		memcpy(lpByte,&lCardRechargeSum,sizeof(lCardRechargeSum));
		lpByte+=sizeof(lCardRechargeSum);

		memcpy(lpByte,&lFailBusinessNum,2);
		lpByte+=2;
		memcpy(lpByte,&lFailBusinessSum,sizeof(lFailBusinessSum));
		lpByte+=sizeof(lFailBusinessSum);

		// 硬币
		memcpy(lpByte,&lCoinLastLeftSum,sizeof(lCoinLastLeftSum));
		lpByte+=sizeof(lCoinLastLeftSum);

		memcpy(lpByte,&lCoinSupplementSum,sizeof(lCoinSupplementSum));
		lpByte+=sizeof(lCoinSupplementSum);

		memcpy(lpByte,&lCoinRecoverSum,sizeof(lCoinRecoverSum));
		lpByte+=sizeof(lCoinRecoverSum);
		
		memcpy(lpByte,&lCoinIncomeSum,sizeof(lCoinIncomeSum));
		lpByte+=sizeof(lCoinIncomeSum);

		memcpy(lpByte,&lCoinChangeSum,sizeof(lCoinChangeSum));
		lpByte+=sizeof(lCoinChangeSum);

		memcpy(lpByte,&lCoinCurrentLeftSum,sizeof(lCoinCurrentLeftSum));
		lpByte+=sizeof(lCoinCurrentLeftSum);
		
		// 纸币
		memcpy(lpByte,&lNoteLastLeftSum,4);
		lpByte += 4;

		memcpy(lpByte,&lNoteSupplementSum,4);
		lpByte += 4;

		memcpy(lpByte,&lNoteRecoverSum,4);
		lpByte += 4;

		memcpy(lpByte,&lNoteIncomeSum,4);
		lpByte += 4;

		memcpy(lpByte,&lNoteChangeSum,4);
		lpByte += 4;

		memcpy(lpByte,&lNoteCurrentLeftSum,4);
		lpByte += 4;

		// 票卡
		memcpy(lpByte,&lCardLastLeftNum,2);
		lpByte += 2;
		memcpy(lpByte,&lCardCurrentInvalidateNum,2);
		lpByte += 2;
		memcpy(lpByte,&lCardCurrrntSupplementNum,2);
		lpByte += 2;
		memcpy(lpByte,&lCardCurrentSelledNum,2);
		lpByte += 2;
		memcpy(lpByte,&lCardCurrentRecoverNum,2);
		lpByte += 2;
		memcpy(lpByte,&lCardCurrentLeftNum,2);
		lpByte += 2;
		/*memcpy(lpByte,&);
		memcpy(lpByte,&lBankCardIncomeSum,sizeof(lBankCardIncomeSum));
		lpByte+=sizeof(lBankCardIncomeSum);
		memcpy(lpByte,&lBankCardBusinessNum,sizeof(lBankCardBusinessNum));
		lpByte+=sizeof(lBankCardBusinessNum);
		memcpy(lpByte,&lWalletIncomeSum,sizeof(lWalletIncomeSum));
		lpByte+=sizeof(lWalletIncomeSum);
		memcpy(lpByte,&lWalletBusinessNum,sizeof(lWalletBusinessNum));
		lpByte+=sizeof(lWalletBusinessNum);

		memcpy(lpByte,&lTokenLastLeftNum,sizeof(lTokenLastLeftNum));
		lpByte+=sizeof(lTokenLastLeftNum);
		memcpy(lpByte,&lTokenCurrentInvalidatedNum,sizeof(lTokenCurrentInvalidatedNum));
		lpByte+=sizeof(lTokenCurrentInvalidatedNum);
		memcpy(lpByte,&lTokenCurrentSupplementNum,sizeof(lTokenCurrentSupplementNum));
		lpByte+=sizeof(lTokenCurrentSupplementNum);
		memcpy(lpByte,&lTokenCurrentSelledNum,sizeof(lTokenCurrentSelledNum));
		lpByte+=sizeof(lTokenCurrentSelledNum);
		memcpy(lpByte,&lTokenCurrentRecoverNum,sizeof(lTokenCurrentRecoverNum));
		lpByte+=sizeof(lTokenCurrentRecoverNum);*/
	}

	void Deserialize(LPBYTE lpByte)
	{
		memcpy(&lSelledCardNum,lpByte,sizeof(lSelledCardNum));
		lpByte+=sizeof(lSelledCardNum);
		memcpy(&lSelledCardSum,lpByte,sizeof(lSelledCardSum));
		lpByte+=sizeof(lSelledCardSum);

		memcpy(&lCardRechargeNum,lpByte,sizeof(lCardRechargeNum));
		lpByte+=sizeof(lCardRechargeNum);
		memcpy(&lCardRechargeSum,lpByte,sizeof(lCardRechargeSum));
		lpByte+=sizeof(lCardRechargeSum);

		memcpy(&lFailBusinessNum,lpByte,sizeof(lFailBusinessNum));
		lpByte+=sizeof(lFailBusinessNum);
		memcpy(&lFailBusinessSum,lpByte,sizeof(lFailBusinessSum));
		lpByte+=sizeof(lFailBusinessSum);

		memcpy(&lCoinSupplementSum,lpByte,sizeof(lCoinSupplementSum));
		lpByte+=sizeof(lCoinSupplementSum);
		memcpy(&lCoinRecoverSum,lpByte,sizeof(lCoinRecoverSum));
		lpByte+=sizeof(lCoinRecoverSum);
		memcpy(&lCoinChangeSum,lpByte,sizeof(lCoinChangeSum));
		lpByte+=sizeof(lCoinChangeSum);
		memcpy(&lCoinLastLeftSum,lpByte,sizeof(lCoinLastLeftSum));
		lpByte+=sizeof(lCoinLastLeftSum);
		memcpy(&lCoinCurrentLeftSum,lpByte,sizeof(lCoinCurrentLeftSum));
		lpByte+=sizeof(lCoinCurrentLeftSum);
		memcpy(&lCoinIncomeSum,lpByte,sizeof(lCoinIncomeSum));
		lpByte+=sizeof(lCoinIncomeSum);

		/*memcpy(&lBankCardIncomeSum,lpByte,sizeof(lBankCardIncomeSum));
		lpByte+=sizeof(lBankCardIncomeSum);
		memcpy(&lBankCardBusinessNum,lpByte,sizeof(lBankCardBusinessNum));
		lpByte+=sizeof(lBankCardBusinessNum);
		memcpy(&lWalletIncomeSum,lpByte,sizeof(lWalletIncomeSum));
		lpByte+=sizeof(lWalletIncomeSum);
		memcpy(&lWalletBusinessNum,lpByte,sizeof(lWalletBusinessNum));
		lpByte+=sizeof(lWalletBusinessNum);

		memcpy(&lTokenLastLeftNum,lpByte,sizeof(lTokenLastLeftNum));
		lpByte+=sizeof(lTokenLastLeftNum);
		memcpy(&lTokenCurrentInvalidatedNum,lpByte,sizeof(lTokenCurrentInvalidatedNum));
		lpByte+=sizeof(lTokenCurrentInvalidatedNum);
		memcpy(&lTokenCurrentSupplementNum,lpByte,sizeof(lTokenCurrentSupplementNum));
		lpByte+=sizeof(lTokenCurrentSupplementNum);
		memcpy(&lTokenCurrentSelledNum,lpByte,sizeof(lTokenCurrentSelledNum));
		lpByte+=sizeof(lTokenCurrentSelledNum);
		memcpy(&lTokenCurrentRecoverNum,lpByte,sizeof(lTokenCurrentRecoverNum));
		lpByte+=sizeof(lTokenCurrentRecoverNum);*/

		memcpy(&lCardLastLeftNum,lpByte,sizeof(lCardLastLeftNum));
		lpByte+=sizeof(lCardLastLeftNum);
		memcpy(&lCardCurrentInvalidateNum,lpByte,sizeof(lCardCurrentInvalidateNum));
		lpByte+=sizeof(lCardCurrentInvalidateNum);
		memcpy(&lCardCurrrntSupplementNum,lpByte,sizeof(lCardCurrrntSupplementNum));
		lpByte+=sizeof(lCardCurrrntSupplementNum);
		memcpy(&lCardCurrentSelledNum,lpByte,sizeof(lCardCurrentSelledNum));
		lpByte+=sizeof(lCardCurrentSelledNum);
		memcpy(&lCardCurrentRecoverNum,lpByte,sizeof(lCardCurrentRecoverNum));
		lpByte+=sizeof(lCardCurrentRecoverNum);
	}

	_US_SETTLEMENT_DATA& _US_SETTLEMENT_DATA::operator+=(const _US_SETTLEMENT_DATA& another)
	{
		lSelledCardNum+=another.lSelledCardNum;
		lSelledCardSum+=another.lSelledCardSum;
		lCardRechargeNum+=another.lCardRechargeNum;
		lCardRechargeSum+=another.lCardRechargeSum;
		lFailBusinessNum+=another.lFailBusinessNum;
		lFailBusinessSum+=another.lFailBusinessSum;
		lCoinSupplementSum+=another.lCoinSupplementSum;
		lCoinRecoverSum+=another.lCoinRecoverSum;
		lCoinChangeSum+=another.lCoinChangeSum;
		lCoinLastLeftSum+=another.lCoinLastLeftSum;
		lCoinCurrentLeftSum+=another.lCoinCurrentLeftSum;
		lCoinIncomeSum+=another.lCoinIncomeSum;
		lNoteSupplementSum+=another.lNoteSupplementSum;
		lNoteRecoverSum+=another.lNoteRecoverSum;
		lNoteChangeSum+=another.lNoteChangeSum;
		lNoteLastLeftSum+=another.lNoteLastLeftSum;
		lNoteCurrentLeftSum+=another.lNoteCurrentLeftSum;
		lNoteIncomeSum+=another.lNoteIncomeSum;
		/*lBankCardIncomeSum+=another.lBankCardIncomeSum;
		lBankCardBusinessNum+=another.lBankCardBusinessNum;
		lWalletIncomeSum+=another.lWalletIncomeSum;
		lWalletBusinessNum+=another.lWalletBusinessNum;
		lTokenLastLeftNum+=another.lTokenLastLeftNum;
		lTokenCurrentInvalidatedNum+=another.lTokenCurrentInvalidatedNum;
		lTokenCurrentSupplementNum+=another.lTokenCurrentSupplementNum;
		lTokenCurrentSelledNum+=another.lTokenCurrentSelledNum;
		lTokenCurrentRecoverNum+=another.lTokenCurrentRecoverNum;*/
		lCardLastLeftNum+=another.lCardLastLeftNum;
		lCardCurrentInvalidateNum+=another.lCardCurrentInvalidateNum;
		lCardCurrrntSupplementNum+=another.lCardCurrrntSupplementNum;
		lCardCurrentSelledNum+=another.lCardCurrentSelledNum;
		lCardCurrentRecoverNum+=another.lCardCurrentRecoverNum;
		lCardCurrentLeftNum += another.lCardCurrentLeftNum;
		return *this;
	}

}US_SETTLEMENT_DATA;

typedef struct _tagSCAuditData
{
	_DATE businessDate;					 // 执行集计的系统运营日
	_DATE_TIME auditTimeFrom;			 // 作业开始时间
	_DATE_TIME auditTimeTo;				 // 作业结束时间

	US_SETTLEMENT_DATA settlementData;	//结算数据

	_tagSCAuditData()
	{
		memset(&settlementData,0,sizeof(US_SETTLEMENT_DATA));
	}
	void Serialize(VARIABLE_DATA& buffer)
	{
		buffer.nLen = sizeof(businessDate) + sizeof(auditTimeFrom) + sizeof(auditTimeTo) + sizeof(settlementData);
		buffer.lpData = new BYTE[buffer.nLen];
		if(NULL!=buffer.lpData )
		{
			memset(buffer.lpData,0,buffer.nLen);
			LPBYTE lpContent = buffer.lpData;
			businessDate.Serialize(lpContent);
			lpContent+=sizeof(businessDate);
			auditTimeFrom.Serialize(lpContent);
			lpContent+=sizeof(auditTimeFrom);
			auditTimeTo.Serialize(lpContent);
			lpContent+=sizeof(auditTimeTo);

			settlementData.Serialize(lpContent);
			lpContent+=sizeof(settlementData);
		}else
		{
			//Out of mem
		}
	}

	void Deserialize(VARIABLE_DATA& buffer)
	{
		LPBYTE lpContent = buffer.lpData;
		businessDate.Deserialize(lpContent);
		lpContent+=sizeof(businessDate);
		auditTimeFrom.Deserialize(lpContent);
		lpContent+=sizeof(auditTimeFrom);
		auditTimeTo.Deserialize(lpContent);
		lpContent+=sizeof(auditTimeTo);

		settlementData.Deserialize(lpContent);
		lpContent+=sizeof(settlementData);
	}

	_tagSCAuditData& _tagSCAuditData::operator+=( _tagSCAuditData& another)
	{
		if(another.businessDate!=_DATE())
		{
			businessDate= another.businessDate;
		}
		if (auditTimeFrom == _DATE_TIME()  || (another.auditTimeFrom<auditTimeFrom && another.auditTimeFrom!=_DATE_TIME()))
		{
			auditTimeFrom	= another.auditTimeFrom;
		}
		if (another.auditTimeTo>auditTimeTo) 
		{
			auditTimeTo = another.auditTimeTo;
		}

		settlementData+=another.settlementData;

		return * this;
	}
} PurchaseData;

