#ifndef _BRIGHTNESS_HELPER
#define _BRIGHTNESS_HELPER

#include "GammaRamp.h"

#define  theScreen_HELPER CScreenHelper::GetInstance()

class CScreenHelper
{
public:
	static CScreenHelper& GetInstance()
	{
		return CScreenHelper::theInstance;
	}
	~CScreenHelper();
	void setBrightness(int ilevel);
	void recoverBrightness();
private:
	CScreenHelper();
private:
	CGammaRamp m_gammaRamp;
	static CScreenHelper theInstance;
};

#endif