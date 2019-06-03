#pragma once

#include "TxnDataEnum.h"
#include "TxnDataStruct.h"
#include <getini.h>
#include "TxnDataException.h"
#include <queue>
#include <map>
#include <afxmt.h>

bool		WritePekgFile(CString,LPBYTE,DWORD);
bool		IsFileExist(CString);
CString		GetPekgDate(CString);
void		ReadPekgFile(CString,PEKG_DATA&);	
bool		CreatePath(CString);
bool		IsFolderExist(CString);
void		GetUnsendFileName(CString,queue<CString>&);
CString		GetTodayDate();
unsigned int GetUnsendDayCount(CString);
//void		GetUnsendDay(unsigned int&);
//void		GetSendedDay(unsigned int&);	
unsigned int GetDirectoryCount(CString,vector<CString>&);
void		Removecate(CString strfolderPath);
void		ResumeUnsendData(DATA_TYPE_CODE dataType,CString strDataPath,CString strBackupPath);

