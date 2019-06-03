#include "StdAfx.h"
#include "ChangeAndInvalidBanknoteboxManageSvc.h"
#include "ChangeAndInvalidBanknoteboxManageDlg.h"
#include "BusinessTranslateSvc.h"
#include "SvcDef.h"
#include "TemplateHelper.h"
#include "CTBanknoteCountInfo.h"

BEGIN_MESSAGE_MAP(CChangeAndInvalidBanknoteboxManageSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
END_MESSAGE_MAP()

CChangeAndInvalidBanknoteboxManageSvc::CChangeAndInvalidBanknoteboxManageSvc(void)
	:CTVMForeService(INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC)
	,m_nBoxSel(0)
	,m_phase(CHOOSE_BOX)
{
	m_pDialogFlow->AddDialog(CChangeAndInvalidBanknoteboxManageDlg::IDD, new CChangeAndInvalidBanknoteboxManageDlg(this));
	m_pDialogFlow->AddDialogGroup(DAILOG_CHANGE_INVALID_BOX_MNG,_T(""));
	m_pDialogFlow->AddDialogGroupMember(DAILOG_CHANGE_INVALID_BOX_MNG, CChangeAndInvalidBanknoteboxManageDlg::IDD);

	m_pDialogFlow->SetFirstDialogGroup(DAILOG_CHANGE_INVALID_BOX_MNG);

	m_nRBox1 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount;
	m_nRBox2 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount;
	m_nRBox3 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount;
	m_nRBox4 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	CleanStack();
}


CChangeAndInvalidBanknoteboxManageSvc::~CChangeAndInvalidBanknoteboxManageSvc(void)
{
}

CChangeAndInvalidBanknoteboxManageSvc::Model& CChangeAndInvalidBanknoteboxManageSvc::GetDataModel()
{
	return m_model;
}

void CChangeAndInvalidBanknoteboxManageSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      回车键响应

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChangeAndInvalidBanknoteboxManageSvc::OnKeyboardEnter(WPARAM, LPARAM)
{
	int nInput = theAPP_SESSION.GetInputToInt() ;

	switch(m_phase)
	{
	case CHOOSE_BOX:
		{
			// 选择安装
			if(nInput == MENU_CODE_1){
				// 判断箱子是否已经安装
				if(m_model.changeBoxAData.boxState == INSTALLED){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_INSTALL_NO_AGAIN));
				}
				else{
					// 判断箱子是否在位
					if(!BANKNOTE_HELPER->IsCashChangeBoxAttached()){
						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_IN_PLACE));
					}
					else{
						// 读取ID
						CString strResult = ReadBoxRFIDAndSetID(CHANGE_BOX);

						if(strResult.IsEmpty()){
							theAPP_SESSION.SetMaxInputLength(3);
							CString strTip ;
							strTip.Format(_opl(TXT_BANKNOTEMGR_BOX_SET_NUM)); 

							//m_stack_ready.push((BOX_INDEX)nInput);
							m_phase = INPUT_COUNT;
							theAPP_SESSION.ShowOfficeGuide(strTip);
						}
						else{
							theAPP_SESSION.ShowOfficeGuide(strResult);
						}
					}
				}
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 输入错误的序号
				theAPP_SESSION.SetMaxInputLength(1);
			}
		}
		break;
	// 选择箱子（就绪）
	//case  CHOOSE_BOX:
	//	{
	//		if ( nInput > 0 && nInput < 3){
	//			if (IsBoxCouldBeInstall((BOX_INDEX)nInput)){		// 如果所选箱子可以安装
	//				m_nBoxSel = nInput;
	//				if (!theFunction_INFO.IsBanknoteChangeBoxIDRead()){	// 1号线
	//					// 手动输入ID
	//					m_phase = SET_ID;
	//					m_stack_ready.push((BOX_INDEX)nInput);// 存入待输入ID的容器中 等待进一步操作

	//					theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//					theAPP_SESSION.SetMaxInputLength(4);
	//				}
	//				else{	// 2号线
	//					CString strErrMsg = ReadBoxRFIDAndSetID((BOX_INDEX)nInput);
	//					if(strErrMsg.IsEmpty()){
	//						// 自动读取RFID 进入设置数量阶段
	//						theAPP_SESSION.SetMaxInputLength(3);
	//						char cBoxNo = (nInput == 1)? 'A':'B';
	//						CString strTip ;
	//						strTip.Format(_T("请输入找零箱%c的数量："),cBoxNo);

	//						m_stack_ready.push((BOX_INDEX)nInput);
	//						m_phase = INPUT_COUNT;
	//						theAPP_SESSION.ShowOfficeGuide(strTip);
	//					}
	//					else{
	//						theAPP_SESSION.ShowOfficeGuide(strErrMsg);
	//					}
	//				}
	//			}
	//			else{
	//				theAPP_SESSION.ShowOfficeGuide(_T("所选纸币箱不可安装。"));
	//			}
	//			
	//		}
	//		else if (nInput == INVALID_BOX)				// 废币箱
	//		{
	//			m_nBoxSel = nInput;
	//			if (IsBoxCouldBeInstall((BOX_INDEX)nInput)){
	//				/*m_phase = SET_ID;
	//				m_stack_ready.push(INVALID_BOX);
	//				theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//				theAPP_SESSION.SetMaxInputLength(4);*/
	//				if(!theFunction_INFO.IsBanknoteChangeBoxIDRead()){
	//					// 待安装状态
	//					m_model.invalidBoxData.strID = m_model.strPreInvalidID;
	//					m_phase = CHOOSE_BOX;
	//					m_model.invalidBoxData.boxState = PRE_INST;
	//					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//					theAPP_SESSION.SetMaxInputLength(1);
	//				}
	//				else{
	//					CString strErrMsg = ReadBoxRFIDAndSetID(INVALID_BOX);
	//					if(strErrMsg.IsEmpty()){
	//						m_phase = CHOOSE_BOX;
	//						m_model.invalidBoxData.boxState = PRE_INST;
	//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//						theAPP_SESSION.SetMaxInputLength(1);
	//					}
	//					else{
	//						theAPP_SESSION.ShowOfficeGuide(strErrMsg);
	//					}
	//				}
	//			}
	//			else{
	//				theAPP_SESSION.ShowOfficeGuide(_T("所选废币箱不可安装。"));
	//			}
	//		}
	//		else if (nInput == ALL_BOX){
	//			// 安装所有箱子
	//			CleanStack();
	//			// 如果箱子可以执行安装操作
	//			for(int idx = 3; idx > 0;idx--){
	//				if(IsBoxCouldBeInstall((BOX_INDEX)idx))
	//					m_stack_ready.push((BOX_INDEX)idx);
	//			}
	//			if (!m_stack_ready.empty()){
	//				BOX_INDEX boxIndex = m_stack_ready.top();
	//				// 手动输入ID
	//				if (!theFunction_INFO.IsBanknoteChangeBoxIDRead()){
	//					if(boxIndex == INVALID_BOX){// 最上面一个为废币箱，说明只有废币箱可以安装
	//						// 待安装状态
	//						m_model.invalidBoxData.strID = m_model.strPreInvalidID;
	//						m_phase = CHOOSE_BOX;
	//						m_model.invalidBoxData.boxState = PRE_INST;
	//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//						theAPP_SESSION.SetMaxInputLength(1);
	//						m_stack_ready.pop();
	//					}
	//					else{
	//						m_phase = SET_ID;
	//						theAPP_SESSION.SetMaxInputLength(4);
	//						theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//					}
	//				}
	//				else {

	//					CString strErrMsg = ReadBoxRFIDAndSetID(ALL_BOX);
	//					// 可以正常安装
	//					if(strErrMsg.IsEmpty()){
	//						CString strShowMsg("");
	//						switch(boxIndex){
	//						case CHANGE_BOX_A:
	//							strShowMsg = _T("请输入找零箱A的数量：");
	//							break;
	//						case CHANGE_BOX_B:
	//							strShowMsg = _T("请输入找零箱B的数量：");
	//							break;
	//						case INVALID_BOX:
	//							//strShowMsg = _opl(GUIDE_ENTER_F2_F3_ESC);// 不输入数量
	//							break;
	//						default:
	//							break;
	//						}
	//						if(strShowMsg.IsEmpty()){
	//							// 只有废币箱安装
	//							m_model.invalidBoxData.boxState = PRE_INST;
	//							theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//						}
	//						else{
	//							theAPP_SESSION.SetMaxInputLength(3);
	//							m_phase = INPUT_COUNT;
	//							theAPP_SESSION.SetPrefixToGuide(strShowMsg);
	//						}
	//					}
	//					else{
	//						theAPP_SESSION.ShowOfficeGuide(strErrMsg);
	//					}
	//				}
	//			}
	//			else{
	//				theAPP_SESSION.ShowOfficeGuide(_T("所选找零箱不可安装。"));
	//			}
	//		}
	//		else{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 输入错误的序号
	//			theAPP_SESSION.SetMaxInputLength(1);
	//		}
	//	}
	//	break;

	//// 输入ID
	//case SET_ID:
	//	{
	//		int inputNum = theAPP_SESSION.GetInputToInt();
	//		if(inputNum == 0){
	//			CString strMsg;
	//			strMsg.Format(_T("请输入正确格式的纸币箱编号：%s"),m_model.strPreID);
	//			theAPP_SESSION.SetPrefixToGuide(strMsg);
	//			m_phase = SET_ID;
	//			break;
	//		}
	//		else{
	//			// 判断箱子类型，并比较是否重复
	//			if(!m_stack_ready.empty()){
	//				BOX_INDEX boxType = m_stack_ready.top();
	//				// 检查ID是否重复
	//				if(CheckInputBoxID(boxType,inputNum)){
	//					// 提示ID重复
	//					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_TH_TEST_TEXT_ID_REPEAT));
	//					Sleep(1000);
	//					theAPP_SESSION.SetMaxInputLength(4);
	//					theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//					return 0;
	//				}
	//				else{
	//					if (boxType == BOX_INDEX::INVALID_BOX){
	//						m_model.invalidBoxData.boxState = PRE_INST;
	//						m_stack_ready.pop();
	//						if (m_stack_ready.empty()){
	//							theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//							m_phase = CHOOSE_BOX;
	//							theAPP_SESSION.SetMaxInputLength(1);
	//						}
	//						else{
	//							m_phase = SET_ID;
	//							theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//							theAPP_SESSION.SetMaxInputLength(4);
	//						}
	//					}
	//					else{
	//						m_phase = INPUT_COUNT;
	//						theAPP_SESSION.SetMaxInputLength(3);
	//						theAPP_SESSION.SetPrefixToGuide(ShowGuide());
	//					}
	//				}
	//			}
	//		}
	//	}break;
	// 输入数量
	case INPUT_COUNT:
		{
			if ( nInput >= 0 && nInput <= 300){
				SetCount(CHANGE_BOX, nInput);
				//m_stack_ready.pop();
				//if (m_stack_ready.empty()){
					m_phase = CHOOSE_BOX;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
					theAPP_SESSION.SetMaxInputLength(1);
				/*}
				else{
					if(INVALID_BOX == m_stack_ready.top()){
						// 待安装状态
						if (!theFunction_INFO.IsBanknoteChangeBoxIDRead())
							m_model.invalidBoxData.strID = m_model.strPreInvalidID;
						m_phase = CHOOSE_BOX;
						m_model.invalidBoxData.boxState = PRE_INST;
						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
						theAPP_SESSION.SetMaxInputLength(1);
						m_stack_ready.pop();
					}
					else{
						if (!theFunction_INFO.IsBanknoteChangeBoxIDRead())
							m_phase = SET_ID;
						else
							m_phase = INPUT_COUNT;

						theAPP_SESSION.SetPrefixToGuide(ShowGuide());
						theAPP_SESSION.SetMaxInputLength(4);
					}
				}*/
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_NUMBER_ERROR));// 输入补充数量不合法
			}
		}
		break;
	default:
		break;
	}
	NotifyDataModelChanged();
	return 0;
}

LRESULT CChangeAndInvalidBanknoteboxManageSvc::OnKeyboardF2(WPARAM , LPARAM)
{
	
	if(m_model.changeBoxAData.boxState != PRE_INST){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FAILED_SAVE));
		return 0;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_SUPPLEBOX_INSTALL_WAIT));
	map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> cashUnit;
	try{
		BANKNOTE_HELPER->Repare(true);
	}
	catch(CSysException&e)
	{
		if(e.GetDetailID()!=CBRHHardwareException::ERR_BUSY){
			theEXCEPTION_MGR.ProcessException(e);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_MODULE_FAILE));
			return FALSE;
		}
		else{
			// 纸币模块忙碌中，下面处理
		}
	}


	do{
		try{
			cashUnit = BANKNOTE_HELPER->GetEmptyFull();
			break;
		}
		catch(CSysException&e)
		{
			if (e.GetDetailID()!=CBRHHardwareException::ERR_BUSY){
				theEXCEPTION_MGR.ProcessException(e);
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_MODULE_FAILE));
				return FALSE;
			}
			else{
				Sleep(5000);//等纸币模块退出忙碌状态
			}
		}
	}while(1);


	int nChangeCount = m_model.changeBoxAData.nCount;
	LOG("安装纸币补充箱:%d", nChangeCount);
	//需要计算补充箱进入到循环箱中的数量
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO rabA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO rabB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO rabC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO rabD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
	if (cashUnit[BNR_RECYCLER_BOX_A].count > m_nRBox1)
	{
		nChangeCount -= (cashUnit[BNR_RECYCLER_BOX_A].count - m_nRBox1);
		rabA.ulCurCount = cashUnit[BNR_RECYCLER_BOX_A].count;
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(rabA);
	}
	if (cashUnit[BNR_RECYCLER_BOX_B].count > m_nRBox2)
	{
		nChangeCount -= (cashUnit[BNR_RECYCLER_BOX_B].count - m_nRBox2);
		rabB.ulCurCount = cashUnit[BNR_RECYCLER_BOX_B].count;
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(rabB);

	}
	if (cashUnit[BNR_RECYCLER_BOX_C].count > m_nRBox3)
	{
		nChangeCount -= (cashUnit[BNR_RECYCLER_BOX_C].count - m_nRBox3);
		rabC.ulCurCount = cashUnit[BNR_RECYCLER_BOX_C].count;
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(rabC);

	}
	if (cashUnit[BNR_RECYCLER_BOX_D].count > m_nRBox4)
	{
		nChangeCount -= (cashUnit[BNR_RECYCLER_BOX_D].count - m_nRBox4);
		rabD.ulCurCount = cashUnit[BNR_RECYCLER_BOX_D].count;
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(rabD);
	}
	if (nChangeCount < 0)
	{
		nChangeCount = 0;
	}
	CString strTips = _T("");
	BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
	BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
	BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBoxInfo;
	changeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	// 初始化硬币专用找零箱AID
	int2BCD(bLineCode, (char*)&changeBoxInfo.banknoteBoxID.bLineCode, sizeof(changeBoxInfo.banknoteBoxID.bLineCode));
	changeBoxInfo.banknoteBoxID.bType = 0x07;
	int2BCD(bStationCode, (char*)&changeBoxInfo.banknoteBoxID.bNumInQu, sizeof(changeBoxInfo.banknoteBoxID.bNumInQu));
	int2BCD(bMachineCode, (char*)&changeBoxInfo.banknoteBoxID.bNum, sizeof(changeBoxInfo.banknoteBoxID.bNum));
	changeBoxInfo.ulCurValue = TransBankNoteValue(theTVM_SETTING.GetBNRChangeBoxAmount());
	changeBoxInfo.ulCurCount			  = nChangeCount/*m_model.changeBoxAData.nCount*/;
	changeBoxInfo.operateTime = ComGetCurTime();
	changeBoxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	m_model.changeBoxAData.strID = changeBoxInfo.banknoteBoxID.ToString();

	// 更新计数
	if(theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBoxInfo)){
		strTips += _opl(TXT_BANKNOTEMGR_SUPPLEBOX_INSTALL_OK);
		// 更新状态
		m_model.changeBoxAData.boxState = INSTALLED;

		InsertOperaterToDatabase(CHANGE_BOX);

		//发送纸币找零箱安装电文
		CASHBOX_CHANGE change;
		change.SetBoxID(changeBoxInfo.banknoteBoxID.bLineCode, changeBoxInfo.banknoteBoxID.bType, changeBoxInfo.banknoteBoxID.bNumInQu, changeBoxInfo.banknoteBoxID.bNum);			//纸币箱ID
		change.handleType = INSTALL;				//操作类型
		change.position = MAGAZINEH;//位置
		change.State = VALID_ATTACH;				//状态

		CASH_TYPE_COUNT cash;
		cash.cash_type  = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());	// 5元
		cash.cash_count = m_model.changeBoxAData.nCount;
		change.vecCash.push_back(cash);
		change.btCount = 0x01;

		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, true); // 日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
		theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
		LOG("安装纸币补充箱成功！循环鼓ABCDE纸币数：%d,%d,%d,%d，补充箱：%d", 
			rabA.ulCurCount, rabB.ulCurCount, rabC.ulCurCount, rabD.ulCurCount, changeBoxInfo.ulCurCount);
	}
	else{
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, false);
		strTips += _opl(TXT_BANKNOTEMGR_SUPPLEBOX_INSTALL_FAILED);
	}

	if(!strTips.IsEmpty())
	{
		strTips += _opl(GUIDE_F3_ESC);
		theAPP_SESSION.ShowOfficeGuide(strTips);
	}

	if ( m_model.changeBoxAData.boxState == INSTALLED)	{ // 如果有操作且成功 打印凭条
		PrintBanknoteChangeReceiption();
		
		if(m_model.bIsException){
			Sleep(1000);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		}
	}
		
	m_Ex_change = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

	//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_info;
	//BYTE line = 0xFF;
	//BYTE Type = 0xFF;
	//WORD wNum = 0xFFFF;

	//CString strTips = _T("");
	//bool isOperated = (m_model.changeBoxAData.boxState == PRE_INST) || (m_model.changeBoxBData.boxState == PRE_INST) || (m_model.invalidBoxData.boxState == PRE_INST);
	//bool isDone = false;

	//// 如果操作，直接返回
	//if(!isOperated){
	//	return 0;
	//}

	//// 找零箱A
	//if (m_model.changeBoxAData.boxState == PRE_INST)
	//{
	//	line = theMAINTENANCE_INFO.GetLineCode();//_tcstoul(m_model.changeBoxAData.strID.Left(2),NULL,10);
	//	Type = 0x60;//_tcstoul(m_model.changeBoxAData.strID.Mid(2,2),NULL,10);
	//	wNum = _tcstoul(m_model.changeBoxAData.strID.Right(4),NULL,16);

	//	box_info = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();

	//	box_info.banknoteBoxID.bLineCode = line ;
	//	box_info.banknoteBoxID.bType = Type ;
	//	box_info.banknoteBoxID.wNum = wNum;

	//	box_info.ulCurCount = m_model.changeBoxAData.nCount;

	//	// 更新软计数
	//	if (theBANKNOTE_COUNT.SetBanknoteChangeBoxAInfo(box_info)){
	//		// 成功
	//		strTips += _T("纸币找零箱A安装成功。");
	//		isDone = true;
	//		// 更新状态
	//		m_model.changeBoxAData.boxState = INSTALLED;

	//		InsertOperaterToDatabase(CHANGE_BOX_A);

	//		//发送纸币找零箱安装电文
	//		CASHBOX_CHANGE change;
	//		change.SetBoxID(line,Type, wNum);			//纸币箱ID
	//		change.handleType = INSTALL;				//操作类型
	//		change.position = BANKNOTE_CHANGE_BOX_A_POS;//位置
	//		change.State = VALID_ATTACH;				//状态

	//		CASH_TYPE_COUNT cash;
	//		cash.cash_type  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());	// 5元
	//		cash.cash_count = m_model.changeBoxAData.nCount;
	//		change.vecCash.push_back(cash);
	//		change.btCount = 0x01;

	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, true); // 日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
	//		theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//	}else{// 失败
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, false);
	//		strTips += _T("纸币找零箱A安装失败。");
	//	}
	//}

	//// 找零箱B
	//if (m_model.changeBoxBData.boxState == PRE_INST)
	//{
	//	box_info = CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO();
	//	line = theMAINTENANCE_INFO.GetLineCode();//_tcstoul(m_model.changeBoxAData.strID.Left(2),NULL,10);
	//	Type = 0x60;//_tcstoul(m_model.changeBoxAData.strID.Mid(2,2),NULL,10);
	//	wNum = _tcstoul(m_model.changeBoxBData.strID.Right(4),NULL,16);

	//	box_info = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();

	//	box_info.banknoteBoxID.bLineCode = line ;
	//	box_info.banknoteBoxID.bType = Type ;
	//	box_info.banknoteBoxID.wNum = wNum;

	//	box_info.ulCurCount = m_model.changeBoxBData.nCount;

	//	// 更新软计数
	//	if (theBANKNOTE_COUNT.SetBanknoteChangeBoxBInfo(box_info)){
	//		// 成功
	//		strTips += _T("纸币找零箱B安装成功。");
	//		// 更新状态
	//		m_model.changeBoxBData.boxState = INSTALLED;

	//		InsertOperaterToDatabase(CHANGE_BOX_B);

	//		isDone = true;
	//		//发送纸币找零箱安装电文
	//		CASHBOX_CHANGE change;
	//		change.SetBoxID(line,Type, wNum);			//纸币箱ID
	//		change.handleType = INSTALL;				//操作类型
	//		change.position = BANKNOTE_CHANGE_BOX_B_POS;//位置
	//		change.State = VALID_ATTACH;				//状态	
	//		CASH_TYPE_COUNT cash;
	//		cash.cash_type  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());	// 5元
	//		cash.cash_count = m_model.changeBoxBData.nCount;
	//		change.vecCash.push_back(cash);
	//		change.btCount = 0x01;

	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, true); // 日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
	//		theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//	}
	//	else{// 失败
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, false);
	//		strTips += _T("纸币找零箱B安装失败。");
	//	}
	//}

	//// 废币回收箱
	//if (m_model.invalidBoxData.boxState == PRE_INST)
	//{
	//	box_info = CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO();
	//	line = theMAINTENANCE_INFO.GetLineCode();
	//	Type = 0x62;
	//	wNum = _tcstoul(m_model.invalidBoxData.strID.Right(4),NULL,16);

	//	box_info = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();

	//	box_info.banknoteBoxID.bLineCode = line ;
	//	box_info.banknoteBoxID.bType = Type ;
	//	box_info.banknoteBoxID.wNum = wNum;

	//	box_info.ulCurCount = m_model.invalidBoxData.nCount;

	//	InsertOperaterToDatabase(INVALID_BOX);

	//	isDone = true;
	//	// 更新软计数
	//	if (theBANKNOTE_COUNT.SetBanknoteInvalidBoxInfo(box_info)){
	//		// 成功
	//		strTips += _T("废币回收箱安装成功。");
	//		// 更新状态
	//		m_model.invalidBoxData.boxState = INSTALLED;

	//		//发送纸币找零废币箱安装电文
	//		CASHBOX_CHANGE change;
	//		change.SetBoxID(line,Type, wNum);			//纸币箱ID
	//		change.handleType = INSTALL;				//操作类型
	//		change.position = BANKNOTE_INVALID_POS;		//位置
	//		change.State = VALID_ATTACH;				//状态

	//		CASH_TYPE_COUNT cash;
	//		cash.cash_type  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());	// 5元
	//		cash.cash_count = 0;
	//		change.vecCash.push_back(cash);
	//		change.btCount = 0x01;
	//		
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, true); // 日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
	//	}
	//	else{
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_INSTALLATION, false);
	//		strTips += _T("废币回收箱安装失败。");
	//	}
	//}
	//if ( isDone && isOperated)	{ // 如果有操作且成功 打印凭条
	//	PrintBanknoteChangeReceiption();
	//	theAPP_SESSION.ShowOfficeGuide(strTips);
	//	if(m_model.bIsException){
	//		Sleep(1000);
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	}
	//}
	//
	//m_Ex_changeA = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();
	//m_Ex_changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	//m_Ex_invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();

	NotifyDataModelChanged();
	return 0;
}

LRESULT CChangeAndInvalidBanknoteboxManageSvc::OnKeyboardF3(WPARAM , LPARAM)
{
	NotifyDataModelChanged();
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}


void CChangeAndInvalidBanknoteboxManageSvc::OnStart()
{
	__super:: OnStart();
	LineCode = 1;
	m_phase = CHOOSE_BOX;
	InitializeData();
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      判断Enter键的用途

@param      None
@retval     BOOL   TRUE: 当前Enter键用于选择操作的箱子
				   FALSE:当前Enter键用于设置相关数量
*/
//////////////////////////////////////////////////////////////////////////
//CChangeAndInvalidBanknoteboxManageSvc::PHASE CChangeAndInvalidBanknoteboxManageSvc::getPhase()
//{
//	return CHOOSE_BOX;
//}


//void CChangeAndInvalidBanknoteboxManageSvc::doInstallBox(int nIndex)
//{
//	BOOL isAllRight = FALSE;
//	switch(nIndex)
//	{
//	case 1:
//		if (m_model.changeBoxAData.boxState == INSTALLED){	// 检查当前状态，如果已安装 不可执行安装
//			theAPP_SESSION.ShowOfficeGuide(_T("纸币找零箱A已安装"));
//			return;
//		}else if (isAllRight){// 检查箱子不在位
//			theAPP_SESSION.ShowOfficeGuide(_T("纸币找零箱A不在位"));
//			return;
//		}else{
//			//m_model.changeBoxAData.boxState = WAIT_TO_BE_INSTALLED;
//		}
//		
//		break;
//	case 2:
//		if (m_model.changeBoxAData.boxState == INSTALLED){	// 检查当前状态，如果已安装 不可执行安装
//			theAPP_SESSION.ShowOfficeGuide(_T("纸币找零箱B已安装"));
//			return;
//		}else if (isAllRight){// 检查箱子不在位
//			theAPP_SESSION.ShowOfficeGuide(_T("纸币找零箱B不在位"));
//			return;
//		}else{
//			//m_model.changeBoxBData.boxState = WAIT_TO_BE_INSTALLED;
//		}
//
//		break;
//	case 3: 
//		if (m_model.changeBoxAData.boxState == INSTALLED){	// 检查当前状态，如果已安装 不可执行安装
//			theAPP_SESSION.ShowOfficeGuide(_T("废币回收箱已安装"));
//			return;
//		}else if (isAllRight){// 检查箱子不在位
//			theAPP_SESSION.ShowOfficeGuide(_T("废币回收箱不在位"));
//			return;
//		}else{
//			//废币箱安装不需要数量
//			m_model.invalidBoxData.boxState = PRE_INST;
//		}
//		break;
//	default:break;
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置纸币找零箱的纸币张数

@param      （in）index	: 纸币箱位置（A,B）
			 (in) nCount: 要设置的数量

@retval      None
*/
//////////////////////////////////////////////////////////////////////////
void CChangeAndInvalidBanknoteboxManageSvc::SetCount(int nIndex, int nCount)
{
	// 设置数量 更新状态
	if (nIndex == 1){
		m_model.changeBoxAData.nCount = nCount;
		m_model.changeBoxAData.boxState = PRE_INST;
	}/*else if (nIndex == 2){
	 m_model.changeBoxBData.nCount = nCount;
	 m_model.changeBoxBData.boxState = PRE_INST;
	 }*/
}

//
//void CChangeAndInvalidBanknoteboxManageSvc::WriteChangeBoxCountInfo()
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置纸币箱的ID

@param      （in）index: 纸币箱位置（A,B,废）
			 (in) strID: 要设置的id

@retval     None
*/
//////////////////////////////////////////////////////////////////////////
void CChangeAndInvalidBanknoteboxManageSvc::SetBoxID(BOX_INDEX index ,CString& strID)
{
	switch(index)
	{
	case CHANGE_BOX:
		{
			m_model.changeBoxAData.strID = strID;
		}break;
		/*case CHANGE_BOX_B:
		{
		m_model.changeBoxBData.strID = strID;
		}break;
		case INVALID_BOX:
		{
		m_model.invalidBoxData.strID = strID;
		}break;*/
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断箱子是否可执行安装操作

@param      （in）index: 纸币箱位置（A,B,废）


@retval     bool : true 可以安装   false 不可安装
*/
//////////////////////////////////////////////////////////////////////////
bool CChangeAndInvalidBanknoteboxManageSvc::IsBoxCouldBeInstall(BOX_INDEX index)
{
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_A = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_B = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_C = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

	bool yes = true;

	bool bIsCashChangeBoxAttched =	false;
	try
	{
		bIsCashChangeBoxAttched = BANKNOTE_HELPER->IsCashChangeBoxAttached();
	}
	catch(...)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN));
		return false;
	}


	if(changeBox.banknoteBoxID.bLineCode == 0xFF && bIsCashChangeBoxAttched){
		yes = true;
	}
	else{
		yes = false;
	}
	//switch(index)
	//{
	//case CHANGE_BOX_A:
	//	{
	//		if (box_A.banknoteBoxID.bLineCode == 0xFF && bIsCashChangeBoxAttched/*box_status.CassetteStatus.isFirstBoxExist*/){ // 判断规则： 软计数中的找零箱不在位 硬件纸币箱在位
	//			yes = true;
	//		}else{
	//			yes = false;
	//		}
	//	}break;

	////case CHANGE_BOX_B:
	////	{
	////		if (box_B.banknoteBoxID.bLineCode == 0xFF && box_status.CassetteStatus.isSecondBoxExist){
	////			yes = true;
	////		}else{
	////			yes = false;
	////		}
	////	}break;
	////case INVALID_BOX:
	////	{
	////		if (box_C.banknoteBoxID.bLineCode == 0xFF && box_status.SensorStatus.isRejectBoxAvailable){
	////			yes = true;
	////		}else{
	////			yes = false;
	////		}
	////	}break;
	//default:
	//	break;
	//}
	return yes;
	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      数据初始化

@param      None


@retval     None
*/
//////////////////////////////////////////////////////////////////////////
void CChangeAndInvalidBanknoteboxManageSvc::InitializeData()
{
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	if(changeBox.banknoteBoxID.bLineCode == 0xFF){
		m_model.changeBoxAData.strID = _T("FFFFFFFF");
		m_model.changeBoxAData.boxState = REMOVED;
	}
	else{
		m_model.changeBoxAData.boxState = INSTALLED;
	}

	m_model.changeBoxAData.nCount = changeBox.ulCurCount;
	m_model.changeBoxAData.strID  = changeBox.banknoteBoxID.ToString();

	m_model.bIsException = false;
	m_model.bIsPrintFailed = false;
	m_Ex_change = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

	CleanStack();

	//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_A = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();
	//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_B = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_C = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();
	//// 找零箱A
	//if (box_A.banknoteBoxID.bLineCode == 0xFF){
	//	m_model.changeBoxAData.strID = _T("FFFFFFFF");
	//	m_model.changeBoxAData.boxState = REMOVED;
	//}else{
	//	m_model.changeBoxAData.boxState = INSTALLED;
	//}
	//m_model.changeBoxAData.nCount = box_A.ulCurCount;
	//m_model.changeBoxAData.strID = box_A.banknoteBoxID.ToString();

	//// 找零箱B
	//if (box_B.banknoteBoxID.bLineCode == 0xFF){
	//	m_model.changeBoxBData.strID = _T("FFFFFFFF");
	//	m_model.changeBoxBData.boxState = REMOVED;
	//}else{
	//	m_model.changeBoxBData.boxState = INSTALLED;
	//}
	//m_model.changeBoxBData.nCount = box_B.ulCurCount;
	//m_model.changeBoxBData.strID = box_B.banknoteBoxID.ToString();

	//if (box_C.banknoteBoxID.bLineCode == 0xFF){
	//	m_model.invalidBoxData.strID = _T("FFFFFFFF");
	//	m_model.invalidBoxData.boxState = REMOVED;
	//}else{
	//	m_model.invalidBoxData.boxState = INSTALLED;
	//}
	//m_model.invalidBoxData.nCount = box_C.ulCurCount;
	//m_model.invalidBoxData.strID = box_C.banknoteBoxID.ToString();
	//m_model.bIsException = false;
	//m_model.bIsPrintFailed = false;
	//m_Ex_changeA = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();
	//m_Ex_changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	//m_Ex_invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();

	//CleanStack();
}

CString CChangeAndInvalidBanknoteboxManageSvc::TranslateState(BOX_STATE boxState)
{
	
	if (boxState == INSTALLED)
	{
		return _opl(TXT_BANKNOTEMGR_BOX_ATTACHED);
	}else if (boxState == REMOVED)
	{
		return _opl(TXT_SMARTCARDMGR_DETACHED);
	}else if (boxState == PRE_INST)
	{
		return _opl(TXT_BANKNOTEMGR_BOX_READY_INSTALL);
	}else if (boxState ==PRE_RMV)
	{
		return _opl(TXT_SMARTCARDMGR_WAITTODETACH);
	}else{
		return _T("");
	}
}


void CChangeAndInvalidBanknoteboxManageSvc::PrintBanknoteChangeReceiption()
{
	try
	{
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
		//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
		//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();

	
		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_Ex_change.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), changeBox.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"), 0);
		strFace1Count_after.Format(_T("%d"), 0);
		// 5元
		CString strFace5Count_before = _T("");
		CString strFace5Count_after = _T("");
		strFace5Count_before.Format(_T("%d"), m_Ex_change.ulCurCount);
		strFace5Count_after.Format(_T("%d"), m_model.changeBoxAData.nCount/*changeBox.ulCurCount*/);
		// 10元
		CString strFace10Count_before = _T("");
		CString strFace10Count_after = _T("");
		strFace10Count_before.Format(_T("%d"),0);
		strFace10Count_after.Format(_T("%d"), 0);
		// 20元
		CString strFace20Count_before = _T("");
		CString strFace20Count_after = _T("");
		strFace20Count_before.Format(_T("%d"), 0);
		strFace20Count_after.Format(_T("%d"), 0);
		// 50元
		CString strFace50Count_before = _T("");
		CString strFace50Count_after = _T("");
		strFace50Count_before.Format(_T("%d"), 0);
		strFace50Count_after.Format(_T("%d"), 0);
		// 100元
		CString strFace100Count_before = _T("");
		CString strFace100Count_after = _T("");
		strFace100Count_before.Format(_T("%d"), 0);
		strFace100Count_after.Format(_T("%d"), 0);

		// 加载Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));																			// 操作类型
			line.Replace(_T("{钱箱}"), _T("纸币找零箱"));
			line.Replace(_T("{操作前钱箱号}"), m_Ex_change.banknoteBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), changeBox.banknoteBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_Ex_change.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), changeBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{操作前5元}"), strFace5Count_before);
			line.Replace(_T("{操作后5元}"), strFace5Count_after);
			line.Replace(_T("{操作前10元}"), strFace10Count_before);
			line.Replace(_T("{操作后10元}"), strFace10Count_after);
			line.Replace(_T("{操作前20元}"), strFace20Count_before);
			line.Replace(_T("{操作后20元}"), strFace20Count_after);
			line.Replace(_T("{操作前50元}"), strFace50Count_before);
			line.Replace(_T("{操作后50元}"), strFace50Count_after);
			line.Replace(_T("{操作前100元}"), strFace100Count_before);
			line.Replace(_T("{操作后100元}"), strFace100Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}

		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_INSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
		m_model.bIsException = true;
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_model.bIsException = true;
	}
}

// 清空等待被卸载的箱子集
void CChangeAndInvalidBanknoteboxManageSvc::CleanStack()
{
	while (!m_stack_ready.empty()){
		m_stack_ready.pop();
	}
}

CString CChangeAndInvalidBanknoteboxManageSvc::ShowGuide()
{
	CString strCurrentBox = _T("");	// 当前箱子
	if (m_stack_ready.empty())
	{
		return _opl(GUIDE_ENTER_F2_F3_ESC);
	}
	/*switch(m_stack_ready.top())
	{
	case CHANGE_BOX_A:
	strCurrentBox = _T("找零箱A");
	break;
	case CHANGE_BOX_B:
	strCurrentBox = _T("找零箱B");
	break;
	case INVALID_BOX:
	strCurrentBox = _T("废币箱");
	break;
	default:
	strCurrentBox = _T("纸币箱");
	break;
	}*/
	CString  strPrefix = _T("请输入纸币箱编号");
	if (m_phase == SET_ID)
	{
		strPrefix.Format(_T("请输入%s的编号：%s"),strCurrentBox,m_model.strPreID);
		//theAPP_SESSION.SetPrefixToGuide(strPrefix);
	}else if (m_phase == INPUT_COUNT)
	{
		strPrefix.Format(_T("请输入%s的数量："),strCurrentBox);
		//theAPP_SESSION.SetPrefixToGuide(strPrefix);
	}else{
		strPrefix = _opl(GUIDE_ENTER_F2_F3_ESC);
	}
	return strPrefix;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      箱子操作插入库

@param      BOX_INDEX boxType 箱子类型

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChangeAndInvalidBanknoteboxManageSvc::InsertOperaterToDatabase(BOX_INDEX boxType){
	try{
		Data::BanknoteOperationRecord banknoteOpRecord;
		
		banknoteOpRecord.dtTransTime = ComGetCurTime();
		banknoteOpRecord.iOperationType = 0x01;// 安装
		banknoteOpRecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
		if(boxType == CHANGE_BOX){
			banknoteOpRecord.txtBanknoteBoxID = m_model.changeBoxAData.strID;
			banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());
			banknoteOpRecord.iQuantity		   = m_model.changeBoxAData.nCount;
		}
		/*else if(boxType == CHANGE_BOX_B){
		banknoteOpRecord.txtBanknoteBoxID = m_model.changeBoxBData.strID;
		banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());
		banknoteOpRecord.iQuantity		   = m_model.changeBoxBData.nCount;
		}
		else if(boxType == INVALID_BOX){
		banknoteOpRecord.txtBanknoteBoxID = m_model.invalidBoxData.strID;
		banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());
		banknoteOpRecord.iQuantity		   = m_model.invalidBoxData.nCount;
		}*/
		else{
			return;
		}
		DATA_HELPER->DbInsertBanknoteOperationRecord(banknoteOpRecord);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析纸币面值代码 BIN	1：1元，2：5元，3：10元，4：20元，5：50元，6：100元

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CChangeAndInvalidBanknoteboxManageSvc::TransBanknoteType(int value) {
	switch (value) {
	case 1: return Banknote1Yuan;
	case 5: return Banknote5Yuan;
	case 10: return Banknote10Yuan;
	case 20: return Banknote20Yuan;
	case 50: return Banknote50Yuan;
	case 100: return Banknote100Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检查箱子ID是否重复

@param      无

@retval     bool true:重复 false:不重复

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CChangeAndInvalidBanknoteboxManageSvc::CheckInputBoxID(BOX_INDEX boxType,int nBoxId){
//	
//	// 新输入的ID编号
//	CString strBoxId(_T(""));
//	strBoxId.Format(_T("%02d60%04d"),theMAINTENANCE_INFO.GetLineCode(),nBoxId);
//	
//	if(boxType == BOX_INDEX::INVALID_BOX){
//		strBoxId.Format(_T("%02d62%04d"),theMAINTENANCE_INFO.GetLineCode(),nBoxId);
//	}
//
//
//	if(boxType == BOX_INDEX::CHANGE_BOX_A){		
//		if(!strBoxId.IsEmpty()){
//			if(strBoxId.Compare(m_model.changeBoxBData.strID) == 0 || strBoxId.Compare(m_model.invalidBoxData.strID) == 0){
//				return true;
//			}
//			else{
//				SetBoxID(boxType,strBoxId);
//			}
//		}
//		return false;
//	}
//	else if(boxType == BOX_INDEX::CHANGE_BOX_B){
//		if(!strBoxId.IsEmpty()){
//			if(strBoxId.Compare(m_model.changeBoxAData.strID) == 0 || strBoxId.Compare(m_model.invalidBoxData.strID) == 0){
//				return true;
//			}
//			else{
//				SetBoxID(boxType,strBoxId);
//			}
//		}
//		return false;
//	}
//	else if(boxType == BOX_INDEX::INVALID_BOX){
//		if(!strBoxId.IsEmpty()){
//			if(strBoxId.Compare(m_model.changeBoxBData.strID) == 0 || strBoxId.Compare(m_model.changeBoxAData.strID) == 0){
//				return true;
//			}
//			else{
//				//SetBoxID(boxType,strBoxId);
//			}
//		}
//		return false;
//	}
//	else 
//		return false;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读取并设置指定纸币找零箱RFID

@param      (i)BOX_INDEX boxIndex;（所指定箱子类型）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CChangeAndInvalidBanknoteboxManageSvc::ReadBoxRFIDAndSetID(BOX_INDEX boxIndex){
	
	CString strReadRfidErrMsg(_T(""));

	try{
		//RFID_DATA outRfid;
		////outRfid.bankNoteChangeRFID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		//long lRet = 0;

		////switch(boxIndex){
		////case CHANGE_BOX_A:
		////case CHANGE_BOX_B:
		////case INVALID_BOX:
		////	break;
		////case ALL_BOX:
		////	{
		////		// 读取所有票箱ID及设置所有的票箱ID
		////		for(int boxTypeIndex = 1;boxTypeIndex <= 3;boxTypeIndex++){

		////			if(!IsBoxCouldBeInstall((BOX_INDEX)boxTypeIndex)){
		////				continue;
		////			}

		////			UEBOXTYPE boxType;

		////			switch(boxTypeIndex){
		////			case CHANGE_BOX_A:
		////				boxType = UEBOXTYPE::UE_BOXA;
		////				outRfid.bankNoteChangeRFID.bType = 0x60;
		////				break;
		////			case CHANGE_BOX_B:
		////				boxType = UEBOXTYPE::UE_BOXB;
		////				outRfid.bankNoteChangeRFID.bType = 0x60;
		////				break;
		////			case INVALID_BOX:
		////				boxType = UEBOXTYPE::UE_BOXC;
		////				outRfid.bankNoteChangeRFID.bType = 0x61;
		////				break;
		////			default:
		////				boxType = UEBOXTYPE::UE_BOXA;
		////			}

		////			//lRet = BH_RFID_HELPER->getRfid(boxType,outRfid);//lichao:暂时注释RFID的读写，后续再说。
		////			outRfid.bankNoteChangeRFID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		////			outRfid.bankNoteChangeRFID.bLineCode = 0x60;
		////			outRfid.bankNoteChangeRFID.wNum = 1;
		////			lRet = E_NO_ERROR;
		////			if(E_NO_ERROR == lRet){
		////				//CString strTemp(outRfid.byBoxID);
		////				SetBoxID((BOX_INDEX)boxTypeIndex,outRfid.bankNoteChangeRFID.ToString());
		////			}
		////			else{
		////				strReadRfidErrMsg = _T("读取纸币找零箱RFID失败！");
		////			}
		////		}

		////		return strReadRfidErrMsg;
		////	}
		////	break;
		////default:
		////	break;
		////}

		////UEBOXTYPE boxType;

		////switch(boxIndex){
		////case CHANGE_BOX_A:
		////	boxType = UEBOXTYPE::UE_BOXA;
		////	outRfid.bankNoteChangeRFID.bType = 0x60;
		////	break;
		////case CHANGE_BOX_B:
		////	boxType = UEBOXTYPE::UE_BOXB;
		////	outRfid.bankNoteChangeRFID.bType = 0x60;
		////	break;
		////case INVALID_BOX:
		////	boxType = UEBOXTYPE::UE_BOXC;
		////	outRfid.bankNoteChangeRFID.bType = 0x61;
		////	break;
		////default:
		////	boxType = UEBOXTYPE::UE_BOXA;
		////}
		//// 设置非所有票箱安装RFID读写设置
		////lRet = BH_RFID_HELPER->getRfid(boxType,outRfid);//lichao:暂时注释RFID的读写，后续再说。
		////outRfid.bankNoteChangeRFID.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		////outRfid.bankNoteChangeRFID.bType = 0x60;
		////outRfid.bankNoteChangeRFID.bNumInQu = theMAINTENANCE_INFO.GetEquNumberInGroup();
		////// 根据线路中的编号生成一个唯一的ID
		////outRfid.bankNoteChangeRFID.bNum = theSETTING.GetBNRChangeBoxId()/*(((theMAINTENANCE_INFO.GetStationCodeWithLine() % 100)&0xFF)<< 8) | ((((int)theMAINTENANCE_INFO.GetMachineCode()) * 5) & 0xFF)*/;


		//if(0 == lRet){
		//	//CString strTemp(outRfid.byBoxID);
		//	SetBoxID(boxIndex,outRfid.bankNoteChangeRFID.ToString());
		//}
		//else{
		//	strReadRfidErrMsg = _opl(TXT_BANKNOTEMGR_BOX_READ_RFID_FAILE);
		//}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		
	}
	return strReadRfidErrMsg;
}