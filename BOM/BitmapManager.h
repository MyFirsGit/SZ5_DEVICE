#pragma once

#define  theBitmap_MGR CBitmapManager::GetInstance()
class CBitmapManager
{
public:
	void Initialize();
	CBitmapManager();
	~CBitmapManager();
	static CBitmapManager& GetInstance()
	{
		return instance;
	};

	CBitmap* m_bgGroup;
	//CBitmap* m_bgArea2;

	CBitmap* m_bgMenu;

	CBitmap* m_bgNormalOn;
	CBitmap* m_bgNormalDown;
	CBitmap* m_bgNormalSelected;
	CBitmap* m_bgOKOn;
	CBitmap* m_bgOKDown;
	CBitmap* m_bgOKSelected;
	CBitmap* m_bgBackOn;
	CBitmap* m_bgBackDown;
	CBitmap* m_bgBackSelected;
	CBitmap* m_bgFocus;

private:
	bool initialized;
	static  CBitmapManager instance;
};