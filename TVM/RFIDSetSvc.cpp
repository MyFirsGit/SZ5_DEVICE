#include "stdafx.h"
#include "RFIDSetSvc.h"
#include "TimeSetBaseOfficeDlg.h"
#include "GuideInfo.h"
#include "CardBoxManageSvc.h"
#include "rfidsetbaseofficedlg.h"

BEGIN_MESSAGE_MAP(CRFIDModeSetSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRFIDModeSetSvc::CRFIDModeSetSvc()
:CTVMForeService(SYSTEM_TIME_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_20122_RFID_MODE_SET_DLG,new CRFIDSetBaseOfficeDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_RFID_MODE_SET, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RFID_MODE_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_RFID_MODE_SET, IDD_20122_RFID_MODE_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_RFID_MODE_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRFIDModeSetSvc::~CRFIDModeSetSvc()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前TH模式

@param      无

@retval     int		0 - 正常模式，1 - 缩退模式

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CRFIDModeSetSvc::GetCurrentRFIDMode()
{
	//m_sRfidMode = theSETTING.GetRFIDWriteStatus();
	return m_sRfidMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置TH模式

@param      (i)int    sThMode  TH模式

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRFIDModeSetSvc::SetRFIDMode(int sThMode)
{
	m_sRfidMode = sThMode;
	if(sThMode == RFID_MODE_READONLY){
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfo;	
		BYTE byBoxIDA[4] = {0x00,0x00,0x00,0x01};
		ticketBoxAInfo.ticketBoxID.Deserialize(byBoxIDA);
		ticketBoxAInfo.ulCurCount = 500;
		ticketBoxAInfo.ulInitCount =  500;
		theCOUNT_INFO.SetTicketboxAInfo(ticketBoxAInfo);

		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfo;	
		BYTE byBoxIDB[4] = {0x00,0x00,0x00,0x02};
		ticketBoxBInfo.ticketBoxID.Deserialize(byBoxIDB);
		ticketBoxBInfo.ulCurCount = 500;
		ticketBoxBInfo.ulInitCount =  500;
		theCOUNT_INFO.SetTicketboxBInfo(ticketBoxBInfo);

		//tTIMCardNum ticketData;
		//ticketData.wMagazineANumber = 500;
		//ticketData.wMagazineBNumber = 500;
		//ticketData.wCollectionBoxNumber = 0;
//		CARD_HELPER->TicketDataWriteCMD(ticketData);
	}
	else{
		//CTicketBoxExchangeSvc pSvc;
		//pSvc.CheckTicketBoxIsInvalidExchanged();	
	}
	//theSETTING.SetRFIDWriteStatus(m_sRfidMode);
}