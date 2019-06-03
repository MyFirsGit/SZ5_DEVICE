#ifndef _UPGRADEAPI_H_
#define _UPGRADEAPI_H_
/*****************************************************************************/
/*                                                                           */
/*    Copyright (C) - LEGATE Intelligent Equipment - All rights reserved     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  Except if expressly provided in a dedicated License Agreement, you are   */
/*  not authorized to:                                                       */
/*                                                                           */
/*  1. Use, copy, modify or transfer this software component, module or      */
/*  product, including any accompanying electronic or paper documentation    */
/*  (together, the "Software").                                              */
/*                                                                           */
/*  2. Remove any product identification, copyright, proprietary notices or  */
/*  labels from the Software.                                                */
/*                                                                           */
/*  3. Modify, reverse engineer, decompile, disassemble or otherwise attempt */
/*  to reconstruct or discover the source code, or any parts of it, from the */
/*  binaries of the Software.                                                */
/*                                                                           */
/*  4. Create derivative works based on the Software (e.g. incorporating the */
/*  Software in another software or commercial product or service without a  */
/*  proper license).                                                         */
/*                                                                           */
/*  By installing or using the "Software", you confirm your acceptance of the*/
/*  hereabove terms and conditions.                                          */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  History:                                                                 */
/*****************************************************************************/
/*  Date       * Author          * Changes                                   */
/*****************************************************************************/
/*  2014-11-13 * Ricky Gong      * Creation of the file                      */
/*             *                 *                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int uint;

#define ERROR_NONE                  0
#define ERROR_FAILURE               1
#define ERROR_RECV_ERR              -2   // receive error
#define ERROR_RESPONSE_FAILED       -3    // Communication timeout
#define ERROR_COMCONFIGURE          -4   // com open error



typedef enum 
{
    STATUS_DOWNLOAD_SUCCESS,
    STATUS_DOWNLOAD_START,
    STATUS_DOWNLOAD_ING,
    STATUS_DOWNLOAD_FAILED
} DownLoadStatus; // downLoad progress state

#if defined (__cplusplus)
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  Definitions                                                              */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*  Variable Declarations                                                    */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*  Function Declarations                                                    */
/*                                                                           */
/*****************************************************************************/
extern int FWDCommOpen(unsigned int uiCommPort, unsigned int BootuiBaudRate,unsigned int WorkuiBaudRate);

extern int FWDCommClose();

extern int FWDGetDLVer(char cDLVersion[32]);

extern int FWDownload(char* pFWFile);

extern int FWDGetProgress(int &trasferedPercentage);

#if defined (__cplusplus)
} // end of extern "C"
#endif
#endif //_UPGRADEAPI_H_
