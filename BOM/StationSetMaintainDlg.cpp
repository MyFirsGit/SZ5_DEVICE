#include "stdafx.h"
#include "StationSetMaintainDlg.h"
#include "StationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CStationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

static CRect editStationinfo[] = 
{
	BASE_INFO_RECT(1,4),
	BASE_INFO_RECT(2,4),
	BASE_INFO_RECT(3,4),
	BASE_INFO_RECT(4,4),
	BASE_INFO_RECT(5,4),
};

static const TCHAR* const TXT_LINE_CODE			=	_T("��·���룺");
static const TCHAR* const TXT_STATION_CODE		= 	_T("��վ���룺");
static const TCHAR* const TXT_STATION_NAME		= 	_T("��վ����");
static const TCHAR* const TXT_MACHINE_CODE		= 	_T("�������룺");
static const TCHAR* const TXT_GROUP_NUMBER		=	_T("��ţ�");
static const TCHAR* const TXT_EQUNOINGROUP		=	_T("���ڱ�ţ�");

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::CStationSetMaintainDlg(CService* pService) 
	: COperationDlg(CStationSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_STATION_INFO;                  // ����
	m_baseInfo->detailInfo.row = 5;                                        // ����
	m_baseInfo->detailInfo.labelGroup = m_labelStationinfo;                // ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 5;                              // �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editStationinfo;           // �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::~CStationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CStationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		if (__super::OnCreate(lpCreateStruct,  BASE_AREA | BTN_RESET | BTN_OK | GUIDE_AREA) == -1){
			return -1;
		}
		// ���öԻ�����������
		GetEdit(0)->SetLimitText(2);
		GetEdit(1)->SetLimitText(2);
		GetEdit(2)->SetLimitText(2);
		GetEdit(3)->SetLimitText(2);
		GetEdit(4)->SetLimitText(2);
		// ��ʼ�����������label��Ϣ
		InitlabelStationInfo();
		// ��ʼ��Edit���������
		InitEditText();
	}
	catch (CSysException& e)	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::InitlabelStationInfo()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try {
		// ��ʼ����վ��Ϣ��������
		m_labelStationinfo[0].nameLeft = add_wsp(TXT_LINE_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[1].nameLeft = add_wsp(TXT_STATION_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[2].nameLeft = add_wsp(TXT_MACHINE_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[3].nameLeft = add_wsp(TXT_GROUP_NUMBER,MAX_SBC_CASE_LEN);
		m_labelStationinfo[4].nameLeft = add_wsp(TXT_EQUNOINGROUP,MAX_SBC_CASE_LEN);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...)	{
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����EDIT���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::InitEditText()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try{
		if(theMAINTENANCE_INFO.Initialize() == 0){
			// ��·���룬��վ���룬�������룬��ţ����ڱ��
			CString sLineCode,sStationCode,sMachineCode,sGroupNumber,sEquNumberInGroup;
			// ��ά����Ϣ��ȡ����·���룬��վ���룬�������룬��ţ����ڱ��
			sLineCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetLineCode());
			sStationCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetStationCode());
			sMachineCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetMachineCode());
			sGroupNumber.Format(_T("%.2X"),theMAINTENANCE_INFO.GetGroupNumber());
			sEquNumberInGroup.Format(_T("%.2X"),theMAINTENANCE_INFO.GetEquNumberInGroup());

			// ����վ���룬�������룬��ţ����ڱ�����õ�����
			SetEditText(STATIONINFO_EDIT_GROUP_LINECODE,sLineCode);
			SetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE,sStationCode);
			SetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE,sMachineCode);
			SetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER,sGroupNumber);
			SetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP,sEquNumberInGroup);

			// ���ñ༭�򽹵�
			SetEditFocus(STATIONINFO_EDIT_GROUP_LINECODE);
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_T("���죩��ȡ��վ��Ϣʧ��"));
			HideCompoment(BTN_OK | BTN_RESET | BASE_AREA);
		}
	}
	catch(CSysException&){
		throw;
	}
	catch (...) {
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó�վ��Ϣ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::SetStationInfo()
{
	// ��ά����Ϣ��ȡ����·���룬��վ���룬�������룬��ţ����ڱ��
	int iLineCode  = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE).GetBuffer(),NULL,16);
	int iStationCode = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE).GetBuffer(),NULL,16);
	int iMachineCode = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE).GetBuffer(),NULL,16);
	int iGroupNumber =  _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER).GetBuffer(),NULL,16);
	int iEquNumberInGroup =_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP).GetBuffer(),NULL,16);
	// ���ó�վ��Ϣ
	theMAINTENANCE_INFO.SetLineCode(iLineCode);
	theMAINTENANCE_INFO.SetStationCode(iStationCode);
	theMAINTENANCE_INFO.SetMachineCode(iMachineCode);
	theMAINTENANCE_INFO.SetGroupNumber(iGroupNumber);
	theMAINTENANCE_INFO.SetEquNumberInGroup(iEquNumberInGroup);
	theMAINTENANCE_INFO.Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���EDIT����������Ƿ�Ϸ�

@param      void

@retval     int  0  �ɹ�
-1 ���ɹ�
1  ����ĳ�վ����Ʊ�۱�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CStationSetMaintainDlg::CheckInputText()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try {
		// �����·�����Ƿ���(0 - 255֮��)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_LINECODE);
			return -1;
		}
		// ��鳵վ�����Ƿ���(0 - 255֮��)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE),0,255)) {
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_STATIONCODE);
			return -1;
		}
		// ��鱾�������Ƿ���(01 - 254֮��)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE),1,254)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_MACHINECODE);
			return -1 ;
		}
		// �������Ƿ���(0 - 255֮��)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_GROUPNUMBER);
			return -1 ;
		}
		// ������ڱ���Ƿ���(0 - 255֮��)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_EQUNOINGROUP);
			return -1 ;
		}
		// ͨ����վ�����ó�վ���ƣ������ò���������false,�����õ��ˣ�����true
		BYTE theLineCode = (_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE).GetBuffer(),NULL,16) & 0xFF);
		BYTE theStationCode = (_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE).GetBuffer(),NULL,16) & 0xFF);
		// ����·�ţ���վ�Ż�ȡ��վID
		WORD dStationId = MAKEWORD(theStationCode,theLineCode);
		// �ɳ�վID��ȡ��վ��Ϣ�б�
		CAccLineInfoParam::_station_Info vStationInfoList;
		bool bSuccess = theACC_LINE.GetStationInfobyCode(dStationId,vStationInfoList);
		if (!bSuccess){
			return 1;
		}
		else{
			// ���õ�ǰ��վ����
			CString station_name_cn = vStationInfoList.station_Chinese_Name;
			CString station_name_en = vStationInfoList.station_Eng_Name;
			theAPP_SESSION.SetStationName(station_name_cn,station_name_en);
		}
		return 0 ;
	}
	catch(CSysException&){
		throw;
	}
	catch (...) {
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϡ����ء����� ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CStationSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// ��ʼ���༭��
	InitEditText();
	// ���õ�����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INFO_INPUT));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CStationSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// ��������Ƿ����Ҫ��
		int errorCode = CheckInputText();
		// �������Ϸ�
		if(errorCode == 0){
			// ���ó�վ��Ϣ
			SetStationInfo();
			HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_FINISH));
		}
		else if(errorCode == 1){
			SetStationInfo();
			HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_FARE_ERROR));
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ؽ������

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// ���ñ�ǩ����
	m_labelStationinfo[0].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_LINECODE);
	m_labelStationinfo[1].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE);
	m_labelStationinfo[2].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE);
	m_labelStationinfo[3].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER);
	m_labelStationinfo[4].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP);
	// �������
	HideCompoment(ulCompomentFlag);
}
