#pragma once
#define PURCHASE_MANAGE_STAFF_ID_LEN		8							// Ա��ID�ĳ���

#define PURCHASE_MANAGE_PERSONAL_FILE_PATH		_T("SCAudit\\Personal\\")		// SC����С��Ŀ¼��
#define PURCHASE_MANAGE_DAY_FILE_PATH		_T("SCAudit\\Day\\")		// SC�ռ�Ŀ¼��

typedef enum _print_kind
{
	NOT_PRINT_DATA = 0,  // δ��ӡ����
	PRINT_DATA = 1,      // �Ѵ�ӡ����
	PRINT_ALL = 2        // ȫ������
}PRINT_KIND;

typedef enum _send_kind
{
	NOT_SEND_DATA = 0,  // δ������λ����
	SEND_DATA = 1,      // �ѷ�����λ����
	ALL_DATA = 2        // ȫ������
}SEND_KIND;

typedef enum _data_kind
{
	CURRENT_DATA = 0,   // ��ǰ����(��ǰ��¼Ա���ļ�������)
	HISTORY_DATA = 1    // ��ʷ����(������ǰ���ݵ���������)
}DATA_KIND;
