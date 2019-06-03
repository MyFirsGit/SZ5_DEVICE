#include "stdafx.h"
#include "BitmapManager.h"
#include "Resource.h"

CBitmapManager CBitmapManager::instance;

void CBitmapManager::Initialize()
{
	if(!initialized){
	
		m_bgGroup = new CBitmap;
		m_bgGroup->LoadBitmap(IDB_BG_GROUP);
		
		m_bgMenu = new CBitmap;
		m_bgMenu->LoadBitmap(IDB_BG_MENU);
		
		m_bgNormalOn = new CBitmap;
		m_bgNormalOn->LoadBitmap(IDB_BG_BUTTON_NORMAL_ON);
		
		m_bgNormalDown = new CBitmap;
		m_bgNormalDown->LoadBitmap(IDB_BG_BUTTON_NORMAL_DOWN);

		m_bgNormalSelected = new CBitmap;
		m_bgNormalSelected->LoadBitmap(IDB_BG_BUTTON_NORMAL_SEL);

		m_bgOKOn = new CBitmap;
		m_bgOKOn->LoadBitmap(IDB_BG_BUTTON_OK_ON);

		m_bgOKDown = new CBitmap;
		m_bgOKDown->LoadBitmap(IDB_BG_BUTTON_OK_DOWN);

		m_bgOKSelected = new CBitmap;
		m_bgOKSelected->LoadBitmap(IDB_BG_BUTTON_OK_ON);

		m_bgBackOn = new CBitmap;
		m_bgBackOn->LoadBitmap(IDB_BG_BUTTON_BACK_ON);

		m_bgBackDown = new CBitmap;
		m_bgBackDown->LoadBitmap(IDB_BG_BUTTON_BACK_DOWN);

		m_bgBackSelected = new CBitmap;
		m_bgBackSelected->LoadBitmap(IDB_BG_BUTTON_BACK_ON);

		m_bgFocus = new CBitmap;
		m_bgFocus->LoadBitmap(IDB_BG_FOCUS);

	}
}

CBitmapManager::CBitmapManager()
{
	initialized = false;
	m_bgGroup = NULL;
	m_bgMenu	 = NULL;
	m_bgOKOn	 = NULL;
	m_bgOKDown	 = NULL;
	m_bgOKSelected	 = NULL;
	m_bgBackOn	 = NULL;
	m_bgBackDown	 = NULL;
	m_bgBackSelected= NULL;
	m_bgNormalOn	 = NULL;
	m_bgNormalDown	 = NULL;
	m_bgNormalSelected= NULL;
}

CBitmapManager::~CBitmapManager()
{
	m_bgGroup->DeleteObject();
	m_bgMenu->DeleteObject();
	m_bgOKOn->DeleteObject();
	m_bgOKDown->DeleteObject();
	m_bgOKSelected->DeleteObject();
	m_bgBackOn->DeleteObject();
	m_bgBackDown->DeleteObject();
	m_bgBackSelected->DeleteObject();
	m_bgNormalOn->DeleteObject();
	m_bgNormalDown->DeleteObject();
	m_bgNormalSelected->DeleteObject();
	m_bgFocus->DeleteObject();

	delete m_bgGroup;
	delete m_bgMenu;
	delete m_bgOKOn;
	delete m_bgOKDown;
	delete m_bgOKSelected;
	delete m_bgBackOn;
	delete m_bgBackDown;
	delete m_bgBackSelected;
	delete m_bgNormalOn;
	delete m_bgNormalDown;
	delete m_bgNormalSelected;
	delete m_bgFocus;
}