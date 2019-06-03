#pragma once
#define PURCHASE_MANAGE_STAFF_ID_LEN		8							// 员工ID的长度

#define PURCHASE_MANAGE_PERSONAL_FILE_PATH		_T("SCAudit\\Personal\\")		// SC个人小计目录名
#define PURCHASE_MANAGE_DAY_FILE_PATH		_T("SCAudit\\Day\\")		// SC日计目录名

typedef enum _print_kind
{
	NOT_PRINT_DATA = 0,  // 未打印数据
	PRINT_DATA = 1,      // 已打印数据
	PRINT_ALL = 2        // 全部数据
}PRINT_KIND;

typedef enum _send_kind
{
	NOT_SEND_DATA = 0,  // 未发送上位数据
	SEND_DATA = 1,      // 已发送上位数据
	ALL_DATA = 2        // 全部数据
}SEND_KIND;

typedef enum _data_kind
{
	CURRENT_DATA = 0,   // 当前数据(当前登录员工的集计数据)
	HISTORY_DATA = 1    // 历史数据(包括当前数据的所有数据)
}DATA_KIND;
