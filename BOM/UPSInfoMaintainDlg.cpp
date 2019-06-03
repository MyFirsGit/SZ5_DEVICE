#include "stdafx.h"
#include "UPSInfoMaintainDlg.h"
#include "AppSession.h"
#include "BOM.h"
#include "XButton.h"
#include "GlobalParameter.h"
#include "SystemSetSvc.h"
#include "CAccLineInfoParam.h"
#include "UPSInfoSvc.h"
#include "guideinfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CUPSInfoMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CUPSInfoMaintainDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoMaintainDlg::CUPSInfoMaintainDlg(CService* pService)
: COperationDlg(CUPSInfoMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_UPS_INFO;					   // ����
	m_baseInfo->detailInfo.row = 2;                                        // ����
	m_baseInfo->detailInfo.labelGroup = m_labelUPSInfo;                   // ��ǩ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoMaintainDlg::~CUPSInfoMaintainDlg()
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
int CUPSInfoMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try
	{
		if (__super::OnCreate(lpCreateStruct, BASE_AREA | GUIDE_AREA) == -1)
		{
			return -1;
		}

		// ��ʼ�����������label��Ϣ
		InitLabelUPSInfo();
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)
	{
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
void CUPSInfoMaintainDlg::InitLabelUPSInfo()
{
	CUPSInfoSvc* pUPSInfoSvc = (CUPSInfoSvc*)GetService();
	try
	{
		// ��ʼ��USDN��Ϣ��������
		m_labelUPSInfo[0].nameLeft = add_wsp(_T("ʣ������ٷֱȣ�"), MAX_SBC_CASE_LEN);
		m_labelUPSInfo[1].nameLeft = add_wsp(_T("ʣ��ʱ�䣺"), MAX_SBC_CASE_LEN);

		CString strTxt;
		strTxt.Format(_T("%d%%"), pUPSInfoSvc->GetUPSHelper()->GetUPSBatteryCapacity());
		m_labelUPSInfo[0].nameRight = strTxt;
		strTxt.Format(_T("%d����"), pUPSInfoSvc->GetUPSHelper()->GetUPSLeftStandbyTime());
		m_labelUPSInfo[1].nameRight = strTxt;

		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_SUCCESS));
	}
	catch(CSysException&)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_FAIL));
		//throw;
	}
	catch (...)
	{
		throw CInnerException(pUPSInfoSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}