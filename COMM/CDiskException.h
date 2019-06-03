#ifndef _DISK_EXCEPTION
#define _DISK_EXCEPTION

#include "SysException.h"
#include "commonexp.h"

class COMMON_API CDiskException : public CSysException
{
public:
    static const int MODULE_ID = 12503;     // ģ���

    typedef enum _detail_error_code {
        UE_DISK_LOWSPACE=101,		//�űP���g��
		UE_DISK_NOSPACE=102			//�űP���g�O��

    } DETAIL_ERROR_CODE;

    CDiskException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CDiskException(void);
};

#endif
