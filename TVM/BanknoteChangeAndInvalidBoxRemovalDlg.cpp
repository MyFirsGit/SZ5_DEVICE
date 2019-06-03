#include "StdAfx.h"
#include "BanknoteChangeAndInvalidBoxRemovalDlg.h"


IMPLEMENT_DYNAMIC(CBanknoteChangeAndInvalidBoxRemovalDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CBanknoteChangeAndInvalidBoxRemovalDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


CBanknoteChangeAndInvalidBoxRemovalDlg::CBanknoteChangeAndInvalidBoxRemovalDlg(CService* pService) 
	:COperationDlg(CBanknoteChangeAndInvalidBoxRemovalDlg::IDD, pService)
{
	m_pSvc = (CBanknoteChangeAndInvalidBoxRemovalSvc*)pService;
	m_baseInfoArea = NULL;
}


CBanknoteChangeAndInvalidBoxRemovalDlg::~CBanknoteChangeAndInvalidBoxRemovalDlg(void)
{
	if (m_baseInfoArea != NULL)	{
		delete m_baseInfoArea;
	}
}


int CBanknoteChangeAndInvalidBoxRemovalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}
	// ����GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);
	return 0; 
}

void CBanknoteChangeAndInvalidBoxRemovalDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	// ����������
	theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_ENTER | KEY_F2 | KEY_INPUT);
	// �����������볤��
	theAPP_SESSION.SetMaxInputLength(1);
	// ���ò�ͬ�������ʾ����
}

// ����ˢ��
void CBanknoteChangeAndInvalidBoxRemovalDlg::UpdateUI()
{
	for (int i = 0; i < 5; i++){
		m_baseInfoArea->SetLableAlign(i, DT_LEFT | DT_VCENTER); // ����ÿһ�ж��뷽ʽ
	}

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, RESOLUTION * 120);		// ֽ��������
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, RESOLUTION * 100);		// ֽ������
	m_baseInfoArea->SetLableWidth(LABEL_COL_3, RESOLUTION * 150);		// ��5 Ԫ
	m_baseInfoArea->SetLableWidth(LABEL_COL_4, RESOLUTION * 150);		// ������999
	m_baseInfoArea->SetLableWidth(LABEL_COL_5, RESOLUTION * 100);		// ״̬

	// �������ļ���ȡ�� ֽ���������е�ֽ�����
	int nChangeNoteFace = 5;
	int nFirstLine		= 0;
	// ȡSVC�е�DATAMODEL
	CBanknoteChangeAndInvalidBoxRemovalSvc::Model Model = m_pSvc->GetDataModel();

	// ������A
	m_lableGroup[LABEL_ROW_1].name1 = _opl(DEVICESTATUS_SUPPLEMENT_BANKNOTE_SET);
	m_lableGroup[nFirstLine].name2 = Model.changeBoxAData.strID;							// ID
	m_lableGroup[nFirstLine].name3.Format(_opl(TXT_COINMGR_TYPE) + _T("��%d ") + _opl(MONEY_UNIT),nChangeNoteFace);					// ���. �ɲ��趨��ֻ�ڳ�ʼ��ʱ���趨
	m_lableGroup[nFirstLine].name4.Format(_opl(TXT_QUANTITY) + _T("�� %d"),Model.changeBoxAData.nCount );	// ����
	m_lableGroup[nFirstLine].name5 = m_pSvc->TranslateState(Model.changeBoxAData.boxState);					// ״̬
	m_baseInfoArea->SetLabelColor(nFirstLine, 4, Model.changeBoxAData.GetColor());			// ״̬��ɫ


	// ������B
	/*m_lableGroup[++nFirstLine].name1 = _T("ֽ��������B");
	m_lableGroup[nFirstLine].name2 = Model.changeBoxBData.strID;	
	m_lableGroup[nFirstLine].name3.Format(_T("��ֵ�� %d Ԫ"),nChangeNoteFace);
	m_lableGroup[nFirstLine].name4.Format(_T("������ %d"),Model.changeBoxBData.nCount ); 
	m_lableGroup[nFirstLine].name5 = m_pSvc->TranslateState(Model.changeBoxBData.boxState);
	m_baseInfoArea->SetLabelColor(nFirstLine, 4, Model.changeBoxBData.GetColor());

	// �ϱ���
	m_lableGroup[++nFirstLine].name1 = _T("�ϱһ�����"); 
	m_lableGroup[nFirstLine].name2 = Model.invalidBoxData.strID;
	m_lableGroup[nFirstLine].name3 = _T("");			
	m_lableGroup[nFirstLine].name4.Format(_T("������ %d"),Model.invalidBoxData.nCount ); 
	m_lableGroup[nFirstLine].name5 = m_pSvc->TranslateState(Model.invalidBoxData.boxState);
	m_baseInfoArea->SetLabelColor(nFirstLine, 4, Model.invalidBoxData.GetColor());*/

	m_baseInfoArea->SetLableWidth(LABEL_ROW_6,0,RESOLUTION * 350);
	m_baseInfoArea->SetLableWidth(LABEL_ROW_7,0,RESOLUTION * 350);
	m_baseInfoArea->SetLableWidth(LABEL_ROW_8,0,RESOLUTION * 350);
	m_baseInfoArea->SetLableWidth(LABEL_ROW_9,0,RESOLUTION * 350);

	m_lableGroup[LABEL_ROW_6].name1.Format(_T("[1] ") + _opl(TXT_ID_TVM_BANKNOTE_CHANGE_INVALID_BOX_UNINSTALL));
	/*m_lableGroup[LABEL_ROW_7].name1 = _T("[2] ж��ֽ��������B");
	m_lableGroup[LABEL_ROW_8].name1 = _T("[3] ж�طϱһ�����");
	m_lableGroup[LABEL_ROW_9].name1 = _T("[4] ж������������ͻ�����");*/
}