#include "StdAfx.h"
#include "FunctionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//static CString FUNCTION_INI_FILE_NAME = _T("TVM_Functioninfo.INI");
CFunctionInfo CFunctionInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::CFunctionInfo()
{
    try {
        m_functionIni = NULL;
		// [TVM_WEB_API]
		//m_strWebMobileHost = _T("");
		//m_WebApiHost       = _T("");
		m_WebApiPort       = 0;
		//m_WebApiPwd        = _T("");
		m_WebApiLoginUrl   = _T("");					 // WEB_API  ��¼��ַ
		//m_WebApiQueryUrl   = _T("");					 // WEB_API  ��ѯ��ַ
		m_WebApiLockUrl    = _T("");					 // WEB_API  ������ַ
		//m_WebApiConfirmUrl = _T("");					 // WEB_API  �ύ��ַ
		//m_WebApiGetOrderUrl   = _T("");					 // WEB_API  ȡƱ���ȡ�����ŵ�ַ
		//m_WebApiTVMStateUrl    = _T("");				 // WEB_API  ��ѯTVM״̬��ַ
		//m_WebApiUpdateStateUrl = _T("");				 // WEB_API  �޸�TVM״̬��ַ
        // ���ڻ�
        Initialize();
    }
    catch (...) {
        // �����쳣��Ϣ
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::~CFunctionInfo()
{
    delete m_functionIni;
    m_functionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::Initialize() 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
        if (m_functionIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }
	
        CString fileName = CString(szAppPath) + FUNCTION_INI_FILE_NAME;

        // check Functioninfo.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // �ļ����Ը�Ϊ��д
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_functionIni = new CGetIni(fileName);
		//[APPLICATION]
		m_accAuthorizeManner = m_functionIni->GetDataInt(_T("APPLICATION"), _T("ACCAuthorizeManner"),2);			// ACC��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		m_ectAuthorizeManner = m_functionIni->GetDataInt(_T("APPLICATION"), _T("ECTAuthorizeManner"),2);			// ECT��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��

		// [TVM_WEB_API]
		//m_strWebMobileHost = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("WebHost"));
		m_WebApiHost       = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("Host"));
		m_WebApiPort       = m_functionIni->GetDataInt(_T("TVM_WEB_API"), _T("Port"));
		//m_WebApiPwd       = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("UserPwd"));
		m_WebApiLoginUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("LoginUrl"));
		//m_WebApiQueryUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("QueryUrl"));
		m_WebApiLockUrl    = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("LockOperatorUrl"));
		/*m_WebApiConfirmUrl = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("ConfirmUrl"));
		m_WebApiGetOrderUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("GetOrderUrl"));
		m_WebApiTVMStateUrl    = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("TVMStatUrl"));
		m_WebApiUpdateStateUrl = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("UpdateStateUrl"));*/

		return 0;
   }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж�ָ����·��ť�Ƿ����

@param       BYTE lineCode ��·����

@retval      bool true:���� ; false:������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsLineEnabled(DWORD lineCode) 
{
	if(mapLineEnable.find(lineCode) == mapLineEnable.end())
	{
		CString slineCode(_T(""));
		slineCode.Format(_T("%d"),lineCode);
		DWORD lineStatus = m_functionIni->GetDataInt(_T("LINE_ENABLE"), slineCode, 1);
		mapLineEnable[lineCode] =  (lineStatus != 0);
	}
	return mapLineEnable[lineCode];
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж�ָ����վ��ť�Ƿ����

@param       WORD stationCode ��վ����

@retval      bool true:����ǰ�� ; false:������ǰ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsStationEnabled(WORD stationCode)
{
	if(mapStationEnable.find(stationCode) == mapStationEnable.end())
	{
		CString sStationCode(_T(""));
		sStationCode.Format(_T("%d"),sStationCode);
		DWORD stationStatus = m_functionIni->GetDataInt(_T("STATION_ENABLE"), sStationCode, 1);
		mapStationEnable[stationCode] = (stationStatus != 0);
	}
	return mapStationEnable[stationCode];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �жϲ˵���ť�Ƿ����

@param       (i)WORD menuID			�˵�ID
@param       (i)CString menuText	��ť����

@retval      bool true:�ɼ� ; false:���ɼ�

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuEnabled(WORD menuID)
{
	CString key(_T(""));
	key.Format(_T("%d"),menuID);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_ENABLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuVisibled(WORD menuID)
{
	CString key(_T(""));
	key.Format(_T("%d"),menuID);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_VISIBLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж�Ʊ���Ƿ�ɷ���

@param       (i)WORD TicketTypeId	Ʊ��ID
@param       (i)CString menuText	��ť����

@retval      bool true:�� ; false:����

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTicketIssueEnabled(WORD TicketTypeId)
{
	CString key(_T(""));
	key.Format(_T("%d"),TicketTypeId);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("ISSUE_ENABLE"), key, 0);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �жϿ��ø�������

@param       (i)WORD PayTypeId	���ѷ�ʽID

@retval      bool true:�� ; false:����

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsPayTypeEnable(WORD ServiceID,CardType_t TicketType,int PayTypeId)
{
	
	CString PayTypeNames = _T("");

	CString key(_T(""));
	key.Format(_T("%.2d"),PayTypeId);
	switch(ServiceID){
		case ISSUE_SVC:			
			PayTypeNames =_T("ISSUE");
			break;
		case CHARGE_SVC:		
			PayTypeNames =_T("CHARGE");	
			break;
		case ISSUE_PRODUCT_SVC:
			PayTypeNames =_T("ISSUE_PRODUCT");	
			break;
	}
	switch(TicketType){
		/*case CARD_TYPE_MACAUPSSS_M1:
			PayTypeNames +=_T("_M1");
			break;*/
		//case CARD_TYPE_MACAUPSSS_CPU:
		//	PayTypeNames +=_T("_CPU");
		//	break;
		case CARD_TYPE_ACC_UL:
		case CARD_TYPE_SY_EPT_UL:
			PayTypeNames +=_T("_UL");
			break;
		case CARD_TYPE_ACC_M1:
			PayTypeNames +=_T("_M1");
			break;
	}
	return m_functionIni->GetDataInt(PayTypeNames,key,0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �жϸ��ʽ�Ƿ����

@param       (i)WORD PayTypeId	���ѷ�ʽID

@retval      bool true:�� ; false:����

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CFunctionInfo::GetTicketCounts()
{
	if (m_vTicketCounts.size()!=0){
		return m_vTicketCounts;
	}

	for (int i =1;;i++ ){
		CString key(_T(""));
		key.Format(_T("%d"),i);
		int ticketCounts = m_functionIni->GetDataInt(_T("TICKET_COUNT"),key,-1);
		if (ticketCounts != -1){
			m_vTicketCounts.push_back(ticketCounts);
		}
		else{
			break;
		}
	}
	return m_vTicketCounts;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����ƱƱ��

@param      ��

@retval     vector<int> m_vTicketAmout;(����Ʊ��)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CFunctionInfo::GetTicketAmount(){
	if (m_vTicketAmount.size()!=0){
		return m_vTicketAmount;
	}

	for (int i =1;;i++ ){
		CString key(_T(""));
		key.Format(_T("%d"),i);
		int ticketAmounts = m_functionIni->GetDataInt(_T("TICKET_AMOUNT"),key,-1);
		if (ticketAmounts == -1){
			break;
		}
		else if(ticketAmounts != 0){
			m_vTicketAmount.push_back(ticketAmounts);
		}
	}
	return m_vTicketAmount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�������δ����������

@param       �� 

@retval      UINT

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CFunctionInfo::GetLimitedDaysOfUnSendData()
{
	return m_functionIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ�������ݱ�������
//
//@param       �� 
//
//@retval      UINT
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
UINT CFunctionInfo::GetLimitedDaysOfSendData()
{
	return m_functionIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       �����������ݱ�������

//@param       �� 
//
//@retval      UINT
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
void CFunctionInfo::SetLimitedDaysOfSendData(int nDays)
{
	CString strTemp;
	strTemp.Format(_T("%d"),nDays);
	m_functionIni->WriteData(_T("TXNDATA"),_T("SendedMaxDay"),strTemp);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       �Ƿ����÷���Ա��
//
//@param       ��
//
//@retval      bool true:���� ; false:������
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsUseFounderStaffID()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ����Ա��ID

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetFounderStaffID()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("FounderStaffID"), 10101028);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsFounderStaffID(int userID)
{
	return IsUseFounderStaffID() && (GetFounderStaffID() == userID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡŷķ��Ա������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetFounderStaffPassword()
{
	return m_functionIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}
////////////////////////////////////////////////////////////////////////////
//**
//@brief       ��ȡACC��֤��ʽ
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
int  CFunctionInfo::GetACCAuthorizeManner()
{
	return m_accAuthorizeManner;
}

////////////////////////////////////////////////////////////////////////////
//**
//@brief       ��ȡECT��֤��ʽ
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
int  CFunctionInfo::GetECTAuthorizeManner()
{
	return m_ectAuthorizeManner;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      TPU����ѹ����־��0����ѹ����1����Ҫѹ����

@param      �� 

@retval     bool

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTpuParamterZipFlag()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("TpuParamterZipFlag"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ����ø߼����ܣ�0�������ã�1�����ã�

@param      �� 

@retval     bool

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsFounderOperation()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("FounderOperation"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      USB��֤��־��0������֤��1����Ҫ��֤��

@param      �� 

@retval     bool

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::GetUSBAuthFlag()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ�ɾ������Э��ͷ

@param      ��

@retval     bool true:ɾ����false����ɾ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsDeleteParameterHead(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsDeleteParameterHead"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ���ʾ��ʼ������

@param      ��

@retval     bool true:��ʾ��false������ʾ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowInitProcess(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowInitProcess"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �豸�Զ�����״̬�Ƿ�����

@param      ��

@retval     bool true:���� false : �ر�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsEquipmentAutomaticRunningOn(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("EquipmentAutomaticRunningOnOff"),0) == 0 ? false:true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ���ʾ�豸�ռ����ղ���ֵ��

@param      ��

@retval     bool true:��ʾ false : ����ʾ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowNearlyFullEmptyParaValue(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowNearlyFullEmptyParaValue"),0) == 0 ? false:true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����Ĵ�ӡʧ�ܵ�����

@param      ��

@retval     (O)int ��Ҫ��ӡ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetPrintLastReceiptCount(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("PrintLastReceiptCount"),16);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ���ʾ֧����ʽ����

@param      ��

@retval     bool true ��ʾ; false ����ʾ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowPaymentMethod(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowPaymentMethod"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ���Ҫ�û����ȷ�ϰ�ť

@param      ��

@retval     bool true ��Ҫ; false ����Ҫ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsNeedPassengerClickOkButton(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsNeedPassengerClickOkButton"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ���δ�ر�ʱ�Ƿ񽵼���

@param      ��

@retval     bool��true������false��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsAutoDegradeWhenCashDoorOpen(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsAutoDegradeWhenCashDoorOpen"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ��Զ���ȡֽ������ģ��RFID

@param      ��

@retval     bool true(��);false(��);

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsBanknoteChangeBoxIDRead(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("BanknoteChangeBoxIDRead"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ɼ��ؽ�����ʷ��¼����

@param      ��

@retval     int nCount;����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetLoadMaxTransactionHistory(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("LoadMaxTransactionHistory"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ա��¼��Ĭ��ҵ��Service ID��

@param      �� 

@retval     int��Service ID��

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsStaffDefaultService()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("StaffDefaultService"));
}

//************=====RFID����=========**************//
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCardNormalBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CardNormalBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCardWasteBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CardWasteBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCoinChangeBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CoinChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCoinRecycleBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CoinChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsBanknoteChangeBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("BanknoteChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
bool CFunctionInfo::IsBanknoteRecycleBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("BanknoteRecycleBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTokenNormalBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("TokenNormalBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTokenWasteBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("TokenWasteBoxRFIDWriteable"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ��Զ���ȡRFID

@param      ��

@retval     bool true:�����ֶ�����; false:��Ҫ�ֶ�����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsAutoReadRfidWithoutManual(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsAutoReadRFIDWithoutManual"),0) == 0 ? false : true;
}

//************=====�����趨=========**************//
//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ն�������������·��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsCollectOverFile()
{
    return m_functionIni->GetDataString(_T("ALARM"), _T("CollectOverFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ն�����������������ʱ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsCollectOverBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("CollectOverBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ն�����������������ʱ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsCollectOverBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("CollectOverBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣����������·��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsExceptionOccurFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("ExceptionOccurFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣��������������ʱ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsExceptionOccurBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("ExceptionOccurBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣��������������ʱ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsExceptionOccurBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("ExceptionOccurBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣����������·��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsOperationMissFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("OperationMissFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣��������������ʱ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsOperationMissBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �쳣��������������ʱ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsOperationMissBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �Զ���������������·��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsDemotionOccurFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("OperationMissFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �Զ�������������������ʱ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsDemotionOccurBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerTime"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Զ�������������������ʱ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::IsDemotionOccurBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerInterval"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ��ģʽ�����ȡҵ��ģʽ��Ӧ����������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguagePN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		int isecond=szWorkLanguage.Find(_T(":"),ifirst+1);
		if(isecond!=-1)
		{
			szRet=szWorkLanguage.Mid(isecond+1);
		}else
		{
		}
	}else
	{
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ��ģʽ�����ȡҵ��ģʽ��Ӧ����������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguageCN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		szRet=szWorkLanguage.Mid(0,ifirst);
	}else
	{
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ��ģʽ�����ȡҵ��ģʽ��Ӧ��Ӣ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguageEN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		/*int isecond=szWorkLanguage.Find(_T(":"),ifirst+1);
		if(isecond!=-1)
		{
			szRet=szWorkLanguage.Mid(ifirst+1,isecond-ifirst-1);
		}else
		{
		}*/
		szRet = szWorkLanguage.Mid(ifirst + 1,szWorkLanguage.GetLength() - 1);
	}else
	{
	}
	return szRet;
}

void CFunctionInfo::getLedContentFormat(CStringList& strList)
{
	for(int i=0;i<10;i++)
	{
		CString szKey(_T(""));
		szKey.Format(_T("%d"),i);
		CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY"),szKey);
		strList.AddTail(szWorkLanguage);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ��ǰ��Ʊָ��Ʊ��  

@param      none

@retval     (o)int
            0:��ָ����
			1��Ʊ��A��
			2��Ʊ��B

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetAvailableMagazineBox(){
	return m_functionIni->GetDataInt(_T("STATUS_ISSUE_SMARTCARD"),_T("ISSUE_FROM_MAGAZINE"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�˿�ҵ������Ĭ��ҵ���쳣�������  

@param      none

@retval     (o)int
            0:������쳣��
			>0:����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetTimesRepareException(){
	return m_functionIni->GetDataInt(_T("REPARE_EXCEPTION_TIMES"),_T("REPARE_TIMES"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ������־��������

@param      WORD Key

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetOperationLogLevelContent(WORD Key,int index)
{
	CStringArray keyvaluearray;		// ��':'�ָ���ַ������array
	CString key= _T("");
	key.Format(_T("%04x"),Key);		// ��ת��
	CString operationlogcontent=m_functionIni->GetDataString(_T("OPERATION_LOG_LEVEL_CONTENT"),key);		// ��ȡ��ֵ
	
	if(operationlogcontent.IsEmpty()){
		return _T("");
	}
	int find_index = operationlogcontent.ReverseFind(':');			// �������':'
	keyvaluearray.Add(operationlogcontent.Mid(find_index+1));
	while(find_index>0)
	{
		
		operationlogcontent=operationlogcontent.Mid(0,find_index);
		find_index=operationlogcontent.ReverseFind(':');
		keyvaluearray.Add(operationlogcontent.Mid(find_index+1));
	}
	int size = keyvaluearray.GetSize();
	
	return keyvaluearray.GetAt(index);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��վ��ȫ�����Ʊ��

@param      ��

@retval     int nMaxAmount;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetTicketMaxAmount() const{
	int nMaxAmount = 0;
	nMaxAmount = m_functionIni->GetDataInt(_T("TICKET_MAX_AMOUNT"),_T("TICKET_LIMITS_AMOUNT"),500);

	if(nMaxAmount == 0){
		nMaxAmount = 500;
	}

	return nMaxAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ñ�վ��ȫ�����Ʊ��

@param      int nMaxAmount;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CFunctionInfo::SetTicketMaxAmount(int nMaxAmount) const {
	CString strMaxAmount;
	strMaxAmount.Format(_T("%d"),nMaxAmount);

	m_functionIni->WriteData(_T("TICKET_MAX_AMOUNT"),_T("TICKET_LIMITS_AMOUNT"),strMaxAmount);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ��ģʽ�����ȡҵ��ģʽ��Ӧ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguage(WORD wdModeCode,int languageId)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%d"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		if(languageId == 1){
			szRet = szWorkLanguage.Mid(0,ifirst);
		}
		else{
			szRet = szWorkLanguage.Mid(ifirst + 1,szWorkLanguage.GetLength() - 1);
		}
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ��ģʽ�����ȡҵ��ģʽ��Ӧ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguage(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%d"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),szWorkCode);
	return szWorkLanguage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡled��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getLedWorkMode(){
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),_T("SHOW_MODE"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ״̬��ʾ����������ģʽ

@param      ��

@retval     Cstring

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getCNWorkMode()
{
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_MODE"),_T("CN"),_T("FFF0425"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ״̬��ʾ��Ӣ��������ģʽ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getENWorkMode()
{
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_MODE"),_T("EN"),_T("FFF0755"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ����

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiHost()
{
	return m_WebApiHost;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int     CFunctionInfo::GetWebApiPort()
{
	return m_WebApiPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ����

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiPwd()
//{
//	return m_WebApiPwd;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ��¼��ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiLoginUrl()
{
	return m_WebApiLoginUrl;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ��ѯ��ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiLockUrl()
//{
//	return m_WebApiLockUrl;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ������ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiLockUrl()
{
	return m_WebApiLockUrl;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API �ύ��ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiConfirmUrl()
//{
//	return m_WebApiConfirmUrl;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ȡƱ���ȡ�����ŵ�ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiGetOrderUrl()
//{
//	return m_WebApiGetOrderUrl;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ WEB_API ��ѯTVM״̬��ַ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiTVMStateUrl()
//{
//	return m_WebApiTVMStateUrl;
//}
