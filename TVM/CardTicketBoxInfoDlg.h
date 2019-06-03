#pragma once
#include "OperationDlg.h"
#include "CardBoxManageSvc.h"

#define UD_MAX_ROW	15				//���������ʾ�������ʾ��
#define UD_MAX_COL	2				//���������������ʾ������

class CTicketBoxInfoDlg: public COperationDlg
{
	DECLARE_DYNAMIC(CTicketBoxInfoDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTicketBoxInfoDlg(CService* pService);          // ��׼���캯��
	~CTicketBoxInfoDlg();

	enum { IDD = IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG };          // �Ի�������

	typedef enum _UE_CURSTATUS
	{
		UE_START=1,			//ѡ��װ����ж�ؽ���
		UE_INSTALL,			//��װ�׶�
		UE_UNINSTALL,		//ж�ؽ׶�
		UE_INSTALLFINISH,	//��װ��ɽ���
		UE_UNINSTALLFINISH	//ж����ɽ���
	}UE_CURSTATUS;			//��ǰά���׶�

	typedef enum _UE_NOTICEID
	{

		UE_ATTACH=TXT_SMARTCARDMGR_ATTACH,
		UE_DETACH=TXT_SMARTCARDMGR_DETACH,				//ж��Ʊ��������ʾ

		UE_CARD_ID=TXT_SMARTCARDMGR_BOX_ID,				//ȡƱ��ID��ʾ����
		UE_CARD_QUANTITY=TXT_SMARTCARDMGR_CARD_QUANTITY,		//ȡƱ��������ʾ����
		UE_WASTED_CARD_ID=TXT_SMARTCARDMGR_WASTED_ID,		//ȡ��Ʊ��ID��ʾ����

		UE_BEFORE_ATTACH=TXT_SMARTCARDMGR_BEFORE_ATTACH,		//��װǰ��ʾ����
		UE_AFTER_ATTACH=TXT_SMARTCARDMGR_AFTER_ATTACH,		//��װ����ʾ����

		UE_BEFORE_DETACH=TXT_SMARTCARDMGR_BEFORE_DETACH,
		UE_AFTER_DETACH=TXT_SMARTCARDMGR_AFTER_DETACH,

		UE_QUANTITY=TXT_SMARTCARDMGR_QUANTITY,			//������ʾ����

		UE_DETACH_CARD=TXT_SMARTCARDMGR_DETACH_BOX,			//ж��Ʊ����ʾ����
		UE_DETACH_WASTED_CARD=TXT_SMARTCARDMGR_DETACH_WASTEDBOX,	//ж�ط�Ʊ����ʾ����
		UE_DETACH_ALL=TXT_SMARTCARDMGR_DETACH_ALL,			//ж������Ʊ����ʾ����
		UE_ATTACHED=TXT_SMARTCARDMGR_ATTACHED,			//�Ѿ���װ��ʾ����
		UE_DETACHED=TXT_SMARTCARDMGR_DETACHED,			//�Ѿ�ж����ʾ����

		UE_ERROR_CARDBOX_ID=TXT_SMARTCARDMGR_INVALID_BOXID,	//�����Ʊ��ID
		UE_ERROR_WASTEDBOX_ID=TXT_SMARTCARDMGR_INVALID_WASTEDID,	//����ķ�Ʊ��ID
		UE_ERROR_CARDBOX_QUANTITY=TXT_SMARTCARDMGR_INVALID_CARD_QUANTITY,//�����Ʊ������
		UE_ERROR_WASTEDBOX_QUANTITY=TXT_SMARTCARDMGR_INVALID_WASTED_QUANTITY,//����ķ�Ʊ����

		UE_NOTICE_WAITTOATTACH=TXT_SMARTCARDMGR_WAITTOATTACH,					//����װ
		UE_NOTICE_WAITTODETACH=TXT_SMARTCARDMGR_WAITTODETACH,					//��ж��
		UE_NOTICE_ATTACHBOXFAILED=TXT_SMARTCARDMGR_BOXATTACHERROR,				//��װƱ�����
		UE_NOTICE_ATTACHWASTEDFAILED=TXT_SMARTCARDMGR_WASTEDBOXATTACHERROR,		//��װ��Ʊ�����
		UE_NOTICE_NOTFOUNDBOX=TXT_SMARTCARDMGE_NOTFOUNDBOX,						//û�а�װƱ��
		UE_NOTICE_NOTFOUNDWASTED=TXT_SMARTCARDMGR_NOTFOUNDWASTED,				//û�а�װ��Ʊ��
		UE_NOTICE_NOTFOUNDANYBOX=TXT_SMARTCARDMGR_NOTFOUNANYBOX					//û�а�װ�κ�Ʊ��
	}UE_NOTICEID;				//�������Զ���

	typedef enum _UE_INPUTSTATUS
	{
		UE_INPUT_CARD_ID=1,			//����Ʊ��ID�׶�
		UE_INPUT_CARD_QUANTITY,		//����Ʊ�����׶�
		UE_INPUT_WASTEDCARD_ID,		//�����Ʊ��ID�׶�
		UE_INPUT_NOTICE,			//֮ǰ������� ������ǰӦ����ʾ��ʾ����
		UE_INPUT_SELECTINSTALL,		//ѡ��װ��ҵ��
		UE_INPUT_SELECTUNINSTALL	//ѡ��ж�ص�ҵ��
	}UE_INPUTSTATUS;				//��ǰ����״̬

public:
	void resetDefaultColor();		//�ָ����û�������������ʾ��ɫ(��ɫ)
	void refreshBaseArea();			//ˢ�»���������,���ڸ�����ʾ������
	void iniBaseStatusInfo(UE_CURSTATUS ueCurStatus);	//���ݲ�ͬ�׶γ�ʼ����ǰ�׶ε���ʾ
	void setlabelgroup(const LABEL_GROUP* plabelgroup,const UINT uigroupnum);	//������ʾ����
	void setCurStatus(UE_CURSTATUS ueCurStatus);	//���õ�ǰ�׶� ��ʼ?��װ/ж��?���?
	void setinputstatus(UE_INPUTSTATUS ueinputstatus);	//����������ǰ״̬,����ʲô����?��ʾ��ʾ����

	void enterInstallNotice();		//���밲װ����
	void installNotice();
	void enterStartNotice();		//���뿪ʼ��ѡ�����
	void startNotice();
	void enterInstallFinishNotice();
	void enterUnInstallFinishNotice();
	void finishNotice(bool bInstall=true);	
	void enterUnInstallNotice();	//����ж�ؽ���
	void uninstallNotice();
	UE_CURSTATUS getCurStatus();	//��ȡ��ǰ�׶� Ӧ�����ĸ�����

	void enterInputNormal();
	void enterInputSelectInstall();
	void enterInputID();			//��������Ʊ��ID��״̬
	void enterInputQuantity();		//��������Ʊ������״̬
	void enterInputWastedID();		//���������Ʊ��ID��״̬
	void enterInputNotice();		//��ʱû�еȴ�����,������ʾ�����Ե�״̬
	void enterInputSelectUninstall();
	void inputIDGuide();			//��ʾ����Ʊ��ID����ʾ����
	void inputQuantityGuide();		//��ʾ����Ʊ����������ʾ����
	void inputWastedIDGuide();		//��ʾ�����Ʊ��ID����ʾ����
	void inputNormalGuide();		//û���������ʾ����,��ʾĬ�ϵ���ʾ����
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
	void updateNotice(int irow,int icol,CString szData,COLORREF clr=RGB(0,0,0));	//����ָ����������
	void updateNotice(int irow,int icol,COLORREF clr=RGB(0,0,0));	//����ָ���е���ʵ��ɫ

	CString getCardID();			//ȡmaster�ļ��е�Ʊ��ID
	CString getCardQuantity();		//ȡmaster�ļ��е�Ʊ������
	CString getWastedID();			//ȡmaster�ļ��еķ�Ʊ��ID
	CString getWastedQuantity();	//ȡmaster�ļ��еķ�Ʊ����

	CString getCardIDBefore();
	CString getCardQuantityBefore();
	CString getWastedIDBefore();

	BOOL	setCardID(CString szID);	//Ʊ��IDд��master�ļ�
	BOOL	setCardQuantity(CString szQuantity);	//Ʊ������д��master�ļ�
	BOOL	setWastedID(CString szWastedID);		//��Ʊ��IDд��master�ļ�

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
	CString m_szLastCardBoxID;		//��¼��װ����ж��֮ǰ�����Ʊ��ID
	CString m_szLastCardQuantity;	//��¼��װ����ж��֮ǰ�����Ʊ������
	CString m_szLasetWastedID;		//��¼��װ����ж��֮ǰ����ķ�Ʊ��ID
	CString m_szLastWastedQuantity;	//��¼��װ����ж��֮ǰ����ķ�Ʊ����

private:
	LABEL_GROUP m_labelgroup[UD_MAX_ROW];
	UE_CURSTATUS m_uestatus;
	UE_INPUTSTATUS m_ueinputstatus;
	void UpdateUIforInstall();			// ���°�װ����
	void UpdateUIForRemoval();			// ����ж�ؽ���
	void UpdateUIforStart();			// ����ѡ��ҵ�����
	void ClearUI();						// ���������������

	LABEL_GROUP_FOUR lablelGroups[UD_MAX_ROW];
	//CTicketBoxExchangeSvc *m_pSvc;
};