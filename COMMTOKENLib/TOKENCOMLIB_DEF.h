#ifndef __THCOMLIBAPI_H__
#define __THCOMLIBAPI_H__

#pragma once

#include "CommonDefine.h"
#include "TOKENException.h"
#include "TOKENHardwareException.h"


typedef enum _tem_token_rsp_code {
	TOKEN_RSP_OK = 0x0000,    // �����ɹ�
	TOKEN_RSP_NG = 0x0001,    // ����ʧ��
};

#endif