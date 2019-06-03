#include "stdafx.h"
#include "CoinAddingBoxSetSvc.h"
#include "CoinAddingBoxSetMaintainDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCoinAddingBoxSetSvc,CTVMForeService)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_CLEAR,OnKeyboardClear)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxSetSvc::CCoinAddingBoxSetSvc()
	:CTVMForeService(MODE_SWITCH_SVC)//��ӦΪ������������ã�������ģʽ�л����棬���Բ˵���ѡģʽ�л�
{
	EnterTime = 1;
	GetGuide = _T("");

	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CCoinAddingBoxMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COINADDINGBOX_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COINADDINGBOX_SET,IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COINADDINGBOX_SET);//���ó�ʼ������
	ReadTvmIni();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxSetSvc::~CCoinAddingBoxSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     currency

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxSetSvc::Model& CCoinAddingBoxSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTVM.INI�ļ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::ReadTvmIni()
{
	m_DataModel.hopone = theTVM_INFO.GetCHHopper1Type();
	m_DataModel.hoponecurrency = ReadCurrency(m_DataModel.hopone);	//������A����
	m_DataModel.hoptwo = theTVM_INFO.GetCHHopper2Type();
	m_DataModel.hoptwocurrency = ReadCurrency(m_DataModel.hoptwo);	//������B����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʶ�����

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinAddingBoxSetSvc::ReadCurrency(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)		//��TVM.INI�ļ��ж�ȡ�ļ�ֵ
	{
	case MENU_CODE_0:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//�����
		break;
	case MENU_CODE_1:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//50AVOS
		break;
	case MENU_CODE_2:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_OMOP);		//1MOP
		break;
	case MENU_CODE_3:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_SMOP);		//2MOP
		break;
	case MENU_CODE_4:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FMOP);		//5MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [0~9] ��������Ϣ

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CCoinAddingBoxSetSvc::OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/)
//{
//	if(1 == EnterTime)		//��һ�����ּ�����ѡ����Ӧ�Ĳ˵�
//	{
//		m_DataModel.CodeChange = FALSE;
//		theAPP_SESSION.AppendOneNumToGuide(wParam);
//		return TRUE;
//	}
//	if(2 == EnterTime)		//�ڶ�����ʾ��׷����Ӧ�����
//	{
//		theAPP_SESSION.SetPrefixToGuide(GetGuide);
//		theAPP_SESSION.AppendOneNumToGuide(wParam);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinAddingBoxSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(1 == EnterTime)			//��һ�ΰ���Enter��ȷ�ϲ˵���ѡ��
	{
		CheckMenuNumber();
	}
	else if(2 == EnterTime)		//�ڶ��ΰ���Enter��ȷ�ϴ������õ�ѡ��
	{
		DoCheckInput();			//�����������Ƿ����������Ӧ���ʵ���ж�
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �˵����Ƿ�Ƿ��ж�

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::CheckMenuNumber()
{
	// ��֤����Ĳ˵����
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > MAX_COINADDMENU_NUM)//���˵���ų�1-2֮���Ϊ�Ƿ��˵���
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		InputGetGuide();		//��GetGuide�ֱ�ֵ
		EnterTime = 2;
		m_DataModel.MenuChange = TRUE;		//�˵��ŷ����仯��־
		NotifyDataModelChanged();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��GetGuide�ֱ�ֵ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::InputGetGuide()
{
	switch(m_DataModel.MenuNumber)//���ݲ˵���ѡ����Ӧ����ʾ
	{
	case MENU_CODE_1:		//������A
		GetGuide = _opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) ;
		break;
	case MENU_CODE_2:		//������B
		GetGuide = _opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �����������Ƿ����������Ӧ���ʵ���ж�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::DoCheckInput()
{
	int index = theAPP_SESSION.GetInputToInt();//��ȡ������������
	CString hopetype = theAPP_SESSION.GetInputToString();
	if(_T("") == hopetype)
	{
		switch(m_DataModel.MenuNumber)
		{
		case MENU_CODE_1:	//������A
			index = theTVM_INFO.GetCHHopper1Type();
			break;
		case MENU_CODE_2:	//������B
			index = theTVM_INFO.GetCHHopper2Type();
			break;
		default:
			break;
		}
	}
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:		//������A
		if(index<0 || index>4)
		{
			m_DataModel.CodeContent = FALSE;
		}
		else
		{
			m_DataModel.hoponecurrency = ReadCurrency(index);
			CurrIndexNumber.numberone = CurrGet(index);
			m_DataModel.CodeContent = TRUE;
		}
		break;
	case MENU_CODE_2:		//������B
		if(index<0 || index>4)
		{
			m_DataModel.CodeContent = FALSE;
		}
		else
		{
			m_DataModel.hoptwocurrency = ReadCurrency(index);
			CurrIndexNumber.numbertwo = CurrGet(index);
			m_DataModel.CodeContent = TRUE;
		}
		break;
	default:
		break;
	}
	m_DataModel.CodeChange = TRUE;//���뷢���仯��־
	m_DataModel.MenuChange = FALSE;//�ò˵��ű仯��־λFALSE��ֹ�˵��ŷ�������
	NotifyDataModelChanged();

	EnterTime = 1;//�ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �Բ˵���ת���󱣴�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinAddingBoxSetSvc::CurrGet(int currmenunumber)
{
	CString Currennubmer = _T("");
	int index = 0;
	switch(currmenunumber)
	{
	case MENU_CODE_1:		//50AVOS
		index = 4;
		break;
	case MENU_CODE_2:		//1MOP
		index = 1;
		break;
	case MENU_CODE_3:		//2MOP
		index = 2;
		break;
	case MENU_CODE_4:		//5MOP
		index = 3;
	default:
		break;
	}
	Currennubmer.Format(_T("%d"),index);
	return Currennubmer;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Clear] ��������Ϣ��ɾ����

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinAddingBoxSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CString empty = theAPP_SESSION.GetInputToString();
	if (_T("") == empty)			//��������Ϊ��
	{
		EnterTime = 1;				//�ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
	return TRUE;
}
