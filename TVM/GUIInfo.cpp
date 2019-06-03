#include "StdAfx.h"
#include "GUIInfo.h"
#include "SysException.h"
#include "Util.h"
#include "tvmsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GUI_INI_FILE_NAME		    _T("TVM_GUIInfo.INI")

CGUIInfo CGUIInfo::theInstance;

CGUIInfo& CGUIInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGUIInfo::CGUIInfo()
{
	try {
		m_GUIIni = NULL;
		Initialize();
	}
	catch (...) {
		// 忽略异常信息
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGUIInfo::~CGUIInfo()
{
	delete m_GUIIni;
	m_GUIIni = NULL;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       (i)CString fileName   GUI设置文件名称 

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CGUIInfo::Initialize() 
{
	try {
		// 已经初始化过后就不用再次初始化
		if(m_GUIIni != NULL){
			return 0;
		}
		// 初期化
		TCHAR szAppPath[_MAX_PATH];
		ComGetAppPath(szAppPath, _MAX_PATH);
		CString fileName = CString(szAppPath) + GUI_INI_FILE_NAME;
		
		// check .INI file exist
		if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
		}

		// 文件属性改为可写
		SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
		m_GUIIni = new CGetIni(fileName);

		// 获取操作员界面中文字体属性
		CString sKey = _T("OPERATOR_FONT_CN");
		m_OperatorGuideFontSize[LANGUAGE_TYPE_INDEX_CN] = m_GUIIni->GetDataInt(sKey,_T("OPERATOR_GUIDE_FONT_SIZE"),18);
		m_OperatorTitleFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE"),16);
		m_OperatorBtnFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("BUTTON_FONT_SIZE"),15);
		m_OperatorIniFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("INI_FONT_SIZE"),18);
		
		m_OperatorNormalFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("NORMAL_FONT_SIZE"),14);
		m_OperatorGuideFontName.RemoveAll();
		m_OperatorTitleFontName.RemoveAll();
		m_OperatorBtnFontName.RemoveAll();
		m_OperatorIniFontName.RemoveAll();
		m_OperatorNormalFontName.RemoveAll();
		m_OperatorCharSet.RemoveAll();
		m_OperatorGuideFontName.Add(m_GUIIni->GetDataString(sKey,_T("OPERATOR_GUIDE_FONT_NAME"),_T("宋体")));
		m_OperatorTitleFontName.Add(m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME"),_T("宋体")));
		m_OperatorBtnFontName.Add(m_GUIIni->GetDataString(sKey,_T("BUTTON_FONT_NAME"),_T("宋体")));
		m_OperatorIniFontName.Add(m_GUIIni->GetDataString(sKey,_T("INI_FONT_NAME"),_T("宋体")));
		m_OperatorNormalFontName.Add(m_GUIIni->GetDataString(sKey,_T("NORMAL_FONT_NAME"),_T("宋体")));
		m_OperatorCharSet .Add(m_GUIIni->GetDataInt(sKey,_T("CHARSET"),136));
		sKey = _T("OPERATOR_FONT_EN");
		m_OperatorGuideFontSize[LANGUAGE_TYPE_INDEX_EN] = m_GUIIni->GetDataInt(sKey,_T("OPERATOR_GUIDE_FONT_SIZE"),18);
		m_OperatorTitleFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE"),16);
		m_OperatorBtnFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("BUTTON_FONT_SIZE"),15);
		m_OperatorIniFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("INI_FONT_SIZE"),18);
		
		m_OperatorNormalFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("NORMAL_FONT_SIZE"),14);
		m_OperatorGuideFontName.Add(m_GUIIni->GetDataString(sKey,_T("OPERATOR_GUIDE_FONT_NAME"),_T("宋体")));
		m_OperatorTitleFontName.Add(m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME"),_T("宋体")));
		m_OperatorBtnFontName.Add(m_GUIIni->GetDataString(sKey,_T("BUTTON_FONT_NAME"),_T("宋体")));
		m_OperatorIniFontName.Add(m_GUIIni->GetDataString(sKey,_T("INI_FONT_NAME"),_T("宋体")));
		m_OperatorNormalFontName.Add(m_GUIIni->GetDataString(sKey,_T("NORMAL_FONT_NAME"),_T("宋体")));
		m_OperatorCharSet .Add(m_GUIIni->GetDataInt(sKey,_T("CHARSET"),136));
		sKey = _T("OPERATOR_FONT_PN");
		m_OperatorGuideFontSize[LANGUAGE_TYPE_INDEX_PN] = m_GUIIni->GetDataInt(sKey,_T("OPERATOR_GUIDE_FONT_SIZE"),18);
		m_OperatorTitleFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE"),16);
		m_OperatorBtnFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("BUTTON_FONT_SIZE"),15);
		m_OperatorIniFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("INI_FONT_SIZE"),18);
		
		m_OperatorNormalFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("NORMAL_FONT_SIZE"),14);
		m_OperatorGuideFontName.Add(m_GUIIni->GetDataString(sKey,_T("OPERATOR_GUIDE_FONT_NAME"),_T("宋体")));
		m_OperatorTitleFontName.Add(m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME"),_T("宋体")));
		m_OperatorBtnFontName.Add(m_GUIIni->GetDataString(sKey,_T("BUTTON_FONT_NAME"),_T("宋体")));
		m_OperatorIniFontName.Add(m_GUIIni->GetDataString(sKey,_T("INI_FONT_NAME"),_T("宋体")));
		m_OperatorNormalFontName.Add(m_GUIIni->GetDataString(sKey,_T("NORMAL_FONT_NAME"),_T("宋体")));
		m_OperatorCharSet .Add(m_GUIIni->GetDataInt(sKey,_T("CHARSET"),136));

		// 乘客界面
		sKey = _T("CLIENT_FONT");
		m_RecStationFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("STATION_FONT_SIZE_CN"),30);
		m_RecStationFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("STATION_FONT_SIZE_EN"),30);
		m_RecStationFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("STATION_FONT_SIZE_PN"),30);
		m_RecStationFontName.RemoveAll();
		m_RecStationFontName.Add( m_GUIIni->GetDataString(sKey,_T("STATION_FONT_NAME_CN"),_T("宋体")));
		m_RecStationFontName.Add( m_GUIIni->GetDataString(sKey,_T("STATION_FONT_NAME_EN"),_T("Arail")));
		m_RecStationFontName.Add( m_GUIIni->GetDataString(sKey,_T("STATION_FONT_NAME_PN"),_T("Arail")));
		m_RecTitleFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE_CN"),15);
		m_RecTitleFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE_EN"),18);
		m_RecTitleFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("TITLE_FONT_SIZE_PN"),18);
		m_RecTitleFontName.RemoveAll();
		m_RecTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME_CN"),_T("宋体")));
		m_RecTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME_EN"),_T("Arail")));
		m_RecTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("TITLE_FONT_NAME_PN"),_T("Arail")));
		m_RecDateFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("DATE_FONT_SIZE_CN"),15);
		m_RecDateFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("DATE_FONT_SIZE_EN"),18);
		m_RecDateFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("DATE_FONT_SIZE_PN"),18);
		m_RecDateFontName.RemoveAll();
		m_RecDateFontName.Add( m_GUIIni->GetDataString(sKey,_T("DATE_FONT_NAME_CN"),_T("宋体")));
		m_RecDateFontName.Add( m_GUIIni->GetDataString(sKey,_T("DATE_FONT_NAME_EN"),_T("Arail")));
		m_RecDateFontName.Add( m_GUIIni->GetDataString(sKey,_T("DATE_FONT_NAME_PN"),_T("Arail")));

		// 服务状态字体
		m_RecSvcStaeFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("SVC_STATE_FONT_SIZE_CN"),20);
		m_RecSvcStaeFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("SVC_STATE_FONT_SIZE_EN"),18);
		m_RecSvcStaeFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("SVC_STATE_FONT_SIZE_PN"),15);
		m_RecSvcStateFontName.RemoveAll();
		m_RecSvcStateFontName.Add( m_GUIIni->GetDataString(sKey,_T("SVC_STATE_FONT_NAME_CN "),_T("黑体")));
		m_RecSvcStateFontName.Add( m_GUIIni->GetDataString(sKey,_T("SVC_STATE_FONT_NAME_EN "),_T("Arail")));
		m_RecSvcStateFontName.Add( m_GUIIni->GetDataString(sKey,_T("SVC_STATE_FONT_NAME_PN "),_T("Arail")));
		//服务状态字体颜色
		m_RectNormalSvcStateFontColor = m_GUIIni->GetDataInt(sKey,_T("SVC_STATE_FONT_COLOR_NORMAL"),65280);
		m_RectUnNormalSvcStateFontColor = m_GUIIni->GetDataInt(sKey,_T("SVC_STATE_FONT_COLOR_UNNORM"),255);

		m_RecBaseInfoFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("BASEINFO_FONT_SIZE_CN"),15);
		m_RecBaseInfoFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("BASEINFO_FONT_SIZE_EN"),18);
		m_RecBaseInfoFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("BASEINFO_FONT_SIZE_PN"),18);
		m_RecBaseInfoFontName.RemoveAll();
		m_RecBaseInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("BASEINFO_FONT_NAME_CN"),_T("宋体")));
		m_RecBaseInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("BASEINFO_FONT_NAME_EN"),_T("Arail")));
		m_RecBaseInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("BASEINFO_FONT_NAME_PN"),_T("Arail")));

		m_RecGuideInfoFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("GUIDEINFO_FONT_SIZE_CN"),15);
		m_RecGuideInfoFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("GUIDEINFO_FONT_SIZE_EN"),18);
		m_RecGuideInfoFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("GUIDEINFO_FONT_SIZE_PN"),18);
		m_RecGuideInfoFontName.RemoveAll();
		m_RecGuideInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("GUIDEINFO_FONT_NAME_CN"),_T("宋体")));
		m_RecGuideInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("GUIDEINFO_FONT_NAME_EN"),_T("Arail")));
		m_RecGuideInfoFontName.Add( m_GUIIni->GetDataString(sKey,_T("GUIDEINFO_FONT_NAME_PN"),_T("Arail")));
		m_RecMessageBoxFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOX_FONT_SIZE_CN"),15);
		m_RecMessageBoxFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOX_FONT_SIZE_EN"),18);
		m_RecMessageBoxFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOX_FONT_SIZE_PN"),18);
		m_RecMessageBoxFontName.RemoveAll();
		m_RecMessageBoxFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOX_FONT_NAME_CN"),_T("宋体")));
		m_RecMessageBoxFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOX_FONT_NAME_EN"),_T("Arail")));
		m_RecMessageBoxFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOX_FONT_NAME_PN"),_T("Arail")));
		m_RecMessageBoxTitleFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOXTITLE_FONT_SIZE_CN"),15);
		m_RecMessageBoxTitleFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOXTITLE_FONT_SIZE_EN"),18);
		m_RecMessageBoxTitleFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("MESSAGEBOXTITLE_FONT_SIZE_PN"),18);
		m_RecMessageBoxTitleFontName.RemoveAll();
		m_RecMessageBoxTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOXTITLE_FONT_NAME_CN"),_T("宋体")));
		m_RecMessageBoxTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOXTITLE_FONT_NAME_EN"),_T("Arail")));
		m_RecMessageBoxTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("MESSAGEBOXTITLE_FONT_NAME_PN"),_T("Arail")));

		m_FuncBtnFontSize[0] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_FONT_SIZE_CN"), 18);// 功能按钮取字体大小
		m_FuncBtnFontSize[1] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_FONT_SIZE_EN"), 18);
		m_FuncBtnFontSize[2] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_FONT_SIZE_PN"), 18);
		m_FuncBtnFontName.RemoveAll();
		m_FuncBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_FONT_NAME_CN"),_T("宋体")));//功能按钮字体名称
		m_FuncBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_FONT_NAME_EN"),_T("Arail")));
		m_FuncBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_FONT_NAME_PN"),_T("Arail")));

		m_MenueBtnFontSize[0] = m_GUIIni->GetDataInt(sKey, _T("MENU_BUTTON_FONT_SIZE_CN"), 18);// 菜单按钮取字体大小
		m_MenueBtnFontSize[1] = m_GUIIni->GetDataInt(sKey, _T("MENU_BUTTON_FONT_SIZE_EN"), 18);
		m_MenueBtnFontSize[2] = m_GUIIni->GetDataInt(sKey, _T("MENU_BUTTON_FONT_SIZE_PN"), 18);
		m_MenueBtnFontName.RemoveAll();
		m_MenueBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("MENU_BUTTON_FONT_NAME_CN"),_T("宋体")));//菜单按钮字体名称
		m_MenueBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("MENU_BUTTON_FONT_NAME_EN"),_T("Arail")));
		m_MenueBtnFontName.Add( m_GUIIni->GetDataString(sKey,_T("MENU_BUTTON_FONT_NAME_PN"),_T("Arail")));

		m_FuncBtnGroupTitleFontSize[0] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_GROUP_TITLE_FONT_SIZE_CN"), 18);// 功能按钮区域标题取字体大小
		m_FuncBtnGroupTitleFontSize[1] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_GROUP_TITLE_FONT_SIZE_EN"), 18);
		m_FuncBtnGroupTitleFontSize[2] = m_GUIIni->GetDataInt(sKey, _T("FUNCTION_BUTTON_GROUP_TITLE_FONT_SIZE_PN"), 18);
		m_FuncBtnGroupTitleFontName.RemoveAll();
		m_FuncBtnGroupTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_NAME_CN"),_T("宋体")));//功能按钮区域标题取字体名称
		m_FuncBtnGroupTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_NAME_EN"),_T("Arail")));
		m_FuncBtnGroupTitleFontName.Add( m_GUIIni->GetDataString(sKey,_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_NAME_PN"),_T("Arail")));

		m_RecLogoFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("LOGO_FONT_SIZE_CN"),24);
		m_RecLogoFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("LOGO_FONT_SIZE_EN"),24);
		m_RecLogoFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("LOGO_FONT_SIZE_PN"),24);
		m_RecLogoFontName.Add(m_GUIIni->GetDataString(sKey,_T("LOGO_FONT_NAME_CN"),_T("黑体")));
		m_RecLogoFontName.Add(m_GUIIni->GetDataString(sKey,_T("LOGO_FONT_NAME_EN"),_T("黑体")));
		m_RecLogoFontName.Add(m_GUIIni->GetDataString(sKey,_T("LOGO_FONT_NAME_PN"),_T("黑体")));
		m_RecVersionInfoFontSize = m_GUIIni->GetDataInt(sKey,_T("VERSION_INFO_FONT_SIZE"),7);

		m_ServiceStateTxtName.RemoveAll();
		m_ServiceStateTxtName.Add(m_GUIIni->GetDataString(sKey,_T("SERVICE_STATE_TXT_FONT_NAME_CN"),_T("黑体")));  // 服务状态界面
		m_ServiceStateTxtName.Add(m_GUIIni->GetDataString(sKey,_T("SERVICE_STATE_TXT_FONT_NAME_EN"),_T("黑体")));
		m_ServiceStateTxtName.Add(m_GUIIni->GetDataString(sKey,_T("SERVICE_STATE_TXT_FONT_NAME_PN"),_T("黑体")));
		m_ServiceStateTxtColor[0] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_COLOR_CN"),255);  
		m_ServiceStateTxtColor[1] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_COLOR_EN"),255);
		m_ServiceStateTxtColor[2] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_COLOR_PN"),255);
		m_ServiceStateTxtSize[0] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_SIZE_CN"),100);
		m_ServiceStateTxtSize[1] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_SIZE_EN"),100);
		m_ServiceStateTxtSize[2] = m_GUIIni->GetDataInt(sKey,_T("SERVICE_STATE_TXT_FONT_SIZE_PN"),100);

		m_HistoryGridFontName.RemoveAll();
		m_HistoryGridFontName.Add(m_GUIIni->GetDataString(sKey,_T("HISTORY_GRID_TXT_FONT_NAME_CN"),_T("黑体")));
		m_HistoryGridFontName.Add(m_GUIIni->GetDataString(sKey,_T("HISTORY_GRID_TXT_FONT_NAME_EN"),_T("黑体")));
		m_HistoryGridFontName.Add(m_GUIIni->GetDataString(sKey,_T("HISTORY_GRID_TXT_FONT_NAME_PN"),_T("黑体")));
		m_HistoryGridFontColor[0] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_COLOR_CN"),0);  
		m_HistoryGridFontColor[1] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_COLOR_EN"),0);
		m_HistoryGridFontColor[2] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_COLOR_PN"),0);
		m_HistoryGridFontSize[0] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_SIZE_CN"),15);
		m_HistoryGridFontSize[1] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_SIZE_EN"),15);
		m_HistoryGridFontSize[2] = m_GUIIni->GetDataInt(sKey,_T("HISTORY_GRID_TXT_FONT_SIZE_PN"),15);

		// 背景色
		sKey = _T("GUI_COLOR");
		m_TitleBakColor = m_GUIIni->GetDataInt(sKey,_T("TITLE_AREA_COLOR"),6763776);
		m_BaseBakColor = m_GUIIni->GetDataInt(sKey,_T("BASE_AREA_COLOR "),15855852);
		m_StatusBakColor = m_GUIIni->GetDataInt(sKey,_T("STATUS_AREA_COLOR"),6763776);
		m_FunBakColor = m_GUIIni->GetDataInt(sKey,_T("FUNCTION_AREA_COLOR"),0);

		sKey  = _T("OPERATOR_GUI_COLOR");
		m_OpBaseAreaColor = m_GUIIni->GetDataInt(sKey,_T("BASE_AREA_COLOR"),15855852);
		m_OpGuidAreaColor = m_GUIIni->GetDataInt(sKey,_T("GUIDE_AREA_COLOR"),6763776);
		m_OpStatAreaColor = m_GUIIni->GetDataInt(sKey,_T("STATUS_AREA_COLOR"),6763776);
		m_OpStateFuncAreaColor = m_GUIIni->GetDataInt(sKey,_T("FUNCTION_AREA_COLOR"),6763776);

		// 操作员界面标题字体颜色
		m_TitleFontColor[0] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("TITLE_FONT_COLOR"),0 );
		m_TitleFontColor[1] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_EN"),_T("TITLE_FONT_COLOR"),0 );
		m_TitleFontColor[2] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_PN"),_T("TITLE_FONT_COLOR"),0 );

		// 操作员界面按钮字体颜色
		m_BtnFontColor[0] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("BUTTON_FONT_COLOR"),0 );
		m_BtnFontColor[1] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_EN"),_T("BUTTON_FONT_COLOR"),0 );
		m_BtnFontColor[2] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_PN"),_T("BUTTON_FONT_COLOR"),0 );

		// 操作员界面基本信息区域正文字体颜色
		m_NormalFontColor[0] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("NORMAL_FONT_COLOR"),0 );
		m_NormalFontColor[1] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_EN"),_T("NORMAL_FONT_COLOR"),0 );
		m_NormalFontColor[2] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_PN"),_T("NORMAL_FONT_COLOR"),0 );

		// 操作员界面向导信息区字体颜色
		m_GuideFontColor[0] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("OPERATPR_GUIDE_FONT_COLOR"),0 );
		m_GuideFontColor[1] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_EN"),_T("OPERATPR_GUIDE_FONT_COLOR"),0 );
		m_GuideFontColor[2] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_PN"),_T("OPERATPR_GUIDE_FONT_COLOR"),0 );

		//初始化界面字体颜色
		m_InitFontColor[0] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("INI_FONT_COLOR "),0 );
		m_InitFontColor[1] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_EN"),_T("INI_FONT_COLOR "),0 );
		m_InitFontColor[2] = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_PN"),_T("INI_FONT_COLOR "),0 );
		
		// 初始化结果成功字体颜色 默认黑色
		m_InitSucceedFontColor = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("INI_RESULT_SUCCES_FONT_COLOR") , 0);
		// 初始化结果失败字体颜色  默认红色
		m_InitFailedFontColor = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("INI_RESULT_FAILED_FONT_COLOR") , 255);
		// 初始化结果未知字体颜色 默认黑色
		m_InitUnknownFontColor = m_GUIIni->GetDataInt(_T("OPERATOR_FONT_CN"),_T("INI_RESULT_UNKNOW_FONT_COLOR") , 0);


		// 乘客界面车站字体颜色
		m_RectStationFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("STATION_FONT_COLOR_CN"),0 );
		m_RectStationFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("STATION_FONT_COLOR_EN"),0 );
		m_RectStationFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("STATION_FONT_COLOR_PN"),0 );

		// 乘客状态栏日期时间字体颜色
		m_RectDateFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("DATE_FONT_COLOR_CN"),0 );
		m_RectDateFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("DATE_FONT_COLOR_EN"),0 );
		m_RectDateFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("DATE_FONT_COLOR_PN"),0 );

		// 乘客基本信息区标题字体大小
		m_RectTitleFontColor[0]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("TITLE_FONT_COLOR_CN"),0 );
		m_RectTitleFontColor[1]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("TITLE_FONT_COLOR_EN"),0 );
		m_RectTitleFontColor[2]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("TITLE_FONT_COLOR_PN"),0 );
		
		// 乘客界面基本信息区域字体颜色
		m_RectBaseInfoFontColor[0]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("BASEINFO_FONT_COLOR_CN"),0 );
		m_RectBaseInfoFontColor[1]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("BASEINFO_FONT_COLOR_EN"),0 );
		m_RectBaseInfoFontColor[2]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("BASEINFO_FONT_COLOR_PN"),0 );

		// 乘客界面 向导信息区字体颜色
		m_RectGuideFontColor[0]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("GUIDEINFO_FONT_COLOR_CN"),0 );
		m_RectGuideFontColor[1]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("GUIDEINFO_FONT_COLOR_EN"),0 );
		m_RectGuideFontColor[2]	= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("GUIDEINFO_FONT_COLOR_PN"),0 );

		// 乘客界面消息框正文字体颜色
		m_RectMsgBoxFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOX_FONT_COLOR_CN"),0 );
		m_RectMsgBoxFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOX_FONT_COLOR_EN"),0 );
		m_RectMsgBoxFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOX_FONT_COLOR_PN"),0 );

		// 乘客界面消息框标题字体颜色
		m_RectMsgBoxTitleFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOXTITLE_FONT_COLOR_CN"),0 );
		m_RectMsgBoxTitleFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOXTITLE_FONT_COLOR_EN"),0 );
		m_RectMsgBoxTitleFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MESSAGEBOXTITLE_FONT_COLOR_PN"),0 );

		// 乘客界面功能按钮字体颜色
		m_FuncBtnFontColor[0]= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_FONT_COLOR_CN"),0 );
		m_FuncBtnFontColor[1]= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_FONT_COLOR_EN"),0 );
		m_FuncBtnFontColor[2]= m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_FONT_COLOR_PN"),0 );

		// 乘客界面菜单按钮字体颜色
		m_MenuBtnFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MENU_BUTTON_FONT_COLOR_CN"),0 );
		m_MenuBtnFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MENU_BUTTON_FONT_COLOR_EN"),0 );
		m_MenuBtnFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("MENU_BUTTON_FONT_COLOR_PN"),0 );

		// 乘客界面功能按钮组标题字体颜色
		m_FuncBtnGroupTitleFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_COLOR_CN"),0 );
		m_FuncBtnGroupTitleFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_COLOR_EN"),0 );
		m_FuncBtnGroupTitleFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("FUNCTION_BUTTON_GROUP_TITLE_FONT_COLOR_PN"),0 );
		// 乘客界面LOGO字体颜色
		m_LogoFontColor[0] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("LOGO_FONT_COLOR_CN"),0 );
		m_LogoFontColor[1] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("LOGO_FONT_COLOR_EN"),0 );
		m_LogoFontColor[2] = m_GUIIni->GetDataInt(_T("CLIENT_FONT"),_T("LOGO_FONT_COLOR_PN"),0 );
		
		// 乘客界面分辨率设置
		m_ReceptionSolutionWidthRatio = (double)m_GUIIni->GetDataInt(_T("RECEPTION_RESOLUTION"), _T("WIDTH"), 1920)/1920;
		m_ReceptionSolutionHeightRatio = (double)m_GUIIni->GetDataInt(_T("RECEPTION_RESOLUTION"), _T("HEIGHT"), 1080)/1080;
		
		// 站员界面分辨率设置
		m_OperatorImageResolutionWidth = (double)m_GUIIni->GetDataInt(_T("OPERATOR_RESOLUTION"), _T("WIDTH"), 640); //640 是澳门TVM维护显示屏的宽度
		m_OperatorImageResolutionHeight = (double) m_GUIIni->GetDataInt(_T("OPERATOR_RESOLUTION"), _T("HEIGHT"),480); // 480 高度

		return 0;
	}
	catch(CSysException&) {
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得操作员界面向导文言字体大小 

@param     无 

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetGuideFontSize()
{
	return m_OperatorGuideFontSize[GetOperatorLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得标题文字字体大小   操作员

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetTitleFontSize()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}
	return m_OperatorTitleFontSize[index];*/
	return m_OperatorTitleFontSize[GetOperatorLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得按钮文字字体大小   操作员

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetBtnFontSize()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_OperatorBtnFontSize[GetOperatorLanguageIndex()/*index*/];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得普通文字字体大小

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetNormalFontSize()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_OperatorNormalFontSize[GetOperatorLanguageIndex()/*index*/];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得提示文字字体名称   操作员

@param     无 

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetGuideFontName()
{
	return m_OperatorGuideFontName[GetOperatorLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得标题文字字体名称  操作员

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetTitleFontName()
{
	//int index = 0;
	//LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	//switch(language_mode)
	//{
	//case LANGUAGE_ID_CHINESE:
	//	index = 0;
	//	break;
	//case LANGUAGE_ID_ENGLISH:
	//	index = 1;
	//	break;
	//case LANGUAGE_ID_PORTUGUESE:
	//	index = 2;
	//	break;
	//}
	return m_OperatorTitleFontName[GetOperatorLanguageIndex()/*index*/];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得按钮文字字体名称    操作员

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetBtnFontName()
{
	//int index = 0;
	//LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	//switch(language_mode)
	//{
	//case LANGUAGE_ID_CHINESE:
	//	index = 0;
	//	break;
	//case LANGUAGE_ID_ENGLISH:
	//	index = 1;
	//	break;
	//case LANGUAGE_ID_PORTUGUESE:
	//	index = 2;
	//	break;
	//}
	return m_OperatorBtnFontName[GetOperatorLanguageIndex()/*index*/];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得普通文字字体名称   操作员

@param     无   

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetNormalFontName()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_OperatorNormalFontName[GetOperatorLanguageIndex()/*index*/];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得普通文字字体名称   

@param     无   

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetLogoFontNameCN()
{
	return m_RecLogoFontName[1];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得普通文字字体名称   

@param     无   

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetLogoFontNameEN()
{
	return m_RecLogoFontName[0];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得版本信息字体大小

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetLogoFontSizeCN( )
{
	return m_RecLogoFontSize[0];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得版本信息字体大小

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetLogoFontSizeEN( )
{
	return m_RecLogoFontSize[0];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得初始化字体大小

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetIniFontSize( )
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_OperatorIniFontSize[GetOperatorLanguageIndex()/*index*/];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得初始化字体名称  操作员

@param     无 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CGUIInfo::GetIniFontName()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_OperatorIniFontName[GetOperatorLanguageIndex()/*index*/];
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得乘客字符集信息

@param      无

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetPassengerFontCodeType()
{
	return m_OperatorCharSet[GetPassengerLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得站员字符集信息

@param      无

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetOperatorFontCodeType()
{
	return m_OperatorCharSet[GetOperatorLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得普通文本字体   操作员

@param      

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LOGFONT& CGUIInfo::GetDefaultFont()
{
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/

	/*m_DefaultFont=GetBtnFont();
	return m_DefaultFont;*/
	
	// 普通文本字体
	TCHAR fontName[LF_FACESIZE]={0};
	//CString strFontName = m_OperatorNormalFontName[GetOperatorLanguageIndex()/*index*/];
	CString strFontName = GetNormalFontName();
	memcpy(fontName,strFontName.GetBuffer(),strFontName.GetLength()>LF_FACESIZE* sizeof(TCHAR)?LF_FACESIZE:strFontName.GetLength()* sizeof(TCHAR));
	strFontName.ReleaseBuffer();
	LOGFONT defFont = {
		GetNormalFontSize(),0,0,0,FW_NORMAL,0,0,0,CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES,
		PROOF_QUALITY,
		DEFAULT_PITCH|FF_SWISS,_T("")
	};
	memcpy(defFont.lfFaceName,fontName,LF_FACESIZE);
	m_DefaultFont = defFont;
	return m_DefaultFont;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得标题字体

@param      

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
// LOGFONT& CGUIInfo::GetTitleFont()
// {	
// 	return m_TitleFont;
// }
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得按钮字体  乘客

@param      

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LOGFONT& CGUIInfo::GetBtnFont()
{	
	/*int index = 0;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentClientLanguageType();
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	// 普通文本字体
	TCHAR fontName[LF_FACESIZE]={0};
	CString strFontName = m_FuncBtnFontName[GetPassengerLanguageIndex()/*index*/];
	memcpy(fontName,strFontName.GetBuffer(),strFontName.GetLength()>LF_FACESIZE* sizeof(TCHAR)?LF_FACESIZE:strFontName.GetLength()* sizeof(TCHAR));
	strFontName.ReleaseBuffer();
	LOGFONT defFont = {
		m_FuncBtnFontSize[GetPassengerLanguageIndex()/*index*/],0,0,0,FW_NORMAL,0,0,0,CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES,
		PROOF_QUALITY,
		DEFAULT_PITCH|FF_SWISS,_T("")
	};
	memcpy(defFont.lfFaceName,fontName,LF_FACESIZE);
	m_BtnFont = defFont;
	return m_BtnFont;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面普通业务向导文言字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetRecStationFontSize(/*LANGUAGETYPE_T language_mode  = LANGUAGE_ID_CHINESE*/ )
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecStationFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面标题字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetRecTitleFontSize(LANGUAGETYPE_T language_mode /*= LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecTitleFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面日期字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD  CGUIInfo::GetRecDateFontSize(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecDateFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面基本信息字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD  CGUIInfo::GetRecBaseInfoFontSize(LANGUAGETYPE_T language_mode /*= LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecBaseInfoFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面普通业务向导文言字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD  CGUIInfo::GetRecGuideFontSize(LANGUAGETYPE_T language_mode/* = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecGuideInfoFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面消息框文言字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD  CGUIInfo::GetRecMessageBoxFontSize(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecMessageBoxFontSize[GetPassengerLanguageIndex()/*index*/];	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面消息框标题文言字体大小

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD  CGUIInfo::GetRecMessageBoxTitleFontSize(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecMessageBoxTitleFontSize[GetPassengerLanguageIndex()/*index*/];	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面车站文言字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo::GetRecStationFontName(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecStationFontName[GetPassengerLanguageIndex()/*index*/];	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面标题字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo::GetRecTitleFontName(LANGUAGETYPE_T language_mode/* = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecTitleFontName[GetPassengerLanguageIndex()/*index*/];	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面日期字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo:: GetRecDateFontName(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecDateFontName[GetPassengerLanguageIndex()/*index*/];	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面基本信息字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo:: GetRecBaseInfoFontName(LANGUAGETYPE_T language_mode /*= LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecBaseInfoFontName[GetPassengerLanguageIndex()/*index*/];	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面普通业务向导文言字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo::GetRecGuideFontName(LANGUAGETYPE_T language_mode /*= LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
		index = 0;
		break;
	case LANGUAGE_ID_ENGLISH:
		index = 1;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		index = 2;
		break;
	}*/
	return m_RecGuideInfoFontName[GetPassengerLanguageIndex()/*index*/];	
}	
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面消息框文言字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo::GetRecMessageBoxFontName(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecMessageBoxFontName[GetPassengerLanguageIndex()/*index*/];	
}		
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得乘客界面消息框标题文言字体

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString  CGUIInfo::GetRecMessageBoxTitleFontName(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/)
{
	/*int index = 0;
	switch(language_mode)
	{
	case LANGUAGE_ID_CHINESE:
	index = 0;
	break;
	case LANGUAGE_ID_ENGLISH:
	index = 1;
	break;
	case LANGUAGE_ID_PORTUGUESE:
	index = 2;
	break;
	}*/
	return m_RecMessageBoxTitleFontName[GetPassengerLanguageIndex()/*index*/];	
}		

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得标题背景色

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetTitleBakColor()
{
	return m_TitleBakColor;	
}			

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得基本区域背景色

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetBaseBakColor()
{
	return m_BaseBakColor;	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得状态区背景色

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetStatusBakColor()
{
	return m_StatusBakColor;	
}			
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得功能区背景色

@param     (i)LANGUAGETYPE_T language_mode   语言类型 

@retval    

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetFunBakColor()
{
	return m_FunBakColor;	
}	
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取乘客画面分辨率宽

@param      none

@retval     int

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
double CGUIInfo::GetReceptionSolutionWidthRatio()
{
	return m_ReceptionSolutionWidthRatio;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取乘客画面分辨率高

@param      none

@retval     int

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
double CGUIInfo::GetReceptionSolutionHeightRatio()
{
	return m_ReceptionSolutionHeightRatio;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取站员画面语言种类索引

@param      none

@retval     LANGUAGE_TYPE_INDEX 语言种类索引

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_TYPE_INDEX CGUIInfo::GetOperatorLanguageIndex()
{
	LANGUAGE_TYPE_INDEX languageIndex = LANGUAGE_TYPE_INDEX_CN;
	LANGUAGETYPE_T language_mode  = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language_mode){
	case LANGUAGE_ID_CHINESE:
		languageIndex = LANGUAGE_TYPE_INDEX_CN;
		break;
	case LANGUAGE_ID_ENGLISH:
		languageIndex = LANGUAGE_TYPE_INDEX_EN;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		languageIndex = LANGUAGE_TYPE_INDEX_PN;
		break;
	}
	return languageIndex;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取乘客画面语言种类索引

@param      none

@retval     LANGUAGE_TYPE_INDEX 语言种类索引

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_TYPE_INDEX CGUIInfo::GetPassengerLanguageIndex()
{
	LANGUAGE_TYPE_INDEX languageIndex = LANGUAGE_TYPE_INDEX_CN;
	LANGUAGETYPE_T language_mode = theAPP_SESSION.GetCurrentClientLanguageType();
	switch(language_mode){
	case LANGUAGE_ID_CHINESE:
		languageIndex = LANGUAGE_TYPE_INDEX_CN;
		break;
	case LANGUAGE_ID_ENGLISH:
		languageIndex = LANGUAGE_TYPE_INDEX_EN;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		languageIndex = LANGUAGE_TYPE_INDEX_PN;
		break;
	}
	return languageIndex;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取乘客界面版本信息文言字体大小

@param      none

@retval     DWORD 乘客界面版本信息文言字体大小

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CGUIInfo::GetVersionFontSize()
{
	return m_RecVersionInfoFontSize;	
}


CString CGUIInfo::GetFunctBtnFontName()
{
	return m_FuncBtnFontName[GetPassengerLanguageIndex()];
}
DWORD CGUIInfo::GetFunctBtnFontSize()
{
	return m_FuncBtnFontSize[GetPassengerLanguageIndex()];
}

CString CGUIInfo::GetMenueBtnFontName()
{
	return m_MenueBtnFontName[GetPassengerLanguageIndex()];
}

DWORD CGUIInfo::GetMenueBtnFontSize()
{
	return m_MenueBtnFontSize[GetPassengerLanguageIndex()];
}
CString CGUIInfo::GetFuncBtnGroupTitleFontName()
{
	return m_FuncBtnGroupTitleFontName[GetPassengerLanguageIndex()];
}

DWORD CGUIInfo::GetFuncBtnGroupTitleFontSize()
{
	return m_FuncBtnGroupTitleFontSize[GetPassengerLanguageIndex()];
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取操作员界面标题字体颜色

@param      none

@retval     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetTitleFontColor()
{
	return m_TitleFontColor[GetOperatorLanguageIndex()];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取乘客界面字体颜色

@param      none

@retval     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COLORREF CGUIInfo::GetBtnFontColor()
{
	return m_BtnFontColor[GetOperatorLanguageIndex()];
}

COLORREF CGUIInfo::GetNormalFontColor()
{
	return m_NormalFontColor[GetOperatorLanguageIndex()];
}

COLORREF CGUIInfo::GetOperatorGuideFontColor()
{
	return m_GuideFontColor[GetOperatorLanguageIndex()];
}

COLORREF CGUIInfo::GetIniFontColor()
{
	return m_InitFontColor[GetOperatorLanguageIndex()];
}

// 状态栏车站字体颜色
COLORREF CGUIInfo::GetRecStationFontColor(/*LANGUAGETYPE_T language_mode*/)
{
	return m_RectStationFontColor[GetPassengerLanguageIndex()];
}

// 状态栏日期字体颜色
COLORREF CGUIInfo::GetRecDateFontColor(/*LANGUAGETYPE_T language_mode*/)
{
	return m_RectDateFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetRecBaseInfoFontColor(LANGUAGETYPE_T language_mode)
{
	return m_RectBaseInfoFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetRecGuideFontColor(LANGUAGETYPE_T language_mode)
{
	return m_RectGuideFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetRecMsgBoxFontColor(/*LANGUAGETYPE_T language_mode*/)
{
	return m_RectMsgBoxFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetRecMsgBoxTitleFontColor(/*LANGUAGETYPE_T language_mode*/)
{
	return m_RectMsgBoxTitleFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetLogoFontColorCN(LANGUAGETYPE_T language_mode)
{
	return m_LogoFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetFuncBtnFontColor()
{
	return m_FuncBtnFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetFuncBtnGroupTitleFontColor()
{
	return m_FuncBtnGroupTitleFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetMenuBtnFontColor()
{
	return m_MenuBtnFontColor[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetInitRsltSucceedFontColor()
{
	return m_InitSucceedFontColor;
}

COLORREF CGUIInfo::GetInitRsltFailedFontColor()
{
	return m_InitFailedFontColor;
}

COLORREF CGUIInfo::GetInitRsltUnknowFontColor()
{
	return m_InitUnknownFontColor;
}

COLORREF CGUIInfo::GetRecTitleFontColor(LANGUAGETYPE_T language_mode /* = LANGUAGE_ID_UNSPECIFIED */)
{
	return m_RectTitleFontColor[GetPassengerLanguageIndex()];
}

//菜单按钮字体
LOGFONT& CGUIInfo::GetRecMenuBtnFont()
{	
	TCHAR fontName[LF_FACESIZE]={0};
	CString strFontName = m_MenueBtnFontName[GetPassengerLanguageIndex()];
	memcpy(fontName,strFontName.GetBuffer(),strFontName.GetLength()>LF_FACESIZE* sizeof(TCHAR)?LF_FACESIZE:strFontName.GetLength()* sizeof(TCHAR));
	strFontName.ReleaseBuffer();
	LOGFONT defFont = {
		m_MenueBtnFontSize[GetPassengerLanguageIndex()],0,0,0,FW_NORMAL,0,0,0,CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES,
		PROOF_QUALITY,
		DEFAULT_PITCH|FF_SWISS,_T("")
	};
	memcpy(defFont.lfFaceName,fontName,LF_FACESIZE);
	m_MenueBtnFont = defFont;
	return m_MenueBtnFont;
}

CString CGUIInfo::GetSvcStateFontName()
{
	return m_RecSvcStateFontName[GetPassengerLanguageIndex()];
}

DWORD CGUIInfo::GetSvcStateFontSize()
{
	return m_RecSvcStaeFontSize[GetPassengerLanguageIndex()];
}

COLORREF CGUIInfo::GetNormalSvcStateFontColor()
{
	return m_RectNormalSvcStateFontColor;
}

COLORREF CGUIInfo::GetUnNormalSvcStateFontColor()
{
	return m_RectUnNormalSvcStateFontColor;
}

COLORREF CGUIInfo::GetGuideAreaColor()
{
	return m_OpGuidAreaColor;
}

COLORREF CGUIInfo::GetBaseInfoAreaColor()
{
	return m_OpBaseAreaColor;
}

COLORREF CGUIInfo::GetStateAreaColor()
{
	return m_OpStatAreaColor;
}

COLORREF CGUIInfo::GetFuncAreaColor()
{
	return m_OpStateFuncAreaColor;
}

double CGUIInfo::GetOperatorImageResolutionWidth()
{
	return m_OperatorImageResolutionWidth;
}

double CGUIInfo::GetOperatorImageResolutionHeight()
{
	return m_OperatorImageResolutionHeight;
}

COLORREF CGUIInfo::GetServiceStateFontColor()
{
	return m_ServiceStateTxtColor[GetPassengerLanguageIndex()];
}

DWORD CGUIInfo::GetServiceStateFontSize()
{
	return m_ServiceStateTxtSize[GetPassengerLanguageIndex()];
}

CString CGUIInfo::GetServiceStateFontName()
{
	return m_ServiceStateTxtName[GetPassengerLanguageIndex()];
}

CString CGUIInfo::GetHistoryGridTxtFontName()
{
	return m_HistoryGridFontName[GetPassengerLanguageIndex()];
}

DWORD CGUIInfo::GetHistoryGridTxtFontSize()
{
	return m_HistoryGridFontSize[GetPassengerLanguageIndex()];
}

DWORD  CGUIInfo::GetHistoryGridTxtFontColor()
{
	return m_HistoryGridFontColor[GetPassengerLanguageIndex()];
}
