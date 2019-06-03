#include "FtpExp.h"
#include "Control.h"
#pragma once



class FTP_API CFTPControl :
	public CControl
{
public:
	CFTPControl(void);
	~CFTPControl(void);
	long Initialize();
	long Shutdown(void);
};
