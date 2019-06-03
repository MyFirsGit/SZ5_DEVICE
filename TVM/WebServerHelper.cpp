#include "stdafx.h"
#include "json.h"
#include "WebServerHelper.h"
#include "QRCodeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define theWEBAPI_TRACE CFileLog::GetInstance(FILTER_WEBAPI_LOG)
CWebServerHelper CWebServerHelper::theInstance;


// �̺߳���
DWORD WINAPI WorkerFunction(IN LPVOID vThreadParm){
	try{
		CWebServerHelper* pThreadParm = reinterpret_cast<CWebServerHelper*>(vThreadParm);

		if(NULL != pThreadParm){

			while(WAIT_OBJECT_0 == WaitForSingleObject(pThreadParm->m_hPauseThread,INFINITE)){

				// �ж��Ƿ��˳��߳�
				if(pThreadParm->m_bExitThread){
					pThreadParm->m_EmThreadReturn = RSP_CODE_OK;
					break;
				}

				try{
					BOOL bResult = pThreadParm->m_Parm.pHttpFile->SendRequest(pThreadParm->m_Parm.strHeader,(LPVOID)(LPCTSTR)pThreadParm->m_Parm.pSendData, strlen(pThreadParm->m_Parm.pSendData));

					if(!bResult){
						pThreadParm->m_EmThreadReturn = RSP_CODE_CONNECT_FAIL;
					}
					else{
						pThreadParm->m_EmThreadReturn = RSP_CODE_OK;
					}
				}
				catch(CInternetException&e){
					DWORD txt = e.m_dwContext;
					DWORD err = e.m_dwError;
					pThreadParm->m_EmThreadReturn = RSP_CODE_CONNECT_FAIL;
				}			 
				catch(...){
					pThreadParm->m_EmThreadReturn = RSP_CODE_CONNECT_FAIL;
				}

				ResetEvent(pThreadParm->m_hPauseThread);
				SetEvent(pThreadParm->m_hWaitResult);
			}

			SetEvent(pThreadParm->m_hExitThread);
		}
		else{
			pThreadParm->m_EmThreadReturn = RSP_CODE_CONNECT_FAIL;
			SetEvent(pThreadParm->m_hWaitResult);
		}
		//BOOL bResult = pThreadParm->pHttpFile->SendRequest(pThreadParm->strHeader,(LPVOID)(LPCTSTR)pThreadParm->pSendData, strlen(pThreadParm->pSendData));

		//if(!bResult){
		//	return RSP_CODE_CONNECT_FAIL;
		//}
	}
	catch(CException* e){
		return RSP_CODE_CONNECT_FAIL;
	}
	catch(...){
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}

DWORD WINAPI IdleWorkerFunction(IN LPVOID vThreadParm){
	try{
		CWebServerHelper* pThreadParm = reinterpret_cast<CWebServerHelper*>(vThreadParm);

		if(NULL != pThreadParm){
			while(1){
				// �ж��Ƿ��˳��߳�
				if(pThreadParm->m_bExitThread){
					break;
				}

				if(theWEBSERVER_HELPER.Web_ConnectCheck()==RSP_CODE_OK){
					if(theWEBSERVER_HELPER.IsEnableIdleProcess()){
						theWEBSERVER_HELPER.Web_IdleProcessOrder();
					}
				}
				Sleep(10000);
			}// end of while
		}
	}
	catch(CException* e){
		return RSP_CODE_CONNECT_FAIL;
	}
	catch(...){
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWebServerHelper::CWebServerHelper(void)
{
	host=theFunction_INFO.GetWebApiHost();//_T("zzdevtest.e-metro.cn");
	port=theFunction_INFO.GetWebApiPort();//9092;
	//loginurl=theFunction_INFO.GetWebApiLoginUrl();//��¼
	//queryurl=theFunction_INFO.GetWebApiQueryUrl();//��ѯ
	//lockurl=theFunction_INFO.GetWebApiLockUrl();//����
	//confirmurl=theFunction_INFO.GetWebApiConfirmUrl();//ȷ��
	//getorderurl=theFunction_INFO.GetWebApiGetOrderUrl();//ȡƱ���ȡ������
	//tvmstateurl=theFunction_INFO.GetWebApiTVMStateUrl();//��ѯTVM״̬
	//updatestateurl=theFunction_INFO.GetWebApiUpdateStateUrl();//�޸�TVM״̬
	//strHeaders = _T("Content-Type: application/json");

	strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	//baseURL.Format(_T("http://18754t6t24.imwork.net:29874"));	
	baseURL.Format(_T("http://10.99.0.50:8080"));//�����ӿ�
	baseURL.Format(_T("http://59.175.176.236:8239"));//�����ӿ�
	baseURL.Format(_T(":%d"), port);
	baseURL = host + baseURL;

	m_nOrderRetryTimes = 3;
	connectURL = baseURL + _T("/IAFCFTFPaySystem");
	weixin_requestOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_wxTradePrecreate"));
	weixin_queryOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_wxTradeQuery"));
	weixin_cancelOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_wxTradeCancel"));
	zhifubao_requestOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_aliTradePrecreate"));
	zhifubao_queryOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_aliTradeQuery"));
	zhifubao_cancelOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_aliTradeCancel"));
	confirmOrderURL.Format(_T("/IAFCFTFPaySystem/tvm/QRpayAction_tickResultSubmit"));

	m_bCurConnect = false;
	m_bLoacalProcessEnable = false;
	m_nOrderProcessType = theTVM_SETTING.GetWebIdleProcessType();
	m_strOrderProcessURL = theTVM_SETTING.GetWebIdleProcessURL();

	// �߳�����¼�
	m_hPauseThread = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWaitResult = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hExitThread = CreateEvent(NULL,TRUE,FALSE,NULL);

	m_bExitThread = FALSE;
	m_Parm.strHeader = strHeaders;


	// ��ʼ�����������߳�
	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(
		NULL,				// Pointer to thread security attributes
		0,					// Initial thread stack size, in bytes 
		WorkerFunction,		// Pointer to thread function
		this,				// The argument for the new thread
		0,					// Creation flags 
		&dwThreadID			// Pointer to returned thread identifier
		);

	if(hThread != NULL && hThread != INVALID_HANDLE_VALUE){
		CloseHandle(hThread);
		hThread = NULL;
	}

	// ��ʼ���������ؿ��ж��������߳�
	DWORD dwIdleThreadID = 0;
	HANDLE hIdleThread = CreateThread(
		NULL,				// Pointer to thread security attributes
		0,					// Initial thread stack size, in bytes 
		IdleWorkerFunction,	// Pointer to thread function
		this,				// The argument for the new thread
		0,					// Creation flags 
		&dwIdleThreadID			// Pointer to returned thread identifier
		);

	if(hIdleThread != NULL && hIdleThread != INVALID_HANDLE_VALUE){
		CloseHandle(hIdleThread);
		hIdleThread = NULL;
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWebServerHelper::~CWebServerHelper(void)
{
	// �ȴ����߳��˳�
	m_bExitThread = TRUE;
	SetEvent(m_hPauseThread);
	WaitForSingleObject(m_hExitThread,INFINITE);

	if(NULL != m_hPauseThread && INVALID_HANDLE_VALUE != m_hPauseThread){
		CloseHandle(m_hPauseThread);
		m_hPauseThread = NULL;
	}

	if(NULL != m_hWaitResult && INVALID_HANDLE_VALUE != m_hWaitResult){
		CloseHandle(m_hWaitResult);
		m_hWaitResult = NULL;
	}

	if(NULL != m_hExitThread && INVALID_HANDLE_VALUE != m_hExitThread){
		CloseHandle(m_hExitThread);
		m_hExitThread = NULL;
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief     UTF8תANSI
@param    (io)CString &strUTF8 �ַ���
@retval     void
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWebServerHelper::UTF8toANSI(CString &strUTF8)
{
	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)strUTF8.GetBuffer(),-1,NULL,NULL);
	WCHAR *wszBuffer = new WCHAR[nLen+1];
	nLen = MultiByteToWideChar(CP_UTF8,NULL,(LPCSTR)strUTF8.GetBuffer(),-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
	CHAR *szBuffer = new CHAR[nLen+1];
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
	szBuffer[nLen] = 0;

	strUTF8 = szBuffer;
	//�����ڴ�
	delete []szBuffer;
	delete []wszBuffer;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ͽ���web
@param    (i)CString senddata post����
@param    (i)CString password url�����ַ
@param    (o)CString& getdata ��������
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::ServerPost(CString senddata, CString url,CString& getdata,int nTimeOut)
{
	CInternetSession sess;//�����Ự
	/*CHAR *cStrFromData =new CHAR[senddata.GetLength()+1];
	memset(cStrFromData,0,senddata.GetLength()+1);
	wcstombs(cStrFromData, senddata, senddata.GetLength());*/
	string strSend;
	strSend = (LPCSTR)CStringA(senddata);
	//CHttpConnection *pConnection = sess.GetHttpConnection(host,(INTERNET_PORT)port);
	//CHttpFile* pF=pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,url);


	CHttpFile* pF = (CHttpFile*)sess.OpenURL(url);
	
	CString sLogString(_T(""));
	if (senddata != _T("Web_ConnectCheck")){	
		sLogString.Format(_T("Webapi send: data = %s; url = %s"), senddata, url);
		theWEBAPI_TRACE->WriteData(__LINE__,_T(" >")+sLogString,NULL,0);
	}
	try{
		if(NULL != pF){
			//BOOL result = pF->SendRequest(strHeaders,(LPVOID)(LPCTSTR)cStrFromData, strlen(cStrFromData));

			// �߳�ͬ��ִ��
			m_Parm.pHttpFile = pF;
			m_Parm.pSendData = ""/*(CHAR*)strSend.c_str()*/;//д��Ϊ��

			ResetEvent(m_hWaitResult);
			SetEvent(m_hPauseThread);


			int iTimeout = nTimeOut;
			// �ȴ��������ݽ��
			if(WaitForSingleObject(m_hWaitResult,iTimeout) == WAIT_TIMEOUT){
				/*if(NULL != cStrFromData)
					delete []cStrFromData;*/

				pF->Close();
			//pConnection->Close();//lichao_debug
				sess.Close();
				
				WaitForSingleObject(m_hWaitResult,INFINITE);
				CString strErrCode;
				strErrCode.Format(_T("GetLastError() = %d"),GetLastError());

				theWEBAPI_TRACE->WriteData(__LINE__,_T("ERROR:���ӷ�������ʱ,Wait timeout") + strErrCode,NULL,0);
				return RSP_CODE_CONNECT_FAIL;
			}

			// �ж�ִ�н��
			if(RSP_CODE_OK != m_EmThreadReturn){
				/*if(NULL != cStrFromData)
					delete []cStrFromData;*/

				pF->Close();
			//pConnection->Close();//lichao_debug
				sess.Close();
				CString strErrCode;
				strErrCode.Format(_T("GetLastError() = %d"),GetLastError());

				theWEBAPI_TRACE->WriteData(__LINE__,_T("ERROR:��������ʧ��,") + strErrCode,NULL,0);
				return RSP_CODE_CONNECT_FAIL;
			}

			/*if(NULL != cStrFromData)
				delete []cStrFromData;*/

			/*if(!result)
			{
				pF->Close();
				sess.Close();
				pConnection->Close();
				return RSP_CODE_CONNECT_FAIL;
			}*/
			CString szData,szAllData;
			while(pF->ReadString(szData))
			{
				szAllData+=szData;
			}
			pF->Close();
			sess.Close();
			//pConnection->Close();//lichao_debug
			UTF8toANSI(szAllData);
			sLogString.Format(_T("Webapi recv: data = %s"), szAllData);
			szAllData.Replace(_T("\\\""),_T("\""));
			szAllData.Replace(_T("\"{"),_T("{"));
			szAllData.Replace(_T("}\""),_T("}"));
			getdata=szAllData;
			if (senddata != _T("Web_ConnectCheck")){
				if(getdata.Find(_T("<html>"))>=0){
					theWEBAPI_TRACE->WriteData(__LINE__,_T("<")+sLogString.Left(100),NULL,0);
				}
				else{
					theWEBAPI_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
				}
			}
		}
		else{
			// ��ǰ���Ӳ�����
			/*if(NULL != cStrFromData)
				delete []cStrFromData;*/
			sLogString = _T("Webapi connection error!");
			theWEBAPI_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
			return RSP_CODE_CONNECT_FAIL;
		}
	}
	catch(CException *e){

		/*if(NULL != cStrFromData)
			delete []cStrFromData;*/

		return RSP_CODE_LOGIN_FAIL;
	}
	catch(...){
		/*if(NULL != cStrFromData)
			delete []cStrFromData;*/
		return RSP_CODE_LOGIN_FAIL;
	}

	return RSP_CODE_OK;
}


// ���ɶ���
RETURN_CODE CWebServerHelper::Web_ConnectCheck()
{
	CString senddata = _T("");
	USES_CONVERSION;

	try
	{
		CString getdata;
		CString tmpData = connectURL;
		if(ServerPost(_T("Web_ConnectCheck")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			SetConnectFlag(false);
			return RSP_CODE_CONNECT_FAIL;
		}
	}
	catch(CException *e)
	{
		SetConnectFlag(false);
		return RSP_CODE_CONNECT_FAIL;
	}

	SetConnectFlag(true);

	return RSP_CODE_OK;
}

// ���ɶ���
RETURN_CODE CWebServerHelper::Weixin_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount)
{
	/*
	�������	1	device_code	String(32)	�豸���
				2	start_station	String(4)	������
				3	end_station	String(4)	�յ����
				4	total_amount	String(11)	�ܽ��
				5	discountable_amount	String (11)	�ɴ��۽������Żݼ���Ľ���λ����
				6	spbill_create_ip	String (16)	�ն�IP
				7	tick_price	String(8)	Ʊ�ۣ���λ����
				8	tick_total	String(8)	��Ʊ����
	�������	1	rtCode	String(32)	��00001��:�ɹ�; ��00000��:ʧ��;
				2	rtMessage	String(64)	������Ϣ
				3	rtData		code_url	String(32)	ɨ��֧����ά��
							out_trade_no	String��64��	�̻�������
							device_code		String��32��	�豸���
	ʾ��	����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_wxTradePrecreate?
						device_code=0123&start_station=0212&end_station=0113& spbill_create_ip=127.0.0.1&total_amount=2&discountable_amount=0&tick_price=2&tick_total=1
			����
					(json)	{"rtCode"��"00001"��
								"rtMessage"��"ɨ��֧��Ԥ�µ��ɹ�!"��
								"rtData"��"{ device_code":"0123"��"out_trade_no"��"tvm20171029033829980335"
								��"qr_code"��"weixin://wxpay/bizpayurl?pr=inYPWI3"}"}

	*/


	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	CString strTmp;

	senddata = _T("");
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d&"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp;
	// ������ʼվ��
	strTmp.Format(_T("start_station=%d&"),beginStation);
	senddata += strTmp;
	// ����Ŀ��վ��
	strTmp.Format(_T("end_station=%d&"),endStation);
	senddata += strTmp;
	// �����ܽ��
	strTmp.Format(_T("total_amount=%d&"),(tickePrice*ticketCount-discountAmount)/100);
	senddata += strTmp;
	// ����ɴ��۽��
	strTmp.Format(_T("discountable_amount=%d&"),discountAmount/100);
	senddata += strTmp;
	// �����ն�IP
	strTmp = _T("spbill_create_ip=") + theMAINTENANCE_INFO.GetIP() + _T("&");
	senddata += strTmp;
	// ����Ʊ��
	strTmp.Format(_T("tick_price=%d&"),tickePrice/100);
	senddata += strTmp;
	// ����Ʊ����
	strTmp.Format(_T("tick_total=%d"),ticketCount);
	senddata += strTmp;

	try
	{
		CString getdata;
		CString tmpData = baseURL+weixin_requestOrderURL;
		tmpData += _T("?") + senddata;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			//delete []resultdata;
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//delete []resultdata;
			if(!json_object["rtCode"].isNull()){
				CStringA strRetCode = json_object["rtCode"].asCString();
				if(strRetCode == "00001"){
					// �ɹ�
					if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
						CString sLogString = _T("Webapi return txnCode null error!");
						theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
						return RSP_CODE_ORDER_REQUEST_FAILED;
					}

					CString strRetMessage = A2T(json_object["rtMessage"].asCString());
					Json::Value vRetData = json_object["rtData"];
					if(vRetData["code_url"].isNull()
						|| vRetData["out_trade_no"].isNull()								
						|| vRetData["device_code"].isNull()){
							CString sLogString = _T("Webapi return txnCode null error!");
							theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
							return RSP_CODE_ORDER_REQUEST_FAILED;
					}

					CString strErweimaData = A2T(vRetData["code_url"].asCString());
					m_strOrderID = A2T(vRetData["out_trade_no"].asCString());
					CString strRetDeviceId = A2T(vRetData["device_code"].asCString());

					// ���ɶ�ά�룬���浽����
					CQRCodeHelper QRhelper;
					CString strFilePath;
					TCHAR szRunPath[MAX_PATH];
					ComGetAppPath(szRunPath, MAX_PATH);
					CString runPath(szRunPath);

					CString strFileName;
					strFileName.Format(_T("%s%d.bmp"),FILTER_QR_ORDER_IMAGES_RUN_PRE,0);

					// ���ɼ���Ϊ8�Ķ�ά��(����)
					strFilePath.Format(_T("%s%s%s"),runPath,"",strFileName);
					long lRet = QRhelper.QRCodeEncode(1,1,1,6,strErweimaData,strFilePath);

					if(lRet != 0){
						strFileName.Format(_T("%s%d.bmp"),FILTER_QR_ORDER_IMAGES_RUN_PRE,0);
						// ���ɼ���Ϊ8�Ķ�ά��(����)
						strFilePath.Format(_T("%s%s%s"),runPath,"",strFileName);
						lRet = QRhelper.QRCodeEncode(1,1,1,6,strErweimaData,strFilePath);
					}

					if(0 != lRet){
						return RSP_CODE_GENER_ERWEIMA_FAILED;
					}

					return RSP_CODE_OK;
				}
				else{
					return RSP_CODE_ORDER_REQUEST_FAILED;
				}
			}
			else{
				CString sLogString = _T("Webapi return txnCode null error!");
				theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
				return RSP_CODE_ORDER_REQUEST_FAILED;
			}

		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ݶ�ά�붩����Ų�ѯ��������
@param    (i)CString csOrderID ����ID
@param    (o) ORDER_INFO orderinfo ������Ϣ�ṹ��
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::Weixin_Query()
{
	/*
	�������	1	out_trade_no  	String(64)	����֧��ʱ������̻�������
				2	device_code	String��32��	�豸���
	�������	1	rtCode	String(32)	��00001��:֧���ɹ�
										��00000��:�����ѳ�Ʊ���ѹر�
										��00002��:δ֪����Ҫ�ȴ���������ѯ
										ע��ֻ�з���00001״̬�ſɳ�Ʊ
				2	rtMessage		String(64)	������Ϣ
				3	rtData		device_code 	String��32��	�豸���
								out_trade_no	String(64)	�̻�������
								trade_state	String(32)	����״̬
												01��δ֧��
												02: ��ȡ��
												03����֧��,δȡƱ��
												04����֧��,�˿���
												05����֧��,���˿�
												10: ��֧��,�ѳ�Ʊ
												14�������쳣
								total_amount	String (11)	���׵Ķ�������λ����
								receipt_amount  	String��11��	ʵ�ս���λ���֡��ý��Ϊ���ʽ��ף��̻��˻��ܹ�ʵ���յ��Ľ��
								time_end	String(14)	֧�����ʱyyyyMMddHHmmss
								start_station	String(4)	������
								end_station	String(4)	�յ����
								tick_total	String(8)	��Ʊ����
								tick_price	String(16)	����(����Ʊ�ļ۸񣬵�λ����)
	ʾ��	
		����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_wxTradeQuery?
					device_code=0123& out_trade_no��tvm20171029033829980335
		����
			(json)	{"rtCode"��"00001"��
			"rtMessage"��"֧���ɹ�!"��
			"rtData"��"{ "out_trade_no"��"tvm20171029033829980335", "trade_status":"03" ,
			"total_amount":"2"��" receipt_amount "��"2"��" start_station" ��"0212"��
			"end_station"��"0113"��"tick_total"��"1","tick_price " ��"2", device_code":"0123"��"time_end "��"20171031160457"}"}
	*/


	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	CString strTmp;
	senddata = _T("");

	// ���������
	strTmp = _T("out_trade_no=") + m_strOrderID + _T("&");
	senddata += strTmp; 
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp; 

	try
	{
		CString getdata;
		CString tmpData = baseURL+weixin_queryOrderURL;
		tmpData += _T("?") + senddata;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			//delete []resultdata;
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//delete []resultdata;
			if(!json_object["rtCode"].isNull()){
				CStringA strRetCode = json_object["rtCode"].asCString();
				if(strRetCode == "00001"){
					// �ɹ�
					if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
						CString sLogString = _T("Webapi return txnCode null error!");
						theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
						return RSP_CODE_ORDER_QUERY_FAILED;
					}

					CString strRetMessage = A2T(json_object["rtMessage"].asCString());
					Json::Value vRetData = json_object["rtData"];
					if(/*vRetData["device_code"].isNull()
						|| vRetData["out_trade_no"].isNull()
						|| */vRetData["trade_state"].isNull()
						/*|| vRetData["total_amount"].isNull()						
						|| vRetData["receipt_amount"].isNull()						
						|| vRetData["time_end"].isNull()		
						|| vRetData["start_station"].isNull()
						|| vRetData["end_station"].isNull()
						|| vRetData["tick_total"].isNull()						
						|| vRetData["tick_price"].isNull()*/){
							CString sLogString = _T("Webapi return txnCode null error!");
							theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
							return RSP_CODE_ORDER_QUERY_FAILED;
					}

					//CString strRetDeviceId = A2T(vRetData["device_code"].asCString());
					//CString strRetOrderID = A2T(vRetData["out_trade_no"].asCString());
					CString strTrade_state = A2T(vRetData["trade_state"].asCString());
					//CString strTotal_amount = A2T(vRetData["total_amount"].asCString());
					//CString strReceipt_amount = A2T(vRetData["receipt_amount"].asCString());
					//CString strTime_end = A2T(vRetData["time_end"].asCString());	
					//CString strStart_station = A2T(vRetData["start_station"].asCString());
					//CString strEnd_station = A2T(vRetData["end_station"].asCString());
					//CString strTick_total = A2T(vRetData["tick_total"].asCString());
					//CString strTick_price = A2T(vRetData["tick_price"].asCString());	

					// ��齻��״̬
					if(strTrade_state != _T("03")){
						return RSP_CODE_ORDER_QUERY_FAILED;
					}
				
					return RSP_CODE_OK;
				}
				else{
					return RSP_CODE_ORDER_QUERY_FAILED;
				}
			}
			else{
				CString sLogString = _T("Webapi return txnCode null error!");
				theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
				return RSP_CODE_ORDER_QUERY_FAILED;
			}
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_ORDER_QUERY_FAILED;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ȡ������
@param    (i)CString csOrderID ����ID
@param    (o) ORDER_INFO orderinfo ������Ϣ�ṹ��
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::Weixin_Cancel()
{
	/*
	�������	1	out_trade_no  	String(64)	����֧��ʱ������̻�������
				2	device_code	String��32��	�豸���
	�������	1	rtCode	String(32)	��00000��:ʧ��; ��00001��:�ɹ�;
				2	rtMessage	String(64)	
				3	rtData		
	ʾ��	
		����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_wxTradeCancel?
					device_code=0123& out_trade_no��tvm20171029033829980335
		����
		(json)	{"rtCode"��"00001"��
		"rtMessage"��"���������ɹ�!"��
		"rtData"��" " }
	*/

	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	CString strTmp;
	senddata = _T("");
	// ���������
	strTmp = _T("out_trade_no=") + m_strOrderID + _T("&");
	senddata += strTmp; 
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp; 
	try
	{
		CString getdata;
		CString tmpData = baseURL+weixin_cancelOrderURL;
		tmpData += _T("?") + senddata;
		m_nOrderProcessType = WEB_ORDER_CANCEL;
		m_strOrderProcessURL = tmpData;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//if(!json_object["rtCode"].isNull()){
			//	CStringA strRetCode = json_object["rtCode"].asCString();
			//	if(strRetCode == "00001"){
			//		// �ɹ�
			//		if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
			//			CString sLogString = _T("Webapi return txnCode null error!");
			//			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//			return RSP_CODE_ORDER_QUERY_FAILED;
			//		}

			//		CString strRetMessage = A2T(json_object["rtMessage"].asCString());
			//		CString strRetData = A2T(json_object["rtData"].asCString());

			//		return RSP_CODE_OK;
			//	}
			//	else{
			//		return RSP_CODE_ORDER_QUERY_FAILED;
			//	}
			//}
			//else{
			//	CString sLogString = _T("Webapi return txnCode null error!");
			//	theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//	return RSP_CODE_ORDER_QUERY_FAILED;
			//}

			return RSP_CODE_OK;
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief     ȡ������
@param    (i)CString csOrderID ����ID
@param    (o) ORDER_INFO orderinfo ������Ϣ�ṹ��
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::Web_Confirm(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo)
{
	/*
	�������	1	out_trade_no  	String(64)	����֧��ʱ������̻�������
				2	device_code	String��32��	�豸���
				3	ticket_num_total	String(2)	Ӧ��Ʊ����
				4	ticket_num_actual	String(2)	ʵ�ʳ�Ʊ��
				5	tick_data	��Ʊ���飬������	card_no	String (32)	Ʊ������
													write_time	String(19)	дƱʱ�� yyyy-MM-dd HH:mm:ss
													tick_time	String(19)	�ն˳�Ʊʱ�� yyyy-MM-dd HH:mm:ss
	�������	1	rtCode	String(32)	
				2	rtMessage	String(64)	������Ϣ
				3	rtData		
	ʾ��	
		����(tick_dataΪjson����)	
				http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_tickResultSubmit?
						device_code=0123&out_trade_no��tvm20171029033829980335&
						ticket_num_total=2&ticket_num_actual=2& 
						tick_data=[{card_no:222,write_time:2017-11-03 12:22:23,
						tick_time:2017-11-0312:22:23},{card_no:223,write_time:2017-11-03 12:22:23,tick_time:2017-11-03 12:22:23}]
	*/

	CString senddata;
	time_t timestamp;
	time(&timestamp);

	USES_CONVERSION;
	char sBufer[MAX_PATH] = {0};
	CString strTmp;
	//// ���������
	//strTmp = m_strOrderID;
	//sendparam["out_trade_no"] = Json::Value(T2A(strTmp));  
	//// �����豸���
	//strTmp.Format(_T("%.2d%.2d%.2d%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	//sendparam["device_code"] = Json::Value(T2A(strTmp));  
	//// ����Ӧ��Ʊ����
	//strTmp.Format(_T("%d"),requestTicketNum);
	//sendparam["ticket_num_total"] = Json::Value(T2A(strTmp));  
	//// ����ʵ�ʳ�Ʊ����
	//strTmp.Format(_T("%d"),actualTicketNum);
	//sendparam["ticket_num_actual"] = Json::Value(T2A(strTmp));  
	//// ��Ʊ����
	//Json::Value detailData;
	//detailData["card_no"] = Json::Value(T2A(ticketID));
	//detailData["write_time"] = Json::Value(T2A(strWriteTicketTime));
	//detailData["tick_time"] = Json::Value(T2A(strIssueTime));
	//sendparam["tick_data"] = detailData;  

	//// ����json��
	//CString paramdata(json_writer.write(sendparam).c_str());
	//paramdata.Replace(_T("\""),_T("\\\""));
	//senddata = paramdata.Trim();

	// ���������
	strTmp = _T("out_trade_no=") + m_strOrderID + _T("&");
	senddata += strTmp; 
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d&"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp; 
	// ����Ӧ��Ʊ����
	strTmp.Format(_T("ticket_num_total=%d&"),requestTicketNum);
	senddata += strTmp;
	// ����ʵ�ʳ�Ʊ����
	strTmp.Format(_T("ticket_num_actual=%d&"),actualTicketNum);
	senddata += strTmp;
	// ��Ʊ����
	strTmp = _T("tick_data=[");
	if(actualTicketNum==0){
		strTmp += _T("]");
	}

	for(int i=0;i<actualTicketNum;i++){
		CString strEle;
		Json::FastWriter json_writer;
		Json::Value detailData;
		detailData["card_no"] = Json::Value(T2A(actrualTicketInfo[i].ticketID));
		detailData["write_time"] = Json::Value(T2A(actrualTicketInfo[i].strWriteTicketTime));
		detailData["tick_time"] = Json::Value(T2A(actrualTicketInfo[i].strIssueTime));
		// ����json��
		CString paramdata(json_writer.write(detailData).c_str());
		//paramdata.Replace(_T("\""),_T(""));	
		strEle = paramdata.Trim();
		if(i!=actualTicketNum-1){
			strTmp +=  strEle + _T(",");
		}
		else{
			strTmp +=  strEle + _T("]");
		}
	}

	 senddata += strTmp;
	/*
		jsonת�������ĸ�ʽ��{\"device_code\":\"114102001\"}
		Ŀ���ʽ��tick_data=[{card_no:222,write_time:2017-11-03 12:22:23,tick_time:2017-11-0312:22:23},{card_no:223,write_time:2017-11-03 12:22:23,tick_time:2017-11-03 12:22:23}]
	*/

	try
	{
		CString getdata;
		CString tmpData = baseURL+confirmOrderURL;
		tmpData += _T("?") + senddata;
		m_nOrderProcessType = WEB_ORDER_CONFIRM;
		m_strOrderProcessURL = tmpData;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(0/*!reader.parse(resultdata, json_object)*/){
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//if(!json_object["rtCode"].isNull()){
			//	CStringA strRetCode = json_object["rtCode"].asCString();
			//	if(strRetCode == "00001"){
			//		// �ɹ�
			//		if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
			//			CString sLogString = _T("Webapi return txnCode null error!");
			//			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//			return RSP_CODE_ORDER_QUERY_FAILED;
			//		}

			//		CString strRetMessage = A2T(json_object["rtMessage"].asCString());
			//		CString strRetData = A2T(json_object["rtData"].asCString());

			//		return RSP_CODE_OK;
			//	}
			//	else{
			//		return RSP_CODE_ORDER_QUERY_FAILED;
			//	}
			//}
			//else{
			//	CString sLogString = _T("Webapi return txnCode null error!");
			//	theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//	return RSP_CODE_ORDER_QUERY_FAILED;
			//}

			return RSP_CODE_OK;
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}



// ���ɶ���
RETURN_CODE CWebServerHelper::Zhifubao_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount)
{
	/*
	�������	1	device_code	String��32��	�豸���
				2	total_amount	String (11)	�����ܽ���λ���֡�
				3	discountable_amount	String (11)	�ɴ��۽��. �����Żݼ���Ľ���λ����
				4	start_station	String(4)	������
				5	end_station	String(4)	�յ����
				6	tick_total	String(8)	��Ʊ����
				7	tick_price	String(16)	����(����Ʊ�ļ۸�100ԪΪ10000)
				8	spbill_create_ip	String (16)	�ն�IP
	�������	1	rtCode	String(32)	��00001��:�ɹ�; ��00000��:ʧ��;
				2	rtMessage	String(64)	������Ϣ
				3	rtData	device_code	String��32��	�豸���
							out_trade_no	String��64��	�̻�������
							qr_code	String(1024)	��ǰԤ�µ��������ɵĶ�ά�봮 
	ʾ��	
		����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_aliTradePrecreate?
					device_code=0123&start_station=0212&end_station=0113&total_amount=2&
					discountable_amount=0&tick_price=2&tick_total=1&spbill_create_ip=127.0.0.1
		����
				(json)	{"rtCode"��"00001"��
				"rtMessage"��"ɨ��֧���µ��ɹ�!"��
				"rtData"��"{ device_code":"0123"��"out_trade_no"��"tvm20171029033829980335"
				��"qr_code"��"https://qr.alipay.com/bax01015lsquwwishvhb80c4"}"}
	*/


	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	char sBufer[MAX_PATH] = {0};
	CString strTmp;
	//// �����豸���
	//strTmp.Format(_T("%.2d%.2d%.2d%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	//sendparam["device_code"] = Json::Value(T2A(strTmp)); 
	//// �����ܽ��
	//strTmp.Format(_T("%d"),tickePrice*ticketCount-discountAmount);
	//sendparam["total_amount"] = Json::Value(T2A(strTmp)); 
	//// ����ɴ��۽��
	//strTmp.Format(_T("%d"),discountAmount);
	//sendparam["discountable_amount"] = Json::Value(T2A(strTmp)); 
	//// ������ʼվ��
	//strTmp.Format(_T("%d"),beginStation);
	//sendparam["start_station"] = Json::Value(T2A(strTmp)); 
	//// ����Ŀ��վ��
	//strTmp.Format(_T("%d"),endStation);
	//sendparam["end_station"] = Json::Value(T2A(strTmp)); 
	//// ����Ʊ��
	//strTmp.Format(_T("%d"),tickePrice);
	//sendparam["tick_price"] = Json::Value(T2A(strTmp)); 
	//// ����Ʊ����
	//strTmp.Format(_T("%d"),ticketCount);
	//sendparam["tick_total"] = Json::Value(T2A(strTmp)); 
	//// �����ն�IP
	//strTmp = theMAINTENANCE_INFO.GetIP();
	//sendparam["spbill_create_ip"] = Json::Value(T2A(strTmp)); 

	//// ����json��
	//CString paramdata(json_writer.write(sendparam).c_str());
	//paramdata.Replace(_T("\""),_T("\\\""));
	//senddata = paramdata.Trim();

	senddata = _T("");
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d&"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp;
	// �����ܽ��
	strTmp.Format(_T("total_amount=%d&"),(tickePrice*ticketCount-discountAmount)/100);
	senddata += strTmp;
	// ����ɴ��۽��
	strTmp.Format(_T("discountable_amount=%d&"),discountAmount/100);
	senddata += strTmp;
	// ������ʼվ��
	strTmp.Format(_T("start_station=%d&"),beginStation);
	senddata += strTmp;
	// ����Ŀ��վ��
	strTmp.Format(_T("end_station=%d&"),endStation);
	senddata += strTmp;
	// ����Ʊ��
	strTmp.Format(_T("tick_price=%d&"),tickePrice/100);
	senddata += strTmp;
	// ����Ʊ����
	strTmp.Format(_T("tick_total=%d&"),ticketCount);
	senddata += strTmp;
	// �����ն�IP
	strTmp = _T("spbill_create_ip=") + theMAINTENANCE_INFO.GetIP();
	senddata += strTmp;

	try
	{

		CString getdata;
		CString tmpData = baseURL+zhifubao_requestOrderURL;
		tmpData += _T("?") + senddata;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		CString sLogString = _T("");
		if(!reader.parse(resultdata, json_object)){
			//delete []resultdata;
			CString sLogString = _T("parse error!");
			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//delete []resultdata;
			if(!json_object["rtCode"].isNull()){
				CStringA strRetCode = json_object["rtCode"].asCString();
				if(strRetCode == "00001"){
					// �ɹ�
					if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
						sLogString = _T("Webapi return txnCode null error!");
						theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
						return RSP_CODE_ORDER_REQUEST_FAILED;
					}

					CString strRetMessage = A2T(json_object["rtMessage"].asCString());
					Json::Value vRetData = json_object["rtData"];
					if(vRetData["qr_code"].isNull()
						|| vRetData["out_trade_no"].isNull()								
						|| vRetData["device_code"].isNull()){
							sLogString = _T("Webapi return txnCode null error!");
							theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
							return RSP_CODE_ORDER_REQUEST_FAILED;
					}				

					CString strErweimaData = A2T(vRetData["qr_code"].asCString());
					m_strOrderID = A2T(vRetData["out_trade_no"].asCString());
					CString strRetDeviceId = A2T(vRetData["device_code"].asCString());

					// ���ɶ�ά�룬���浽����
					CQRCodeHelper QRhelper;
					CString strFilePath;
					TCHAR szRunPath[MAX_PATH];
					ComGetAppPath(szRunPath, MAX_PATH);
					CString runPath(szRunPath);

					CString strFileName;
					strFileName.Format(_T("%s%d.bmp"),FILTER_QR_ORDER_IMAGES_RUN_PRE,0);

					// ���ɼ���Ϊ8�Ķ�ά��(����)
					strFilePath.Format(_T("%s%s%s"),runPath,"",strFileName);
					QRhelper.DeleteQRCode(strFilePath);
					long lRet = QRhelper.QRCodeEncode(1,1,1,6,strErweimaData,strFilePath);

					if(lRet == 0){
						strFileName.Format(_T("%s%d.bmp"),FILTER_QR_ORDER_IMAGES_RUN_PRE,0);
						// ���ɼ���Ϊ8�Ķ�ά��(����)
						strFilePath.Format(_T("%s%s%s"),runPath,"",strFileName);
						QRhelper.DeleteQRCode(strFilePath);
						lRet = QRhelper.QRCodeEncode(1,1,1,6,strErweimaData,strFilePath);
					}
					else{
						sLogString = _T("generate erweima failed!");
						theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
						return RSP_CODE_GENER_ERWEIMA_FAILED;
					}

					return RSP_CODE_OK;
				}
				else{
					sLogString = _T("request order failed!");
					theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
					return RSP_CODE_ORDER_REQUEST_FAILED;
				}
			}
			else{
				CString sLogString = _T("Webapi return txnCode null error!");
				theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
				return RSP_CODE_ORDER_REQUEST_FAILED;
			}

		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ݶ�ά�붩����Ų�ѯ��������
@param    (i)CString csOrderID ����ID
@param    (o) ORDER_INFO orderinfo ������Ϣ�ṹ��
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::Zhifubao_Query()
{
	/*
	�������	1	out_trade_no  	String(64)	����֧��ʱ������̻�������
				2	device_code	String��32��	�豸���
	�������	1	rtCode	String(32)	��00001��:֧���ɹ�
										��00000��:�����ѳ�Ʊ���ѹر� ��00002��:δ֪����Ҫ�ȴ���������ѯ;
										ע��ֻ�з���00001״̬�ſɳ�Ʊ
				2	rtMessage	String(64)	������Ϣ
				3	rtData	trade_no 	String (64)	֧�������׺�
							out_trade_no 	String(64)	�̻�������
							trade_status 	String (32)	��������״̬
											01��δ֧��
											02: ��ȡ��
											03����֧��,δȡƱ��
											04����֧��,�˿���
											05����֧��,���˿�
											10: ��֧��,�ѳ�Ʊ
											14�������쳣
							total_amount  	tring (11)	���׵Ķ�������λ����
							receipt_amount  	String��11��	ʵ�ս���λ���֡��ý��Ϊ���ʽ��ף��̻��˻��ܹ�ʵ���յ��Ľ��
							start_station	String(4)	������
							end_station	String(4)	�յ����
							tick_total	String(8)	��Ʊ����
							tick_price	String(16)	����(����Ʊ�ļ۸񣬵�λ����)
							device_code	String��32��	�豸���
	ʾ��	
		����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_aliTradeQuery?
					device_code=0123&out_trade_no��tvm20171029033829980335
		����(json)	{"rtCode"��"00001"��
					"rtMessage"��"֧���ɹ�!"��
					"rtData"��"{ "trade_no":"4200000008201710311462446213",
					"out_trade_no"��"tvm20171029033829980335", "trade_status":"03" ,
					"total_amount":"2"��" receipt_amount "��"2"��" start_station" ��"0212"��
					"end_station"��"0113"��"tick_total"��"1","tick_price "��"2", device_code":"0123"}"}
	*/
	

	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	char sBufer[MAX_PATH] = {0};
	CString strTmp;

	//// ���������
	//strTmp = m_strOrderID;
	//sendparam["out_trade_no"] = Json::Value(T2A(strTmp));  
	//// �����豸���
	//strTmp.Format(_T("%.2d%.2d%.2d%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	//sendparam["device_code"] = Json::Value(T2A(strTmp));  

	//// ����json��
	//CString paramdata(json_writer.write(sendparam).c_str());
	//paramdata.Replace(_T("\""),_T("\\\""));
	//senddata = paramdata.Trim();
	//senddata += strTmp;

	senddata = _T("");
	// ���������
	strTmp = _T("out_trade_no=") + m_strOrderID + _T("&");
	senddata += strTmp; 
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp; 


	try
	{
		CString getdata;
		CString tmpData = baseURL+zhifubao_queryOrderURL;
		tmpData += _T("?") + senddata;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			//delete []resultdata;
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//delete []resultdata;
			if(!json_object["rtCode"].isNull()){
				CStringA strRetCode = json_object["rtCode"].asCString();
				if(strRetCode == "00001"){
					// �ɹ�
					if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
						CString sLogString = _T("Webapi return txnCode null error!");
						theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
						return RSP_CODE_ORDER_QUERY_FAILED;
					}

					CString strRetMessage = A2T(json_object["rtMessage"].asCString());
					Json::Value vRetData = json_object["rtData"];
					if(/*vRetData["device_code"].isNull()
						|| vRetData["out_trade_no"].isNull()*/
						/*||*/ vRetData["trade_status"].isNull()//lichao
/*						|| vRetData["total_amount"].isNull()	*/	// lichao:��ʱע��						
						//|| vRetData["receipt_amount"].isNull()						
						//|| vRetData["start_station"].isNull()
						//|| vRetData["end_station"].isNull()
						//|| vRetData["tick_total"].isNull()		// lichao:��ʱע��				
						/*|| vRetData["tick_price"].isNull()*/){
							CString sLogString = _T("Webapi return txnCode null error!");
							theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
							return RSP_CODE_ORDER_QUERY_FAILED;
					}


					//CString strRetDeviceId = A2T(vRetData["device_code"].asCString());
					//CString strRetOrderID = A2T(vRetData["out_trade_no"].asCString());
					CString strTrade_state = A2T(vRetData["trade_status"].asCString());//lichao
					//CString strTotal_amount = A2T(vRetData["total_amount"].asCString());
					//CString strReceipt_amount = A2T(vRetData["receipt_amount"].asCString());
					//CString strTime_end = A2T(vRetData["time_end"].asCString());	
					//CString strStart_station = A2T(vRetData["start_station"].asCString());
					//CString strEnd_station = A2T(vRetData["end_station"].asCString());
					////CString strTick_total = A2T(vRetData["tick_total"].asCString());		
					//CString strTick_price = A2T(vRetData["tick_price"].asCString());	

					// ��齻��״̬
					if(strTrade_state!=_T("03")){
						return RSP_CODE_ORDER_QUERY_FAILED;
					}

					return RSP_CODE_OK;
				}
				else{
					return RSP_CODE_ORDER_QUERY_FAILED;
				}
			}
			else{
				CString sLogString = _T("Webapi return txnCode null error!");
				theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
				return RSP_CODE_ORDER_QUERY_FAILED;
			}
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ȡ������
@param    (i)CString csOrderID ����ID
@param    (o) ORDER_INFO orderinfo ������Ϣ�ṹ��
@retval     RETURN_CODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RETURN_CODE CWebServerHelper::Zhifubao_Cancel()
{
	/*
	�������	1	out_trade_no  	String(64)	����֧��ʱ������̻�������
				2	device_code	String��32��	�豸���
	�������	1	rtCode	String(32)	��00000��:ʧ��; 
										��00001��:�ɹ�;
				2	rtMessage	String(64)	
				3	rtData		
	ʾ��	
		����	http//:[ip]:[port]/IAFCFTFPaySystem/tvm/QRpayAction_ aliTradeCancel?
					device_code=0123& out_trade_no��tvm20171029033829980335
	����
				(json)	{"rtCode"��"00001"��
				"rtMessage"��"���������ɹ�!"��
				"rtData"��" " }
	*/

	CString senddata;
	time_t timestamp;
	time(&timestamp);
	Json::Value sendparam;
	Json::FastWriter json_writer;

	USES_CONVERSION;
	char sBufer[MAX_PATH] = {0};
	CString strTmp;
	//// ���������
	//strTmp = m_strOrderID;
	//sendparam["out_trade_no"] = Json::Value(T2A(strTmp));  
	//// �����豸���
	//strTmp.Format(_T("%.2d%.2d%.2d%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	//sendparam["device_code"] = Json::Value(T2A(strTmp));  

	//// ����json��
	//CString paramdata(json_writer.write(sendparam).c_str());
	//paramdata.Replace(_T("\""),_T("\\\""));
	//senddata = paramdata.Trim();

	senddata = _T("");
	// ���������
	strTmp = _T("out_trade_no=") + m_strOrderID + _T("&");
	senddata += strTmp; 
	// �����豸���
	strTmp.Format(_T("device_code=%.2d%.2d%.2d%.2d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	senddata += strTmp; 

	try
	{
		CString getdata;
		CString tmpData = baseURL+zhifubao_cancelOrderURL;
		tmpData += _T("?") + senddata;
		m_nOrderProcessType = WEB_ORDER_CANCEL;
		m_strOrderProcessURL = tmpData;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			//delete []resultdata;
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//if(!json_object["rtCode"].isNull()){
			//	CStringA strRetCode = json_object["rtCode"].asCString();
			//	if(strRetCode == "00001"){
			//		// �ɹ�
			//		if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
			//			CString sLogString = _T("Webapi return txnCode null error!");
			//			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//			return RSP_CODE_ORDER_QUERY_FAILED;
			//		}

			//		CString strRetMessage = A2T(json_object["rtMessage"].asCString());
			//		CString strRetData = A2T(json_object["rtData"].asCString());

			//		return RSP_CODE_OK;
			//	}
			//	else{
			//		return RSP_CODE_ORDER_QUERY_FAILED;
			//	}
			//}
			//else{
			//	CString sLogString = _T("Webapi return txnCode null error!");
			//	theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//	return RSP_CODE_ORDER_QUERY_FAILED;
			//}

			return RSP_CODE_OK;
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}


RETURN_CODE CWebServerHelper::Web_IdleConfirm(CString strConfirmURL)
{
	try
	{
		CString getdata;
		CString tmpData = strConfirmURL;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		USES_CONVERSION;
		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(0/*!reader.parse(resultdata, json_object)*/){
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//if(!json_object["rtCode"].isNull()){
			//	CStringA strRetCode = json_object["rtCode"].asCString();
			//	if(strRetCode == "00001"){
			//		// �ɹ�
			//		if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
			//			CString sLogString = _T("Webapi return txnCode null error!");
			//			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//			return RSP_CODE_ORDER_QUERY_FAILED;
			//		}

			//		CString strRetMessage = A2T(json_object["rtMessage"].asCString());
			//		CString strRetData = A2T(json_object["rtData"].asCString());

			//		return RSP_CODE_OK;
			//	}
			//	else{
			//		return RSP_CODE_ORDER_QUERY_FAILED;
			//	}
			//}
			//else{
			//	CString sLogString = _T("Webapi return txnCode null error!");
			//	theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//	return RSP_CODE_ORDER_QUERY_FAILED;
			//}

			return RSP_CODE_OK;
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}

RETURN_CODE CWebServerHelper::Web_IdleCancel(CString strCancelURL)
{
	try
	{
		CString getdata;
		CString tmpData = strCancelURL;
		if(ServerPost(_T("")/*senddata*/,tmpData,getdata)!=RSP_CODE_OK){
			return RSP_CODE_CONNECT_FAIL;
		}

		USES_CONVERSION;
		Json::Reader reader;
		Json::Value json_object;		
		//CHAR *resultdata=new CHAR[getdata.GetLength()+1];
		//wcstombs(resultdata, getdata, getdata.GetLength());
		string resultdata;
		resultdata = (LPCSTR)CStringA(getdata);
		if(!reader.parse(resultdata, json_object)){
			return RSP_CODE_CONNECT_FAIL;
		}
		else{
			//if(!json_object["rtCode"].isNull()){
			//	CStringA strRetCode = json_object["rtCode"].asCString();
			//	if(strRetCode == "00001"){
			//		// �ɹ�
			//		if(json_object["rtMessage"].isNull() || json_object["rtData"].isNull()){
			//			CString sLogString = _T("Webapi return txnCode null error!");
			//			theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//			return RSP_CODE_ORDER_QUERY_FAILED;
			//		}

			//		CString strRetMessage = A2T(json_object["rtMessage"].asCString());
			//		CString strRetData = A2T(json_object["rtData"].asCString());

			//		return RSP_CODE_OK;
			//	}
			//	else{
			//		return RSP_CODE_ORDER_QUERY_FAILED;
			//	}
			//}
			//else{
			//	CString sLogString = _T("Webapi return txnCode null error!");
			//	theWEBAPI_TRACE->WriteData(__LINE__,_T("-")+sLogString,NULL,0);
			//	return RSP_CODE_ORDER_QUERY_FAILED;
			//}

			return RSP_CODE_OK;
		}
	}
	catch(CException *e)
	{
		return RSP_CODE_CONNECT_FAIL;
	}

	return RSP_CODE_OK;
}


RETURN_CODE CWebServerHelper::Weixin_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount)
{
	EnableIdleProcess(false);//ֹͣ���ش���δ���ͳɹ�����
	return Weixin_Requeset(beginStation, endStation, tickePrice, ticketCount, discountAmount);
}

RETURN_CODE CWebServerHelper::Weixin_QueryOrder()
{
	return Weixin_Query();;
}

RETURN_CODE CWebServerHelper::Weixin_CancelOrder()
{
	RETURN_CODE retCode = RSP_CODE_OK;
	int retryCount = 0;
	do{
		retCode = Weixin_Cancel();
		if(RSP_CODE_OK==retCode){
			break;
		}
		retryCount++;
	}while(retryCount<m_nOrderRetryTimes);

	if(RSP_CODE_OK==retCode){
		m_nOrderProcessType = 0;
		m_strOrderProcessURL = _T("0");
	}
	else{
		SaveLocalOrder();
	}
	EnableIdleProcess(true);

	return retCode;
}

RETURN_CODE CWebServerHelper::Zhifubao_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount)
{
	EnableIdleProcess(false);//ֹͣ���ش���δ���ͳɹ�����
	return Zhifubao_Requeset(beginStation, endStation, tickePrice, ticketCount, discountAmount);
}

RETURN_CODE CWebServerHelper::Zhifubao_QueryOrder()
{
	return Zhifubao_Query();
}

RETURN_CODE CWebServerHelper::Zhifubao_CancelOrder()
{
	RETURN_CODE retCode = RSP_CODE_OK;
	int retryCount = 0;
	do{
		retCode = Zhifubao_Cancel();
		if(RSP_CODE_OK==retCode){
			break;
		}
		retryCount++;
	}while(retryCount<m_nOrderRetryTimes);

	if(RSP_CODE_OK==retCode){
		m_nOrderProcessType = 0;
		m_strOrderProcessURL = _T("0");
	}
	else{
		SaveLocalOrder();
	}
	EnableIdleProcess(true);

	return retCode;
}


RETURN_CODE CWebServerHelper::Web_ConfirmOrder(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo)
{
	RETURN_CODE retCode = RSP_CODE_OK;
	int retryCount = 0;
	do{
		if (0!=actualTicketNum && NULL==actrualTicketInfo){
			CString sLogString = _T("��Ʊ����ύʱƱ����Ϣ��Ч������");
			theWEBAPI_TRACE->WriteData(__LINE__,_T(" >")+sLogString,NULL,0);
			break;
		}
		retCode = Web_Confirm(requestTicketNum, actualTicketNum, actrualTicketInfo);
		if(RSP_CODE_OK==retCode){
			break;
		}
		retryCount++;
	}while(retryCount<m_nOrderRetryTimes);

	if(RSP_CODE_OK==retCode){
		m_nOrderProcessType = 0;
		m_strOrderProcessURL = _T("0");
	}
	else{
		SaveLocalOrder();
	}
	EnableIdleProcess(true);

	return retCode;
}


RETURN_CODE CWebServerHelper::Web_IdleProcessOrder()
{
	RETURN_CODE ret = RSP_CODE_OK;

	if(!IsExistLocalOrder()){
		return ret;
	}

	int count = 0;
	do{
		if(WEB_ORDER_CANCEL == m_nOrderProcessType){
			ret = Web_IdleCancel(m_strOrderProcessURL);
		}
		else if(WEB_ORDER_CONFIRM == m_nOrderProcessType){
			ret = Web_IdleConfirm(m_strOrderProcessURL);
		}

		if(RSP_CODE_OK==ret){
			break;
		}
		count++;
	}while(count<m_nOrderRetryTimes);

	if (RSP_CODE_OK==ret){
		ClearLocalOrder();
	}

	return RSP_CODE_OK;
}


void CWebServerHelper::EnableIdleProcess(bool bEnabled)
{
	m_bLoacalProcessEnable = bEnabled;
}


bool CWebServerHelper::IsEnableIdleProcess()
{
	return m_bLoacalProcessEnable;
}

bool CWebServerHelper::IsExistLocalOrder()
{
	return 	(m_nOrderProcessType!=0) && (m_strOrderProcessURL!=_T("0"));
}

void CWebServerHelper::ClearLocalOrder()
{
	m_nOrderProcessType = 0;
	m_strOrderProcessURL = _T("0");
	theTVM_SETTING.SetWebIdleProcessType(_T("0"));
	theTVM_SETTING.SetWebIdleProcessURL(_T("0"));
}

void CWebServerHelper::SaveLocalOrder()
{
	CString strTmp;
	strTmp.Format(_T("%d"), m_nOrderProcessType);
	theTVM_SETTING.SetWebIdleProcessType(strTmp);
	theTVM_SETTING.SetWebIdleProcessURL(m_strOrderProcessURL);
}


void CWebServerHelper::SetConnectFlag(bool bConnected)
{
	m_bCurConnect = bConnected;
}


bool CWebServerHelper::GetConnectFlag()
{
	return m_bCurConnect;
}