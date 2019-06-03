#include "stdafx.h"
#include "CardReadWriteMachineTestMaintainDlg.h"
#include "RWTestSvc.h"
#include "GuideInfo.h"
#include "RWMonitorSvc.h"
#include "TWMonitorSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCardReadWriteMachineTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CCardReadWriteMachineTestMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRWTestBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnRWDetailTestBtnClick)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

static const CString STR_RW_TYPE[] = {_T("外部读写器"),_T("内部读写器")};
static const CString STR_OUTER_RW_DETAIL_TYPE[] = {_T("读取TPU信息"),_T("读取时钟"),_T("读取参数版本"),_T("读卡测试"),_T("重置测试")};
static const CString STR_INNER_RW_DETAIL_TYPE[] = {_T("读取TPU信息"),_T("读取时钟"),_T("读取参数版本"),_T("读卡测试"),_T("重置测试")};

static const CString STR_RW_TYPE_MSG[] = {_T("外部读写器"),_T("内部读写器")};
static const CString STR_OUTER_RW_DETAIL_TYPE_MSG[] = {_T("读取TPU信息"),_T("读取时钟"),_T("读取参数版本"),_T("读卡测试"),_T("重置测试")};
static const CString STR_INNER_RW_DETAIL_TYPE_MSG[] = {_T("读取TPU信息"),_T("读取时钟"),_T("读取参数版本"),_T("读卡测试"),_T("重置测试")};

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCardReadWriteMachineTestMaintainDlg::CCardReadWriteMachineTestMaintainDlg(CService* pService)
: COperationDlg(CCardReadWriteMachineTestMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TXT_RW_TEST_SVC_CN;					// 标题
	m_baseInfo->detailInfo.row = 2;											// 行数
	m_baseInfo->detailInfo.labelGroup = m_CardReadWriteInfo;				// 标签文字
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCardReadWriteMachineTestMaintainDlg::~CCardReadWriteMachineTestMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CCardReadWriteMachineTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		对话框初始化

@param      无

@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCardReadWriteMachineTestMaintainDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// 显示BOM文言：请选择RW测试项目
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_CHOOSE_ITEM));
	
	//初始化lable信息
	m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("读写器类型："),MAX_SBC_CASE_LEN);
	m_CardReadWriteInfo[1].nameLeft = add_wsp(_T("测试项目："),MAX_SBC_CASE_LEN);
	
	// 初始化按钮
	InitialButtons();

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		初始化按键

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCardReadWriteMachineTestMaintainDlg::InitialButtons()
{
	m_vec_Right_1_BTN_INFO.clear();
	// 设置单选
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	for (int i = 0; i < 2; i++){
		BTN_INFO pRWTypeInfo;
		pRWTypeInfo.btnID = i + 1;
		pRWTypeInfo.btnMsg = STR_RW_TYPE_MSG[i];
		pRWTypeInfo.text = STR_RW_TYPE[i];
		m_vec_Right_1_BTN_INFO.push_back(pRWTypeInfo);
	}
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickFirstAvailButton();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认、返回 按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n		TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetForeServiceBusy(true);

	CRWTestSvc* pRWTestSvc = (CRWTestSvc*)GetService();
	try{
		m_CardReadWriteInfo[0].nameLeft = _T("");
		m_CardReadWriteInfo[0].nameRight = _T("");
		m_CardReadWriteInfo[1].nameLeft = _T("");
		m_CardReadWriteInfo[1].nameRight = _T("");

		if(m_RWDetailType == 11 || m_RWDetailType == 21){
			m_baseInfo->titleInfo.titleName = _T("TPU信息");
			m_baseInfo->detailInfo.row = 8;
			m_CardReadWriteInfo[0].nameLeft = add_hsp(_T("TPU设备编号："),20);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[1].nameLeft = add_hsp(_T("应用软件版本："),20);
			m_CardReadWriteInfo[1].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[2].nameLeft = add_hsp(_T("主控软件版本："),20);
			m_CardReadWriteInfo[2].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[3].nameLeft = add_hsp(_T("硬件版本："),20);
			m_CardReadWriteInfo[3].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[4].nameLeft = add_hsp(_T("BMAC PSAM编号："),20);
			m_CardReadWriteInfo[4].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[5].nameLeft = add_hsp(_T("BMAC ISAM编号："),20);
			m_CardReadWriteInfo[5].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[6].nameLeft = add_hsp(_T("ACC PSAM编号："),20);
			m_CardReadWriteInfo[6].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[7].nameLeft = add_hsp(_T("ACC ISAM编号："),20);
			m_CardReadWriteInfo[7].nameRight = TXT_UNDEFINE;
			RW_RSP_TPU_INFO tpuInfo;
			long errorCode = -1;
			if(m_RWDetailType == 11){
				errorCode = pRWTestSvc->GetRWHelper()->GetTPUInfo(tpuInfo);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetTPUInfo(tpuInfo);
			}
			if(errorCode == 0){
				CString tmp;
				CHAR pDataMsgNo[28+2]={0x00};
				PSTR2BCD((char*)tpuInfo.DeviceCode,4,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.DeviceCode,4));
				tmp.Format(_T("    0x%s"),pDataMsgNo);

				m_CardReadWriteInfo[0].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.softwareVersionApp[1],tpuInfo.softwareVersionApp[0]));
				m_CardReadWriteInfo[1].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.softwareVersionMain[1],tpuInfo.softwareVersionMain[0]));
				m_CardReadWriteInfo[2].nameRight = tmp;

				tmp.Format(_T("    %d"),ComMakeWORD(tpuInfo.hardwareVersion[1],tpuInfo.hardwareVersion[0]));
				m_CardReadWriteInfo[3].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.bmacPSAMid,6,pDataMsgNo);	
				//tmp.Format(_T("    0x%s"),changeBuff2String((char*)tpuInfo.bmacPSAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[4].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.bmacISAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.bmacISAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[5].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.accPSAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.accPSAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[6].nameRight = tmp;

				PSTR2BCD((char*)tpuInfo.accISAMid,6,pDataMsgNo);	
				//tmp.Format("    0x%s",changeBuff2String((char*)tpuInfo.accISAMid,6));
				tmp.Format(_T("    0x%s"),pDataMsgNo);
				m_CardReadWriteInfo[7].nameRight = tmp;

				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 12 || m_RWDetailType == 22){
			m_baseInfo->titleInfo.titleName = _T("时钟信息");
			m_baseInfo->detailInfo.row = 1;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("日期时间："),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			_DATE_TIME timeInfo;
			long errorCode = -1;
			if(m_RWDetailType == 12){
				errorCode = pRWTestSvc->GetRWHelper()->GetTime(timeInfo);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetTime(timeInfo);
			}
			if(errorCode == 0){
				//m_CardReadWriteInfo[0].nameRight = timeInfo.ToString("%.4d/%.2d/%.2d %.2d:%.2d:%.2d");
				_DATE_TIME curTime = ComGetCurTime();
				m_CardReadWriteInfo[0].nameRight = curTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 13 || m_RWDetailType == 23){
			m_baseInfo->titleInfo.titleName = _T("参数版本信息");
			m_baseInfo->detailInfo.row = 10;
			CString strLeft;
			CString strRight;
			strLeft.Format(_T("运营：--"));
			strRight.Format(_T("       行政处理手续费：--"));
			m_CardReadWriteInfo[0].nameLeft = strLeft;
			m_CardReadWriteInfo[0].nameRight = strRight;

			strLeft.Format(_T("超时补交费用：--"));
			strRight.Format(_T("       设备控制：--"));
			m_CardReadWriteInfo[1].nameLeft = strLeft;
			m_CardReadWriteInfo[1].nameRight = strRight;

			strLeft.Format(_T("区段：--"));
			strRight.Format(_T("       车站位置：--"));
			m_CardReadWriteInfo[2].nameLeft = strLeft;
			m_CardReadWriteInfo[2].nameRight = strRight;

			strLeft.Format(_T("时间浮动表：--"));
			strRight.Format(_T("       计费站点：--"));
			m_CardReadWriteInfo[3].nameLeft = strLeft;
			m_CardReadWriteInfo[3].nameRight = strRight;

			strLeft.Format(_T("时间类型表：--"));
			strRight.Format(_T("       票卡属性：--"));
			m_CardReadWriteInfo[4].nameLeft = strLeft;
			m_CardReadWriteInfo[4].nameRight = strRight;

			strLeft.Format(_T("折扣率表：--"));
			strRight.Format(_T("       基础票价表：--"));
			m_CardReadWriteInfo[5].nameLeft = strLeft;
			m_CardReadWriteInfo[5].nameRight = strRight;

			strLeft.Format(_T("单条黑名单：--"));
			strRight.Format(_T("       票价级别表：--"));
			m_CardReadWriteInfo[6].nameLeft = strLeft;
			m_CardReadWriteInfo[6].nameRight = strRight;

			strLeft.Format(_T("区段黑名单：--"));
			strRight.Format(_T("       票价调整策略表：--"));
			m_CardReadWriteInfo[7].nameLeft = strLeft;
			m_CardReadWriteInfo[7].nameRight = strRight;

			strLeft.Format(_T("白名单：--"));
			strRight.Format(_T("       时间类型明细：--"));
			m_CardReadWriteInfo[8].nameLeft = strLeft;
			m_CardReadWriteInfo[8].nameRight = strRight;

			strLeft.Format(_T("城市代码对照：--"));
			strRight.Format(_T("       票卡发行商：--"));
			m_CardReadWriteInfo[9].nameLeft = strLeft;
			m_CardReadWriteInfo[9].nameRight = strRight;

			VERSION_ACC accVersion;
			VERSION_ECT ectVersion;
			VERSION_AFC afcVersion;
			long errorCode = -1;
			if(m_RWDetailType == 13){
				errorCode = pRWTestSvc->GetRWHelper()->GetParameterInfo(accVersion,ectVersion,afcVersion);
			}
			else{
				errorCode = pRWTestSvc->GetTWHelper()->GetParameterInfo(accVersion,ectVersion,afcVersion);
			}
			if(errorCode == 0){
				strLeft.Format(_T("运营：%d"),accVersion.lBusinessParam);
				strRight.Format(_T("       行政处理手续费：%d"),accVersion.lServerFeeParam);
				m_CardReadWriteInfo[0].nameLeft = strLeft;
				m_CardReadWriteInfo[0].nameRight = strRight;

				strLeft.Format(_T("超时补交费用：%d"),accVersion.lOverTimeAdjuestParam);
				strRight.Format(_T("       设备控制：%d"),accVersion.lDeviceControlParam);
				m_CardReadWriteInfo[1].nameLeft = strLeft;
				m_CardReadWriteInfo[1].nameRight = strRight;

				strLeft.Format(_T("区段参数：%d"),accVersion.lSectionParam);
				strRight.Format(_T("       车站位置参数：%d"),accVersion.lStationParam);
				m_CardReadWriteInfo[2].nameLeft = strLeft;
				m_CardReadWriteInfo[2].nameRight = strRight;

				strLeft.Format(_T("时间浮动：%d"),accVersion.lTimeFloatParam);
				strRight.Format(_T("       计费站点：%d"),accVersion.lPayStationParam);
				m_CardReadWriteInfo[3].nameLeft = strLeft;
				m_CardReadWriteInfo[3].nameRight = strRight;

				strLeft.Format(_T("时间类型表：%d"),accVersion.lTimeTypeParam);
				strRight.Format(_T("       票卡属性：%d"),accVersion.lCardAttributeParam);
				m_CardReadWriteInfo[4].nameLeft = strLeft;
				m_CardReadWriteInfo[4].nameRight = strRight;

				strLeft.Format(_T("折扣率表：%d"),accVersion.lDiscountParam);
				strRight.Format(_T("       基础票价表：%d"),accVersion.lBasePriceParam);
				m_CardReadWriteInfo[5].nameLeft = strLeft;
				m_CardReadWriteInfo[5].nameRight = strRight;

				strLeft.Format(_T("单条黑名单：%d"),accVersion.lSingleBlackListParam);
				strRight.Format(_T("       票价级别表：%d"),accVersion.lPrieLevelParam);
				m_CardReadWriteInfo[6].nameLeft = strLeft;
				m_CardReadWriteInfo[6].nameRight = strRight;

				strLeft.Format(_T("区段黑名单：%d"),accVersion.lSectionBlackListParam);
				strRight.Format(_T("       票价调整策略表：%d"),accVersion.lPriceAdjuestParam);
				m_CardReadWriteInfo[7].nameLeft = strLeft;
				m_CardReadWriteInfo[7].nameRight = strRight;

				strLeft.Format(_T("白名单：%d"),accVersion.lWriteListParam);
				strRight.Format(_T("       时间类型明细：%d"),accVersion.lTimeDetailParam);
				m_CardReadWriteInfo[8].nameLeft = strLeft;
				m_CardReadWriteInfo[8].nameRight = strRight;

				strLeft.Format(_T("城市代码对照：%d"),accVersion.lOtherCityCardParam);
				strRight.Format(_T("       票卡发行商：%d"),accVersion.lissueCompanyParam);
				m_CardReadWriteInfo[9].nameLeft = strLeft;
				m_CardReadWriteInfo[9].nameRight = strRight;

				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
		else if(m_RWDetailType == 14 || m_RWDetailType == 24){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_PUT_CARD));
			m_baseInfo->titleInfo.titleName = _T("读卡信息");
			m_baseInfo->detailInfo.row = 2;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("票面卡号："),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			m_CardReadWriteInfo[1].nameLeft = add_wsp(_T("车票种类："),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[1].nameRight = TXT_UNDEFINE;
			if(m_RWDetailType == 14){
				pRWTestSvc->GetRWHelper()->ReadCard();
				INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pRWTestSvc->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				if(ret == IDCANCEL){
					pRWTestSvc->GetRWHelper()->StopReadCard();
				}
			}
			else{
				pRWTestSvc->GetTWHelper()->ReadCard();
				INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pRWTestSvc->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				if(ret == IDCANCEL){
					pRWTestSvc->GetTWHelper()->StopReadCard();
				}
			}
		}
		else if(m_RWDetailType == 15 || m_RWDetailType == 25){
			m_baseInfo->titleInfo.titleName = _T("重置测试信息");
			m_baseInfo->detailInfo.row = 1;
			m_CardReadWriteInfo[0].nameLeft = add_wsp(_T("重置状态："),MAX_SBC_CASE_LEN);
			m_CardReadWriteInfo[0].nameRight = TXT_UNDEFINE;
			bool result = false;
			if(m_RWDetailType == 15){
				// 清除RW异常对象
				theEXCEPTION_MGR.RemoveException(CRWHardwareException::MODULE_ID);
				theEXCEPTION_MGR.RemoveException(CRWException::MODULE_ID);
				// 修复外部读卡器
				result = pRWTestSvc->GetRWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(true)){
					theSERVICE_MGR.GetService<CRWMonitorSvc>(MONITOR_RW_SVC)->Resume();
				}					
			}
			else{
				// 清除TW异常对象
				theEXCEPTION_MGR.RemoveException(CTWHardwareException::MODULE_ID);
				theEXCEPTION_MGR.RemoveException(CTWException::MODULE_ID);
				// 修复内部读卡器
				result = pRWTestSvc->GetTWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(false)){
					theSERVICE_MGR.GetService<CTWMonitorSvc>(MONITOR_TW_SVC)->Resume();
				}
			}
			if(result){
				m_CardReadWriteInfo[0].nameRight = "成功";
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_SUCCESS));
			}
			else{
				m_CardReadWriteInfo[0].nameRight = "失败";
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_FAIL));
			}
		}
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		CInnerException e = CInnerException(pRWTestSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	// 隐藏编辑框和确认按钮
	HideCompoment(BTN_OK);
	// 刷新窗体
	ShowCompoment(BASE_AREA);
	// 隐藏两侧按钮
	m_vec_Right_1_BTN_INFO.clear();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_vec_Right_2_BTN_INFO.clear();
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);

	theSERVICE_MGR.SetForeServiceBusy(false);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读写器测试按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnRWTestBtnClick(WPARAM wParam,LPARAM lParam)
{
	try	{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;

		m_CardReadWriteInfo[0].nameRight = btnInfo->btnMsg;

		m_RWType = btnInfo->btnID;
		m_vec_Right_2_BTN_INFO.clear();
		// 设置单选
		m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
		switch(m_RWType)
		{
			// 外部读写器
		case 1:
			for (int i = 0; i < 5; i++){
				BTN_INFO pRWDetailTypeBtnInfo;
				pRWDetailTypeBtnInfo.btnID = i + 11;
				pRWDetailTypeBtnInfo.btnMsg = STR_OUTER_RW_DETAIL_TYPE_MSG[i];
				pRWDetailTypeBtnInfo.text = STR_OUTER_RW_DETAIL_TYPE[i];
				m_vec_Right_2_BTN_INFO.push_back(pRWDetailTypeBtnInfo);
			}
			break;
			// 内部读写器
		case 2:
			for (int i = 0; i < 5; i++){
				BTN_INFO pRWDetailTypeBtnInfo;
				pRWDetailTypeBtnInfo.btnID = i + 21;
				pRWDetailTypeBtnInfo.btnMsg = STR_INNER_RW_DETAIL_TYPE_MSG[i];
				pRWDetailTypeBtnInfo.text = STR_INNER_RW_DETAIL_TYPE[i];
				m_vec_Right_2_BTN_INFO.push_back(pRWDetailTypeBtnInfo);
			}
			break;
		default:
			break;
		}
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_2_Group->ClickFirstAvailButton();

		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读写器详细测试按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardReadWriteMachineTestMaintainDlg::OnRWDetailTestBtnClick(WPARAM wParam,LPARAM lParam)
{
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;

		m_CardReadWriteInfo[1].nameRight = btnInfo->btnMsg;

		m_RWDetailType = btnInfo->btnID;

		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

