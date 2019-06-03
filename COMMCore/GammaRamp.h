#ifndef GAMMARAMP_H_
#define GAMMARAMP_H_
#include "CoreExp.h"

/*
CGammaRamp class

Encapsulates the Gamma Ramp API and changes the brightness of 
the entire screen.

Written by Nir Sofer.
http://www.nirsoft.net


*/

class CORE_API CGammaRamp
{
protected:
	HMODULE hGDI32;
	HDC hScreenDC;
	typedef BOOL (WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);

	Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
	Type_SetDeviceGammaRamp pSetDeviceGammaRamp;

	BOOL LoadLibrary();
	void FreeLibrary();
	BOOL LoadLibraryIfNeeded();
	BOOL SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
	BOOL GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
public:

	CGammaRamp();
	~CGammaRamp();
	BOOL SetBrightness(HDC hDC, WORD wBrightness);

};



#endif