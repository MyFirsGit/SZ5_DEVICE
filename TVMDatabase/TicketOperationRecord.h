#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief		Æ±¿¨²Ù×÷Ã÷Ï¸Àà
	*/
	class TicketOperationRecord
	{
		public:
		int iTransID;			//½»Ò×±àºÅ
		_DATE_TIME dtOperationTime;	//²Ù×÷Ê±¼ä
		int iMediaType;				//Æ±¿¨½éÖÊ
		CString txtTicketBoxID;		//Æ±Ïä±àºÅ
		int iCardSerialNumber;		//Æ±¿¨±àºÅ
		int iEncodeState;			//±àÂë×´Ì¬
		int iReleaseState;			//·Å³ö×´Ì¬
		CString txtReserve;			//Ô¤Áô
	public:
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Æ´½ÓSQL²åÈëÓï¾ä

		@param

		@retval  CString SQLÓï¾ä   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToInsertSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("insert into tb_TicketOperationRecord(\
						  iTransID,\
						  dtOperationTime,\
						  iMediaType,\
						  txtTicketBoxID,\
						  iCardSerialNumber,\
						  iEncodeState,\
						  iReleaseState,\
						  txtReserve)values('%d','%s','%d', '%s','%d','%d', '%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iMediaType,		
						  txtTicketBoxID,	
						  iCardSerialNumber,
						  iEncodeState,		
						  iReleaseState,	
						  txtReserve);
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Æ´½ÓSQL¸üÐÂÓï¾ä

		@param

		@retval   CString SQLÓï¾ä   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToUpdateSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("update tb_TicketOperationRecord set \
						  dtOperationTime='%s',\
						  iMediaType,\
						  txtTicketBoxID='%s,\
						  iCardSerialNumber='%d',\
						  iEncodeState='%d',\
						  iReleaseState='%d',\
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iMediaType,		
						  txtTicketBoxID,	
						  iCardSerialNumber,
						  iEncodeState,		
						  iReleaseState,
						  txtReserve, 
						  iTransID);
			return strSql;
		}

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Æ´½ÓSQLÉ¾³ýÓï¾ä

		@param

		@retval   CString SQLÓï¾ä   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToDeleteSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("delete from tb_TicketOperationRecord where iTransID='%d'"),
						  iTransID);
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Æ´½ÓSQLÉ¾³ýÓï¾ä,²»´øÓÐwhereÓï¾ä

		@param

		@retval   CString SQLÓï¾ä   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToDeleteSqlWithoutWhere()
		{
			CString strSql = _T("");
			strSql.Format(_T("delete from tb_TicketOperationRecord "));
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Æ´½ÓSQL²éÑ¯Óï¾ä

		@param

		@retval  CString SQLÓï¾ä 

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToSelectSql()
		{
			CString strSql = _T("select iTransID, dtOperationTime,iMediaType,\
								txtTicketBoxID,\
								iCardSerialNumber,\
								iEncodeState,\
								iReleaseState,\
								txtReserve \
								from tb_TicketOperationRecord");
			return strSql;
		}

		
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    Ìî³äÊµÌå

		@param	  sqlite3_stmt* stmt ½á¹û¼¯

		@retval   long 0£ºÖ´ÐÐ³É¹¦£¬·Ç0£ºÖ´ÐÐÊ§°Ü

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////	
		long Fill(sqlite3_stmt* stmt)
		{
			iTransID = sqlite3_column_int(stmt,0);
			dtOperationTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));

			iMediaType = sqlite3_column_int(stmt,2);

			txtTicketBoxID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));

			iCardSerialNumber = sqlite3_column_int(stmt,4);
			iEncodeState = sqlite3_column_int(stmt,5);
			iReleaseState = sqlite3_column_int(stmt,6);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			return 0;
		}

		
	};
}