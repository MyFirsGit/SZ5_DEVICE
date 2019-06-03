#pragma once
#include "stdafx.h"
#include "scapi.h"
#include "sccommand.h"
#include <list>
#include "scexp.h"


class SC_API CSCClientHelper
{
public:
	~CSCClientHelper();
	static CSCClientHelper& GetInstance();
private:

	CSCClientHelper();
	
	// 分包数据
	typedef struct _package_data 
	{
		BYTE bData[MAX_TCP_TRANS_SIZE+40];
		int nLen;
		_package_data(){
			memset(bData, 0, MAX_TCP_TRANS_SIZE);
			nLen = 0;
		}
	}PACKAGE_DATA,*LPPACKAGE_DATA;
	typedef CMap<UCHAR,UCHAR,LPPACKAGE_DATA,LPPACKAGE_DATA> CARRAY_PACKAGE_DATA;
	typedef enum _StartTimerReason
	{
		RECEIVE_NACK,
		RECEIVE_PART,
		RECEIVE_COMPLETE
	} STARTTIMERREASON;
	typedef enum _Timers
	{
		SEND_TIMEOUT_TIMER = 0x01,
		RECEIVE_TIMEOUT_TIMER,
		ENQUIRELINK_TIMER,
	} TIMERS;
	
	static CMap<UINT,UINT,CSCClientHelper*,CSCClientHelper*> s_CallbackObjects;

	UINT_PTR iSendTimeOutTimer;
	UINT_PTR iReceiveTimeOutTimer;
	UINT_PTR iEnquireLinkTimer;

	UINT iSendTimeOutInterval;
	UINT iReceiveTimeOutInterval;
	UINT iEnquireLinkInterval;
	void SetTimerInterval(UINT iEnquireLinkInterval,UINT iSendTimeOutInterval,UINT iReceiveTimeOutInterval);
	long MakePackages(CSCCommand*,CARRAY_PACKAGE_DATA &);  
	bool SaveOnePackage(VARIABLE_DATA& buffer);
	bool IsPackageOK(VARIABLE_DATA& package,SC_NACK_CODE& nackCode );
	bool IsKeyError(SC_NACK_CODE& nackCode);
	void ComposePackagesToCommandMsg(VARIABLE_DATA& commandMsg);
	bool IsCommandMsgOK(VARIABLE_DATA& commandMsg,SC_NACK_CODE& nackCode);
	
	void StartTimer(STARTTIMERREASON reason);
	void StopTimer();

	void ClearPackagesBuffer();
	
	delegate<void()> ReceiveTimeouted;
	static void CALLBACK OnReceiveTimeout(HWND,UINT,UINT,DWORD);//接受数据超时处理
	delegate<void()> SendTimeouted;
	static void CALLBACK OnSendTimeout(HWND,UINT,UINT,DWORD);	//发送数据超时处理
	delegate<void()> EnquireLinked;
	static void CALLBACK OnEnquireLinkTimer(HWND,UINT,UINT,DWORD);//需要发送心跳

	void StartSendTimeOutTimer();
	void StopSendTimeOutTimer();
	void StartReceiveTimeOutTimer();
	void StopReceiveTimeOutTimer();
	void StartEnquireLinkTimer();
	void StopEnquireLinkTimer();

	long MakePDU(CMD_HEADER& header, LPBYTE lpInData, UINT nInLen,LPBYTE lpOutData, UINT &nOutLen);
	long SplitIntoPackages(CMD_HEADER& header, LPVARIABLE_DATA body, CARRAY_PACKAGE_DATA &carryBlock);  

	CARRAY_PACKAGE_DATA  currentReceiveMessage;

	friend class CSCClient;

};