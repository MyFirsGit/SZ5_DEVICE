#include "stdafx.h"
#include "UDSNSetMaintainDlg.h"
#include "AppSession.h"
#include "TVM.h"
#include "XButton.h"
#include "ExceptionInfo.h"
#include "UDSNSetSvc.h"
#include "guideinfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CUDSNSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CUDSNSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

static CRect editUDSNinfo[] = 
{
	BASE_INFO_RECT(1,10),
	BASE_INFO_RECT(2,10),
};

#define TXT_ACC_UDSN	_opl(UDSNSET_TXT_ACC_UDSN)+_opl(TXT_COLON)  
#define TXT_AFC_UDSN	_opl(UDSNSET_TXT_AFC_UDSN)+_opl(TXT_COLON)  
//static const char* const TXT_ECT_UDSN						 = 	"ECT��ˮ�ţ�";

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetMaintainDlg::CUDSNSetMaintainDlg(CService* pService)
	: COperationDlg(CUDSNSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = _opl(TITLE_UDSN_INFO);				   // ����
	m_baseInfo->detailInfo.row = 2;                                        // ����
	m_baseInfo->detailInfo.labelGroup = m_labelUDSNinfo;                   // ��ǩ����
	m_baseInfo->detailInfo.editGroup.cnt = 2;                              // �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editUDSNinfo;              // �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetMaintainDlg::~CUDSNSetMaintainDlg()
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
int CUDSNSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try
	//{
	//	if (__super::OnCreate(lpCreateStruct, BASE_AREA | BTN_OK | BTN_RESET | GUIDE_AREA) == -1)
	//	{
	//		return -1;
	//	}

	//	// ���öԻ�����������
	//	GetEdit(0)->SetLimitText(10);
	//	GetEdit(1)->SetLimitText(10);
	//	// ��ʼ�����������label��Ϣ
	//	InitLabelUDSNInfo();
	//	// ��ʼ��Edit���������
	//	InitEditTextUDSNInfo();
	//}
	//catch (CSysException& e)
	//{
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...)
	//{
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
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
void CUDSNSetMaintainDlg::InitLabelUDSNInfo()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		// ��ʼ��USDN��Ϣ��������
		m_labelUDSNinfo[0].nameLeft = add_hsp(TXT_ACC_UDSN, MAX_HALF_CASE_LEFT_LEN);
		m_labelUDSNinfo[1].nameLeft = add_hsp(TXT_AFC_UDSN, MAX_HALF_CASE_LEFT_LEN);
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CUDSNSetMaintainDlg::InitEditTextUDSNInfo()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		CString sAccUDSN, sAfcUDSN, sEctUDSN;

		// ��ȡACC,AFC��ECT����ˮ��
		sAccUDSN.Format(_T("%u"),theCOUNT_INFO.GetUDSNForDisplay());
		sAfcUDSN.Format(_T("%u"),theCOUNT_INFO.GetTransactionNumForDsp());

		// ��ACC,AFC��ECT����ˮ�����õ�����
		SetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN,sAccUDSN);
		SetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN,sAfcUDSN);

		// ���ñ༭�򽹵�
		SetEditFocus(UDSNINFO_EDIT_GROUP_ACCUDSN);
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��EDIT���������ݵı��浽SVC����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetMaintainDlg::SetUDSNInfoToSvc()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
	{
		// �ӽ���ȡ����ˮ����Ϣ
		// ����ˮ����Ϣд���ļ�
		pUDSNSetSvc->SetAccUDSN(_tcstoul(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN), NULL, 10));
		pUDSNSetSvc->SetAfcUDSN(_tcstoul(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN), NULL, 10));
	}
	catch(CSysException&)
	{
		throw;
	}
	catch (...)
	{
		throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���EDIT����������Ƿ�Ϸ�

@param      void

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CUDSNSetMaintainDlg::CheckInputText()
{
	CUDSNSetSvc* pUDSNSetSvc = (CUDSNSetSvc*)GetService();
	try
    {
        // �����д����ˮ���Ƿ�Ϸ�
        INT64 number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN));
		if ( number > 0xFFFFFFFF )
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_ACCUDSN);
			return false;
		}

        number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN));
        if ( number > 0xFFFFFFFF )
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_AFCUDSN);
            return false;
        }

       /* number = _ttoi64(GetEditText(UDSNINFO_EDIT_GROUP_ECTUDSN));
        if ( number > 0xFFFFFFFF ){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INPUT_ERROR));
			SetEditFocus(UDSNINFO_EDIT_GROUP_ECTUDSN);
            return false;
        }*/

        return true;

    }
    catch (CSysException)
	{
        throw;
    }
    catch (...)
	{
        throw CInnerException(pUDSNSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
LRESULT  CUDSNSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// ��ʼ���༭��
	InitEditTextUDSNInfo();
	// ���õ�����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INFO_INPUT));
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
LRESULT  CUDSNSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//CString sMsg = (LPCTSTR)lParam;
	//try
	//{		
	//	// ��������Ƿ����Ҫ��
	//	if (CheckInputText())
	//	{
	//		// ���ı����õ�������
	//		SetUDSNInfoToSvc();
	//		//
	//		m_labelUDSNinfo[0].nameRight = add_hsp_right(GetEditText(UDSNINFO_EDIT_GROUP_ACCUDSN), MAX_HALF_CASE_LEFT_LEN);
	//		m_labelUDSNinfo[1].nameRight = add_hsp_right(GetEditText(UDSNINFO_EDIT_GROUP_AFCUDSN), MAX_HALF_CASE_LEFT_LEN);
	//		// ��ʾ���óɹ�
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_USDN_SET_FINISH));
	//		// ���ص���ť
	//		HideCompoment(BTN_OK | BTN_RESET | EDIT_BOX);
	//		return __super::XButtonClick(wParam,lParam);
	//	}
	//}
	//catch (CSysException& e)
	//{
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...)
	//{
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	return 0;
}