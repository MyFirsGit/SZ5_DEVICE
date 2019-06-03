#include "stdafx.h"
#include "GuiParaMgr.h"


// 实例化对象
CGuiParaMgr CGuiParaMgr::instance;


//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuiParaMgr::CGuiParaMgr(){
	InitLoadPicture();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuiParaMgr::~CGuiParaMgr(){
	if(NULL != m_pNormalBtn){
		delete m_pNormalBtn;
		m_pNormalBtn = NULL;
	}

	for(auto it = m_map_pic_info.begin() ; it != m_map_pic_info.end();it++){
		if(it->second.pImage != NULL){
			delete it->second.pImage;
			it->second.pImage = NULL;
		}
	}
}

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
//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化底图

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuiParaMgr::InitLoadPicture(){
	map<CString,CString> map_path_files;
	map_path_files.clear();

	GetFileFromPath(TVM_GUI_LAYOUT,AFC_MAP_ID_3085,map_path_files);
	
	m_map_pic_info.clear();
	map<CString,CString>::iterator it = map_path_files.begin();
	while(it != map_path_files.end()){
		LINE_LAUGUAGE_IMAGE language_image;
		language_image.wLineId = _ttoi(it->first.Mid(11,2));
		language_image.language = ((it->first.Find(_T("CHN")) != -1) ? LANGUAGE_ID_CHINESE : LANGUAGE_ID_ENGLISH);

		// 文件是否存在
		if(PathFileExists(it->second)){
			language_image.szImageName = it->second;
			language_image.pImage = new CImage();

			if(NULL != language_image.pImage){
				HRESULT hr = language_image.pImage->Load(it->second);

				if(SUCCEEDED(hr)){
					transparentPng(it->second,language_image.pImage);
					m_map_pic_info[it->first] = language_image;
				}
				else{
					delete language_image.pImage;
					language_image.pImage = NULL;
				}
			}
		}
		it++;
	}

	// 初始化按钮信息
	m_pNormalBtn = new CImage();

	if(NULL != m_pNormalBtn){
		HRESULT result = m_pNormalBtn->Load(TVM_GUI_BTN_NORMAL);
		if(SUCCEEDED(result)){
			transparentPng(TVM_GUI_BTN_NORMAL,m_pNormalBtn);
		}
		else{
			delete m_pNormalBtn;
			m_pNormalBtn = NULL;
		}
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取线路底图配置信息

@param      IN DWORD dwLineId,OUT MAP_LAYOUT& layout

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuiParaMgr::GetLineMapInfo(IN DWORD dwLineId,OUT MAP_LAYOUT& layout){

	/*  检查是否有当前指定线路(除了总图)  */
	
	if(dwLineId == 0){
		layout.dwLineId = dwLineId;
		layout.bEnable = true;
	}
	else{
		//CAccAutomaticMachineMapParam::LINE_STATION_INFO stationInfo;
		//theACC_AutomaticMachineMap.GetLineInfo(dwLineId,stationInfo);
		//layout.dwLineId = stationInfo.dwLineId;
		//layout.bEnable	  = stationInfo.bLineRunAtNow;
	}

	SIZE size;
	size.cx = 0;
	size.cy = REC_FRMAE_RECT.top;

	layout.startPoint = REC_LINE_MAP_RECT.TopLeft() - size;
	layout.mapSize	  = REC_LINE_MAP_RECT.Size();

	layout.pImageBackupground = GetMapBackgroundImage(dwLineId,theAPP_SESSION.GetCurrentClientLanguageType());
	layout.szBackupground = GetMapBackground(dwLineId,theAPP_SESSION.GetCurrentClientLanguageType());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取指定线路中的按钮信息

@param      IN DWORD layoutId,OUT vector<MAP_BTN_INFO>& vecBtnInfo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuiParaMgr::GetLayoutBtnInfo(IN DWORD layoutId,OUT vector<MAP_BTN_INFO>& vecBtnInfo){

	//// 判断是否为主图

	//vector<CAccAutomaticMachineMapParam::STATION_INFO> vecStationInfo;
	//if(layoutId == 0)
	//	theACC_AutomaticMachineMap.GetMapBtnInfo(vecStationInfo);
	//else
	//	theACC_AutomaticMachineMap.GetLineBtnInfo(layoutId,vecStationInfo);

	//// 取得按钮信息后解析
	//for(auto it = vecStationInfo.begin();it != vecStationInfo.end();it++){
	//	MAP_BTN_INFO btnInfo;
	//	btnInfo.dwBtnId = it->dwStationId;
	//	btnInfo.bEnable = it->bRunAtNow;
	//	btnInfo.pBtnImage = m_pNormalBtn;
	//	if(layoutId == 0){
	//		double dbRadios = 1/*0.4965*/;
	//		CPoint point;
	//		point.x = ((int)((double)it->wXPos * dbRadios));
	//		point.y = ((int)((double)it->wYPos * dbRadios));
	//		
	//	
	//		CSize size;
	//		size.cx = ((int)((double)it->wXSelectArea * dbRadios));
	//		size.cy = ((int)((double)it->wYSelectArea * dbRadios));

	//		point.x -= size.cx;
	//		btnInfo.startBntPiont = point;//CPoint(it->wXPos * 4 /10,it->wYPos);
	//		btnInfo.sizeOfBtn	 = size;//CSize(it->wXSelectArea,it->wYSelectArea);
	//	}
	//	else{
	//		double dbRadios = 1.2415;
	//		CPoint point;
	//		point.x = ((int)(double)it->wXPos * dbRadios);
	//		point.y = ((int)(double)it->wYPos * dbRadios);
	//		

	//		CSize size;
	//		size.cx = ((int)(double)it->wXSelectArea * dbRadios);
	//		size.cy = ((int)(double)it->wYSelectArea * dbRadios);

	//		point.x -= size.cx;
	//		btnInfo.startBntPiont = point;//CPoint(it->wXPos,it->wYPos);
	//		btnInfo.sizeOfBtn	 = size;//CSize(it->wXSelectArea,it->wYSelectArea);
	//	}

	//	vecBtnInfo.push_back(btnInfo);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取指定语言及线路的背景图

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CImage* CGuiParaMgr::GetMapBackgroundImage(IN DWORD dwLindId,IN LANGUAGETYPE_T languageType){
	CImage* pImage = NULL;
	BYTE bLineId = (((dwLindId & 0xFF000000)>>24)& 0xFF);
	WORD wLineId = ((bLineId&0xf0)>>4)*10+(bLineId&0x0f);//((bLineId & 0xF0) >> 4) & 0xFF) * 10 + bLineId & 0x0F;

	// 总图
	if(dwLindId == 0)wLineId = 99;

	map<CString,LINE_LAUGUAGE_IMAGE>::iterator it = m_map_pic_info.begin();

	while(it != m_map_pic_info.end()){
		LINE_LAUGUAGE_IMAGE& itImage = it->second;

		if(wLineId == itImage.wLineId && languageType == itImage.language){
			pImage = itImage.pImage;
			break;
		}
		it++;
	}
	return pImage;
}
CString CGuiParaMgr::GetMapBackground(IN DWORD dwLindId,IN LANGUAGETYPE_T languageType)
{
	CString szImage = _T("");
	BYTE bLineId = (((dwLindId & 0xFF000000)>>24)& 0xFF);
	WORD wLineId = ((bLineId&0xf0)>>4)*10+(bLineId&0x0f);//((bLineId & 0xF0) >> 4) & 0xFF) * 10 + bLineId & 0x0F;

	// 总图
	if(dwLindId == 0)wLineId = 99;

	map<CString,LINE_LAUGUAGE_IMAGE>::iterator it = m_map_pic_info.begin();

	while(it != m_map_pic_info.end()){
		LINE_LAUGUAGE_IMAGE& itImage = it->second;

		if(wLineId == itImage.wLineId && languageType == itImage.language){
			szImage = itImage.szImageName;
			break;
		}
		it++;
	}
	return szImage;

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得指定目录下的文件

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuiParaMgr::GetFileFromPath(IN CString strPath,IN CString strFormat,OUT map<CString,CString>& mapPathFiles){
	try {
		// 取得文件目录
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + strPath;

		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir + strFormat);

		mapPathFiles.clear();

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return;
		}

		while (bContinue) {
			bContinue = finder.FindNextFile();
			if (finder.IsDots() || finder.IsDirectory()) {
				continue;
			}
			else {
				// find a file
				mapPathFiles[finder.GetFileName()] = finder.GetFilePath();
			}
		}

		finder.Close();

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}