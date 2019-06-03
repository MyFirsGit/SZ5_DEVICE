#include "stdafx.h"
#include "CardBoxManageSvc.h"
#include "CTCardCountInfo.h"
#include "SCMessageMakeHelper.h"
#include "TemplateHelper.h"
#include "DataHelper.h"
#include "CardTicketBoxInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

static const TCHAR* const WM_BASE_TO_INSTALL = _T("WM_BASE_TO_INSTALL");
static const TCHAR* const WM_BASE_TO_REMOVE = _T("WM_BASE_TO_REMOVE");
static const TCHAR* const WM_INSTALL_TO_BASE = _T("WM_INSTALL_TO_BASE");
static const TCHAR* const WM_REMOVE_TO_BASE = _T("WM_REMOVE_TO_BASE");

BEGIN_MESSAGE_MAP(CTicketBoxExchangeSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	/*ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F2,OnKeyboardF2)*/
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
///**
//@brief      构造函数
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxExchangeSvc::CTicketBoxExchangeSvc():CTVMForeService(CARD_BOX_MANAGE_SVC)
{
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG, new CTicketBoxInfoDlg(this));

    // 设置乘客画面

    // 设置画面分组
	m_pDialogFlow->AddDialogGroup(DLG_BASE, _opl(GUIDE_ENTER_ESC));			// 选择安装卸载业务
    m_pDialogFlow->AddDialogGroupMember(DLG_BASE, CTicketBoxInfoDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DLG_INSTALL, _opl(GUIDE_ENTER_F2_F3_ESC));		// 安装业务
	m_pDialogFlow->AddDialogGroupMember(DLG_INSTALL, CTicketBoxInfoDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DLG_REMOVE, _opl(GUIDE_ENTER_F2_F3_ESC));		// 卸载业务
	m_pDialogFlow->AddDialogGroupMember(DLG_REMOVE, CTicketBoxInfoDlg::IDD);

	m_pDialogFlow->AddFlow(DLG_BASE,	WM_BASE_TO_INSTALL,	DLG_INSTALL);	// 主界面->安装
	m_pDialogFlow->AddFlow(DLG_INSTALL, WM_INSTALL_TO_BASE,	DLG_BASE);	// <-
	m_pDialogFlow->AddFlow(DLG_BASE,	WM_BASE_TO_REMOVE,	DLG_REMOVE);	// 主界面 -> 卸载
	m_pDialogFlow->AddFlow(DLG_REMOVE,	WM_REMOVE_TO_BASE,	DLG_BASE );	// <-

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DLG_BASE);

	m_CurrentPhase = PHASE_CHOOSE_BOX;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      析构函数
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxExchangeSvc::~CTicketBoxExchangeSvc()
{
}

void CTicketBoxExchangeSvc::OnStart()
{
	__super::OnStart();
	// 每次OnStart的时候需重新获取一下票箱的实际数据
	//InitCardBoxData();
	InitSvcData();
	

	m_CurrentPhase = PHASE_CHOOSE_BOX;

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DLG_BASE);
	m_pDialogFlow->ShowFirstDialogGroup();
	m_buffer_result =0;

}

CTicketBoxExchangeSvc::DATA_MODEL& CTicketBoxExchangeSvc::getModel()
{
	return m_dataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印票箱更换凭条

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::PrintReceipt(int nType)
{
	try
	{
		CTCardCountInfo::CARD_BOX_INFO BoxA = theCARD_COUNT.GetCardboxAInfo();
		CTCardCountInfo::CARD_BOX_INFO BoxB = theCARD_COUNT.GetCardboxBInfo();
		CTCardCountInfo::CARD_BOX_INFO BoxC = theCARD_COUNT.GetCardboxWastedInfo();

		
		CString CoinBox_templatename = _T("TVM_CARD_RECEIPT.template");
		CStringArray printtext;
		CStringArray TemplateContents;

		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

		int Index = 0;
		CString strCount;

		CString strOP = _T("");
		if (nType == CTicketBoxInfoDlg::UE_UNINSTALL){
			theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_UNINSTALL);
			strOP = _T("卸载");
		}
		else{
			theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_INSTALL);
			strOP = _T("安装");
		}

		int nHalfCount = TemplateContents.GetCount() / 2 - 2;

		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			
			line.Replace(_T("{操作}"),strOP);																				// 操作类型



			// 操作前 ID
			line.Replace(_T("{前ID1}"),m_Ex_BoxA.ticketBoxID.ToString());
			line.Replace(_T("{前ID2}"),m_Ex_BoxB.ticketBoxID.ToString());
			line.Replace(_T("{前ID3}"),m_Ex_INLD.ticketBoxID.ToString());
			

			// 操作前 数量
			line.Replace(_T("{前数量1}"),ComFormatCount(m_Ex_BoxA.ulCurCount ));
			line.Replace(_T("{前数量2}"),ComFormatCount(m_Ex_BoxB.ulCurCount ));
			line.Replace(_T("{前数量3}"),ComFormatCount(m_Ex_INLD.ulCurCount ));
			
			line.Replace(_T("{前合计}"),ComFormatCount(m_Ex_BoxA.ulCurCount + m_Ex_BoxB.ulCurCount + m_Ex_INLD.ulCurCount));

			// 操作后 
			if(Index > nHalfCount){
				if(m_Ex_BoxA.ticketBoxID.ToString() != BoxA.ticketBoxID.ToString() || m_Ex_BoxA.ulCurCount != BoxA.ulCurCount){
					line.Replace(_T("票箱A"),_T("*票箱A"));
				}

				if(m_Ex_BoxB.ticketBoxID.ToString() != BoxB.ticketBoxID.ToString() || m_Ex_BoxB.ulCurCount != BoxB.ulCurCount){
					line.Replace(_T("票箱B"),_T("*票箱B"));
				}

				if(m_Ex_INLD.ticketBoxID.ToString() != BoxC.ticketBoxID.ToString() || m_Ex_INLD.ulCurCount != BoxC.ulCurCount){
					line.Replace(_T("废票箱"),_T("*废票箱"));
				}
			}


			// 操作后 ID
			// 操作前 ID
			line.Replace(_T("{后ID1}"),BoxA.ticketBoxID.ToString());
			line.Replace(_T("{后ID2}"),BoxB.ticketBoxID.ToString());
			line.Replace(_T("{后ID3}"),BoxC.ticketBoxID.ToString());
							  
							  
			// 操作前 数量	 h后
			line.Replace(_T("{后数量1}"),ComFormatCount(BoxA.ulCurCount ));
			line.Replace(_T("{后数量2}"),ComFormatCount(BoxB.ulCurCount ));
			line.Replace(_T("{后数量3}"),ComFormatCount(BoxC.ulCurCount ));

			line.Replace(_T("{后合计}"),ComFormatCount(BoxA.ulCurCount + BoxB.ulCurCount + BoxC.ulCurCount));


			line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_dataModel.bIsException = true;
		//throw;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_dataModel.bIsException = true;
	}

	//try {
	//	CTicketBoxInfoDlg::UE_CURSTATUS uestatus = (CTicketBoxInfoDlg::UE_CURSTATUS)nType; // 因为svc和dlg的头文件相互包含 引起冲突
	//	// 加载模板
	//	CString templateName= _T("TVM_CARD_RECEIPT.template");
	//	CStringArray printTexts;
	//	CStringArray templateTexts;
	//	theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

	//	CString szLastCardBoxID;
	//	CString szLastCardBoxQuantity;
	//	CString szLastWastedID;
	//	CString szLastWastedQuantity;
	//	CString szCardBoxID;
	//	CString szCardBoxQuantity;
	//	CString szWastedID;
	//	CString szWastedQuantity;
	//	CTicketBoxInfoDlg* pdlgnotice=dynamic_cast<CTicketBoxInfoDlg*>(m_pDialogFlow->GetDialog(IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG));
	//	if(NULL!=pdlgnotice)
	//	{
	//		szLastCardBoxID=pdlgnotice->m_szLastCardBoxID;
	//		szLastCardBoxQuantity=pdlgnotice->m_szLastCardQuantity;
	//		szLastWastedID=pdlgnotice->m_szLasetWastedID;
	//		szLastWastedQuantity=pdlgnotice->m_szLastWastedQuantity;
	//		szCardBoxID=m_usinstallinfo.szID;
	//		szCardBoxQuantity=m_usinstallinfo.szQuantity;
	//		szWastedID=m_usinstallinfo.szWastedID;
	//		szWastedQuantity=_T("0");
	//	}

	//	// 填充数据
	//	int index = 0;
	//	while (index < templateTexts.GetCount()) {
	//		CString& line = templateTexts.ElementAt(index);

	//		CString szOperateInfoCH=_T("安装");
	//		CString szOperateInfoEN=_T("Install");
	//		CString szOperateInfoPN=_T("Install");
	//		switch(uestatus)
	//		{
	//		case CTicketBoxInfoDlg::UE_INSTALL:
	//			szOperateInfoCH=_T("安装");
	//			szOperateInfoEN=_T("Install");
	//			szOperateInfoPN=_T("Install");
	//			break;
	//		case CTicketBoxInfoDlg::UE_UNINSTALL:
	//			szOperateInfoCH=_T("卸载");
	//			szOperateInfoEN=_T("Uninstall");
	//			szOperateInfoPN=_T("Uninstall");
	//			break;
	//		}
	//		line.Replace(_T("{操作}"),szOperateInfoCH);
	//		line.Replace(_T("{Operation}"),szOperateInfoEN);
	//		line.Replace(_T("{Substituição}"),szOperateInfoPN);
	//		line.Replace(_T("{票箱1ID}"),szLastCardBoxID);
	//		line.Replace(_T("{票箱1數量}"),szLastCardBoxQuantity);
	//		line.Replace(_T("{廢票箱ID}"),szLastWastedID);
	//		line.Replace(_T("{廢票箱數量}"),szLastWastedQuantity);

	//		BOOL bReplaced=FALSE;
	//		int ireplace=line.Replace(_T("{*票箱1ID}"),szCardBoxID);
	//		if(ireplace!=0 && szCardBoxID!=szLastCardBoxID)
	//		{
	//			line=_T("*")+line;
	//			bReplaced=TRUE;
	//		}
	//		ireplace=0;

	//		ireplace=line.Replace(_T("{*票箱1數量}"),szCardBoxQuantity);
	//		if(ireplace!=0 && szCardBoxQuantity!=szLastCardBoxQuantity)
	//		{
	//			if(bReplaced==FALSE)
	//			{
	//				line=_T("*")+line;
	//			}
	//		}
	//		ireplace=0;

	//		bReplaced=FALSE;
	//		ireplace=line.Replace(_T("{*廢票箱ID}"),szWastedID);
	//		if(ireplace!=0 && szWastedID!=szLastWastedID)
	//		{
	//			line=_T("*")+line;
	//			bReplaced=TRUE;
	//		}
	//		ireplace=0;

	//		ireplace=line.Replace(_T("{*廢票箱數量}"),szWastedQuantity);
	//		if(ireplace!=0 && szWastedQuantity!=szLastWastedQuantity)
	//		{
	//			if(bReplaced==FALSE)
	//			{
	//				line=_T("*")+line;
	//			}
	//		}
	//		ireplace=0;


	//		line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
	//		line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
	//		line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
	//		line.Replace(_T("{Station Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
	//		line.Replace(_T("{Nome da estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
	//		line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

	//		// 打印时字符串的格式化处理（模板读取数据）
	//		CStringArray printLines;
	//		ComPrintLineFormatForTemplate(line,printLines);
	//		printTexts.Append(printLines);
	//		++index;
	//	}

	//	// 打印帐票
	//	PRINTER_HELPER->PrintReceiptByBmp(&printTexts,true,true);
	//}
	//catch (CSysException& e){
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
}

void CTicketBoxExchangeSvc::ShowTips(OPERATE_BOX box)
{
	CString strTips = _T("");
	if (box == TICKET_BOX_A){
		strTips = _opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_A_COUNT);
	}
	if (box == TICKET_BOX_B){
		strTips = _opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_B_COUNT);
	}
	if (box == INVALID_BOX){
		strTips = _opl(GUIDE_ENTER_F2_F3_ESC);
	}
	theAPP_SESSION.ShowOfficeGuide(strTips);
}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      按下回车键的消息响应函数
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxExchangeSvc::OnKeyboardEnter(WPARAM wParam, LPARAM lParam)
{
	// Has exception 
	UINT dlg = GetCurDialogGroupID();
	switch(dlg)
	{
	case DLG_BASE:
		{
			int nInput = theAPP_SESSION.GetInputToInt();
			if (nInput < 1 || nInput >2){
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
				return 0;
			}else{

				ClearStk();
				
				if (nInput == 1){
					DoDialogFlow(WM_BASE_TO_INSTALL);	// 流向安装界面
				}else{
					DoDialogFlow(WM_BASE_TO_REMOVE);	// 流向卸载界面
				}
				//InitCardBoxData();
			}
		}break;
	case DLG_INSTALL:
		{
			switch(m_CurrentPhase)		// 取目前所处阶段
			{
			case PHASE_CHOOSE_BOX:		// 选择票箱阶段
				{
					int nMenu = theAPP_SESSION.GetInputToInt();
					if(nMenu < 0 || nMenu > 4){
						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
						return FALSE;
					}
					OPERATE_BOX box;
					if (theAPP_SESSION.GetInputToInt() == 4){// 安装操作中 4 对应的是安装所有票箱
						box = ALL_BOX;
					}
					else{
						box = (OPERATE_BOX)theAPP_SESSION.GetInputToInt();
					}
					
					if (isBoxCouldBeOperated(box)){		// 可以安装
						// 读取箱子ID 显示出来
						long lRet = ReadBoxDataAndShow(box);

						// 正常返回
						if(0 == lRet){
							if (box != ALL_BOX ){	// 安装单个箱子
								if (box!= INVALID_BOX){
									CString strGuide = _T("");
									if (box==TICKET_BOX_A)
									{
										strGuide.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_A_COUNT));
									} else {
										strGuide.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_B_COUNT));
									}
									theAPP_SESSION.SetPrefixToGuide(strGuide);
									m_stkOpBox.push(box);
									theAPP_SESSION.SetMaxInputLength(3);
								}
								else{
									setCountAndState(INVALID_BOX,_T(""));
									theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
									m_CurrentPhase = PHASE_CHOOSE_BOX;
									theAPP_SESSION.SetMaxInputLength(1);
									return TRUE;
								}
							}
							else{// 安装所有箱子
								// 判断可以操作的箱子
								if(m_dataModel.m_bInvalidBoxCanOperator){
									m_stkOpBox.push((OPERATE_BOX)INVALID_BOX);
								}
								if(m_dataModel.m_bBoxBCanOperator){
									m_stkOpBox.push((OPERATE_BOX)TICKET_BOX_B);
								}
								if(m_dataModel.m_bBoxACanOperator){
									m_stkOpBox.push((OPERATE_BOX)TICKET_BOX_A);
								}

								if(m_stkOpBox.size() > 0){
									if(m_stkOpBox.top() == INVALID_BOX){
										setCountAndState(INVALID_BOX,_T(""));
										theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
										m_CurrentPhase = PHASE_CHOOSE_BOX;
										theAPP_SESSION.SetMaxInputLength(1);
										return TRUE;
									}
									else{
										CString strTips = _T("");
										if (m_stkOpBox.top()==TICKET_BOX_A)
										{
											strTips.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_A_COUNT));
										} else {
											strTips.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_B_COUNT));
										}
										theAPP_SESSION.SetPrefixToGuide(strTips);
										theAPP_SESSION.SetMaxInputLength(3);
									}
								}
							}
							m_CurrentPhase = PHASE_SET_COUNT;
						}
					}
					else{
						//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_ALREADY)/*T("所选的票箱不可安装")*/);
					}
				}
				break;
			case PHASE_SET_COUNT:
				{
					//  如果待操作的箱子队列非空
					if (!m_stkOpBox.empty()){						// 检查是否仍存在其他的待设置的箱子 如果存在
						CString strCount = theAPP_SESSION.GetInputToString();
						setCountAndState(m_stkOpBox.top(),strCount);	// 从容器中取出第一个已选定的箱子，设置其状态和数量
						m_stkOpBox.pop();								// 将已经设置过的箱子弹出
						if (!m_stkOpBox.empty()){
							if(m_stkOpBox.top() != INVALID_BOX){		// 而且不是费票箱，因为费票箱不需要输入数量
								CString strTip = _T("");
								if (m_stkOpBox.top()==TICKET_BOX_A)
								{
									strTip.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_A_COUNT));
								} else {
									strTip.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_B_COUNT));
								}
								theAPP_SESSION.SetPrefixToGuide(strTip);
								m_CurrentPhase = PHASE_SET_COUNT;		// 继续进入设置数量阶段
								theAPP_SESSION.SetMaxInputLength(3);
							}
							else{
								setCountAndState(m_stkOpBox.top(),_T(""));
								m_stkOpBox.pop();						// 再次弹出 ，跳过废票箱的设置
								if (m_stkOpBox.empty()){
									m_CurrentPhase = PHASE_CHOOSE_BOX;
									theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
									theAPP_SESSION.SetMaxInputLength(1);
								}
								else{
									m_CurrentPhase = PHASE_SET_COUNT;	// 进入选择欲操作的箱子阶段
									CString strTip = _T("");
									if (m_stkOpBox.top()==TICKET_BOX_A)
									{
										strTip.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_A_COUNT));
									} else {
										strTip.Format(_opl(TXT_SMARTCARDMGR_NEEDINPUTCOUNT_B_COUNT));
									}
									theAPP_SESSION.SetPrefixToGuide(strTip);
									theAPP_SESSION.SetMaxInputLength(3);
								}
							}
						}
						else{
							m_CurrentPhase = PHASE_CHOOSE_BOX;	// 进入选择欲操作的箱子阶段
							theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
							theAPP_SESSION.SetMaxInputLength(1);
						}
					}
					else{
						m_CurrentPhase = PHASE_CHOOSE_BOX;	// 进入选择欲操作的箱子阶段
						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
						theAPP_SESSION.SetMaxInputLength(1);
					}
				}
				break;
			}
		}break;
	case DLG_REMOVE:
		{
			int nMenu = theAPP_SESSION.GetInputToInt();
			if(nMenu < 0 || nMenu > 4){
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
				return FALSE;
			}
			theAPP_SESSION.SetMaxInputLength(1);
			// 卸载的过程：直接输入箱子对应的编号
			DoRemoval(theAPP_SESSION.GetInputToInt());
		}break;
	default:
		break;
	}
	NotifyDataModelChanged();
	return 0;
}

//LRESULT CTicketBoxExchangeSvc::OnKeyboardNumber(WPARAM wParam, LPARAM lparam)
//{
//	if (m_CurrentPhase == PHASE_SET_COUNT){
//		if (m_stkOpBox.top()!=INVALID_BOX)
//		{
//			CString strTips = _T("");
//			strTips.Format(_T("请输入票箱%c的票卡数量："), (m_stkOpBox.top()==TICKET_BOX_A) ? 'A' : 'B');
//			theAPP_SESSION.SetPrefixToGuide(strTips);
//		}
//	}
//	return 0;
//}

long checkCardBoxID(CString szID)
{
	long lret=CTicketBoxExchangeSvc::UE_INVALID_CARDBOXID;
	if(szID.GetLength()!=0)
	{
		lret=CTicketBoxExchangeSvc::UE_INVALID_CARDBOXID;
		BOOL bNum=TRUE;
		for(int iloop=0;iloop<szID.GetLength();iloop++)
		{
			if(_istdigit(szID[iloop])==0)
			{
				bNum=FALSE;
				lret=CTicketBoxExchangeSvc::UE_INVALID_CARDBOXID;
				break;
			}else
			{
			}
		}

		if(bNum==TRUE)
		{
			CString szLineCode=szID.Mid(0,2);
			if(szLineCode==_T("01"))
			{//lineCode 匹配
				CString szType=szID.Mid(2,2);
				if(szType==_T("96"))
				{
					lret=CTicketBoxExchangeSvc::UE_VALID;
				}else
				{
				}
			}else
			{
			}
		}else
		{
		}
	}else{
	}
	return lret;
}

long checkCardQuantity(CString szQuantity)
{
	long lret=CTicketBoxExchangeSvc::UE_INVALID_CARDQUANTITY;
	if(szQuantity.GetLength()!=0){
		lret=CTicketBoxExchangeSvc::UE_VALID;
		for(int iloop=0;iloop<szQuantity.GetLength();iloop++)
		{
			if(_istdigit(szQuantity[iloop])==0)
			{
				lret=CTicketBoxExchangeSvc::UE_INVALID_CARDQUANTITY;
				break;
			}
		}
	}else{
	}
	if(lret==CTicketBoxExchangeSvc::UE_VALID)
	{
		//数字检测通过 检测是否超过 1000张
		int inum=_ttoi(szQuantity);
		if(inum<0 || inum>1000)
		{
			lret=CTicketBoxExchangeSvc::UE_INVALID_CARDQUANTITY;
		}
	}else{
		//数字检测不通过
	}
	return lret;
}

long checkWastedID(CString szID)
{
	long lret=CTicketBoxExchangeSvc::UE_INVALID_WASTEDID;
	if(szID.GetLength()!=0){
		lret=CTicketBoxExchangeSvc::UE_VALID;
		for(int iloop=0;iloop<szID.GetLength();iloop++)
		{
			if(_istdigit(szID[iloop])==0)
			{
				lret=CTicketBoxExchangeSvc::UE_INVALID_WASTEDID;
				break;
			}
		}
	}else{
	}
	return lret;
}



////////////////////////////////////////////////////////////////////////////
///**
//@brief      安装票箱
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::UpdateCardBoxDataForInstall(OPERATE_BOX opBox)
{
	long lret=1;
	US_INSTALL_INFO box;
	TH_RSP_GET_DEV_STATUS status;
	bool isMagazineAExist=false;bool isMagazineBExist=false;bool isMagazineCExist = false;
	
	// 再次判断箱子是否在位
	try{
		CARD_HELPER->CheckTicketBoxExist(&status,isMagazineAExist,isMagazineBExist,isMagazineCExist);	// 检查箱子在位情况
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		return lret;
	}
	catch(...){
		return lret;
	}
	
	bool isBoxExist = false;
	TH_BOX_TYPE thBoxType;
	TH_BOX_TYPE cardBoxType;
	tTIMRFIDInfo timRfidInfo;
	if (opBox == TICKET_BOX_A)
	{
		isBoxExist = isMagazineAExist;
		box = m_dataModel.BoxAinfo;
		thBoxType = TH_BOX_TYPE::TH_BOX_A;
		cardBoxType = TH_BOX_A;
	}
	if (opBox == TICKET_BOX_B)
	{
		isBoxExist = isMagazineBExist;
		box = m_dataModel.BoxBinfo;
		thBoxType = TH_BOX_TYPE::TH_BOX_B;
		cardBoxType = TH_BOX_B;
	}
		try
		{
			if(isBoxExist)  // 箱子在位
			{
				int iquantity=0;
				
				// 再次读取RFID，判读是否与保存前一致
				try{
					bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,timRfidInfo);

					// 读取失败，直接返回
					if(!rst){
						return lret;
					}
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
					return lret;
				}
				catch(...){
					return lret;
				}

				// 箱子数量
				iquantity=_ttoi(box.szQuantity);

				// 箱子ID
				CTCardCountInfo::CARD_BOX_INFO cardBoxInfo;

				cardBoxInfo.ticketBoxID.Deserialize(timRfidInfo.ucBoxID);
				cardBoxInfo.ulCurCount = iquantity;

				BOOL bret = FALSE;
				if (opBox == TICKET_BOX_A){
					cardBoxInfo.ulInitCount = theCARD_COUNT.GetCardboxAInfo().ulInitCount;
					bret = theCARD_COUNT.SetCardboxAInfo(cardBoxInfo);
				}else if (opBox == TICKET_BOX_B){
					cardBoxInfo.ulInitCount = theCARD_COUNT.GetCardboxBInfo().ulInitCount;
					bret = theCARD_COUNT.SetCardboxBInfo(cardBoxInfo);
				}


				if(bret){
					//向服务器发送安装数据包
					MAGAZINE_CHANGE change;
					change.operType =  ATTACH;
					change.magazineId.Deserialize(timRfidInfo.ucBoxID);

					if (opBox == TICKET_BOX_A){
						change.magazne = MAGAZINEB;
					}
					else if (opBox == TICKET_BOX_B){
						change.magazne = MAGAZINEA;
					}

					change.cardType = ULTRALIGHT;
					change.providerId = 0x0002;		//ACC
					change.cardProductType = 0x6200;//单程票
					change.preProperty = ACC_INIT_CARD;
					change.childproductId = 0;
					change.CardCount = iquantity;
					change.magazineStatus = VALID_ATTACH;
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


					// 插入本地数据库
					CardBoxInstallOperationRecord cardInstall;
					cardInstall.dtOperationTime = ComGetCurTime();						// 操作时间
					cardInstall.iQuantity = iquantity;									// 数量
					cardInstall.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;	// 操作员编号
					cardInstall.txtCardBoxID  = cardBoxInfo.ticketBoxID.ToString();		// 箱子编号
					DATA_HELPER->DbInsertCardBoxInstallOperationRecord(cardInstall);

					// 写票卡硬计数
					try{
						tTIMCardNum timCardNum;
						timCardNum.uiCassetteACardNum = theCARD_COUNT.GetCardboxAInfo().ulCurCount;
						timCardNum.uiCassetteBCardNum = theCARD_COUNT.GetCardboxBInfo().ulCurCount;
						timCardNum.uiRejectCassetteNum = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;

						CARD_HELPER->SetTicketBoxCountInfo(cardBoxType,&timCardNum);
					}
					catch(CSysException& e){
						theEXCEPTION_MGR.ProcessException(e);
					}
					catch(...){
						// Do nothing
					}

					lret = 0;
				}
			}
		}catch(...)
		{
			return lret;
			// 异常提示后续需要加上
		}

	return lret;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      安装废票箱
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::installWastedCardBox()
{
	bool lret=false;
	CTicketBoxInfoDlg* pdlgnotice=dynamic_cast<CTicketBoxInfoDlg*>(m_pDialogFlow->GetDialog(IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG));
	if(NULL!=pdlgnotice)
	{
		int iquantity=0;
		CTCardCountInfo::CARD_BOX_INFO wastedBoxInfo;
		TH_BOX_TYPE thBoxType;
		tTIMRFIDInfo timRfidInfo;

		try{
			thBoxType = TH_BOX_TYPE::TH_BOX_C;
			bool bRet = CARD_HELPER->GetStaticBoxId(thBoxType,timRfidInfo);

			// 读取票箱ID失败
			if(!bRet){
				return bRet;
			}
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
			return false;
		}
		
		wastedBoxInfo.ticketBoxID.Deserialize(timRfidInfo.ucBoxID);
		wastedBoxInfo.ulInitCount = iquantity;

		lret =theCARD_COUNT.SetCardboxWastedInfo(wastedBoxInfo);

		if(lret){			
			//向服务器发送安装数据包
			MAGAZINE_CHANGE change;
			change.operType =  ATTACH;
			change.magazineId.Deserialize(timRfidInfo.ucBoxID);
						
			change.magazne = MAGAZINEC;

			change.cardType = ULTRALIGHT;
			change.providerId = 0x0002;		//ACC
			change.cardProductType = 0x6200;  //单程票
			change.preProperty = ACC_INIT_CARD;
			change.childproductId = 0;
			change.CardCount = iquantity;
			change.magazineStatus = VALID_ATTACH;
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
		}
		else{
			lret = false;
		}

	}
	return lret;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      卸载所有设备
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::UninstallDevices()
{
	long lret=1;
	if(UpdateCardBoxDataForRemove(TICKET_BOX_A)==0 || UpdateCardBoxDataForRemove(TICKET_BOX_B)==0 ||/*&&*/ uninstallWastedCardBox()==0){
		lret=0;
	}else{
	}	
	return lret;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      票箱卸载操作电文信息
//
//@param      (i)OPERATE_BOX opBox箱子类型
//
//@retval     (o)long lRet（成功）
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::UpdateCardBoxDataForRemove(OPERATE_BOX opBox)
{
	long lret=0;
	CTicketBoxInfoDlg* pdlgnotice=dynamic_cast<CTicketBoxInfoDlg*>(m_pDialogFlow->GetDialog(IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG));
	if(NULL!=pdlgnotice)
	{
		// 清空master文件数据之前，先取得卸载之前的票箱数据，插入数据库需要使用
		CTCardCountInfo::CARD_BOX_INFO boxInfo;
		TH_BOX_TYPE ticketBoxType;
		if (opBox == TICKET_BOX_A){
			boxInfo=theCARD_COUNT.GetCardboxAInfo();
			//更新数据模型信息
			m_dataModel.BoxAinfo = US_INSTALL_INFO();
			m_dataModel.BoxAinfo.state = REMOVED;
			m_dataModel.BoxAinfo.szID = INVALID_ID;
			ticketBoxType = TH_BOX_A;
		}
		else if (opBox == TICKET_BOX_B){
			boxInfo=theCARD_COUNT.GetCardboxBInfo();
			//更新数据模型信息
			m_dataModel.BoxBinfo = US_INSTALL_INFO();
			m_dataModel.BoxBinfo.state = REMOVED;
			m_dataModel.BoxBinfo.szID = INVALID_ID;
			ticketBoxType = TH_BOX_B;
		}
		// 卸载之前的票箱ID，为插数据库使用
		CString szIDBefore  = boxInfo.ticketBoxID.ToString();
		int iQuantityBefore = boxInfo.ulCurCount;

		// 实例化一个空的结构
		UINT uiLineCode	= 0xFF;
		UINT uiType		= 0xFF;
		UINT uiNum		= 0xFF;
		UINT uiStationId = 0xff;
		int	 iquantity=0;

		boxInfo.ticketBoxID.bStationID	= uiLineCode;
		boxInfo.ticketBoxID.bType		= uiType;
		boxInfo.ticketBoxID.bNum		= uiNum;
		boxInfo.ticketBoxID.bStationID = uiStationId;
		boxInfo.ulCurCount				= iquantity;

		// 开始清空数据
		BOOL bSetRight = FALSE;
		if (opBox == TICKET_BOX_A){
			bSetRight = theCARD_COUNT.SetCardboxAInfo(boxInfo);
			uiLineCode = m_Ex_BoxA.ticketBoxID.bDeviceType;
			uiType = m_Ex_BoxA.ticketBoxID.bType;
			uiNum  = m_Ex_BoxA.ticketBoxID.bNum;
			uiStationId = m_Ex_BoxA.ticketBoxID.bStationID;
		}
		else if (opBox == TICKET_BOX_B){
			bSetRight = theCARD_COUNT.SetCardboxBInfo(boxInfo);
			uiLineCode = m_Ex_BoxB.ticketBoxID.bDeviceType;
			uiType = m_Ex_BoxB.ticketBoxID.bType;
			uiNum  = m_Ex_BoxB.ticketBoxID.bNum;
			uiStationId = m_Ex_BoxB.ticketBoxID.bStationID;
		}
		
		if(bSetRight){
			//向服务器发送卸载数据包
			MAGAZINE_CHANGE change;
			change.operType =  DETACH;
			change.magazineId.bDeviceType = uiLineCode;//_ttoi(szIDBefore.Mid(0,2));
			change.magazineId.bType		= uiType;//_ttoi(szIDBefore.Mid(2,2));
			change.magazineId.bNum		= uiNum;//_ttoi(szIDBefore.Mid(4,4));
			change.magazineId.bStationID = uiStationId;

			if (opBox == TICKET_BOX_A){
				change.magazne = MAGAZINEB;
			}
			else if (opBox == TICKET_BOX_B){
				change.magazne = MAGAZINEA;
			}

			change.cardType = ULTRALIGHT;
			change.providerId = 0x0002;		//ACC
			change.cardProductType = 0x6200;  //单程票
			change.preProperty = ACC_INIT_CARD;
			change.childproductId = 0;
			change.CardCount = iQuantityBefore;
			change.magazineStatus = VALID_DETACH;
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


			// 插入本地数据库
			CardBoxRemoveOperationRecord removeCardInfo;
			removeCardInfo.dtOperationTime = ComGetCurTime();					// 操作时间
			removeCardInfo.txtCardBoxID = szIDBefore;							// 卸载前ID
			removeCardInfo.iOriginalQuantity = iQuantityBefore;					// 卸载前数量
			removeCardInfo.iRemoveQuantity = 0;									// 卸载后数量
			removeCardInfo.txtOperatorID=theAPP_SESSION.GetUserInfo().sUserID;	// 操作员ID
			DATA_HELPER->DbInsertCardBoxRemoveOperationRecord(removeCardInfo);

			// 写票卡硬计数
			try{
				tTIMCardNum ticketNum;
				ticketNum.uiCassetteACardNum = theCARD_COUNT.GetCardboxAInfo().ulCurCount;
				ticketNum.uiCassetteBCardNum = theCARD_COUNT.GetCardboxBInfo().ulCurCount;
				ticketNum.uiRejectCassetteNum = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;

				CARD_HELPER->SetTicketBoxCountInfo(ticketBoxType,&ticketNum);
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				// Do nothing
			}
		}
	}
	return lret;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      卸载废票箱
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::uninstallWastedCardBox()
{
	bool bRet = false;
	try{
		UINT uiLineCode	= 0xFF;
		UINT uiType		= 0xFF;
		UINT uiNum		= 0xFF;
		UINT uiStationID = 0XFF;
		int iquantity	= 0;
		int iBeforeQuantity = 0;
		CTCardCountInfo::CARD_BOX_INFO boxInfo;
		boxInfo = theCARD_COUNT.GetCardboxWastedInfo();
		iBeforeQuantity					= boxInfo.ulCurCount;
		CString szWastedIDBefore		= boxInfo.ticketBoxID.ToString();
		boxInfo.ticketBoxID.bDeviceType	= uiLineCode;
		boxInfo.ticketBoxID.bType		= uiType;
		boxInfo.ticketBoxID.bNum		= uiNum;
		boxInfo.ticketBoxID.bStationID = uiStationID;
		boxInfo.ulCurCount				= iquantity;

		bRet = theCARD_COUNT.SetCardboxWastedInfo(boxInfo);


		if(bRet){
			//向服务器发送卸载数据包
			MAGAZINE_CHANGE change;
			change.operType =  DETACH;
			change.magazineId.bDeviceType = m_Ex_INLD.ticketBoxID.bDeviceType;
			change.magazineId.bType		= m_Ex_INLD.ticketBoxID.bType;
			change.magazineId.bNum		= m_Ex_INLD.ticketBoxID.bNum;
			change.magazineId.bStationID = m_Ex_INLD.ticketBoxID.bStationID;

			change.magazne = MAGAZINEC;

			change.cardType = ULTRALIGHT;
			change.providerId = 0x0002;		//ACC
			change.cardProductType = 0x6200;  //单程票
			change.preProperty = ACC_INIT_CARD;
			change.childproductId = 0;
			change.CardCount = iquantity;
			change.magazineStatus = VALID_DETACH;
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


			// 插入数据库
			CardCollectBoxRemoveOperationRecord removeWastedInfo;
			removeWastedInfo.dtTransTime  = ComGetCurTime();
			removeWastedInfo.txtCardBoxID = szWastedIDBefore;
			removeWastedInfo.iQuantity    = iBeforeQuantity;
			removeWastedInfo.txtOperatorID= theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertCardCollectBoxRemoveOperationRecord(removeWastedInfo);
		}
	}
	catch(...){
		// 异常暂时不处理
	}
	
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxExchangeSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UINT dlg = GetCurDialogGroupID();
	InitSvcData();
	switch(dlg)
	{
	case DLG_BASE:
		theAPP_SESSION.ReturnMaintainMenu();
		break;
	case DLG_INSTALL:
		DoDialogFlow(WM_INSTALL_TO_BASE);
		break;
	case DLG_REMOVE:
		DoDialogFlow(WM_REMOVE_TO_BASE);
		break;
	default:
		theAPP_SESSION.ReturnMaintainMenu();
		break;
	}
	/*m_Ex_BoxA = theCARD_COUNT.GetCardboxAInfo();
	m_Ex_BoxB = theCARD_COUNT.GetCardboxBInfo();
	m_Ex_INLD = theCARD_COUNT.GetCardboxWastedInfo();*/

	m_CurrentPhase = PHASE_CHOOSE_BOX;
	isBufferSelected = false;
	m_buffer_result = 0;
	NotifyDataModelChanged();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      F2按键的消息响应函数
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxExchangeSvc::OnKeyboardF2(WPARAM wParam, LPARAM lParam)
{
	// Has exception 
	/*if(m_bHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_MODULEEXCEPTION));
		return FALSE;
	}*/
	
	CString strTips = _T("");
	UINT dlg_id = GetCurDialogGroupID();
	bool isPrint = false;
	switch(dlg_id)
	{
	case DLG_INSTALL:
		{
			if(m_dataModel.BoxAinfo.state != PRE_INSTALL && m_dataModel.BoxBinfo.state != PRE_INSTALL && m_dataModel.InvalidBox.state != PRE_INSTALL){
				return -1;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_INSTALL_WAIT));
			if (m_dataModel.BoxAinfo.state == PRE_INSTALL)
			{
				// 硬件操作
				if (CheckBoxStatus(TICKET_BOX_A))
				{
					long lRet = UpdateCardBoxDataForInstall(TICKET_BOX_A);
					
					if(0 == lRet){
						MoveTicketBox(TICKET_BOX_A,BOX_UP);
						strTips += _opl(TXT_SMARTCARDMGR_INSTALL_A_SUCCESS);
						strTips += _opl(GUIDE_F3_ESC);
						// 日志等数据的更新
						m_dataModel.BoxAinfo.state = INSTALLED;
						isPrint = true;
						//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,true); // 写日志
						theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
					}
					else{
						strTips += _opl(TXT_SMARTCARDMGR_INSTALL_A_FAILED);
						strTips += _opl(GUIDE_F3_ESC);
						//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,false); // 写日志
					}
				}
				else{
					strTips += _opl(TXT_SMARTCARDMGR_INSTALL_A_FAILED);
					strTips += _opl(GUIDE_F3_ESC);

					//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,false); // 写日志
				}
			}
			if (m_dataModel.BoxBinfo.state == PRE_INSTALL)
			{
				if (CheckBoxStatus(TICKET_BOX_B))
				{
					long lRet = UpdateCardBoxDataForInstall(TICKET_BOX_B);
					
					if(0 == lRet){
						MoveTicketBox(TICKET_BOX_B,BOX_UP);
						strTips += _opl(TXT_SMARTCARDMGR_INSTALL_B_SUCCESS);
						strTips += _opl(GUIDE_F3_ESC);

						m_dataModel.BoxBinfo.state = INSTALLED;

						isPrint = true;
						//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,true); // 写日志
						theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
					}
					else{
						strTips += _opl(TXT_SMARTCARDMGR_INSTALL_B_FAILED);
						strTips += _opl(GUIDE_F3_ESC);

						//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,false); // 写日志
					}					
				}
				else{
					strTips += _opl(TXT_SMARTCARDMGR_INSTALL_B_FAILED);
					strTips += _opl(GUIDE_F3_ESC);

					//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,false); // 写日志
				}

			}
			if (m_dataModel.InvalidBox.state == PRE_INSTALL)
			{
				if (installWastedCardBox()){
					m_dataModel.InvalidBox.state = INSTALLED;
					strTips += _opl(TXT_SMARTCARDMGR_INSTALL_C_SUCCESS);
					strTips += _opl(GUIDE_F3_ESC);

					isPrint = true;
					//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,true); // 写日志
					theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					strTips += _opl(TXT_SMARTCARDMGR_INSTALL_C_FAILED);
					strTips += _opl(GUIDE_F3_ESC);

					//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_INSTALLATION,false); // 写日志
				}
			}
			// 打印
			if(isPrint)
				PrintReceipt(CTicketBoxInfoDlg::UE_INSTALL);
		}break;
	case DLG_REMOVE:
		{
			// 需要清理缓冲区的票卡，再卸载废票箱
			if (isBufferSelected)
			{
				try
				{
					// 先判断缓冲区是否有票？
					TH_RSP_GET_DEV_STATUS rsp;
					CARD_HELPER->GetDevStatus(&rsp);

					if(rsp.bCardInReadArea){
						UINT ulRecycCount = 0;
						long lRet = CARD_HELPER->RetractCard(&ulRecycCount);

						if(TIM_ERR_CODE_NONE == lRet){
							m_buffer_result = 1;
							strTips += _T("缓冲区票卡清理成功;");

							// 最后回收的票需要计入废票箱中
							CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
							cardInfo.ulCurCount += ulRecycCount;
							theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

							// 现在为卸载前的数量
							m_Ex_INLD = cardInfo;

							// 需要生成废票交易
							// 需要生成废票交易
							INVALIDE_TICKET_RECYCLETRADE cardInvalid;
							// 保存废卡交易
							cardInvalid.reson = 0x02;// 正常票卡
							cardInvalid.cardPhysicalType = ULTRALIGHT;
							// 卡号
							//memset(cardInvalid.cardSerialNo, 0, sizeof(cardInvalid.cardSerialNo));
							cardInvalid.cardSerialNo = 0;
							// 发行商
							cardInvalid.issueId		 = 0x0002;//ACC
							// 废票箱ID
							//cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
							//cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
							//cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;
							//cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;
							//cardInvalid.productType		= 0x6200;

							theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
						}
						
					}
					else{
						strTips += _T("缓冲区无票卡清理;");

					}
				}
				catch(...){
					strTips += _T("缓冲区票卡清理失败;");

					m_buffer_result = 3;
				}
				isBufferSelected = false;
			}

			if(m_dataModel.BoxAinfo.state != PRE_REMOVE && m_dataModel.BoxBinfo.state != PRE_REMOVE && m_dataModel.InvalidBox.state != PRE_REMOVE){
				break;//return -1;
			}
			theAPP_SESSION.ShowOfficeGuide(_T("票箱卸载中..."));
			
			if (m_dataModel.BoxAinfo.state == PRE_REMOVE)			// 如果是待卸载
			{
				if (CheckBoxStatus(TICKET_BOX_A))
				{
					
					isPrint = true;
					UpdateCardBoxDataForRemove(TICKET_BOX_A);

					if(MoveTicketBox(TICKET_BOX_A,BOX_DOWN))
					{
						strTips += _T("票箱A卸载成功；");

						m_dataModel.BoxAinfo.szQuantity = _T("0");
					}	
					else
						strTips += _T("票箱A卸载失败；");


					theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
				}else{
					strTips += _T("票箱A卸载失败；");

				}
			}
			if (m_dataModel.BoxBinfo.state == PRE_REMOVE)
			{
				if (CheckBoxStatus(TICKET_BOX_B))
				{
					
					isPrint = true;
					UpdateCardBoxDataForRemove(TICKET_BOX_B);
					if(MoveTicketBox(TICKET_BOX_B,BOX_DOWN))
					{
						m_dataModel.BoxBinfo.szQuantity = _T("0");
						strTips += _T("票箱B卸载成功；");
					}
					else
						strTips += _T("票箱B卸载失败；");
					theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
				}
				else{
					strTips += _T("票箱B卸载失败；");
				}
			}

			// 卸载废票箱
			if (m_dataModel.InvalidBox.state == PRE_REMOVE)
			{
				if(uninstallWastedCardBox() == 1){
					m_dataModel.InvalidBox.szQuantity = _T("0");
					strTips += _T("废票箱卸载成功；");
					isPrint = true;
					theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
				}
				else{
					strTips += _T("废票箱卸载失败；");
				}
				m_dataModel.InvalidBox.state = REMOVED;
				m_dataModel.InvalidBox.szID = INVALID_ID;
			}
			strTips += _opl(GUIDE_F3_ESC);

			if (isPrint)
			{
				PrintReceipt(CTicketBoxInfoDlg::UE_UNINSTALL);
			}
			
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);
		}break;
	default:
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(strTips);
	Sleep(1000);
	// 提示打印失败
	if(m_dataModel.bIsException && !strTips.IsEmpty() && isPrint){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	}

	// 需要初始化数据
	m_Ex_BoxA = theCARD_COUNT.GetCardboxAInfo();
	m_Ex_BoxB = theCARD_COUNT.GetCardboxBInfo();
	m_Ex_INLD = theCARD_COUNT.GetCardboxWastedInfo();

	NotifyDataModelChanged();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      检查输入的票箱ID是否合法
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::checkInputCardBoxID(CString szID)
{
	long lret=1;
	lret=checkCardBoxID(szID);	
	return lret;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      检查输入的票箱票卡数量是否合法
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::checkInputCardBoxQuantity(CString szQuantity)
{
	long lret=1;
	lret=checkCardQuantity(szQuantity);
	return lret;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      检查输入的废票箱是否合法
//
//@param      (i)无
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::checkInputWastedID(CString szID)
{
	long lret=1;
	lret=checkWastedID(szID);
	return lret;
}

void CTicketBoxExchangeSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}


bool CTicketBoxExchangeSvc::isBoxCouldBeOperated(OPERATE_BOX opBox)
{
	try{
		// 检查硬件连接情况
		UINT ueCurStatus = GetCurDialogGroupID();
		bool admission = false;

		TH_RSP_GET_DEV_STATUS status;
		bool isBoxAExist = false;
		bool isBoxBExist = false;
		bool isBoxCExist = false;

		m_dataModel.m_bInvalidBoxCanOperator = false;
		m_dataModel.m_bBoxACanOperator = false;
		m_dataModel.m_bBoxBCanOperator = false;
		CARD_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist ,isBoxCExist);

		//CARD_HELPER-> ReadStatus(status);
		switch(opBox)
		{
		case TICKET_BOX_A:  
			{
				if (isBoxAExist ){ // 箱子在位
					// 未安装的箱子可安装 未卸载的箱子可卸载
					if(((ueCurStatus == DLG_INSTALL) && (m_dataModel.BoxAinfo.state != INSTALLED) ) ||
						((ueCurStatus ==DLG_REMOVE) && (m_dataModel.BoxAinfo.state != REMOVED )) ){
							admission = true;
					}else{
						//admission = false;
						ueCurStatus == DLG_INSTALL ? theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL)): theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL));
					}
				}else{
					//admission = false;
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_NOT_STANDBY));
				}
			}break;
		case TICKET_BOX_B: 
			{
				if (isBoxBExist){ // 箱子在位
					// 未安装的箱子可安装 未卸载的箱子可卸载
					if(((ueCurStatus == DLG_INSTALL) && (m_dataModel.BoxBinfo.state != INSTALLED) ) ||
						((ueCurStatus == DLG_REMOVE) && (m_dataModel.BoxBinfo.state != REMOVED) ) ){
							admission = true;
					}else{
						ueCurStatus == DLG_INSTALL ? theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL)): theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL));
						//admission = false;
					}
				}else{
					//admission = false;
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_NOT_STANDBY));
				}
			}break;

		case INVALID_BOX:	
			{
				// 未安装的箱子可安装 未卸载的箱子可卸载
				if( isBoxCExist ){
					if((ueCurStatus == DLG_INSTALL && m_dataModel.InvalidBox.state != INSTALLED ) ||
					   (ueCurStatus == DLG_REMOVE) && m_dataModel.InvalidBox.state != REMOVED ) {
						admission = true;
					}
					else{
						ueCurStatus == DLG_INSTALL ? theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL)): theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL));
						//admission = false;
					}
				}
				else {
					//admission = false;
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_INVALID_BOX_NOT_STANDBY));
				}
					
			}break;
		case ALL_BOX:
			{
				admission = false;

				// 票箱A
				if(isBoxAExist){
					if(((ueCurStatus == DLG_INSTALL) && (m_dataModel.BoxAinfo.state != INSTALLED) ) ||
						((ueCurStatus ==DLG_REMOVE) && (m_dataModel.BoxAinfo.state != REMOVED )) ){
							admission = true;
							m_dataModel.m_bBoxACanOperator = true;
					}
				}
				

				// 票箱B
				if(isBoxBExist){
					if(((ueCurStatus == DLG_INSTALL) && (m_dataModel.BoxBinfo.state != INSTALLED) ) ||
						((ueCurStatus == DLG_REMOVE) && (m_dataModel.BoxBinfo.state != REMOVED) ) ){
							admission = true;
							m_dataModel.m_bBoxBCanOperator = true;
					}
				}
				

				// 废票箱
				if(isBoxCExist){
					if((ueCurStatus == DLG_INSTALL && m_dataModel.InvalidBox.state != INSTALLED ) ||
						(ueCurStatus == DLG_REMOVE) && m_dataModel.InvalidBox.state != REMOVED ) {
							admission = true;
							m_dataModel.m_bInvalidBoxCanOperator = true;
					}
				}				

				if(!admission){
					if(isBoxAExist || isBoxBExist || isBoxCExist){
						ueCurStatus == DLG_INSTALL ? theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL)): theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CARD_BOX_ALEARDY_INSTALL));
					}
					else{
						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_CAN_NOT_FOUND_BOX));
					}
				}
				
				/*if (isBoxCouldBeOperated(TICKET_BOX_A) || isBoxCouldBeOperated(TICKET_BOX_B) || isBoxCouldBeOperated(INVALID_BOX))
				{
					admission = true;
				}else{
					admission = false;
				}*/
			}
			break;

		default: 
			break;

		}

		return admission;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}


// 设置票箱的票卡数量 和 状态
void CTicketBoxExchangeSvc::setCountAndState(OPERATE_BOX box, CString strCount)
{
	switch(box/*m_choosed*/)
	{
	case TICKET_BOX_A: 
		m_dataModel.BoxAinfo.szQuantity= strCount;
		m_dataModel.BoxAinfo.state = PRE_INSTALL;
		break;
	case TICKET_BOX_B:
		m_dataModel.BoxBinfo.szQuantity= strCount;
		m_dataModel.BoxBinfo.state = PRE_INSTALL;
		break;
	case  INVALID_BOX:
		m_dataModel.InvalidBox.state = PRE_INSTALL;
		break;
	default:
		break;
	}
}

// 处理卸载操作
void CTicketBoxExchangeSvc::DoRemoval(int nInput)
{
	OPERATE_BOX box = (OPERATE_BOX)nInput;
	bool bShowErrMsg = false;
	switch(box)
	{
	case TICKET_BOX_A: 
		{
			if (isBoxCouldBeOperated(TICKET_BOX_A)){ // 如果箱子可操作
				m_dataModel.BoxAinfo.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.BoxAinfo.szID  = _T("FFFFFFFF");
			}
			else{
				bShowErrMsg = true;
			}
		}
		break;
	case TICKET_BOX_B:
		{
			if (isBoxCouldBeOperated(TICKET_BOX_B)){ // 如果箱子可操作
				m_dataModel.BoxBinfo.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.BoxBinfo.szID  = _T("FFFFFFFF");
			}
			else{
				bShowErrMsg = true;
			}
		}
		break;
	case INVALID_BOX:
		{
			if (isBoxCouldBeOperated(INVALID_BOX)){ // 费票箱不用考虑硬件操作
				m_dataModel.InvalidBox.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.InvalidBox.szID  = _T("FFFFFFFF");
			}
			else{
				bShowErrMsg = true;
			}
		}
		break;
	case ALL_BOX:
		{
			/*DoRemoval(TICKET_BOX_A);
			DoRemoval(TICKET_BOX_B);
			DoRemoval(INVALID_BOX);*/
			bShowErrMsg = true;
			if(isBoxCouldBeOperated(TICKET_BOX_A)){
				m_dataModel.BoxAinfo.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.BoxAinfo.szID  = _T("FFFFFFFF");
				bShowErrMsg = false;
			}

			if(isBoxCouldBeOperated(TICKET_BOX_B)){
				m_dataModel.BoxBinfo.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.BoxBinfo.szID  = _T("FFFFFFFF");
				bShowErrMsg = false;
			}

			if(isBoxCouldBeOperated(INVALID_BOX)){
				m_dataModel.InvalidBox.state = PRE_REMOVE;	// 将箱子的状态设为待卸载
				//m_dataModel.InvalidBox.szID  = _T("FFFFFFFF");
				bShowErrMsg = false;
			}
			//isBufferSelected = true;
		}
		break;
		/*case BUFFER:
		{
		isBufferSelected = true;
		m_buffer_result = 2;
		}break;*/
	default: 
		break;
	}

	// 显示错误提示
	if(bShowErrMsg){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_ALREADY));// 箱子已经卸载，请勿重复卸载
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}

// 初始化业务数据
void CTicketBoxExchangeSvc::InitSvcData()
{
	CTCardCountInfo::CARD_BOX_INFO card_box_A = theCARD_COUNT.GetCardboxAInfo();
	CTCardCountInfo::CARD_BOX_INFO card_box_B = theCARD_COUNT.GetCardboxBInfo();
	CTCardCountInfo::CARD_BOX_INFO card_box_C = theCARD_COUNT.GetCardboxWastedInfo();
	// 票箱A
	m_dataModel.BoxAinfo.szID = card_box_A.ticketBoxID.ToString();
	m_dataModel.BoxAinfo.szQuantity = card_box_A.ulCurCount != 0 ? ComFormatCount(card_box_A.ulCurCount) : _T("0");
	if (m_dataModel.BoxAinfo.szID == _T("FFFFFFFF") || m_dataModel.BoxAinfo.szID == _T("00000000")){
		m_dataModel.BoxAinfo.state = REMOVED;
	}else{
		m_dataModel.BoxAinfo.state = INSTALLED;
	}
	// 票箱B
	m_dataModel.BoxBinfo.szID = card_box_B.ticketBoxID.ToString();
	m_dataModel.BoxBinfo.szQuantity = card_box_B.ulCurCount != 0 ? ComFormatCount(card_box_B.ulCurCount) : _T("0");
	if (m_dataModel.BoxBinfo.szID == _T("FFFFFFFF") || m_dataModel.BoxBinfo.szID == _T("00000000")){
		m_dataModel.BoxBinfo.state = REMOVED;
	}else{
		m_dataModel.BoxBinfo.state = INSTALLED;
	}
	// 废票箱
	m_dataModel.InvalidBox.szID = card_box_C.ticketBoxID.ToString();
	m_dataModel.InvalidBox.szQuantity = card_box_C.ulCurCount != 0 ? ComFormatCount(card_box_C.ulCurCount) : _T("0");
	if (m_dataModel.InvalidBox.szID == _T("FFFFFFFF") || m_dataModel.InvalidBox.szID == _T("00000000")){
		m_dataModel.InvalidBox.state = REMOVED;
	}else{
		m_dataModel.InvalidBox.state = INSTALLED;
	}

	isBufferSelected = false;

	m_Ex_BoxA = theCARD_COUNT.GetCardboxAInfo();
	m_Ex_BoxB = theCARD_COUNT.GetCardboxBInfo();
	m_Ex_INLD = theCARD_COUNT.GetCardboxWastedInfo();

	m_dataModel.bIsException = false;
}

// 票箱数据更新
void CTicketBoxExchangeSvc::InitCardBoxData()
{
	//TH_RSP_GET_DEV_STATUS status;
	//bool isBoxAExist = false;
	//bool isBoxBExist = false;
	//bool isBoxCExist = false;
	//try{
	//	CARD_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist ,isBoxCExist);
	//	CTCardCountInfo::CARD_BOX_INFO boxAinfo = theCARD_COUNT.GetCardboxAInfo();
	//	US_INSTALL_INFO boxInfo;
	//	// 重置票箱A数据
	//	CTCardCountInfo::CARD_BOX_ID id = boxAinfo.ticketBoxID;
	//	if(id.wNum != 0xFFFF){// 如果Countinfo里面记录的是票箱是安装的
	//		boxInfo.state = INSTALLED;
	//		// 检查是否和硬件票箱ID相同
	//		if (isBoxAExist) // 如果票箱A在位
	//		{
	//			SC_RES_READ_RFID rfid;
	//			rfid.boxType = RFID_MAGAZINE_A;
	//			bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//			CTCardCountInfo::CARD_BOX_ID boxID;
	//			if (rst){
	//				boxID.Serialze(rfid.RFIDData.byDeviceID);
	//				if ( 0 != boxAinfo.ticketBoxID.ToString().Compare(boxID.ToString())) // 对比硬件票箱与CountInfo数据的票箱ID是否相同
	//				{
	//					// 非法安装
	//					// 需先卸载票箱再进行安装操作
	//					boxInfo.state = INVALID_AT;
	//					boxInfo.szID = boxAinfo.ticketBoxID.ToString();
	//				}
	//			}
	//		}else{
	//			// 非法卸载
	//			boxInfo.state = INVALID_DT;
	//			boxInfo.szID = boxAinfo.ticketBoxID.ToString();
	//		}

	//	}else{	// Countinfo 里面记录的是卸载状态
	//		boxInfo.state = REMOVED;
	//		// 如果发现硬件票箱
	//		SC_RES_READ_RFID rfid;
	//		rfid.boxType = RFID_MAGAZINE_A;
	//		bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//		CTCardCountInfo::CARD_BOX_ID boxID;
	//		if (rst&& (rfid.result == 16)){
	//			boxID.Deserialize(rfid.RFIDData.byDeviceID);
	//			boxInfo.szID = boxID.ToString();
	//		}
	//	}
	//	boxInfo.szQuantity.Format(_T("%d"),boxAinfo.ulCurCount);
	//	m_dataModel.BoxAinfo = boxInfo;

	//	// 重置票箱B数据
	//	boxInfo = US_INSTALL_INFO();
	//	CTCardCountInfo::CARD_BOX_INFO boxBinfo = theCARD_COUNT.GetCardboxBInfo();
	//	id = boxBinfo.ticketBoxID;
	//	if(id.wNum != 0xFFFF){// 如果Countinfo里面记录的是票箱是安装的
	//		boxInfo.state = INSTALLED;
	//		// 检查是否和硬件票箱ID相同
	//		if (isBoxBExist) // 如果票箱A在位
	//		{
	//			SC_RES_READ_RFID rfid;
	//			rfid.boxType = RFID_MAGAZINE_B;
	//			bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//			CTCardCountInfo::CARD_BOX_ID boxID;
	//			if (rst  &&(rfid.result == 0x10)){
	//				boxID.Deserialize(rfid.RFIDData.byDeviceID);
	//				if ( 0 != boxBinfo.ticketBoxID.ToString().Compare(boxID.ToString())) // 对比硬件票箱与CountInfo数据的票箱ID是否相同
	//				{
	//					// 非法安装
	//					boxInfo.state = INVALID_AT;
	//					// 需先卸载票箱再进行安装操作
	//					boxInfo.szID = boxBinfo.ticketBoxID.ToString();
	//				}
	//			}
	//		}else{
	//			// 非法卸载
	//			boxInfo.state = INVALID_DT;
	//			boxInfo.szID = boxBinfo.ticketBoxID.ToString();
	//		}

	//	}else{	// Countinfo 里面记录的是卸载状态
	//		boxInfo.state = REMOVED;
	//		// 如果发现硬件票箱
	//		SC_RES_READ_RFID rfid;
	//		rfid.boxType = RFID_MAGAZINE_B;
	//		bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//		CTCardCountInfo::CARD_BOX_ID boxID;
	//		if (rst && (rfid.result == 0x10)){	// 读取RFID成功
	//			boxID.Deserialize(rfid.RFIDData.byDeviceID);
	//			boxInfo.szID = boxID.ToString();
	//		}
	//	}
	//	boxInfo.szQuantity.Format(_T("%d"),theCARD_COUNT.GetCardboxBInfo().ulCurCount);
	//	m_dataModel.BoxBinfo = boxInfo;

	//	// 重置废票箱数据
	//	boxInfo = US_INSTALL_INFO();
	//	CTCardCountInfo::CARD_BOX_INFO boxCinfo = theCARD_COUNT.GetCardboxWastedInfo();
	//	id = boxCinfo.ticketBoxID;
	//	if(id.wNum != 0xFFFF){// 如果Countinfo里面记录的是票箱是安装的
	//		boxInfo.state = INSTALLED;
	//		// 检查是否和硬件票箱ID相同
	//		if (isBoxCExist) // 如果票箱A在位
	//		{
	//			SC_RES_READ_RFID rfid;
	//			rfid.boxType = RFID_MAGAZINE_C;
	//			bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//			CTCardCountInfo::CARD_BOX_ID boxID;
	//			if (rst){
	//				boxID.Serialze(rfid.RFIDData.byDeviceID);
	//				if ( 0 != boxCinfo.ticketBoxID.ToString().Compare(boxID.ToString())) // 对比硬件票箱与CountInfo数据的票箱ID是否相同
	//				{
	//					// 非法安装
	//					boxInfo.state = INVALID_AT;
	//					// 需先卸载票箱再进行安装操作
	//					boxInfo.szID = boxCinfo.ticketBoxID.ToString();
	//				}
	//			}
	//		}else{
	//			// 非法卸载
	//			boxInfo.state = INVALID_DT;
	//			boxInfo.szID = boxCinfo.ticketBoxID.ToString();
	//		}

	//	}else{	// Countinfo 里面记录的是卸载状态
	//		boxInfo.state = REMOVED;
	//		// 如果发现硬件票箱
	//		SC_RES_READ_RFID rfid;
	//		rfid.boxType = RFID_MAGAZINE_C;
	//		bool rst = CARD_HELPER->ReadTicketBoxRFID(rfid);
	//		CTCardCountInfo::CARD_BOX_ID boxID;
	//		if (rst &&(rfid.result == 0x10)){
	//			boxID.Deserialize(rfid.RFIDData.byDeviceID);
	//			boxInfo.szID = boxID.ToString();
	//		}
	//	}
	//	boxInfo.szQuantity.Format(_T("%d"),boxCinfo.ulCurCount);
	//	m_dataModel.InvalidBox = boxInfo;
	//}
	//catch(...)
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_T("票卡模块异常"));
	//}

}

// 操作票箱硬件
bool CTicketBoxExchangeSvc::HandleBox(TH_MOVE_BOX_CMD_INFO cmd)
{
	bool ret = false;
	UINT ueCurStatus = GetCurDialogGroupID();
	TH_RSP_GET_DEV_STATUS status;
	TH_REPLACE_BOX_CMD_INFO replaceSelect;
	try{
		//CARD_HELPER->GetDevStatus(&status);
		if (ueCurStatus == DLG_INSTALL)						// 安装票箱
		{
			if (cmd.boxId == 1) // A票箱
			{
				// 检查盖板
				//if (status.bBoxAArrive && status.bBoxACoverArrive)
				//{
					replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_A;//票箱物理位置与界面显示相反
					replaceSelect.actionType = TH_INSTALL;
					CARD_HELPER->ReplaceTicketBox(replaceSelect);
					ret = true;
				//}
				/*else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}*/
			}
			if (cmd.boxId == 2) // A票箱
			{
				// 检查盖板
				/*if (status.bBoxBArrive && status.bBoxBCoverArrive)
				{*/
					replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_B;
					replaceSelect.actionType = TH_INSTALL;
					CARD_HELPER->ReplaceTicketBox(replaceSelect);
					ret = true;
				/*}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}*/
			}

		}
		if (ueCurStatus == DLG_REMOVE)						// 卸载票箱
		{
			if (cmd.boxId == 1) // A票箱
			{
				// 检查盖板
				/*if (status.bBoxAArrive && status.bBoxACoverArrive)
				{*/
					replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_A;
					replaceSelect.actionType = TH_UNINSTALL;
					CARD_HELPER->ReplaceTicketBox(replaceSelect);
					ret = true;
				/*}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}*/
			}
			if (cmd.boxId == 2) // A票箱
			{
				// 检查盖板
				/*if (status.bBoxBArrive && status.bBoxBCoverArrive)
				{*/
					replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_B;
					replaceSelect.actionType = TH_UNINSTALL;
					CARD_HELPER->ReplaceTicketBox(replaceSelect);
					ret = true;
				/*}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}*/
			}
		}
		return ret;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_T("票箱模块异常。"));
		Sleep(2000);
		return false;
	}
}


bool CTicketBoxExchangeSvc::MoveTicketBox(OPERATE_BOX box, BOX_MOVAL drc)
{
	TH_MOVE_BOX_CMD_INFO cmd;
	if (box == TICKET_BOX_A){
		cmd.boxId = 1;
	}
	else if (box == TICKET_BOX_B){
		cmd.boxId = 2;
	}
		
	if (drc == BOX_UP){
		cmd.actionType = 1;
	}
	else if ( drc == BOX_DOWN){
		cmd.actionType = 2;
	}
			
	return HandleBox(cmd);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读取票箱id及数量信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTicketBoxExchangeSvc::ReadBoxDataAndShow(int index)
{
	long ret = 0;
	TH_BOX_TYPE thBoxType;
	tTIMRFIDInfo tTimRfidInfo;
	//SC_RES_READ_RFID RF_ID;
	try
	{

		switch(index)
		{
		case 1:
			{
				thBoxType = TH_BOX_TYPE::TH_BOX_A;
				bool rst = CARD_HELPER->GetStaticBoxId(TH_BOX_A,tTimRfidInfo);/*CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);*/
				if(!rst){
					theAPP_SESSION.ShowOfficeGuide(_T("读取票箱A编号出错，请检查票箱是否安装"));
					return 1;
				}
				

				CTCardCountInfo::CARD_BOX_ID boxID;
				boxID.Deserialize(tTimRfidInfo.ucBoxID);
				m_dataModel.BoxAinfo.szID = boxID.ToString();
				if(m_dataModel.BoxAinfo.szQuantity.IsEmpty()){
					m_dataModel.BoxAinfo.szQuantity.Format(_T("%d"),((tTimRfidInfo.ucCardNum[1] << 8 ) | tTimRfidInfo.ucCardNum[0]));
					m_dataModel.BoxAinfo.state = PRE_INSTALL;
				}
			}
			break;
		case 2:
			{
				thBoxType = TH_BOX_TYPE::TH_BOX_B;
				bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,tTimRfidInfo);;//CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);
				if(!rst){
					theAPP_SESSION.ShowOfficeGuide(_T("读取票箱B编号出错，请检查票箱是否安装"));
					return 1;
				}
				CTCardCountInfo::CARD_BOX_ID boxID;
				boxID.Deserialize(tTimRfidInfo.ucBoxID);
				m_dataModel.BoxBinfo.szID = boxID.ToString();
				if(m_dataModel.BoxBinfo.szQuantity.IsEmpty()){
					m_dataModel.BoxBinfo.szQuantity.Format(_T("%d"),((tTimRfidInfo.ucCardNum[1] << 8 ) | tTimRfidInfo.ucCardNum[0]));
					m_dataModel.BoxBinfo.state = PRE_INSTALL;
				}
			}
			break;
		case 3:
			{
				thBoxType = TH_BOX_TYPE::TH_BOX_C;
				bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,tTimRfidInfo);;//CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);
				if(!rst){
					theAPP_SESSION.ShowOfficeGuide(_T("读取废票箱编号出错，请检废票箱是否安装"));
					return 1;
				}
				CTCardCountInfo::CARD_BOX_ID boxID;
				boxID.Deserialize(tTimRfidInfo.ucBoxID);
				m_dataModel.InvalidBox.szID = boxID.ToString();
			}
			break;

		case ALL_BOX:
			{
				if(m_dataModel.m_bBoxACanOperator){
					thBoxType = TH_BOX_TYPE::TH_BOX_A;
					bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,tTimRfidInfo);//CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);
					if(!rst){
						theAPP_SESSION.ShowOfficeGuide(_T("读取票箱A编号出错，请检查票箱是否安装"));
						return 1;
					}
					CTCardCountInfo::CARD_BOX_ID boxID;
					boxID.Deserialize(tTimRfidInfo.ucBoxID);
					m_dataModel.BoxAinfo.szID = boxID.ToString();
					if(m_dataModel.BoxAinfo.szQuantity.IsEmpty()){
						m_dataModel.BoxAinfo.szQuantity.Format(_T("%d"),((tTimRfidInfo.ucCardNum[1] << 8 ) | tTimRfidInfo.ucCardNum[0]));
						m_dataModel.BoxAinfo.state = PRE_INSTALL;
					}
				}

				if(m_dataModel.m_bBoxBCanOperator){
					thBoxType = TH_BOX_TYPE::TH_BOX_B;
					bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,tTimRfidInfo);//CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);
					if(!rst){
						// 如果A已读取，设置为不能安装状态
						if(m_dataModel.BoxAinfo.state == PRE_INSTALL){
							m_dataModel.BoxAinfo.szID = INVALID_ID;
							m_dataModel.BoxAinfo.state = REMOVED;
						}
						
						theAPP_SESSION.ShowOfficeGuide(_T("读取票箱B编号出错，请检查票箱是否安装"));
						return 1;
					}
					CTCardCountInfo::CARD_BOX_ID boxID;
					boxID.Deserialize(tTimRfidInfo.ucBoxID);
					m_dataModel.BoxBinfo.szID = boxID.ToString();
					if(m_dataModel.BoxBinfo.szQuantity.IsEmpty()){
						m_dataModel.BoxBinfo.szQuantity.Format(_T("%d"),((tTimRfidInfo.ucCardNum[1] << 8 ) | tTimRfidInfo.ucCardNum[0]));
						m_dataModel.BoxBinfo.state = PRE_INSTALL;
					}
				}

				if(m_dataModel.m_bInvalidBoxCanOperator){
					thBoxType = TH_BOX_TYPE::TH_BOX_C;
					bool rst = CARD_HELPER->GetStaticBoxId(thBoxType,tTimRfidInfo);//CARD_HELPER->ReadTicketBoxRFID(thBoxType,&tTimRfidInfo);
					if(!rst){

						// 如果A已读取，设置为不能安装状态
						if(m_dataModel.BoxAinfo.state == PRE_INSTALL){
							m_dataModel.BoxAinfo.szID = INVALID_ID;
							m_dataModel.BoxAinfo.state = REMOVED;
						}

						// 如果B已读取，设置为不能安装状态
						if(m_dataModel.BoxBinfo.state == PRE_INSTALL){
							m_dataModel.BoxBinfo.szID = INVALID_ID;
							m_dataModel.BoxBinfo.state = REMOVED;
						}

						theAPP_SESSION.ShowOfficeGuide(_T("读取废票箱编号出错，请检查废票箱是否安装"));
						return 1;
					}
					CTCardCountInfo::CARD_BOX_ID boxID;
					boxID.Deserialize(tTimRfidInfo.ucBoxID);
					m_dataModel.InvalidBox.szID = boxID.ToString();
					if(!m_dataModel.InvalidBox.szID.IsEmpty()){
						m_dataModel.InvalidBox.state = PRE_INSTALL;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	catch(...){
		ret = 1;
		theAPP_SESSION.ShowOfficeGuide(_T("读取票箱编号出错"));
		Sleep(1500);
	}

	NotifyDataModelChanged();
	return ret;
	
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      检查票箱及盖板状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::CheckBoxStatus(OPERATE_BOX boxId){
	bool ret = false;
	UINT ueCurStatus = GetCurDialogGroupID();
	TH_RSP_GET_DEV_STATUS status;
	TH_REPLACE_BOX_CMD_INFO replaceSelect;
	try{
		CARD_HELPER->GetDevStatus(&status);
		if (ueCurStatus == DLG_INSTALL)						// 安装票箱
		{
			if (boxId == 1) // A票箱
			{
				// 检查盖板
				if (status.bBoxAArrive && status.bBoxACoverArrive)
				{
					ret = true;
				}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}
			}
			if (boxId == 2) // A票箱
			{
				// 检查盖板
				if (status.bBoxBArrive && status.bBoxBCoverArrive)
				{
					ret = true;
				}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}
			}

		}
		if (ueCurStatus == DLG_REMOVE)						// 卸载票箱
		{
			if (boxId == 1) // A票箱
			{
				// 检查盖板
				if (status.bBoxAArrive && status.bBoxACoverArrive)
				{
					ret = true;
				}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}
			}
			if (boxId == 2) // A票箱
			{
				// 检查盖板
				if (status.bBoxBArrive && status.bBoxBCoverArrive)
				{
					ret = true;
				}
				else{
					ret = false;
					theAPP_SESSION.ShowOfficeGuide(_T("请检查票箱是否到位、盖板状态"));
					Sleep(2000);
				}
			}
		}
		return ret;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_T("票箱模块异常。"));
		Sleep(2000);
		return false;
	}
}
