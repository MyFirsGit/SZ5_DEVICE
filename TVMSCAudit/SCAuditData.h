#pragma once

#include <map>
#include <vector>
#include "Util.h"
#include "SCAuditException.h"
#include "SCAuditDef.h"

typedef struct _US_SETTLEMENT_DATA
{
	long lSelledCardNum;			//����Ʊ��������
	long lSelledCardSum;			//����Ʊ���ܽ��

	long lCardRechargeNum;			//Card��ֵ������
	long lCardRechargeSum;			//Card��ֵ�ܽ��

	long lFailBusinessNum;			//���Ͻ�������
	long lFailBusinessSum;			//���Ͻ��׽��

	long lSVTPayCardNum;			//��ֵ����Ʊ������
	long lSVTPayCardSum;			//��ֵ����Ʊ�ܽ��

	long lNETPayCardNum;			//��ά�빺Ʊ������
	long lNETPayCardSum;			//��ֵ����Ʊ�ܽ��

	long lCoinSupplementSum;		//Ӳ�Ҳ�����
	long lCoinRecoverSum;			//Ӳ�һ��ս��
	long lCoinChangeSum;			//Ӳ��������
	long lCoinLastLeftSum;			//Ӳ���ϴ�������
	long lCoinCurrentLeftSum;		//Ӳ�ұ���������
	long lCoinIncomeSum;			//Ӳ��������

	long lNoteSupplementSum;		//ֽ�Ҳ�����
	long lNoteRecoverSum;			//ֽ�һ��ս��
	long lNoteChangeSum;			//ֽ��������
	long lNoteLastLeftSum;			//ֽ���ϴ�����
	long lNoteCurrentLeftSum;		//ֽ�ұ�������
	long lNoteIncomeSum;			//ֽ��������

	/*
	long lBankCardIncomeSum;		//���п�������
	long lBankCardBusinessNum;		//���п���������
	long lWalletIncomeSum;			//����Ǯ��������
	long lWalletBusinessNum;		//����Ǯ����������

	long lTokenLastLeftNum;			//Token�ϴ�����
	long lTokenCurrentInvalidatedNum;//Token���η�Ʊ
	long lTokenCurrentSupplementNum;//Token���β���
	long lTokenCurrentSelledNum;	//Token�����۳�
	long lTokenCurrentRecoverNum;	//Token���λ���*/

	long lCardLastLeftNum;			//Card�ϴ�����
	long lCardCurrentInvalidateNum;	//Card���η�Ʊ
	long lCardCurrrntSupplementNum;	//Card���β���
	long lCardCurrentSelledNum;		//Card�����۳�
	long lCardCurrentRecoverNum;	//Card���λ���
	long lCardCurrentLeftNum;		//Card��������

	// ��ʼ��
	_US_SETTLEMENT_DATA(){
		lSelledCardNum			= 0;		//����Ʊ��������
		lSelledCardSum			= 0;		//����Ʊ���ܽ��

		lCardRechargeNum		= 0;		//Card��ֵ������
		lCardRechargeSum		= 0;		//Card��ֵ�ܽ��

		lSVTPayCardNum		= 0;			//��ֵ����Ʊ������
		lSVTPayCardSum		= 0;			//��ֵ����Ʊ�ܽ��

		lNETPayCardNum		= 0;			//��ά�빺Ʊ������
		lNETPayCardSum		= 0;			//��ά�빺Ʊ�ܽ��

		lFailBusinessNum		= 0;		//���Ͻ�������
		lFailBusinessSum		= 0;		//���Ͻ��׽��

		lCoinSupplementSum		= 0;		//Ӳ�Ҳ�����
		lCoinRecoverSum			= 0;		//Ӳ�һ��ս��
		lCoinChangeSum			= 0;		//Ӳ��������
		lCoinLastLeftSum		= 0;		//Ӳ���ϴ�������
		lCoinCurrentLeftSum		= 0;		//Ӳ�ұ���������
		lCoinIncomeSum			= 0;		//Ӳ��������

		lNoteSupplementSum		= 0;		//ֽ�Ҳ�����
		lNoteRecoverSum			= 0;		//ֽ�һ��ս��
		lNoteChangeSum			= 0;		//ֽ��������
		lNoteLastLeftSum		= 0;		//ֽ���ϴ�����
		lNoteCurrentLeftSum		= 0;		//ֽ�ұ�������
		lNoteIncomeSum			= 0;		//ֽ��������

		lCardLastLeftNum		= 0;			//Card�ϴ�����
		lCardCurrentInvalidateNum		= 0;	//Card���η�Ʊ
		lCardCurrrntSupplementNum		= 0;	//Card���β���
		lCardCurrentSelledNum			= 0;	//Card�����۳�
		lCardCurrentRecoverNum			= 0;	//Card���λ���
		lCardCurrentLeftNum				= 0;	//Card��������
	}

	void Serialize(LPBYTE lpByte) const 
	{
		// ͳ��
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

		// Ӳ��
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
		
		// ֽ��
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

		// Ʊ��
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
	_DATE businessDate;					 // ִ�м��Ƶ�ϵͳ��Ӫ��
	_DATE_TIME auditTimeFrom;			 // ��ҵ��ʼʱ��
	_DATE_TIME auditTimeTo;				 // ��ҵ����ʱ��

	US_SETTLEMENT_DATA settlementData;	//��������

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

