#include "stdafx.h"
#include "TVMFailureRefundBaseOfficeDlg.h"
#include "TVMFailureRefundSvc.h"
#include "CTTVMRefund.h"
#include "guideinfo.h"
#include "TranscationTranslateSvc.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 最大退款金额，根据一卡通最大充值金额和上位5.80电文为2个字节计算得到(元)
static const int MAX_REFUND_MONEY  = 655;

// 编辑框位置
static CRect editFailure[] = 
{
	BASE_INFO_RECT(1,8),
	BASE_INFO_RECT(2,10),
	BASE_INFO_RECT(3,7),
};

IMPLEMENT_DYNAMIC(CTVMFailureRefundBaseOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CTVMFailureRefundBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundBaseOfficeDlg::CTVMFailureRefundBaseOfficeDlg(CService* pService) :
COperationDlg(CTVMFailureRefundBaseOfficeDlg::IDD, pService)
{
	CTVMFailureRefundSvc* pTVMFailureRefundSvc = (CTVMFailureRefundSvc*)GetService();
	m_baseInfo->titleInfo.titleName = TITLE_FAILURE_TITLENAME;									// 标题
	m_baseInfo->detailInfo.row = FAILURE_INFO_SIZE;												// 行数
	m_baseInfo->detailInfo.labelGroup = pTVMFailureRefundSvc->GetFailureInfo();					// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = EDIT_COUNT;											// 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editFailure;									// 编辑框位置

	m_strTransNum = _T("");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundBaseOfficeDlg::~CTVMFailureRefundBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int \n
0:成功  -1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMFailureRefundBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域，返回键，确认键
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|BASE_AREA /*|  BTN_RESET*/ | BTN_OK)){
		return -1;
	}
	
	// 设备编号最大8位
	GetEdit(0)->SetLimitText(8);
	GetEdit(0)->SetScope(CGEdit::HEX_CHAR);

	// 退款凭条号最大10位
	GetEdit(1)->SetLimitText(10);

	// 退款金额最大3位
	GetEdit(2)->SetLimitText(6);
	GetEdit(2)->SetScope(CGEdit::CASH);
	// 设置编辑框焦点
	SetEditFocus(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CTVMFailureRefundBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CString sMsg = (LPCTSTR)lParam;

	CTVMFailureRefundSvc* pTVMFailureRefundSvc = (CTVMFailureRefundSvc*)GetService();

	try{
			if (CheckValid()){
				// 打开钱箱
				pTVMFailureRefundSvc->GetPRTHelper()->PrinterOpenBox();

				int i=1,j=0;
				CString strDeviceNo = _T("");
				CString strNo = _T("");
				CString strLine=_T("");

				// 设置设备编号
				strDeviceNo = GetEditText(0);
				pTVMFailureRefundSvc->SetFailureInfo(i++,strDeviceNo);
			
				// 设置退款凭条号
				strNo = GetEditText(1);
				pTVMFailureRefundSvc->SetFailureInfo(i++,strNo);
				DWORD iNo = _ttoi64(strNo);

				// 设置退款金额
				m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(ComGetAmount(GetEditText(2))) + MONEY_UNIT;
				// 刷新窗体
				ShowCompoment(BASE_AREA);

				// 刷新乘客画面相关信息
				pTVMFailureRefundSvc->SetRefundMoney(ComGetAmount(GetEditText(2)));

				// 发电文
				BYTE bitNum[15]={0};
				
				// 故障设备号
				USES_CONVERSION;
				ComfrHEX(T2A(strDeviceNo),bitNum,4);
				//memcpy(bitNum,,4);
				// 故障时间
				memset(bitNum+4,0,7);
				// 故障设备AFC流水号
		
				memcpy(bitNum+4+7,&iNo,4);
				DWORD dwRefundAmount = ComGetAmount(GetEditText(2));

				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertBomRefundTVMMsg(bitNum,dwRefundAmount);

				// 增加收益情况
				SCAuditKey purchaseKey;
				purchaseKey.serviceID = TVMFAILURE_REFUND_SVC;
				purchaseKey.cardIssuancer = ACC;
				purchaseKey.ticketType = TICKET_SINGLE_RIDE;
				
				SCAuditValue purchaseValue;
				purchaseValue.transCount = 1;
				purchaseValue.transAmount = 0-dwRefundAmount;
				theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

				// 一键明细处理
				//pTVMFailureRefundSvc->DoDetailInfo(atoi(GetEditText(2)));

				// 打印帐票	
				pTVMFailureRefundSvc->PrintServiceInfo(strDeviceNo,strNo);

				// 隐藏所有编辑框，数字键和确认，返回，即时退款，非即时退款按钮
				HideCompoment(BTN_OK|BTN_RESET|EDIT_BOX);

				// 默认时间后转至默认业务
				theSERVICE_MGR.SetState(FINISH);

				// 显示BOM文言：TVM故障退款成功
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_FINISH));
			}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pTVMFailureRefundSvc->GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CTVMFailureRefundBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// 重置对话框文字
	SetEditText(0,_T(""));
	SetEditText(1,_T(""));
	SetEditText(2,_T(""));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      对话框初始化

@param      无

@retval     BOOL  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTVMFailureRefundBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查数据有效性

@param      无

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMFailureRefundBaseOfficeDlg::CheckValid()
{
	CString strTicketNo = GetEditText(1);
	CString strDeviceNo = GetEditText(0);
	CString strMoney = GetEditText(2);
	

	// 检查设备编号是否为空
	if (_T("") == strDeviceNo){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_DEVICE_NO));
		return false;
	}

	// 检查设备编号是否为8位
	if (strDeviceNo.GetLength() != 8){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_DEVICE_MUST_BE_NUMBER));
		return false;
	}

	// 检查凭条号是否为空
	if (_T("") == strTicketNo || strTicketNo.GetLength() != 10){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_TICKET_NO));
		return false;
	}

	__int64  iNo = _ttoi64(strTicketNo);
	if(iNo > 0xFFFFFFFF)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TICKET_MUST_BE_NUMBER));
		return false;
	}

	// 检查金额是否为空
	if (_T("") == strMoney){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_MONEY));
		return false;
	}

	// 检查是否已退款
	// 由亦庄线代码copy过来，由于不能实现功能，注释掉

	/*CString sTVMErrorID = _T("");
	CString sErrorNum = _T("");
	sErrorNum.Format("%08d", atoi(strTicketNo));
	sTVMErrorID = strDeviceNo + sErrorNum;

	CCTVMRefund::TVM_ERROR_TICKET_ID ErrId;
	ErrId.csTVMErrorSierialNo = sTVMErrorID;

	
	if (theTVMRefund.CheckIsTVMTicket(strTicketNo) == false){
		vector<CCTVMRefund::TVM_ERROR_TICKET_ID> vecErrorID;
		vecErrorID.push_back(ErrId);
		theTVMRefund.UpdateTVMTicketInfo(vecErrorID);
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_ERROR));
		return false;
	}*/
	return true; 
}


