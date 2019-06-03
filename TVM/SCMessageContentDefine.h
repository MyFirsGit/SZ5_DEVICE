#pragma once
#include "stdafx.h"
#include "ParamHelper.h"
#include "scapi.h"
#include "SCSvcDef.h"

struct ParameterAndSoftwareUpdate
{
	PARAMETER_ID parameterID;
	DEVICE_PROVIDER providerID;
	WORD softwareBatchNO;
	PARAM_VERSION_TYPE_CODE versionType;
	DWORD version;
	_DATE startDate;
	CString downLoadPath;
	DWORD refrenceVersion;
	bool autoUpgrade;

	ParameterAndSoftwareUpdate(LPBYTE lpData)
	{
		short sParameterID;
		memcpy(&sParameterID,lpData,2);
		parameterID = (PARAMETER_ID)(mstos(sParameterID)&0xFFFF);
		lpData += 2;
		short sProviderID;
		memcpy(&sProviderID,lpData,2);
		providerID = (DEVICE_PROVIDER)sProviderID;
		lpData +=2;
		memcpy(&softwareBatchNO,lpData,2);
		lpData +=2;
		char cVersionType;
		memcpy(&cVersionType,lpData,1);
		versionType = (PARAM_VERSION_TYPE_CODE)cVersionType;
		lpData +=1;
		memcpy(&version,lpData,4);
		lpData +=4;
		char chStartDate[4];
		memcpy(&chStartDate,lpData,4);
		startDate = BCD2Date(chStartDate);
		lpData +=4;
		LPSTR pDownloadPath = (LPSTR)lpData;
		USES_CONVERSION;
		downLoadPath.SetString(A2T(pDownloadPath));
		lpData +=64;
		memcpy(&refrenceVersion,lpData,4);
		lpData +=4;
		autoUpgrade = ( *lpData == 0x00);
		lpData+=1;
	}
	ParameterAndSoftwareUpdate()
	{
		TCHAR emptyDownloadPath[64] = {'\0'};
		downLoadPath.SetString(emptyDownloadPath,64);
	}

	ParameterAndSoftwareUpdate* FromProgramFileName(CString& programFileName)
	{
		softwareBatchNO = _ttoi(thePARAM_HELPER.GetProgBatchNoFromProgName(programFileName));
		versionType = thePARAM_HELPER.GetProgVersionType(programFileName);
		version = _ttol(thePARAM_HELPER.GetProgVerFromProgName(programFileName));

		return this;
	}

	void Serialize(LPBYTE lpData)
	{
		short sParameterID = stoms(parameterID);
		memcpy(lpData,&sParameterID ,2);
		lpData +=2;
		short sProviderID = providerID;
		memcpy(lpData,&sProviderID,2);
		lpData +=2;
		memcpy(lpData,&softwareBatchNO,2);
		lpData +=2;
		memcpy(lpData,&versionType,1);
		lpData +=1;
		memcpy(lpData,&version,4);
		lpData +=4;
		BYTE chStartDate[4];
		Date2BCD(startDate,chStartDate);
		memcpy(lpData,chStartDate,4);
		lpData +=4;
		USES_CONVERSION;
		LPSTR lpDownloadPath = T2A((LPTSTR)downLoadPath.GetString());
		memcpy(lpData,lpDownloadPath,strlen(lpDownloadPath));
		lpData +=64;
		memcpy(lpData,&refrenceVersion,4);
		lpData +=4;
		*lpData = autoUpgrade ? 0x00:0x01;
	}

	bool SaveToFile(CString& fileName)
	{
		BYTE data[84];
		Serialize(data);
		CDXFile cFile(fileName,_T(""));
		cFile.WriteBinFile((char*)data,84);

		return true;
	}


};