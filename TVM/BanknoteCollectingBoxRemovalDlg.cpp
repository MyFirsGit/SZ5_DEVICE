#include "StdAfx.h"
#include "BanknoteCollectingBoxRemovalDlg.h"

static const int FULL_WITDH = 600;
static const int NAME_WITDH = 400;
static const int ID_WITDH = 150;
static const int INDENT_WIDTH = 50;
static const int GIRD_WITDH = 160;
static const int STATUE_WIDTH = 100;
static const int FACE_WIDTH = 80;
static const int FACE_WIDTH_1 = 120;
static const int COUNT_WIDTH = 50;
static const int MONEY_WIDTH = 130;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteCollectingBoxRemovalDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CBanknoteCollectingBoxRemovalDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxRemovalDlg::CBanknoteCollectingBoxRemovalDlg(CService* pService)
	: COperationDlg(CBanknoteCollectingBoxRemovalDlg::IDD,pService)
{
	pSvc = (CBanknoteCollectingBoxRemovalSvc*)pService;
}

////////////////////////////// ////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxRemovalDlg::~CBanknoteCollectingBoxRemovalDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CBanknoteCollectingBoxRemovalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}

	// ����GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	CBanknoteCollectingBoxRemovalSvc::Model& model = pSvc->GetDataModel();

	// �����������
	Clear();

	theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_ENTER|KEY_INPUT|KEY_F2);
	theAPP_SESSION.SetMaxInputLength(1);

	// �������棨��װ��ж�أ�
	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_OPERATION ){	// ��ʼж��
		InitDetachBoxUI();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_FINISH){		// ж�����
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalDlg::UpdateUI()
{
	CBanknoteCollectingBoxRemovalSvc::Model& model = pSvc->GetDataModel();

	// �����������
	Clear();
	InitDetachBoxUI();
	ShowDetachBoxData();
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		ж��ֽ���仭�沼�ֳ�ʼ��   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalDlg::InitDetachBoxUI()
{
	// ������Ϣ��ʽ
	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);

	

	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_OPERATION)
	{
		m_baseInfoArea->SetLableWidth(LABEL_ROW_1,LABEL_COL_1,RESOLUTION *  NAME_WITDH);					// ֽ����
		m_baseInfoArea->SetLableWidth(LABEL_ROW_1,LABEL_COL_2,RESOLUTION * ID_WITDH);					// ���
		m_baseInfoArea->SetLableWidth(LABEL_ROW_1,LABEL_COL_3,RESOLUTION *  STATUE_WIDTH);				// �Ƿ�װ

		for ( int i = LABEL_ROW_2; i < LABEL_ROW_7; i++)
		{
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_1,RESOLUTION *  FACE_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_2,RESOLUTION *  COUNT_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_3, RESOLUTION * MONEY_WIDTH);	// �ϼ�
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_4, RESOLUTION * FACE_WIDTH_1);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_5, RESOLUTION * COUNT_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_6, RESOLUTION * MONEY_WIDTH);	// �ϼ�

			m_baseInfoArea->SetLableAlign(i,LABEL_COL_1, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_2, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_3, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_4, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_5, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_6, DT_RIGHT | DT_VCENTER);

		}

		// �˵���
		m_baseInfoArea->SetLableWidth(LABEL_ROW_7,LABEL_COL_1,RESOLUTION *  NAME_WITDH);					// ֽ����

		//m_baseInfoArea->SetLableWidth(7,0,RESOLUTION * 200);
	}else{

		m_baseInfoArea->SetLableWidth(LABEL_ROW_1,LABEL_COL_1, RESOLUTION * NAME_WITDH);					// ֽ����
		m_baseInfoArea->SetLableWidth(LABEL_ROW_2,LABEL_COL_1, RESOLUTION * ID_WITDH);					// ���
		m_baseInfoArea->SetLableWidth(LABEL_ROW_2,LABEL_COL_2, RESOLUTION * STATUE_WIDTH);				// �Ƿ�װ

		for ( int i = LABEL_ROW_3; i <= LABEL_ROW_9; i++)
		{
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_1, RESOLUTION * FACE_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_2, RESOLUTION * COUNT_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_3, RESOLUTION * MONEY_WIDTH);	// �ϼ�
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_4, RESOLUTION * FACE_WIDTH_1);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_5, RESOLUTION * COUNT_WIDTH);	// ����
			m_baseInfoArea->SetLableWidth(i, LABEL_COL_6, RESOLUTION * MONEY_WIDTH);	// �ϼ�

			m_baseInfoArea->SetLableAlign(i,LABEL_COL_1, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_2, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_3, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_4, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_5, DT_RIGHT | DT_VCENTER);
			m_baseInfoArea->SetLableAlign(i,LABEL_COL_6, DT_RIGHT | DT_VCENTER);
		}

		
		//m_baseInfoArea->SetLableWidth(LABEL_ROW_8, LABEL_COL_1, RESOLUTION * ID_WITDH);	
		m_baseInfoArea->SetLableWidth(LABEL_ROW_9, LABEL_COL_1, RESOLUTION * ID_WITDH);	
		m_baseInfoArea->SetLableWidth(LABEL_ROW_9, LABEL_COL_2, RESOLUTION * ID_WITDH);	
		// ж�غ��id
		m_baseInfoArea->SetLableWidth(9,0,RESOLUTION * 300);
	}
	

}

//////////////////////////////////////////////////////////////////////////
/*
@brief		�������   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalDlg::Clear()
{
	for (int i = 0; i < 15; ++i)
	{
		m_lableGroup[i].name1 = _T(""); 
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
	// Ӧ��ܸ��ģ������������ģʽ�����ֽ���书�ܵ���ʾ����Ϊ�״�����ģʽ��bug
	CBanknoteCollectingBoxRemovalSvc::Model& model = pSvc->GetDataModel();
	switch( pSvc->GetCurDialogGroupID())
	{
	case CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_OPERATION:
		break; 
	case CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_FINISH:
	case CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_ERROR:
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_ESC);
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);break;
	}
}



//////////////////////////////////////////////////////////////////////////
/*
@brief		��ʾж�ػ���������   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalDlg::ShowDetachBoxData()
{
	CBanknoteCollectingBoxRemovalSvc::Model& model = pSvc->GetDataModel();
	// ֽ����ж�ز�������
	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_OPERATION)
	{
	
		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_PLEASE_BOX_ID) +  model.box_data.now_id;

		m_lableGroup[LABEL_ROW_1].name2.Format(_T("%s"), model.strState);
		m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, model.clrState);

		m_lableGroup[LABEL_ROW_2].name1 = _T("");
		m_lableGroup[LABEL_ROW_2].name2 = _opl(TXT_BANKNOTEMGT_QTY);
		m_lableGroup[LABEL_ROW_2].name3 = _opl(TXT_BANKNOTEMGY_AMOUNT);
		m_lableGroup[LABEL_ROW_2].name4 = _T("");
		m_lableGroup[LABEL_ROW_2].name5 = _opl(TXT_BANKNOTEMGT_QTY);
		m_lableGroup[LABEL_ROW_2].name6 = _opl(TXT_BANKNOTEMGY_AMOUNT);

		// ����  ����(�޵�λ)  ���
		
		m_lableGroup[LABEL_ROW_3].name1 = _T("1 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_3].name2.Format(_T(" %d ") , model.box_data.face1);
		m_lableGroup[LABEL_ROW_3].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face1*100));
		m_lableGroup[LABEL_ROW_3].name4 = _T("5 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_3].name5.Format(_T(" %d ") , model.box_data.face5);
		m_lableGroup[LABEL_ROW_3].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face5*500));
		
		m_lableGroup[LABEL_ROW_4].name1 = _T("10 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_4].name2.Format(_T(" %d ") , model.box_data.face10);
		m_lableGroup[LABEL_ROW_4].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face10*1000));
		m_lableGroup[LABEL_ROW_4].name4 = _T("20 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_4].name5.Format(_T(" %d ") , model.box_data.face20);
		m_lableGroup[LABEL_ROW_4].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face20*2000));
		
		m_lableGroup[LABEL_ROW_5].name1 = _T("50 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_5].name2.Format(_T(" %d ") , model.box_data.face50);
		m_lableGroup[LABEL_ROW_5].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face50*2000));
		m_lableGroup[LABEL_ROW_5].name4 = _T("100 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_5].name5.Format(_T(" %d ") , model.box_data.face100);
		m_lableGroup[LABEL_ROW_5].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face100*10000));
		//m_lableGroup[LABEL_ROW_5].name4 = _T("100 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);

		m_lableGroup[LABEL_ROW_6].name1 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_6].name3.Format( _T("%s ") + _opl(MONEY_UNIT), 
			ComFormatAmount(100*(model.box_data.face10*10
			+ model.box_data.face20*20
			+ model.box_data.face50*50
			+ model.box_data.face100*100
			+ model.box_data.face5*5
			+ model.box_data.face1*1)));

		m_lableGroup[LABEL_ROW_7].name1.Format(_T("[%d] ")+_opl(TXT_BANKNOTEMGR_MENU_DETACH), MENU_CODE_1);
	}
	// ж�����
	else if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxRemovalSvc::DIALOG_GROUP_FINISH) {
		// ж��֮ǰ:
		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_BOX_BEFORE_DETACH);
		// ֽ����ID
		m_lableGroup[LABEL_ROW_2].name1 = _opl(TXT_BANKNOTEMGR_BOX_ID) ;
		m_lableGroup[LABEL_ROW_2].name3 = + model.box_data.raw_id;
		// QTY  Amount ��ͷ����
		m_lableGroup[LABEL_ROW_3].name1 = _T("");
		m_lableGroup[LABEL_ROW_3].name2 = _opl(TXT_BANKNOTEMGT_QTY);
		m_lableGroup[LABEL_ROW_3].name3 = _opl(TXT_BANKNOTEMGY_AMOUNT);
		m_lableGroup[LABEL_ROW_3].name4 = _T("");
		m_lableGroup[LABEL_ROW_3].name5 = _opl(TXT_BANKNOTEMGT_QTY);
		m_lableGroup[LABEL_ROW_3].name6 = _opl(TXT_BANKNOTEMGY_AMOUNT);
		// ���� ���� ���
		m_lableGroup[LABEL_ROW_4].name1 = _T("1 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_4].name2.Format(_T(" %d ") , model.box_data.face1);
		m_lableGroup[LABEL_ROW_4].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face1*100));
		m_lableGroup[LABEL_ROW_4].name4 = _T("5 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_4].name5.Format(_T(" %d ") , model.box_data.face5);
		m_lableGroup[LABEL_ROW_4].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face5*500));
		
		m_lableGroup[LABEL_ROW_5].name1 = _T("10 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_5].name2.Format(_T(" %d ") , model.box_data.face10);
		m_lableGroup[LABEL_ROW_5].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face10*1000));
		m_lableGroup[LABEL_ROW_5].name4 = _T("20 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_5].name5.Format(_T(" %d ") , model.box_data.face20);
		m_lableGroup[LABEL_ROW_5].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face20*2000));
		
		m_lableGroup[LABEL_ROW_6].name1 = _T("50 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_6].name2.Format(_T(" %d ") , model.box_data.face50);
		m_lableGroup[LABEL_ROW_6].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face50*5000));
		m_lableGroup[LABEL_ROW_6].name4 = _T("100 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_6].name5.Format(_T(" %d ") , model.box_data.face100);
		m_lableGroup[LABEL_ROW_6].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.box_data.face100*10000));

		m_lableGroup[LABEL_ROW_7].name1 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT) + _opl(TXT_COLON);
		m_lableGroup[LABEL_ROW_7].name3.Format(_T("%s ") + _opl(MONEY_UNIT), 
			ComFormatAmount(100*(model.box_data.face10*10
			+ model.box_data.face20*20
			+ model.box_data.face50*50
			+ model.box_data.face100*100
			+ model.box_data.face5*5
			+ model.box_data.face1*1)));
		// ж��֮��
		m_lableGroup[LABEL_ROW_8].name1 = _opl(TXT_BANKNOTEMGR_BOX_AFTER_DETACH);
		m_lableGroup[LABEL_ROW_9].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)/*+_T(" %s"), _T("FFFFFFFF")*/);
		m_lableGroup[LABEL_ROW_9].name2 = _T("FFFFFFFF");
	}
	else{
		Clear();
	}
}
