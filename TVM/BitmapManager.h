#pragma once
#include "CGuiParm.h"

// 全局唯一访问接口
#define  theBitmap_MGR CBitmapManager::GetInstance()

/**
@brief 图片管理类
*/
class CBitmapManager
{
public:
	~CBitmapManager();

	void Initialize();

	static CBitmapManager& GetInstance()
	{
		return instance;
	};

	CImage m_pGroup;
	CImage m_pMenu;

	CImage m_pNormalNormal;
	CImage m_pNormalPressed;
	CImage m_pNormalHover;
	CImage m_pNormalDisabled;

	// 临时解决办法：用于不现实背景图
	CImage m_pLineAreaNormal;
	CImage m_pLineAreaHover;
	CImage m_pLineAreaPressed;
	CImage m_pLineAreaDisabled;

	CImage m_pOkNormal;
	CImage m_pOkPressed;
	CImage m_pOkHover;
	CImage m_pOkDisabled;

	CImage m_pBackNormal;
	CImage m_pBackPressed;
	CImage m_pBackHover;
	CImage m_pBackDisabled;

	CImage m_pStationNormal;
	CImage m_pStationPressed;
	CImage m_pStationHover;
	CImage m_pStationDisabled;	

	CImage m_pGroupNormal;
	CImage m_pGroupHover;
	CImage m_pGroupPressed;
	CImage m_pGroupDisabled;

	CImage m_pServiceNormal;
	CImage m_pServicePressed;
	CImage m_pServiceHover;
	CImage m_pServiceDisabled;

	CImage m_pLanguageNormal;
	CImage m_pLanguageHover;
	CImage m_pLanguagePressed;
	CImage m_pLanguageDisabled;

	// 以下是针对武汉界面，新增加
	CImage m_pGroupBlueNormal;
	CImage m_pGroupBluePressed;
	CImage m_pGroupBlueHover;
	CImage m_pGroupBlueDisabled;

	CImage m_pGroupBrownNormal;
	CImage m_pGroupBrownHover;
	CImage m_pGroupBrownPressed;
	CImage m_pGroupBrownDisabled;

	CImage m_pGroupBtnOrange;
	CImage m_pGroupBtnYellow;
	CImage m_pGroupBtnBlue;
	CImage m_pGroupBtnHot;

	// 20180309，热点车站新处理（各热点车站图标中包含站点名称，需要每个站点单独处理）
	map<CString,CImage*> map_HotStation;

	CGuiParm m_guiparam;
private:
	void LoadHotStationImg();
	static CBitmapManager instance;
	CBitmapManager();
};