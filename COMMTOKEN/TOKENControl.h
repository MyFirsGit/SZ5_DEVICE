#include "Control.h"
#include "TOKEN_def.h"
#include "TOKENMessage.h"



/******************************************************************************/
/**	@brief TOKEN通信控制类
/******************************************************************************/
class TOKEN_API CTokenControl : public CControl
{
public:
	CTokenControl(void);
	virtual ~CTokenControl(void);
	long DoCommand(CTOKENCommands *pRequestItem, long lExecuteMode = SP_EXECMODE_SYNC);
protected:
private:
};

