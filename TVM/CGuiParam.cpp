#include "stdafx.h"
#include "CGuiParm.h"
#include <Shlwapi.h>
#pragma comment (lib,"shlwapi.lib")

#include <string>
#include <atlpath.h>

/** 
version 1.0.0
author mxw
time stamp 20140704 09.31

time stamp 20140714 17.31	修正 getNext** 系列函数中错误,应该都是getNextLine

time stamp 20140716 10.00	修正 UE_CSV_RESULT::UEUNKNOWN和其他的文件中UE_UNKNOWN冲突的问
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

CCsvReader::CLineData::CLineData()
{
}

CCsvReader::CLineData::~CLineData()
{
	std::map<int ,const TCHAR*>::iterator iter;
	for(iter=m_mapLine.begin();iter!=m_mapLine.end();iter++)
	{
		_aligned_free((void*)iter->second);
		iter->second=NULL;		
	}	
}

TCHAR* CCsvReader::CLineData::getFirstSplit()
{
	TCHAR* pword=NULL;
	m_iter=m_mapLine.begin();		
	if(m_mapLine.end()!=m_iter)
	{
		pword=(TCHAR*)m_iter->second;
	}else
	{
	}
	return pword;
}

TCHAR* CCsvReader::CLineData::getNextSplit()
{
	TCHAR* pword=NULL;
	if(m_iter!=m_mapLine.end()){
		m_iter++;
		if(m_iter!=m_mapLine.end())
		{
			pword=(TCHAR*)m_iter->second;
		}else
		{
		}
	}else{
	}
	return pword;
}

TCHAR* CCsvReader::CLineData::operator[](size_t index)
{	
	TCHAR* pword=NULL;
	std::map<int, const TCHAR*>::size_type isize;
	isize=m_mapLine.size();

	if(index<isize)
	{
		std::map <int , const TCHAR*> :: const_iterator iter;
		iter=m_mapLine.find(index);
		if(iter!=m_mapLine.end())
		{
			pword=(TCHAR*)iter->second;
		}
	}else
	{
		//请求越界
		pword=NULL;
	}	
	return pword;
}

size_t CCsvReader::CLineData::getMapSize()
{
	std::map<int, const TCHAR*>::size_type isize;
	isize=m_mapLine.size();
	return isize;
}
////////////////////////////////////////////////////////////////////////////////////////////
CCsvReader::CCsvReader()
{
}

CCsvReader::~CCsvReader()
{
	std::list <CLineData* >::iterator iter;
	for(iter=m_listLines.begin();iter!=m_listLines.end();iter++)
	{
		delete *iter;		
		*iter=NULL;
	}
	m_listLines;
}

UE_CSV_RESULT CCsvReader::splitLine(const TCHAR* tcLineData,const TCHAR* tcsplit)
{
	UE_CSV_RESULT ueret=UE_FAILE;
	//单独取到每一行的数据
	CLineData* pLineSplit=new CLineData();
	if(NULL!=pLineSplit)
	{

		int iindex=0;
		CString str(tcLineData);
		CString strTemp;
		int nIndex=0; 
		int nLast=0; 
		size_t alignment = 16;
		while(nIndex != -1)   
		{   
			nIndex = str.Find(tcsplit,nLast);   
			if(nIndex != -1)   
			{   
				strTemp = str.Mid(nLast,nIndex - nLast);   
				nLast   = nIndex + _tcslen(tcsplit);   
			}   
			else   
			{   
				strTemp = str.Mid(nLast,str.GetLength() - nLast);   
			}         
			TCHAR* tcLineWord=(TCHAR*)_aligned_malloc(sizeof(TCHAR)*(strTemp.GetLength()+1),alignment);
			memset(tcLineWord,0,sizeof(TCHAR)*(strTemp.GetLength()+1));
			_tcscpy_s(tcLineWord,strTemp.GetLength()+1,strTemp);
			typedef std::pair<int ,const TCHAR*>Key_Value;
			pLineSplit->m_mapLine.insert(Key_Value(iindex,tcLineWord));
			iindex++;
		} 

		/*TCHAR tcline[UD_MAX_LINEWORD];
		_tcscpy_s(tcline,UD_MAX_LINEWORD,tcLineData);
			
		int iindex=0;
		TCHAR* pNextToken=NULL;
		TCHAR* pToken=_tcstok_s(tcline,tcsplit,&pNextToken);
		while(NULL!=pToken)
		{
			int istrlen=_tcslen(pToken);
			size_t alignment = 16;
			TCHAR* tcLineWord=(TCHAR*)_aligned_malloc(sizeof(TCHAR)*(istrlen+1),alignment);
			_tcscpy_s(tcLineWord,istrlen+1,pToken);
			typedef std::pair<int ,const TCHAR*>Key_Value;
			pLineSplit->m_mapLine.insert(Key_Value(iindex,tcLineWord));
			pToken=_tcstok_s(NULL,tcsplit,&pNextToken);
			iindex++;
		}*/
		m_listLines.push_back(pLineSplit);
	}else
	{
		ueret=UE_OUTMEM;
	}
	return ueret;
}

void trimReturn(const TCHAR* tcBuffer)
{
	TCHAR* tcline=(TCHAR*)tcBuffer;
	TCHAR* pReturn=_tcsstr(tcline,_T("\r\n"));
	while(NULL!=pReturn)
	{
		int ilineLen=_tcslen(tcline)+1;
		int skiplen=pReturn-tcline;
		int cpylen=ilineLen-skiplen-2;
		TCHAR* pLeft=pReturn+2;
		_tcscpy_s(pReturn,cpylen,pLeft);
		memset(tcline+ilineLen-2,0,2);
		pReturn=_tcsstr(tcline,_T("\r\n"));
	}
}

UE_CSV_RESULT CCsvReader::loadCsvFile(const TCHAR* tcpath,const TCHAR* tcsplit)
{
	UE_CSV_RESULT ueret=UE_UNKNOWNCSVRET;
	BOOL bExist=PathFileExists(tcpath);
	if(bExist)
	{
		FILE* pfile;
		errno_t	error=_tfopen_s(&pfile,tcpath,_T("rb"));
		TCHAR tcline[UD_MAX_LINEWORD];
		if(error==0)
		{
			//文件打开成功
			TCHAR* pline=_fgetts(tcline,UD_MAX_LINEWORD,pfile);
			if(NULL!=pline)
			{//加1是为了忽略掉unicode的开头标识L
				trimReturn(tcline+1);
				splitLine(tcline+1,_T("$"));
			}
			while(_fgetts(tcline,UD_MAX_LINEWORD,pfile))
			{
				//此处加1 是因为 读取出来的 tcline 内存第一个单元是 L 不知道为什么 可能是 宽字符的标记
				trimReturn(tcline);
				splitLine(tcline,_T("$"));
			}
			/*while(_fgetts(tcline,UD_MAX_LINEWORD,pfile))
			{
				splitLine(tcline,_T("$"));
			}*/
			ueret=UE_SUCCESS;
		}else
		{
			ueret=UE_FAILOPEN;
			//文件打开失败
		}
	}else
	{
		ueret=UE_ERRPATH;
	}
	return ueret;
}

CCsvReader::CLineData* CCsvReader::getFirstLine()
{
	CLineData* pline=NULL;
	m_iter=m_listLines.begin();
	if(m_iter!=m_listLines.end())
	{
		pline=*m_iter;
	}else
	{
		//链表为空
	}
	return pline;
}

CCsvReader::CLineData* CCsvReader::getNextLine()
{
	CLineData* pline=NULL;
	if(m_iter!=m_listLines.end())
	{
		m_iter++;
		if(m_iter!=m_listLines.end())
		{
			pline=*m_iter;
		}else
		{
			//已经到链表最后
		}
	}else
	{
		//已经到链表最后
	}
	return pline;
}

size_t CCsvReader::getLineCount()
{
	size_t ilineCount=m_listLines.size();
	return ilineCount;
}
/////////////////////////////////////////////////////////////////////////////////
CImageMgr::CImageMgr()
{
}

CImageMgr::~CImageMgr()
{
	typedef pair<UINT,CImage*> Image_Pair;
	std::map<UINT,CImage*>::iterator it;
	for(it=m_mapImages.begin();it!=m_mapImages.end();it++)
	{
		CImage* pimage=it->second;
		if(NULL!=pimage)
		{
			delete pimage;
			pimage=NULL;
		}
	}
	m_mapImages.clear();
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

void CImageMgr::insertImage(TCHAR* tcpiccode, const TCHAR* pcpath)
{
	typedef pair<UINT,CImage*> Image_Pair;
	_TCHAR* tcstop=NULL;
	UINT uipicCode=_tcstoul(tcpiccode,&tcstop,10);
	if(PathFileExists(pcpath))
	{
		CImage* pimage=new CImage();
		if(NULL!=pimage)
		{
			HRESULT hr=pimage->Load(pcpath);
			if(SUCCEEDED(hr))
			{
				transparentPng(pcpath,pimage);
				m_mapImages.insert(Image_Pair(uipicCode,pimage));
			}
			else{
				delete pimage;
				pimage = NULL;
			}
		}
	}else
	{
		DWORD dwError=GetLastError();
		int a=1;
	}
}

CImage* CImageMgr::getImage(TCHAR* tcpiccode)
{
	CImage* pret=NULL;
	_TCHAR* tcstop=NULL;
	UINT uipicCode=_tcstoul(tcpiccode,&tcstop,10);

	std::map<UINT,CImage*>::iterator itfind;
	itfind=m_mapImages.find(uipicCode);
	if(itfind!=m_mapImages.end())
	{
		pret=itfind->second;
	}
	return pret;
}

/////////////////////////////////////////////////////////////////////////////////
CGuiParm::CGuiParm()
{
	CString sAppPath;
	GetAppPath(sAppPath);
	initializeLayOut(sAppPath+TVM_GUI_LAYOUT_CSV);
	initializeButton(sAppPath+TVM_GUI_BUTTON_CSV);
    initializePic(sAppPath+TVM_GUI_IMAGE_CSV);
}

CGuiParm::~CGuiParm()
{
}

void CGuiParm::initializeLayOut(const TCHAR* tcpath,const TCHAR* tcsplit)
{
	m_ueLayOutRet=m_layOutCsv.loadCsvFile(tcpath,tcsplit);
	if(m_ueLayOutRet==UE_SUCCESS)
	{
		size_t linecount=m_layOutCsv.getLineCount();
		m_usSpecialLayoutDatas.m_pDataPtr=(US_LAYOUT_DATA*)_aligned_malloc(sizeof(US_LAYOUT_DATA)*linecount,UD_MEM_ALIGNMEMENT);
		if(NULL!=m_usSpecialLayoutDatas.m_pDataPtr)
		{
			memset(m_usSpecialLayoutDatas.m_pDataPtr,0,sizeof(US_LAYOUT_DATA)*linecount);
		}
	}else
	{
	}
}

void CGuiParm::initializeButton(const TCHAR* tcpath,const TCHAR* tcsplit)
{
	m_ueButtonRet=m_buttonCsv.loadCsvFile(tcpath,tcsplit);
	if(m_ueButtonRet==UE_SUCCESS)
	{
		size_t linecount=m_buttonCsv.getLineCount();
		m_usSpecialBtnDatas.m_pDataPtr=(US_BUTTON_DATA*)_aligned_malloc(sizeof(US_BUTTON_DATA)*linecount,UD_MEM_ALIGNMEMENT);
		if(NULL!=m_usSpecialBtnDatas.m_pDataPtr)
		{
			memset(m_usSpecialBtnDatas.m_pDataPtr,0,sizeof(US_BUTTON_DATA)*linecount);
		}
	}else{
	}
}

void CGuiParm::initializePic(const TCHAR* tcpath,const TCHAR* tcsplit)
{
	m_uePicRet=m_picCsv.loadCsvFile(tcpath,tcsplit);
	if(m_uePicRet==UE_SUCCESS)
	{
		size_t linecount=m_picCsv.getLineCount();
		m_usSpecialPicDatas.m_pDataPtr=(US_PIC_DATA*)_aligned_malloc(sizeof(US_PIC_DATA)*linecount,UD_MEM_ALIGNMEMENT);
		if(NULL!=m_usSpecialPicDatas.m_pDataPtr)
		{
			memset(m_usSpecialPicDatas.m_pDataPtr,0,sizeof(US_PIC_DATA)*linecount);
		}

		CString sAppPath;
		GetAppPath(sAppPath);
		CCsvReader::CLineData* pPicData=m_picCsv.getFirstLine();
		while(NULL!=pPicData)
		{
			TCHAR tcPath[MAX_PATH];
			_tcscpy_s(tcPath,MAX_PATH,sAppPath);
			_tcscat_s(tcPath,MAX_PATH,TVM_GUI_PATH);
			TCHAR* tcpicCode=(*pPicData)[US_PIC_DATA::UE_PICCODE];
			TCHAR* tcpicName=(*pPicData)[US_PIC_DATA::UE_PICNAME];
			_tcscat_s(tcPath,MAX_PATH,tcpicName);
			pPicData=m_picCsv.getNextLine();
			m_imageMgr.insertImage(tcpicCode,tcPath);
			CPath path(tcPath);
			BOOL bexist=path.FileExists();
			int a=1;
		}
	}else
	{
	}
}

US_LAYOUT_DATA*	CGuiParm::getFirstLayOutData()
{
	US_LAYOUT_DATA* pret=NULL;
	if(m_ueLayOutRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pFirstLine=m_layOutCsv.getFirstLine();
		if(NULL!=pFirstLine)
		{
			m_layoutData.pPageCode=(*pFirstLine)[US_LAYOUT_DATA::UE_PAGECODE];
			m_layoutData.pPageMark=(*pFirstLine)[US_LAYOUT_DATA::UE_PAGEMARK];
			m_layoutData.pChBaseMapCode=(*pFirstLine)[US_LAYOUT_DATA::UE_CHBASEMAPCODE];
			m_layoutData.pEnBaseMapCode=(*pFirstLine)[US_LAYOUT_DATA::UE_ENBASEMAPCODE];
			m_layoutData.pPuBaseMapCode=(*pFirstLine)[US_LAYOUT_DATA::UE_PUBASEMAPCODE];
			m_layoutData.PTopLeftX=(*pFirstLine)[US_LAYOUT_DATA::UE_TOPLEFTX];
			m_layoutData.PTopLeftY=(*pFirstLine)[US_LAYOUT_DATA::UE_TOPLEFTY];
			m_layoutData.pPicWidth=(*pFirstLine)[US_LAYOUT_DATA::UE_PICWIDTH];
			m_layoutData.pPicHeight=(*pFirstLine)[US_LAYOUT_DATA::UE_PICHEIGHT];
			m_layoutData.pIsHomePage=(*pFirstLine)[US_LAYOUT_DATA::UE_ISHOMEPAGE];
			m_layoutData.PEnableMark=(*pFirstLine)[US_LAYOUT_DATA::UE_ENABLEAMRK];
			m_layoutData.pElementEnableMark=(*pFirstLine)[US_LAYOUT_DATA::UE_ELEMENTENABLEMARK];
			pret=&m_layoutData;
		}else
		{
		}		
	}
	return pret;
}

US_LAYOUT_DATA*	CGuiParm::getNextLayOutData()
{
	US_LAYOUT_DATA* pret=NULL;
	if(m_ueLayOutRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pNextLine=m_layOutCsv.getNextLine();
		if(NULL!=pNextLine){
			m_layoutData.pPageCode=(*pNextLine)[US_LAYOUT_DATA::UE_PAGECODE];
			m_layoutData.pPageMark=(*pNextLine)[US_LAYOUT_DATA::UE_PAGEMARK];
			m_layoutData.pChBaseMapCode=(*pNextLine)[US_LAYOUT_DATA::UE_CHBASEMAPCODE];
			m_layoutData.pEnBaseMapCode=(*pNextLine)[US_LAYOUT_DATA::UE_ENBASEMAPCODE];
			m_layoutData.pPuBaseMapCode=(*pNextLine)[US_LAYOUT_DATA::UE_PUBASEMAPCODE];
			m_layoutData.PTopLeftX=(*pNextLine)[US_LAYOUT_DATA::UE_TOPLEFTX];
			m_layoutData.PTopLeftY=(*pNextLine)[US_LAYOUT_DATA::UE_TOPLEFTY];
			m_layoutData.pPicWidth=(*pNextLine)[US_LAYOUT_DATA::UE_PICWIDTH];
			m_layoutData.pPicHeight=(*pNextLine)[US_LAYOUT_DATA::UE_PICHEIGHT];
			m_layoutData.pIsHomePage=(*pNextLine)[US_LAYOUT_DATA::UE_ISHOMEPAGE];
			m_layoutData.PEnableMark=(*pNextLine)[US_LAYOUT_DATA::UE_ENABLEAMRK];
			m_layoutData.pElementEnableMark=(*pNextLine)[US_LAYOUT_DATA::UE_ELEMENTENABLEMARK];
			pret=&m_layoutData;
		}else{
		}
	}
	return pret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      獲取指定條件的 layout 信息

@param      (i) ueIndex				指定索引列
			(i) tcSpecialValue		指定索引值
@retval     US_SPECIALLAYOUTDATAS

@exception  none
*/
//////////////////////////////////////////////////////////////////////////

US_SPECIALLAYOUTDATAS* CGuiParm::getSpecialLayoutDatas(US_LAYOUT_DATA::UE_LAYOUT_INDEX ueIndex,const TCHAR* tcSpecialValue)
{
	US_SPECIALLAYOUTDATAS* pret=NULL;
	if(NULL!=m_usSpecialLayoutDatas.m_pDataPtr)
	{
		int ifindcount=0;
		CCsvReader::CLineData* pfirstLine=m_layOutCsv.getFirstLine();
		if(NULL!=pfirstLine)
		{
			TCHAR* tcFindValue=(*pfirstLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialLayoutDatas.m_pDataPtr[0].pPageCode=(*pfirstLine)[US_LAYOUT_DATA::UE_PAGECODE];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pPageMark=(*pfirstLine)[US_LAYOUT_DATA::UE_PAGEMARK];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pChBaseMapCode=(*pfirstLine)[US_LAYOUT_DATA::UE_CHBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pEnBaseMapCode=(*pfirstLine)[US_LAYOUT_DATA::UE_ENBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pPuBaseMapCode=(*pfirstLine)[US_LAYOUT_DATA::UE_PUBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[0].PTopLeftX=(*pfirstLine)[US_LAYOUT_DATA::UE_TOPLEFTX];
					m_usSpecialLayoutDatas.m_pDataPtr[0].PTopLeftY=(*pfirstLine)[US_LAYOUT_DATA::UE_TOPLEFTY];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pPicWidth=(*pfirstLine)[US_LAYOUT_DATA::UE_PICWIDTH];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pPicHeight=(*pfirstLine)[US_LAYOUT_DATA::UE_PICHEIGHT];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pIsHomePage=(*pfirstLine)[US_LAYOUT_DATA::UE_ISHOMEPAGE];
					m_usSpecialLayoutDatas.m_pDataPtr[0].PEnableMark=(*pfirstLine)[US_LAYOUT_DATA::UE_ENABLEAMRK];
					m_usSpecialLayoutDatas.m_pDataPtr[0].pElementEnableMark=(*pfirstLine)[US_LAYOUT_DATA::UE_ELEMENTENABLEMARK];
					ifindcount++;
					m_usSpecialLayoutDatas.idataCount=ifindcount;
				
				}else
				{
				}
			}
			
		}else
		{
		}
		int icount=1;
		CCsvReader::CLineData* pnextLine=m_layOutCsv.getNextLine();
		while(NULL!=pnextLine)
		{
			TCHAR* tcFindValue=(*pnextLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pPageCode=(*pnextLine)[US_LAYOUT_DATA::UE_PAGECODE];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pPageMark=(*pnextLine)[US_LAYOUT_DATA::UE_PAGEMARK];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pChBaseMapCode=(*pnextLine)[US_LAYOUT_DATA::UE_CHBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pEnBaseMapCode=(*pnextLine)[US_LAYOUT_DATA::UE_ENBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pPuBaseMapCode=(*pnextLine)[US_LAYOUT_DATA::UE_PUBASEMAPCODE];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].PTopLeftX=(*pnextLine)[US_LAYOUT_DATA::UE_TOPLEFTX];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].PTopLeftY=(*pnextLine)[US_LAYOUT_DATA::UE_TOPLEFTY];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pPicWidth=(*pnextLine)[US_LAYOUT_DATA::UE_PICWIDTH];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pPicHeight=(*pnextLine)[US_LAYOUT_DATA::UE_PICHEIGHT];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pIsHomePage=(*pnextLine)[US_LAYOUT_DATA::UE_ISHOMEPAGE];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].PEnableMark=(*pnextLine)[US_LAYOUT_DATA::UE_ENABLEAMRK];
					m_usSpecialLayoutDatas.m_pDataPtr[ifindcount].pElementEnableMark=(*pnextLine)[US_LAYOUT_DATA::UE_ELEMENTENABLEMARK];
					ifindcount++;
				}else
				{
				}
			}
			pnextLine=m_layOutCsv.getNextLine();
			icount++;
		}

		m_usSpecialLayoutDatas.idataCount=ifindcount;

		pret=&m_usSpecialLayoutDatas;
	}
	return pret;
}

void CGuiParm::m_getAreaInfo(US_LAYOUT_DATA* playOutData,LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut)
{
	TCHAR* pstoping=NULL;
	//iD
	firstLayOut.layOutId=_tcstoul(playOutData->pPageCode,&pstoping,10);
	//isused
	if(_tcscmp(playOutData->PEnableMark,_T("1"))==0)
	{
		firstLayOut.isEnable=true;
	}else
	{
		firstLayOut.isEnable=false;
	}
	//startpoint
	firstLayOut.startpoint.x=_tstol(playOutData->PTopLeftX);
	firstLayOut.startpoint.y=_tstol(playOutData->PTopLeftY);
	//size
	firstLayOut.areaSize.cx=_tstol(playOutData->pPicWidth);
	firstLayOut.areaSize.cy=_tstol(playOutData->pPicHeight);
	//mapcode
	switch(languageType)
	{
	case LANGUAGE_ID_CHINESE:
		firstLayOut.pImageBackground=m_imageMgr.getImage(playOutData->pChBaseMapCode);
		//firstLayOut.areaBackImage=playOutData->pChBaseMapCode;
		break;
	case LANGUAGE_ID_ENGLISH:
		firstLayOut.pImageBackground=m_imageMgr.getImage(playOutData->pEnBaseMapCode);
		//firstLayOut.areaBackImage=playOutData->pEnBaseMapCode;
		break;
	case LANGUAGE_ID_PORTUGUESE:
		firstLayOut.pImageBackground=m_imageMgr.getImage(playOutData->pPuBaseMapCode);
		//firstLayOut.areaBackImage=playOutData->pPuBaseMapCode;
		break;
	case LANGUAGE_ID_UNSPECIFIED:
	default:
		break;
	}

	/*US_SPECIALPICDATAS* pPic=getSpecialPicDatas(US_PIC_DATA::UE_PICCODE,playOutData->pChBaseMapCode);
	if(NULL!=pPic)
	{
		int ipicCount=pPic->m_idataCount;
		if(ipicCount>=1)
		{
			firstLayOut.areaBackImage = TVM_GUI_PATH;
			firstLayOut.areaBackImage += pPic->m_pDataPtr[0].pPicName;
			firstLayOut.areaBackImage.TrimRight(_T("\r\n"));
		}
	}else
	{
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      獲取第一條 區域 信息

@param      
			(i) languageType		語種
			(o) firstLayOut	[out]	所有符合條件的信息數組
@retval     long 
			0 成功
			1 失敗

@exception  none
*/
//////////////////////////////////////////////////////////////////////////

long CGuiParm::getFirstAreaLaylout(LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut)
{
	long lret=1;
	US_LAYOUT_DATA* pfirstLayOut=getFirstLayOutData();
	if(NULL!=pfirstLayOut)
	{
		m_getAreaInfo(pfirstLayOut,languageType,firstLayOut);
		lret=0;
	}else
	{
	}
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根據指定條件獲取 區域信息

@param      (i)	areaLayoutID		頁編碼
			(i) languageType		語種
			(o) firstLayOut			符合條件的信息
@retval     long 
			0 成功
			1 失敗

@exception  none
*/
//////////////////////////////////////////////////////////////////////////

long CGuiParm::getAreaLaylout(DWORD areaLayoutID,LANGUAGETYPE_T languageType,AREA_LAYOUT& layOutData)
{
	long lret=1;
	US_LAYOUT_DATA* pfirstLayOut=getFirstLayOutData();
	if(NULL!=pfirstLayOut)
	{
		TCHAR* pstoping=NULL;
		DWORD dwCurCode=_tcstoul(pfirstLayOut->pPageCode,&pstoping,10);
		if(dwCurCode==areaLayoutID)
		{
			//找到了
			m_getAreaInfo(pfirstLayOut,languageType,layOutData);
			lret=0;
		}else
		{
			//没有找到,需要继续遍历
			US_LAYOUT_DATA* pnext=getNextLayOutData();
			while(NULL!=pnext)
			{
				dwCurCode=_tcstoul(pnext->pPageCode,&pstoping,10);
				if(dwCurCode==areaLayoutID)
				{
					m_getAreaInfo(pnext,languageType,layOutData);
					lret=0;
					break;
				}
				else
				{
					pnext=getNextLayOutData();
					continue;
				}
			}
		}
	}
	return lret;
}

US_PIC_DATA* CGuiParm::getFirstPicData()
{
	US_PIC_DATA* pret=NULL;
	if(m_uePicRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pFirstLine=m_picCsv.getFirstLine();
		if(NULL!=pFirstLine)
		{
			m_picData.pPicCode=(*pFirstLine)[US_PIC_DATA::UE_PICCODE];
			m_picData.pPicName=(*pFirstLine)[US_PIC_DATA::UE_PICNAME];
			pret=&m_picData;
		}else
		{
		}
	}
	return pret;
}

US_PIC_DATA* CGuiParm::getNextPicData()
{
	US_PIC_DATA* pret=NULL;
	if(m_uePicRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pNextLine=m_picCsv.getNextLine();
		if(NULL!=pNextLine)
		{
			m_picData.pPicCode=(*pNextLine)[US_PIC_DATA::UE_PICCODE];
			m_picData.pPicName=(*pNextLine)[US_PIC_DATA::UE_PICNAME];
			pret=&m_picData;
		}else{
		}
	}
	return pret;
}

US_SPECIALPICDATAS* CGuiParm::getSpecialPicDatas(US_PIC_DATA::UE_PIC_INDEX ueIndex,const TCHAR* tcSpecialValue)
{
	US_SPECIALPICDATAS* pret=NULL;
	if(NULL!=m_usSpecialPicDatas.m_pDataPtr)
	{
		int ifindcount=0;
		CCsvReader::CLineData* pfirstLine=m_picCsv.getFirstLine();
		if(NULL!=pfirstLine)
		{
			TCHAR* tcFindValue=(*pfirstLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialPicDatas.m_pDataPtr[0].pPicCode=(*pfirstLine)[US_PIC_DATA::UE_PICCODE];
					m_usSpecialPicDatas.m_pDataPtr[0].pPicName=(*pfirstLine)[US_PIC_DATA::UE_PICNAME];
					ifindcount++;
					m_usSpecialPicDatas.m_idataCount=ifindcount;
				}else
				{
				}
			}
			
		}else
		{
		}
		int icount=1;
		CCsvReader::CLineData* pnextLine=m_picCsv.getNextLine();
		while(NULL!=pnextLine)
		{
			TCHAR* tcFindValue=(*pnextLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialPicDatas.m_pDataPtr[ifindcount].pPicCode=(*pnextLine)[US_PIC_DATA::UE_PICCODE];
					m_usSpecialPicDatas.m_pDataPtr[ifindcount].pPicName=(*pnextLine)[US_PIC_DATA::UE_PICNAME];
					ifindcount++;
				}else
				{
				}
			}
			pnextLine=m_picCsv.getNextLine();
			icount++;
		}
		m_usSpecialPicDatas.m_idataCount=ifindcount;
		pret=&m_usSpecialPicDatas;
	}
	return pret;
}

US_BUTTON_DATA* CGuiParm::getFirstBtnData()
{
	US_BUTTON_DATA* pret=NULL;
	if(m_ueButtonRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pFirstLine=m_buttonCsv.getFirstLine();
		if(NULL!=pFirstLine)
		{
			m_btnData.pBtnCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCODE];
			m_btnData.pBtnPage=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPAGE];
			m_btnData.pBtnMark=(*pFirstLine)[US_BUTTON_DATA::UE_BTNMARK];
			m_btnData.pBtnValue=(*pFirstLine)[US_BUTTON_DATA::UE_BTNVALUE];
			m_btnData.pBtnJumpPage=(*pFirstLine)[US_BUTTON_DATA::UE_BTNJUMPPAGE];
			m_btnData.pBtnXPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNXPOS];
			m_btnData.pBtnYPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNYPOS];
			m_btnData.pBtnWidth=(*pFirstLine)[US_BUTTON_DATA::UE_BTNWIDTH];
			m_btnData.pBtnHeight=(*pFirstLine)[US_BUTTON_DATA::UE_BTNHEIGHT];
			m_btnData.pBtnNormamPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNNORMALPICCODE];
			m_btnData.pBtnPressedPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPRESSEDPICCODE];
			m_btnData.pBtnHoverPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNHOVERPICCODE];
			m_btnData.pBtnDisablePicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNDISABLEPICCODE];
			m_btnData.pBtnIsEnable=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISENABLE];
			m_btnData.pBtnIsShowMark=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISSHOWMARK];
			m_btnData.pBtnIsHotStation=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISHOTSTATION];


			m_btnData.pChtxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHTXTPOS];
			m_btnData.pChcontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHTXTCONTENT];
			m_btnData.pChfontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTNAME];
			m_btnData.pChfontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTSIZE];
			m_btnData.pChfontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTCOLOR];

			m_btnData.pEntxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENTXTPOS];
			m_btnData.pEncontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENTXTCONTENT];
			m_btnData.pEnfontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTNAME];
			m_btnData.pEnfontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTSIZE];
			m_btnData.pEnfontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTCOLOR];


			m_btnData.pPutxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUTXTPOS];
			m_btnData.pPucontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUTXTCONTENT];
			m_btnData.pPufontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTNAME];
			m_btnData.pPufontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTSIZE];
			m_btnData.pPufontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTCOLOR];

			pret=&m_btnData;
		}else
		{
		}
	}
	return pret;
}

US_BUTTON_DATA* CGuiParm::getNextBtnData()
{
	US_BUTTON_DATA* pret=NULL;
	if(m_ueButtonRet==UE_SUCCESS)
	{
		CCsvReader::CLineData* pNextLine=m_buttonCsv.getNextLine();
		if(NULL!=pNextLine)
		{
			m_btnData.pBtnCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNCODE];
			m_btnData.pBtnPage=(*pNextLine)[US_BUTTON_DATA::UE_BTNPAGE];
			m_btnData.pBtnMark=(*pNextLine)[US_BUTTON_DATA::UE_BTNMARK];
			m_btnData.pBtnValue=(*pNextLine)[US_BUTTON_DATA::UE_BTNVALUE];
			m_btnData.pBtnJumpPage=(*pNextLine)[US_BUTTON_DATA::UE_BTNJUMPPAGE];
			m_btnData.pBtnXPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNXPOS];
			m_btnData.pBtnYPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNYPOS];
			m_btnData.pBtnWidth=(*pNextLine)[US_BUTTON_DATA::UE_BTNWIDTH];
			m_btnData.pBtnHeight=(*pNextLine)[US_BUTTON_DATA::UE_BTNHEIGHT];
			m_btnData.pBtnNormamPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNNORMALPICCODE];
			m_btnData.pBtnPressedPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNPRESSEDPICCODE];
			m_btnData.pBtnHoverPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNHOVERPICCODE];
			m_btnData.pBtnDisablePicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNDISABLEPICCODE];
			m_btnData.pBtnIsEnable=(*pNextLine)[US_BUTTON_DATA::UE_BTNISENABLE];
			m_btnData.pBtnIsShowMark=(*pNextLine)[US_BUTTON_DATA::UE_BTNISSHOWMARK];
			m_btnData.pBtnIsHotStation=(*pNextLine)[US_BUTTON_DATA::UE_BTNISHOTSTATION];

			m_btnData.pChtxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHTXTPOS];
			m_btnData.pChcontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHTXTCONTENT];
			m_btnData.pChfontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTNAME];
			m_btnData.pChfontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTSIZE];
			m_btnData.pChfontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTCOLOR];

			m_btnData.pEntxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNENTXTPOS];
			m_btnData.pEncontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNENTXTCONTENT];
			m_btnData.pEnfontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTNAME];
			m_btnData.pEnfontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTSIZE];
			m_btnData.pEnfontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTCOLOR];


			m_btnData.pPutxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUTXTPOS];
			m_btnData.pPucontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUTXTCONTENT];
			m_btnData.pPufontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTNAME];
			m_btnData.pPufontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTSIZE];
			m_btnData.pPufontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTCOLOR];

			pret=&m_btnData;
		}else
		{
		}
	}
	return pret;
}

US_SPECIALBTNDATAS* CGuiParm::getSpecialBtnDatas(US_BUTTON_DATA::UE_BTN_INDEX ueIndex,const TCHAR* tcSpecialValue)
{
	US_SPECIALBTNDATAS* pret=NULL;
	if(NULL!=m_usSpecialBtnDatas.m_pDataPtr)
	{
		int ifindcount=0;
		CCsvReader::CLineData* pFirstLine=m_buttonCsv.getFirstLine();
		if(NULL!=pFirstLine)
		{
			TCHAR* tcFindValue=(*pFirstLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCODE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnPage=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPAGE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnMark=(*pFirstLine)[US_BUTTON_DATA::UE_BTNMARK];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnValue=(*pFirstLine)[US_BUTTON_DATA::UE_BTNVALUE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnJumpPage=(*pFirstLine)[US_BUTTON_DATA::UE_BTNJUMPPAGE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnXPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNXPOS];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnYPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNYPOS];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnWidth=(*pFirstLine)[US_BUTTON_DATA::UE_BTNWIDTH];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnHeight=(*pFirstLine)[US_BUTTON_DATA::UE_BTNHEIGHT];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnNormamPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNNORMALPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnPressedPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPRESSEDPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnHoverPicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNHOVERPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnDisablePicCode=(*pFirstLine)[US_BUTTON_DATA::UE_BTNDISABLEPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnIsEnable=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISENABLE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnIsShowMark=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISSHOWMARK];
					m_usSpecialBtnDatas.m_pDataPtr[0].pBtnIsHotStation=(*pFirstLine)[US_BUTTON_DATA::UE_BTNISHOTSTATION];


					m_usSpecialBtnDatas.m_pDataPtr[0].pChtxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[0].pChcontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[0].pChfontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[0].pChfontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pChfontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNCHFONTCOLOR];

					m_usSpecialBtnDatas.m_pDataPtr[0].pEntxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[0].pEncontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[0].pEnfontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[0].pEnfontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pEnfontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNENFONTCOLOR];


					m_usSpecialBtnDatas.m_pDataPtr[0].pPutxtPos=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[0].pPucontent=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[0].pPufontName=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[0].pPufontSize=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[0].pPufontColor=(*pFirstLine)[US_BUTTON_DATA::UE_BTNPUFONTCOLOR];

					ifindcount++;
					m_usSpecialBtnDatas.m_idataCount=ifindcount;
				}else
				{
				}
			}
			
		}else
		{
		}
		int icount=1;
		CCsvReader::CLineData* pNextLine=m_buttonCsv.getNextLine();
		while(NULL!=pNextLine)
		{
			TCHAR* tcFindValue=(*pNextLine)[ueIndex];
			if(NULL!=tcFindValue && NULL!=tcSpecialValue)
			{
				if(_tcscmp(tcFindValue,tcSpecialValue)==0)
				{
					//找到了匹配行
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNCODE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnPage=(*pNextLine)[US_BUTTON_DATA::UE_BTNPAGE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnMark=(*pNextLine)[US_BUTTON_DATA::UE_BTNMARK];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnValue=(*pNextLine)[US_BUTTON_DATA::UE_BTNVALUE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnJumpPage=(*pNextLine)[US_BUTTON_DATA::UE_BTNJUMPPAGE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnXPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNXPOS];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnYPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNYPOS];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnWidth=(*pNextLine)[US_BUTTON_DATA::UE_BTNWIDTH];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnHeight=(*pNextLine)[US_BUTTON_DATA::UE_BTNHEIGHT];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnNormamPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNNORMALPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnPressedPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNPRESSEDPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnHoverPicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNHOVERPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnDisablePicCode=(*pNextLine)[US_BUTTON_DATA::UE_BTNDISABLEPICCODE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnIsEnable=(*pNextLine)[US_BUTTON_DATA::UE_BTNISENABLE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnIsShowMark=(*pNextLine)[US_BUTTON_DATA::UE_BTNISSHOWMARK];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pBtnIsHotStation=(*pNextLine)[US_BUTTON_DATA::UE_BTNISHOTSTATION];

					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pChtxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pChcontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pChfontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pChfontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pChfontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNCHFONTCOLOR];

					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pEntxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNENTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pEncontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNENTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pEnfontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pEnfontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pEnfontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNENFONTCOLOR];


					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pPutxtPos=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUTXTPOS];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pPucontent=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUTXTCONTENT];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pPufontName=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTNAME];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pPufontSize=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTSIZE];
					m_usSpecialBtnDatas.m_pDataPtr[ifindcount].pPufontColor=(*pNextLine)[US_BUTTON_DATA::UE_BTNPUFONTCOLOR];
					ifindcount++;
				}else
				{
				}
			}
			pNextLine=m_buttonCsv.getNextLine();
			icount++;
		}
		m_usSpecialBtnDatas.m_idataCount=ifindcount;
		pret=&m_usSpecialBtnDatas;
	}
	return pret;
}

long CGuiParm::getLayOutPageCodes(vector<DWORD>& pageCodes)
{
	long lret=0;
	US_LAYOUT_DATA*	pItem=getFirstLayOutData();
	while(NULL!=pItem){
		DWORD dwPageCode=_ttoi(pItem->pPageCode);
		pageCodes.push_back(dwPageCode);
		pItem=getNextLayOutData();
	}
	return lret;
}

long CGuiParm::getPicData(vector<US_PIC_DATA>& picsInfo)
{
	long lret=0;
	US_PIC_DATA* pItem=getFirstPicData();
	while(NULL!=pItem)
	{
		US_PIC_DATA picData;
		picData.pPicCode=pItem->pPicCode;
		picData.pPicName=pItem->pPicName;
		picsInfo.push_back(picData);

		pItem=getNextPicData();
	}
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      獲取按鈕信息

@param      (i)	areaID	所屬頁編號 對應 button 中的第二列
			(i) languageType	語種
			(o) btnInfos	[out] 所有符合條件的信息數組
@retval     long 
			0 成功
			1 失敗
			

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CGuiParm::getAreaBtnInfo(DWORD areaID,LANGUAGETYPE_T languageType,vector<AREA_BTN_INFO>& btnInfos)
{
	long lret=1;
	//TCHAR tcAreaId[11];
	//_stprintf_s(tcAreaId,11,_T("%ul"),areaID);
	CString tcAreaId;
	tcAreaId.Format(_T("%d"),areaID);
	US_SPECIALBTNDATAS* pspecialBtns=getSpecialBtnDatas(US_BUTTON_DATA::UE_BTNPAGE,tcAreaId);
	if(NULL!=pspecialBtns)
	{
		US_BUTTON_DATA* pBtns=pspecialBtns->m_pDataPtr;
		TCHAR* pStop=NULL;
		if(NULL!=pBtns)
		{
			for(int iloop=0;iloop<pspecialBtns->m_idataCount;iloop++)
			{
				AREA_BTN_INFO btnInfo;
				//按钮编号
				btnInfo.number = _tcstoul(pBtns[iloop].pBtnCode,&pStop,10);
				//按钮值
				btnInfo.btnID=_tcstoul(pBtns[iloop].pBtnValue,&pStop,10);
				//跳转页
				btnInfo.nextPage=_tcstoul(pBtns[iloop].pBtnJumpPage,&pStop,10);
				//是否热点
				btnInfo.isHotStation=(_tcscmp(pBtns[iloop].pBtnIsHotStation,_T("1"))==0) ? true : false;
				//是否启用
				btnInfo.isEnable=(_tcscmp(pBtns[iloop].pBtnIsEnable,_T("1"))==0) ? true : false;
				//是否显示
				btnInfo.isVisiable=(_tcscmp(pBtns[iloop].pBtnIsShowMark,_T("1"))==0) ? true : false;
				//按钮坐标
				btnInfo.stationBtnPoint.x=_tstol(pBtns[iloop].pBtnXPos);
				btnInfo.stationBtnPoint.y=_tstol(pBtns[iloop].pBtnYPos);
				//按钮大小
				btnInfo.stationBtnSize.cx=_tstol(pBtns[iloop].pBtnWidth);
				btnInfo.stationBtnSize.cy=_tstol(pBtns[iloop].pBtnHeight);
				//多国语言相关部分
				LPTSTR pstr = NULL;
				switch(languageType)
				{
					case LANGUAGE_ID_CHINESE:
						// 文本内容
						btnInfo.text = pBtns[iloop].pChcontent;
						// 字体大小
						btnInfo.fontname = pBtns[iloop].pChfontName;
						// 字体大小
						btnInfo.fontsize = _tstol(pBtns[iloop].pChfontSize);
						// 字体颜色
						btnInfo.textcolor = _tcstoul(pBtns[iloop].pChfontColor, &pstr, 16);
						// 文本偏移
						btnInfo.textoffset.x = _tcstol(pBtns[iloop].pChtxtPos, &pstr, 10); ASSERT(pstr);
						btnInfo.textoffset.y = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						break;
					case LANGUAGE_ID_ENGLISH:
						btnInfo.text = pBtns[iloop].pEncontent;
						btnInfo.fontname = pBtns[iloop].pEnfontName;
						btnInfo.fontsize = _tstol(pBtns[iloop].pEnfontSize);
						btnInfo.textcolor = _tcstoul(pBtns[iloop].pEnfontColor, &pstr, 16);
						btnInfo.textoffset.x = _tcstol(pBtns[iloop].pEntxtPos, &pstr, 10); ASSERT(pstr);
						btnInfo.textoffset.y = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						break;
					case LANGUAGE_ID_PORTUGUESE:
						btnInfo.text = pBtns[iloop].pPucontent;
						btnInfo.fontname = pBtns[iloop].pPufontName;
						btnInfo.fontsize = _tstol(pBtns[iloop].pPufontSize);
						btnInfo.textcolor = _tcstoul(pBtns[iloop].pPufontColor, &pstr, 16);
						btnInfo.textoffset.x = _tcstol(pBtns[iloop].pPutxtPos, &pstr, 10); ASSERT(pstr);
						btnInfo.textoffset.y = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						break;
					case LANGUAGE_ID_UNSPECIFIED: 
					default:
						break;
				}
				//normal pic
				CString relpath(TVM_GUI_PATH);
				US_SPECIALPICDATAS* pSpecialPic=getSpecialPicDatas(US_PIC_DATA::UE_PICCODE,pBtns[iloop].pBtnNormamPicCode);
				if(NULL!=pSpecialPic)
				{
					if(pSpecialPic->m_idataCount>=1)
					{
						btnInfo.pImageNormal=m_imageMgr.getImage(pSpecialPic->m_pDataPtr[0].pPicCode);
						//btnInfo.stationNormalBitmap=relpath + pSpecialPic->m_pDataPtr[0].pPicName;
						//btnInfo.stationNormalBitmap.TrimRight(_T("\r\n"));
					}
				}

				pSpecialPic=getSpecialPicDatas(US_PIC_DATA::UE_PICCODE,pBtns[iloop].pBtnHoverPicCode);
				if(NULL!=pSpecialPic)
				{
					if(pSpecialPic->m_idataCount>=1)
					{
						btnInfo.pImageHover=m_imageMgr.getImage(pSpecialPic->m_pDataPtr[0].pPicCode);
						//btnInfo.stationOnBitmap=relpath + pSpecialPic->m_pDataPtr[0].pPicName;
						//btnInfo.stationOnBitmap.TrimRight(_T("\r\n"));
					}
				}

				pSpecialPic=getSpecialPicDatas(US_PIC_DATA::UE_PICCODE,pBtns[iloop].pBtnPressedPicCode);
				if(NULL!=pSpecialPic)
				{
					if(pSpecialPic->m_idataCount>=1)
					{
						btnInfo.pImagePressed=m_imageMgr.getImage(pSpecialPic->m_pDataPtr[0].pPicCode);
						//btnInfo.stationDownBitmap=relpath + pSpecialPic->m_pDataPtr[0].pPicName;
						//btnInfo.stationDownBitmap.TrimRight(_T("\r\n"));
					}
				}

				pSpecialPic=getSpecialPicDatas(US_PIC_DATA::UE_PICCODE,pBtns[iloop].pBtnDisablePicCode);
				if(NULL!=pSpecialPic)
				{
					if(pSpecialPic->m_idataCount>=1)
					{
						btnInfo.pImageDisabled=m_imageMgr.getImage(pSpecialPic->m_pDataPtr[0].pPicCode);
						//btnInfo.stationDisableBitmap=relpath + pSpecialPic->m_pDataPtr[0].pPicName;
						//btnInfo.stationDisableBitmap.TrimRight(_T("\r\n"));
					}
				}
				btnInfos.push_back(btnInfo);
			}
			lret=0;
		}
	}else
	{
	}
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取当前配置文件中的主页底图配置信息

@param      LANGUAGETYPE_T languageType;
            AREA_LAYOUT	   homeLayout;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CGuiParm::GetHomePageLayout(LANGUAGETYPE_T languageType,AREA_LAYOUT& homeLayout){
	long lRet = UE_SUCCESS;
	if(m_ueLayOutRet == UE_SUCCESS){
		CCsvReader::CLineData* pLine=m_layOutCsv.getFirstLine();
		
		while(NULL != pLine){
			TCHAR* chIsHomePage = (*pLine)[US_LAYOUT_DATA::UE_ISHOMEPAGE];
			// 判断是否是首页
			if(_tcscmp(chIsHomePage,_T("1")) == 0){
				TCHAR* chLayoutID = (*pLine)[US_LAYOUT_DATA::UE_PAGECODE];// 取得当前页码
				TCHAR* chTemp = NULL;
				DWORD dwPageCode = _tcstoul(chLayoutID,&chTemp,10);

				getAreaLaylout(dwPageCode,languageType,homeLayout);
				break;
			}
			pLine = m_layOutCsv.getNextLine();
		}
	}
	else{
		lRet = UE_FAILOPEN;
	}
	return lRet;
}