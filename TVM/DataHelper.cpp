#include "stdafx.h"
#include "DataHelper.h"
#include <direct.h>
#include "DatabaseException.h"
#include "DataCommand.h"
#include "CloseCommand.h"
#include "UpdateCommand.h"
#include "ConnectCommand.h"
#include "NonQueryCommand.h"
#include "QueryCommand.h"
//#include "../Include/Sqlite3/sqlite3.h"
//#include "ESDef.h"
//#include "TaskHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataHelper::CDataHelper()
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
CDataHelper::~CDataHelper()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݿ�

@param      ��
@param		��

@retval     long

@exception  CDataBaseException
*/
//////////////////////////////////////////////////////////////////////////
long CDataHelper::DbInitialize()
{
	int sqlite_result = SQLITE_OK;
	//��Ŀ¼
	if(!ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath()+DATABASE_DIR))
	{
		throw CDataBaseException(CDataBaseException::DATABASE_MAIN_FOLDER_CREATE_ERRR, _T(__FILE__), __LINE__);
	}
	//��������Ŀ¼
	if(!ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath()+DATABASE_DIR))
	{
		throw CDataBaseException(CDataBaseException::DATABASE_BACKUP_FOLDER_CREATE_ERRR, _T(__FILE__), __LINE__);
	}
	
	CString strMainDatabasePath = theTVM_INFO.GetDataPath()+ DATABASE_DIR + DATABASE_FILE_NAME_OF_TVM;
	CString strBackupDatabasePath = theTVM_INFO.GetBackupPath()+ DATABASE_DIR + DATABASE_FILE_NAME_OF_TVM;
	BOOL mainDatabaseExist = PathFileExists(strMainDatabasePath);
	BOOL backupDatabaseExist = PathFileExists(strBackupDatabasePath);

	if(!mainDatabaseExist && backupDatabaseExist)
	{
		if(!CopyFile(strBackupDatabasePath,strMainDatabasePath,FALSE))
		{
			throw CDataBaseException(CDataBaseException::DATABASE_BACKUP_COPY_FALL, _T(__FILE__), __LINE__);
		}
	}
	if(mainDatabaseExist && !backupDatabaseExist)
	{
		if(!CopyFile(strMainDatabasePath,strBackupDatabasePath,FALSE))
		{
			throw CDataBaseException(CDataBaseException::DATABASE_BACKUP_COPY_FALL, _T(__FILE__), __LINE__);
		}
	}
	
	::SetFileAttributes(strMainDatabasePath,FILE_ATTRIBUTE_NORMAL);
	::SetFileAttributes(strBackupDatabasePath,FILE_ATTRIBUTE_NORMAL);

	Data::CConnectCommand connectCommand(strMainDatabasePath,strBackupDatabasePath);
	int connectResult = connectCommand.Execute();
	if(connectResult!=SQLITE_OK)
	{
		throw CDataBaseException(CDataBaseException::DATABASE_OPEN_ERR, _T(__FILE__), __LINE__,connectCommand.GetError());
	}

	if(!mainDatabaseExist && !backupDatabaseExist)
	{
		TCHAR szAppPath[_MAX_PATH];

		if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			throw CDataBaseException(CDataBaseException::DATABASE_GET_APP_PATH_ERROR, _T(__FILE__), __LINE__);;
		}

		CString strSqlScriptFileName = CString(szAppPath) + DATABASE_SQL_FILE_NAME;
		if(!ComCheckFileExist(strSqlScriptFileName))
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SCRIPT_ERRR, _T(__FILE__), __LINE__);
		}
		CXFile sqlScriptFile(strSqlScriptFileName);		
		CString strContent = _T("");
		CString strLine = _T("");
		// ��ȡ���ݿ�ű��ļ�
		while (sqlScriptFile.ReadString(strLine))
		{
			strContent = strContent+_T(" ")+strLine;
		}
		//���ݿ�ű�Ϊ�գ��׳��쳣
		if(strContent.IsEmpty())
		{
			throw CDataBaseException(CDataBaseException::DATABASE_READ_SCRIPT_FALL, _T(__FILE__), __LINE__);
		}
		g_pDataControl->BeginTransaction();
		Data::CNonQueryCommand createStructureCommand(strContent);
		int createStructureResult = createStructureCommand.Execute();
		if(createStructureResult!=SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,createStructureCommand.GetError());
		}
		g_pDataControl->Commit();
	}
	return sqlite_result;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �ر����ݿ�

@param      ��
@param      ��

@retval     long

@exception  CDataBaseException
*/
//////////////////////////////////////////////////////////////////////////
//long CDataHelper::DbClose()
//{
//	// �����ݿ�
//	int sqlite_result = SQLITE_OK;
//	CCloseCommand closeCommand;
//	sqlite_result = closeCommand.Execute();
//	if (sqlite_result != SQLITE_OK)
//	{
//		throw CDataBaseException(CDataBaseException::DATABASE_CLOSE_ERR, _T(__FILE__), __LINE__,closeCommand.GetError());
//	}
//	return sqlite_result;
//}


long CDataHelper::DbSelectTopNumTransInfo(vector<TransactionRecord>& vec_transactionInfo,long lNum)
{
	CString strLimit=_T("");
		strLimit.Format(_T(" where iState!=2 order by dtTransTime desc limit %d"),lNum);//ȥ��ȡ������
		CQueryCommand<TransactionDetail>  queryCommand(strLimit,CQueryCommand<TransactionDetail>::UE_LIMIT);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vector<TransactionDetail> vec_transactionDetail = queryCommand.GetResults();
		for(int i = 0; i < vec_transactionDetail.size(); i++)
		{
			TransactionRecord trans;
			trans.iTransactionDetail = vec_transactionDetail[i];
			long nRet = DbSelectCoinInsertRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinDetail);
			nRet = DbSelectBanknoteInsertRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteDetail);
			nRet = DbSelectTicketOperationRecord(vec_transactionDetail[i].iTransID, trans.vCardDetail);
			nRet = DbSelectCoinChangeRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinChangeDetail);


			nRet = DbSelectCoinAcceptRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinAcceptDetail);

			// ��ѯָ�����׵�ֽ�����������ϸ(Ԥ��)
			nRet = DbSelectBanknoteChangeRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteChangeDetail);
			// ��ѯָ�����׵�ֽ�����ɲ�����ϸ
			nRet = DbSelectBanknoteAcceptRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteAcceptDetail);
			// ��ѯָ�����׵�ֽ�ҷϱҲ�����ϸ(Ԥ��)
			nRet = DbSelectBanknoteDiscardRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteDiscardDetail);
			// ��ѯָ�����׵Ĺ�����ϸ
			nRet = DbSelectExceptionRecord(vec_transactionDetail[i].iTransID, trans.vExceptionDetail);


			vec_transactionInfo.push_back(trans);
		}
		return sqlite_result;
}

/*********************************************************

lyj add start 2014-12-23
**************************************************************/


//////////////////////////////////////////////////////////////////////////
/**
	@brief      ��ѯָ��ʱ��ε�Ӳ�����ɼ�¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinAcceptRecord>& vec_CoinAcceptRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
//////////////////////////////////////////////////////////////////////////
	long CDataHelper::DbSelectCoinAcceptRecord(CString beginTime,CString endTime, vector<CoinAcceptRecord>& vec_CoinAcceptRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinAcceptRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinAcceptRecord = queryCommand.GetResults();
		return sqlite_result;
	}
	
/**
	@brief      ��ѯָ��ʱ��ε�Ӳ�������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinChangeRecord>& vec_CoinChangeRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinChangeRecord(CString beginTime,CString endTime, vector<CoinChangeRecord>& vec_CoinChangeRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinChangeRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinChangeRecord = queryCommand.GetResults();

		return sqlite_result;
	}

/**
	@brief      ��ѯָ��ʱ��ε�ֽ�����ɼ�¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectBanknoteAcceptRecord(CString beginTime,CString endTime, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<BanknoteAcceptRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteAcceptRecord = queryCommand.GetResults();

		return sqlite_result;
	}

/**
	@brief      ��ѯָ��ʱ��ε�ֽ�������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectBanknoteChangeRecord(CString beginTime,CString endTime, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<BanknoteChangeRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteChangeRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��εĹ��ϼ�¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<ExceptionRecord>& vec_TransFailureRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectTransFailureRecord(CString beginTime,CString endTime, vector<ExceptionRecord>& vec_TransFailureRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<ExceptionRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TransFailureRecord = queryCommand.GetResults();

		return sqlite_result;
	}

/**
	@brief      ɾ��N��ǰ�Ľ�����Ϣ

	@param		(i)WORD dwDaysʱ����

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbDeleteTransactionInfoBeforeNdays(WORD dwDays)
	{
		//TODO
		CTime tm; tm=CTime::GetCurrentTime();
		tm -= dwDays*24*3600;
		_DATE newDate;
		if (tm < 0 ) {
			newDate.wYear = 1970;
			newDate.biMonth = 1;
			newDate.biDay = 1; 
		}
		else {
			newDate.wYear = tm.GetYear();
			newDate.biMonth = tm.GetMonth();
			newDate.biDay = tm.GetDay();
		}
		CString beginDate = newDate.ToString(_T("%.4d%.2d%.2d"));
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND  substr(dtTransTime,1,14)<= '%s'"), beginDate);

		int sqlite_result = SQLITE_OK;
		//ɾ��detail����е�����
		{
			TransactionDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		strWhere.Format(_T(" where 1=1  AND  substr(dtOperationTime,1,14)<= '%s'"), beginDate);
		//ɾ��Ӳ����Ϣ���
		{
			CoinInsertRecord CoinTmp;
			CNonQueryCommand commandTaskCoin(CoinTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCoin.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCoin.GetError());
			}
		}
		//ɾ��ֽ�ұ��
		{
			BanknoteInsertRecord BanknoteTmp;
			CNonQueryCommand commandTaskBanknote(BanknoteTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskBanknote.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskBanknote.GetError());
			}
		}
		//ɾ��Ʊ��������Ϣ���
		{
			TicketOperationRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			CoinChangeRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			CoinAcceptRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteChangeRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteAcceptRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteDiscardRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			ExceptionRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		g_pDataControl->Commit();
		return sqlite_result;
	}

// ----------------------------------վԱҵ�����ݿ�����ӿڶ���-----------------------------------
/**
	@brief      ����Ӳ�������䲹�������¼

	@param		(i)CoinAddOperationRecord &coinAddOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertCoinAddOperationRecord(CoinAddOperationRecord &coinAddOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(coinAddOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Ӳ�һ��ղ�����¼

	@param		(i)CoinCollectOperationRecord &coinCollectOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertCoinCollectOperationRecord(CoinCollectOperationRecord &coinCollectOperationRecord)
	{
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(coinCollectOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Ӳ�һ�����ж�ز�����¼

	@param		(i)CoinCollectBoxRemoveOperationRecord &coinCollectBoxRemoveOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/ 
	long CDataHelper::DbInsertCoinCollectBoxRemoveOperationRecord(CoinCollectBoxRemoveOperationRecord &coinCollectBoxRemoveOperationRecord)
	{
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(coinCollectBoxRemoveOperationRecord.coinCollectBoxRemoveOperationDetail.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}

		
		CString strLimit=_T("");
		strLimit.Format(_T(" order by dtTransTime desc limit 1"));
		CQueryCommand<CoinCollectBoxRemoveOperationDetail>  queryCommand(strLimit,CQueryCommand<CoinCollectBoxRemoveOperationDetail>::UE_LIMIT);
		sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}

		vector<CoinCollectBoxRemoveOperationDetail> vec_transactionDetail = queryCommand.GetResults();
		if(vec_transactionDetail.size() != 1)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}


		for(int i = 0; i < coinCollectBoxRemoveOperationRecord.v_coinBoxCashDetail.size(); i++)
		{
			coinCollectBoxRemoveOperationRecord.v_coinBoxCashDetail[i].iID = vec_transactionDetail[0].iID;
			CNonQueryCommand commandTask1(coinCollectBoxRemoveOperationRecord.v_coinBoxCashDetail[i].ToInsertSql());
			sqlite_result = commandTask1.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask1.GetError());
			}
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Ӳ�������䲹�������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinAddOperationRecord>& vec_CoinAddOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinAddOperationRecord(CString beginTime,CString endTime, vector<CoinAddOperationRecord>& vec_CoinAddOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinAddOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinAddOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Ӳ�һ��ղ�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinCollectOperationRecord>& vec_CoinCollectOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinCollectOperationRecord(CString beginTime,CString endTime, vector<CoinCollectOperationRecord>& vec_CoinCollectOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinCollectOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinCollectOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Ӳ�һ�����ж�ز�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinCollectBoxRemoveOperationRecord>& vec_CoinCollectBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CoinCollectBoxRemoveOperationRecord>& vec_CoinCollectBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinCollectBoxRemoveOperationDetail>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vector<CoinCollectBoxRemoveOperationDetail> v_detail = queryCommand.GetResults();

		for(int i = 0; i < v_detail.size(); i++)
		{
			CoinCollectBoxRemoveOperationRecord record;
			record.coinCollectBoxRemoveOperationDetail = v_detail[i];

			strWhere.Format(_T("1=1  AND iID ='%d'"),record.coinCollectBoxRemoveOperationDetail.iID);
			CQueryCommand<CoinBoxCashDetail>  queryCommand1(strWhere);
			sqlite_result = queryCommand1.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand1.GetError());
			}
			record.v_coinBoxCashDetail = queryCommand1.GetResults();
			vec_CoinCollectBoxRemoveOperationRecord.push_back(record);
		}
		

		return sqlite_result;
	}
/**
	@brief      ɾ��N������ǰ������Ӳ�Ҳ�����¼

	@param		(i)WORD dwDays����

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/ 
	long CDataHelper::DbDeleteCoinOperationRecordBeforeNdays(WORD dwDays)
	{
		//TODO
		CTime tm; tm=CTime::GetCurrentTime();
		tm -= dwDays*24*3600;
		_DATE newDate;
		if (tm < 0 ) {
			newDate.wYear = 1970;
			newDate.biMonth = 1;
			newDate.biDay = 1; 
		}
		else {
			newDate.wYear = tm.GetYear();
			newDate.biMonth = tm.GetMonth();
			newDate.biDay = tm.GetDay();
		}
		CString beginDate = newDate.ToString(_T("%.4d%.2d%.2d"));
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND  substr(dtOperationTime,1,14)<= '%s'"), beginDate);

		int sqlite_result = SQLITE_OK;
		//ɾ��Ӳ�������䲹�������¼
		{
			CoinAddOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		// ɾ��Ӳ�������䰲װж�ز�����¼
		{
			CoinChangeBoxOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		//ɾ��Ӳ�һ��ղ�����¼
		{
			CoinCollectOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		//ɾ��Ӳ�һ�����ж�ز�����¼
		//��ɾ��ǰ����ѡ���������Ҫɾ����ID����ɾ��Ӳ�һ������ֽ���ϸʹ��
		vector<CoinCollectBoxRemoveOperationRecord> vec_CoinCollectBoxRemoveOperationRecord;
		vec_CoinCollectBoxRemoveOperationRecord.clear();
		{
			CString startDate =  _T("19700101");
			sqlite_result =  DbSelectCoinCollectBoxRemoveOperationRecord(startDate,beginDate, vec_CoinCollectBoxRemoveOperationRecord);
		}
		
		strWhere.Format(_T(" where 1=1  AND  substr(dtTransTime,1,14)<= '%s'"), beginDate);
		{
			CoinCollectBoxRemoveOperationDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		for(int i = 0; i < vec_CoinCollectBoxRemoveOperationRecord.size(); i++)
		{
			strWhere.Format(_T(" where 1=1  AND  iID='%d'"), vec_CoinCollectBoxRemoveOperationRecord[i].coinCollectBoxRemoveOperationDetail.iID);
			CoinBoxCashDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		//ɾ��Ӳ�һ������ֽ���ϸ
		g_pDataControl->Commit();
		return sqlite_result;

	}

/**
	@brief      ��ѯָ��ʱ���Ӳ�Ҳ����������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CoinCollectOperationRecord>& vec_CoinCollectOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinChangeBoxOperationRecord(CString beginTime,CString endTime, vector<CoinChangeBoxOperationRecord>& vec_CoinChangeBoxOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinChangeBoxOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinChangeBoxOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ѯָ��ʱ���Ӳ��Ͷ����Ϣ

@param      (i)CString beginTime��ʼʱ��
			(i)CString endTime����ʱ��
			(o)vector<CoinInsertRecord>& vec_CoinInsertRecord��ѯ���

@retval     long 0��ִ�гɹ�����0��ִ��ʧ��

@exception  CDataBaseException
*/
//////////////////////////////////////////////////////////////////////////
	long CDataHelper::DbSelectCoinInsertRecord(CString beginTime,CString endTime,vector<CoinInsertRecord>& vec_CoinInsertRecord){

		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CoinInsertRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinInsertRecord = queryCommand.GetResults();

		return sqlite_result;
	}

/**
	@brief      ����Ӳ�������������¼

	@param		(i)CoinChangeBoxOperationRecord& coinChangeBoxOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertCoinChangeBoxOperationRecord(CoinChangeBoxOperationRecord& coinChangeBoxOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(coinChangeBoxOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}

/**
	@brief      ����ֽ�������䲹�������¼

	@param		(i)BanknoteOperationRecord &banknoteAddOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertBanknoteOperationRecord(BanknoteOperationRecord &banknoteAddOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(banknoteAddOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����ֽ�һ����䰲װ������¼

	@param		(i)BanknoteBoxInstallOperationRecord &banknoteBoxInstallOperationRecord��Ҫ����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertBanknoteBoxInstallOperationRecord(BanknoteBoxInstallOperationRecord &banknoteBoxInstallOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(banknoteBoxInstallOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����ֽ��ж�ػ��ղ�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertBanknoteBoxRemoveOperationRecord(BanknoteBoxRemoveOperationRecord &banknoteBoxRemoveOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}

		CString strLimit=_T("");
		strLimit.Format(_T(" order by dtTransTime desc limit 1"));
		CQueryCommand<BanknoteBoxRemoveOperationDetail>  queryCommand(strLimit,CQueryCommand<BanknoteBoxRemoveOperationDetail>::UE_LIMIT);
		sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}

		vector<BanknoteBoxRemoveOperationDetail> vec_transactionDetail = queryCommand.GetResults();
		if(vec_transactionDetail.size() != 1)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}

		for(int i = 0; i < banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.size(); i++)
		{
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail[i].iID = vec_transactionDetail[0].iID;
			CNonQueryCommand commandTask1(banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail[i].ToInsertSql());
			sqlite_result = commandTask1.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask1.GetError());
			}
		}

		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�ֽ�������䲹�������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteOperationRecord>& vec_BanknoteAddOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectBanknoteOperationRecord(CString beginTime,CString endTime, vector<BanknoteOperationRecord>& vec_BanknoteAddOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<BanknoteOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteAddOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�ֽ�һ����䰲װ������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteBoxInstallOperationRecord>& vec_BanknoteBoxInstallOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectBanknoteBoxInstallOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxInstallOperationRecord>& vec_BanknoteBoxInstallOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<BanknoteBoxInstallOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteBoxInstallOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�ֽ��ж�ػ��ղ�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<BanknoteBoxRemoveOperationRecord>& vec_BanknoteBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectBanknoteBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxRemoveOperationRecord>& vec_BanknoteBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<BanknoteBoxRemoveOperationDetail>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vector<BanknoteBoxRemoveOperationDetail> v_detail = queryCommand.GetResults();
		
		for(int i = 0; i < v_detail.size(); i++)
		{
			BanknoteBoxRemoveOperationRecord record;
			record.banknoteBoxRemoveOperationDetail  = v_detail[i];
			strWhere.Format(_T("1=1  AND iID='%d'"),v_detail[i].iID);
			CQueryCommand<BanknoteBoxCashDetail>  queryCommand1(strWhere);
			sqlite_result = queryCommand1.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand1.GetError());
			}
		 record.v_banknoteBoxCashDetail = queryCommand1.GetResults();
		 vec_BanknoteBoxRemoveOperationRecord.push_back(record);

		}
		return sqlite_result;
	}
/**
	@brief      ɾ��N������ǰ������ֽ�Ҳ�����¼

	@param		(i)WORD dwDays���������

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbDeleteBanknoteOperationRecordBeforeNdays(WORD dwDays)
	{
		//TODO
		CTime tm; tm=CTime::GetCurrentTime();
		tm -= dwDays*24*3600;
		_DATE newDate;
		if (tm < 0 ) {
			newDate.wYear = 1970;
			newDate.biMonth = 1;
			newDate.biDay = 1; 
		}
		else {
			newDate.wYear = tm.GetYear();
			newDate.biMonth = tm.GetMonth();
			newDate.biDay = tm.GetDay();
		}
		CString beginDate = newDate.ToString(_T("%.4d%.2d%.2d"));
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND  substr(dtTransTime,1,14)<= '%s'"), beginDate);

		int sqlite_result = SQLITE_OK;
		//
		{
			BanknoteOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		{
			BanknoteBoxInstallOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		//��ɾ��ǰ����ѡ���������Ҫɾ����ID����ɾ��Ӳ�һ������ֽ���ϸʹ��
		vector<BanknoteBoxRemoveOperationRecord> vec_BanknoteBoxRemoveOperationRecord;
		vec_BanknoteBoxRemoveOperationRecord.clear();
		{
			CString startDate =  _T("19700101");
			sqlite_result =  DbSelectBanknoteBoxRemoveOperationRecord(startDate,beginDate, vec_BanknoteBoxRemoveOperationRecord);
		}

		{
			BanknoteBoxRemoveOperationDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for(int i = 0; i < vec_BanknoteBoxRemoveOperationRecord.size(); i++)
		{
			strWhere.Format(_T(" where 1=1  AND  iID='%d'"), vec_BanknoteBoxRemoveOperationRecord[i].banknoteBoxRemoveOperationDetail.iID);
			BanknoteBoxCashDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		
		g_pDataControl->Commit();
		return sqlite_result;
	}

/**
	@brief      ����Token�䰲װ������¼(Ԥ��)

	@param		(i)TokenBoxInstallOperationRecord &tokenBoxInstallOperationRecord����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbInsertTokenBoxInstallOperationRecord(TokenBoxInstallOperationRecord &tokenBoxInstallOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(tokenBoxInstallOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Token��ж�ز�����¼(Ԥ��)

	@param		(i)TokenBoxRemoveOperationRecord &tokenBoxRemoveOperationRecord����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertTokenBoxRemoveOperationRecord(TokenBoxRemoveOperationRecord &tokenBoxRemoveOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(tokenBoxRemoveOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Token�䲹�������¼

	@param		(i)TokenBoxAddOperationRecord &tokenBoxAddOperationRecord����ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertTokenBoxAddOperationRecord(TokenBoxAddOperationRecord &tokenBoxAddOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(tokenBoxAddOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Token���ղ�����¼

	@param		(i)TokenCollectOperationRecord &tokenBoxAddOperationRecord������ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertTokenCollectOperationRecord(TokenCollectOperationRecord &tokenBoxAddOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(tokenBoxAddOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;

	}
/**
	@brief      ����Token��Ʊ��ж�ز�����¼

	@param		(i)TokenCollectBoxRemoveOperationRecord &tokenCollectBoxRemoveOperationRecord������ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertTokenCollectBoxRemoveOperationRecord(TokenCollectBoxRemoveOperationRecord &tokenCollectBoxRemoveOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(tokenCollectBoxRemoveOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Token�䰲װ������¼(Ԥ��)

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<TokenBoxInstallOperationRecord>& vec_TokenBoxInstallOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectTokenBoxInstallOperationRecord(CString beginTime,CString endTime, vector<TokenBoxInstallOperationRecord>& vec_TokenBoxInstallOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TokenBoxInstallOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TokenBoxInstallOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Token��ж�ز�����¼(Ԥ��)

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<TokenBoxRemoveOperationRecord>& vec_TokenBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectTokenBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenBoxRemoveOperationRecord>& vec_TokenBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TokenBoxRemoveOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TokenBoxRemoveOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Token�䲹�������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<TokenBoxAddOperationRecord>& vec_TokenBoxAddOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectTokenBoxAddOperationRecord(CString beginTime,CString endTime, vector<TokenBoxAddOperationRecord>& vec_TokenBoxAddOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TokenBoxAddOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TokenBoxAddOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Token���ղ�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<TokenCollectOperationRecord>& vec_TokenCollectOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectTokenCollectOperationRecord(CString beginTime,CString endTime, vector<TokenCollectOperationRecord>& vec_TokenCollectOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TokenCollectOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TokenCollectOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Token��Ʊ��ж�ز�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<TokenCollectBoxRemoveOperationRecord>& vec_TokenCollectBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectTokenCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenCollectBoxRemoveOperationRecord>& vec_TokenCollectBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TokenCollectBoxRemoveOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_TokenCollectBoxRemoveOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ɾ��N������ǰ������Token������¼

	@param		(i)WORD dwDaysʱ����

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbDeleteTokenOperationRecordBeforeNdays(WORD dwDays)
	{
		//TODO
		CTime tm; tm=CTime::GetCurrentTime();
		tm -= dwDays*24*3600;
		_DATE newDate;
		if (tm < 0 ) {
			newDate.wYear = 1970;
			newDate.biMonth = 1;
			newDate.biDay = 1; 
		}
		else {
			newDate.wYear = tm.GetYear();
			newDate.biMonth = tm.GetMonth();
			newDate.biDay = tm.GetDay();
		}
		CString beginDate = newDate.ToString(_T("%.4d%.2d%.2d"));
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND  substr(dtOperationTime,1,14)<= '%s'"), beginDate);

		int sqlite_result = SQLITE_OK;
		//
		{
			TokenBoxInstallOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		{
			TokenBoxRemoveOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		{
			TokenBoxAddOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		{
			TokenCollectOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		strWhere.Format(_T(" where 1=1  AND  substr(dtTransTime,1,14)<= '%s'"), beginDate);
		{
			TokenCollectBoxRemoveOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		g_pDataControl->Commit();
		return sqlite_result;
	}


/**
	@brief      ����Card�䰲װ������¼

	@param		(i)CardBoxInstallOperationRecord &cardBoxInstallOperationRecord������ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertCardBoxInstallOperationRecord(CardBoxInstallOperationRecord &cardBoxInstallOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(cardBoxInstallOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Card��ж�ز�����¼

	@param		(i)CardBoxRemoveOperationRecord &cardBoxRemoveOperationRecord������ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertCardBoxRemoveOperationRecord(CardBoxRemoveOperationRecord &cardBoxRemoveOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(cardBoxRemoveOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ����Card��Ʊ��ж�ز�����¼

	@param		(iCardCollectBoxRemoveOperationRecord &cardCollectBoxRemoveOperationRecord������ļ�¼

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbInsertCardCollectBoxRemoveOperationRecord(CardCollectBoxRemoveOperationRecord &cardCollectBoxRemoveOperationRecord)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(cardCollectBoxRemoveOperationRecord.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Card�䰲װ������¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CardBoxInstallOperationRecord>& vec_CardBoxInstallOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectCardBoxInstallOperationRecord(CString beginTime,CString endTime, vector<CardBoxInstallOperationRecord>& vec_CardBoxInstallOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CardBoxInstallOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CardBoxInstallOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Card��ж�ز�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CardBoxRemoveOperationRecord>& vec_CardBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectCardBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardBoxRemoveOperationRecord>& vec_CardBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CardBoxRemoveOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CardBoxRemoveOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
/**
	@brief      ��ѯָ��ʱ��ε�Card��Ʊ��ж�ز�����¼

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(i)vector<CardCollectBoxRemoveOperationRecord>& vec_CardCollectBoxRemoveOperationRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectCardCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardCollectBoxRemoveOperationRecord>& vec_CardCollectBoxRemoveOperationRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<CardCollectBoxRemoveOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CardCollectBoxRemoveOperationRecord = queryCommand.GetResults();

		return sqlite_result;
	}
	
/**
	@brief      ɾ��N������ǰ������Card������¼

	@param		(i)WORD dwDaysʱ����

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbDeleteCardOperationRecordBeforeNdays(WORD dwDays)
	{
		//TODO
		CTime tm; tm=CTime::GetCurrentTime();
		tm -= dwDays*24*3600;
		_DATE newDate;
		if (tm < 0 ) {
			newDate.wYear = 1970;
			newDate.biMonth = 1;
			newDate.biDay = 1; 
		}
		else {
			newDate.wYear = tm.GetYear();
			newDate.biMonth = tm.GetMonth();
			newDate.biDay = tm.GetDay();
		}
		CString beginDate = newDate.ToString(_T("%.4d%.2d%.2d"));
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND  substr(dtOperationTime,1,14)<= '%s'"), beginDate);

		int sqlite_result = SQLITE_OK;
		//
		{
			CardBoxInstallOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		{
			CardBoxRemoveOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		strWhere.Format(_T(" where 1=1  AND  substr(dtTransTime,1,14)<= '%s'"), beginDate);
		{
			CardCollectBoxRemoveOperationRecord templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		g_pDataControl->Commit();
		return sqlite_result;
	}


	// ----------------------------------�˿�ҵ�����ݿ�����ӿڶ���-----------------------------------


/**
	@brief      ��ѯָ�����׵�ֽ�ҷϱҲ�����ϸ��Ԥ����

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<BanknoteDiscardRecord>& vec_BanknoteDiscardRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectBanknoteDiscardRecord(int iTransID, vector<BanknoteDiscardRecord>& vec_BanknoteDiscardRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<BanknoteDiscardRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteDiscardRecord = queryCommand.GetResults();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ�����׵�Ӳ�����������ϸ

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<CoinChangeRecord>& vec_CoinChangeRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	// 
	long CDataHelper::DbSelectCoinChangeRecord(int iTransID, vector<CoinChangeRecord>& vec_CoinChangeRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<CoinChangeRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinChangeRecord = queryCommand.GetResults();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ�����׵�Ӳ�����ɲ�����ϸ

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<CoinAcceptRecord>& vec_CoinAcceptRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectCoinAcceptRecord(int iTransID, vector<CoinAcceptRecord>& vec_CoinAcceptRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<CoinAcceptRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinAcceptRecord = queryCommand.GetResults();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ�����׵�ֽ�����������ϸ��Ԥ����

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectBanknoteChangeRecord(int iTransID, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<BanknoteChangeRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteChangeRecord = queryCommand.GetResults();
		return sqlite_result;
	}
/**
	@brief      ��ѯָ�����׵�ֽ�����ɲ�����ϸ

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectBanknoteAcceptRecord(int iTransID, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<BanknoteAcceptRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteAcceptRecord = queryCommand.GetResults();
		return sqlite_result;
	}

/**
	@brief      ��ѯָ�����׵Ĺ�����ϸ

	@param		(i)CString iTransIDָ������ID
	@param		(i)vector<ExceptionRecord>& vec_ExceptionRecord��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/
	long CDataHelper::DbSelectExceptionRecord(int iTransID, vector<ExceptionRecord>& vec_ExceptionRecord)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<ExceptionRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_ExceptionRecord = queryCommand.GetResults();
		return sqlite_result;
	}
	

    // ----------------------------------վԱҵ�����ݿ�����ӿڶ���-----------------------------------
/**
	@brief      ��ѯӲ�һ������ֽ���ϸ

	@param		(i)CString iRecordIDָ��Ӳ�һ������ֽ��¼ID
	@param		(i)vector<CoinBoxCashDetail>& vec_CoinBoxCashDetail��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectCoinBoxCashDetail(int iRecordID, vector<CoinBoxCashDetail>& vec_CoinBoxCashDetail)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iID='%d'"),iRecordID);
		CQueryCommand<CoinBoxCashDetail>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_CoinBoxCashDetail = queryCommand.GetResults();
		return sqlite_result;
	}
/**
	@brief      ��ѯֽ�һ������ֽ���ϸ

	@param		(i)CString iRecordIDָ��ֽ�һ������ֽ��¼ID
	@param		(i)vector<BanknoteBoxCashDetail>& vec_BanknoteBoxCashDetail��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
*/	// 
	long CDataHelper::DbSelectBanknoteBoxCashDetail(int iRecordID, vector<BanknoteBoxCashDetail>& vec_BanknoteBoxCashDetail)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iID='%d'"),iRecordID);
		CQueryCommand<BanknoteBoxCashDetail>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_BanknoteBoxCashDetail = queryCommand.GetResults();
		return sqlite_result;
	}
/*********************************************************

lyj add end 2014-12-23
**************************************************************/
	/*
	@brief		��ѯָ��ʱ��εĽ�����Ϣ

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��
	@param		(o)vector<TransactionRecord>& vec_transactionInfo��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/
	long CDataHelper::DbSelectTransactionInfo(CString beginTime,CString endTime, vector<TransactionRecord>& vec_transactionInfo,bool bSelectDetail /* = true */)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);
		CQueryCommand<TransactionDetail>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vector<TransactionDetail> vec_transactionDetail = queryCommand.GetResults();
		for(int i = 0; i < vec_transactionDetail.size(); i++)
		{
			TransactionRecord trans;
			trans.iTransactionDetail = vec_transactionDetail[i];

			if(bSelectDetail){
				long nRet = DbSelectCoinInsertRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinDetail);
				nRet = DbSelectBanknoteInsertRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteDetail);
				nRet = DbSelectTicketOperationRecord(vec_transactionDetail[i].iTransID, trans.vCardDetail);
				nRet = DbSelectCoinChangeRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinChangeDetail);


				nRet = DbSelectCoinAcceptRecord(vec_transactionDetail[i].iTransID, trans.vTransCoinAcceptDetail);

				// ��ѯָ�����׵�ֽ�����������ϸ��Ԥ����
				nRet = DbSelectBanknoteChangeRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteChangeDetail);
				// ��ѯָ�����׵�ֽ�����ɲ�����ϸ
				nRet = DbSelectBanknoteAcceptRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteAcceptDetail);
				// ��ѯָ�����׵�ֽ�ҷϱҲ�����ϸ��Ԥ����
				nRet = DbSelectBanknoteDiscardRecord(vec_transactionDetail[i].iTransID, trans.vTransBanknoteDiscardDetail);
				// ��ѯָ�����׵Ĺ�����ϸ
				nRet = DbSelectExceptionRecord(vec_transactionDetail[i].iTransID, trans.vExceptionDetail);
			}

			vec_transactionInfo.push_back(trans);
		}
		return sqlite_result;
	}
	/*
	@brief		��ѯָ����Ӳ�Ҳ�����ϸ

	@param		(i)CString iTransID ���ױ��
	@param		(o)vector<CoinInsertRecord>& vec_transCoinDetail��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/

	long CDataHelper::DbSelectCoinInsertRecord(int iTransID, vector<CoinInsertRecord>& vec_transCoinDetail)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<CoinInsertRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		vec_transCoinDetail = queryCommand.GetResults();
		return sqlite_result;
	}
	/*
	@brief		��ѯָ����ֽ�Ҳ�����ϸ

	@param		(i)CString iTransID ���ױ��
	@param		(o)vector<TransBanknoteDetail>& vec_transBanknoteDetail��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/
	long CDataHelper::DbSelectBanknoteInsertRecord(int iTransID, vector<BanknoteInsertRecord>& vec_transBanknoteDetail)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		CQueryCommand<BanknoteInsertRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		size_t irecordNum=queryCommand.GetResults().size();
		for(size_t iloop=0;iloop<irecordNum;iloop++)
		{
			vec_transBanknoteDetail.push_back(queryCommand.GetResults()[iloop]);
		}
		//vec_transBanknoteDetail = queryCommand.GetResults();
		return sqlite_result;
	}
	/*
	@brief		��ѯָ����Ʊ��������ϸ

	@param		(i)CString iTransID ���ױ��
	@param		(o)vector<CardDetail>& vec_cardDetail��ѯ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/
	long CDataHelper::DbSelectTicketOperationRecord(int iTransID, vector<TicketOperationRecord>& vec_cardDetail)
	{
		CString strWhere = _T("");	
		strWhere.Format(_T("1=1  AND iTransID='%d'"),iTransID);
		//CQueryCommand<TransBanknoteDetail>  queryCommand(strWhere);
		CQueryCommand<TicketOperationRecord>  queryCommand(strWhere);
		int sqlite_result = queryCommand.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,queryCommand.GetError());
		}
		size_t iretNum=queryCommand.GetResults().size();
		for(size_t iloop=0;iloop<iretNum;iloop++)
		{
			vec_cardDetail .push_back(queryCommand.GetResults()[iloop]);
		}
		//vec_cardDetail = queryCommand.GetResults();
		return sqlite_result;
	}

	/*
	@brief		���ӽ��׼�¼

	@param		(i)TransactionRecord &transactionInfo ���ӵĽ���

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/
	long CDataHelper::DbInsertTransactionInfo(TransactionRecord &transactionInfo)
	{
		// ���������Ϣ���
		g_pDataControl->BeginTransaction();
		int sqlite_result = SQLITE_OK;
		CNonQueryCommand commandTask(transactionInfo.iTransactionDetail.ToInsertSql());
		sqlite_result = commandTask.Execute();
		if(sqlite_result != SQLITE_OK)
		{
			g_pDataControl->Rollback();
			throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
		}
		vector<TransactionRecord> v_rec;
		sqlite_result = DbSelectTopNumTransInfo(v_rec, 1);
		if(v_rec.size() != 1)
		{
			g_pDataControl->Rollback();
			return -1;
		}
		for (int i = 0; i < transactionInfo.vTransCoinDetail.size(); i++)
		{
			transactionInfo.vTransCoinDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask1(transactionInfo.vTransCoinDetail[i].ToInsertSql());
			sqlite_result = commandTask1.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		for (int i = 0; i < transactionInfo.vTransBanknoteDetail.size(); i++)
		{
			transactionInfo.vTransBanknoteDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask2(transactionInfo.vTransBanknoteDetail[i].ToInsertSql());
			sqlite_result = commandTask2.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		for (int i = 0; i < transactionInfo.vCardDetail.size(); i++)
		{
			transactionInfo.vCardDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vCardDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vTransCoinChangeDetail.size(); i++)
		{
			transactionInfo.vTransCoinChangeDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vTransCoinChangeDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vTransCoinAcceptDetail.size(); i++)
		{
			transactionInfo.vTransCoinAcceptDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vTransCoinAcceptDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vTransBanknoteChangeDetail.size(); i++)
		{
			transactionInfo.vTransBanknoteChangeDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vTransBanknoteChangeDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vTransBanknoteAcceptDetail.size(); i++)
		{
			transactionInfo.vTransBanknoteAcceptDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vTransBanknoteAcceptDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vTransBanknoteDiscardDetail.size(); i++)
		{
			transactionInfo.vTransBanknoteDiscardDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vTransBanknoteDiscardDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}

		for (int i = 0; i < transactionInfo.vExceptionDetail.size(); i++)
		{
			transactionInfo.vExceptionDetail[i].iTransID = v_rec[0].iTransactionDetail.iTransID;
			CNonQueryCommand commandTask3(transactionInfo.vExceptionDetail[i].ToInsertSql());
			sqlite_result = commandTask3.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		g_pDataControl->Commit();
		return sqlite_result;
	}

	/*
	@brief		ɾ��ָ��ʱ��εĽ�����Ϣ

	@param		(i)CString beginTime��ʼʱ��
	@param		(i)CString endTime����ʱ��

	@retval		long 0��ִ�гɹ�����0��ִ��ʧ��

	@exception  CDataBaseException
	*/
	long CDataHelper::DbDeleteTransactionInfo(CString beginTime,CString endTime)
	{
		g_pDataControl->BeginTransaction();
		CString strWhere = _T("");	
		strWhere.Format(_T(" where 1=1  AND substr(dtTransTime,1,14)>='%s' and  substr(dtTransTime,1,14)<= '%s'"),beginTime,endTime);

		int sqlite_result = SQLITE_OK;
		//ɾ��detail����е�����
		{
			TransactionDetail templateTmp;
			CNonQueryCommand commandTask(templateTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTask.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTask.GetError());
			}
		}
		//ɾ��Ӳ����Ϣ���
		strWhere.Format(_T(" where 1=1  AND substr(dtOperationTime,1,14)>='%s' and  substr(dtOperationTime,1,14)<= '%s'"),beginTime,endTime);
		//ɾ��Ӳ����Ϣ���
		{
			CoinInsertRecord CoinTmp;
			CNonQueryCommand commandTaskCoin(CoinTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCoin.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCoin.GetError());
			}
		}
		//ɾ��ֽ�ұ��
		{
			BanknoteInsertRecord BanknoteTmp;
			CNonQueryCommand commandTaskBanknote(BanknoteTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskBanknote.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskBanknote.GetError());
			}
		}
		//ɾ��Ʊ��������Ϣ���
		{
			TicketOperationRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			CoinChangeRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			CoinAcceptRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteChangeRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteAcceptRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			BanknoteDiscardRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}

		{
			ExceptionRecord CardTmp;
			CNonQueryCommand commandTaskCard(CardTmp.ToDeleteSqlWithoutWhere() + strWhere);
			sqlite_result = commandTaskCard.Execute();
			if(sqlite_result != SQLITE_OK)
			{
				g_pDataControl->Rollback();
				throw CDataBaseException(CDataBaseException::DATABASE_SQL_EXECUTE_FALL, _T(__FILE__), __LINE__,commandTaskCard.GetError());
			}
		}
		g_pDataControl->Commit();
		return sqlite_result;
		
	}
	