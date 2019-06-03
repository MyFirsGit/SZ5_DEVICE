#include "stdafx.h"
#include "TransactionHistorySvc.h"
#include "TransactionHistoryBaseDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTransactionHistoryBaseDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CTransactionHistoryBaseDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//#define UD_SPACE _T("                    ")
#define UD_SPACE _T("         ")
#define UD_HALF_SPACE _T("          ")
#define UD_QUARTER_SPACE _T("     ")
#define UD_MOP (_T("元"))

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTransactionHistoryBaseDlg::CTransactionHistoryBaseDlg(CService* pService)
	: COperationDlg(CTransactionHistoryBaseDlg::IDD,pService)
{
	//m_baseInfo->detailInfo.row = UD_MAX_ROW;           // 行数
	//m_baseInfo->detailInfo.labelGroup = m_labelGroup;
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// 获取设备类型

}

CTransactionHistoryBaseDlg::UE_TRANSTYPE detectTransType(TransactionRecord& transInfo)
{
	CTransactionHistoryBaseDlg::UE_TRANSTYPE ueret=CTransactionHistoryBaseDlg::UE_UNKNOWNTRANS;
	switch(transInfo.iTransactionDetail.iUdType)
	{
	case 1:
		ueret=CTransactionHistoryBaseDlg::UE_ISSUETOKE;
		break;
	case 2:
		ueret=CTransactionHistoryBaseDlg::UE_ISSUECARD;
		break;
	case 3:
		ueret=CTransactionHistoryBaseDlg::UE_ADDPRODUCT;
		break;
	case 4:
		ueret=CTransactionHistoryBaseDlg::UE_RECHARGE;
		break;
	default:
		break;
	}
	return ueret;
	
	if(transInfo.iTransactionDetail.iUdType==1)
	{
		ueret=CTransactionHistoryBaseDlg::UE_ISSUECARD;
	}else
	{
		if(transInfo.iTransactionDetail.iUdType==3)
		{
			if(transInfo.iTransactionDetail.iUdSubtype==10 ||
				transInfo.iTransactionDetail.iUdSubtype==11 ||
				transInfo.iTransactionDetail.iUdSubtype==12)
			{
				//目前已知的三種充值業務
				ueret=CTransactionHistoryBaseDlg::UE_RECHARGE;
			}else
			{
				//剩下的就是售票業務
				ueret=CTransactionHistoryBaseDlg::UE_ISSUECARD;
			}
		}else
		{
			ueret=CTransactionHistoryBaseDlg::UE_UNKNOWNTRANS;
		}
	}
	return ueret;
}

void CTransactionHistoryBaseDlg::showDetail()
{
	CTransactionHistorySvc* pSvc = (CTransactionHistorySvc*)GetService();
	if(NULL!=pSvc)
	{
		CTransactionHistorySvc::Model& model = pSvc->GetDataModel();
		// 交易数据
		TransactionRecord* ptransInfo=model.ptransInfo;
		if(NULL!=ptransInfo)
		{
			UE_TRANSTYPE ueTransType=detectTransType(*ptransInfo);
			m_showDetail(model,ueTransType);

			if(pSvc->GetCurrentPage()==pSvc->GetTotalPage())
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_F5_ESC);
			}else if(pSvc->GetCurrentPage()==1)
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_F6_ESC_REPARE);
			}else
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_F5_F6_ESC);
			}
		}else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TRANSDETAIL_NOT_QUERY));
		}
	}else
	{
	}
}

long getTypeNum(TransactionRecord& transInfo,BankNoteAndCoinType_t valueType,CTransactionHistoryBaseDlg::UE_OPERATE_TYPE ueOperateType=CTransactionHistoryBaseDlg::UE_ACCEPTED)
{
	long lret=0;
	switch(valueType)
	{
	case CoinHalfYuan:
	case Coin1Yuan:
	/*case Coin5MOP:
	case Coin50AVOS:*/
		switch(ueOperateType)
		{
		case CTransactionHistoryBaseDlg::UE_ACCEPTED:
			{
				vector<CoinAcceptRecord>::iterator iter;
				for(iter=transInfo.vTransCoinAcceptDetail.begin();iter!=transInfo.vTransCoinAcceptDetail.end();iter++)
				{
					if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
					{
						if(iter->iCoinTypeCode==valueType)
						{
							lret+=iter->iQuantity;
						}
					}
				}
			}
			break;
		case CTransactionHistoryBaseDlg::UE_INSERT:
			{
				vector<CoinInsertRecord>::iterator iter;
				for(iter=transInfo.vTransCoinDetail.begin();iter!=transInfo.vTransCoinDetail.end();iter++)
				{
					if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
					{
						if(iter->iCoinTypeCode==valueType)
						{
							lret+=iter->iQuantity;
						}
					}
				}
			}
			break;
		case CTransactionHistoryBaseDlg::UE_CHANGE:
			{
				vector<CoinChangeRecord>::iterator iter;
				for(iter=transInfo.vTransCoinChangeDetail.begin();iter!=transInfo.vTransCoinChangeDetail.end();iter++)
				{
					if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
					{
						if(iter->iCoinTypeCode==valueType)
						{
							lret+=iter->iQuantity;
						}
					}
				}
			}
			break;
		default:
			break;
		}
		break;
	case Banknote1Yuan:
	case Banknote5Yuan:
	case Banknote10Yuan:
	case Banknote20Yuan:
	case Banknote50Yuan:
	case Banknote100Yuan:
		switch(ueOperateType)
		{
		case CTransactionHistoryBaseDlg::UE_ACCEPTED:
			{
				// 交易取消，读取纸币接收信息
				if(transInfo.iTransactionDetail.iState == 2){
					vector<BanknoteInsertRecord>::iterator iter;
					for(iter=transInfo.vTransBanknoteDetail.begin();iter!=transInfo.vTransBanknoteDetail.end();iter++)
					{
						if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
						{
							if(iter->iCoinTypeCode==valueType)
							{
								lret+=iter->iQuantity;
							}
						}
					}
				}
				else{
					vector<BanknoteAcceptRecord>::iterator iter;
					for(iter=transInfo.vTransBanknoteAcceptDetail.begin();iter!=transInfo.vTransBanknoteAcceptDetail.end();iter++)
					{
						if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
						{
							if(iter->iCoinTypeCode==valueType)
							{
								lret+=iter->iQuantity;
							}
						}
					}
				}
			}
			break;
		case CTransactionHistoryBaseDlg::UE_INSERT:

			{
				vector<BanknoteInsertRecord>::iterator iter;
				for(iter=transInfo.vTransBanknoteDetail.begin();iter!=transInfo.vTransBanknoteDetail.end();iter++)
				{
					if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
					{
						if(iter->iCoinTypeCode==valueType)
						{
							lret+=iter->iQuantity;
						}
					}
				}
			}
			break;
		case CTransactionHistoryBaseDlg::UE_CHANGE:
			{
				vector<BanknoteChangeRecord>::iterator iter;
				for(iter=transInfo.vTransBanknoteChangeDetail.begin();iter!=transInfo.vTransBanknoteChangeDetail.end();iter++)
				{
					if(iter->iTransID==transInfo.iTransactionDetail.iTransID)
					{
						if(iter->iCoinTypeCode==valueType)
						{
							lret+=iter->iQuantity;
						}
					}
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return lret;
}

CString getTransStatus(long lTransStatusID)
{
	CString szRet;
	switch(lTransStatusID)
	{
	case 0:
		szRet=_opl(UE_SUCCESSFUL);
		break;
	case 1:
		szRet=_opl(UE_FAILED);
		break;
	case 2:
		szRet=_opl(UE_CANCELSUCCESSFUL);
		break;
	case 3:
		szRet=_opl(UE_FAILED);
		break;
	}
	return szRet;
}

CString getTransType(long lType)
{
	CString szRet(_T(""));
	switch(lType)
	{
	case 1:	// 郑州没有token 
		szRet=_opl(TXT_SALEGUIDE_ISSUE);
		break;
	case 2:
		szRet=_opl(UE_ISSUECARD);
		break;
	case 3:
		szRet=_opl(UE_ISSUEPRODUCT);
		break;
	case 4:
		szRet=_opl(UE_CHARGE);
		break;
	case 5:
		szRet=_opl(TXT_SALEGUIDE_CARD_TICKET);
		break;
	default:
		break;
	}
	return szRet;
}

CString getTransType(long lType,long lSubType)
{
	CString szRet(_T(""));
	szRet=theACC_PRODUCT.GetProductName(ComMakeWORD(lType,lSubType),theAPP_SESSION.GetCurrentOperatorLanguageType());
	return szRet;
}

CString getPayMentMethod(long payType)
{
	CString szRet=_T("");
	switch(payType){
		case PAYMENT_METHOD_CASH:
			szRet = _T("现金");
			break;	
		case PAYMENT_METHOD_ACC_CARD:
		case PAYMENT_METHOD_YKT_CARD:
			szRet = _T("储值卡");
			break;	
		case PAYMENT_METHOD_CRIDIT_CARD:
			szRet = _T("信用卡");
			break;
		case PAYMENT_METHOD_WITH_NET:
			szRet = _T("网络支付");
			break;
		case PAYMENT_METHOD_WITH_ZHIFUBAO:
			szRet = _T("扫码支付");
			break;	
		default:
			szRet = _T("未知");
			break;
	}

	return szRet;
}

//获取票种
CString getTicketType(long lproductType)
{
	CString szRet(_T(""));
	szRet=theACC_PRODUCT.GetProductName(lproductType,theAPP_SESSION.GetCurrentOperatorLanguageType());
	return szRet;
}

CString getWriteStatus(long lWriteStatus)
{
	CString szRet;
	return szRet;
}

typedef enum _ue_exceptiontype
{
	UE_RELEASETOKEN=1,	//Token放出
	UE_RELEASECARD,		//Card放出
	UE_CHANGECOIN,		//硬币找零
	UE_CHANGEBANKNOTE	//纸币找零
}UE_EXCEPTIONTYPE;

static long getExceptionAmount(TransactionRecord& transInfo,UE_EXCEPTIONTYPE ueType)
{
	long lexceptionAmount=0;
	vector<ExceptionRecord>::iterator iter;
	for(iter=transInfo.vExceptionDetail.begin();iter!=transInfo.vExceptionDetail.end();iter++)
	{
		if(iter->iExceptionType==ueType)
		{
			lexceptionAmount+=iter->IExceptionAmount;
		}
	}
	return lexceptionAmount;
}

static long getExceptionChangeAmount(TransactionRecord& transInfo)
{
	long lret=0;
	lret+=getExceptionAmount(transInfo,UE_CHANGECOIN);
	lret+=getExceptionAmount(transInfo,UE_CHANGEBANKNOTE);
	return lret;
}
// 纸币接收
long CTransactionHistoryBaseDlg::m_get1YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote1Yuan);}
long CTransactionHistoryBaseDlg::m_get5YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote5Yuan);}
long CTransactionHistoryBaseDlg::m_get10YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote10Yuan);}
long CTransactionHistoryBaseDlg::m_get20YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote20Yuan);}
long CTransactionHistoryBaseDlg::m_get50YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote50Yuan);}
long CTransactionHistoryBaseDlg::m_get100YuanAcceptedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote100Yuan);}

// 纸币找零
long CTransactionHistoryBaseDlg::m_get1YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote1Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get5YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote5Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get10YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote10Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get20YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote20Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get50YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote50Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get100YuanChangedNoteNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Banknote100Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}

long CTransactionHistoryBaseDlg::m_get50AVOSAcceptedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,CoinHalfYuan,CTransactionHistoryBaseDlg::UE_INSERT);}
long CTransactionHistoryBaseDlg::m_get1MOPAcceptedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_INSERT);}
long CTransactionHistoryBaseDlg::m_get2MOPAcceptedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_INSERT);}
long CTransactionHistoryBaseDlg::m_get5MOPAcceptedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_INSERT);}

long CTransactionHistoryBaseDlg::m_get50AVOSReleasedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,CoinHalfYuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get1MOPReleasedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get2MOPReleasedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}
long CTransactionHistoryBaseDlg::m_get5MOPReleasedCoinNum(TransactionRecord& transInfo){return getTypeNum(transInfo,Coin1Yuan,CTransactionHistoryBaseDlg::UE_CHANGE);}

void CTransactionHistoryBaseDlg::m_showDetail(CTransactionHistorySvc::Model& dataModel,UE_TRANSTYPE ueTransType)
{
	TransactionRecord& transInfo=*dataModel.ptransInfo;

	if(NULL == dataModel.ptransInfo){
		return;
	}

	LABEL_GROUP_SEVEN notice[15];
	CString szConv(_T(""));
	double width = theGUI_INFO.GetOperatorImageResolutionWidth();

	double Resolution = width / 640;

	//EC
	m_baseInfoArea->SetLableWidth(0,0,Resolution * 60);
	CString sztemp=_opl(UE_EC)+_T(":");
	notice[0].name1=sztemp;

	m_baseInfoArea->SetLableWidth(0,1,Resolution * 40);
	m_baseInfoArea->SetLableAlign(0,1,DT_LEFT | DT_VCENTER);
	sztemp=dataModel.szEc;
	if(sztemp.IsEmpty()){
		sztemp = _T("--");
	}

	notice[0].name2=sztemp;
	//MTC
	m_baseInfoArea->SetLableWidth(0,2,Resolution * 100);
	m_baseInfoArea->SetLableAlign(0,2,DT_RIGHT | DT_VCENTER);
	sztemp=_opl(UE_MTC)+_T(":");
	notice[0].name3=sztemp;

	m_baseInfoArea->SetLableWidth(0,3,Resolution * 100);
	m_baseInfoArea->SetLableAlign(0,3,DT_LEFT | DT_VCENTER);
	sztemp=dataModel.szMtc;
	if(sztemp.IsEmpty()){
		sztemp = _T("--");
	}
	notice[0].name4=sztemp;


	//异常时间
	m_baseInfoArea->SetLableWidth(0,4,Resolution * 100);
	m_baseInfoArea->SetLableAlign(0,4,DT_RIGHT | DT_VCENTER);
	sztemp=_opl(UE_EXCEPTIONTIME);
	sztemp+=_T(":");
	notice[0].name5=sztemp;

	m_baseInfoArea->SetLableWidth(0,5,Resolution * 200);
	m_baseInfoArea->SetLableAlign(0,5,DT_LEFT|DT_VCENTER);
	sztemp=dataModel.szExceptionTime;
	if(sztemp.IsEmpty()){
		sztemp = _T("--");
	}
	notice[0].name6=sztemp;
	


	// 错误描述
	m_baseInfoArea->SetLableAlign(1,0,DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(1,0,Resolution * 100);
	sztemp=_opl(UE_EXCEPTION)+_T(":");
	notice[1].name1=sztemp;
	// 内容
	m_baseInfoArea->SetLableAlign(1,1,DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(1,1,Resolution * 500);
	sztemp=dataModel.szException;
	if(sztemp.IsEmpty()){
		sztemp = _T("--");
	}
	notice[1].name2=sztemp;

	//处理建议
	m_baseInfoArea->SetLableAlign(2,0,DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(2,0,Resolution * 100);
	sztemp=_opl(UE_SOLUTION)+_T(":");
	notice[2].name1=sztemp;

	m_baseInfoArea->SetLableAlign(2,1,DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(2,1,Resolution * 500);
	sztemp=dataModel.szSolution;
	if(sztemp.IsEmpty()){
		sztemp = _T("--");
	}
	notice[2].name2=sztemp;

	//交易訊息
	m_baseInfoArea->SetLableWidth(3,0,Resolution * 100);
	m_baseInfoArea->SetLableAlign(3,0,DT_LEFT | DT_VCENTER);
	sztemp=_opl(UE_TRANSINFO)+_T(":");
	notice[3].name1=sztemp;

	//交易編號
	m_baseInfoArea->SetLableWidth(3,1,Resolution * 100);
	m_baseInfoArea->SetLableAlign(3,1,DT_LEFT | DT_VCENTER);
	sztemp.Format(_T("%s"),transInfo.iTransactionDetail.iUDSN);
	notice[3].name2=sztemp;

	//交易時間
	//sztemp=_opl(UE_TRANSACTIONTIME)+_T(":");
	//notice[4].name1=sztemp;
	m_baseInfoArea->SetLableAlign(3,2,DT_LEFT|DT_VCENTER);
	m_baseInfoArea->SetLableWidth(3,2,Resolution * 150);
	szConv=transInfo.iTransactionDetail.dtTransTime.ToString(_T("%04d-%02d-%02d %02d:%02d"));
	sztemp=szConv;
	notice[3].name3=sztemp;

	//交易狀態
	m_baseInfoArea->SetLableWidth(3,3,Resolution * 100);
	m_baseInfoArea->SetLableAlign(3,3,DT_RIGHT|DT_VCENTER);
	//sztemp=_opl(UE_TRANSACTIONSTATUS)+_T(":");
	//notice[3].name4=sztemp;
	m_baseInfoArea->SetLableWidth(3,4,Resolution * 100);
	m_baseInfoArea->SetLableAlign(3,4,DT_RIGHT|DT_VCENTER);
	sztemp=getTransStatus(transInfo.iTransactionDetail.iState);
	notice[3].name4=sztemp;

	//交易类型
	m_baseInfoArea->SetLableWidth(4,0,Resolution * 100);
	m_baseInfoArea->SetLableAlign(4,0,DT_LEFT|DT_VCENTER);
	sztemp= _T("")/*_opl(UE_OPERATETYPE)*/;
	notice[4].name1=sztemp;
	m_baseInfoArea->SetLableWidth(4,1,Resolution * 100);
	m_baseInfoArea->SetLableAlign(4,1,DT_LEFT| DT_VCENTER);
	sztemp=getTransType(transInfo.iTransactionDetail.iUdType);
	notice[4].name2=sztemp;

	//票種
	m_baseInfoArea->SetLableAlign(4,2,DT_CENTER| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(4,2,Resolution * 150);
	//sztemp=_opl(10110);
	
	WORD wProductType = (transInfo.iTransactionDetail.iProductType << 8| transInfo.iTransactionDetail.iProductSubType);
	sztemp=getTicketType(wProductType);
	notice[4].name3=sztemp;

	// 支付方式
	m_baseInfoArea->SetLableAlign(4,3,DT_RIGHT| DT_VCENTER);
	m_baseInfoArea->SetLableWidth(4,3,Resolution * 100);
	notice[4].name4.Format(_T("%s"),getPayMentMethod(transInfo.iTransactionDetail.iPaymentMethod) );
	
	// 第六行//////////////////////////////////////////////////////////////
	//单价
	m_baseInfoArea->SetLableWidth(5,0,Resolution * 100);
	m_baseInfoArea->SetLableWidth(5,1,Resolution * 50);
	m_baseInfoArea->SetLableAlign(5,1,DT_LEFT| DT_VCENTER);
	sztemp=_opl(UE_UNITPRICE) + _T(":");;
	notice[5].name2=sztemp;
	m_baseInfoArea->SetLableWidth(5,2,Resolution * 100);
	sztemp=ComFormatAmount(transInfo.iTransactionDetail.iUnitAmount);
	sztemp+=UD_MOP;
	notice[5].name3=sztemp;

	//Tax(稅收)
	//m_baseInfoArea->SetLableWidth(6,3,150);
	//m_baseInfoArea->SetLableAlign(5,3,DT_LEFT| DT_VCENTER);
	//m_baseInfoArea->SetLableWidth(5,3,120);
	/*sztemp=_opl(UE_TAX);
	sztemp+=_T(":");
	sztemp+=ComFormatAmount(transInfo.iTransactionDetail.iQuantity*transInfo.iTransactionDetail.TaxAmount);
	sztemp+=UD_MOP;
	notice[5].name4=sztemp;*/


	//switch(ueTransType)
	//{
	//case UE_ISSUECARD:
	//	//押金
	//	m_baseInfoArea->SetLableAlign(5,2,DT_LEFT| DT_VCENTER);
	//	m_baseInfoArea->SetLableWidth(5,2,100);
	//	sztemp=_opl(UE_DEPOSIT);
	//	sztemp+=_T(":");
	//	sztemp+=ComFormatAmount(transInfo.iTransactionDetail.DepositAmount*transInfo.iTransactionDetail.iQuantity);
	//	sztemp+=UD_MOP;
	//	notice[5].name4=sztemp;
	//	break;
	//}


	//第7行///////////////////////////////////////////////////////////////////////////	
	bool isShowQuantity = false;
	m_baseInfoArea->SetLableWidth(6,0,Resolution * 100);
	m_baseInfoArea->SetLableAlign(6,0,DT_LEFT| DT_VCENTER);
	//數量
	switch(ueTransType)
	{
	case UE_ISSUETOKE:
	case UE_ISSUECARD:
		m_baseInfoArea->SetLableAlign(6,1,DT_LEFT| DT_VCENTER);
		m_baseInfoArea->SetLableWidth(6,1,Resolution * 100);
		sztemp=_opl(UE_QUANTITY);
		sztemp+=_T(":");
		sztemp+=ComFormatCount(transInfo.iTransactionDetail.iQuantity);
		notice[6].name2=sztemp;
		isShowQuantity = true;
		break;
	default:
		break;
	}


	// 应付金额
	m_baseInfoArea->SetLableWidth(6,2,Resolution * 100);
	m_baseInfoArea->SetLableAlign(6,2,DT_RIGHT| DT_VCENTER);
	sztemp=_opl(UE_PAYABLE);
	sztemp+=_T(":");
	if (isShowQuantity){
		notice[6].name3=sztemp;
	}else{
		notice[6].name2=sztemp;
	}
	m_baseInfoArea->SetLableWidth(6,3,Resolution * 100);
	m_baseInfoArea->SetLableAlign(6,3,DT_LEFT| DT_VCENTER);
	sztemp=ComFormatAmount(transInfo.iTransactionDetail.iTotalAmount);
	sztemp+=UD_MOP;
	if (isShowQuantity){
		notice[6].name4=sztemp;
	}else{
		notice[6].name3=sztemp;
	}

	
	
	//第8行//////////////////////////////////////////////////////////
	//投入金额
	m_baseInfoArea->SetLableWidth(7,0,Resolution * 100);
	m_baseInfoArea->SetLableAlign(7,0,DT_LEFT| DT_VCENTER);
	sztemp=_opl(UE_INSERTEDAMOUNT)+_T(":");
	notice[7].name1=sztemp;
	//内容
	m_baseInfoArea->SetLableWidth(7,1,Resolution * 100);
	m_baseInfoArea->SetLableAlign(7,1,DT_LEFT| DT_VCENTER);
	sztemp=ComFormatAmount(transInfo.iTransactionDetail.CoinInsertAmount+transInfo.iTransactionDetail.BankNoteAcceptAmount);
	sztemp+=UD_MOP;
	notice[7].name2=sztemp;

	switch(ueTransType)
	{
	case UE_ISSUECARD:
	case UE_ISSUETOKE:
	case UE_ADDPRODUCT:
		//未送出票卡
		m_baseInfoArea->SetLableWidth(7,2,Resolution * 100);
		m_baseInfoArea->SetLableAlign(7,2,DT_RIGHT| DT_VCENTER);
		sztemp=_opl(UE_TICKETUNRELEASED)+_T(":");
		notice[7].name3=sztemp;

		m_baseInfoArea->SetLableWidth(7,3,Resolution * 100);
		m_baseInfoArea->SetLableAlign(7,3,DT_LEFT| DT_VCENTER);

		// 交易类型为取消，未放出票卡数量为0 
		if(transInfo.iTransactionDetail.iState == 2){
			sztemp = _T("0");
		}
		else{
			sztemp=ComFormatCount(transInfo.iTransactionDetail.iQuantity-transInfo.iTransactionDetail.iUnReleaseedMediaQuantity);
		}
		sztemp += _opl(TXT_TRANSDETAIL_PIECE);
		notice[7].name4=sztemp;
		break;
	case UE_RECHARGE:
		//写卡状态
		m_baseInfoArea->SetLableWidth(7,2,Resolution * 100);
		m_baseInfoArea->SetLableAlign(7,2,DT_RIGHT| DT_VCENTER);
		sztemp=_opl(UE_WRITESTATUS)+_T(":");
		notice[7].name3=sztemp;

		m_baseInfoArea->SetLableWidth(7,3,Resolution * 100);
		m_baseInfoArea->SetLableAlign(7,3,DT_LEFT| DT_VCENTER);
		//szConv.Format(_T("%d"),transInfo.iTransactionDetail.iState);
		sztemp=getTransStatus(transInfo.iTransactionDetail.iState);
		notice[7].name4=sztemp;
		break;
	}

	//if(DEVICE_TYPE_TVM == DeviceType)
	if (transInfo.iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_CASH) //如果是现金支付
	{
		//應找零金額
		m_baseInfoArea->SetLableWidth(8,0,Resolution * 100);
		m_baseInfoArea->SetLableAlign(8,0,DT_LEFT| DT_VCENTER);
		sztemp=_opl(UE_CHANGEAMOUNT)+_T(":");
		notice[8].name1=sztemp;

		m_baseInfoArea->SetLableWidth(8,1,Resolution * 100);
		m_baseInfoArea->SetLableAlign(8,1,DT_LEFT| DT_VCENTER);
		sztemp=ComFormatAmount(transInfo.iTransactionDetail.iChangeAmount > 0 ? transInfo.iTransactionDetail.iChangeAmount : 0);
		sztemp+=UD_MOP;
		notice[8].name2=sztemp;

		//未找零金額
		m_baseInfoArea->SetLableWidth(8,2,Resolution * 100);
		m_baseInfoArea->SetLableAlign(8,2,DT_RIGHT| DT_VCENTER);
		sztemp=_opl(UE_CHANGEUNRELEASED)+_T(":");
		notice[8].name3=sztemp;

		m_baseInfoArea->SetLableWidth(8,3,Resolution * 100);
		m_baseInfoArea->SetLableAlign(8,3,DT_LEFT| DT_VCENTER);
		long lUnChangedAmount=getExceptionChangeAmount(transInfo);
		sztemp=ComFormatAmount((lUnChangedAmount));
		sztemp+=UD_MOP;
		notice[8].name4=sztemp;
	}
	
	
	if (transInfo.iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_CASH) //如果是现金支付
	{
		//已接受紙幣金額
		sztemp=_opl(UE_BANKNOTEACCEPTED)+_T(":");
		notice[9].name1=sztemp;

		sztemp=_T("1元:");
		notice[9].name2=sztemp;

		sztemp=ComFormatCount(m_get1YuanAcceptedNoteNum(transInfo));
		notice[9].name3=sztemp;

		sztemp=_T("5") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[9].name4=sztemp;

		sztemp=ComFormatCount(m_get5YuanAcceptedNoteNum(transInfo));
		notice[9].name5=sztemp;

		sztemp=_T("10") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[9].name6=sztemp;

		sztemp=ComFormatCount(m_get10YuanAcceptedNoteNum(transInfo));
		notice[9].name7=sztemp;
		////////////////////////////////////////////////////////////////////////////
		sztemp=_T("20") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[10].name2=sztemp;

		sztemp=ComFormatCount(m_get20YuanAcceptedNoteNum(transInfo));
		notice[10].name3=sztemp;

		sztemp=_T("50") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[10].name4=sztemp;

		sztemp=ComFormatCount(m_get50YuanAcceptedNoteNum(transInfo));
		notice[10].name5=sztemp;

		sztemp=_T("100") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[10].name6=sztemp;

		sztemp=ComFormatCount(m_get100YuanAcceptedNoteNum(transInfo));
		notice[10].name7=sztemp;

		// 纸币找零
		sztemp=_opl(BH_TEST_TEXT_CHANGE_BANKNOTES) + _opl(TXT_COLON);
		notice[11].name1=sztemp;

		/*sztemp=_T("1元:");
		notice[1].name2=sztemp;

		sztemp=ComFormatCount(m_get1YuanChangedNoteNum(transInfo));
		notice[11].name3=sztemp;*/

		sztemp=_T("5") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		notice[11].name2=sztemp;

		sztemp=ComFormatCount(m_get5YuanChangedNoteNum(transInfo));
		notice[11].name3 = sztemp;

		sztemp=_T("10元:");
		notice[11].name4=sztemp;

		sztemp=ComFormatCount(m_get10YuanChangedNoteNum(transInfo));
		notice[11].name5=sztemp;
		/*
		////////////////////////////////////////////////////////////////////////////
		sztemp=_T("20元:");
		notice[12].name2=sztemp;

		sztemp=ComFormatCount(m_get20YuanChangedNoteNum(transInfo));
		notice[12].name3=sztemp;

		sztemp=_T("50元:");
		notice[12].name4=sztemp;

		sztemp=ComFormatCount(m_get50YuanChangedNoteNum(transInfo));
		notice[12].name5=sztemp;

		sztemp=_T("100元:");
		notice[12].name6=sztemp;

		sztemp=ComFormatCount(m_get100YuanChangedNoteNum(transInfo));
		notice[12].name7=sztemp;*/

		if(DEVICE_TYPE_TVM == DeviceType)
		{
			// 接收硬币
			sztemp=_opl(UE_COINACCEPTED)+_T(":");
			notice[12].name1=sztemp;

			sztemp=_T("1 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
			notice[12].name2=sztemp;

			sztemp=ComFormatCount(m_get1MOPAcceptedCoinNum(transInfo)+m_get50AVOSAcceptedCoinNum(transInfo));
			notice[12].name3=sztemp;


			//sztemp = _T("0.5 元:");
			//notice[12].name4 = sztemp;

			//sztemp = ComFormatCount(m_get50AVOSAcceptedCoinNum(transInfo));
			//notice[12].name5 = sztemp;


			/*sztemp=_T("1 MOP:");
			notice[11].name4=sztemp;

			sztemp=ComFormatCount(m_get1MOPAcceptedCoinNum(transInfo));
			notice[11].name5=sztemp;*/

			//////////////////////////////////////////////////////
			//sztemp=_T("2 MOP:");
			//notice[12].name2=sztemp;

			//sztemp=ComFormatCount(m_get2MOPAcceptedCoinNum(transInfo));
			//notice[12].name3=sztemp;

			//sztemp=_T("5 MOP:");
			//notice[12].name4=sztemp;

			//sztemp=ComFormatCount(m_get5MOPAcceptedCoinNum(transInfo));
			//notice[12].name5=sztemp;

			//硬币找零
			//m_baseInfoArea->SetLableWidth(13,0,Resolution * 110);
			//m_baseInfoArea->SetLableWidth(13,1,Resolution * 90);
			sztemp=_opl(UE_RELEASECHANGE)+_T(":");
			notice[13].name1=sztemp;

			sztemp=_T("1 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
			notice[13].name2=sztemp;

			sztemp=ComFormatCount(m_get50AVOSReleasedCoinNum(transInfo) + m_get1MOPReleasedCoinNum(transInfo));
			notice[13].name3=sztemp;


			//sztemp = _T("0.5 元:");
			//notice[13].name4 = sztemp;

			//notice[13].name5 = ComFormatCount(m_get50AVOSReleasedCoinNum(transInfo));

			//sztemp=_T("1 MOP:");
			//notice[13].name4=sztemp;

			//sztemp=ComFormatCount(m_get1MOPReleasedCoinNum(transInfo));
			//notice[13].name5=sztemp;
			//////////////////////////////////////////////////////////////////////////////
			//sztemp=_T("2 MOP:");
			//notice[14].name2=sztemp;

			//sztemp=ComFormatCount(m_get2MOPReleasedCoinNum(transInfo));
			//notice[14].name3=sztemp;

			//sztemp=_T("5 MOP:");
			//notice[14].name4=sztemp;

			//sztemp=ComFormatCount(m_get5MOPReleasedCoinNum(transInfo));
			//notice[14].name5=sztemp;

		}
	}
	//if (transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::EP ||transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::BANK_CARD
	//	|| transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::CREDIT_CARD) //  如果是EP支付
	//{
	//	long lAmount = 0;
	//	notice[++iRow].name1.Format(_T("%s:"),getPayMentMethod(transInfo.iTransactionDetail.iPaymentMethod));
	//	notice[iRow].name2 = _T("支付金額:");
	//	if (transInfo.iTransactionDetail.iState==0 || transInfo.iTransactionDetail.iState==2)// 正常结束或交易取消
	//	{
	//		lAmount = transInfo.iTransactionDetail.iTotalAmount;
	//	}
	//	notice[iRow].name3 = ComFormatAmount(lAmount);
	//}
	
	setlabelgroup(notice,15);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTransactionHistoryBaseDlg::~CTransactionHistoryBaseDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CTransactionHistoryBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
        return -1;
    }
	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_labelGroup);

	//double width = theGUI_INFO.GetOperatorImageResolutionWidth();

	double Resolution = RESOLUTION;

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,Resolution * 100);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,Resolution * 100);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,Resolution * 60);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,Resolution * 100);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,Resolution * 60);		// 设置第五列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,Resolution * 100);		// 设置第六列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,Resolution * 60);		// 设置第七列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		强制刷新基础数据区

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTransactionHistoryBaseDlg::forceRefreshBaseInfoArea()
{
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTransactionHistoryBaseDlg::UpdateUI()
{
	CTransactionHistorySvc* pSvc = (CTransactionHistorySvc*)GetService();
	if(NULL!=pSvc)
	{
		ClearUI();
		long lCurrentPage=pSvc->GetCurrentPage();
		pSvc->turnPages(lCurrentPage-1);
		showDetail();
	}else
	{
	}
	m_baseInfoArea->Invalidate();
	return ;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面激活后的处理
//
//@param      (i)UINT nState      不使用
//@param      (i)CWnd* pWndOther  不使用
//@param      (i)BOOL bMinimized  不使用
//
//@retval     void
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
void CTransactionHistoryBaseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	theAPP_SESSION.SetKeyboard(KEY_PAGE|KEY_RETURN|KEY_ENTER);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置基础数据区的显示内容
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
void CTransactionHistoryBaseDlg::setlabelgroup(const LABEL_GROUP_SEVEN* plabelgroup,const UINT uigroupnum)
{
	UINT uiloop=0;
	for(;uiloop<UD_MAX_ROW;uiloop++)
	{
		if(uiloop<uigroupnum)
		{			
			const LABEL_GROUP_SEVEN* ptemp=plabelgroup+uiloop;
			m_labelGroup[uiloop].name1 = ptemp->name1;
			m_labelGroup[uiloop].name2 = ptemp->name2;
			m_labelGroup[uiloop].name3= ptemp->name3;
			m_labelGroup[uiloop].name4= ptemp->name4;
			m_labelGroup[uiloop].name5= ptemp->name5;
			m_labelGroup[uiloop].name6= ptemp->name6;
			m_labelGroup[uiloop].name7= ptemp->name7;
		}else
		{
			m_labelGroup[uiloop].name1 = _T("");
			m_labelGroup[uiloop].name2 = _T("");
			m_labelGroup[uiloop].name3 = _T("");
			m_labelGroup[uiloop].name4 = _T("");
			m_labelGroup[uiloop].name5 = _T("");
			m_labelGroup[uiloop].name6 = _T("");
			m_labelGroup[uiloop].name7 = _T("");
		}
	}
}


CString CTransactionHistoryBaseDlg::GetStrPayment(TransactionRecord& transInfo)
{
	CString str = _T("");
	if (transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::CASH)
	{
		str = _opl(TXT_PAYMENT_CASH);
	}else if (transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::EP)
	{
		str = _opl(TXT_PAYMENT_EP);
	}else if (transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::BANK_CARD)
	{
		str = _opl(TXT_PAYMENT_BANK_CARD);
	}else if (transInfo.iTransactionDetail.iPaymentMethod == CAuditHelper::CREDIT_CARD)
	{
		str = _opl(TXT_PAYMENT_CREDIT_CARD);
	}
	return str;
}

void CTransactionHistoryBaseDlg::ClearUI()
{
	for (int i = 0; i<15 ;i++ ){
		m_labelGroup[i].name1 = _T("");
		m_labelGroup[i].name2 = _T("");
		m_labelGroup[i].name3 = _T("");
		m_labelGroup[i].name4 = _T("");
		m_labelGroup[i].name5 = _T("");
		m_labelGroup[i].name6 = _T("");
		m_labelGroup[i].name7 = _T("");

	}
}