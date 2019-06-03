#ifndef __THCOMLIBAPI_H__
#define __THCOMLIBAPI_H__

#pragma once

#include "CommonDefine.h"
#include "THException.h"
#include "THHardwareException.h"


typedef enum _tem_th_rsp_code {
	TH_RSP_OK = 0x0000,    // 操作成功
	TH_RSP_NG = 0x0001,    // 操作失败
};

#endif