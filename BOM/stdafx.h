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
#include "BomStatusMgr.h"
#include "businesstranslatesvc.h"

#include "BomInfo.h"
#include "VersionInfo.h"
#include "AppSession.h"
#include "CursorMgr.h"
#include "MaintenanceInfo.h"
#include "Setting.h"

#include "serverclientsvc.h"
#include "scapi.h"
#include "MessageBox.h"
#include "SNTPHelper.h"
#include "servicemgr.h"
#include "bomforeservice.h"

#include "MainSvc.h"
#include "MainFrm.h"

// 日志
#include "DebugLog.h"
#include "ExceptionLog.h"

#include "CTLOGCountInfo.h"
#include "CTLOGPresellTicketInfo.h"
#include "CTLOGRegister.h"
#include "CTLOGDeviceInfo.h"
#include "CTLOGSoftVer.h"
#include "Log.h"

// 异常
#include "ExceptionMgr.h"
#include "SysException.h"
#include "SCException.h"
#include "FtpException.h"
#include "InnerException.h"
#include "RWException.h"
#include "RWHardwareException.h"
#include "PrinterException.h"
#include "LogException.h"
#include "TxnDataException.h"
#include "InnerException.h"
#include "csntpexception.h"
#include "TWException.h"
#include "TWHardwareException.h"
#include "THException.h"
#include "THHardwareException.h"
// 交易 
#include "TxnDataMgr.h"
#include "TxnDataEnum.h"

// 参数
#include "CAfcDeviceCommonParam.h"
#include "cacclineinfoparam.h"
#include "ParamHelper.h"
#include "cpwdinputerror.h"
#include "cparamexception.h"
#include "ctmessageaudit.h"
#include "ctissuepermit.h"
#include "CNamedCardApplyResult.h"
#include "caccdevicecontrolparam.h"
#include "CAfcModeRecord.h"
#include "CRefundApplyResult.h"
#include "caccsectionparam.h"
#include "CAccIssueCompanyParam.h"

// 审计
#include "BomAR.h"
#include "SCAuditManager.h"
#include "ACCAuditManager.h"
#include "bomdef.h"


#include "InitDlg.h"
#include "menumgr.h"
#include "ServiceDefinition.h"
#include "ServiceMgr.h"
#include "EventDataSvc.h"
#include "CTTVMRefund.h"
#include "TranscationTranslateSvc.h"

