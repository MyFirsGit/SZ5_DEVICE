#include "stdafx.h"
#include "BitmapManager.h"
#include "Resource.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

CBitmapManager CBitmapManager::instance;


static void transparentPng(const TCHAR* tcpath,CImage* pImage)
{
	FILE *pfile = NULL;
	errno_t err;
	err = _tfopen_s(&pfile,tcpath, _T("r"));
	if(err==0)
	{
		byte PNGMARK[8] = { 0x89, 0x50, 0x4E, 0x47};
		byte pngHead[8];
		size_t readCount = fread(pngHead, sizeof(byte), 4, pfile);
		fclose(pfile);
		pfile = NULL;
		if (readCount == 4)
		{
			if (memcmp(pngHead, PNGMARK, 4)==0)
			{
				for (int i = 0; i < pImage->GetWidth(); i++)
				{
					for (int j = 0; j < pImage->GetHeight(); j++)
					{
						unsigned char* pucColor = reinterpret_cast<unsigned char *>(pImage->GetPixelAddress(i, j));
						pucColor[0] = pucColor[0] * pucColor[3] / 255;
						pucColor[1] = pucColor[1] * pucColor[3] / 255;
						pucColor[2] = pucColor[2] * pucColor[3] / 255;
					}
				}
			}
		}
	}
}

static void loadPic(const TCHAR* tcPath,CImage& image)
{
	HRESULT hr=image.Load(tcPath);
	//if(SUCCEEDED(hr))
	//{
	//	transparentPng(tcPath,&image);
	//}
}

void CBitmapManager::Initialize()
{
	CString bmppath;
	CString apppath;
	GetAppPath(apppath);

	bmppath=apppath+_T(".\\res\\TVM_btn_Print_Confirm.bmp");
	loadPic(bmppath,m_pGroup);
	bmppath=apppath+_T(".\\res\\TVM_btn_Cancel_Default.bmp");
	loadPic(bmppath,m_pMenu);
		
	// 普通按钮
	bmppath = apppath + TVM_IMAGE_BTN_NORMAL;
	//m_pNormalNormal.Load(bmppath );
	loadPic(bmppath,m_pNormalNormal);
	bmppath = apppath + TVM_IMAGE_BTN_DOWN;
	//m_pNormalPressed.Load(bmppath );
	loadPic(bmppath,m_pNormalPressed);
	bmppath = apppath + TVM_IMAGE_BTN_DOWN;
	//m_pNormalHover.Load(bmppath);
	loadPic(bmppath,m_pNormalHover);
	bmppath = apppath + TVM_IMAGE_BTN_DOWN;
	//m_pNormalDisabled.Load(bmppath);
	loadPic(bmppath,m_pNormalDisabled);
		
	// OK按钮
	bmppath = apppath + TVM_IMAGE_OK_NORMAL;
	//m_pOkNormal.Load(bmppath );
	loadPic(bmppath,m_pOkNormal);
	bmppath = apppath + TVM_IMAGE_OK_DOWN;
	//m_pOkPressed.Load(bmppath );
	loadPic(bmppath,m_pOkPressed);
	bmppath = apppath + TVM_IMAGE_OK_NORMAL;
	//m_pOkHover.Load(bmppath );
	loadPic(bmppath,m_pOkHover);
	bmppath = apppath + TVM_IMAGE_OK_NORMAL;
	//m_pOkDisabled.Load(bmppath );
	loadPic(bmppath,m_pOkDisabled);

	// 车站按钮
	bmppath = apppath + TVM_IMAGE_STATION_BTN_NORMAL;
	//m_pStationNormal.Load(bmppath );
	loadPic(bmppath,m_pStationNormal);
	bmppath = apppath + TVM_IMAGE_STATION_BTN_DOWN;
	//m_pStationPressed.Load(bmppath );
	loadPic(bmppath,m_pStationPressed);
	bmppath = apppath + TVM_IMAGE_STATION_BTN_DOWN;
	//m_pStationHover.Load(bmppath );
	loadPic(bmppath,m_pStationHover);
	bmppath = apppath + TVM_IMAGE_STATION_BTN_DOWN;
	//m_pStationDisabled.Load(bmppath );
	loadPic(bmppath,m_pStationDisabled);

	// 取消按钮
	bmppath = apppath + TVM_IMAGE_CANCLE_BTN_NORMAL;
	//m_pBackNormal.Load(bmppath);
	loadPic(bmppath,m_pBackNormal);
	bmppath = apppath + TVM_IMAGE_CANCLE_BTN_NORMAL;
	//m_pBackPressed.Load(bmppath);
	loadPic(bmppath,m_pBackPressed);
	bmppath = apppath + TVM_IMAGE_CANCLE_BTN_NORMAL;
	//m_pBackHover.Load(bmppath);
	loadPic(bmppath,m_pBackHover);
	bmppath = apppath + TVM_IMAGE_CANCLE_BTN_NORMAL;
	//m_pBackDisabled.Load(bmppath);
	loadPic(bmppath,m_pBackDisabled);

	// 右侧功能区按钮
	bmppath = apppath + TVM_IMAGE_RIGHT_BTN_NORMAL;
	//m_pGroupNormal.Load(bmppath );
	loadPic(bmppath,m_pGroupNormal);
	bmppath = apppath + TVM_IMAGE_RIGHT_BTN_DOWN;
	//m_pGroupHover.Load(bmppath);
	loadPic(bmppath,m_pGroupHover);
	bmppath = apppath + TVM_IMAGE_RIGHT_BTN_DOWN;
	//m_pGroupPressed.Load(bmppath);
	loadPic(bmppath,m_pGroupPressed);
	bmppath = apppath + TVM_IMAGE_RIGHT_BTN_DOWN;
	//m_pGroupDisabled.Load(bmppath);
	loadPic(bmppath,m_pGroupDisabled);

	// 业务按钮
	bmppath = apppath + TVM_IMAGE_CHARGE_BTN_NORMAL;
	//m_pServiceNormal.Load(bmppath );
	loadPic(bmppath,m_pServiceNormal);
	bmppath = apppath + TVM_IMAGE_CHARGE_BTN_DOWN;
	//m_pServicePressed.Load(bmppath );
	loadPic(bmppath,m_pServicePressed);
	bmppath = apppath + TVM_IMAGE_CHARGE_BTN_DOWN;
	//m_pServiceHover.Load(bmppath );
	loadPic(bmppath,m_pServiceHover);
	bmppath = apppath + TVM_IMAGE_CHARGE_BTN_DOWN;
	//m_pServiceDisabled.Load(bmppath );
	loadPic(bmppath,m_pServiceDisabled);

	// 语言按钮
	bmppath = apppath + TVM_IMAGE_LANG_BTN_NORMAL;
	//m_pLanguageNormal.Load(bmppath );
	loadPic(bmppath,m_pLanguageNormal);
	bmppath = apppath + TVM_IMAGE_LANG_BTN_DOWN;
	//m_pLanguageHover.Load(bmppath );
	loadPic(bmppath,m_pLanguageHover);
	bmppath = apppath + TVM_IMAGE_LANG_BTN_DOWN;
	//m_pLanguagePressed.Load(bmppath );
	loadPic(bmppath,m_pLanguagePressed);
	bmppath = apppath + TVM_IMAGE_LANG_BTN_DOWN;
	//m_pLanguageDisabled.Load(bmppath );
	loadPic(bmppath,m_pLanguageDisabled);

	// 临时解决办法：用于不现实背景图，bmp后缀故意改成png
	bmppath = apppath + _T(".\\res\\TVM_btn_Purchase_Ok.bmp");
	//m_pLineAreaNormal.Load(bmppath );
	loadPic(bmppath,m_pLineAreaNormal);
	bmppath = apppath + _T(".\\res\\TVM_btn_Print_Confirm.bmp");
	//m_pLineAreaHover.Load(bmppath );
	loadPic(bmppath,m_pLineAreaHover);
	bmppath = apppath + _T(".\\res\\TVM_btn_Print_Confirm.bmp");
	//m_pLineAreaPressed.Load(bmppath );
	loadPic(bmppath,m_pLineAreaPressed);
	bmppath = apppath + _T(".\\res\\TVM_btn_Print_Confirm.bmp");
	//m_pLineAreaDisabled.Load(bmppath );
	loadPic(bmppath,m_pLineAreaDisabled);
	// end

	// 以下是针对武汉界面，新增加
	bmppath = apppath + TVM_IMAGE_GROUP_BLUE_NORMAL;
	loadPic(bmppath,m_pGroupBlueNormal);
	bmppath = apppath + TVM_IMAGE_GROUP_BLUE_DOWN;
	loadPic(bmppath,m_pGroupBluePressed);
	bmppath = apppath + TVM_IMAGE_GROUP_BLUE_DOWN;
	loadPic(bmppath,m_pGroupBlueHover);
	bmppath = apppath + TVM_IMAGE_GROUP_BLUE_DISABLE;
	loadPic(bmppath,m_pGroupBlueDisabled);

	bmppath = apppath + TVM_IMAGE_GROUP_BROWN_NORMAL;
	loadPic(bmppath,m_pGroupBrownNormal);
	bmppath = apppath + TVM_IMAGE_GROUP_BROWN_DOWN;
	loadPic(bmppath,m_pGroupBrownHover);
	bmppath = apppath + TVM_IMAGE_GROUP_BROWN_DOWN;
	loadPic(bmppath,m_pGroupBrownPressed);
	bmppath = apppath + TVM_IMAGE_GROUP_BROWN_DISABLE;
	loadPic(bmppath,m_pGroupBrownDisabled);

	bmppath = apppath + TVM_IMAGE_GROUP_BTN_ORANGE;
	loadPic(bmppath,m_pGroupBtnOrange);
	bmppath = apppath + TVM_IMAGE_GROUP_BTN_YELLOW;
	loadPic(bmppath,m_pGroupBtnYellow);
	bmppath = apppath + TVM_IMAGE_GROUP_BTN_BLUE;
	loadPic(bmppath,m_pGroupBtnBlue);
	bmppath = apppath + TVM_IMAGE_GROUP_BTN_HOT;
	loadPic(bmppath,m_pGroupBtnHot);

	// 20180308，热点车站新处理（各热点车站图标中包含站点名称，需要每个站点单独处理）
	LoadHotStationImg();
}

CBitmapManager::CBitmapManager()
{
}

CBitmapManager::~CBitmapManager()
{
	map<CString,CImage*>::iterator it = map_HotStation.begin();
	for(;it!=map_HotStation.end();it++){
		if(it->second!=NULL){
			delete it->second;
			it->second = NULL;
		}
	}
	map_HotStation.clear();
}


void CBitmapManager::LoadHotStationImg()
{
	// 取得文件目录
	CString appPath;
	GetAppPath(appPath);
	CString sDir = appPath + _T("res\\gui\\layout\\05\\");

	// 查找文件
	CFileFind finder;
	CString sFileName = _T("");
	BOOL bContinue = finder.FindFile(sDir +_T("*.jpg"));

	// 如果没有找到文件，就关闭文件
	if (!bContinue) {
		finder.Close();
		return;
	}

	while (bContinue) {
		bContinue = finder.FindNextFile();
		if (finder.IsDots()) {
			continue;
		}
		else if(!finder.IsDirectory()){ 
			sFileName = finder.GetFileName();
			CString sStationId = sFileName.Left(4);
			if(sStationId==sStationId.SpanIncluding(_T("0123456789"))){//纯数字字符串
				CImage* pTmpImg = new CImage;
				loadPic(sDir + sFileName, *pTmpImg);
				map_HotStation[sFileName.Left(7)] = pTmpImg;
			}			
			else{
				continue;
			}
		}
	}

	finder.Close();
}