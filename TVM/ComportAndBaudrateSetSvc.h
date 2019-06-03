#pragma once
#include "TVMForeService.h"

/**
@brief    设备串口号与波特率设定
*/
class CComportAndBaudrateSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CComportAndBaudrateSetSvc();
	~CComportAndBaudrateSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_COMPORT_BAUD_SET = 1,
	} DIALOG_GROUP;

	/*
		菜单定义
	*/
	typedef enum _em_menu_def_{
		MENU_KEY_RW	=1,
		//MENU_KEY_BH,
		MENU_KEY_TH,
		MENU_KEY_CH,
		//MENU_KEY_PRINT,
		MENU_KEY_DISPLAY,
		//MENU_KEY_BHCHANGE,
		MENU_KEY_IO,
	};

	/*
		菜单当前状态
	*/
	typedef enum _tag_menu_status_{
		MENU_STATUS_NORMAL,			// 正常状态
		MENU_STATUS_SELECT,			// 选择状态
		MENU_STATUS_UPDATED,		// 更新状态
	}MENU_STATUS;

	/*
		数据结构定义
	*/
	typedef struct _tag_menu_detail_{
		CString strMenuName;			// 模块名称
		CString strComport;				// 串口号
		CString strBaudRate;			// 波特率

		MENU_STATUS	emComportStatus;	// 串口号菜单状态
		MENU_STATUS emBaudRateStatus;	// 波特率菜单状态

		_tag_menu_detail_(){
			strMenuName = _T("");
			strComport  = _T("");
			strBaudRate = _T("");

			emComportStatus = MENU_STATUS_NORMAL;
			emBaudRateStatus= MENU_STATUS_NORMAL;
		}

	}Menu_Detail;


	/*
		数据模型
	*/
	typedef struct _tag_data_model_{
		Menu_Detail stMenuDetail[7];	// 当前设备共有7个模块
		int iCurSelectMenu;				// 当前选中菜单
		int iCurSelSonMenu;				// 当前选中子菜单
		bool bAlreadySaved;				// 是否已经保存

		// Init
		_tag_data_model_(){
			for(int i = 0;i < 7;i++){
				stMenuDetail[i] = _tag_menu_detail_();
			}

			iCurSelectMenu = 0;
			iCurSelSonMenu = 0;
			bAlreadySaved = true;
		}
	}Model;

	/*
		获取数据模型
	*/
	inline Model& GetDataModel(){
		return m_data_model;
	}

	virtual void	NotifyDataModelChanged();
protected:
	void OnStart();								// Service启动后处理
private:
	Model	m_data_model;						// 数据模型定义
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）

	void ReadFromSettingIni();													// 从配置文件中获取串口配置信息
};