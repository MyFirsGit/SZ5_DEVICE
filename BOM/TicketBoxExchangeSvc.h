#pragma once
#include "stdafx.h"
#include "BOMForeService.h"
#include "GStatic.h"

class CTicketBoxExchangeSvc:public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef struct _tagTest
	{

	} SC_RSP_RFID;

	typedef enum 
	{
		SRC_NOT_BUT_NEW_HAVE = 0,	//更换票箱之前没有票箱，之后安装上新的票箱
		SRC_HAVE_BUT_NEW_NOT,		//更换票箱之前存在票箱，之后没有安装上新的票箱， 即卸下票箱，未安装新票箱。
		SRC_NEW_SAME,				//更换票箱之前存在票箱，之后安装上票箱与原来的一样, 即票箱没有更换
		SRC_NEW_DIFFER,				//更换票箱之前存在票箱，之后安装上票箱与原来的不一样， 即更换了票箱。
		SRC_NEW_ALL_NOT,			//更换前后都不存在票箱。
		SRC_NEW_DIFFER_NUM			//更换前后，数量不一致
	} OP_BOX_STATUS;

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE=1,
		//	DIALOG_GROUP_INFO,
		DIALOG_GROUP_NEW,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfoBefore;
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfoBefore;
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxCInfoBefore;

		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfoAfter;
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfoAfter;
		CCTLOGCountInfo::TICKETBOX_INFO ticketBoxCInfoAfter;
		//SC_RSP_READ_STATUS statusBeforeExchange;
		TH_RSP_GET_DEV_STATUS statusAfterExchange;
		TICKETBOX_OPERATION_TYPE moveType;
		map<BYTE,bool> moveBoxInfo;
		map<BYTE,bool> moveBoxBtnInfo;
	} DataModel;

	CTicketBoxExchangeSvc();
	~CTicketBoxExchangeSvc();
	DataModel& GetDataModel();
	void SetMoveType(TICKETBOX_OPERATION_TYPE moveType);
	void GetMasterBoxInfo();


	bool CheckTicketBoxIsInvalidExchanged();
	void RefreshTicketboxStatus();
	void DetachBox();
	bool AttachBox();
	void GetNewBoxInfo();
	void OnGetBoxInfo();


	bool THStartWork();
	//void MoveBox(SC_RSP_READ_STATUS&,BYTE actionType);

	void OnDetachBox();
	void OnAttachBox();

protected:

	void OnStart();

private:

	DataModel m_DataModel;

	LABEL_GROUP_THREE m_TicketInfoLabel[LABEL_GROUP_EDIT_ROWS_6];

	void GetActualTicketboxInfo(TH_RSP_GET_DEV_STATUS&);

	bool IsSingleTicketBoxExchanged(bool actualBoxExists, const CCTLOGCountInfo::TICKETBOX_ID &actualTicketBoxID, const CCTLOGCountInfo::TICKETBOX_INFO &saveBoxInfo,OP_BOX_STATUS& ticketBoxOperationType);

	void UpdateMasterFromRFID();
	void PrinterPrint();
	int  SaveTicketboxExchangeUD(TICKETBOX_OPERATION_TYPE type);

	MAGAZINE_NUM_STATUS TH2SCMagazineStatus(MAGAZINE_PALACE boxId);
	MAGAZINE_NUM_STATUS TH2SCCollectionBoxStatus();

};
