#pragma once
#include "afx.h"
#include "SysException.h"
#include "dataexp.h"
/**
@brief	���ݿ⴦���쳣��
*/

class TVMDB_API CDataBaseException : public CSysException
{
//    DECLARE_DYNAMIC(LogException);
public:
    static const int MODULE_ID = 10109;					// ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR						= 1,	    // �趨ֵ�������쳣
        FUNC_PARAM_ERR						= 2,	    // ���������쳣
        SEQUENCE_ERR						= 3,	    // ��������˳���쳣
        FUNC_ERR_RETURN						= 4,	    // ���������쳣
        LOGIC_ERR							= 5,	    // �߼��쳣
        OTHER_ERR							= 6,	    // �쳣����

        FILE_DELETE_FAIL					= 105,	    // �ļ�ɾ���쳣

		FOLDER_NOT_EXIST					= 401,	    // Ŀ¼������
		FOLDER_CREATE_FAIL 					= 402,	    // �½�Ŀ¼�쳣

		GET_APP_FOLDER_FAIL 				= 500,	    // ��ȡ��ǰӦ�ó����·��ʧ��

		DATABASE_INITIALIZE_ERR				= 600,		// ���ݿ��ʼ������
		DATABASE_NAME_ERR					= 601,		// ��ȡ���ݿ���������
		DATABASE_SCRIPT_ERRR				= 602,		// ��ȡ���ݿ�ű��ļ�������
		DATABASE_MAIN_FOLDER_NOT_EXIST		= 603,		// ��ȡ�����ݿ�Ŀ¼������
		DATABASE_BACKUP_FOLDER_NOT_EXIST	= 604,		// ��ȡ�������ݿ�Ŀ¼������
		DATABASE_MAIN_FOLDER_CREATE_ERRR	= 605,		// �����ݿ�Ŀ¼��������
		DATABASE_BACKUP_FOLDER_CREATE_ERRR	= 606,		// �������ݿ�Ŀ¼��������
		DATABASE_READ_SCRIPT_FALL			= 607,		// ��ȡ���ݿ�ű��ļ�ʧ��
		DATABASE_MAIN_CTEATE_FAIL			= 608,		// ���������ݿ�ʧ��
		DATABASE_BACKUP_CREATE_FAIL			= 609,		// �����������ݿ�ʧ��
		DATABASE_MAIN_OPEN_FAIL				= 610,		// �������ݿ�ʧ��
		DATABASE_BACKUP_OPEN_FAIL			= 611,		// �򿪱������ݿ�ʧ��
		DATABASE_BACKUP_FALL				= 612,		// �������ݿ�ʧ��
		DATABASE_BACKUP_COPY_FALL			= 613,		// �����������ݿ�ʧ��
		DATABASE_SQL_NULL					= 614,		// Ҫִ�е�SQL���Ϊ��
		DATABASE_SQL_EXECUTE_FALL			= 615,		// SQL���ִ��ʧ��	
		DATABASE_OPEN_ERR					= 616,		// �����ݿ�ʧ��
		DATABASE_CLOSE_ERR					= 617,		// �ر����ݿ�ʧ��
		DATABASE_GET_APP_PATH_ERROR			= 618,		// ��ȡӦ�ó���ǰĿ¼�쳣
		

    } DETAIL_ERROR_CODE;

    CDataBaseException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CDataBaseException(void);

private:
};
