#include "stdafx.h"
#include "ComportAndBaudrateSetSvc.h"
#include "ComportAndBaudrateSetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CComportAndBaudrateSetSvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetSvc::CComportAndBaudrateSetSvc() : CTVMForeService(DEVICE_COMPORT_SET_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(CComportAndBaudrateSetDlg::IDD,new CComportAndBaudrateSetDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMPORT_BAUD_SET, CComportAndBaudrateSetDlg::IDD);
	
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET);	// ���ó�ʼ������
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetSvc::~CComportAndBaudrateSetSvc()
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
void CComportAndBaudrateSetSvc::OnStart()
{
	__super::OnStart();
	
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET);	// ���ó�ʼ������
	
	m_data_model = _tag_data_model_();
	
	ReadFromSettingIni();
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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/){

	// ��ǰδѡ���κβ˵�
	if(m_data_model.iCurSelectMenu == 0){

		// �жϲ˵��Ϸ���
		int nIndex = theAPP_SESSION.GetInputToInt();// 21

		int nMainMenu	= nIndex / 10;				// 2
		int nChildMenu	= nIndex % 10;				// 1


		// �Ӳ˵�ֻ������
		if(nChildMenu != MENU_CODE_1 && nChildMenu != MENU_CODE_2){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}

		m_data_model.iCurSelectMenu = nMainMenu;
		m_data_model.iCurSelSonMenu = nChildMenu;

		CString strMainModelName(_T(""));

		
		// ��֤���˵�����ʾ
		switch(nMainMenu){
		case MENU_KEY_RW:// ��д��
		//case MENU_KEY_BHCHANGE://��ֵ��д��
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_RW_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_BH:// ֽ�ҽ���
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_BH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		case MENU_KEY_TH:// Ʊ��
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_TH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		case MENU_KEY_CH:// Ӳ��ģ��
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_CH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_PRINT:// ��ӡ��
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_PRINTER_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		case MENU_KEY_DISPLAY:// ״̬��ʾ��
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_DISPLAY_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		case MENU_KEY_IO:// IO
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_IO_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_BHCHANGE:// ֽ������
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_BHCHANGE_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		default:
			m_data_model.iCurSelectMenu = 0;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			break;
		}

		nChildMenu == MENU_CODE_1 ? theAPP_SESSION.SetMaxInputLength(2) : theAPP_SESSION.SetMaxInputLength(6);//115200

		theAPP_SESSION.SetPrefixToGuide(strMainModelName);
	}
	else{
		CString strInputDetail = theAPP_SESSION.GetInputToString();

		if(strInputDetail.IsEmpty()){
			m_data_model.iCurSelectMenu = m_data_model.iCurSelSonMenu = 0;
			theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_INPUT_ILLEGAL);
			return FALSE;
		}

		Menu_Detail& menuDetail = m_data_model.stMenuDetail[m_data_model.iCurSelectMenu -1];

		// ���ں�����
		if(m_data_model.iCurSelSonMenu == MENU_CODE_1){
			menuDetail.strComport = strInputDetail;
			menuDetail.emComportStatus = MENU_STATUS_SELECT;
		}
		// ����������
		else{
			menuDetail.strBaudRate = strInputDetail;
			menuDetail.emBaudRateStatus = MENU_STATUS_SELECT;
		}

		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F2_ESC);

		m_data_model.bAlreadySaved = false;

		m_data_model.iCurSelectMenu = m_data_model.iCurSelSonMenu = 0;

		theAPP_SESSION.SetMaxInputLength(2);

		NotifyDataModelChanged();
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
void CComportAndBaudrateSetSvc::NotifyDataModelChanged(){
	__super::NotifyDataModelChanged();
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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/){

	if(!m_data_model.bAlreadySaved){

		for(int i = 0;i< 7;i++){
			Menu_Detail& thisMenu = m_data_model.stMenuDetail[i];
			if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT || thisMenu.emComportStatus == MENU_STATUS_SELECT){
				int nMenu = i + 1;
				switch(nMenu){
				case MENU_KEY_RW:// ��д��
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetRwComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetRwBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_BH:// ֽ�ҽ���
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetBHComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetBHBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_TH:// Ʊ��
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetTokenComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetTokenBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;

					{
				case MENU_KEY_CH:
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetCHComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetCHBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_PRINT:// ��ӡ��
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetPrinterComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetPrinterBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_DISPLAY:// ״̬��ʾ��
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetDisplayComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetDisplayBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_BHCHANGE:// ֽ������
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetBanknoteChangeComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetBanknoteChangeBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_IO:// ״̬��ʾ��
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetIOComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetIOBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				default:
					break;
				}
			}
		}
		
		m_data_model.bAlreadySaved = true;

		theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_SAVE_SUCCESS);
		
		// ˢ�½���
		NotifyDataModelChanged();
		return TRUE;
	}

	theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_PLEASE_EDIT_COMPORT_BAUDRATE);

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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/){
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����ں�������Ϣ�������ļ��ж�ȡ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetSvc::ReadFromSettingIni(){

	int index = 0;
	m_data_model.stMenuDetail[index].strMenuName = /*_opl(GUIDE_SMARTCARD_RW_SET);*/_T("��д��");
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetRWComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetRWBaudRate());
	index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_BH);//.Format(_T("ֽ�ҽ���"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetBHComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetBHBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_TH);//.Format(_T("Ʊ��"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetTHComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetTHBaudRate());
	index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_CH);//.Format(_T("Ӳ��"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetCHComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetCHBaudRate());
	index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_PRINTER);//.Format(_T("��ӡ��"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetPrinterComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetPrinterBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_DISPLAY);//.Format(_T("״̬��ʾ��"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetDisplayComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetDisplayBaudRate());
	index++;
	//m_data_model.stMenuDetail[index].strMenuName = _opl(GUIDE_RECHARGE_SET);//.Format(_T("��д��"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetChargeRWComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetChargeRWBaudRate());
	//index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_BHCHANGE);//.Format(_T("ֽ������"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetBanknoteChangeComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetBanknoteChangeBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName.Format(_T("IO"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetIOComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetIOBaudRate());
	index++;
}