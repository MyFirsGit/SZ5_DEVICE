// TVMBRH.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "TVMBRH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CTVMBRHApp

BEGIN_MESSAGE_MAP(CTVMBRHApp, CWinApp)
END_MESSAGE_MAP()


// CTVMBRHApp ����

CTVMBRHApp::CTVMBRHApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTVMBRHApp ����

CTVMBRHApp theApp;


// CTVMBRHApp ��ʼ��

BOOL CTVMBRHApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
