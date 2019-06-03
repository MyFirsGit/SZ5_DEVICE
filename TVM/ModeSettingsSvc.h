#pragma once
#include "tvmforeservice.h"
class CModeSettingsSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CModeSettingsSvc(void);
	~CModeSettingsSvc(void);


	typedef enum _dialog_group
	{
		DLG_GROUP_BASE		= 1,	//选择设定
		DLG_GROUP_SERVICE	= 2,	//业务模式
		DLG_GROUP_PAYMENT	= 3,	//支付模式
		DLG_GROUP_CHANGE	= 4,	//找零模式
		DLG_GROUP_PRINT		= 5		//打印模式
	}DLG_GROUP;

	typedef enum _svc_item
	{
		ISSUE_TOKEN,
		ISSUE_CARD,
		ADD_PRODUCT,
		RECHARGE_CARD,
		QUERY_INFO,
		COIN,
		BANKNOTE,
		BANKCARD,
		EP,
		PRINT,
		COIN_CHANGE,
		BANKNOTE_CHANGE,
	}SVC_ITEM;

	typedef enum _status	//状态定义
	{
		ON = 1,				//开启
		OFF= 2,				//关闭
		UNDEFINE			//未知
	}STATUS;

	typedef struct _selection
	{
		CString	strItem;	//要设置的业务
		STATUS	Statuse;	//当前状态
		DEVICE_CONTRL_CODE code;
		_selection()
		{
			strItem = "";
			Statuse = UNDEFINE;
			code = (DEVICE_CONTRL_CODE)0;
		}

		_selection& setUnit(CString str, STATUS st)
		{
			_selection another;
			another.strItem = str;
			another.Statuse = st;
			return another;
		}
	}SELECTION, *PSELECTION;

	typedef struct _svc_data
	{
		map<int ,SELECTION> mapItem;	//Menu and its state
		_svc_data()
		{
			mapItem.clear();
			SELECTION sel;
			sel.strItem = _opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC);
			sel.Statuse = UNDEFINE;
			mapItem[1] = sel;
			sel.strItem = _opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC);
			sel.Statuse = UNDEFINE;
			mapItem[2] = sel;
			if(DEVICE_TYPE_AVM == theMAINTENANCE_INFO.GetDeviceType())
			{
				sel.strItem = _opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC);
				sel.Statuse = UNDEFINE;
				mapItem[3] = sel;
				return;
			}
			sel.strItem = _opl(TXT_ID_TVM_CHANGE_MODE_SETTING_SVC);
			sel.Statuse = UNDEFINE;
			mapItem[3] = sel;
			sel.strItem = _opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC);
			sel.Statuse = UNDEFINE;
			mapItem[4] = sel;
		}
	}SVC_DATA, *PSVC_DATA;

	SVC_DATA& GetDataModel();
	void OnStart();
	void NotifyDataModelChanged();
	void ClearPage();
private:
	int DeviceType;
	SVC_DATA  m_SvcData;			// 业务数据
	LRESULT OnKeyboardF3(WPARAM, LPARAM );
	LRESULT OnKeyboardEnter(WPARAM, LPARAM);
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);
	//LRESULT OnKeyboardClear(WPARAM, LPARAM);
	void LoadItem(map<int, SELECTION>& mapItem, DLG_GROUP dlg);
	STATUS GetCurSvcStatus(SVC_ITEM svc);//获取当前状态
	void SetSvcWorkMode(int nIndex);
	BOOL SetItemRunMode(/*WORD wCode*/); //设置状态
	map<int, DEVICE_CONTRL_CODE> m_mapDvcCode; //设置项与对应的 DEVICE_CONTRL_CODE
};

