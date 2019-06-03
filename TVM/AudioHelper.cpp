#include "StdAfx.h"
#include "IssueSvc.h"
#include "ChargeSvc.h"
#include "AudioHelper.h"
#include "WavPlayer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TTSCONFIG_PATH	_T("\\res\\tts\\TTS_CONFIG.xml")
#define TTSWAV_FOLDER	_T("\\res\\tts\\")

CEmptyAudioHelper CEmptyAudioHelper::thEmptyAudioHelper;

CEmptyAudioHelper& CEmptyAudioHelper::GetInstance()
{
	return thEmptyAudioHelper;
}

UINT g_uiDuration=0;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAudioHelper::CAudioHelper(void)
{
	CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	CString szXmlPath(_T(""));
	bool bget=GetAppPath(szXmlPath);
	if(bget)
	{
		m_strCurDirectory = szXmlPath;
		szXmlPath+=TTSCONFIG_PATH;
		m_loadXml(szXmlPath);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAudioHelper::~CAudioHelper(void)
{
	CoUninitialize();
}

void CAudioHelper::playEventAudio(DWORD dwEventID,UINT& uiDuration)
{
	stopWave();
	clearWaves();
	std::list <CString > listWavFiles;
	DWORD dwcoun1 = GetTickCount();
	if(AUDIO_COMMON_PLEASE_SELECT_LANGUAGE == dwEventID)
	{
		m_getEventWavFiles(listWavFiles, dwEventID, LANGUAGE_ID_CHINESE);
		m_getEventWavFiles(listWavFiles, dwEventID, LANGUAGE_ID_CANTONESE);
		m_getEventWavFiles(listWavFiles, dwEventID, LANGUAGE_ID_ENGLISH);
		m_getEventWavFiles(listWavFiles, dwEventID, LANGUAGE_ID_PORTUGUESE);
	}
	else{
		m_getEventWavFiles(listWavFiles, dwEventID, theAPP_SESSION.GetCurrentBlindPlayVoiceLanguageType());
	}	

	std::list<CString >::iterator itPath;
	_TCHAR tcpath[MAX_PATH];

	for (itPath = listWavFiles.begin(); itPath != listWavFiles.end(); itPath++)
	{
		//_tcscpy_s(tcpath, MAX_PATH, _T("D:\\"));
		_tcscpy_s(tcpath,MAX_PATH,m_strCurDirectory);
		_tcscat_s(tcpath,TTSWAV_FOLDER);
		_tcscat_s(tcpath, *itPath);
		addWave(tcpath);
	}
	playWave();
	uiDuration=detectDuration();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      停止播放当前语音

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAudioHelper::StopPlayEventAudio(){
	stopWave();
	clearWaves();
}

#define UD_EVENTTAG		_T("event")
#define UD_VARIABLETAG	_T("variable")
#define UD_DEGRADE		_T("degradedVoice")
#define UD_VOICETAG		_T("voice")
#define UD_KEYBOARD		_T("keyboard")
#define UD_KEYID		_T("id")
#define UD_EVENTID		_T("id")
#define UD_KEYVALUE		_T("value")
#define UD_VARIABLETYPE _T("type")
#define UD_VARIABLENAME	_T("name")
#define UD_CODENAME		_T("code")
#define UD_LANTYPE		_T("languageId")
#define UD_FILETAG		_T("file")
#define UD_CH			_T("zh-cn")
#define UD_HK			_T("zh-hk")
#define UD_EN			_T("en-us")
#define UD_PT			_T("pt-pt")

void CAudioHelper::m_loadXml(const TCHAR* tcpath)
{
	_tcscpy_s(m_tcXmlPath, MAX_PATH, tcpath);
	if (PathFileExists(m_tcXmlPath))
	{
		HRESULT hr=m_pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if (SUCCEEDED(hr))
		{
			VARIANT_BOOL vbLoad = m_pDoc->load(m_tcXmlPath);
			if (vbLoad == VARIANT_TRUE)
			{
				_bstr_t bsEvents(_T(""));
				m_pEvents = m_pDoc->getElementsByTagName(UD_EVENTTAG);
			}
		}
	}
}

void CAudioHelper::getKeyMap(std::map<int, DWORD>& mapKeyValue, DWORD dwEventID)
{
	if (NULL != m_pDoc && NULL != m_pEvents)
	{
		long lElementNum = m_pEvents->length;
		for (long lLoop = 0; lLoop < lElementNum; lLoop++)
		{
			MSXML2::IXMLDOMNodePtr pEventNode = m_pEvents->nextNode();
			if (NULL!=pEventNode)
			{
				MSXML2::IXMLDOMElementPtr pElementEvent=(MSXML2::IXMLDOMElementPtr)pEventNode;
				_variant_t vtEventID=pElementEvent->getAttribute((_bstr_t)UD_EVENTID);
				DWORD l_dwEventID=_ttol(vtEventID.bstrVal);
				if(dwEventID==l_dwEventID)
				{
					MSXML2::IXMLDOMNodeListPtr pEventChildren= pEventNode->childNodes;
					if (pEventChildren->length!=0)
					{
						long lEventChildNum = pEventChildren->length;
						for (long lloopEventChildren = 0; lloopEventChildren <lEventChildNum ; lloopEventChildren++)
						{
							MSXML2::IXMLDOMNodePtr pEventChild = pEventChildren->nextNode();
							if (NULL!=pEventChild)
							{
								if (pEventChild->baseName == _bstr_t(UD_KEYBOARD))
								{
									MSXML2::IXMLDOMNodeListPtr pKeyMaps= pEventChild->childNodes;
									long lKeyBoardNum = pKeyMaps->length;
									for (long lloopKeyMap = 0; lloopKeyMap < lKeyBoardNum; lloopKeyMap++)
									{
										MSXML2::IXMLDOMNodePtr pKeyMap = pKeyMaps->nextNode();
										if (NULL != pKeyMap)
										{
											MSXML2::IXMLDOMNamedNodeMapPtr pAttributes = pKeyMap->Getattributes();
											if (NULL != pAttributes)
											{
												int iId = 0;
												DWORD dwValue = 0;
												MSXML2::IXMLDOMNodePtr pAttributeID = pAttributes->getNamedItem(_bstr_t(UD_KEYID));
												MSXML2::IXMLDOMNodePtr pAttributeValue = pAttributes->getNamedItem(_bstr_t(UD_KEYVALUE));
												if (NULL != pAttributeValue && NULL != pAttributeID)
												{
													_bstr_t bstrValueValue = pAttributeValue->Gettext();
													dwValue = _ttol(bstrValueValue);
													_bstr_t bstrIDValue = pAttributeID->Gettext();
													iId = _ttoi(bstrIDValue);

													typedef std::pair<int, DWORD> Key_pari;
													mapKeyValue.insert(Key_pari(iId,dwValue));
												}
											}
										}
									}
									break;
								}
							}
						}
					}
					break;
				}else
				{
				}
			}
		}
		m_pEvents->reset();
	}
}
//確認交易咨詢
#define VARIABLE_PRODUCTTYPE		_T("ProductType")
#define VARIABLE_ORIGINZONE			_T("OriginZone")
#define VARIABLE_DESTZONE			_T("DestinationZone")
#define VARIABLE_ORIGINSTATION		_T("OriginStation")
#define VARIABLE_DESTSTATION		_T("DestinationStation")
#define VARIABLE_UNITAMOUNTMOP		_T("UnitAmountMop")
#define VARIABLE_UNITAMOUNTAVOS		_T("UnitAmountAvos")
#define VARIABLE_RECHARGEAMOUNTMOP	_T("RechargeAmountMop")
#define VARIABLE_RECHARGEAMOUNTAVOS	_T("RechargeAmountAvos")
#define VARIABLE_DEPOSITMOP			_T("DepositMop")
#define VARIABLE_DEPOSITAVOS		_T("DepositAvos")
#define VARIABLE_TAXMOP				_T("TaxMop")
#define VARIABLE_TAXAVOS			_T("TaxAvos")
#define VARIABLE_SUMMOP				_T("SumMop")
#define VARIABLE_SUMAVOS			_T("SumAvos")
//確認支付信息
#define VARIABLE_FAREMOP			_T("FareMop")
#define VARIABLE_FAREAVOS			_T("FareAvos")
#define VARIABLE_PAYMOP				_T("PayMop")
#define VARIABLE_PAYAVOS			_T("PayAvos")
#define VARIABLE_CHANGEMOP			_T("ChangeMop")
#define VARIABLE_CHANGEAVOS			_T("ChangeAvos")

#define VARIABLE_PASSENGERTYPE		_T("PassengerType")			// 乘客类型
#define VARIABLE_QUANTITY			_T("Quantity")				// 张数
//投入现金数量超限
#define VARIABLE_CASH_INSERTED_MAX_MOP   _T("CashInvestedQuantityMop")	//MOP
#define VARIABLE_CASH_INSERTED_MAX_AVOS  _T("CashInvestedQuantityAvos")	//AVOS
//找零数量超限
#define VARIABLE_COIN_CHANGE_MAX_MOP	_T("CoinInvestedAccountMop")	//MOP
#define VARIABLE_COIN_CHANGE_MAX_AVOS	_T("CoinInvestedAccountAvos")	//AVOS
//找零不足
#define VARIABLE_COIN_NOT_ENOUGH_MOP	_T("CoinInvestedQuantityMop")	//MOP
#define VARIABLE_COIN_NOT_ENOUGH_AVOS	_T("CoinInvestedQuantityAvos")	//AVOS

CString getLanType(LANGUAGETYPE_T lantype)
{
	CString szret(_T(""));
	switch (lantype)
	{
	case LANGUAGE_ID_CHINESE:
		szret = UD_CH;
		break;
	case LANGUAGE_ID_ENGLISH:
		szret = UD_EN;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		szret = UD_PT;
		break;
	case LANGUAGE_ID_CANTONESE:
		szret = UD_HK;
		break;
	default:
		break;
	}
	return szret;
}

void CAudioHelper::m_getLanTypeWavFiles(MSXML2::IXMLDOMNodeListPtr& pVoiceChildren,LANGUAGETYPE_T lantype,std::list<CString>& listWavFiles)
{
	long lVoiceChildNum = pVoiceChildren->length;
	for (long lloopSubVoice = 0; lloopSubVoice < lVoiceChildNum; lloopSubVoice++)
	{
		MSXML2::IXMLDOMElementPtr pVoiceChildElement = pVoiceChildren->nextNode();
		if (NULL != pVoiceChildElement)
		{
			_bstr_t bsVoiceChildBaseName = pVoiceChildElement->baseName;
			if (bsVoiceChildBaseName == _bstr_t(UD_VOICETAG))
			{
				//是Voice元素
				CString szFilePath(pVoiceChildElement->getAttribute(_bstr_t(UD_FILETAG)));
				listWavFiles.push_back(szFilePath);
			}
			else if (bsVoiceChildBaseName == _bstr_t(UD_VARIABLETAG))
			{
				//不是Voice元素 Variable元素
				CString szVariableType(pVoiceChildElement->getAttribute(_bstr_t(UD_VARIABLETYPE)));
				CString szVariableName(pVoiceChildElement->getAttribute(_bstr_t(UD_VARIABLENAME)));

				DWORD dwCode = getCode(szVariableName);
				CString szFilePath(_T(""));
				szFilePath=m_getVariableValue(szVariableType, dwCode, lantype);
				listWavFiles.push_back(szFilePath);
			}else if(bsVoiceChildBaseName == _bstr_t(UD_DEGRADE))
			{
				CString szVariableType(pVoiceChildElement->getAttribute(_bstr_t(UD_VARIABLENAME)));
				BOOL bDegrade=getDegrade(szVariableType);
				//BOOL bDegrade=m_getDegrade(szVariableType);
				if(bDegrade)
				{
					MSXML2::IXMLDOMNodeListPtr pDegradeChildren=pVoiceChildElement->childNodes;
					if(NULL!=pDegradeChildren)
					{
						m_getLanTypeWavFiles(pDegradeChildren,lantype,listWavFiles);
					}
				}
			}
		}
	}
}

void CAudioHelper::m_getEventWavFiles(std::list<CString>& listWavFiles, DWORD dwEventID, LANGUAGETYPE_T lantype)
{
	CString szLanType(_T(""));
	szLanType = getLanType(lantype);
	if (NULL != m_pDoc && NULL != m_pEvents)
	{
		long leventNum = m_pEvents->length;
		for (long lloopEvent = 0; lloopEvent < leventNum;lloopEvent++)
		{
			MSXML2::IXMLDOMNodePtr pEventNode = m_pEvents->nextNode();
			if (NULL != pEventNode)
			{
				MSXML2::IXMLDOMElementPtr pEventElement = pEventNode;
				_TCHAR tcID[33];
				_stprintf_s(tcID, _T("%u"), dwEventID);
				_bstr_t bsId(UD_EVENTID);
				_variant_t vtFindID(tcID);
				_variant_t vtId = pEventElement->getAttribute(bsId);
				if (vtFindID==vtId)
				{
					MSXML2::IXMLDOMNodeListPtr pEventChildren = pEventElement->childNodes;
					//找到對應的Event元素
					long lVoiceElementNum = pEventChildren->length;
					for (long lloopVoice = 0; lloopVoice< lVoiceElementNum; lloopVoice++)
					{
						MSXML2::IXMLDOMNodePtr pEventChild = pEventChildren->nextNode();
						if (NULL != pEventChild)
						{
							_bstr_t bsElementName=pEventChild->baseName;
							if (bsElementName == _bstr_t(UD_VOICETAG))
							{
								MSXML2::IXMLDOMElementPtr pEventChildElement = pEventChild;
								_variant_t vtLangType=pEventChildElement->getAttribute(_bstr_t(UD_LANTYPE));
								CString szFindLang(vtLangType);
								if (szFindLang == szLanType)
								{
									//找到正確語種的voice元素
									MSXML2::IXMLDOMNodeListPtr pVoiceChildren = pEventChildElement->childNodes;
									if (pVoiceChildren->length!=0)
									{
										//递归调用
										m_getLanTypeWavFiles(pVoiceChildren,lantype,listWavFiles);
										lloopVoice = lVoiceElementNum;
										lloopEvent = leventNum;
									}
									else
									{
										//無孩子節點的情況
										//不存在孩子節點的情況 直接讀取file
										MSXML2::IXMLDOMElementPtr pVoiceElement = pEventChild;
										_variant_t vtfilepath = pVoiceElement->getAttribute(_bstr_t(UD_FILETAG));
										CString szFilePath(vtfilepath);
										listWavFiles.push_back(szFilePath);
										lloopVoice = lVoiceElementNum;
										lloopEvent = leventNum;
									}
								}
								else
								{
									//
								}
							}
						}
					}
				}
			}
		}
		m_pEvents->reset();
	}
}

CString CAudioHelper::m_getVariableValue(const TCHAR* tcVariableType,DWORD dwCode, LANGUAGETYPE_T lantype)
{
	CString szPath(_T(""));
	_variant_t vtLanType(getLanType(lantype));
	_TCHAR tcCode[33];
	_stprintf_s(tcCode, _T("%u"), dwCode);
	//_variant_t vtCode(tcCode);
	_variant_t vtCode(dwCode);
	if (NULL!=m_pDoc)
	{
		MSXML2::IXMLDOMElementPtr pRootElement = m_pDoc->documentElement;
		if (NULL!=pRootElement)
		{
			MSXML2::IXMLDOMNodeListPtr pVariables = pRootElement->childNodes;
			long lvariableNum = pVariables->length;
			for (long lloopVariable = 0; lloopVariable < lvariableNum; lloopVariable++)
			{
				MSXML2::IXMLDOMElementPtr pVariableElement = pVariables->nextNode();
				if (NULL != pVariableElement)
				{
					if (pVariableElement->baseName == _bstr_t(UD_VARIABLETAG))
					{
						_variant_t vtTypeName = pVariableElement->getAttribute(_bstr_t(UD_VARIABLETYPE));
						if (_variant_t(tcVariableType) == vtTypeName)
						{
							//找到對應的變量節點
							MSXML2::IXMLDOMNodeListPtr pVariableChildren = pVariableElement->childNodes;
							long lChildrenNum = pVariableChildren->length;
							for (long lloopVariableChildren = 0; lloopVariableChildren < lChildrenNum; lloopVariableChildren++)
							{
								MSXML2::IXMLDOMElementPtr pVariableElement = pVariableChildren->nextNode();
								_variant_t vtVariableCode = pVariableElement->getAttribute(_bstr_t(UD_CODENAME));

								//char* pStr = _com_util::ConvertBSTRToString(vtVariableCode.bstrVal);
								CString strTemp = vtVariableCode.bstrVal;
								DWORD dwGetCode = _ttol(vtVariableCode.vt == VT_BSTR ? vtVariableCode.bstrVal : 0);
								if (dwGetCode == dwCode)
								{
									_variant_t vtLanTypeValue = pVariableElement->getAttribute(_bstr_t(UD_LANTYPE));
									if (vtLanTypeValue == vtLanType)
									{
										_variant_t vtWavPath = pVariableElement->getAttribute(_bstr_t(UD_FILETAG));
										szPath = vtWavPath;
										lloopVariableChildren = lChildrenNum;
										lloopVariable = lvariableNum;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return szPath;
}

DWORD	CAudioHelper::getCode(const _TCHAR* tcVariableType)
{
	DWORD dwret=0;
	return dwret;
}

#define UD_DEGRADE_PRINT _T("Print")
#define UD_DEGRADE_COINPAYMENT _T("CoinPaymentMethod")
#define UD_DEGRADE_COIN5PAYMENT _T("Coin5MOPPaymentMethod")
#define UD_DEGRADE_COIN2PAYMENT _T("Coin2MOPPaymentMethod")
#define UD_DEGRADE_COIN1PAYMENT _T("Coin1MOPPaymentMethod")
#define UD_DEGRADE_COINHALFPAYMENT _T("Coin50AVOSPaymentMethod")
#define UD_DEGRADE_NOTEPAYMENT _T("NotePaymentMethod")
#define UD_DEGRADE_NOTE10PAYMENT _T("Note10MOPPaymentMethod")
#define UD_DEGRADE_NOTE20PAYMENT _T("Note20MOPPaymentMethod")
#define UD_DEGRADE_NOTE50PAYMENT _T("Note50MOPPaymentMethod")
#define UD_DEGRADE_NOTE100PAYMENT _T("Note100MOPPaymentMethod")
#define UD_DEGRADE_NOTE500PAYMENT _T("Note500MOPPaymentMethod")
#define UD_DEGRADE_NOTE1000PAYMENT _T("Note1000MOPPaymentMethod")
#define UD_DEGRADE_CHANGE _T("Change")

BOOL CAudioHelper::getDegrade(const _TCHAR* tcDegradeVriableName)
{
	BOOL bret=TRUE;
	if(!_tcsicmp(UD_DEGRADE_PRINT,tcDegradeVriableName))
	{
		//取是否支持打印
		bret = theTVM_STATUS_MGR.GetPrintMode() != 0;
	}else if(!_tcsicmp(UD_DEGRADE_COINPAYMENT,tcDegradeVriableName))
	{
		//是否支持硬币支付
		bret = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_AVM ? false : (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN;
	}else if(!_tcsicmp(UD_DEGRADE_NOTEPAYMENT,tcDegradeVriableName))
	{
		//是否支持纸币支付
		bret = (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
	}else if(!_tcsicmp(UD_DEGRADE_CHANGE,tcDegradeVriableName))
	{
		//是否支持找零
		bret = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_AVM ? false : (theTVM_STATUS_MGR.GetChangeMode() & DEVICE_CONTRL_CODE_NO_BILL_CHANGE) != DEVICE_CONTRL_CODE_NO_BILL_CHANGE;
	}
	return bret;
}

CIssueAudioHelper::CIssueAudioHelper(CService& service):CServiceHelper(service)
{

}

CIssueAudioHelper::~CIssueAudioHelper()
{

}

#define VARIABLE_PURCHASEDPERIOD	_T("PurchasedPeriod")
#define VARIABLE_PURCHASEDRIDES		_T("PurchasedRides")

DWORD CIssueAudioHelper::getCode(const _TCHAR* tcVariableType)
{
	DWORD dwret=0;
	CIssueSvc* pIssueSvc=dynamic_cast<CIssueSvc*>(&this->m_service);
	if(NULL!=pIssueSvc)
	{
		CIssueSvc::Model issueDataModel = pIssueSvc->GetDataModel();
		CString strProductID;
		strProductID.Format(_T("%05d%05d"),issueDataModel.productInfo.productType,issueDataModel.productInfo.productSubType);
		if (_tcsicmp(tcVariableType, VARIABLE_PRODUCTTYPE) == 0)
		{//产品类型
			dwret = issueDataModel.productInfo.productType;
		}
		// 乘客类型
		else if(_tcsicmp(tcVariableType,VARIABLE_PASSENGERTYPE) == 0){
			dwret = issueDataModel.mediumInfo.passengerType;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_ORIGINZONE) == 0)
		{//起始区域
			dwret = issueDataModel.productInfo.beginLocation;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_DESTZONE) == 0)
		{//目标区域
			dwret = issueDataModel.productInfo.endLocation;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_ORIGINSTATION) == 0)
		{//起始站点
			dwret = issueDataModel.productInfo.beginLocation;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_DESTSTATION) == 0)
		{//目标站点
			dwret = issueDataModel.productInfo.endLocation;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_UNITAMOUNTMOP) == 0)
		{//产品单价 元
			dwret = issueDataModel.productInfo.lProductSaleAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_UNITAMOUNTAVOS) == 0)
		{//产品单价 分
			dwret = issueDataModel.productInfo.lProductSaleAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_RECHARGEAMOUNTMOP) == 0)
		{//找零元
			dwret = issueDataModel.paymentInfo.lChangeAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_RECHARGEAMOUNTAVOS) == 0)
		{//找零分
			dwret = issueDataModel.paymentInfo.lChangeAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_DEPOSITMOP) == 0)
		{//押金元
			dwret = issueDataModel.productInfo.lProductDeposit/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_DEPOSITAVOS) == 0)
		{//押金分
			dwret = issueDataModel.productInfo.lProductDeposit%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_TAXMOP) == 0)
		{//税费元
			dwret = issueDataModel.paymentInfo.lTaxAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_TAXAVOS) == 0)
		{//税费分
			dwret = issueDataModel.paymentInfo.lTaxAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_SUMMOP) == 0)
		{//总额元
			dwret = issueDataModel.paymentInfo.lPriceAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_SUMAVOS) == 0)
		{//总额分
			dwret = issueDataModel.paymentInfo.lPriceAmount%100;
		}
		// 产品数量
		else if(_tcsicmp(tcVariableType,VARIABLE_QUANTITY) == 0){
			dwret = issueDataModel.productInfo.nProductPiece;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_FAREMOP) == 0)
		{//税前金额 元
			dwret = issueDataModel.paymentInfo.lPriceAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_FAREAVOS) == 0)
		{//税前金额 分
			dwret = issueDataModel.paymentInfo.lPriceAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_PAYMOP) == 0)
		{//支付金额 元
			dwret = issueDataModel.paymentInfo.lPaidAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_PAYAVOS) == 0)
		{//支付金额 分
			dwret = issueDataModel.paymentInfo.lPaidAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_CHANGEMOP) == 0)
		{//找零金额 元
			dwret = issueDataModel.paymentInfo.lChangeAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_CHANGEAVOS) == 0)
		{//找零金额 分
			dwret = issueDataModel.paymentInfo.lChangeAmount%100;
		}
		// 定期票30天
		else if(!_tcsicmp(tcVariableType,VARIABLE_PURCHASEDPERIOD)){
			dwret = _ttoi(strProductID);
		}
		// 计次票30次
		else if(!_tcsicmp(tcVariableType,VARIABLE_PURCHASEDRIDES)){
			dwret = _ttoi(strProductID);
		}
		// 投入超限退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_CASH_INSERTED_MAX_MOP) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount/100;
		}
		// 投入超限退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_CASH_INSERTED_MAX_AVOS) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount%100;
		}
		// 找零超限退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_CHANGE_MAX_MOP) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount/100;
		}
		// 找零超限退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_CHANGE_MAX_AVOS) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount%100;
		}
		// 找零不足退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_NOT_ENOUGH_MOP) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount/100;
		}
		// 找零不足退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_NOT_ENOUGH_AVOS) == 0){
			dwret = issueDataModel.paymentInfo.lReturnAmount%100;
		}
	}else
	{
	}
	return dwret;
}

BOOL CIssueAudioHelper::getDegrade(const _TCHAR* tcDegradeVriableName)
{
	//BOOL bret=__super::getDegrade(tcDegradeVriableName);

	//CIssueSvc* pIssueSvc=dynamic_cast<CIssueSvc*>(&this->m_service);
	//if(NULL!=pIssueSvc)
	//{
	//	COIN_ACCEPTABLE acceptCoinTable=pIssueSvc->GetAcceptCoinFaceTable();
	//	BH_FACEVALUE_TABLE acceptBanknoteTable=pIssueSvc->GetAcceptBanknotesFace();
	//	bool bIsSupportCoin = (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN;
	//	bool bIsSupportBanknote = (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
	//
	//	if(!_tcsicmp(UD_DEGRADE_COIN5PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持5硬币支付
	//		bret = acceptCoinTable.CH_COIN_ALLOW_5 && bIsSupportCoin;
	//	}else if(!_tcsicmp(UD_DEGRADE_COIN2PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持2硬币支付
	//		bret = acceptCoinTable.CH_COIN_ALLOW_2 && bIsSupportCoin;
	//	}else if(!_tcsicmp(UD_DEGRADE_COIN1PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持1硬币支付
	//		bret = acceptCoinTable.CH_COIN_ALLOW_1 && bIsSupportCoin;
	//	}else if(!_tcsicmp(UD_DEGRADE_COINHALFPAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持0.5硬币支付
	//		bret = acceptCoinTable.CH_COIN_ALLOW_05 && bIsSupportCoin;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE10PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持10纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote10MOP && bIsSupportBanknote;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE20PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持20纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote20MOP && bIsSupportBanknote;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE50PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持50纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote50MOP && bIsSupportBanknote;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE100PAYMENT,tcDegradeVriableName)){
	//		//是否支持100纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote100MOP && bIsSupportBanknote;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE500PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持500纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote500MOP && bIsSupportBanknote;
	//	}else if(!_tcsicmp(UD_DEGRADE_NOTE1000PAYMENT,tcDegradeVriableName))
	//	{
	//		//是否支持1000纸币支付
	//		bret = acceptBanknoteTable.bIsBanknote1000MOP && bIsSupportBanknote;
	//	}
	//	else if(!_tcsicmp(UD_DEGRADE_PRINT,tcDegradeVriableName)){
	//		bret = theAFC_TVM_OPERATION.SupportPrintSwitch() && theTVM_STATUS_MGR.GetPrintMode() == 0;
	//	}
	//	// 是否支持纸币支付？
	//	else if(!_tcsicmp(UD_DEGRADE_NOTEPAYMENT,tcDegradeVriableName)){
	//		bret = bIsSupportBanknote;
	//	}
	//	// 是否支持硬币支付？
	//	else if(!_tcsicmp(UD_DEGRADE_COINPAYMENT,tcDegradeVriableName)){
	//		bret = bIsSupportCoin;
	//	}
	//}
	return true;//bret;
}

CChargeAudioHelper::CChargeAudioHelper(CService& service):CServiceHelper(service)
{

}

CChargeAudioHelper::~CChargeAudioHelper()
{

}
// 充值
//SVT
#define VARIABLE_RECHARGE_BEFORE_MOP	_T("RechargeBeforeMop")
#define VARIABLE_RECHARGE_BEFORE_AVOS	_T("RechargeBeforeAvos")
#define VARIABLE_RECHARGE_MONEY_MOP		_T("RechargeMoneyMop")
#define VARIABLE_RECHARGE_MONEY_AVOS	_T("RechargeMoneyAvos")
#define VARIABLE_RECHARGE_AFTER_MOP		_T("RechargeAfterMop")
#define VARIABLE_RECHARGE_AFTER_AVOS	_T("RechargeAfterAvos")
// MST
#define VARIABLE_RECHARGE_BEFOR_TIMES	_T("RechargeBeforeTimes")
#define VARIABLE_RECHARGE_TIMES			_T("RechargeTimes")
#define VARIABLE_RECHARGE_AFTER_TIMES	_T("RechargeAfterTimes")
#define VARIABLE_RECHARGE_UNIT_MOP		_T("UnitPriceMop")
#define VARIABLE_RECHARGE_UNIT_AVOS		_T("UnitPriceAvos")
// PST
#define VARIABLE_RECHARGE_PERIODAY		_T("RechargePeriodDay")

// ----------------------降级--------------------------------------------------
// 定期
//#define DEGRADE_PERIOD_SEASON_TICKET_30	_T("PeriodSeasonTicket30Days")
//#define DEGRADE_PERIOD_SEASON_TICKET_60	_T("PeriodSeasonTicket60Days")
//#define DEGRADE_PERIOD_SEASON_TICKET_90	_T("PeriodSeasonTicket90Days")
//// 计次
//#define DEGRADE_MULTI_SEASON_TICKET_30	_T("Multi-rideSeasonTicket30Rides")
//#define DEGRADE_MULTI_SEASON_TICKET_60	_T("Multi-rideSeasonTicket60Rides")
//#define DEGRADE_MULTI_SEASON_TICKET_90	_T("Multi-rideSeasonTicket90Rides")
// 选择业务
#define DEGRADE_STORED_VALUE_TICKET		_T("StoredValueTicket")
#define DEGRADE_MULTI_SEASON_TICKET		_T("Multi-rideSeasonTicket")
#define DEGRADE_PERIOD_SEASON_TICKET	_T("PeriodSeasonTicket")

DWORD CChargeAudioHelper::getCode(const _TCHAR* tcVariableType)
{
	DWORD dwret=0;
	CChargeSvc* pChargeSvc=dynamic_cast<CChargeSvc*>(&this->m_service);
	if(NULL!=pChargeSvc)
	{
		CChargeSvc::DataModel data=pChargeSvc->GetDataModel();
		CString strProductSubType;
		strProductSubType.Format(_T("%05d%05d"),data.productInfo.productType,data.productInfo.productSubType);
		// 产品类型
		if(_tcsicmp(tcVariableType, VARIABLE_PRODUCTTYPE) == 0){
			dwret = data.productInfo.productType;
		}
		//-------------------------SVT---------------------------------------
		// 充值前金额:元
		else if(_tcsicmp(tcVariableType, VARIABLE_RECHARGE_BEFORE_MOP) == 0){
			dwret = data.oldProductInfo.lBalanceAmount/100;
		}
		// 充值前金额：分
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_BEFORE_AVOS) == 0){
			dwret = data.oldProductInfo.lBalanceAmount%100;
		}
		// 充值金额：元
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_MONEY_MOP) == 0){
			dwret = data.lAddAmount/100;
		}
		// 充值金额：分
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_MONEY_AVOS) == 0){
			dwret = data.lAddAmount%100;
		}
		// 充值后：元
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_AFTER_MOP) == 0){
			dwret = data.productInfo.lBalanceAmount/100;
		}
		// 充值后：分
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_AFTER_AVOS) == 0){
			dwret = data.productInfo.lBalanceAmount%100;
		}
		//--------------------------MST-----------------------------------------
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_BEFOR_TIMES) == 0){// 充值前次数
			dwret = data.oldProductInfo.lBalanceRides;
		}
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_TIMES) == 0){// 充值次数
			dwret = _ttoi(strProductSubType);//data.lAddRides;// 注意：定期票30天 code：0x000a0001！！！
		}
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_AFTER_TIMES) == 0){//充值后次数
			dwret = data.productInfo.lBalanceRides;
		}
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_UNIT_MOP) == 0){//单价：元
			dwret = data.paymentInfo.lDueAmount/100;
		}
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_UNIT_AVOS) == 0){//单价：分
			dwret = data.paymentInfo.lDueAmount%100;
		}
		//--------------------------PST-----------------------------------------
		else if(_tcsicmp(tcVariableType,VARIABLE_RECHARGE_PERIODAY) == 0){
			dwret = _ttoi(strProductSubType);//data.lValidatePeriod;//注意：定期票30天 code：0x000a0001！！！
		}
		// 起始区段
		else if(_tcsicmp(tcVariableType,VARIABLE_ORIGINZONE) == 0){
			dwret = data.productInfo.beginLocation;
		}
		//目标区域
		else if (_tcsicmp(tcVariableType, VARIABLE_DESTZONE) == 0){
			dwret = data.productInfo.endLocation;
		}
		//--------------------------------现金----------------------------------------
		else if (_tcsicmp(tcVariableType, VARIABLE_FAREMOP) == 0){//充值前金额 元
			dwret = data.paymentInfo.lPriceAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_FAREAVOS) == 0){//充值前金额 分
			dwret = data.paymentInfo.lPriceAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_PAYMOP) == 0)
		{//支付金额 元
			dwret = data.paymentInfo.lPaidAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_PAYAVOS) == 0)
		{//支付金额 分
			dwret = data.paymentInfo.lPaidAmount%100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_CHANGEMOP) == 0)
		{//找零金额 元
			dwret = data.paymentInfo.lChangeAmount/100;
		}
		else if (_tcsicmp(tcVariableType, VARIABLE_CHANGEAVOS) == 0)
		{//找零金额 分
			dwret = data.paymentInfo.lChangeAmount%100;
		}
		// 投入超限退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_CASH_INSERTED_MAX_MOP) == 0){
			dwret = data.paymentInfo.lReturnAmount/100;
		}
		// 投入超限退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_CASH_INSERTED_MAX_AVOS) == 0){
			dwret = data.paymentInfo.lReturnAmount%100;
		}
		// 找零超限退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_CHANGE_MAX_MOP) == 0){
			dwret = data.paymentInfo.lReturnAmount/100;
		}
		// 找零超限退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_CHANGE_MAX_AVOS) == 0){
			dwret = data.paymentInfo.lReturnAmount%100;
		}
		// 找零不足退錢:MOP
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_NOT_ENOUGH_MOP) == 0){
			dwret = data.paymentInfo.lReturnAmount/100;
		}
		// 找零不足退錢:AVOS
		else if(_tcsicmp(tcVariableType,VARIABLE_COIN_NOT_ENOUGH_AVOS) == 0){
			dwret = data.paymentInfo.lReturnAmount%100;
		}
	}
	return dwret;
}

BOOL CChargeAudioHelper::getDegrade(const _TCHAR* tcDegradeVriableName)
{
	BOOL bret=__super::getDegrade(tcDegradeVriableName);
	CChargeSvc* pChargeSvc=dynamic_cast<CChargeSvc*>(&this->m_service);
	CChargeSvc::DataModel model = pChargeSvc->GetDataModel();

	if(NULL!=pChargeSvc)
	{
		COIN_ACCEPTABLE acceptCoinTable=pChargeSvc->GetAcceptCoinFaceTable();
		BRH_FACE_ACEPTED_TABLE acceptBanknoteTable=pChargeSvc->GetAcceptBanknotesFace();
		bool bIsSupportCoin = (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN;
		bool bIsSupportBanknote = (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		if(!_tcsicmp(UD_DEGRADE_COIN5PAYMENT,tcDegradeVriableName))
		{
			//是否支持5硬币支付
			//bret = acceptCoinTable.CH_COIN_ALLOW_5 && bIsSupportCoin;
		}else if(!_tcsicmp(UD_DEGRADE_COIN2PAYMENT,tcDegradeVriableName))
		{
			//是否支持2硬币支付
			//bret = acceptCoinTable.CH_COIN_ALLOW_2 && bIsSupportCoin;
		}else if(!_tcsicmp(UD_DEGRADE_COIN1PAYMENT,tcDegradeVriableName))
		{
			//是否支持1硬币支付
			bret = acceptCoinTable.CH_COIN_ALLOW_1 && bIsSupportCoin;
		}else if(!_tcsicmp(UD_DEGRADE_COINHALFPAYMENT,tcDegradeVriableName))
		{
			//是否支持0.5硬币支付
			bret = acceptCoinTable.CH_COIN_ALLOW_05 && bIsSupportCoin;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE10PAYMENT,tcDegradeVriableName))
		{
			//是否支持10纸币支付
			bret = acceptBanknoteTable.bIsBanknote1Yuan && bIsSupportBanknote;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE20PAYMENT,tcDegradeVriableName))
		{
			//是否支持20纸币支付
			bret = acceptBanknoteTable.bIsBanknote5Yuan && bIsSupportBanknote;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE50PAYMENT,tcDegradeVriableName))
		{
			//是否支持50纸币支付
			bret = acceptBanknoteTable.bIsBanknote10Yuan && bIsSupportBanknote;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE100PAYMENT,tcDegradeVriableName))
		{
			//是否支持100纸币支付
			bret = acceptBanknoteTable.bIsBanknote20Yuan && bIsSupportBanknote;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE500PAYMENT,tcDegradeVriableName))
		{
			//是否支持500纸币支付
			bret = acceptBanknoteTable.bIsBanknote50Yuan && bIsSupportBanknote;
		}else if(!_tcsicmp(UD_DEGRADE_NOTE1000PAYMENT,tcDegradeVriableName))
		{
			//是否支持1000纸币支付
			bret = acceptBanknoteTable.bIsBanknote100Yuan && bIsSupportBanknote;
		}
		// ----------------------------------------------------------------------------------------
		// SVT
		else if(!_tcsicmp(DEGRADE_STORED_VALUE_TICKET,tcDegradeVriableName)){
			bret = model.lAvailableRechargeProduct & CChargeSvc::TYPE_SVT;
		}
		// MST
		else if(!_tcsicmp(DEGRADE_MULTI_SEASON_TICKET,tcDegradeVriableName)){
			bret = model.lAvailableRechargeProduct & CChargeSvc::TYPE_MST;
		}
		// PST
		else if(!_tcsicmp(DEGRADE_PERIOD_SEASON_TICKET,tcDegradeVriableName)){
			bret = model.lAvailableRechargeProduct & CChargeSvc::TYPE_PST;
		}
		// 定期30天
		//else if(!_tcsicmp(DEGRADE_PERIOD_SEASON_TICKET_30,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_PST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(PST);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;
		//	}
		//	else
		//		bret = false;
		//}
		//else if(!_tcsicmp(DEGRADE_PERIOD_SEASON_TICKET_60,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_PST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(PST*2);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;				
		//	}
		//	else
		//		bret = false;
		//}
		//else if(!_tcsicmp(DEGRADE_PERIOD_SEASON_TICKET_90,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_PST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(PST*3);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;
		//	}
		//	else
		//		bret = false;
		//}
		//// 计次30次
		//else if(!_tcsicmp(DEGRADE_MULTI_SEASON_TICKET_30,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_MST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(MST);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;
		//	}
		//	else
		//		bret = false;
		//}
		//else if(!_tcsicmp(DEGRADE_MULTI_SEASON_TICKET_60,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_MST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(MST*2);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;
		//	}
		//	else
		//		bret = false;
		//}
		//else if(!_tcsicmp(DEGRADE_MULTI_SEASON_TICKET_90,tcDegradeVriableName)){
		//	if(model.lAvailableRechargeProduct&CChargeSvc::TYPE_MST){
		//		map<unsigned long ,bool>::iterator it =  model.mapAvailableProduct.find(MST*3);
		//		bret = model.mapAvailableProduct.end() != it ? it->second : false;
		//	}
		//	else
		//		bret = false;
		//}
		// 是否支持纸币支付？
		else if(!_tcsicmp(UD_DEGRADE_NOTEPAYMENT,tcDegradeVriableName)){
			bret = bIsSupportBanknote;
		}
		// 是否支持硬币支付？
		else if(!_tcsicmp(UD_DEGRADE_COINPAYMENT,tcDegradeVriableName)){
			bret = bIsSupportCoin;
		}
		else if(!_tcsicmp(UD_DEGRADE_PRINT,tcDegradeVriableName)){
			bret = theAFC_TVM_OPERATION.SupportPrintSwitch() && theTVM_STATUS_MGR.GetPrintMode() == 0;
		}
		// 取充值读子产品是否降级？
		else {
			bret = false;
			//CString strProductSubType;
			//strProductSubType.Format(_T("%05d%05d"),);
			map<CString,bool>::iterator it = model.mapAvailableProduct.begin();
			while(it != model.mapAvailableProduct.end()){
				if(!_tcsicmp(it->first,tcDegradeVriableName)){
					bret = it->second;
					break;
				}
				it++;
			}
		}
	}
	return bret;
}
