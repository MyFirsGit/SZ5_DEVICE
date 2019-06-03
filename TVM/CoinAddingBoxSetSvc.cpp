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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxSetSvc::CCoinAddingBoxSetSvc()
	:CTVMForeService(MODE_SWITCH_SVC)//本应为补充箱币种设置，现在用模式切换代替，所以菜单先选模式切换
{
	EnterTime = 1;
	GetGuide = _T("");

	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CCoinAddingBoxMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COINADDINGBOX_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COINADDINGBOX_SET,IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COINADDINGBOX_SET);//设置初始画面组
	ReadTvmIni();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxSetSvc::~CCoinAddingBoxSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

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
@brief      读取TVM.INI文件

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::ReadTvmIni()
{
	m_DataModel.hopone = theTVM_INFO.GetCHHopper1Type();
	m_DataModel.hoponecurrency = ReadCurrency(m_DataModel.hopone);	//补充箱A币种
	m_DataModel.hoptwo = theTVM_INFO.GetCHHopper2Type();
	m_DataModel.hoptwocurrency = ReadCurrency(m_DataModel.hoptwo);	//补充箱B币种
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      识别币种

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinAddingBoxSetSvc::ReadCurrency(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)		//从TVM.INI文件中读取的键值
	{
	case MENU_CODE_0:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//多币种
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
@brief      处理 [0~9] 键按下消息

@param      (i)WPARAM wParam  数字键值，例如：数字键7，值为7
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CCoinAddingBoxSetSvc::OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/)
//{
//	if(1 == EnterTime)		//第一次数字键按下选择相应的菜单
//	{
//		m_DataModel.CodeChange = FALSE;
//		theAPP_SESSION.AppendOneNumToGuide(wParam);
//		return TRUE;
//	}
//	if(2 == EnterTime)		//第二次提示后追加相应项代码
//	{
//		theAPP_SESSION.SetPrefixToGuide(GetGuide);
//		theAPP_SESSION.AppendOneNumToGuide(wParam);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinAddingBoxSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(1 == EnterTime)			//第一次按下Enter键确认菜单的选择
	{
		CheckMenuNumber();
	}
	else if(2 == EnterTime)		//第二次按下Enter键确认代码设置的选择
	{
		DoCheckInput();			//对输入数据是否合理做出相应项的实际判断
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      菜单号是否非法判断

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::CheckMenuNumber()
{
	// 验证输入的菜单编号
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > MAX_COINADDMENU_NUM)//主菜单编号除1-2之外的为非法菜单号
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		InputGetGuide();		//对GetGuide分别赋值
		EnterTime = 2;
		m_DataModel.MenuChange = TRUE;		//菜单号发生变化标志
		NotifyDataModelChanged();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对GetGuide分别赋值

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::InputGetGuide()
{
	switch(m_DataModel.MenuNumber)//依据菜单号选择相应的提示
	{
	case MENU_CODE_1:		//补充箱A
		GetGuide = _opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) ;
		break;
	case MENU_CODE_2:		//补充箱B
		GetGuide = _opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对输入数据是否合理做出相应项的实际判断

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxSetSvc::DoCheckInput()
{
	int index = theAPP_SESSION.GetInputToInt();//获取向导栏输入内容
	CString hopetype = theAPP_SESSION.GetInputToString();
	if(_T("") == hopetype)
	{
		switch(m_DataModel.MenuNumber)
		{
		case MENU_CODE_1:	//补充箱A
			index = theTVM_INFO.GetCHHopper1Type();
			break;
		case MENU_CODE_2:	//补充箱B
			index = theTVM_INFO.GetCHHopper2Type();
			break;
		default:
			break;
		}
	}
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:		//补充箱A
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
	case MENU_CODE_2:		//补充箱B
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
	m_DataModel.CodeChange = TRUE;//代码发生变化标志
	m_DataModel.MenuChange = FALSE;//置菜单号变化标志位FALSE防止菜单号发生更新
	NotifyDataModelChanged();

	EnterTime = 1;//置按键次数为1，重新从选择菜单编号开始
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对菜单号转换后保存

@param      void

@retval     none

@exception  无
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
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinAddingBoxSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CString empty = theAPP_SESSION.GetInputToString();
	if (_T("") == empty)			//向导栏内容为空
	{
		EnterTime = 1;				//置按键次数为1，重新从选择菜单编号开始
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
	return TRUE;
}
