
typedef enum _rqcode_error_code
{
	QRCODE_OK = 0,						// ִ�гɹ�
	QRCODE_FILENAME_ERROR = 101,		// �ļ�������
	QRCODE_CREATE_FILE_ERROR = 102,		// �����ļ�ʧ��
	QRCODE_BUFFER_LENGTH_LESS,			// �������ݵĻ���������̫С
	QRCODE_VERSION_ERROR,				// �汾������Χ
	QRCODE_OUT_OF_IMAGE,				// ���뷽ʽ���ҳ�����ͼ��
	QRCODE_INVALID_NUMBER,				// ��Ч�����ݼ��ģʽ
	QRCODE_VERSION_RECOVERY,			// �汾��Ϣ����ʧ��
	QRCODE_FORMATE_RECOVERY,			// ��ʽ��Ϣ����ʧ��
	QRCODE_OUT_OF_SAMPLING,				// ����ͼ���������
	QRCODE_NOT_SUPPORT_CHARACTERSET,	// ��֧�ֵ��ַ���
									
};