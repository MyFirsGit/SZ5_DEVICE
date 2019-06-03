#include "stdafx.h"
#include "TicketBoxExchangeSvc.h"
#include "TicketBoxInfoDlg.h"
#include "guideinfo.h"
#include "Util.h"
#include "templatehelper.h"
#include "lptmonitorsvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_BASE_TO_NEW              = _T("WM_BASE_TO_NEW");                   // 基本画面
static const TCHAR* const WM_NEW_TO_FINISH            = _T("WM_NEW_TO_FINISH");                 // 结束画面
static const TCHAR* const WM_NEW_TO_ERROR             = _T("WM_NEW_TO_ERROR");                  // 失败画面
static const TCHAR* const WM_BASE_TO_ERROR			  = _T("WM_BASE_TO_ERROR");					// 错误画面
static const TCHAR* const WM_BASE_TO_FINISH			  = _T("WM_BASE_TO_FINISH");			    // 结束画面

BEGIN_MESSAGE_MAP(CTicketBoxExchangeSvc, CBOMForeService)
	
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
///**
//@brief      构造函数
//
//@param      (i)CService* pService
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxExchangeSvc::CTicketBoxExchangeSvc()
:CBOMForeService(TICKETBOX_EXCHANGE_SVC)
{
	m_pDialogFlow->AddDialog(IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG, new CTicketBoxInfoDlg(this));
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_TICKETBOX_CHANGE_FINISH));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG);
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_NEW, _T("请输入票箱张数信息后按确认键")/*theGUIDE_INFO.GetOperationMessage(GUIDE_TICKETBOX_INPUT_BUSIMESS_INFO)*/);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_NEW, IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG);
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_TICKETBOX_BUSIMESS_FINISH));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG);
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR,_T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_NEW,DIALOG_GROUP_NEW);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_NEW,WM_NEW_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_NEW,WM_NEW_TO_ERROR,DIALOG_GROUP_ERROR);

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}



////////////////////////////////////////////////////////////////////////////
///**
//@brief      析构函数
//
//@param      none
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxExchangeSvc::~CTicketBoxExchangeSvc(void)
{
}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::OnStart()
{
	GetMasterBoxInfo();
	__super::OnStart();
}

void CTicketBoxExchangeSvc::GetMasterBoxInfo()
{
	m_DataModel.ticketBoxAInfoBefore = theCOUNT_INFO.GetTicketboxAInfo();
	m_DataModel.ticketBoxBInfoBefore = theCOUNT_INFO.GetTicketboxBInfo();
	m_DataModel.ticketBoxCInfoBefore = theCOUNT_INFO.GetTicketboxCInfo();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得服务中的数据模型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::SetMoveType(TICKETBOX_OPERATION_TYPE moveType)
{
	try{
		m_DataModel.moveType = moveType;
		m_DataModel.moveBoxBtnInfo.clear();
		CCTLOGCountInfo::TICKETBOX_ID emptyTicketBoxID;
		GetMasterBoxInfo();
		if (moveType==DETACH){
			m_DataModel.moveBoxBtnInfo[1] = m_DataModel.ticketBoxAInfoBefore.ticketBoxID == emptyTicketBoxID? false:true;
			m_DataModel.moveBoxBtnInfo[2] = m_DataModel.ticketBoxBInfoBefore.ticketBoxID == emptyTicketBoxID? false:true;
			m_DataModel.moveBoxBtnInfo[3] = m_DataModel.ticketBoxCInfoBefore.ticketBoxID == emptyTicketBoxID? false:true;
		} 
		else{
			m_DataModel.moveBoxBtnInfo[1] = m_DataModel.ticketBoxAInfoBefore.ticketBoxID == emptyTicketBoxID? true:false;
			m_DataModel.moveBoxBtnInfo[2] = m_DataModel.ticketBoxBInfoBefore.ticketBoxID == emptyTicketBoxID? true:false;
			m_DataModel.moveBoxBtnInfo[3] = m_DataModel.ticketBoxCInfoBefore.ticketBoxID == emptyTicketBoxID? true:false;
		}
		NotifyDataModelChanged();
	}
	catch (CSysException& e){
		theSERVICE_MGR.SetState(FINISH);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theSERVICE_MGR.SetState(FINISH);
		CInnerException e(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得服务中的数据模型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTicketBoxExchangeSvc::DataModel& CTicketBoxExchangeSvc::GetDataModel()
{
	return m_DataModel;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::OnDetachBox()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
	
		// 卸下票箱
		DetachBox();
		// 打印更换票箱凭证
		//PrinterPrint();		
		GetMasterBoxInfo();
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException& e){
		theSERVICE_MGR.SetState(FINISH);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theSERVICE_MGR.SetState(FINISH);
		CInnerException e(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::OnGetBoxInfo()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
	    CCTLOGCountInfo::TICKETBOX_ID emptyTicketBoxID;
		// 获取新票箱信息
		GetNewBoxInfo();	
		// 不按顺序的原因：出票机状态中的A和B与出票机上贴的AB相反，因此代码应该写反
		m_DataModel.moveBoxBtnInfo[1] = m_DataModel.statusAfterExchange.bBoxAArrive&&(m_DataModel.ticketBoxAInfoBefore.ticketBoxID == emptyTicketBoxID);
		m_DataModel.moveBoxBtnInfo[2] = m_DataModel.statusAfterExchange.bBoxBArrive&&(m_DataModel.ticketBoxBInfoBefore.ticketBoxID == emptyTicketBoxID);
		m_DataModel.moveBoxBtnInfo[3] = m_DataModel.statusAfterExchange.bBoxCArrive&&(m_DataModel.ticketBoxCInfoBefore.ticketBoxID == emptyTicketBoxID);
		theSERVICE_MGR.SetState(FINISH);
		// 转至更换界面
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_NEW);	
	}
	catch (CSysException& e){
		theSERVICE_MGR.SetState(FINISH);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theSERVICE_MGR.SetState(FINISH);
		CInnerException e(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::OnAttachBox()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
		// 装上票箱
		AttachBox();
		// 打印更换票箱凭证
		//PrinterPrint();
		// 删除非法更换票箱异常
/*		theEXCEPTION_MGR.RemoveException(CTHException::MODULE_ID,CTHException::INVALID_EXCHANGE);	*/	
		// 转至更换结束界面
		m_pDialogFlow->ShowNextDialogGroup(WM_NEW_TO_FINISH);
		theAPP_SESSION.ShowOfficeGuide(_T("票箱安装完成。")/*_tl(GUIDE_TICKETBOX_INSTALL_COMPLETE)*/);
	}
	catch (CSysException& e){
		theSERVICE_MGR.SetState(FINISH);
		m_pDialogFlow->ShowNextDialogGroup(WM_NEW_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theSERVICE_MGR.SetState(FINISH);
		CInnerException e(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		m_pDialogFlow->ShowNextDialogGroup(WM_NEW_TO_ERROR);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	theSERVICE_MGR.SetState(FINISH);
}

void CTicketBoxExchangeSvc::DetachBox()
{
	//GetActualTicketboxInfo(m_DataModel.statusBeforeExchange);
	SaveTicketboxExchangeUD(DETACH);	
	UpdateMasterFromRFID();
	RefreshTicketboxStatus();
	//if (m_DataModel.moveBoxInfo[1] && m_DataModel.moveBoxInfo[2])
	//{
	//	GetTHHelper()->TestSetting(CMD_TEST_AB_TRAY_DOWN);
	//} 
	//else
	{
		if (m_DataModel.moveBoxInfo[1])
		{
			GetTHHelper()->TestSetting(CMD_TEST_A_TRAY_DOWN);
		}
		if (m_DataModel.moveBoxInfo[2])
		{
			GetTHHelper()->TestSetting(CMD_TEST_B_TRAY_DOWN);
		}
	}
	
	//MoveBox(m_DataModel.statusBeforeExchange,DETACH);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      装上票箱

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::AttachBox()
{
	UpdateMasterFromRFID();
	SaveTicketboxExchangeUD(ATTACH);
	RefreshTicketboxStatus();
	//if (m_DataModel.moveBoxInfo[1] && m_DataModel.moveBoxInfo[2])
	//{
	//	GetTHHelper()->TestSetting(CMD_TEST_AB_TRAY_UP);
	//} 
	//else
	{
		if (m_DataModel.moveBoxInfo[1])
		{
			GetTHHelper()->TestSetting(CMD_TEST_A_TRAY_UP);
		}
		if (m_DataModel.moveBoxInfo[2])
		{
			GetTHHelper()->TestSetting(CMD_TEST_B_TRAY_UP);
		}
	}
	
    //MoveBox(m_DataModel.statusAfterExchange,ATTACH);
	THStartWork();
	return true;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief     开始工作
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::THStartWork()
{
	try{
		//m_pTHHelper->EnableWork(TH_BEGIN_WORK);
		return true;
	}catch(...){
		throw;
	}
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief     移动票箱
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//void CTicketBoxExchangeSvc::MoveBox(SC_RSP_READ_STATUS&status,BYTE actionType)
//{
//	try{
//		SC_MOVE_BOX_CMD_INFO move_box_info;		
//		if (actionType == DETACH){
//			if (m_DataModel.moveBoxInfo[1]&&(status.Box1FrettingSensorFlag||status.Box1UpLimitSensorFlag))
//			{// A票箱
//				move_box_info.actionType = actionType;
//				move_box_info.boxId = MAGAZINE_A;
//				//m_pTHHelper->MoveBox(move_box_info);
//			}
//			if (m_DataModel.moveBoxInfo[2]&&(status.Box2FrettingSensorFlag||status.Box2UpLimitSensorFlag))
//			{// A票箱
//				move_box_info.actionType = actionType;
//				move_box_info.boxId = MAGAZINE_B;
//				//m_pTHHelper->MoveBox(move_box_info);
//			}
//		}
//		else if (actionType == ATTACH){
//			if (m_DataModel.moveBoxInfo[1]&&status.Box1LowLimitSensorFlag)
//			{	// A票箱
//				move_box_info.actionType = actionType;
//				move_box_info.boxId = MAGAZINE_A;
//				//m_pTHHelper->MoveBox(move_box_info);
//			}
//			if (m_DataModel.moveBoxInfo[2]&&status.Box2LowLimitSensorFlag)
//			{	// B票箱
//				move_box_info.actionType = actionType;
//				move_box_info.boxId = MAGAZINE_B;
//				//m_pTHHelper->MoveBox(move_box_info);
//			}
//		}
//	}catch(...){
//		throw;
//	}
//}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::GetNewBoxInfo()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
		GetActualTicketboxInfo(m_DataModel.statusAfterExchange);
		char deviceType = 0x20;
		//SC_RES_READ_RFID readinfo;
		// 票箱A编号
		//readinfo.boxType = RFID_MAGAZINE_A;
		//bool readBoxA = m_pTHHelper->ReadTicketBoxRFID(readinfo);	
		//if (readBoxA){
			//m_DataModel.ticketBoxAInfoAfter.ticketBoxID.bLineCode = readinfo.RFIDData.byDeviceID[0];
			//m_DataModel.ticketBoxAInfoAfter.ticketBoxID.bType = readinfo.RFIDData.byDeviceID[1];
			//m_DataModel.ticketBoxAInfoAfter.ticketBoxID.wNum = ComMakeWORD(readinfo.RFIDData.byDeviceID[2],readinfo.RFIDData.byDeviceID[3]);
		m_DataModel.ticketBoxAInfoAfter.ticketBoxID.bType = 01;
		m_DataModel.ticketBoxAInfoAfter.ticketBoxID.bStationID = char2BCD((BYTE)theMAINTENANCE_INFO.GetStationCode());
		m_DataModel.ticketBoxAInfoAfter.ticketBoxID.bNum = theMAINTENANCE_INFO.GetMachineCode()* 2 - 2;
		//}
		// 票箱B编号
		//readinfo.boxType = RFID_MAGAZINE_B;
		//bool readBoxB = m_pTHHelper->ReadTicketBoxRFID(readinfo);
		//if (readBoxB){	
		m_DataModel.ticketBoxBInfoAfter.ticketBoxID.bType = 01;
		m_DataModel.ticketBoxBInfoAfter.ticketBoxID.bStationID = char2BCD((BYTE)theMAINTENANCE_INFO.GetStationCode());
		m_DataModel.ticketBoxBInfoAfter.ticketBoxID.bNum = theMAINTENANCE_INFO.GetMachineCode()* 2 - 1;
		//}

		// 废票箱编号
		//readinfo.boxType = RFID_MAGAZINE_C;
		//bool readBoxC = m_pTHHelper->ReadTicketBoxRFID(readinfo);
		//if (readBoxC){	
		m_DataModel.ticketBoxCInfoAfter.ticketBoxID.bType = 02;
		m_DataModel.ticketBoxCInfoAfter.ticketBoxID.bStationID = char2BCD((BYTE)theMAINTENANCE_INFO.GetStationCode());
		m_DataModel.ticketBoxCInfoAfter.ticketBoxID.bNum = theMAINTENANCE_INFO.GetMachineCode()* 2;
		//}
		theSERVICE_MGR.SetState(FINISH);

	}catch(...){
		throw;
	}
}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      一个票箱是否与本地保存不一致
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::IsSingleTicketBoxExchanged(bool actualBoxExists, const CCTLOGCountInfo::TICKETBOX_ID &actualTicketBoxID, const CCTLOGCountInfo::TICKETBOX_INFO &saveBoxInfo,OP_BOX_STATUS& ticketBoxOperationType)
{
	bool hasChange = false;
	if(actualBoxExists)//实际票箱存在
	{
		if(saveBoxInfo.ticketBoxID == CCTLOGCountInfo::TICKETBOX_ID())//保存的票箱不存在
		{
			ticketBoxOperationType = SRC_NOT_BUT_NEW_HAVE;
			hasChange = true;
		}
		//票箱号一致，张数一致
		else if(actualTicketBoxID  == saveBoxInfo.ticketBoxID /*&& actualBoxInfo.sTicketNumber == saveBoxInfo.ulInitCount*/)
		{
			ticketBoxOperationType = SRC_NEW_SAME;
		}
		else
		{
			ticketBoxOperationType = SRC_NEW_DIFFER;
			hasChange = true;
		}
	}
	else//实际票箱不存在
	{
		if(saveBoxInfo.ticketBoxID == CCTLOGCountInfo::TICKETBOX_ID())//保存的票箱不存在
		{
			ticketBoxOperationType = SRC_NEW_ALL_NOT;
		}
		else
		{
			ticketBoxOperationType = SRC_NOT_BUT_NEW_HAVE;
			hasChange = true;
		}
	}
	return hasChange;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      取得票箱的实际信息
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::GetActualTicketboxInfo(TH_RSP_GET_DEV_STATUS &status)
{	
	try{
		m_pTHHelper->GetDevStatus(&status);
	}
	catch(CTHHardwareException& e)
	{
		throw e;
	}
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      将实际票箱信息保存在本地
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::UpdateMasterFromRFID()
{
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfo;
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfo;
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxCInfo;

	CCTLOGCountInfo::TICKETBOX_ID emptyBoxID;
	if (m_DataModel.moveType == DETACH)
	{
		if(m_DataModel.moveBoxInfo[1])	theCOUNT_INFO.SetTicketboxAInfo(ticketBoxAInfo);
		if(m_DataModel.moveBoxInfo[2])	theCOUNT_INFO.SetTicketboxBInfo(ticketBoxBInfo);
		if(m_DataModel.moveBoxInfo[3])	theCOUNT_INFO.SetTicketboxCInfo(ticketBoxCInfo);
	}
	else{
		if(m_DataModel.moveBoxInfo[1]){
			ticketBoxAInfo.ticketBoxID = m_DataModel.ticketBoxAInfoAfter.ticketBoxID;
			ticketBoxAInfo.ulCurCount = m_DataModel.moveBoxInfo[1] == SET ? m_DataModel.ticketBoxAInfoAfter.ulCurCount : 0;
			ticketBoxAInfo.ulInitCount = m_DataModel.moveBoxInfo[1] == SET ? m_DataModel.ticketBoxAInfoAfter.ulInitCount : 0;
			theCOUNT_INFO.SetTicketboxAInfo(ticketBoxAInfo);
		}

		if(m_DataModel.moveBoxInfo[2]){
			ticketBoxBInfo.ticketBoxID = m_DataModel.ticketBoxBInfoAfter.ticketBoxID;
			ticketBoxBInfo.ulCurCount = m_DataModel.moveBoxInfo[2] == SET ? m_DataModel.ticketBoxBInfoAfter.ulCurCount:0;
			ticketBoxBInfo.ulInitCount =  m_DataModel.moveBoxInfo[2] == SET ? m_DataModel.ticketBoxBInfoAfter.ulInitCount:0;
			theCOUNT_INFO.SetTicketboxBInfo(ticketBoxBInfo);
		}
		if(m_DataModel.moveBoxInfo[3]){
			ticketBoxCInfo.ticketBoxID = m_DataModel.ticketBoxCInfoAfter.ticketBoxID;
			ticketBoxCInfo.ulCurCount = m_DataModel.moveBoxInfo[3] == SET ? m_DataModel.ticketBoxCInfoAfter.ulCurCount:0;
			ticketBoxCInfo.ulInitCount =  m_DataModel.moveBoxInfo[3] == SET ? m_DataModel.ticketBoxCInfoAfter.ulInitCount:0;
			theCOUNT_INFO.SetTicketboxCInfo(ticketBoxCInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     保存票箱更换业务数据 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int  CTicketBoxExchangeSvc::SaveTicketboxExchangeUD(/*SC_RSP_READ_STATUS& status, */TICKETBOX_OPERATION_TYPE operationType)
{
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfo;
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfo;
	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxCInfo;
	if (operationType == DETACH ){
		ticketBoxAInfo = m_DataModel.ticketBoxAInfoBefore;
		ticketBoxBInfo = m_DataModel.ticketBoxBInfoBefore;
		ticketBoxCInfo = m_DataModel.ticketBoxCInfoBefore;
	}
	if (operationType == ATTACH){
		ticketBoxAInfo = m_DataModel.ticketBoxAInfoAfter;
		ticketBoxBInfo = m_DataModel.ticketBoxBInfoAfter;
		ticketBoxCInfo = m_DataModel.ticketBoxCInfoAfter;
	}

	if(m_DataModel.moveBoxInfo[1]){
		MAGAZINE_CHANGE change;
		change.operType =  operationType;
		change.magazineId = ticketBoxAInfo.ticketBoxID;
		change.magazne =MAGAZINEA;
		change.cardType = ULTRALIGHT;
		change.providerId =  ACC;
		change.cardProductType = ComMakeWORD(98,00);
		change.preProperty = 3;
		change.childproductId = 0;
		change.CardCount = ticketBoxAInfo.ulCurCount;
		change.magazineStatus = operationType == ATTACH? VALID_ATTACH:VALID_DETACH;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	}
	if(m_DataModel.moveBoxInfo[2]){
		MAGAZINE_CHANGE change;
		change.operType =  operationType;
		change.magazineId = ticketBoxBInfo.ticketBoxID;
		change.magazne =MAGAZINEB;
		change.cardType = ULTRALIGHT;
		change.providerId =  ACC;
		change.cardProductType = ComMakeWORD(98,00);
		change.preProperty = 3;
		change.childproductId = 0;
		change.CardCount = ticketBoxBInfo.ulCurCount;
		change.magazineStatus = operationType == ATTACH? VALID_ATTACH:VALID_DETACH;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	}
	if(m_DataModel.moveBoxInfo[3]){
		MAGAZINE_CHANGE change;
		change.operType =  operationType;
		change.magazineId = ticketBoxCInfo.ticketBoxID;
		change.magazne =MAGAZINEC;
		change.cardType = ULTRALIGHT;
		change.providerId =  ACC;
		change.cardProductType = ComMakeWORD(98,00);
		change.preProperty = 3;
		change.childproductId = 0;
		change.CardCount = ticketBoxCInfo.ulCurCount;
		change.magazineStatus = operationType == ATTACH? VALID_ATTACH:VALID_DETACH;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////
///*
//@brief      更新BOM状态管理器中的票箱状态
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::RefreshTicketboxStatus()
{
	// 设置操作标志
	bool setSign = UN_SET;
	if (m_DataModel.moveType == ATTACH)
	{
		setSign = SET;
	} 
	// 分别判断该按钮是否被选中，选中则进行下一步判断，未选中不做处理
	if (m_DataModel.moveBoxInfo[1])
	{
		theBOM_STATUS_MGR.SetMagAStatus(m_DataModel.moveBoxInfo[1] == setSign?STATUS_MAGAZINE_SET:STATUS_MAGAZINE_UNSET);
		theBOM_STATUS_MGR.SetMagANumStatus(m_DataModel.moveBoxInfo[1] == setSign? TH2SCMagazineStatus(MAGAZINEA) : MAG_NUM_EMPTY);
	}
	if (m_DataModel.moveBoxInfo[2])
	{
		theBOM_STATUS_MGR.SetMagBStatus(m_DataModel.moveBoxInfo[2] == setSign?STATUS_MAGAZINE_SET:STATUS_MAGAZINE_UNSET);
		theBOM_STATUS_MGR.SetMagBNumStatus(m_DataModel.moveBoxInfo[2] == setSign? TH2SCMagazineStatus(MAGAZINEB) : MAG_NUM_EMPTY);
	}
	if (m_DataModel.moveBoxInfo[3])
	{
		theBOM_STATUS_MGR.SetCycleBoxStatus(m_DataModel.moveBoxInfo[3] == setSign?STATUS_MAGAZINE_SET:STATUS_MAGAZINE_UNSET);
		theBOM_STATUS_MGR.SetCycleBoxNumStatus(m_DataModel.moveBoxInfo[3] == setSign?TH2SCCollectionBoxStatus():MAG_NUM_NEAR_FULL);
	}

}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CTicketBoxExchangeSvc::TH2SCMagazineStatus(MAGAZINE_PALACE boxId)
{	
	CAfcBomOperationParam::MAGAZINE_PARAM temParam;
	theAFC_BOM_OPERATION.GetMagazineParam(temParam);
	TH_RSP_GET_DEV_STATUS thMagazineStatus = m_DataModel.statusAfterExchange;
	if (boxId == MAGAZINEA ){
		if (m_DataModel.ticketBoxAInfoAfter.ulCurCount <= temParam.haveEmptyValue || thMagazineStatus.cCassetteAStatus == 2){ 
			return MAG_NUM_EMPTY;
		}
		else if (m_DataModel.ticketBoxAInfoAfter.ulCurCount <= temParam.willEmptyValue || thMagazineStatus.cCassetteAStatus == 1){
			return MAG_NUM_NEAR_EMPTY;
		} 
		else {
			return MAG_NUM_NORMAL;
		}
	}
	else{
		if (m_DataModel.ticketBoxBInfoAfter.ulCurCount <= temParam.haveEmptyValue || thMagazineStatus.cCassetteBStatus == 2){ 
			return MAG_NUM_EMPTY;
		}
		else if (m_DataModel.ticketBoxBInfoAfter.ulCurCount <= temParam.willEmptyValue || thMagazineStatus.cCassetteBStatus == 1){
			return MAG_NUM_NEAR_EMPTY;
		} 
		else {
			return MAG_NUM_NORMAL;
		}
	}	
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CTicketBoxExchangeSvc::TH2SCCollectionBoxStatus()
{
	TH_RSP_GET_DEV_STATUS thMagazineStatus = m_DataModel.statusAfterExchange;
	if (thMagazineStatus.cRejectCassetteStatus == 2){ 
		return MAG_NUM_FULL;
	}
	else {
		return MAG_NUM_NORMAL;
	} 
}

//////////////////////////////////////////////////////////////////////////
/*
@brief       检查票箱是否为非法更换票箱

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketBoxExchangeSvc::CheckTicketBoxIsInvalidExchanged()
{
	//if(theSETTING.GetRFIDWriteStatus()==0){
	//	return false;
	//}
	TH_RSP_GET_DEV_STATUS status;
	GetActualTicketboxInfo(status);
	//SC_RES_READ_RFID readinfo;
	// 票箱A编号
	//readinfo.boxType = RFID_MAGAZINE_A;
	//bool readBoxA = m_pTHHelper->ReadTicketBoxRFID(readinfo);	
	//if (readBoxA){
	//	m_DataModel.ticketBoxAInfoBefore.ticketBoxID.bLineCode = readinfo.RFIDData.byDeviceID[0];
	//	m_DataModel.ticketBoxAInfoBefore.ticketBoxID.bType = readinfo.RFIDData.byDeviceID[1];
	//	m_DataModel.ticketBoxAInfoBefore.ticketBoxID.wNum = ComMakeWORD(readinfo.RFIDData.byDeviceID[2],readinfo.RFIDData.byDeviceID[3]);
	//}
	// 票箱B编号
	//readinfo.boxType = RFID_MAGAZINE_B;
	//bool readBoxB = m_pTHHelper->ReadTicketBoxRFID(readinfo);
	//if (readBoxB){	
	//	m_DataModel.ticketBoxBInfoBefore.ticketBoxID.bLineCode = readinfo.RFIDData.byDeviceID[0];
	//	m_DataModel.ticketBoxBInfoBefore.ticketBoxID.bType = readinfo.RFIDData.byDeviceID[1];
	//	m_DataModel.ticketBoxBInfoBefore.ticketBoxID.wNum = ComMakeWORD(readinfo.RFIDData.byDeviceID[2],readinfo.RFIDData.byDeviceID[3]);
	//}
	// 废票箱编号
	//readinfo.boxType = RFID_MAGAZINE_C;
	//bool readBoxC = m_pTHHelper->ReadTicketBoxRFID(readinfo);
	//if (readBoxC){	
	//	m_DataModel.ticketBoxCInfoBefore.ticketBoxID.bLineCode = readinfo.RFIDData.byDeviceID[0];
	//	m_DataModel.ticketBoxCInfoBefore.ticketBoxID.bType = readinfo.RFIDData.byDeviceID[1];
	//	m_DataModel.ticketBoxCInfoBefore.ticketBoxID.wNum = ComMakeWORD(readinfo.RFIDData.byDeviceID[2],readinfo.RFIDData.byDeviceID[3]);
	//}
	
	OP_BOX_STATUS ticketBoxAOperationType;
	OP_BOX_STATUS ticketBoxBOperationType;
	OP_BOX_STATUS ticketBoxCOperationType;
	bool isBoxAExchanged = IsSingleTicketBoxExchanged(status.bBoxAArrive,m_DataModel.ticketBoxAInfoBefore.ticketBoxID,theCOUNT_INFO.GetTicketboxAInfo(),ticketBoxAOperationType);
	bool isBoxBExchanged = IsSingleTicketBoxExchanged(status.bBoxBArrive,m_DataModel.ticketBoxBInfoBefore.ticketBoxID,theCOUNT_INFO.GetTicketboxBInfo(),ticketBoxBOperationType);
	bool isBoxCExchanged = IsSingleTicketBoxExchanged(status.bBoxCArrive,m_DataModel.ticketBoxCInfoBefore.ticketBoxID,theCOUNT_INFO.GetTicketboxCInfo(),ticketBoxCOperationType);
	bool isInvalidExchanged =  isBoxAExchanged || isBoxBExchanged ||isBoxCExchanged;
	if(!isInvalidExchanged){
		//UpdateRFIDFromMaster(status,rfid);
		//SyncTHTicketboxInfo();
	}
	else{
		//throw CTHException(CTHException::INVALID_EXCHANGE, _T(__FILE__),__LINE__);
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief   打印凭证   
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxExchangeSvc::PrinterPrint()
{
	CString templateName= _T("TICKETBOX_EXCHANGE.template");
	CStringArray printArr;
	for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("TICKETBOX_EXCHANGE"));printIndex++)
	{
		CStringArray printTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
		const int hspCount = 20;
		int i=0;
		while(i<printTexts.GetCount())
		{
			CString& line = printTexts.ElementAt(i);
			line.Replace(_T("{TICKETBOXA_ID_BEFORE}"),m_DataModel.ticketBoxAInfoBefore.ticketBoxID.ToString());
			line.Replace(_T("{TICKETBOXA_COUNT_BEFORE}"),ComFormatCount(m_DataModel.ticketBoxAInfoBefore.ulCurCount));
			line.Replace(_T("{TICKETBOXB_ID_BEFORE}"),m_DataModel.ticketBoxBInfoBefore.ticketBoxID.ToString());
			line.Replace(_T("{TICKETBOXB_COUNT_BEFORE}"),ComFormatCount(m_DataModel.ticketBoxBInfoBefore.ulCurCount));

			line.Replace(_T("{TICKETBOXA_ID_AFTER}"),m_DataModel.ticketBoxAInfoAfter.ticketBoxID.ToString());
			line.Replace(_T("{TICKETBOXA_COUNT_AFTER}"),ComFormatCount(m_DataModel.ticketBoxAInfoAfter.ulCurCount));
			line.Replace(_T("{TICKETBOXB_ID_AFTER}"),m_DataModel.ticketBoxBInfoAfter.ticketBoxID.ToString());
			line.Replace(_T("{TICKETBOXB_COUNT_AFTER}"),ComFormatCount(m_DataModel.ticketBoxBInfoAfter.ulCurCount));

			line.Replace(_T("{TRANS_TIME}"),ComGetCurTime().ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
			line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
			line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
			line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
			CStringArray printLine;
			ComPrintLineFormatForTemplate(line,printLine);
			printArr.Append(printLine);
			i++;
		}
	}
	m_pPRTHelper->PrinterPrint(&printArr,false);

}
