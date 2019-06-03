// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <memory>

// 通用
#include "CommonDefine.h"
#include "Util.h"
#include "delegate.h"
#include "CommonFunc.h"
#include "Validation.h"
#include "SntpApi.h"
#include "tvmStatusMgr.h"
#include "businesstranslatesvc.h"
#include "DXFile.h"

#include "GUIInfo.h"
#include "TVMInfo.h"
#include "VersionInfo.h"
#include "AppSession.h"
#include "CursorMgr.h"
#include "MaintenanceInfo.h"
#include "tvmsetting.h"
#include "FunctionInfo.h"
#include "LanguageInfo.h"

#include "serverclientsvc.h"
#include "scapi.h"
#include "MessageBox.h"
#include "MessagePictureBox.h"
#include "SNTPHelper.h"
#include "servicemgr.h"
#include "tvmforeservice.h"

#include "MainSvc.h"
#include "MainFrm.h"
#include "LanguageInfo.h"
#include "ProcessDlg.h"

// 日志
#include "DebugLog.h"
#include "ExceptionLog.h"

#include "CTLOGCountInfo.h"
//#include "CTLOGPresellTicketInfo.h"
#include "CTLOGRegister.h"
#include "CTLOGDeviceInfo.h"
#include "CTLOGSoftVer.h"
#include "CTCoinCountInfo.h"
#include "CTTokenCountInfo.h"
#include "CTCardCountInfo.h"
#include "CTBanknoteCountInfo.h"
#include "Log.h"


// 异常
#include "ExceptionMgr.h"
#include "SysException.h"
#include "SCException.h"
#include "SCAuditException.h"
#include "FtpException.h"
#include "InnerException.h"
#include "RWException.h"
#include "RWHardwareException.h"
#include "LogException.h"
#include "TxnDataException.h"
#include "InnerException.h"
#include "csntpexception.h"
#include "THException.h"
#include "THHardwareException.h"
#include "PrinterException.h"
#include "CHException.h"
#include "CHHardwareException.h"
#include "IOException.h"
#include "StatusDisplayException.h"
#include "CDiskException.h"
// 交易 
#include "TxnDataMgr.h"
#include "TxnDataEnum.h"

// 参数

#include "ParamHelper.h"
#include "cpwdinputerror.h"
#include "cparamexception.h"
#include "ctmessageaudit.h"
#include "ctissuepermit.h"
//!!#include "CNamedCardApplyResult.h"
//!!#include "caccdevicecontrolparam.h"
#include "CAfcModeRecord.h"
//!!#include "CRefundApplyResult.h"
//!!#include "caccsectionparam.h"
#include "CAfcTVMOperationParam.h"

// 审计
#include "DeviceAR.h"
//#include "SCAuditManager.h"
#include "ACCAuditManager.h"
#include "DeviceRegAR.h"
#include "tvmdef.h"


#include "InitDlg.h"
#include "menumgr.h"
#include "ServiceDefinition.h"
#include "ServiceMgr.h"
#include "EventDataSvc.h"
#include "TranscationTranslateSvc.h"

#include "CHAApi.h"
#include "TIMApi.h"
#include "BNAApi.h"
#include "BNDApi.h"
#include "BNCApi.h"
#include "DIOApi.h"