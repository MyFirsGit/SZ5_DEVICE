#include "stdafx.h"
#include "ScreenHelper.h"

#ifdef DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

static UINT g_uiBrightnessLevel=128;

CScreenHelper CScreenHelper::theInstance;

CScreenHelper::CScreenHelper()
{
}

CScreenHelper::~CScreenHelper()
{

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      亮度 值爲 1-10		默認爲5

@param      (i)int ilevel		亮度等級

@retval     0

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CScreenHelper::setBrightness(int ilevel)
{
	if(ilevel<=10 && ilevel>=1)
	{
		switch(ilevel)
		{
		case 10:
			g_uiBrightnessLevel=255;
			break;
		case 1:
			g_uiBrightnessLevel=0;
			break;
		default:
			g_uiBrightnessLevel=ilevel*12;
			break;
		}
		m_gammaRamp.SetBrightness(NULL,g_uiBrightnessLevel);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      亮度 值爲 1-10		默認爲5

@param      (i)int ilevel		亮度等級

@retval     0

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CScreenHelper::recoverBrightness()
{
	g_uiBrightnessLevel=128;
	m_gammaRamp.SetBrightness(NULL,g_uiBrightnessLevel);
}