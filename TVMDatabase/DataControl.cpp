#include "stdafx.h"
#include "DataControl.h"
#include "nonquerycommand.h"

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief     构造函数 

	@param		无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	Data::CDataControl::CDataControl():CControl()
	{

	}
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief     析构函数

	@param		无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	Data::CDataControl::~CDataControl()
	{

	}
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief    初期化  

	@param		无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	long Data::CDataControl::Initialize()
	{
		return __super::Initialize();		
	}
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief    结束时  

	@param		无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	long Data::CDataControl::Shutdown()
	{
		return __super::Shutdown();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief    开启数据库事务  

	@param		无

	@retval     long，0：成功，非0：失败	

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	long Data::CDataControl::BeginTransaction()
	{
		CString strBegin = _T("begin transaction");
		Data::CNonQueryCommand beginTransactionCommand(strBegin);
		beginTransactionCommand.Execute();
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief    提交数据库事务  

	@param		无

	@retval     long，0：成功，非0：失败	

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	long Data::CDataControl::Commit()
	{
		CString strCommit = _T("commit transaction");
		Data::CNonQueryCommand commitTransactionCommand(strCommit);
		commitTransactionCommand.Execute();
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief  回滚数据库事物    

	@param		无

	@retval     long，0：成功，非0：失败	

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	long Data::CDataControl::Rollback()
	{
		CString strRollback = _T("rollback transaction");
		Data::CNonQueryCommand rollbackTransactionCommand(strRollback);
		rollbackTransactionCommand.Execute();
		return 0;
	}
	//test code
	//CDataHelper datacontrol(*this);
	//datacontrol.DbInitialize();
	//TransactionRecord transInfo;
	//transInfo.iTransactionDetail.BankNoteAcceptAmount=10010;
	//transInfo.iTransactionDetail.BanknoteAcceptState=10086;
	//transInfo.iTransactionDetail.CardFeeAmount=900;
	//transInfo.iTransactionDetail.CoinInsertAmount=900;
	//transInfo.iTransactionDetail.CoinState=900;
	//transInfo.iTransactionDetail.DepositAmount=900;
	//transInfo.iTransactionDetail.DepositAmount=900;
	//transInfo.iTransactionDetail.dtTransTime.wYear=2014;
	//transInfo.iTransactionDetail.dtTransTime.biMonth=7;
	//transInfo.iTransactionDetail.dtTransTime.biDay=25;
	//transInfo.iTransactionDetail.dtTransTime.biHour=6;
	//transInfo.iTransactionDetail.dtTransTime.biMinute=15;
	//transInfo.iTransactionDetail.dtTransTime.biSecond=23;
	//transInfo.iTransactionDetail.iBanknoteChangeState=900;
	//transInfo.iTransactionDetail.iBanknoteChangeAmount=900;
	//transInfo.iTransactionDetail.iCardType=900;
	//transInfo.iTransactionDetail.iChangeAmount=900;
	//transInfo.iTransactionDetail.iCoinChangeAmount=900;
	//transInfo.iTransactionDetail.iCoinChangeState=900;
	//transInfo.iTransactionDetail.iDestination=900;
	//transInfo.iTransactionDetail.iOrigin=900;
	//transInfo.iTransactionDetail.iPayCardNo=_T("10086597423");
	//transInfo.iTransactionDetail.iPaymentMethod=900;
	//transInfo.iTransactionDetail.iPaymentState=900;
	//transInfo.iTransactionDetail.iPrintState=900;
	//transInfo.iTransactionDetail.iProductSubType=900;
	//transInfo.iTransactionDetail.iProductType=900;
	//transInfo.iTransactionDetail.iQuantity=900;
	//transInfo.iTransactionDetail.iState=900;
	//transInfo.iTransactionDetail.iTotalAmount=900;
	//transInfo.iTransactionDetail.iTransID=_T("1001231546");
	//transInfo.iTransactionDetail.iUDSN=_T("48897551225");
	//transInfo.iTransactionDetail.iUdSubtype=1;
	//transInfo.iTransactionDetail.iUdType=1;
	//transInfo.iTransactionDetail.iUnitAmount=10;
	//transInfo.iTransactionDetail.iUnReleaseedMediaQuantity=200;
	//transInfo.iTransactionDetail.TaxAmount=100;

	///*datacontrol.DbInsertTransactionInfo(transInfo);

	//
	//transInfo.iTransactionDetail.iTransID=_T("1");
	//datacontrol.DbInsertTransactionInfo(transInfo);

	//transInfo.iTransactionDetail.iTransID=_T("2");
	//datacontrol.DbInsertTransactionInfo(transInfo);

	//transInfo.iTransactionDetail.iTransID=_T("3");
	//datacontrol.DbInsertTransactionInfo(transInfo);*/

	///*transInfo.iTransactionDetail.iTransID=_T("4");
	//transInfo.iTransactionDetail.dtTransTime.wYear=2014;
	//transInfo.iTransactionDetail.dtTransTime.biMonth=7;
	//transInfo.iTransactionDetail.dtTransTime.biDay=26;
	//transInfo.iTransactionDetail.dtTransTime.biHour=6;
	//transInfo.iTransactionDetail.dtTransTime.biMinute=15;
	//transInfo.iTransactionDetail.dtTransTime.biSecond=23;
	//datacontrol.DbInsertTransactionInfo(transInfo);

	//transInfo.iTransactionDetail.iTransID=_T("5");
	//transInfo.iTransactionDetail.dtTransTime.wYear=2014;
	//transInfo.iTransactionDetail.dtTransTime.biMonth=7;
	//transInfo.iTransactionDetail.dtTransTime.biDay=27;
	//transInfo.iTransactionDetail.dtTransTime.biHour=6;
	//transInfo.iTransactionDetail.dtTransTime.biMinute=15;
	//transInfo.iTransactionDetail.dtTransTime.biSecond=23;
	//datacontrol.DbInsertTransactionInfo(transInfo);*/

	//CString szbegintime=_T("20140726 6:15:23");
	//CString szendtime=_T("20140729 6:15:29");
	////vector<TransactionRecord> vec_transactionInfo;
	////datacontrol.DbSelectTransactionInfo(szbegintime,szendtime,vec_transactionInfo);
	//datacontrol.DbDeleteTransactionInfo(szbegintime,szendtime);