#define MAX_DEVICES		2     // ���������豸����

// �豸���Ӽ�ͨ�ſ��ƽṹ
typedef struct device_tbl_tag 
{
	bool						bUsed;			// �豸�����Ƿ��ѱ�ʹ��  A flag indicates the device is being used.
	bool						bClose;			// �豸�����Ƿ��Ѿ��ر�  A flad indicate the service is closed.
	HANDLE						hSync;			// �����߳�����ͬ���¼�
	HANDLE						hThread;		// �����߳̾��
	BHTRS_RESULT				nResult;		// ����ִ�н��
	HANDLE						hSend;			// Handle of an event used to send data
	HANDLE						hRecv;			// Handle of an event used to receive data
	HANDLE						hClose;			// �ر������¼�
	BHCHANGETRS_CONNECT_PARAM	Parm;		// �豸���Ӳ����ṹ
	HTDLWORK*					pWork;			// Pointer to a structure storing active transaction's information. Reffer @ref HTDLWORK.
} DEVICETBL, *PDEVICETBL;