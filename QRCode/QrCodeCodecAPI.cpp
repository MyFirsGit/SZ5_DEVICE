#include "StdAfx.h"
#include "QrCodeCodecAPI.h"
#include "QR_Encode.h"
#include "QRcodeImage.h"
#include "ContentDecoder.h"
#include "QRErrorCode.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief		图片解码

@param      (i)LPCSTR strFileName 文件名
@param      (o)int &nLevel 纠错级别
@param      (o)int &nVersion版本
@param      (o)int &nCodeSize 图片大小
@param      (o)char * &szContent图片内容
@param      (o)int length图片内容长度

@retval     int 0:解码成功，非0:失败

@exception  无
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

	// 获取bmp图片信息
	int nGetData = QR_image.GetImageData();
	if (nGetData != QRCODE_OK)
	{
		return nGetData;
	}

	// 获取版本号
	nVersion = QR_image.m_finderPattern.m_version;
	nCodeSize = nVersion * 4 +17;
	// 图片内容
	BYTE codeData[MAX_MODULESIZE][MAX_MODULESIZE]; // 存储条形码位数据
	for(int i=0;i<nCodeSize;i++)
	{
		for(int j=0;j<nCodeSize;j++)
		{
			codeData[i][j]=QR_image.bitMatrix[i][j];
		}
	}
	// 解码
	int nDecode =  QR_Decode.DecodeData(nCodeSize,nVersion,codeData);	
	 if (nDecode != QRCODE_OK)
	 {
		 return nDecode;
	 }

	// 纠错级别
	nLevel = QR_Decode.m_nLevel;

	// 图片内容文本	
	memcpy(szContent,QR_Decode.m_strData.c_str(),length);
	return QRCODE_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		数据编码

@param      (i)int nLevel 纠错级别
@param      (i)int nVersion版本
@param      (i)int nMaskingNo 掩模
@param      (i)BOOL nSizeLevel 图片放大倍数
@param      (i)CString lpsSource 数据内容
@param      (i)CString strFileName文件名(包括路径以及文件后缀名.bmp)

@retval     int 0:编码成功，非0:失败

@exception  无
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

	// 生成图片尺寸
	nSymbleSize = pQR_Encode.m_nSymbleSize + (QR_MARGIN * 2);

	// 创建一个带有特定宽度、高度和颜色格式的位图
	pSymbleBitmap = CreateBitmap(nSymbleSize, nSymbleSize, 1, 1, NULL);

	pSymbleDC = CreateCompatibleDC(NULL);

	// 选择一对象到指定的设备上下文环境中，该新对象替换先前的相同类型的对象	
	pOldBitmap = (HBITMAP)SelectObject(pSymbleDC,pSymbleBitmap);

	// 使用当前选入指定设备环境中的刷子绘制给定的矩形区域。通过使用给出的光栅操作来对该刷子的颜色和表面颜色进行组合。
	PatBlt(pSymbleDC,0, 0, nSymbleSize, nSymbleSize, WHITENESS);

	// 指定坐标处的像素设为指定的颜色。
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

	// 生成图片尺寸
	int nPixelSize = nSymbleSize * nSizeLevel;

	// 图片大小
	int nBmpSize = ((nPixelSize + 31) / 32) * 32 / 8;
	nBmpSize *= nPixelSize;

	// 创建文件
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{			
		return 2;
	}

	// 创建一个新的文件映射内核对象
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_COMMIT, 0,sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize, NULL);

	// 将一个文件映射对象映射到当前应用程序的地址空间
	LPBYTE lpbyMapView = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

	// 用0来填充一块内存区域
	ZeroMemory(lpbyMapView, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize);

	// 位图文件头结构体
	LPBITMAPFILEHEADER pbfh = (LPBITMAPFILEHEADER)lpbyMapView;

	// 位图信息表结构体
	LPBITMAPINFO       pbmi = (LPBITMAPINFO)(lpbyMapView + sizeof(BITMAPFILEHEADER));

	// 位图数据实体
	LPVOID             pbdt = lpbyMapView + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// 位图文件头
	pbfh->bfType      = (WORD) (('M' << 8) | 'B'); // "BM"
	pbfh->bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize;
	pbfh->bfReserved1 = 0;
	pbfh->bfReserved2 = 0;
	pbfh->bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// 位图信息表
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

	// 函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩
	StretchBlt(pWorkDC,0, 0, nPixelSize, nPixelSize, pSymbleDC, 0, 0, nSymbleSize, nSymbleSize, SRCCOPY);
	GetDIBits(pWorkDC, (HBITMAP)pWorkBitmap, 0, nPixelSize, pbdt, pbmi, DIB_RGB_COLORS);
	SelectObject(pWorkDC,pOldBitmap);
	UnmapViewOfFile(lpbyMapView);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	return QRCODE_OK;
}
