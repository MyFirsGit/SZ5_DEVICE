#include "StdAfx.h"
#include "QrCodeCodecAPI.h"
#include "QR_Encode.h"
#include "QRcodeImage.h"
#include "ContentDecoder.h"
#include "QRErrorCode.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief		ͼƬ����

@param      (i)LPCSTR strFileName �ļ���
@param      (o)int &nLevel ������
@param      (o)int &nVersion�汾
@param      (o)int &nCodeSize ͼƬ��С
@param      (o)char * &szContentͼƬ����
@param      (o)int lengthͼƬ���ݳ���

@retval     int 0:����ɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int DecodeQrCodeBmp(LPCSTR strFileName,int &nLevel,int &nVersion,int &nCodeSize,char *szContent,int length)
{
	if(strFileName == NULL)
	{
		return QRCODE_FILENAME_ERROR;
	}

	QRcodeImage QR_image(strFileName);
	ContentDecoder QR_Decode;

	// ��ȡbmpͼƬ��Ϣ
	int nGetData = QR_image.GetImageData();
	if (nGetData != QRCODE_OK)
	{
		return nGetData;
	}

	// ��ȡ�汾��
	nVersion = QR_image.m_finderPattern.m_version;
	nCodeSize = nVersion * 4 +17;
	// ͼƬ����
	BYTE codeData[MAX_MODULESIZE][MAX_MODULESIZE]; // �洢������λ����
	for(int i=0;i<nCodeSize;i++)
	{
		for(int j=0;j<nCodeSize;j++)
		{
			codeData[i][j]=QR_image.bitMatrix[i][j];
		}
	}
	// ����
	int nDecode =  QR_Decode.DecodeData(nCodeSize,nVersion,codeData);	
	 if (nDecode != QRCODE_OK)
	 {
		 return nDecode;
	 }

	// ������
	nLevel = QR_Decode.m_nLevel;

	// ͼƬ�����ı�	
	memcpy(szContent,QR_Decode.m_strData.c_str(),length);
	return QRCODE_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		���ݱ���

@param      (i)int nLevel ������
@param      (i)int nVersion�汾
@param      (i)int nMaskingNo ��ģ
@param      (i)BOOL nSizeLevel ͼƬ�Ŵ���
@param      (i)CString lpsSource ��������
@param      (i)CString strFileName�ļ���(����·���Լ��ļ���׺��.bmp)

@retval     int 0:����ɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
QRCODE_API int CreateQrCodeBmp(int nLevel, int nVersion, int nMaskingNo,int nSizeLevel, LPCSTR strSource,LPCSTR strFileName)
{
	int nSymbleSize = 0;
	HDC pSymbleDC;
	HBITMAP pSymbleBitmap;
	HBITMAP pOldBitmap;

	CQR_Encode pQR_Encode;
	int bDataEncoded = pQR_Encode.EncodeData(nLevel,nVersion, TRUE, nMaskingNo, strSource);
	if (bDataEncoded != QRCODE_OK)
	{
		return QRCODE_CREATE_FILE_ERROR;
	}

	// ����ͼƬ�ߴ�
	nSymbleSize = pQR_Encode.m_nSymbleSize + (QR_MARGIN * 2);

	// ����һ�������ض���ȡ��߶Ⱥ���ɫ��ʽ��λͼ
	pSymbleBitmap = CreateBitmap(nSymbleSize, nSymbleSize, 1, 1, NULL);

	pSymbleDC = CreateCompatibleDC(NULL);

	// ѡ��һ����ָ�����豸�����Ļ����У����¶����滻��ǰ����ͬ���͵Ķ���	
	pOldBitmap = (HBITMAP)SelectObject(pSymbleDC,pSymbleBitmap);

	// ʹ�õ�ǰѡ��ָ���豸�����е�ˢ�ӻ��Ƹ����ľ�������ͨ��ʹ�ø����Ĺ�դ�������Ը�ˢ�ӵ���ɫ�ͱ�����ɫ������ϡ�
	PatBlt(pSymbleDC,0, 0, nSymbleSize, nSymbleSize, WHITENESS);

	// ָ�����괦��������Ϊָ������ɫ��
	for (int i = 0; i < pQR_Encode.m_nSymbleSize; ++i)
	{
		for (int j = 0; j < pQR_Encode.m_nSymbleSize; ++j)
		{
			if (pQR_Encode.m_byModuleData[i][j])
			{	
				SetPixel(pSymbleDC,i + QR_MARGIN, j + QR_MARGIN, RGB(0, 0, 0));
			}
		}
	}	

	// ����ͼƬ�ߴ�
	int nPixelSize = nSymbleSize * nSizeLevel;

	// ͼƬ��С
	int nBmpSize = ((nPixelSize + 31) / 32) * 32 / 8;
	nBmpSize *= nPixelSize;

	// �����ļ�
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{			
		return 2;
	}

	// ����һ���µ��ļ�ӳ���ں˶���
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_COMMIT, 0,sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize, NULL);

	// ��һ���ļ�ӳ�����ӳ�䵽��ǰӦ�ó���ĵ�ַ�ռ�
	LPBYTE lpbyMapView = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

	// ��0�����һ���ڴ�����
	ZeroMemory(lpbyMapView, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize);

	// λͼ�ļ�ͷ�ṹ��
	LPBITMAPFILEHEADER pbfh = (LPBITMAPFILEHEADER)lpbyMapView;

	// λͼ��Ϣ��ṹ��
	LPBITMAPINFO       pbmi = (LPBITMAPINFO)(lpbyMapView + sizeof(BITMAPFILEHEADER));

	// λͼ����ʵ��
	LPVOID             pbdt = lpbyMapView + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// λͼ�ļ�ͷ
	pbfh->bfType      = (WORD) (('M' << 8) | 'B'); // "BM"
	pbfh->bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize;
	pbfh->bfReserved1 = 0;
	pbfh->bfReserved2 = 0;
	pbfh->bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// λͼ��Ϣ��
	pbmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth			= nPixelSize;
	pbmi->bmiHeader.biHeight		= nPixelSize;
	pbmi->bmiHeader.biPlanes		= 1;
	pbmi->bmiHeader.biBitCount		= 1;
	pbmi->bmiHeader.biCompression	= BI_RGB;
	pbmi->bmiHeader.biSizeImage		= nBmpSize;
	pbmi->bmiHeader.biXPelsPerMeter = 3780;
	pbmi->bmiHeader.biYPelsPerMeter = 3780;
	pbmi->bmiHeader.biClrUsed		= 0;
	pbmi->bmiHeader.biClrImportant	= 0;

	HDC pWorkDC;
	pWorkDC = CreateCompatibleDC(NULL);

	HBITMAP pWorkBitmap = CreateBitmap(nPixelSize, nPixelSize, 1, 1, NULL);

	pOldBitmap = (HBITMAP)SelectObject(pWorkDC,pWorkBitmap);

	// ������Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��ĿǰĿ���豸���õ�ģʽ����ͼ��������ѹ��
	StretchBlt(pWorkDC,0, 0, nPixelSize, nPixelSize, pSymbleDC, 0, 0, nSymbleSize, nSymbleSize, SRCCOPY);
	GetDIBits(pWorkDC, (HBITMAP)pWorkBitmap, 0, nPixelSize, pbdt, pbmi, DIB_RGB_COLORS);
	SelectObject(pWorkDC,pOldBitmap);
	UnmapViewOfFile(lpbyMapView);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	return QRCODE_OK;
}
