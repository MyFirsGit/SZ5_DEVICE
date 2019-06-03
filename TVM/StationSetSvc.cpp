#include "stdafx.h"
#include "StationSetSvc.h"
#include "StationSetMaintainDlg.h"
#include "GuideInfo.h"
#include "AudioHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStationSetSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::CStationSetSvc()
	:CTVMForeService(STATION_SET_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CStationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_STATION_SET, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_STATION_SET, IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STATION_SET);// ���ó�ʼ������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::~CStationSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������

	EnterTime = 1;
	GuideMsg = _T("");
	
	ReadIniText();

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STATION_SET);	// ���ó�ʼ������
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_STATION_SET);	
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::Model& CStationSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStationSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardEnter(0,0);
		if(1 == EnterTime)			// ��һ�ΰ���Enter��ȷ�ϲ˵���ѡ��
		{
			CheckMenuNumber();		// �˵����Ƿ�Ƿ��ж�
		}
		else if(2 == EnterTime )	// �ڶ��ΰ���Enter��ȷ�ϴ������õ�ѡ��
		{
			DoCheckInput();			// �����������Ƿ����������Ӧ���ʵ���ж�
			theAPP_SESSION.SetMaxInputLength(1);
		}
		return TRUE;
	}
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
void CStationSetSvc::CheckMenuNumber()
{
	// ��֤����Ĳ˵����
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > MAX_SMMENU_NUM)		// ���˵���ų�1-5֮���Ϊ�Ƿ��˵���
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		InputGuideMsg();
		theAPP_SESSION.DeleteAllInput();
		theAPP_SESSION.SetPrefixToGuide(GuideMsg);
		EnterTime = 2;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡMaintenance.INI�����ļ�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::ReadIniText()
{
	// ��ά����Ϣ��ȡ�ó�վ���룬�������룬��ţ����ڱ��
	m_DataModel.sLineCode.Format(_T("%d"),(theMAINTENANCE_INFO.GetStationCodeWithLine()/100));
	m_DataModel.sStationCode.Format(_T("%d"),(theMAINTENANCE_INFO.GetStationCodeWithLine()%100));
	m_DataModel.sMachineCode.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode());
	m_DataModel.sGroupNumber.Format(_T("%d"),theMAINTENANCE_INFO.GetGroupNumber());
	m_DataModel.sEquNumberInGroup.Format(_T("%d"),theMAINTENANCE_INFO.GetEquNumberInGroup());

	n_NubIndex.LineIndex = theMAINTENANCE_INFO.GetStationCodeWithLine()/100;
	n_NubIndex.StationIndex = theMAINTENANCE_INFO.GetStationCodeWithLine()%100;
	n_NubIndex.MachineIndex = theMAINTENANCE_INFO.GetMachineCode();
	n_NubIndex.GroupIndex = theMAINTENANCE_INFO.GetGroupNumber();
	n_NubIndex.EquNumberIndex = theMAINTENANCE_INFO.GetEquNumberInGroup();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     �����������Ƿ����������Ӧ���ʵ���ж�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::DoCheckInput()
{
	CString GuideContent = theAPP_SESSION.GetInputToString();	// ������������Ϊ�ղ�������
	if(_T("") == GuideContent)
	{
		return;
	}
	int index = theAPP_SESSION.GetInputToInt();					// ��ȡ������������
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:			// ��·����
		if(index<0 || index>99) // 0-255֮��Ƿ�
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.LineIndex = index;
			m_DataModel.sLineCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_2:		// ��վ����		
		if(index<0 || index>99)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.StationIndex = index;
			m_DataModel.sStationCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_3:		// ��������
		if(index<1 || index>254)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.MachineIndex = index;
			m_DataModel.sMachineCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_4:		// ����
		if(index<0 || index>255)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.GroupIndex = index;
			m_DataModel.sGroupNumber.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_5:		// ���ڱ��
		if(index<0 || index>255)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.EquNumberIndex = index;
			m_DataModel.sEquNumberInGroup.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	default:
		break;

	}
	m_DataModel.CodeChange = TRUE;	// ���뷢���仯��־
	NotifyDataModelChanged();

	EnterTime = 1;					// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief     ��GuideMsg�ֱ�ֵ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::InputGuideMsg()
{
	switch(m_DataModel.MenuNumber)	// ���ݲ˵���ѡ����Ӧ����ʾ
	{
	case MENU_CODE_1:				// ��·����
		GuideMsg = _opl(STATIONSET_LINE_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(2);
		break;
	case MENU_CODE_2:				// ��վ����
		GuideMsg =  _opl(STATIONSET_STATION_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(2);
		break;
	case MENU_CODE_3:				// ��������
		GuideMsg =  _opl(STATIONSET_MACHINE_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	case MENU_CODE_4:				// ����
		GuideMsg = _opl(STATIONSET_GROUP_NUMBER)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	case MENU_CODE_5:				// ���ڱ��
		GuideMsg = _opl(STATIONSET_EQUNOINGROUP)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStationSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		theMAINTENANCE_INFO.SetStationCode(n_NubIndex.LineIndex*100+n_NubIndex.StationIndex);
		theMAINTENANCE_INFO.SetMachineCode(n_NubIndex.MachineIndex);
		theMAINTENANCE_INFO.SetGroupNumber(n_NubIndex.GroupIndex);
		theMAINTENANCE_INFO.SetEquNumberInGroup(n_NubIndex.EquNumberIndex);
		theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));

		// ����������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEVICE_SETTINGS,true);
		return TRUE;
	}
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
LRESULT CStationSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;		// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
		theAPP_SESSION.SetMaxInputLength(1);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStationSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}