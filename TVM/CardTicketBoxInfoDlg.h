#pragma once
#include "OperationDlg.h"
#include "CardBoxManageSvc.h"

#define UD_MAX_ROW	15				//定义基本显示区最多显示行
#define UD_MAX_COL	2				//定义基本数据区显示的列数

class CTicketBoxInfoDlg: public COperationDlg
{
	DECLARE_DYNAMIC(CTicketBoxInfoDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTicketBoxInfoDlg(CService* pService);          // 标准构造函数
	~CTicketBoxInfoDlg();

	enum { IDD = IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG };          // 对话框数据

	typedef enum _UE_CURSTATUS
	{
		UE_START=1,			//选择安装或者卸载界面
		UE_INSTALL,			//安装阶段
		UE_UNINSTALL,		//卸载阶段
		UE_INSTALLFINISH,	//安装完成界面
		UE_UNINSTALLFINISH	//卸载完成界面
	}UE_CURSTATUS;			//当前维护阶段

	typedef enum _UE_NOTICEID
	{

		UE_ATTACH=TXT_SMARTCARDMGR_ATTACH,
		UE_DETACH=TXT_SMARTCARDMGR_DETACH,				//卸载票箱文言提示

		UE_CARD_ID=TXT_SMARTCARDMGR_BOX_ID,				//取票箱ID提示文言
		UE_CARD_QUANTITY=TXT_SMARTCARDMGR_CARD_QUANTITY,		//取票箱数量提示文言
		UE_WASTED_CARD_ID=TXT_SMARTCARDMGR_WASTED_ID,		//取废票箱ID提示文言

		UE_BEFORE_ATTACH=TXT_SMARTCARDMGR_BEFORE_ATTACH,		//安装前提示文言
		UE_AFTER_ATTACH=TXT_SMARTCARDMGR_AFTER_ATTACH,		//安装后提示文言

		UE_BEFORE_DETACH=TXT_SMARTCARDMGR_BEFORE_DETACH,
		UE_AFTER_DETACH=TXT_SMARTCARDMGR_AFTER_DETACH,

		UE_QUANTITY=TXT_SMARTCARDMGR_QUANTITY,			//数量提示文言

		UE_DETACH_CARD=TXT_SMARTCARDMGR_DETACH_BOX,			//卸载票箱提示文言
		UE_DETACH_WASTED_CARD=TXT_SMARTCARDMGR_DETACH_WASTEDBOX,	//卸载废票箱提示文言
		UE_DETACH_ALL=TXT_SMARTCARDMGR_DETACH_ALL,			//卸载所有票箱提示文言
		UE_ATTACHED=TXT_SMARTCARDMGR_ATTACHED,			//已经安装提示文言
		UE_DETACHED=TXT_SMARTCARDMGR_DETACHED,			//已经卸载提示文言

		UE_ERROR_CARDBOX_ID=TXT_SMARTCARDMGR_INVALID_BOXID,	//错误的票箱ID
		UE_ERROR_WASTEDBOX_ID=TXT_SMARTCARDMGR_INVALID_WASTEDID,	//错误的废票箱ID
		UE_ERROR_CARDBOX_QUANTITY=TXT_SMARTCARDMGR_INVALID_CARD_QUANTITY,//错误的票卡数量
		UE_ERROR_WASTEDBOX_QUANTITY=TXT_SMARTCARDMGR_INVALID_WASTED_QUANTITY,//错误的废票数量

		UE_NOTICE_WAITTOATTACH=TXT_SMARTCARDMGR_WAITTOATTACH,					//待安装
		UE_NOTICE_WAITTODETACH=TXT_SMARTCARDMGR_WAITTODETACH,					//待卸载
		UE_NOTICE_ATTACHBOXFAILED=TXT_SMARTCARDMGR_BOXATTACHERROR,				//安装票箱错误
		UE_NOTICE_ATTACHWASTEDFAILED=TXT_SMARTCARDMGR_WASTEDBOXATTACHERROR,		//安装废票箱错误
		UE_NOTICE_NOTFOUNDBOX=TXT_SMARTCARDMGE_NOTFOUNDBOX,						//没有安装票箱
		UE_NOTICE_NOTFOUNDWASTED=TXT_SMARTCARDMGR_NOTFOUNDWASTED,				//没有安装废票箱
		UE_NOTICE_NOTFOUNDANYBOX=TXT_SMARTCARDMGR_NOTFOUNANYBOX					//没有安装任何票箱
	}UE_NOTICEID;				//界面文言定义

	typedef enum _UE_INPUTSTATUS
	{
		UE_INPUT_CARD_ID=1,			//输入票箱ID阶段
		UE_INPUT_CARD_QUANTITY,		//输入票数量阶段
		UE_INPUT_WASTEDCARD_ID,		//输入废票箱ID阶段
		UE_INPUT_NOTICE,			//之前输入完成 向导栏此前应该显示提示文言
		UE_INPUT_SELECTINSTALL,		//选择安装的业务
		UE_INPUT_SELECTUNINSTALL	//选择卸载的业务
	}UE_INPUTSTATUS;				//当前输入状态

public:
	void resetDefaultColor();		//恢复设置基础数据区的显示颜色(黑色)
	void refreshBaseArea();			//刷新基础数据区,用于更新显示的内容
	void iniBaseStatusInfo(UE_CURSTATUS ueCurStatus);	//根据不同阶段初始化当前阶段的显示
	void setlabelgroup(const LABEL_GROUP* plabelgroup,const UINT uigroupnum);	//设置显示内容
	void setCurStatus(UE_CURSTATUS ueCurStatus);	//设置当前阶段 开始?安装/卸载?完成?
	void setinputstatus(UE_INPUTSTATUS ueinputstatus);	//设置向导栏当前状态,输入什么内容?显示提示文言

	void enterInstallNotice();		//进入安装界面
	void installNotice();
	void enterStartNotice();		//进入开始的选择界面
	void startNotice();
	void enterInstallFinishNotice();
	void enterUnInstallFinishNotice();
	void finishNotice(bool bInstall=true);	
	void enterUnInstallNotice();	//进入卸载界面
	void uninstallNotice();
	UE_CURSTATUS getCurStatus();	//获取当前阶段 应该是哪个界面

	void enterInputNormal();
	void enterInputSelectInstall();
	void enterInputID();			//进入输入票箱ID的状态
	void enterInputQuantity();		//进入输入票数量的状态
	void enterInputWastedID();		//进入输入废票箱ID的状态
	void enterInputNotice();		//此时没有等待输入,而是显示向导文言的状态
	void enterInputSelectUninstall();
	void inputIDGuide();			//显示输入票箱ID的提示文言
	void inputQuantityGuide();		//显示输入票张数量的提示文言
	void inputWastedIDGuide();		//显示输入废票箱ID的提示文言
	void inputNormalGuide();		//没有输入的提示文言,显示默认的提示文言
	void finishInputID(CString szID);
	void finishInputQuantity(CString szQuantity);
	void finishInputWasted(CString szWastedID);

	void finishSelectUninstallBoxA();
	void finishUnSelectUninstallBoxA();
	void finishSelectUninstallWasted();
	void finishUnSelectUninstallWasted();
	UE_INPUTSTATUS getInputStatus();

	void updateGuide(CString szNotice);
	void updateGuide(UE_NOTICEID ueNoticeID);
	void updateNotice(int irow,int icol,CString szData,COLORREF clr=RGB(0,0,0));	//更新指定行列内容
	void updateNotice(int irow,int icol,COLORREF clr=RGB(0,0,0));	//更新指定行的现实颜色

	CString getCardID();			//取master文件中的票箱ID
	CString getCardQuantity();		//取master文件中的票张数量
	CString getWastedID();			//取master文件中的废票箱ID
	CString getWastedQuantity();	//取master文件中的废票数量

	CString getCardIDBefore();
	CString getCardQuantityBefore();
	CString getWastedIDBefore();

	BOOL	setCardID(CString szID);	//票箱ID写入master文件
	BOOL	setCardQuantity(CString szQuantity);	//票张数量写入master文件
	BOOL	setWastedID(CString szWastedID);		//废票箱ID写入master文件

	BOOL detectBoxA();
	BOOL detectBoxB();
	BOOL detectWastedBox();

private:
	/////////////////////////////////////////////////////////////////////////////////////
	//implement base class virtual function
	virtual void UpdateUI();
	/////////////////////////////////////////////////////////////////////////////////////
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	
public:
	CString m_szLastCardBoxID;		//记录安装或者卸载之前保存的票箱ID
	CString m_szLastCardQuantity;	//记录安装或者卸载之前保存的票张数量
	CString m_szLasetWastedID;		//记录安装或者卸载之前保存的废票箱ID
	CString m_szLastWastedQuantity;	//记录安装或者卸载之前保存的废票数量

private:
	LABEL_GROUP m_labelgroup[UD_MAX_ROW];
	UE_CURSTATUS m_uestatus;
	UE_INPUTSTATUS m_ueinputstatus;
	void UpdateUIforInstall();			// 更新安装界面
	void UpdateUIForRemoval();			// 更新卸载界面
	void UpdateUIforStart();			// 更新选择业务界面
	void ClearUI();						// 清除界面所有数据

	LABEL_GROUP_FOUR lablelGroups[UD_MAX_ROW];
	//CTicketBoxExchangeSvc *m_pSvc;
};