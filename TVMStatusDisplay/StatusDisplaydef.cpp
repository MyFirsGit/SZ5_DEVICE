#include "stdafx.h"
#include "StatusDisplaydef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLedDispInfo::CLedDispInfo()
{
}

CLedDispInfo::~CLedDispInfo()
{
}

byte* CLedDispInfo::getDispCommandBytes()
{
	return (byte*)&m_dispInfo;
}
byte* CLedDispInfo::getDispTextCommandBytes()
{
	return (byte*)&m_dispInfoText;
}
BOOL CLedDispInfo::addScene(TDISPINFO tdisp)
{
	memcpy(&m_dispInfo,&tdisp,sizeof(TDISPINFO));
	return TRUE;
}
BOOL CLedDispInfo::addSceneText(TDISPINFOTEXT tdisptext)
{
	memcpy(&m_dispInfoText,&tdisptext,sizeof(TDISPINFOTEXT));
	return TRUE;

}