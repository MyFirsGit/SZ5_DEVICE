#pragma once
#include "afx.h"
#include "SysException.h"
#include "dataexp.h"
/**
@brief	数据库处理异常类
*/

class TVMDB_API CDataBaseException : public CSysException
{
//    DECLARE_DYNAMIC(LogException);
public:
    static const int MODULE_ID = 10109;					// 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR						= 1,	    // 设定值：参数异常
        FUNC_PARAM_ERR						= 2,	    // 函数参数异常
        SEQUENCE_ERR						= 3,	    // 函数调用顺序异常
        FUNC_ERR_RETURN						= 4,	    // 函数返回异常
        LOGIC_ERR							= 5,	    // 逻辑异常
        OTHER_ERR							= 6,	    // 异常发生

        FILE_DELETE_FAIL					= 105,	    // 文件删除异常

		FOLDER_NOT_EXIST					= 401,	    // 目录不存在
		FOLDER_CREATE_FAIL 					= 402,	    // 新建目录异常

		GET_APP_FOLDER_FAIL 				= 500,	    // 获取当前应用程序的路径失败

		DATABASE_INITIALIZE_ERR				= 600,		// 数据库初始化错误
		DATABASE_NAME_ERR					= 601,		// 获取数据库名称有误
		DATABASE_SCRIPT_ERRR				= 602,		// 获取数据库脚本文件名有误
		DATABASE_MAIN_FOLDER_NOT_EXIST		= 603,		// 获取主数据库目录不存在
		DATABASE_BACKUP_FOLDER_NOT_EXIST	= 604,		// 获取备份数据库目录不存在
		DATABASE_MAIN_FOLDER_CREATE_ERRR	= 605,		// 主数据库目录创建有误
		DATABASE_BACKUP_FOLDER_CREATE_ERRR	= 606,		// 备份数据库目录创建有误
		DATABASE_READ_SCRIPT_FALL			= 607,		// 读取数据库脚本文件失败
		DATABASE_MAIN_CTEATE_FAIL			= 608,		// 创建主数据库失败
		DATABASE_BACKUP_CREATE_FAIL			= 609,		// 创建备份数据库失败
		DATABASE_MAIN_OPEN_FAIL				= 610,		// 打开主数据库失败
		DATABASE_BACKUP_OPEN_FAIL			= 611,		// 打开备份数据库失败
		DATABASE_BACKUP_FALL				= 612,		// 备份数据库失败
		DATABASE_BACKUP_COPY_FALL			= 613,		// 拷贝备份数据库失败
		DATABASE_SQL_NULL					= 614,		// 要执行的SQL语句为空
		DATABASE_SQL_EXECUTE_FALL			= 615,		// SQL语句执行失败	
		DATABASE_OPEN_ERR					= 616,		// 打开数据库失败
		DATABASE_CLOSE_ERR					= 617,		// 关闭数据库失败
		DATABASE_GET_APP_PATH_ERROR			= 618,		// 获取应用程序当前目录异常
		

    } DETAIL_ERROR_CODE;

    CDataBaseException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CDataBaseException(void);

private:
};
