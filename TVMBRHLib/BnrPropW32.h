/******************************************************************************
 *
 *  BNR SDK Project
 *
 ******************************************************************************
 *
 * File :   BnrPropW32.h
 * 
 * Author : MEI Inc.
 *
 * Date :   26.07.07
 *
 * History of modifications
 * Date       Rev.    Reason
 *
 ******************************************************************************
 * FILE DESCRIPTION *//** @file
 *  This file contains constants and types declaration for the BNR API for Win32.
 ***************************************************************************//*
 * 
 *  MEI
 *
 *      U.S.A.  1301 Wilson Drive, West Chester, Pennsylvania 19380
 *                      Tel: +1-610-430-2500
 *
 *      C.H.    Ch. Pont-du-Centenaire 109, Plan-les-Ouates, P.O. Box 2650
 *                      Tel: +41-22-884-0505
 *
 *  http://www.meigroup.com
 *
 ************************* Copyright (c) MEI 2007 *****************************
 *
 * The information contained here-in is the property of MEI and is not to be
 * disclosed or used without the prior written permission of MEI. This
 * copyright extends to all media in which this information may be preserved
 * including magnetic storage, computer print-out or visual display.
 *
 *****************************************************************************/

#ifndef _BNR_PROP_W32_H_
#define _BNR_PROP_W32_H_

#include "XfsConsts.h"
#ifdef WIN32

  #include "windows.h"

  #if (_MSC_VER < 1400)
    #ifndef UINT16
      typedef unsigned __int16 UINT16;
    #endif
    #ifndef UINT8
      typedef unsigned __int8 UINT8;
    #endif
    typedef __int64 T_Time64;
  #else
    typedef __time64_t T_Time64;
  #endif

#else

#include <sys/types.h>
typedef unsigned long UINT32;
typedef signed long INT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
#define LONG32 int32_t
typedef int BOOL;
typedef signed long long T_Time64;
typedef char CHAR;
typedef unsigned long DWORD;
#endif

#ifndef WIN32
#pragma pack(push,2)
#else
#include <pshpack2.h>
#endif

/** @addtogroup API_RESULT 
*/
//@{
#define BXR_NO_ERROR                    (0)  /**< Returned by synchronious method when there is no error */
#define BXR_USB_ERROR_OFFSET        (10000)  /**< Group of code returned by the USB driver */
#define BXR_BNR_EXCEPTION_OFFSET     (1000)  /**< Group of XFS error and exception codes returned by the BNR */
//@} API_RESULT

/** @addtogroup API_ERROR
*/
//@{
#define BXR_API_NOT_OPENED                (BXR_NO_ERROR + 0x01)          /**< The library was not opened by a previous bnr_Open() call */
#define BXR_API_FILE_NOT_FOUND            (BXR_NO_ERROR + 0x02)          /**< A file given in parameter can not be found */
#define BXR_API_NO_MEMORY                 (BXR_NO_ERROR + 0x03)          /**< Insufficient memory available */
#define BXR_API_INVALID_PARAMETER         (BXR_NO_ERROR + 0x04)          /**< One of the parameter given in the function call is invalid */
#define BXR_API_INVALID_HANDLE            (BXR_NO_ERROR + 0x05)          /**< The API was not able to get a valid hande from Windows system. */
#define BXR_API_MISSING_DEPENDENCY        (BXR_NO_ERROR + 0x06)          /**< The API was not able to delay-load something it needs (e.g. a DLL). */
#define BXR_API_ACCESS_DENIED             (BXR_NO_ERROR + 0x07)          /**< Acces of this command is denied. */
//@} API_ERROR

/** @addtogroup USB_ERROR
*/   
//@{
#define BXR_USB_CRC                     (BXR_USB_ERROR_OFFSET + 0x01)
#define BXR_USB_BTSTUFF                 (BXR_USB_ERROR_OFFSET + 0x02)
#define BXR_USB_DATA_TOGGLE_MISMATCH    (BXR_USB_ERROR_OFFSET + 0x03)
#define BXR_USB_STALL_PID               (BXR_USB_ERROR_OFFSET + 0x04)
#define BXR_USB_DEV_NOT_RESPONDING      (BXR_USB_ERROR_OFFSET + 0x05)
#define BXR_USB_PID_CHECK_FAILURE       (BXR_USB_ERROR_OFFSET + 0x06)
#define BXR_USB_UNEXPECTED_PID          (BXR_USB_ERROR_OFFSET + 0x07)
#define BXR_USB_DATA_OVERRUN            (BXR_USB_ERROR_OFFSET + 0x08)
#define BXR_USB_DATA_UNDERRUN           (BXR_USB_ERROR_OFFSET + 0x09)
#define BXR_USB_BUFFER_OVERRUN          (BXR_USB_ERROR_OFFSET + 0x0C)
#define BXR_USB_BUFFER_UNDERRUN         (BXR_USB_ERROR_OFFSET + 0x0D)
#define BXR_USB_NOT_ACCESSED            (BXR_USB_ERROR_OFFSET + 0x0F)
#define BXR_USB_FIFO                    (BXR_USB_ERROR_OFFSET + 0x10)
#define BXR_USB_ENDPOINT_HALTED         (BXR_USB_ERROR_OFFSET + 0x30)

#define BXR_USB_NO_MEMORY               (BXR_USB_ERROR_OFFSET + 0x0100)
#define BXR_USB_INVALID_URB_FUNCTION    (BXR_USB_ERROR_OFFSET + 0x0101)
#define BXR_USB_INVALID_PARAMETER       (BXR_USB_ERROR_OFFSET + 0x0102)
#define BXR_USB_ERROR_BUSY              (BXR_USB_ERROR_OFFSET + 0x0103)
#define BXR_USB_REQUEST_FAILED          (BXR_USB_ERROR_OFFSET + 0x0104)
#define BXR_USB_INVALID_PIPE_HANDLE     (BXR_USB_ERROR_OFFSET + 0x0105)
#define BXR_USB_NO_BANDWIDTH            (BXR_USB_ERROR_OFFSET + 0x0106)
#define BXR_USB_INTERNAL_HC_ERROR       (BXR_USB_ERROR_OFFSET + 0x0107)
#define BXR_USB_ERROR_SHORT_TRANSFER    (BXR_USB_ERROR_OFFSET + 0x0108)
#define BXR_USB_BAD_START_FRAME         (BXR_USB_ERROR_OFFSET + 0x0109)
#define BXR_USB_ISOCH_REQUEST_FAILED    (BXR_USB_ERROR_OFFSET + 0x010A)
#define BXR_USB_FRAME_CONTROL_OWNED     (BXR_USB_ERROR_OFFSET + 0x010B)
#define BXR_USB_FRAME_CONTROL_NOT_OWNED (BXR_USB_ERROR_OFFSET + 0x010C)

#define BXR_USB_CANCELED                (BXR_USB_ERROR_OFFSET + 0x0200)
#define BXR_USB_CANCELING               (BXR_USB_ERROR_OFFSET + 0x0201)
#define BXR_USB_ALREADY_CONFIGURED      (BXR_USB_ERROR_OFFSET + 0x0202)  /**< Device is already configured. */
#define BXR_USB_UNCONFIGURED            (BXR_USB_ERROR_OFFSET + 0x0203)  /**< Device is unconfigured. */
#define BXR_USB_NO_SUCH_DEVICE          (BXR_USB_ERROR_OFFSET + 0x0204)  /**< The specified Device doesn't exists. */
#define BXR_USB_DEVICE_NOT_FOUND        (BXR_USB_ERROR_OFFSET + 0x0205)  /**< The specified Device not found in system. */
#define BXR_USB_NOT_SUPPORTED           (BXR_USB_ERROR_OFFSET + 0x0206)  /**< Operation isn't supported by Driver */
#define BXR_USB_IO_PENDING              (BXR_USB_ERROR_OFFSET + 0x0207)  /**< I/O operation is still in progress */
#define BXR_USB_IO_TIMEOUT              (BXR_USB_ERROR_OFFSET + 0x0208)  /**< Request timeout */
#define BXR_USB_DEVICE_REMOVED          (BXR_USB_ERROR_OFFSET + 0x0209)  /**< The USB device was removed (power off or disconnected). */
#define BXR_USB_PIPE_NOT_LINKED         (BXR_USB_ERROR_OFFSET + 0x020A)  /**< Pipe not linked */
#define BXR_USB_CONNECTED_PIPES         (BXR_USB_ERROR_OFFSET + 0x020B)  /**< Device cannot be reconfigured because pipe connections already exist. */
#define BXR_USB_DEVICE_LOCKED           (BXR_USB_ERROR_OFFSET + 0x020C)  /**< The device is already locked by another application. */
//@} USB_ERROR

/** @ingroup API_RESULT
 * Type returned by all the BNR API functions. 
 *
 * For synchronous operations, the returned value can be :
 * - #BXR_NO_ERROR (0) - The operation has been completed successfully.
 * - A negative value - An error occured, see below.
 * . 
 * For asynchronous operations, the returned value can be :
 * - A strictly positive value - The required conditions to start the asynchronous 
 *   operation are ok, the value gives the identificationId for the operation.
 *   This identificationId changes on each asynchronous operation call.
 * - A negative value - An error occured, see below.
 * . 
 *
 * In case of error, returned value is negative, and its absolute value 
 * contains the error code. Errors are divided in 3 ranges :
 *  - From 1 to 1000 - API error code, see @ref API_ERROR.
 *  - From 1001 to 10000 - XFS Exception code, see @ref XFS_ERROR.
 *  - From 10001 - USB Exception, see @ref USB_ERROR.
 * .
 */
typedef INT32 T_BnrXfsResult;

/** @name Data Size
 * Data size
 */
//@{
#define NBMAXMODULES                  (16)  /**< Maximum of modules */
#define NBMAXCASHUNIT                  (8)  /**< Maximum of cashunit */
#define NBMAXCOMP                     (20)  /**< Maximum of components for the getIdentificationReturn structure */
#define NBMAXREQ                      (20)  /**< Maximum of requirements for the Components structure */
#define MAX_NR_OF_DENOMINATION        (61)  /**< Maximum of denomination in the BNR */
#define MAX_NR_OF_DENOMINATION_ITEMS  (20)  /**< Maximum of Items for the cashInOrder */
#define NBMAXELEMENTS                 (20)  /**< Maximum of elements */
#define MAX_NR_OF_STATUS_EVENTS       (20)  /**< Maximum of Status events in list. */
//@} Data Size

/**
 * Maximum number of Logical Cash Units.
 */
#define NBMAXLCU (83) 

/**
 * Maximum number of Physical Cash Units.
 */
#define NBMAXPCU (10) 

/** @addtogroup ID
 */
//@{

/**
 * Mask used to isolate the ClassType of an ID. 
 * @par Usage: 
 * @code
 *  if ((id & CLASS_TYPE_MASK) == CT_TAPE_POSITION_PROXY) { ... } 
 * @endcode 
 */
#define CLASS_TYPE_MASK (0x00FF0000)   

#define CLASS_TYPE_SHIFT (16)  /**< The number of right shifts to extract the ClassType. */

//Get classType in elementId
#define elementId_GetClassType(elementId) ((T_ClassType) (((elementId) & CLASS_TYPE_MASK) >> CLASS_TYPE_SHIFT)) /**< Returns the ClassType of an id. */

/** 
 * Types of the different existing classes in the BNR. 
 */
typedef enum ClassType {
  CT_UNKNOWN =                   0x00,  /**< Not used in the BNR. */
  CT_BASIC_MOTOR =               0x01,  /**< Class for Motors in the MainModule only. */
  CT_BUNDLER_DIV_SYSTEM =        0x02,  /**< Class for the Bundler Diverter System element. */
  CT_DETECTOR =                  0x03,  /**< Class for the Sensors. */
  CT_DIVERTER_PROXY =            0x04,  /**< Class for the Diverter elements in the Spine. */
  CT_RECOGNITION_SENSOR =        0x06,  /**< Class for the Recognition Sensor element. */
  CT_MOTOR_PROXY =               0x07,  /**< Class for Motors in the modules except for the MainModule. */
  CT_POSITIONER =                0x08,  /**< Class for the Positioner element. */
  CT_STACKER =                   0x0C,  /**< Class for the Stacker element. */
  CT_TAPE_POSITION_PROXY =       0x0D,  /**< Class for the Tape Position elements in the Recyclers. */
  CT_MODULE_INTERFACE =          0x0E,  /**< Class for BNR Modules, ie MainModule, Recycler, Loader, etc ... */
  CT_COVER =                     0x0F,  /**< Class for the Cover elements. */
  CT_COMPONENT =                 0x10,  /**< Class for the Tape Position elements in the Recyclers. */
  CT_PRESSURE_PLATE_PROXY =      0x11,  /**< Class for the Pressure Plate elements in the Loaders. */
  CT_FLAP =                      0x12,  /**< Class for the Flap elements. */
  CT_CASH_MODULES_LOCK =         0x1D,  /**< Class for the Cash Module Lock. */
  CT_CASHBOX_STACK_HEIGHT =      0x56,  /**< Class for the Cashbox Stack Height element. */
  CT_LOADER_STACK_HEIGHT_PROXY = 0x5B,  /**< @deprecated No more used. */
  CT_BASIC_FLAP =                0x58   /**< @deprecated No more used. */
} T_ClassType;
//@} ID


/** @addtogroup MODULE_ID
 */
//@{
#define BARCODE_READER_CLASS  (0x0E0700)  /**< Class identifier of the Barcode Reader. <b>Decimal value:</b> 919296 */
#define BUNDLER_CLASS         (0x0E0600)  /**< Class identifier of the Bundler. <b>Decimal value:</b> 919040 */
#define CASHBOX_CLASS         (0x0E0200)  /**< Class identifier of the Cashbox. <b>Decimal value:</b> 918016 */
#define CASHMODULELOCK_CLASS  (0x0E0500)  /**< Class identifier of the Cash Module Lock. <b>Decimal value:</b> 918784 */
#define LOADER_CLASS          (0x0E0400)  /**< Class identifier of the Loader. <b>Decimal value:</b> 918528 */
#define MAIN_MODULE_CLASS     (0x0E0000)  /**< Class identifier of the MainModule. <b>Decimal value:</b> 917504 */
#define RECYCLER_CLASS        (0x0E0300)  /**< Class identifier of the Recycler. <b>Decimal value:</b> 918272 */
#define SPINE_CLASS           (0x0E0100)  /**< Class identifier of the Spine. <b>Decimal value:</b> 917760 */

/**
 * Mask used to isolate the module class of a module ID. 
 * @par Usage: 
 * @code
 *  if ((id & MODULE_CLASS_MASK) == MAIN_MODULE_CLASS) { ... }
 * @endcode 
 */
#define MODULE_CLASS_MASK (0xFFFF00)

#define BARCODE_READER_ID (BARCODE_READER_CLASS)   /**< Identifier of the Barcode Reader. See @ref BARCODE_READER_CLASS for its decimal value. */
#define BUNDLER_ID        (BUNDLER_CLASS)          /**< Identifier of the Bundler. See @ref BUNDLER_CLASS for its decimal value. */
#define CASHBOX_ID        (CASHBOX_CLASS)          /**< Identifier of the Cashbox. See @ref CASHBOX_CLASS for its decimal value. */
#define CASHMODULELOCK_ID (CASHMODULELOCK_CLASS)   /**< Identifier of the Cash Module Lock. See @ref CASHMODULELOCK_CLASS for its decimal value. */
#define MAIN_MODULE_ID    (MAIN_MODULE_CLASS)      /**< Identifier of the MainModule. See @ref MAIN_MODULE_CLASS for its decimal value. */ 
#define MAX_RECYCLER_ID   (RECYCLER_CLASS + 0xFF)  /**< Last possible identifier of a Recycler in the BNR. <b>Decimal value:</b> 918527 */
#define MIN_RECYCLER_ID   (RECYCLER_CLASS + 0x01)  /**< First possible identifier of a Recycler in the BNR. <b>Decimal value:</b> 918273 */
#define MAX_LOADER_ID     (LOADER_CLASS + 0xFF)    /**< Last possible identifier of a Recycler in the BNR. <b>Decimal value:</b> 918783 */
#define MIN_LOADER_ID     (LOADER_CLASS + 0x01)    /**< First possible identifier of a Loader in the BNR. <b>Decimal value:</b> 918529 */
#define SPINE_ID          (SPINE_CLASS)            /**< Identifier of the Spine. See @ref SPINE_CLASS for its decimal value. */ 

#define moduleId_IsRecyclerClass(id) (((id) & MODULE_CLASS_MASK) == RECYCLER_CLASS) /**< Returns TRUE if the id corresponds to a Recycler. */
#define moduleId_IsLoaderClass(id)   (((id) & MODULE_CLASS_MASK) == LOADER_CLASS)   /**< Returns TRUE if the id corresponds to a Loader. */

#define MAX_NR_OF_MODULES (13)  /**< Maximum number of modules in the BNR. */
typedef UINT32 T_ModuleId;
//@} MODULE_ID

/** @addtogroup ELEMENT_ID
 */
//@{
typedef UINT32 T_ElementId;
#define BUNDLER_DIV_SYSTEM_ID    (0x020000)                           /**< Identifier of the BundlerDivSystem. <b>Decimal value:</b> 131072  */ 
#define RECOGNITION_SENSOR_ID    (0x060000)                           /**< Identifier of the Recognition sensor. <b>Decimal value:</b> 393216 */ 
#define POSITIONER_ID            (0x080000)                           /**< Identifier of the Positioner. <b>Decimal value:</b> 524288 */
#define STACKER_ID               (0x0C0000)                           /**< Identifier of the Stacker. <b>Decimal value:</b> 786432 */
#define CASHBOX_STACK_HEIGHT_ID  (0x560000)                           /**< Identifier of the CashBox Stack Height sensor. This sensor provides an estimation of the filling level of the cashbox. \n\n <b>Decimal value:</b> 5636096 */
#define SPINE_DIVERTER_CLASS     (0x040000)                           /**< Class identifier of the Spine diverters. <b>Decimal value:</b> 262144 */ 
#define MIN_SPINE_DIVERTER_ID    (SPINE_DIVERTER_CLASS + 0x01)        /**< First possible identifier of a Diverter in the Spine. The last 2 hexadecimal digits are the Diverter index. It matches the position in the Spine. \n\n #MIN_SPINE_DIVERTER_ID corresponds to the Diverter in position 1. \n\n <b>Decimal value:</b> 262145 */ 
#define MAX_SPINE_DIVERTER_ID    (SPINE_DIVERTER_CLASS + NBMAXPCU)    /**< Last possible identifier of a Diverter in the Spine. <b>Decimal value:</b> 262155 */ 
#define TAPE_POSITION_CLASS      (0x0D0000)                           /**< Class identifier of the Tape Position elements in the Recyclers. <b>Decimal value:</b> 851968 */ 
#define MIN_TAPE_POSITION_ID     (TAPE_POSITION_CLASS + 0x01)         /**< First possible identifier of a Tape Position in a Recycler. The last 2 hexadecimal digits are the Recycler index. It matches the position in the Spine. \n\n #MIN_TAPE_POSITION_ID corresponds to the Tape Position of the Recycler in position 1. \n\n <b>Decimal value:</b> 851969 */ 
#define MAX_TAPE_POSITION_ID     (TAPE_POSITION_CLASS + NBMAXPCU)     /**< Last possible identifier of a Tape Position in a Recycler. <b>Decimal value:</b> 851979 */ 
#define PRESSURE_PLATE_CLASS     (0x110000)                           /**< Class identifier of the Pressure Plate elements in the Loaders. <b>Decimal value:</b> 1114112 */
#define MIN_PRESSURE_PLATE_ID    (PRESSURE_PLATE_CLASS + 0x01)        /**< First possible identifier of a Pressure Plate in a Loader. The last 2 hexadecimal digits are the Loader index. It matches the position in the Spine. \n\n #MIN_PRESSURE_PLATE_ID corresponds to the Pressure Plate of the Loader in position 1. \n\n <b>Decimal value:</b> 1114113 */ 
#define MAX_PRESSURE_PLATE_ID    (PRESSURE_PLATE_CLASS + NBMAXPCU)    /**< Last possible identifier of a Pressure Plate in a Loader. <b>Decimal value:</b> 1114123 */ 
#define ANTIFISHING_FLAP_CLASS   (0x120000)                           /**< Class identifier of the Anti-fishing flaps in the Loaders. <b>Decimal value:</b> 1179648 */ 
#define MIN_ANTIFISHING_FLAP_ID  (ANTIFISHING_FLAP_CLASS + 0x01)      /**< First possible identifier of an Anti-fishing Flap in a Loader. The last 2 hexadecimal digits are the Loader index. It matches the position in the Spine. \n\n #MIN_ANTIFISHING_FLAP_ID corresponds to the Anti-fishing Flap of the Loader in position 1. \n\n <b>Decimal value:</b> 1179649 */ 
#define MAX_ANTIFISHING_FLAP_ID  (ANTIFISHING_FLAP_CLASS + NBMAXPCU)  /**< Last possible identifier of an Anti-fishing Flap in a Loader. <b>Decimal value:</b> 1179659 */ 
#define OUTLET_FLAP_ID           (0x120100)                           /**< Identifier of the Outlet Flap. <b>Decimal value:</b> 1179904 */ 
//@} ELEMENT_ID

/** @addtogroup MOTOR_ID
 */
//@{
#define THREE_WAY_DIVERTER_MOTOR       (0x010100)                           /**< Motor identifier of the 3 Way Diverter. To locate this motor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 65792 */
#define BUNDLER_TRANSPORT_MOTOR        (0x010200)                           /**< Transport Motor identifier of the Bundler. To locate this motor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 66048 */
#define MAIN_TRANSPORT_MOTOR           (0x010201)                           /**< Main Transport Motor identifier. To locate this motor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 66049 */
#define STACKER_MOTOR                  (0x010202)                           /**< Motor identifier of the Stacker. To locate the Stacker, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 66050 */
#define BILL_INTAKE_MOTOR              (0x010300)                           /**< Bill Intake Motor identifier. To locate this motor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 66304 */
#define BUNDLER_DIVERTER_SYSTEM_MOTOR  (0x010400)                           /**< Motor identifier of the Bundler Diverter System. To locate the Bundler Diverter System, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 66560 */
#define MOTOR_PROXY_CLASS              (0x070000)                           /**< Class identifier of a Motor in a Module. <b>Decimal value:</b> 458752 */ 
#define MIN_RECYCLER_MOTOR_ID          (MOTOR_PROXY_CLASS + 0x01)           /**< First possible identifier of a Motor in a Recycler. \n #MIN_RECYCLER_MOTOR_ID corresponds to the Motor of the Recycler in position 1. \n\n <b>Decimal value:</b> 458753 */ 
#define MAX_RECYCLER_MOTOR_ID          (MOTOR_PROXY_CLASS + NBMAXPCU)       /**< Last possible identifier of a Motor in a Recycler. <b>Decimal value:</b> 458762 */ 
#define MIN_LOADER_MOTOR_ID            (MOTOR_PROXY_CLASS + 0x32)           /**< First possible identifier of a Motor in a Loader. \n #MIN_LOADER_MOTOR_ID corresponds to the Motor of the Loader in position 1. \n\n <b>Decimal value:</b> 458802 */ 
#define MAX_LOADER_MOTOR_ID            (MIN_LOADER_MOTOR_ID + NBMAXPCU - 1) /**< Last possible identifier of a Motor in a Loader. <b>Decimal value:</b> 458811 */ 
//@} MOTOR_ID

/** @addtogroup SENSOR_ID
 */
//@{
#define RECOGNITION_SENSOR_BILL_SENSOR_UP                 (0x030300)  /**< Bill Sensor identifier of the Recognition Bill Sensor, upper part. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197376. */
#define RECOGNITION_SENSOR_BILL_SENSOR_DOWN               (0x030301)  /**< Bill Sensor identifier of the Recognition Bill Sensor, lower part. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197377. */
#define BUNDLER_DIVERTER_SYSTEM_DIVERTER_POSITION_SENSOR  (0x030400)  /**< Diverter Position Sensor identifier of the Bundler Diverter System. To locate the Bundler Diverter System, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197632. */
#define BUNDLER_DIVERTER_SYSTEM_ANGULAR_POSITION_SENSOR   (0x030401)  /**< Angular Position Sensor identifier of the Bundler Diverter System. To locate the Bundler Diverter System, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197633. */
#define STACKER_HOME_POSITION_SENSOR                      (0x030402)  /**< Home Position Sensor identifier of the Stacker. To locate the Stacker, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197634. */
#define RECOGNITION_SENSOR_COVER                          (0x0F0000)  /**< Sensor identifier of Recognition Sensor Cover. To locate the Recognition Sensor Cover, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 983040. */
#define BILL_INTAKE_COVER                                 (0x0F0001)  /**< Sensor identifier of Bill Intake Cover. To locate the Bill Intake Cover, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 983041. */
#define SPINE_COVER                                       (0x0F0002)  /**< Sensor identifier of Spine Cover. To locate the Spine Cover, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 983042. */
#define INLET_BILL_SENSOR                                 (0x030200)  /**< Identifier of the Inlet Bill Sensor. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197120. */
#define XCHANNEL_DETECTOR                                 (0x030600)  /**< Identifier of the XChannel Sensor. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198144. */
#define BILL_WIDTH_SENSOR                                 (0x030201)  /**< Identifier of the Bill Width Sensor. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197121. */
#define BILL_INTAKE_BILL_SENSOR                           (0x030105)  /**< Bill Sensor identifier of the Bill Intake. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 196869. */
#define POSITIONER_BILL_SENSOR                            (0x030100)  /**< Bill Sensor identifier of the Positioner. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 196864. */
#define BOTTOM_TRANSPORT_BILL_SENSOR                      (0x030107)  /**< Bill Sensor identifier of the Bottom Transport. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 196871. */
#define OUTLET_BILL_SENSOR                                (0x030108)  /**< Bill Sensor identifier of the Outlet. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 196872. */
#define BILL_AND_DRUM_SENSOR                              (0x030106)  /**< Bill and Drum Sensor identifier. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 196870. */
#define SPINE_BILL_SENSOR_1                               (0x030702)  /**< Bill Sensor 1 identifier of the Spine. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198402. */
#define SPINE_BILL_SENSOR_3                               (0x030703)  /**< Bill Sensor 3 identifier of the Spine. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198403. */
#define SPINE_BILL_SENSOR_5                               (0x030704)  /**< Bill Sensor 5 identifier of the Spine. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198404. */
#define SPINE_BILL_SENSOR_7                               (0x030705)  /**< Bill Sensor 7 identifier of the Spine. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198405. */
#define SPINE_BILL_SENSOR_9                               (0x030706)  /**< Bill Sensor 9 identifier of the Spine. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198406. */
#define RECYCLER_1_BILL_SENSOR                            (0x030732)  /**< Bill Sensor identifier of the Recycler 1. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198450. */
#define RECYCLER_2_BILL_SENSOR                            (0x030733)  /**< Bill Sensor identifier of the Recycler 2. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198451. */
#define RECYCLER_3_BILL_SENSOR                            (0x030734)  /**< Bill Sensor identifier of the Recycler 3. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198452. */
#define RECYCLER_4_BILL_SENSOR                            (0x030735)  /**< Bill Sensor identifier of the Recycler 4. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198453. */
#define RECYCLER_5_BILL_SENSOR                            (0x030736)  /**< Bill Sensor identifier of the Recycler 5. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198454. */
#define RECYCLER_6_BILL_SENSOR                            (0x030737)  /**< Bill Sensor identifier of the Recycler 6. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198455. */
#define RECYCLER_7_BILL_SENSOR                            (0x030738)  /**< Bill Sensor identifier of the Recycler 7. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198456. */
#define RECYCLER_8_BILL_SENSOR                            (0x030739)  /**< Bill Sensor identifier of the Recycler 8. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198457. */
#define RECYCLER_9_BILL_SENSOR                            (0x03073A)  /**< Bill Sensor identifier of the Recycler 9. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198458. */
#define RECYCLER_10_BILL_SENSOR                           (0x03073B)  /**< Bill Sensor identifier of the Recycler 10. To locate this sensor, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 198459. */
//@} SENSOR_ID

/** @addtogroup COMPONENT_ID
 */
//@{
typedef UINT32  T_ComponentId;
#define COMPONENT_CLASS_MASK (0x00FF00)  /**< Mask used to isolate the component class of a component ID. */
#define COMPONENT_TYPE_SHIFT (8)         /**< The number of right shifts to extract the ComponentType. */

#define SOFTWARE_CLASS (0x00)  /**< An identifier for a software component type. */
#define HARDWARE_CLASS (0x80)  /**< An identifier for a hardware component type. */

#define BASIC_BOOT_ID  (SOFTWARE_CLASS)         /**< Basic boot component type. <b>Full id value:</b> 1048576 or 0x100000 */
#define MAIN_BOOT_ID   (SOFTWARE_CLASS + 0x10)  /**< Main boot component type. <b>Full id value:</b> 1052672 or 0x101000 */
#define FPGA_ID        (SOFTWARE_CLASS + 0x30)  /**< FPGA component type. <b>Full id value:</b> 1060864 or 0x103000 */
#define APPLICATION_ID (SOFTWARE_CLASS + 0x20)  /**< Application component type. <b>Full id value:</b> 1056768 or 0x102000 */
#define BILLSET_ID     (SOFTWARE_CLASS + 0x40)  /**< ALGOS component type used with billset. <b>Full id value:</b> 1064960 or 0x104000 */

#define CPU_BOARD_ID                (HARDWARE_CLASS)         /**< CPU Board component type. <b>Full id value:</b> 1081344 or 0x108000 */
#define SENSOR_BOARD_ID             (HARDWARE_CLASS + 0x10)  /**< Sensor Board component type. <b>Full id value:</b> 1085440 or 0x109000 */
#define RECOGNITION_SENSOR_BOARD_ID (HARDWARE_CLASS + 0x20)  /**< Recognition Sensor Board component type. <b>Full id value:</b> 1089536 or 0x10A000 */
//@} COMPONENT_ID

/**
 * Counter type that never loop to zero. It stays at its maximum value if reached.
 */
typedef UINT32 T_LongAbsoluteCount;
typedef UINT16 T_AbsoluteCount;

/**
 * Counter type that loop to zero when its maximum value is reached.
 */
typedef UINT16 T_FreeRunningCount;

/** @ingroup BNR_SYSTEM_STATUS 
 * Filling status of a cash unit.
 *
 * How the threshold status of a cash unit is determined, depends of the threshold mode :
 *
 * @par SensorMode (default value):
 * ThresholdStatus changes are determined by the sensors of the BNR (physical filling status).
 *
 * @par CountMode:
 * The management of these values depends of the physical cash unit type :
 *  - Cashbox, Recycler and Bundler - Based on the Threshold levels, but if the physical limit of filling is reached before the threshold Full, then the ThresholdStatus is forced to Full anyway.
 *  - Loader - Based on the Threshold levels, but if there is a lack of bills before the threshold Empty, the ThresholdStatus is forced to Empty anyway.
 *
 * @see T_XfsThreshold, T_ThresholdMode, T_XfsPhysicalCashUnit.
 */
typedef enum XfsThresholdStatus {
  XFS_S_BIN_OK            = 0,                                  /**< The cash unit is neither empty nor full. */
  XFS_S_BIN_FULL          = 1,                                  /**< The cash unit is full. In CountMode, the cash unit count is higher or equal to its full threshold level. */
  XFS_S_BIN_HIGH          = 2,                                  /**< The cash unit is alomst full. In CountMode, the cash unit count is higher than its high threshold level. */
  XFS_S_BIN_LOW           = 4,                                  /**< The cash unit is almost empty. In CountMode, the cash unit count is lower than its low threshold level. */
  XFS_S_BIN_EMPTY         = 8,                                  /**< The cash unit is empty. In CountMode, the cash unit count is lower or equal to its empty threshold level. */
  XFS_S_BIN_UNKNOWN       = 16,                                 /**< Threshold state cannot be determined. */
  XFS_S_BIN_NOTSUPPORTED  = 32                                  /**< Threshold state is not supported. */
} T_XfsThresholdStatus;

/**
 * Maximum size for a Physical Cash Unit name (2 letters abbreviation + 2 integer + 0 terminal).
 * 
 * @see T_XfsPcuName.
 */
#define SIZE_OF_PHYSICAL_NAME (5)

/** @ingroup BNR_CASH_UNITS
 * String for a Physical Cash Unit name.
 * 
 * @see T_XfsPhysicalCashUnit.
 */
typedef char T_XfsPcuName[SIZE_OF_PHYSICAL_NAME];

/**
 * Maximum size for a XFS unit id.
 * 
 * @see T_XfsUnitId.
 */
#define SIZE_OF_UNIT_ID (20)

/**
 * Maximum size for a module description.
 * 
 * @see T_ModuleDescription.
 */
#define SIZE_OF_MODULE_DESCRIPTION (50)

/**
 * Maximum size for module user data.
 * 
 * @see T_ModuleUserData
 */
#define SIZE_OF_MODULE_USER_DATA (255)

/**
 * String for a XFS unit id, and readable through module_GetIdentification().
 * 
 * @see T_XfsPhysicalCashUnit, T_XfsLogicalCashUnit, T_ModuleIdentification.
 */
typedef char T_XfsUnitId[SIZE_OF_UNIT_ID];

/** @ingroup MODULE_CONFIG
 * String for module description, and readable through module_GetIdentification().
 */
typedef char T_ModuleDescription[SIZE_OF_MODULE_DESCRIPTION];

/** @ingroup MODULE_CONFIG
 * String for module user data, settable through module_SetIdentification(), and readable through module_GetIdentification().
 */
typedef char T_ModuleUserData[SIZE_OF_MODULE_USER_DATA];


/** @addtogroup MODULE_STATUS 
 */

//@{
/** Bill Storage Status */
typedef enum BillStorageStatus {
  BSS_NO_ERROR =        0,  /**< No error. */
  BSS_ALMOST_EMPTY =    1,  /**< The physical cash unit bill count is lower than the low threshold value. */
  BSS_EMPTY =           2,  /**< The physical cash unit is empty. */
  BSS_ALMOST_FULL =     3,  /**< The physical cash unit bill count is higher than the high threshold value. */
  BSS_FULL =            4,  /**< The physical cash unit bill count is full. */
  BSS_NOT_TESTED =      5,  /**< @deprecated No more used. */
  BSS_WRONG_DENO =      6,  /**< @deprecated No more used. */
  BSS_BILLS_NOT_VALID = 7   /**< @deprecated No more used. */
} T_BillStorageStatus;

/** Bundler Error Code */
typedef enum BundlerErrorCode {
  BUEC_NO_ERROR =               0,  /**< No error. */
  BUEC_CANNOT_FIND_MARK =       1,  /**< The bundler could not find the position mark during initialization cycle. */
  BUEC_CANNOT_INIT_WITH_BILLS = 2,  /**< The bundler initialization cycle could not be done because it contains bills. */
  BUEC_INIT_REQUIRED =          3,  /**< After a cycle, the position mark has not been detected. */
} T_BundlerErrorCode;

/** Bundler Functional Status. */
typedef enum BundlerFunctionalStatus {
  BUFS_UNKNOWN =     0,
  BUFS_POSITIONNED = 1,
} T_BundlerFunctionalStatus;

/** Bundler Div System Functional Status */
typedef enum BundlerDivSystemFunctionalStatus {
  BDFS_UNKNOWN =             0,
  BDFS_BUNDLER_N_POSITION =  1,
  BDFS_OUTLET_POSITION =     2,
  BDFS_TRASHBIN_POSITION =   3,
  BDFS_EXTRACTION_POSITION = 4,
} T_BundlerDivSystemFunctionalStatus;

/** Bundler Div System Error Code */
typedef enum BundlerDivSystemErrorCode {
  BDEC_NO_ERROR =                     0,  /**< No error. */
  BDEC_BLOCKED =                      1,  /**< The bundler div system motor speed was null or too low. */
  BDEC_CANNOT_POSITION =              2,  /**< The bundler div system failed to reach te requested position. */
  BDEC_EXTRACTION_POS_NOT_CONFIRMED = 3,  /**< The bundler div system position confirmation sensor (BU.S3) should have been covered on extraction position. */
  BDEC_OUTLET_POS_NOT_CONFIRMED =     4,  /**< The bundler div system position confirmation sensor (BU.S3) should have been covered on outlet position. */
  BDEC_CANNOT_FIND_MARK =             5,  /**< The bundler div system could not find the position mark during initialization cycle. */
} T_BundlerDivSystemErrorCode;

/** Cashbox stack height functional status. */
typedef enum CashboxStackHeightFunctionalStatus {
  CSHFS_UNKNOWN,
  CSHFS_OK,
  CSHFS_LAST_BUNDLE,
  CSHFS_MAX_HEIGHT,
  CSHFS_HIGH
} T_CashboxStackHeightFunctionalStatus;

/**
 * Cashbox stack height error code.
 * @deprecated No more used (2011-06-17).
 */
typedef enum CashboxStackHeightErrorCode {
  CSHEC_NO_ERROR,
  CSHEC_SENSOR_STAYS_COVERED,
} T_CashboxStackHeightErrorCode;

/** Cover Functional Status. */
typedef enum CoverFunctionalStatus {
  CFS_UNKNOWN,  /**< The status of the cover can not be determined. */
  CFS_OPENED,   /**< The cover is opened. */
  CFS_CLOSED    /**< The cover is closed. */
} T_CoverFunctionalStatus;

/** Sensor Functional Status. */
typedef enum SensorFunctionalStatus {
  DFS_UNKNOWN =           0,  /**< The status of the sensor can not be determined. */
  DFS_UNCOVERED =         1,  /**< The sensor is uncovered. */
  DFS_COVERED =           2,  /**< The sensor is covered. */
  DFS_PARTIALLY_COVERED = 3,  /**< The sensor is partially covered (BI.S1 and BI.S3 only). */
} T_SensorFunctionalStatus;

/** Detector Error Code. */
typedef enum SensorErrorCode {
  DEC_NO_ERROR =       0,  /**< No error. */
  DEC_SIGNAL_HIGH =    1,  /**< Detector could not be regulated because signal is too high. */
  DEC_SIGNAL_LOW =     2,  /**< Detector could not be regulated because signal is too low. */
  DEC_COM_BREAKDOWN =  3,  /**< No communication with the module that owns the sensor. */
  DEC_BAD_REGULATION = 4,  /**< Transitory error code, sensor regulation attempts are made (bundler's sensor only). */
} T_SensorErrorCode;

/** Diverter Functional Status. */
typedef enum DiverterFunctionalStatus {
  DIFS_UNKNOWN =         0,  /**< The status of the diverter can not be determined. */
  DIFS_SPINE_POSITION =  1,  /**< The diverter is spine position. */
  DIFS_MODULE_POSITION = 2,  /**< The diverter is module position. */
} T_DiverterFunctionalStatus;

/** Diverter Error Code. */
typedef enum DiverterErrorCode {
  DIEC_NO_ERROR =              0,  /**< No error. */
  DIEC_CANNOT_MOVE_TO_SPINE =  1,  /**< The diverter failed to move to spine position. */
  DIEC_CANNOT_MOVE_TO_MODULE = 2,  /**< The diverter failed to move to module position. */
  DIEC_CANNOT_STAY_TO_MODULE = 3,  /**< The diverter failed to stay on module position. */
  DIEC_COM_BREAKDOWN =         4,  /**< No communication with the module that owns the diverter. */
} T_DiverterErrorCode;

/** Flap Error Code. */
typedef enum FlapErrorCode {
  FEC_NO_ERROR,       /**< No error. */
  FEC_STAYS_CLOSED,   /**< The flap stays closed. */
  FEC_COM_BREAKDOWN,  /**< No communication with the flap. */
  FEC_STAYS_OPENED    /**< The flap stays opened. */
} T_FlapErrorCode;

/** Flap Functional Status. */
typedef enum FlapFunctionalStatus {
  FFS_UNKNOWN,  /**< The status can not be determined. */
  FFS_CLOSED,   /**< The flap is closed. */
  FFS_OPENED    /**< The flap is openned. */
} T_FlapFunctionalStatus;

/** Recognition Sensor Error Code. */
typedef enum RecognitionSensorErrorCode {
  RSEC_NO_ERROR,                       /**< No error. */
  RSEC_OFFSET_COMPENSATION_ERROR,      /**< Impossible to compensate the offset */
  RSEC_REMAINING_OFFSET_TOO_LOW,       /**< The remaining offset is too low. */
  RSEC_REMAINING_OFFSET_TOO_HIGH,      /**< The remaining offset is too high. */
  RSEC_CANNOT_REGULATE_FOR_DSR_IDENT,  /**< Impossible to regulate for identification. */
  RSEC_C_COEF_DIV_BY_ZERO,             /**< Divide by zero while calculating C coefficients. */
  RSEC_C_COEF_NEG_VALUE,               /**< One or more C coefficients are below zero. */
  RSEC_C_COEF_OVERFLOW,                /**< Overflow on C coefficients. */
  RSEC_C_COEF_OUT_OF_RANGE,            /**< One or more C coefficient values are out of range. */
  RSEC_CANNOT_REGULATE                 /**< Impossible to regulate the sensor. */
} T_RecognitionSensorErrorCode ;

/** Loader Functional Status */
typedef enum LoaderFunctionalStatus {
  PFS_UNKNOWN =     0,
  PFS_POSITIONNED = 1,
  PFS_PREFEEDED =   2,
} T_LoaderFunctionalStatus;

/** Loader stack height functional status. */
typedef enum LoaderStackHeightFunctionalStatus {
  LSHFS_UNKNOWN,
  LSHFS_OK,
  LSHFS_MIN_HEIGHT,
} T_LoaderStackHeightFunctionalStatus;

/** Main Module Error Code. */
typedef enum MainModuleErrorCode {
  MMEC_NO_ERROR =        0,  /**< No error. */
  MMEC_INCOMPATIBLE_SW = 1,  /**< Software not compatible with one of the Main Module component. */
  MMEC_OPENED =          2,  /**< Module is open. */
  MMEC_BOOT_RUNNING =    3   /**< The boot application is running. There may be 2 reasons : - mmApplication is not available. - mmMainBoot has to be tested after a new firmware update. Just reboot again the BNR to start mmApplication. */
} T_MainModuleErrorCode;

/** 
 * Module Error Code.
 *
 * Applicable to all BNR modules, except the Main Module, and the Bundler Module.
 */
typedef enum ModuleErrorCode {
  MODEC_NO_ERROR,               /**< No error. */
  MODEC_COM_BREAKDOWN,          /**< No communication with the module. */
  MODEC_INCOMPATIBLE_MODULE,    /**< The firmware of this module is incompatible with the Main Module. */
  MODEC_MISSING_MODULE,         /**< No module present in the system. */
  MODEC_OPENED,                 /**< The module is open; for Spine module only. */
  MODEC_WRONG_MODULE_TYPE,      /**< The type of the module is not compatible with the BNR configuration. */
  MODEC_BOOT_RUNNING,           /**< Module working with the boot application. */
  MODEC_CANNOT_EXTRACT_BILL,    /**< @deprecated No more used. */
  MODEC_INCOMPATIBLE_SOFTWARE,  /**< @deprecated No more used. */
  MODEC_MODULE_NOT_DETECTED,    /**< The module has been removed during an exchange operation. Can happen only when the cash module lock is open; for Cashbox module only. */
  MODEC_SHUTTER_CLOSED,         /**< The Cashbox shutter is closed and cannot be opened by the BNR; for Cashbox module only. */
  MODEC_NO_COM,                 /**< A smartCashbox is configured but no communication is detected when the cash modules lock is open; for Cashbox module only.*/
  MODEC_SIGNAL_TOO_HIGH,        /**< Sensor signal is too high; for BarcodeReader module only.*/
  MODEC_SIGNAL_TOO_LOW          /**< Sensor signal is too low; for BarcodeReader module only.*/
} T_ModuleErrorCode;

/** Motor Functional Status. */
typedef enum MotorFunctionalStatus {
  MFS_UNKNOWN,
  MFS_STOPPED,   
  MFS_RUNNING
} T_MotorFunctionalStatus;

/** Motor Error Code. */
typedef enum MotorErrorCode {
  MEC_NO_ERROR =      0,  /**< No error. */
  MEC_COM_BREAKDOWN = 1,  /**< No communication with the module that owns the motor. */
  MEC_NOT_STARTED =   2,  /**< Motor failed to start. */
  MEC_SPEED_TO_LOW =  3   /**< Motor speed low limit has been reached. */
} T_MotorErrorCode;

/** Operational Status for module's elements. */
typedef enum OperationalStatus {
  OS_OPERATIONAL,           /**< The element is operational - No errors. */
  OS_CHECKING_ERROR,        /**< Can not determine operational state of the element - Will be determinated on next test. */
  OS_NOT_OPERATIONAL,       /**< Element not operational - Element or sub-element part has an error. */
  OS_OPERATIONAL_DEGRADED,  /**< For future use. */
  OS_SW_LOCKED              /**< The module is locked. Use module_ConfigureCashUnit() to unlock the corresponding PhysicalCashUnit. */
} T_OperationalStatus;

/** CashType Status */
typedef enum CashTypeStatus {
  CTS_UNKNOWN,         /**< Can not determine the CashType status of the module. */
  CTS_OK,              /**< The CashType status is OK - No errors. */
  CTS_WRONG_SETTING,   /**< The CashType in the module does not correspond to its LogicalCashUnit's CashType. */
  CTS_WRONG_BILLS,     /**< The bills in the module are different than the CashType defines in the module. */
  CTS_BILLS_NOT_VALID  /**< The bills in the module correspond to its CashType, but are not recognized by the system. */
} T_CashTypeStatus;

/** Positioner Functional Status. */
typedef enum PositionerFunctionalStatus {
  POS_UNKNOWN,
  POS_POSITIONING,
  POS_TRANSPORT,
} T_PositionerFunctionalStatus;

/** Positioner Error Code. */
typedef enum PositionerErrorCode {
  PEC_NO_ERROR,                   /**< No error. */
  PEC_CANNOT_MOVE_TO_POSITIONING  /**< Cannot move to positioning position. */
} T_PositionerErrorCode;

/** Pressure Plate Error Code. */
typedef enum PressurePlateErrorCode {
  PPEC_NO_ERROR,       /**< No error. */
  PPEC_COM_BREAKDOWN,  /**< No communication with the module that owns the plate. */
  PPEC_PLATE_BLOCKED,  /**< The plate is blocked. */
  PPEC_PLATE_TOO_LOW,  /**< The plate position is too low. */
  PPEC_FULL            /**< The plate is in full position. */
} T_PressurePlateErrorCode;

/** Pressure Plate Functional Status. */
typedef enum PressurePlateFunctionalStatus {
  PPFS_UNKNOWN =     0,  /**< The status can not be determined. */
  PPFS_POSITIONNED = 1,  /**< The plate is positioned. */
  PPFS_MOVED_DOWN =  2,  /**< The plate is in low position. */
} T_PressurePlateFunctionalStatus;

/** Recycler Functional Status. */
typedef enum RecyclerFunctionalStatus {
  RFS_UNKNOWN = 0,      /**< The status can not be determined. */
  RFS_POSITIONNED = 1,  /**< The recycler is positioned. */
  RFS_PARKING = 2,      /**< The recycler is moving to parked state. */
  RFS_PARKED = 3        /**< The recycler is parked. */
} T_RecyclerFunctionalStatus;

/** Stacker Functional Status. */
typedef enum StackerFunctionalStatus {
  SFS_UNKNOWN =           0,
  SFS_HOME_POSITION =     1,
  SFS_TRASHBIN_POSITION = 2,
} T_StackerFunctionalStatus;

/** Stacker Error Code. */
typedef enum StackerErrorCode {
  STEC_NO_ERROR =                0,  /**< No error. */
  STEC_CANNOT_POSITION =         1,  /**< ST.S2 sensor did not change while moving around home position. */
  STEC_CANNOT_MOVE_TO_TRASHBIN = 2,  /**< The stacker motor was blocked while trying to move to trashbin position from its home position. The stacker is still in home position. */
  STEC_HARDWARE_FAILURE =        3,  /**< The stacker sensor ST.S2 not covered after some steps. */
  STEC_BLOCKED_AT_HOME =         4,  /**< The stacker motor did not start. The stacker is still in home position. */
  STEC_BLOCKED_AT_BILL_PATH =    5,  /**< While moving up, the stacker was blocked at the bill path position. */
  STEC_BLOCKED_AT_CB_ENTRY =     6,  /**< While moving up, the stacker was blocked at the cashbox entry. */
  STEC_BLOCKED_AROUND_HOME =     7,  /**< While moving up, the stacker was blocked around its home position. */
  STEC_BLOCKED_IN_CB =           8,  /**< While moving up, the stacker was blocked inside the cashbox. */
  STEC_STOPPED_AT_BILL_PATH =    9,  /**< While moving down, the stacker was stopped at the bill path position. BNR tries to move back the stacker in home position before declaring this error. */
  STEC_STOPPED_AT_CB_ENTRY =    10,  /**< While moving down, the stacker was stopped at the cashbox entry. BNR tries to move back the stacker in home position before declaring this error.May be due to bad insertion of the cashbox. */
  STEC_STOPPED_IN_CB =          11,  /**< While moving down, the stacker was stopped inside the cashbox. BNR tries to move back the stacker in home position before declaring this error. Plate might be blocked. */
} T_StackerErrorCode;

/** Tape Position Functional Status */
typedef enum TapePositionFunctionalStatus {
  TPFS_UNKNOWN =            0,
  TPFS_MIN_POSITION =       1,
  TPFS_ZERO_BILL_POSITION = 2,
  TPFS_OK =                 3,
  TPFS_LAST_BILL_POSITION = 4,
  TPFS_MAX_POSITION =       5,
} T_TapePositionFunctionalStatus;

/** Tape Position Error Code .*/
typedef enum TapePositionErrorCode {
  TPEC_NO_ERROR =             0,  /**< No error. */
  TPEC_COM_BREAKDOWN =        1,  /**< No communication with the module that owns the tape. */
  TPEC_NO_STEP_FOR_BILL_REEL= 2,  /**< Bill reel coding wheel counted no step while motor was on. */
  TPEC_NO_STEP_FOR_TAPE_REEL= 3,  /**< Tape reel coding wheel counted no step while motor was on. */
} T_TapePositionErrorCode;

//@} MODULE_STATUS

/** @ingroup MODULE_MAINTENANCE
 * Module Maintenance Status. 
 */
typedef enum MaintenanceStatus {
  MS_OK,                      /**< No maintenance needed. */
  MS_MAINTENANCE_RECOMMENDED  /**< Maintenance operation is recommended. */ 
} T_MaintenanceStatus;

/** @ingroup MODULE_MAINTENANCE
 * Module Maintenance Reason. 
 */
typedef enum MaintenanceReason {
  MR_NO_REASON,                    /**< No maintenance recomended. */
  MR_MAX_CYCLES_NR_REACHED,        /**< The maximum number of notes treatment cycles before maintenance has been reached. */
  MR_MAX_SENSORS_DIRT_REACHED,     /**< The maximum dirt threshold has been over-crossed for at least one sensor. */
  MR_MAX_BUNDLER_SLIPPAGE_REACHED  /**< The maximum slippage threshold has been reached for the bundler. */
} T_MaintenanceReason;

/** @ingroup MODULE_MAINTENANCE
 * Module Maintenance Sensor. 
 */
typedef struct MaintenanceSensor {
  T_ElementId  elementId;  /**< Identifier of the sensor that is checked for maintenance. */
  /**
   * Estimation of the sensor remaining signal, modified because of dirt.
   * The value is between 0 (so dirty that it is out of order) and 100 (clean).
   * When the signal estimation is 10 or lower, the maintenance event is
   * sent with the MR_MAX_SENSORS_DIRT_REACHED reason.
   */
  UINT8  sensorSignalEstimation;
} T_MaintenanceSensor;

/** @ingroup MODULE_MAINTENANCE
 *
 * Maximum number of maintenance reasons that can be reported.
 *
 * @see T_MaintenanceInfo
 */
#define MAX_NR_MAINTENANCE_REASONS (3)

/** @ingroup MODULE_MAINTENANCE
 *
 * Maximum number of sensor status that are reported for maintenance.
 *
 * @see T_MaintenanceInfo
 */
#define MAX_NR_MAINTENANCE_SENSOR_STATUS (11)

/** @ingroup MODULE_MAINTENANCE
 * Information on the Maintenance status of a module. This structure is typically returned by module_GetMaintenanceInfo().
 *
 * @see module_GetMaintenanceInfo(), module_SetMaintenanceInterval(), module_MaintenanceDone().
 */
typedef struct MaintenanceInfo {
  T_MaintenanceStatus  maintenanceStatus;                                /**< Indicates if a maintenance is recommended or not. Based on this flag, an #XFS_S_CDR_MAINTENANCE_STATUS_CHANGED event could be raised. Resetted to #MS_OK on module_MaintenanceDone() call. */
  T_LongAbsoluteCount  cycleSinceLastMaintenance;                        /**< Number of module cycles since the last maintenance operation. Resetted to 0 on module_MaintenanceDone() call. */ 
  T_LongAbsoluteCount  maintenanceInterval;                              /**< Number of module cycles between two maintenance operations. This field can be changed using module_SetMaintenanceInterval(). Default value is 50000 for all modules, except for Loader (20000). */
  UINT32               sizeMaintenanceReasons;                           /**< Number of T_MaintenanceReason items available in the array maintenanceReasons. */ 
  T_MaintenanceReason  maintenanceReasons [MAX_NR_MAINTENANCE_REASONS];  /**< Array storing all reasons why the selected module should receive preventative maintenance. */
  UINT32               sizeMaintenanceSensorStatus;                      /**< Number of T_MaintenanceSensor items available in the array maintenanceSensors. */ 
  T_MaintenanceSensor  maintenanceSensorStatus [MAX_NR_MAINTENANCE_SENSOR_STATUS]; /**< Array storing the maintenance status of the sensors. The size depends on the module type: 11 for the main module, 1 for the bundler, recycler or loader and 3 for the spine. */
  UINT16               maintenanceBundlerSlippage;                       /**< Percentage of bundler's slippage. */
} T_MaintenanceInfo;

/**
 * Loader capabilities.
 */
typedef struct LoaderCapabilities {
  BOOL  testContent;  /**< Specifies if the content of a newly inserted loader must be checked on the bnr_Reset() function call. */
} T_LoaderCapabilities;

/** 
 * ModuleCapabilities.
 *
 * Capabilities of a module.
 */
typedef union ModuleCapabilities {
  T_LoaderCapabilities  loader;
} T_ModuleCapabilities;

/**
 * Amount expressed in MDU. 
 *
 * @see T_XfsCashType, T_XfsDenomination, bnr_CashIn().
 */
typedef UINT32 T_Amount;                                        
typedef UINT32 T_BillCount;                                     /**< Type for bill counter. */

#define SIZE_OF_CURRENCY_CODE       (4)   /**< 3 letters ISO code + 0 terminal */

/** 
 * ISO currency code. 3 spaces if not significant. 
 *
 * @see bnr_CashIn().
 */
typedef char   T_XfsCurrencyCode[SIZE_OF_CURRENCY_CODE];        
typedef INT32  T_XfsCurrencyExponent;

/** @ingroup BNR_CASH_OPERATIONS BNR_LISTENER 
 * Structure that describes a currency with its exponent and ISO code.
 * 
 * @see T_XfsCashOrder.
 */
typedef struct XfsCurrency {
  T_XfsCurrencyCode      currencyCode; /**< ISO code (3 letters). */
  T_XfsCurrencyExponent  exponent;     /**< Determines the MDU. -2 for euro or dollar, for instance. */
} T_XfsCurrency;

/** @ingroup MODULE_CONFIG BNR_DENOMINATIONS BNR_CASH_UNITS
 * Structure that describes a Cash type ISO currency code, value, and variant.
 *
 * @see module_GetCashType(), module_SetCashType().
 */
typedef struct XfsCashType {
  T_XfsCurrencyCode  currencyCode;
  T_Amount           value;
  UINT32             variant;
} T_XfsCashType;

/** Security level for bill acceptance */
typedef enum SecurityLevel {
  SL_HIGH_ACCEPTANCE,
  SL_HIGH_SECURITY
} T_SecurityLevel;

/** @ingroup BNR_DENOMINATIONS
 * Structure that describes a denomination and its parameters.
 *
 * By default, these parameters are read-only properties, but the following ones are writable with bnr_UpdateDenominations():
 *  - enableDenomination,
 *  - securityLevel.
 * 
 * @see bnr_QueryDenominations(), bnr_UpdateDenominations().
 */
typedef struct XfsDenominationInfo {
  T_XfsCashType    cashType;                /**< Cash type of the denomination.*/
  BOOL             enableDenomination;      /**< Specifies if the denomination is enabled or not. Default value is TRUE. */
  T_SecurityLevel  securityLevel;           /**< Specifies the security level for acceptance of the denomination. Default value is #SL_HIGH_ACCEPTANCE. */
  char             studyVersion;            /**< Indicates the study version of the denomination.*/
  UINT32           billsetCompatibilityNr;  /**< Indicates the bill compatibility number of the denomination; must match the Billset Main Module component version.*/
  char             recognitionSensorType;   /**< Indicates the recognition sensor type required for the denomination.*/
} T_XfsDenominationInfo;

/** @ingroup BNR_DENOMINATIONS
 * List of denominations that can be queried from the BNR, then updated.
 *
 * @see bnr_QueryDenominations(), bnr_UpdateDenominations().
 */
typedef struct DenominationList {
  UINT32                 maxSize;                        /**< Maximum number of T_XfsDenominationInfo items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                 size;                           /**< Size of items array. */
  T_XfsDenominationInfo  items[MAX_NR_OF_DENOMINATION];  /**< List of denominations.*/
} T_DenominationList;

/** @ingroup BNR_CASH_UNITS
 * Structure that defines the levels determining a physical cash unit ThresholdStatus.
 *
 * @see T_XfsPhysicalCashUnit.
 */
typedef struct XfsThreshold {
  UINT32 full;   /**< The PCU ThresholdStatus becomes Full when the bill count is greater or equal to this value.*/
  UINT32 high;   /**< The PCU ThresholdStatus becomes High when the bill count is greater than this value.*/
  UINT32 low;    /**< The PCU ThresholdStatus becomes Low when the bill count is lower to this value.*/
  UINT32 empty;  /**< The PCU ThresholdStatus becomes Empty when the bill count is lower or equal to this value.*/
} T_XfsThreshold;

/** @ingroup BNR_CASH_UNITS
 * Threshold mode used to determine the ThresholdStatus of a PCU.
 */
typedef enum ThresholdMode {
  TM_SENSOR,  /**< ThresholdStatus changes are determined by the sensors of the BNR (physical filling status). */
  TM_COUNT    /**< ThresholdStatus changes are determined by comparing the PCU counts to the Threshold levels. */
} T_ThresholdMode;

/**
 * Maximum number of secondary cash types settable to a Logical cash unit.
 *
 * @see T_XfsCashTypeList.
 */
#define MAX_NR_OF_SECONDARY_CASH_TYPES (14)

/** @ingroup BNR_CASH_UNITS
 *  CashType list used for LCU's secondary cashType.
 */
typedef struct XfsCashTypeList {
  UINT8          size;
  T_XfsCashType  items[MAX_NR_OF_SECONDARY_CASH_TYPES];
} T_XfsCashTypeList;

/** @ingroup BNR_CASH_UNITS
 * Structure that describe a XFS Physical Cash Unit and its parameters.
 *
 * @see T_XfsCashUnit, bnr_QueryCashUnit(), bnr_UpdateCashUnit(), bnr_ConfigureCashUnit().
 */
typedef struct XfsPhysicalCashUnit {
  T_XfsPcuName          name;             /**< Name of the physical location in the BNR where this cash unit is installed. */
  T_XfsUnitId           unitId;           /**< Physical cash unit ID. Correspond to the BNR Module Serial Number (MSN). */
  T_BillCount           count;            /**< Actual count of bills or coins in the physical cash unit. For the cashbox, and loader this value is initialized by the bnr_ConfigureCashUnit() and bnr_UpdateCashUnit() methods. */
  T_XfsThreshold        threshold;        /**< Defines limits to determine #T_XfsPhysicalCashUnit.thresholdStatus. */
  UINT32                status;           /**< Status of the physical cash unit. */
  T_XfsThresholdStatus  thresholdStatus;  /**< Thresholdstatus of the physical cash unit. */
  T_ThresholdMode       thresholdMode;    /**< Defines how #T_XfsPhysicalCashUnit.thresholdStatus is determined. */
  BOOL                  lock;             /**< Enables or disables the physical cash unit */
} T_XfsPhysicalCashUnit;

/** @ingroup BNR_CASH_UNITS
 * Counters for #XFS_C_CDR_LCU_DEPOSIT cash unit types
 */
typedef struct DepositCounters {
  T_BillCount  depositCount;    /**< Number of bills that were deposited by the BNR. */
  T_BillCount  retractedCount;  /**< Number of bills that were retracted from the bezel and stored in this cash unit. */
  /**
   * Number of bills in this cash unit that were in the transport system while a BNR door was opened and then were 
   * stored in the cashbox on the subsequent bnr_Reset(). 
   * \b Note: Due to the fact that those bills were accessible to an operator, this count cannot be guaranteed to be 
   * accurate. 
   */
  T_BillCount  emptiedCount;
  T_BillCount  forgeryCount;  /**< Number of forgery bills in this cash unit. */
  /**
   * Number of bills that were in the transport system while a BNR door was opened and then were not seen on 
   * subsequent bnr_Reset(). 
   */
  T_BillCount  disappearedCount;
} T_DepositCounters;

/** @ingroup BNR_CASH_UNITS
 * Counters for #XFS_C_CDR_LCU_DISPENSE cash unit types
 */
typedef struct DispenseCounters {
  /** 
   * Number of bills from this logical unit which have been extracted by the BNR.
   * This value is initialized by the bnr_ConfigureCashUnit() and bnr_UpdateCashUnit() methods. 
   */
  T_BillCount  dispenseCount;
  /**
   * Number of bills from this logical unit that have been rejected during any operation which handles bills 
   * originating from this logical unit. 
   * The rejected bills are stored in the cashbox reject cash unit.
   * 
   * This value is initialized by the bnr_ConfigureCashUnit() and bnr_UpdateCashUnit() methods.
   */
  T_BillCount  rejectCount;
} T_DispenseCounters;

/** @ingroup BNR_CASH_UNITS
 * Extended counters for #XFS_C_CDR_LCU_DEPOSIT and #XFS_C_CDR_LCU_DISPENSE cash unit types
 */
typedef union ExtendedCounters {
  T_DepositCounters   deposit;
  T_DispenseCounters  dispense;
} T_ExtendedCounters;

/** @ingroup BNR_CASH_UNITS
 * Structure that describe a XFS Physical Cash Unit and its parameters.
 *
 * @see T_XfsCashUnit, bnr_QueryCashUnit(), bnr_UpdateCashUnit(), bnr_ConfigureCashUnit().
 */
typedef struct XfsLogicalCashUnit {
  T_XfsCashType      cashType;            /**< Defines the main type of cash used by this cash unit. */
  T_XfsCashTypeList  secondaryCashTypes;  /**< Defines the additional types of cash used by this cash unit. */
  /** 
   * Logical number of cash unit.
   * Unique number of the cash unit. This number is assigned (or reassigned) on bnr_Reset() and identifies the unit 
   * along the time; therefore, it can be used to track unit changes, or uniquely reference units in method calls 
   * (#T_XfsDenominationItem.unit property is an example).
   */
  UINT32  number;
  /**
   * Specifies, if cash unit can dispense, deposit cash or both.
   * One of the following values:
   * - XFS_C_CDR_LCU_NA
   * - XFS_C_CDR_LCU_DEPOSIT
   * - XFS_C_CDR_LCU_DISPENSE
   * - XFS_C_CDR_LCU_RECYCLE
   */
  UINT32  cuKind;
  /**
   * Type of cash unit.
   * One of the following values:
   * - XFS_C_CDR_LCU_NA
   * - XFS_C_CDR_LCU_BILL_CASSETTE
   * - XFS_C_CDR_LCU_REJECT_CASSETTE
   */
  UINT32                  cuType;
  T_XfsUnitId             unitId;            /**< See T_XfsPhysicalCashUnit.unitId. */
  T_BillCount             initialCount;      /**< For customer purpose only. This value is initialized by the bnr_ConfigureCashUnit() and bnr_UpdateCashUnit() methods and not changed by the BNR. */
  T_BillCount             count;             /**< Actual count of bills of this cashType in the physical cash unit. For the cashbox, and loader this value is initialized by the bnr_ConfigureCashUnit() and bnr_UpdateCashUnit() methods. It is incremented or decremented by the BNR when it inserts or extracts a bill. */
  UINT32                  status;            /**< Cash unit status. Correspond to T_XfsPhysicalCashUnit.status. */
  T_ExtendedCounters      extendedCounters;  /**< Extended counters for #XFS_C_CDR_LCU_DEPOSIT and #XFS_C_CDR_LCU_DISPENSE cash unit types. */
  T_XfsPhysicalCashUnit  *physicalCashUnit;  /**< Reference to its corresponding physical cash unit. */
} T_XfsLogicalCashUnit;

/** @ingroup BNR_CASH_UNITS
 * Structure that handles a list of T_XfsLogicalCashUnits.
 *
 * @see bnr_UpdateCashUnit(), bnr_ConfigureCashUnit().
 */
typedef struct LogicalCashUnitList {
  UINT32                maxSize;          /**< Maximum number of T_XfsLogicalCashUnit items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                size;             /**< Number of T_XfsLogicalCashUnit items available in the array. */
  T_XfsLogicalCashUnit  items[NBMAXLCU];
} T_LogicalCashUnitList;

/** @ingroup BNR_CASH_UNITS
 * Structure that handles a list of T_XfsPhysicalCashUnits.
 *
 * @see bnr_UpdateCashUnit(), bnr_ConfigureCashUnit().
 */
typedef struct PhysicalCashUnitList {
  UINT32                 maxSize;          /**< Maximum number of T_XfsPhysicalCashUnit items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                 size;             /**< Number of T_XfsPhysicalCashUnit items available in the array. */
  T_XfsPhysicalCashUnit  items[NBMAXPCU];
} T_PhysicalCashUnitList;

/** @ingroup BNR_CASH_UNITS
 * Structure that describes the whole set of T_XfsPhysicalCashUnits and T_XfsLogicalCashUnits in a BNR.
 *
 * @see bnr_QueryCashUnit().
 */
typedef struct XfsCashUnit {
  T_BillCount             transportCount;       /**< Number of bills in the transport system */
  T_LogicalCashUnitList   logicalCashUnitList;  /**< List of the LogicalCashUnit */
  T_PhysicalCashUnitList  physicalCashUnitList; /**< List of the PhysicalCashUnit */
} T_XfsCashUnit;

/*
 * System Status definitions
 */

/**
 * Maximum number of T_PositionStatus items in XFS CDR Status.
 */
#define MAX_NR_OF_POSITIONS (2)

/** @addtogroup BNR_SYSTEM_STATUS 
*/
//@{

/**
 * Content of a given position.
 */
typedef enum XfsCdrContentStatus {
  XFS_S_CDR_CT_EMPTY,                               /**< The position is empty. */
  XFS_S_CDR_CT_NOT_EMPTY                            /**< The position is not empty. */
} T_XfsCdrContentStatus;

typedef struct PositionStatus {
  T_XfsCdrPosition        position;       /**< Identification of the position. */
  T_XfsCdrContentStatus   contentStatus;  /**< Status of the contents. */
  T_XfsCdrShutterStatus   shutterStatus;  /**< Status of the shutter. */
} T_PositionStatus;

typedef struct XfsCdrPositionStatusList {
  UINT32                maxSize;                     /**< Maximum number of T_PositionStatus items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                size;                        /**< Number of T_PositionStatus items available in the array. */
  T_PositionStatus      items[MAX_NR_OF_POSITIONS];
} T_XfsCdrPositionStatusList;

/**
 * XfsCdrStatus structure.
 *
 * @see bnr_GetStatus().
 */
typedef struct XfsCdrStatus {
  UINT32                      deviceStatus;               /**< Current device status. */
  UINT32                      dispenserStatus;            /**< Status of dispenser. */
  UINT32                      intermediateStackerStatus;  /**< Intermediate stacker status. */
  UINT32                      safeDoorStatus;             /**< Safe door status. */
  UINT32                      shutterStatus;              /**< Status of the outlet shutter. @deprecated Use the #T_XfsCdrStatus.positionStatusList (2009-09-18). */
  UINT32                      transportStatus;            /**< Status of transport unit as descibed in XFS standard. */
  T_XfsCdrPositionStatusList  positionStatusList;         /**< List of the PositionStatus */
} T_XfsCdrStatus;

/**
 * Type of Cash transaction.
 */
typedef enum TransactionType {
  TT_NONE,                 /**< No transaction. */
  TT_CASHIN_TRANSACTION,   /**< Starts with bnr_CashInStart() and ends usually with bnr_CashInEnd(). */
  TT_DISPENSE_TRANSACTION  /**< Starts with the OperationCompleteEvent after a bnr_Dispense() and ends usually with the OperationCompleteEvent of bnr_Present(). */
} T_TransactionType;

/**
 * System transaction status.
 *
 * @see bnr_GetTransactionStatus().
 */
typedef struct TransactionStatus {
  T_TransactionType  currentTransaction;  /**< The active transaction in the BNR. */
  LONG32             currentOperation;    /**< The ID of the curent operation the BNR is running. If an operation is running with cash at bezel, this means the BNR is waiting for someone to remove the cash. */
  BOOL               cashAtBezel;         /**< TRUE if there is cash at bezel. Use bnr_GetStatus() to know if it is at inlet or outlet. */
} T_TransactionStatus;

/**
 * Bill transport status.
 *
 * Status specific to the bill transport in the system. Used to highlight if there is one or more bills blocked in the 
 * system. 
 *
 */
typedef enum BillTransportStatus {
  BTS_OK,              /**< No error and no bill is stopped in the transport system. */
  BTS_BILL_STOP,       /**< One or more bills are stopped in the transport system due to a power down, system restart, system opening or a hardware failure (e.g.\ motor). */
  BTS_BILL_JAM,        /**< One or more bills are stopped in the transport system due to a bill jam or an undetected failure. */
  BTS_BILL_ERROR,      /**< An non transportable bill or bundle has been detected in the transport system.*/
  BTS_TRANSPORT_ERROR  /**< Something is preventing the motor from spinning or the coding wheel is unable to sense that the motor is spinning.  This could be a faulty motor, a faulty sensor/broken coding wheel, a faulty board, or in rare cases a jam.*/
} T_BillTransportStatus;

/**
 * Section of the bill transport system.
 */
typedef enum BillTransportSection {
  BTSE_UNKNOWN,                /**< The section is not available. */
  BTSE_INLET_FW,               /**< In the inlet section while moving forward.*/
  BTSE_INLET_BW,               /**< In the inlet section while moving backward.*/
  BTSE_POSITIONER_FW,          /**< In the positioner section while moving forward. */
  BTSE_POSITIONER_BW,          /**< In the positioner section moving backward. */
  BTSE_RECOGNITION_SENSOR_FW,  /**< In the recognition sensor section while moving forward. */
  BTSE_RECOGNITION_SENSOR_BW,  /**< In the recognition sensor section moving backward. */
  BTSE_RS_SP_INTERFACE_FW,     /**< Between recognition sensor (RS) and spine (SP) sections while moving forward. */
  BTSE_RS_SP_INTERFACE_BW,     /**< Between recognition sensor (RS) and spine (SP) sections moving backward. */
  BTSE_SPINE_FW,               /**< In the spine section while moving forward. */
  BTSE_SPINE_BW,               /**< In the spine section moving backward. */
  BTSE_MODULE_INTERFACE_FW,    /**< Between a module and the spine sections while moving forward. */
  BTSE_MODULE_INTERFACE_BW,    /**< Between a module and the spine sections moving backward. */
  BTSE_MODULE_FW,              /**< In the module section while moving forward. */
  BTSE_MODULE_BW,              /**< In the module section moving backward. */
  BTSE_BT_SP_INTERFACE_FW,     /**< Between the MainModule bottom transport and the spine sections while moving forward. */
  BTSE_BT_SP_INTERFACE_BW,     /**< Between the MainModule bottom transport and the spine sections moving backward. */
  BTSE_BOTTOM_TRANSPORT_FW,    /**< In the MainModule bottom transport section while moving forward. */
  BTSE_BOTTOM_TRANSPORT_BW,    /**< In the MainModule bottom transport section moving backward. */
  BTSE_BUNDLER_FW,             /**< In the bundler section while moving forward. */
  BTSE_BUNDLER_BW,             /**< In the bundler section moving backward. */
  BTSE_OUTLET_FW,              /**< In the outlet section while moving forward. */
  BTSE_OUTLET_BW               /**< In the outlet section moving backward. */
} T_BillTransportSection;
//@} BNR_SYSTEM_STATUS

/** @ingroup MODULE_CONFIG
 * Simple version structure, used in version requirements.
 * 
 * @see T_VersionRequirement.
 */
typedef struct Version {
  UINT32  major;
  UINT32  minor;
} T_Version;


/** @ingroup MODULE_CONFIG
 * VersionRequirement structure
 */
typedef struct VersionRequirement {
  UINT32     moduleType;
  UINT32     componentType;  /**< See @ref COMPONENT_ID */
  T_Version  version;
} T_VersionRequirement;


#define BUILD_LENGTH (5)

/** @ingroup BNR_SYS_CONFIG
 * Full version structure
 */
typedef struct FullVersion {
  UINT32 major;
  UINT32 minor;
  UINT32 service;
  char build[BUILD_LENGTH];
} T_FullVersion;

/**
 * Status of a software component.
 */
typedef enum ComponentStatus {
  CS_NO_ERROR,          /**< The component is present in the flash memory and has been tested. */
  CS_INVALID_CHECKSUM,  /**< The component in the flash memory has a wrong checksum. */
  CS_MISSING,           /**< This component is not available in the flash memory. */
  CS_OTHER_ERROR,       /**< The component in the flash memory cannot be started. It was started but did not run well. */
  CS_NOT_TESTED         /**< The component has been updated recently and never started. Would be started next reboot. */
} T_ComponentStatus;

/** @ingroup MODULE_CONFIG
 * Component structure.
 */
typedef struct Component {
  T_ComponentId         id;                       /**< Identifier of the component, see @ref COMPONENT_ID */
  T_FullVersion         version;
  T_ComponentStatus     status;                   /**< Component status in the flash memory. */
  UINT32                size;
  T_VersionRequirement  requirements[NBMAXREQ];
} T_Component;

/** @ingroup MODULE_CONFIG
 * ModuleIdentification structure.
 *
 * @see module_GetIdentification().
 */
typedef struct ModuleIdentification {
  T_XfsUnitId          Name;
  T_ModuleDescription  Description;
  T_ModuleUserData     UserData;
  UINT32               size;
  T_Component          components[NBMAXCOMP];
} T_ModuleIdentification;

/** @ingroup MODULE_STATUS
 * Basic Element Status 
 */
typedef struct ElementStatus {
  T_ModuleId           id;
  T_OperationalStatus  operationalStatus;
} T_ElementStatus;

/** @ingroup MODULE_STATUS
 * Cover Element Status. 
 */
typedef struct CoverStatus {
  T_ModuleId               id;
  T_OperationalStatus      operationalStatus;  /**< Always set to OS_OPERATIONAL. */
  T_CoverFunctionalStatus  functionalStatus;
} T_CoverStatus;

/** @ingroup MODULE_STATUS
 * TapePosition Element Status. 
 */
typedef struct TapePositionStatus {
  T_ModuleId                      id;
  T_OperationalStatus             operationalStatus;
  T_TapePositionFunctionalStatus  functionalStatus;
  T_TapePositionErrorCode         errorCode;
} T_TapePositionStatus;

/** @ingroup MODULE_STATUS
 * Motor Element Status. 
 */
typedef struct MotorStatus {
  T_ModuleId               id;
  T_OperationalStatus      operationalStatus;
  T_MotorFunctionalStatus  functionalStatus;
  T_MotorErrorCode         errorCode;
} T_MotorStatus;

/** @ingroup MODULE_STATUS
 * Diverter Element Status. 
 */
typedef struct DiverterStatus {
  T_ModuleId                  id;
  T_OperationalStatus         operationalStatus;
  T_DiverterFunctionalStatus  functionalStatus;
  T_DiverterErrorCode         errorCode;
} T_DiverterStatus;

/** @ingroup MODULE_STATUS
 * Sensor Element Status. 
 */
typedef struct SensorStatus {
  T_ModuleId                id;
  T_OperationalStatus       operationalStatus;
  T_SensorFunctionalStatus  functionalStatus;
  T_SensorErrorCode         errorCode;
} T_SensorStatus;

/** @ingroup MODULE_STATUS
 * Positioner element Status. 
 */
typedef struct PositionerStatus {
  T_ModuleId                    id;
  T_OperationalStatus           operationalStatus;
  T_PositionerFunctionalStatus  functionalStatus;
  T_PositionerErrorCode         errorCode;
} T_PositionerStatus;

/** @ingroup MODULE_STATUS
 * PressurePlate element Status. 
 */
typedef struct PressurePlateStatus {
  T_ModuleId                       id;
  T_OperationalStatus              operationalStatus;
  T_PressurePlateFunctionalStatus  functionalStatus;
  T_PressurePlateErrorCode         errorCode;
} T_PressurePlateStatus;

/** @ingroup MODULE_STATUS
 * BundlerDivSystem element Status. 
 */
typedef struct BundlerDivSystemStatus {
  T_ModuleId                          id;
  T_OperationalStatus                 operationalStatus;
  T_BundlerDivSystemFunctionalStatus  functionalStatus;
  T_BundlerDivSystemErrorCode         errorCode;
} T_BundlerDivSystemStatus;

/** @ingroup MODULE_STATUS
 * CashboxStackHeight element Status. 
 */
typedef struct CashboxStackHeightStatus {
  T_ModuleId                            id;
  T_OperationalStatus                   operationalStatus;  /**< @deprecated No more used, always set to #OS_OPERATIONAL (2010-05-10). */
  T_CashboxStackHeightErrorCode         errorCode;          /**< @deprecated No more used, always set to #CSHEC_NO_ERROR (2011-06-17).*/
  T_CashboxStackHeightFunctionalStatus  functionalStatus;
} T_CashboxStackHeightStatus;

/** @ingroup MODULE_STATUS
 * Flap element Status. 
 */
typedef struct FlapStatus {
  T_ModuleId              id;                 /**< Id of the module. */
  T_OperationalStatus     operationalStatus;  /**< Operational status of the flap. */
  T_FlapErrorCode         errorCode;          /**< Error code of the element. Useful if the operationalStatus is different from #OS_OPERATIONAL */
  T_FlapFunctionalStatus  functionalStatus;   /**< Functional status of the flap. */
} T_FlapStatus;

/** @ingroup MODULE_STATUS
 * LoaderStackHeight element Status. 
 */
typedef struct LoaderStackHeightStatus {
  T_ModuleId                           id;
  T_OperationalStatus                  operationalStatus;  /**< Always set to #OS_OPERATIONAL. */
  T_LoaderStackHeightFunctionalStatus  functionalStatus;
} T_LoaderStackHeightStatus;

/** @ingroup MODULE_STATUS
 * Stacker element Status. 
 */
typedef struct StackerStatus {
  T_ModuleId                 id;
  T_OperationalStatus        operationalStatus;
  T_StackerFunctionalStatus  functionalStatus;
  T_StackerErrorCode         errorCode;
} T_StackerStatus;

/** @ingroup MODULE_STATUS
 * RecognitionSensor element Status. 
 */
typedef struct RecognitionSensorStatus {
  T_ModuleId                    id;
  T_OperationalStatus           operationalStatus;
  T_RecognitionSensorErrorCode  errorCode;
  T_SensorFunctionalStatus      upFunctionalStatus;    /**< Functional Status of the RS Bill Sensor Up */
  T_SensorFunctionalStatus      downFunctionalStatus;  /**< Functional Status of the RS Bill Sensor Down */
} T_RecognitionSensorStatus;

/** @ingroup MODULE_STATUS
 * Union of all BarcodeReader Module element statuses. 
 */
typedef union BarcodeReaderElementStatusItem {
  T_ElementStatus  elementStatus;
  T_SensorStatus   sensorStatus;
} T_BarcodeReaderElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Spine Module element statuses. 
 */
typedef union SpineElementStatusItem {
  T_ElementStatus   elementStatus;
  T_CoverStatus     coverStatus;
  T_SensorStatus    sensorStatus;
  T_SensorStatus    detectorStatus;  /**< @deprecated Use #T_SpineElementStatusItem.sensorStatus (2009-06-26). */
  T_DiverterStatus  diverterStatus;
} T_SpineElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Recycler Module element statuses. 
 */
typedef union RecyclerElementStatusItem {
  T_ElementStatus       elementStatus;
  T_SensorStatus        sensorStatus;
  T_SensorStatus        detectorStatus;  /**< @deprecated Use #T_RecyclerElementStatusItem.sensorStatus (2009-06-26). */
  T_MotorStatus         motorStatus;
  T_TapePositionStatus  tapePositionStatus;
} T_RecyclerElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Cashbox Module element statuses. 
 */
typedef union CashboxElementStatusItem {
  T_ElementStatus  elementStatus;
  T_SensorStatus   sensorStatus;
  T_SensorStatus   detectorStatus;  /**< @deprecated Use #T_CashboxElementStatusItem.sensorStatus (2009-06-26). */
} T_CashboxElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Bundler Module element statuses. 
 */
typedef union BundlerElementStatusItem {
  T_ElementStatus  elementStatus;
  T_SensorStatus   sensorStatus;
  T_SensorStatus   detectorStatus;  /**< @deprecated Use #T_BundlerElementStatusItem.sensorStatus (2009-06-26) */
  T_MotorStatus    motorStatus;
} T_BundlerElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Loader Module element statuses. 
 */
typedef union LoaderElementStatusItem {
  T_ElementStatus            elementStatus;
  T_SensorStatus             sensorStatus;
  T_SensorStatus             detectorStatus;       /**< @deprecated Use #T_LoaderElementStatusItem.sensorStatus (2009-06-26). */
  T_MotorStatus              motorStatus;
  T_PressurePlateStatus      pressurePlateStatus;
  T_FlapStatus               antiFishingStatus;
  T_LoaderStackHeightStatus  stackHeightStatus;
} T_LoaderElementStatusItem;

/** @ingroup MODULE_STATUS
 * Union of all Main Module element statuses. 
 */
typedef union MainModuleElementStatusItem {
  T_ElementStatus             elementStatus;
  T_SensorStatus              sensorStatus;
  T_CoverStatus               coverStatus;
  T_MotorStatus               motorStatus;
  T_StackerStatus             stackerStatus;
  T_BundlerDivSystemStatus    bundlerDivSystemStatus;
  T_RecognitionSensorStatus   recognitionSensorStatus;
  T_PositionerStatus          positionerStatus;
  T_CashboxStackHeightStatus  cashboxStackHeightStatus;
  T_FlapStatus                outletFlapStatus;
  T_SensorStatus              detectorStatus;            /**< @deprecated Use #T_MainModuleElementStatusItem.sensorStatus (2009-06-26). */
  T_RecognitionSensorStatus   hibouStatus;               /**< @deprecated Use #T_MainModuleElementStatusItem.recognitionSensorStatus (2009-06-26). */
} T_MainModuleElementStatusItem;

/** Module status */
/* -------------- */

/** @ingroup MODULE_STATUS
 * Base class of all Module statuses.
 *
 * @see module_GetStatus().
 */
typedef struct ModuleStatusBaseClass {
  T_ModuleId           id;                 /**< Id of the MainModule module. @see Module Identification. */
  T_OperationalStatus  operationalStatus;  /**< Operational status of the module. Possible values are the same for all operational statuses. @see T_OperationalStatus. */
} T_ModuleStatusBaseClass;

/** @ingroup MODULE_STATUS
 * BarcodeReader Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct BarcodeReaderStatus {
  T_ModuleId                        id;
  T_OperationalStatus               operationalStatus;
  T_ModuleErrorCode                 errorCode;
  UINT32                            size;
  T_BarcodeReaderElementStatusItem  items[NBMAXELEMENTS];
} T_BarcodeReaderStatus;

/** @ingroup MODULE_STATUS
 * Spine Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct SpineStatus {
  T_ModuleId                id;
  T_OperationalStatus       operationalStatus;
  T_ModuleErrorCode         errorCode;
  T_BillTransportStatus     billTransportStatus;
  T_BillTransportSection    billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  UINT32                    size;
  T_SpineElementStatusItem  items[NBMAXELEMENTS];
} T_SpineStatus;

/** @ingroup MODULE_STATUS
 * Recycler Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct RecyclerStatus {
  T_ModuleId                   id;
  T_OperationalStatus          operationalStatus;
  T_ModuleErrorCode            errorCode;
  T_RecyclerFunctionalStatus   functionalStatus;
  T_BillStorageStatus          billStorageStatus;
  T_BillTransportStatus        billTransportStatus;
  T_BillTransportSection       billTransportSection;   /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  T_CashTypeStatus             cashTypeStatus;         /**< Status of the CashType compatibility between the module and its LogicalCashUnit. */
  UINT32                       size;
  T_RecyclerElementStatusItem  items[NBMAXELEMENTS];
} T_RecyclerStatus;

/** @ingroup MODULE_STATUS
 * Cashbox Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct CashboxStatus {
  T_ModuleId                  id;
  T_OperationalStatus         operationalStatus;
  T_ModuleErrorCode           errorCode;
  T_BillStorageStatus         billStorageStatus;
  UINT32                      size;
  T_CashboxElementStatusItem  items[NBMAXELEMENTS];
} T_CashboxStatus;

/** @ingroup MODULE_STATUS
 * Bundler Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct BundlerStatus {
  T_ModuleId                  id;
  T_OperationalStatus         operationalStatus;
  T_BundlerErrorCode          errorCode;
  T_BundlerFunctionalStatus   functionalStatus;
  T_BillStorageStatus         billStorageStatus;
  UINT32                      size;
  T_BundlerElementStatusItem  items[NBMAXELEMENTS];
} T_BundlerStatus;

/** @ingroup MODULE_STATUS
 * Loader Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct LoaderStatus {
  T_ModuleId                 id;
  T_OperationalStatus        operationalStatus;
  T_ModuleErrorCode          errorCode;
  T_LoaderFunctionalStatus   functionalStatus;
  T_BillStorageStatus        billStorageStatus;
  T_BillTransportStatus      billTransportStatus;
  T_BillTransportSection     billTransportSection;   /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  T_CashTypeStatus           cashTypeStatus;         /**< Status of the CashType compatibility between the module and its LogicalCashUnit. */
  UINT32                     size;
  T_LoaderElementStatusItem  items[NBMAXELEMENTS];
} T_LoaderStatus;

/** @ingroup MODULE_STATUS
 * Main Module status.
 *
 * This class inherits from T_ModuleStatusBaseClass.
 */
typedef struct MainModuleStatus {
  T_ModuleId                     id;                       
  T_OperationalStatus            operationalStatus;
  T_MainModuleErrorCode          errorCode;             /**< Specific main module error status. @see T_MainModuleErrorCode */
  T_BillTransportStatus          billTransportStatus;   /**< Status of the bill transport. */
  T_BillTransportSection         billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  UINT32                         size;                  /**< Number of element containing in items field. */
  T_MainModuleElementStatusItem  items[NBMAXELEMENTS];  /**< Table of structures containing the status of all the elements in MainModule module. */
} T_MainModuleStatus;

/** @ingroup MODULE_STATUS
 * Union of all T_ModuleStatus classes.
 *
 * Those classes inherit from T_ModuleStatusBaseClass.
 */
typedef union ModuleStatus {
  T_ModuleStatusBaseClass  baseClass;
  T_MainModuleStatus       mainModuleStatus;
  T_BundlerStatus          bundlerStatus;
  T_RecyclerStatus         recyclerStatus;
  T_CashboxStatus          cashboxStatus;
  T_SpineStatus            spineStatus;
  T_LoaderStatus           loaderStatus;
  T_BarcodeReaderStatus    barcodeReaderStatus;
} T_ModuleStatus;

/** @addtogroup BNR_SYS_CONFIG
 */
//@{

/**
 * Bill routing option used for the Bill Routing Table (#T_BillRoutingTable).
 */
typedef enum BillRoutingOption {
  BRO_ESCROW,         /**< The bill is escrowed. On bnr_CashInEnd() (end of the transaction), it is stored in a Recycler or in the Cashbox, according to the CashUnit configuration. */
  BRO_ESCROW_ENCASH,  /**< The bill is escrowed. On bnr_CashInEnd() (end of the transaction), it is stored in the Cashbox. */
  BRO_REFUND,         /**< The bill is returned to the BNR user. */
  BRO_STORE,          /**< The bill is @b not escrowed. The bill is stored either in a Recycler or in the Cashbox, according to the CashUnit configuration. */
  BRO_STORE_ENCASH    /**< The bill is @b not escrowed. The bill is stored in the Cashbox. */
} T_BillRoutingOption;

/**
 * Bill Routing Table
 *
 * For each Recognition Result, it is possible to define a Bill Routing Option (T_BillRoutingOption), 
 * that will determine how the bill will be routed according to the recognition result.
 *
 * @see bnr_GetDefaultBillRouting() and bnr_SetDefaultBillRouting().
 */
typedef struct BillRoutingTable {
  T_BillRoutingOption  valid;       /**< Good, genuine, fit bills recognized. Default value is #BRO_ESCROW. */
  T_BillRoutingOption  validUnfit;  /**< Genuine bill recognized unfit. Default value is #BRO_ESCROW. */
  T_BillRoutingOption  suspect;     /**< Almost valid bill (see Art 6 definitions). Default value is #BRO_REFUND. */
  T_BillRoutingOption  forgery;     /**< Forgery, denominated, not authentified. Default value is #BRO_REFUND. */
  T_BillRoutingOption  unknown;     /**< Non recognized bill. Default value is #BRO_REFUND. */
} T_BillRoutingTable;

/** Characteristics of an input/output position. */
typedef struct XfsCdrPositionCapabilities {
  T_XfsCdrPosition  position;
  BOOL              shutterStatusSupported;  /**< Specifies whether shutter status is supported for this position. */
  BOOL              shutterCmd;              /**< Defines if the shutter has to be explicitly controlled by the application. */
  BOOL              contentStatusSupported;  /**< Specifies whether there is a sensor to detect if the position is empty. */
  UINT32            maxItems;                /**< Maximum number of items which this position can hold. */
  BOOL              input;                   /**< Specifies whether this position can be used as source for an accept command. */
  BOOL              output;                  /**< Specifies whether this position can be used as target for a dispense command. */
  BOOL              rollback;                /**< Specifies whether this position can be used as target for cashInRollback command. */
  BOOL              refusal;                 /**< Specifies whether refused notes can be moved to this position during cashIn command. */
} T_XfsCdrPositionCapabilities;

/**
 * List of position capabilities. One for each position supported by the device.
 *
 * @see T_XfsCapabilities.
 */
typedef struct XfsCdrPositionCapabilitiesList {
  UINT32                        maxSize;                    /**< Maximum number of T_PositionStatus items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                        size;                       /**< Number of T_PositionStatus items available in the array. */
  T_XfsCdrPositionCapabilities  items[MAX_NR_OF_POSITIONS];
} T_XfsCdrPositionCapabilitiesList;

/** 
 * Capabilities for Euro Article 6 .
 *
 * @see T_XfsCapabilities.
 */
typedef struct XfsEuroArt6Capability {
  BOOL  category2;
  BOOL  category3;
  BOOL  unfit;
} T_XfsEuroArt6Capability;

/**
 * Defines how the BNR perform the self tests.
 *
 * @par Integration recommendations :
 *
 * If the slave mode (#STM_SLAVE) is used, the Host should send a bnr_SelfTest() command 
 * at every time that internal tests and movements of the BNR can be allowed.
 *
 * It is recommended to send bnr_SelfTest(), after a bnr_Present() command to allow the 
 * BNR to refloat the recyclers.
 *
 * @see T_XfsCapabilities.
 */
typedef enum SelfTestMode {
  STM_AUTO,  /**< This is the default and recommended mode. In this mode, the BNR does self tests automatically. After bnr_Reset() or bnr_Present(), the BNR refloats the Recyclers from the Loader, when necessary.*/
  STM_SLAVE  /**< In this mode, the BNR doesn't do anything automatically, but waits for a bnr_SelfTest() command to do all self test actions. */
} T_SelfTestMode;

/**
 * Defines the sensitivity level of string detection at Inlet.
 *
 * @see T_XfsCapabilities.
 */
typedef enum AntiFishingLevel {
  AFL_NORMAL,  /**< Normal sensitivity (default).*/
  AFL_HIGH,    /**< High sensitivity.*/
  AFL_SPECIAL  /**< Special mode for use in applications where rain water blows into the BNR bezel, please @ref CONTACT "contact" MEI technical support for details. */
} T_AntiFishingLevel;

/**
 * Defines the kind of error report to be generated when a failure is detected whith no bill being transported.
 *
 * @see T_XfsCapabilities.
 */
typedef enum ReportingMode {
  RM_NORMAL,  /**< Normal reporting mode (default) : on failure detection, a SimpleFailureReport will saved.*/
  RM_FULL     /**< Full reporting mode : on failure detection, a BillTransportErrorReport will saved.*/
} T_ReportingMode;

/**
 * Describes the BNR capabilities.
 *
 * By default, capabilites are read-only properties, but the following ones are writable :
 *  - autoPresent,
 *  - selfTestMode,
 *  - antiFishingLevel,
 *  - allowUsbFrontSwitch,
 *  - reportingMode.
 */
typedef struct XfsCapabilities {
  /**
   * This specifies whether cash will be automatically presented to the user on execution of a dispense (autoPresent 
   * set to TRUE), or whether the cash will only be transported to the Bundler. In the latter case, a bnr_Present() command 
   * will need to be issued following the bnr_Dispense() command. Default value is FALSE.
   */
  BOOL                              autoPresent;
  UINT32                            cdType;                    /**< Type of device. Allways #XFS_C_CDR_TYPE_ATM for the BNR. */
  T_XfsEuroArt6Capability           euroArt6Capability;        /**< Reserved for future use.*/
  BOOL                              trustedUser;               /**< Reserved for future use.*/
  UINT32                            maxInBills;                /**< Maximum number of bills to be accepted by one command.  Allways 1 for the BNR. */
  UINT32                            maxOutBills;               /**< Maximum number of bills to be dispensed by one command. Equals to 20 in BNA6 configuration, otherwise equals to 15. */
  BOOL                              shutterCmd;                /**< The shutter be accessed by commands. Allways FALSE in the BNR. */
  BOOL                              retract;                   /**< The cash dispenser can retract presented bills. Allways TRUE in the BNR since FW v1.3.0, FALSE with previous versions. */
  BOOL                              safeDoorCmd;               /**< This device supports a safe door command. Allways FALSE in the BNR. */
  BOOL                              cashBox;                   /**< The service can handle a cash box. Allways TRUE in the BNR. */
  BOOL                              refill;                    /**< Can the BNR be refilled by placing bills of the same size on the stack of the Loader. Allways TRUE in the BNR. */
  BOOL                              dispense;                  /**< The device can dispense cash. Equals to FALSE in BNA6 configuration, otherwise equals to TRUE. */
  BOOL                              deposit;                   /**< The device can deposit cash. Allways TRUE in the BNR. */
  BOOL                              intermediateStacker;       /**< The device has a temporary storage before presenting bills. Allways TRUE in the BNR. */
  BOOL                              billTakenSensor;           /**< The device has a bills taken sensor. Allways TRUE in the BNR. */
  BOOL                              escrow;                    /**< The device supports an escrow. Allways TRUE in the BNR. */
  UINT32                            escrowSize;                /**< Specifies the maximum number of bills on the escrow. Equals to 20 in BNA6 configuration, otherwise equals to 15. */
  BOOL                              detector;                  /**< The device supports a detector to verify accepted cash. Allways TRUE in the BNR. */
  T_XfsCdrPosition                  defaultRollbackPosition;   /**< Specifies the default output position to rollback cash. Allways #XFS_C_CDR_POS_BOTTOM in the BNR. */
  T_XfsCdrPositionCapabilitiesList  positionCapabilitiesList;  /**< Specifies the capabilities of each position supported by the device. Please refer to T_XfsCdrPositionCapabilities for default values. */
  T_SelfTestMode                    selfTestMode;              /**< Allows to choose when the BNR can perform the self tests. Default value is #STM_AUTO (recommended). */
  CHAR                              recognitionSensorType;     /**< Recognition sensor type identification. Allways 'B' in the BNR. */
  T_AntiFishingLevel                antiFishingLevel;          /**< Sensitivity level of string detection at Inlet. Default value is #AFL_NORMAL. */
  BOOL                              allowUsbFrontSwitch;       /**< Allows to use USB Front interface to communicate with the BNR. Default value is TRUE. */
  T_ReportingMode                   reportingMode;             /**< Specifies the kind of report generated on failure detection with no bill transported. Default value is #RM_NORMAL. */
} T_XfsCapabilities;
//@} BNR_SYS_CONFIG

/** @ingroup BNR_SYSTEM_STATUS
 * Base class of all Module status overviews.
 */
typedef struct ModuleStatusOverviewBaseClass {
  T_ModuleId           id;                 /**< Id of the MainModule module. @see Module Identification. */
  T_OperationalStatus  operationalStatus;  /**< Operational status of the module. Possible values are the same for all operational statuses. @see T_OperationalStatus. */
} T_ModuleStatusOverviewBaseClass;

/** @ingroup BNR_SYSTEM_STATUS
 * BarcodeReader status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct BarcodeReaderStatusOverview {
  T_ModuleId           id;                 /**< Module identification. */
  T_OperationalStatus  operationalStatus;  /**< OperationalStatus of the module. */
  T_ModuleErrorCode    errorCode;          /**< BarcodeReader error code. */
} T_BarcodeReaderStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Bundler status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct BundlerStatusOverview {
  T_ModuleId                 id;                 /**< Module identification. */
  T_OperationalStatus        operationalStatus;  /**< OperationalStatus of the module. */
  T_BundlerErrorCode         errorCode;          /**< Bundler error code. */
  T_BillStorageStatus        billStorageStatus;  /**< Status of the bill storage cassette. */
  T_BundlerFunctionalStatus  functionalStatus;
} T_BundlerStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Cashbox status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct CashboxStatusOverview {
  T_ModuleId           id;                 /**< Module identification. */
  T_OperationalStatus  operationalStatus;  /**< OperationalStatus of the module. */
  T_ModuleErrorCode    errorCode;          /**< Cashbox error code. */
  T_BillStorageStatus  billStorageStatus;  /**< Status of the bill storage cassette. */
} T_CashboxStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Loader status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct LoaderStatusOverview {
  T_ModuleId                id;                    /**< Module identification. */
  T_OperationalStatus       operationalStatus;     /**< OperationalStatus of the module. */
  T_ModuleErrorCode         errorCode;             /**< Loader error code. */
  T_BillStorageStatus       billStorageStatus;     /**< Status of the bill storage cassette. */
  T_BillTransportStatus     billTransportStatus;   /**< Status of the bill transport. */
  T_BillTransportSection    billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  T_CashTypeStatus          cashTypeStatus;        /**< Status of the CashType compatibility between the module and its LogicalCashUnit. Available only for Recycler and Loader modules. */
  T_LoaderFunctionalStatus  functionalStatus;
} T_LoaderStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * MainModule status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct MainModuleStatusOverview {
  T_ModuleId              id;                    /**< Module identification. */
  T_OperationalStatus     operationalStatus;     /**< OperationalStatus of the module. */
  T_MainModuleErrorCode   errorCode;             /**< MainModule error code. */
  T_BillTransportStatus   billTransportStatus;   /**< Status of the bill transport. */
  T_BillTransportSection  billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
} T_MainModuleStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Recycler status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct RecyclerStatusOverview {
  T_ModuleId                  id;                    /**< Module identification. */
  T_OperationalStatus         operationalStatus;     /**< OperationalStatus of the module. */
  T_ModuleErrorCode           errorCode;             /**< Recycler error code. */
  T_BillTransportStatus       billTransportStatus;   /**< Status of the bill transport. */
  T_BillTransportSection      billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
  T_BillStorageStatus         billStorageStatus;     /**< Status of the bill storage cassette. */
  T_CashTypeStatus            cashTypeStatus;        /**< Status of the CashType compatibility between the module and its LogicalCashUnit. Available only for Recycler and Loader modules. */
  T_RecyclerFunctionalStatus  functionalStatus;      /**< Recycler functional status. */
} T_RecyclerStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Spine status overview.
 *
 * This class inherits from T_ModuleStatusOverviewBaseClass.
 */
typedef struct SpineStatusOverview {
  T_ModuleId              id;                    /**< Module identification. */
  T_OperationalStatus     operationalStatus;     /**< OperationalStatus of the module. */
  T_ModuleErrorCode       errorCode;             /**< Spine error code. */
  T_BillTransportStatus   billTransportStatus;   /**< Status of the bill transport. */
  T_BillTransportSection  billTransportSection;  /**< Transport section related to the billTransportStatus. Not set if billTransportStatus = BTS_OK */
} T_SpineStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * Union of all T_ModuleStatusOverview classes.
 *
 * Those classes inherit from T_ModuleStatusOverviewBaseClass.
 *
 * @see T_SystemStatusOverview.
 */
typedef union ModuleStatusOverview {
  T_ModuleStatusOverviewBaseClass  baseClass;
  T_BundlerStatusOverview          bundler;
  T_CashboxStatusOverview          cashbox;
  T_LoaderStatusOverview           loader;
  T_MainModuleStatusOverview       mainModule;
  T_RecyclerStatusOverview         recycler;
  T_SpineStatusOverview            spine;
  T_BarcodeReaderStatusOverview    barcodeReader;
} T_ModuleStatusOverview;

/** @ingroup BNR_SYSTEM_STATUS
 * System Operational Status.
 */
typedef enum SystemOperationalStatus {
  SOS_OPERATIONAL,      /**< The system is operational - No errors. */
  SOS_IDLE,             /**< The system is idle due to a configuration change, a power up, reboot or because the system has been opened. */
  SOS_NOT_OPERATIONAL,  /**< The system is not operational - One or more Module have an error. */
  SOS_RESETED           /**< Transitional state at system start up. */
} T_SystemOperationalStatus;

/** @ingroup BNR_SYSTEM_STATUS
 * System Error Code.
 */
typedef enum SystemErrorCode {
  SEC_NO_ERROR,  /**< No error. */
  SEC_OPENED,    /**< At least one of the BNR covers is open. */
  SEC_UNLOCKED   /**< The CashModulesLock is open. */
} T_SystemErrorCode;

/** @ingroup BNR_SYSTEM_STATUS
 * System CashType Status.
 */
typedef enum SystemCashTypeStatus {
  SCTS_UNKNOWN,  /**< Can not determine the CashType status of the module. */
  SCTS_OK,       /**< The CashType status is OK - No errors. */
  SCTS_WARNING,
  SCTS_ERROR
} T_SystemCashTypeStatus;

/** @ingroup BNR_SYSTEM_STATUS
 * System Bill Storage Status.
 */
typedef enum SystemBillStorageStatus {
  SBSS_UNKNOWN,    /**< The status can not be determined, because system is idle, or a loader needs to have its content tested - A bnr_Reset() command must be sent. */
  SBSS_ERROR,      /**< The Cashbox is full. */
  SBSS_NO_CHANGE,  /**< The BNR has no change available. */
  SBSS_OK,         /**< No error : Cashbox is not full, and every loader has bills of the right denomination.*/
  SBSS_WARNING     /**< At least one loader is empty or contains bills of wrong denomination. */
} T_SystemBillStorageStatus;

/** @ingroup BNR_SYSTEM_STATUS
 * Defines the top level status for BNR diagnostics and details information.
 *
 * @see T_SystemStatusOverview.
 */
typedef struct SystemStatus {
  T_SystemOperationalStatus  operationalStatus;    /**< System Operational Status. */
  T_SystemErrorCode          errorCode;            /**< System Error Code. */
  T_BillTransportStatus      billTransportStatus;  /**< Status of the bill transport. */
  T_SystemBillStorageStatus  billStorageStatus;    /**< System Bill Storage Status. */
  T_SystemCashTypeStatus     cashTypeStatus;       /**< System CashType Status. */
} T_SystemStatus;

/** @ingroup BNR_SYSTEM_STATUS
 * Defines the top level structure for BNR diagnostics and details information.
 *
 * The field systemStatus gives a global information about the system. If its 
 * operationalStatus is #SOS_NOT_OPERATIONAL, its errorCode or billTransportStatus 
 * may give the reason of the error if it is a system error. Otherwise, if all 
 * of those fileds have no error, it means that the error comes from one or more 
 * BNR's modules.
 *
 * Module statuses can be checked using the moduleStatusOverviews table, based 
 * on the same principle, as described above. First check the module 
 * operationalStatus. If it is #OS_OPERATIONAL, check the next one. In case the
 * operationalStatus is #OS_NOT_OPERATIONAL :
 * -# If the errorCode is different than #MEC_NO_ERROR, use the error code to 
 * determine recovery action.
 * -# If the errorCode is #MEC_NO_ERROR, check the billTransportStatus and
 *  billStorageStatus field.
 *
 * @see bnr_GetSystemStatus().
 */
typedef struct SystemStatusOverview {
  T_SystemStatus          systemStatus;                               /**< Status of the system. */
  UINT32                  maxSize;                                   /**< Maximum number of T_ModuleStatusOverview items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                  size;                                      /**< Number of T_ModuleStatusOverview items available in the array. */
  T_ModuleStatusOverview  moduleStatusOverviews[MAX_NR_OF_MODULES];  /**< Status overview of all BNR's defined modules. */
} T_SystemStatusOverview;

/** 
 * Size of a Special configuration string (11 chars + 0 terminal).
 */
#define SIZE_OF_SPECIAL_CONFIGURATION (12)

/** @ingroup BNR_SYS_CONFIG
 * Special configuration string.
 *
 * @see bnr_GetSpecialConfiguration(), bnr_SetSpecialConfiguration().
 */
typedef char T_SpecialConfiguration[SIZE_OF_SPECIAL_CONFIGURATION];

/** @ingroup BNR_SYS_CONFIG
 * Modules structure.
 *
 * @see T_ModuleIdList.
 */
typedef struct Modules {
  T_ModuleId  moduleId;
} T_Modules;

/** @ingroup BNR_SYS_CONFIG
 * List of Module Ids.
 *
 * @see bnr_GetModules().
 */
typedef struct ModuleIdList {
  UINT32     size;
  T_Modules  modules[NBMAXMODULES];
} T_ModuleIdList;

/** @ingroup BNR_SYS_CONFIG 
 * Defines different BNR types.
 */
typedef enum BnrType {
  BT_BNA6 =  0,  /**< BNA6: one module (the Cashbox module, no additional module box slot under it).*/
  BT_BNR2 =  1,  /**< BNR2: two modules (the Cashbox module and one module box slot).*/
  BT_BNR3 =  2,  /**< BNR3: three modules (the Cashbox module and two module box slots).*/
  BT_BNR4 =  3,  /**< BNR4: four modules (the Cashbox module and three module box slots).*/
  BT_BNR5 =  4,  /**< BNR5: five modules (the Cashbox module and four module box slots).*/
  BT_BNA6F = 5   /**< BNA6F: BNA6 Free Fall (without Cashbox module and without module box slots).*/
} T_BnrType;

/**@ingroup BNR_SYS_CONFIG 
 * Defines different cashbox capacities.
 */
typedef enum CashboxCapacity {
  CC_SMALL =  0,  /**< Cashbox with a capacity of approximately 600 bills.*/
  CC_MEDIUM = 1,  /**< Cashbox with a capacity of approximately 1200 bills.*/
  CC_LARGE =  2   /**< Cashbox with a capacity of approximately 2000 bills.*/
} T_CashboxCapacity;

/**@ingroup BNR_SYS_CONFIG 
 * Defines different cashbox types.
 */
typedef enum CashboxType {
  CBT_SIMPLE,  /**< Simple Cashbox without PCB.*/
  CBT_SMART    /**< Smart Cashbox with PCB.*/
} T_CashboxType;

/** @ingroup BNR_SYS_CONFIG 
 * Top level system configuration of the BNR.
 *
 * @see bnr_GetSystemConfiguration(), bnr_SetSystemConfiguration().
 */
typedef struct SystemConfiguration {
  T_BnrType          bnrType;               /**< Defines the number of module boxes that can be configured under the cashbox. Default value is #BT_BNR4. */
  T_CashboxCapacity  cashboxSlotCapacity;   /**< Maximum cashbox capacity for the cashbox slot. Default value is #CC_SMALL. */
  T_CashboxType      cashboxSlotType;       /**< Expected cashbox type. Default value is #CBT_SIMPLE. */
  BOOL               barcodeReaderEnabled;  /**< Defines if the BNR is equipped with a Barcode Reader or not. Default value is FALSE. */
} T_SystemConfiguration;

/** @ingroup BNR_SYS_CONFIG 
 * Structure that describes a list of Status Event Ids.
 * This list is filled by bnr_GetEnabledStatusEvents() with currently enabled status events, and 
 * bnr_EnableStatusEvents() allows to define the list of status events that must be enabled.
 *
 * @see bnr_EnableStatusEvents() and bnr_GetEnabledStatusEvents().
 */
typedef struct StatusEventIdList {
  UINT32  size;
  LONG32  statusEventId[MAX_NR_OF_STATUS_EVENTS];
} T_StatusEventIdList;

/** @ingroup BNR_CASH_OPERATIONS 
 * This structure describes the number of bills stored to or dispensed from a Logical Cash Unit.
 *
 * @see T_XfsDenomination.
 */
typedef struct XfsDenominationItem {
  UINT32       unit;   /**< Logical Cash Unit number */
  T_BillCount  count;  /**< Bill count */
} T_XfsDenominationItem;

/** @ingroup BNR_CASH_OPERATIONS BNR_LISTENER
 * This structure handles a list of T_XfsDenominationItems;
 * - it is used as a parameter of bnr_Dispense() or bnr_Denominate() to specifiy a list of banknotes to dispense,
 * - it is also given in a T_XfsCashOrder, when a status event or operation complete is fired.
 *
 * @see T_XfsDispenseRequest, T_XfsCashOrder, bnr_Dispense(), bnr_Denominate().
 */
typedef struct XfsDenomination {
  UINT32                 size;                                 /**< Size of items array. */
  T_Amount               amount;                               /**< Amount in MDU */
  UINT32                 cashbox;                              /**< Amount the BNR cannot denominate or dispense */
  T_XfsDenominationItem  items[MAX_NR_OF_DENOMINATION_ITEMS];  /**< The denomination items */
} T_XfsDenomination;

/** @ingroup BNR_LISTENER
 * XfsCashOrder structure
 *
 * @see T_OperationCompleteOccuredMethod, T_StatusOccuredMethod, T_IntermediateOccuredMethod.
 */
typedef struct XfsCashOrder {
  T_XfsCurrency      currency;      /**< Currency */
  T_XfsDenomination  denomination;  /**< Denomination info */
} T_XfsCashOrder;

/** @ingroup BNR_CASH_OPERATIONS 
 * Structure that defines the parameters of bnr_Dispense() or bnr_Denominate(), to specifiy either an amount or a list of banknotes to dispense.
 *
 * @see bnr_Denominate(), bnr_Dispense().
 */
typedef struct XfsDispenseRequest {
  UINT32             mixNumber;     /**< Type of mixing used, either #XFS_C_CDR_MXA_MIN_BILLS, #BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE or #XFS_C_CDR_MIX_DENOM (#XFS_C_CDR_MIX_TABLE is not used at the moment).*/
  T_XfsDenomination  denomination;  /**< Denomination used */
  T_XfsCurrency      currency;      /**< Name of the currency */
} T_XfsDispenseRequest;

/** @ingroup BNR_CASH_OPERATIONS
 * Types of the different existing external recognition results.
 *
 * @see bnr_SetRecognitionResult().
 */
typedef enum RecognitionResult {
  RR_NONE,        /**< No code                            */
  RR_UNKNOWN,     /**< The code is invalid or unknown     */
  RR_FORGERY,     /**< Not needed for the bar code reader */
  RR_SUSPECT,     /**< Not needed for the bar code reader */
  RR_VALID,       /**< The code is valid                  */
  RR_VALID_UNFIT  /**< Not needed for the bar code reader */
} T_RecognitionResult; 

/**
 * Maximum number of characters in bar code string.
 */
#define MAX_BARCODE_LENGTH (63)

/** @ingroup BNR_LISTENER
 * Bar code string returned in a field data of a #BNRXFS_I_CDR_BCC_INSERTED Intermediate event.
 */
typedef char T_BarcodeChars[MAX_BARCODE_LENGTH + 1];

/** @ingroup BNR_LISTENER
 * Possible reasons of bill rejection during a bnr_CashIn(), returned in a 
 * field data of a #XFS_I_CDR_INPUT_REFUSED Intermediate event. 
 *
 * @see T_IntermediateOccuredMethod.
 */
typedef enum SequenceRejectionCode {
  SRC_UNKNOWN                             =   1,  /**< Unknown rejection code.*/
  SRC_CANCEL_REJECT                       =   2,  /**< Operation has been cancelled by a bnr_Cancel().*/
  SRC_TRANSPORT_EVENT                     =   4,  /**< A jam has occured during bill transportation.*/
  SRC_SYSTEM_EVENT                        =   5,  /**< A hardware failure has occured during bill transportation.*/
  SRC_CONFIGURATION_REJECT                =   6,  /**< Bill refused because its denomination is disabled.*/
  SRC_INSERTION_ERROR_HELD_BACK           = 101,  /**< Document held back at Inlet.*/
  SRC_INSERTION_ERROR_NOT_INSERTED        = 102,  /**< Document not inserted.*/
  SRC_INSERTION_ERROR_POSITIONING_FAILED  = 103,  /**< Document positioning failed.*/
  SRC_INSERTION_ERROR_BAD_SHAPE           = 104,  /**< Bad document shape.*/
  SRC_INSERTION_ERROR_BAD_ROUGH_SHAPE     = 105,  /**< Bad document rough shape.*/
  SRC_INSERTION_ERROR_TOO_THICK           = 106,  /**< Document too thick.*/
  SRC_INSERTION_ERROR_REMOVED             = 107,  /**< Document removed from Inlet.*/
  SRC_INSERTION_ERROR_STRING_DETECTION    = 108,  /**< String detected.*/
  SRC_INSERTION_ERROR_INLET_DETECTION     = 109,  /**< Document rejected due to inlet detection.*/
  SRC_RECOGNITION_CONFUSION               = 201,  /**< Document confusion.*/
  SRC_RECOGNITION_FITNESS_REJECT          = 202,  /**< Bad document fitness state.*/
  SRC_RECOGNITION_FORGERY                 = 203,  /**< Document identified as forgery.*/
  SRC_RECOGNITION_UNKNOWN                 = 204   /**< Document not recognized.*/
} T_SequenceRejectionCode;

/**
 * Maximum number of T_BnrSerialNr items that can be returned by usb_GetBnrSerialNrList().
 */
#define MAX_NR_OF_SERIAL_NR (256)

/**
 * Maximum length of a BNR serial number.
 */
#define SIZE_OF_SERIAL_NR (12)

/** @ingroup USB_INTERFACE
 * Serial number of a BNR.
 *
 * @see usb_GetBnrSerialNrList(), bnr_OpenWithSerialNr().
 */
typedef char T_BnrSerialNr[SIZE_OF_SERIAL_NR+1];

/** @ingroup USB_INTERFACE
 * Serial number list of BNR.
 *
 * @see usb_GetBnrSerialNrList().
 */
typedef struct BnrSerialNrList {
  UINT32         maxSize;                             /**< Maximum number of T_BnrSerialNr items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32         size;
  T_BnrSerialNr  items[MAX_NR_OF_SERIAL_NR];
} T_BnrSerialNrList;


//If symbol DISABLE_DEPRECATED_DEFINITIONS is defined, no deprecated symbol will be defined. By default, DISABLE_DEPRECATED_DEFINITIONS is not defined. 
#ifndef DISABLE_DEPRECATED_DEFINITIONS

/** @name Deprecated definitions */
//@{
#define AFEC_NO_ERROR                                 (FEC_NO_ERROR)                                      /**< @deprecated Use #FEC_NO_ERROR (2009-06-26). */
#define AFEC_CLOSE                                    (FEC_STAYS_CLOSED)                                  /**< @deprecated Use #FEC_STAYS_CLOSED (2009-06-26). */
#define AFEC_COM_BREAKDOWN                            (FEC_COM_BREAKDOWN)                                 /**< @deprecated Use #FEC_COM_BREAKDOWN (2009-06-26). */
#define AFEC_NO_COM                                   (FEC_COM_BREAKDOWN)                                 /**< @deprecated Use #FEC_COM_BREAKDOWN (2009-06-26). */
#define AFFS_UNKNOWN                                  (FFS_UNKNOWN)                                       /**< @deprecated Use #FFS_UNKNOWN (2009-06-26). */
#define AFFS_CLOSE                                    (FFS_CLOSED)                                        /**< @deprecated Use #FFS_CLOSED (2009-06-26). */
#define AFFS_OPEN                                     (FFS_OPENED)                                        /**< @deprecated Use #FFS_OPENED (2009-06-26). */
#define BI_D1                                         (INLET_BILL_SENSOR)                                 /**< @deprecated Use #INLET_BILL_SENSOR (2012-12-04). */
#define BI_D3                                         (BILL_WIDTH_SENSOR)                                 /**< @deprecated Use #BILL_WIDTH_SENSOR (2012-12-04). */
#define BI_D4                                         (BILL_INTAKE_BILL_SENSOR)                           /**< @deprecated Use #BILL_INTAKE_BILL_SENSOR (2012-12-04). */
#define BT_1                                          (BT_BNA6)                                           /**< @deprecated Use #BT_BNA6 (2011-11-09). */
#define BT_2                                          (BT_BNR2)                                           /**< @deprecated Use #BT_BNR2 (2011-11-09). */
#define BT_3                                          (BT_BNR3)                                           /**< @deprecated Use #BT_BNR3 (2011-11-09). */
#define BT_4                                          (BT_BNR4)                                           /**< @deprecated Use #BT_BNR4 (2011-11-09). */
#define BT_5                                          (BT_BNR5)                                           /**< @deprecated Use #BT_BNR5 (2011-11-09). */
#define BT_D1                                         (BOTTOM_TRANSPORT_BILL_SENSOR)                      /**< @deprecated Use #BOTTOM_TRANSPORT_BILL_SENSOR (2012-12-04). */
#define BT_D2                                         (OUTLET_BILL_SENSOR)                                /**< @deprecated Use #OUTLET_BILL_SENSOR (2012-12-04). */
#define BT_M1                                         (THREE_WAY_DIVERTER_MOTOR)                          /**< @deprecated Use #THREE_WAY_DIVERTER_MOTOR (2012-12-04). */
#define BTS_BILL_STOPPED                              (BTS_BILL_STOP)                                     /**< @deprecated Use #BTS_BILL_STOP (2008-10-06). */
#define BTS_UNKNOWN                                   (BTSE_UNKNOWN)                                      /**< @deprecated Use #BTSE_UNKNOWN (2008-12-09). */
#define BTS_HI_SP_INTERFACE                           (BTSE_RS_SP_INTERFACE_FW)                           /**< @deprecated Use #BTSE_RS_SP_INTERFACE_FW (2008-10-06). */
#define BTS_MEASURING_HEAD                            (BTSE_RECOGNITION_SENSOR_FW)                        /**< @deprecated Use #BTSE_RECOGNITION_SENSOR_FW (2008-10-06). */
#define BU_D1                                         (BILL_AND_DRUM_SENSOR)                              /**< @deprecated Use #BILL_AND_DRUM_SENSOR (2012-12-04). */
#define BUNDLER_MOTOR                                 (BUNDLER_TRANSPORT_MOTOR)                           /**< @deprecated Use #BUNDLER_TRANSPORT_MOTOR (2012-12-04). */
#define BUNDLER_DIV_SYSTEM_ANGULAR_POSITION_DETECTOR  (BUNDLER_DIVERTER_SYSTEM_ANGULAR_POSITION_SENSOR)   /**< @deprecated Use #BUNDLER_DIVERTER_SYSTEM_ANGULAR_POSITION_SENSOR (2012-12-04). */
#define BUNDLER_DIV_SYSTEM_DIVERTER_POSITION_DETECTOR (BUNDLER_DIVERTER_SYSTEM_DIVERTER_POSITION_SENSOR)  /**< @deprecated Use #BUNDLER_DIVERTER_SYSTEM_DIVERTER_POSITION_SENSOR (2012-12-04). */
#define BUNDLER_DIV_SYSTEM_MOTOR                      (BUNDLER_DIVERTER_SYSTEM_MOTOR)                     /**< @deprecated Use #BUNDLER_DIVERTER_SYSTEM_MOTOR (2012-12-04). */
#define BUNDLERDIVSYSTEM                              (BUNDLER_DIV_SYSTEM_ID)                             /**< @deprecated Use #BUNDLER_DIV_SYSTEM_ID (2009-01-26). */
#define CC_600_BILLS                                  (CC_SMALL)                                          /**< @deprecated Use #CC_SMALL (2011-11-09). */
#define CC_1200_BILLS                                 (CC_MEDIUM)                                         /**< @deprecated Use #CC_MEDIUM (2011-11-09). */
#define CEC_NO_ERROR                                  (MODEC_NO_ERROR)                                    /**< @deprecated Use #MODEC_NO_ERROR (20012-02-28).*/        
#define CEC_COM_BREAKDOWN                             (MODEC_COM_BREAKDOWN)                               /**< @deprecated Use #MODEC_COM_BREAKDOWN (20012-02-28).*/  
#define CEC_INCOMPATIBLE_MODULE                       (MODEC_INCOMPATIBLE_MODULE)                         /**< @deprecated Use #MODEC_INCOMPATIBLE_MODULE (20012-02-28).*/ 
#define CEC_MODULE_MISSING                            (MODEC_MISSING_MODULE)                              /**< @deprecated Use #MODEC_MISSING_MODULE (20012-02-28).*/ 
#define CEC_WRONG_MODULE_TYPE                         (MODEC_WRONG_MODULE_TYPE)                           /**< @deprecated Use #MODEC_WRONG_MODULE_TYPE (20012-02-28).*/ 
#define CEC_BOOT_RUNNING                              (MODEC_BOOT_RUNNING)                                /**< @deprecated Use #MODEC_BOOT_RUNNING (20012-02-28).*/ 
#define CEC_MODULE_NOT_DETECTED                       (MODEC_MODULE_NOT_DETECTED)                         /**< @deprecated Use #MODEC_MODULE_NOT_DETECTED (20012-02-28).*/ 
#define CEC_SHUTTER_CLOSED                            (MODEC_SHUTTER_CLOSED)                              /**< @deprecated Use #MODEC_SHUTTER_CLOSED (20012-02-28).*/ 
#define CEC_NO_COM                                    (MODEC_NO_COM)                                      /**< @deprecated Use #MODEC_NO_COM (20012-02-28).*/ 
#define CT_ANTI_FISHING_FLAP_PROXY                    (CT_FLAP)                                           /**< @deprecated Use #CT_FLAP (2012-12-07). */
#define CT_HIBOU                                      (CT_RECOGNITION_SENSOR)                             /**< @deprecated Use #CT_RECOGNITION_SENSOR (2013-04-10). */
#define CT_POSITIONNER                                (CT_POSITIONER)                                     /**< @deprecated Use #CT_POSITIONER (2009-01-26). */
#define HEC_NO_ERROR                                  (RSEC_NO_ERROR)                                     /**< @deprecated Use #RSEC_NO_ERROR (2009-06-26). */
#define HEC_OFFSET_COMPENSATION_ERROR                 (RSEC_OFFSET_COMPENSATION_ERROR)                    /**< @deprecated Use #RSEC_OFFSET_COMPENSATION_ERROR (2009-06-26). */
#define HEC_REMAINING_OFFSET_TOO_LOW                  (RSEC_REMAINING_OFFSET_TOO_LOW)                     /**< @deprecated Use #RSEC_REMAINING_OFFSET_TOO_LOW (2009-06-26). */
#define HEC_REMAINING_OFFSET_TOO_HIGH                 (RSEC_REMAINING_OFFSET_TOO_HIGH)                    /**< @deprecated Use #RSEC_REMAINING_OFFSET_TOO_HIGH (2009-06-26). */
#define HEC_CANNOT_REGULATE_FOR_DSR_IDENT             (RSEC_CANNOT_REGULATE_FOR_DSR_IDENT)                /**< @deprecated Use #RSEC_CANNOT_REGULATE_FOR_DSR_IDENT (2009-06-26). */
#define HEC_C_COEF_DIV_BY_ZERO                        (RSEC_C_COEF_DIV_BY_ZERO)                           /**< @deprecated Use #RSEC_C_COEF_DIV_BY_ZERO (2009-06-26). */
#define HEC_C_COEF_NEG_VALUE                          (RSEC_C_COEF_NEG_VALUE)                             /**< @deprecated Use #RSEC_C_COEF_NEG_VALUE (2009-06-26). */
#define HEC_C_COEF_OVERFLOW                           (RSEC_C_COEF_OVERFLOW)                              /**< @deprecated Use #RSEC_C_COEF_OVERFLOW (2009-06-26). */
#define HEC_C_COEF_OUT_OF_RANGE                       (RSEC_C_COEF_OUT_OF_RANGE)                          /**< @deprecated Use #RSEC_C_COEF_OUT_OF_RANGE (2009-06-26). */
#define HEC_CANNOT_REGULATE                           (RSEC_CANNOT_REGULATE)                              /**< @deprecated Use #RSEC_CANNOT_REGULATE (2009-06-26). */
#define HIBOU                                         (RECOGNITION_SENSOR_ID)                             /**< @deprecated Use #RECOGNITION_SENSOR_ID (2009-01-26). */
#define HIBOU_BOARD_ID                                (RECOGNITION_SENSOR_BOARD_ID)                       /**< @deprecated Use #RECOGNITION_SENSOR_BOARD_ID (2013-04-10). */
#define HIBOU_COVER                                   (RECOGNITION_SENSOR_COVER)                          /**< @deprecated Use #RECOGNITION_SENSOR_COVER (2012-12-04). */
#define HIBOU_DOWN_DETECTOR                           (RECOGNITION_SENSOR_BILL_SENSOR_DOWN)               /**< @deprecated Use #RECOGNITION_SENSOR_BILL_SENSOR_DOWN (2012-12-04). */
#define HIBOU_ID                                      (RECOGNITION_SENSOR_ID)                             /**< @deprecated Use #RECOGNITION_SENSOR_ID (2012-12-04). */
#define HIBOU_UP_DETECTOR                             (RECOGNITION_SENSOR_BILL_SENSOR_UP)                 /**< @deprecated Use #RECOGNITION_SENSOR_BILL_SENSOR_UP (2012-12-04). */
#define JXFS_S_BIN_OK                                 (XFS_S_BIN_OK)                                      /**< @deprecated Use #XFS_S_BIN_OK (2009-09-29).*/
#define JXFS_S_BIN_FULL                               (XFS_S_BIN_FULL)                                    /**< @deprecated Use #XFS_S_BIN_FULL (2009-09-29). */
#define JXFS_S_BIN_HIGH                               (XFS_S_BIN_HIGH)                                    /**< @deprecated Use #XFS_S_BIN_HIGH (2009-09-29).*/
#define JXFS_S_BIN_LOW                                (XFS_S_BIN_LOW)                                     /**< @deprecated Use #XFS_S_BIN_LOW (2009-09-29).*/
#define JXFS_S_BIN_EMPTY                              (XFS_S_BIN_EMPTY)                                   /**< @deprecated Use #XFS_S_BIN_EMPTY (2009-09-29).*/
#define JXFS_S_BIN_UNKNOWN                            (XFS_S_BIN_UNKNOWN)                                 /**< @deprecated Use #XFS_S_BIN_UNKNOWN (2009-09-29).*/
#define JXFS_S_BIN_NOTSUPPORTED                       (XFS_S_BIN_NOTSUPPORTED)                            /**< @deprecated Use #XFS_S_BIN_NOTSUPPORTED (2009-09-29).*/
#define LOADER_1_ANTIFISHING_FLAP                     (0x120001)                                          /**< @deprecated See #MIN_ANTIFISHING_FLAP_ID. (2012-12-17). */
#define LOADER_1_TRANSPORT_MOTOR                      (0x070032)                                          /**< @deprecated See #MIN_LOADER_MOTOR_ID. (2013-04-15). */
#define LOADER_2_ANTIFISHING_FLAP                     (0x120002)                                          /**< @deprecated See #MIN_ANTIFISHING_FLAP_ID. (2012-12-17). */
#define LOADER_2_PRESSURE_PLATE                       (0x110002)                                          /**< @deprecated See #MIN_PRESSURE_PLATE_ID. (2012-12-17). */
#define LOADER1ANTIFISHINGFLAP                        (ANTIFISHING_FLAP_CLASS + 0x01)                     /**< @deprecated Use #ANTIFISHING_FLAP_CLASS + Loader number (2012-12-04). */
#define LOADER2ANTIFISHINGFLAP                        (ANTIFISHING_FLAP_CLASS + 0x02)                     /**< @deprecated Cannot exist in a BNR (2012-12-04). */
#define LOADER1PRESSUREPLATE                          (PRESSURE_PLATE_CLASS + 0x01)                       /**< @deprecated Use #LOADER_1_PRESSURE_PLATE (2012-12-04). */
#define LOADER2PRESSUREPLATE                          (PRESSURE_PLATE_CLASS + 0x02)                       /**< @deprecated Cannot exist in a BNR (2012-12-04). */
#define MMEC_MAIN_BOOT_RUNNING                        (MMEC_BOOT_RUNNING)                                 /**< @deprecated Use #MMEC_BOOT_RUNNING (2008-11-28). */
#define MODEC_BOOT                                    (MODEC_BOOT_RUNNING)                                /**< @deprecated Use #MODEC_BOOT_RUNNING (2008-11-28). */
#define MOS_OPERATIONAL                               (OS_OPERATIONAL)                                    /**< @deprecated Use #OS_OPERATIONAL (2011-05-04). */
#define MOS_CHECKING_ERROR                            (OS_CHECKING_ERROR)                                 /**< @deprecated Use #OS_CHECKING_ERROR (2011-05-04). */
#define MOS_NOT_OPERATIONAL                           (OS_NOT_OPERATIONAL)                                /**< @deprecated Use #OS_NOT_OPERATIONAL (2011-05-04). */
#define MOS_SW_LOCKED                                 (OS_SW_LOCKED)                                      /**< @deprecated Use #OS_SW_LOCKED (2011-05-04). */
#define OUTLET_FLAP_SENSOR                            (0x030403)                                          /**< @deprecated - Sensor identifier of Outlet Flap. To locate the Outlet Flap, see diagram @ref BNR_FUNCTIONAL_ELEMENTS "BNR Modules &amp; Functional Elements". \n\n <b>Decimal value:</b> 197635. */
#define POS_POSITIONNING                              (POS_POSITIONING)                                   /**< @deprecated Use #POS_POSITIONING (2009-01-26). */
#define POSITIONNER                                   (POSITIONER_ID)                                     /**< @deprecated Use #POSITIONNER_ID (2009-01-26). */
#define POSITIONNER_ID                                (POSITIONER_ID)                                     /**< @deprecated Use #POSITIONER_ID (2009-01-26). */
#define PR_D1                                         (POSITIONER_BILL_SENSOR)                            /**< @deprecated Use #POSITIONER_BILL_SENSOR (2012-12-04). */
#define RE1_D1                                        (RECYCLER_1_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_1_BILL_SENSOR (2012-12-04). */
#define RE2_D1                                        (RECYCLER_2_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_2_BILL_SENSOR (2012-12-04). */
#define RE3_D1                                        (RECYCLER_3_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_3_BILL_SENSOR (2012-12-04). */
#define RE4_D1                                        (RECYCLER_4_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_4_BILL_SENSOR (2012-12-04). */
#define RE5_D1                                        (RECYCLER_5_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_5_BILL_SENSOR (2012-12-04). */
#define RE6_D1                                        (RECYCLER_6_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_6_BILL_SENSOR (2012-12-04). */
#define RE7_D1                                        (RECYCLER_7_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_7_BILL_SENSOR (2012-12-04). */
#define RE8_D1                                        (RECYCLER_8_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_8_BILL_SENSOR (2012-12-04). */
#define RE9_D1                                        (RECYCLER_9_BILL_SENSOR)                            /**< @deprecated Use #RECYCLER_9_BILL_SENSOR (2012-12-04). */
#define RE10_D1                                       (RECYCLER_10_BILL_SENSOR)                           /**< @deprecated Use #RECYCLER_10_BILL_SENSOR (2012-12-04). */
#define RECYCLER_1_TRANSPORT_MOTOR                    (0x070001)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_2_TRANSPORT_MOTOR                    (0x070002)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_3_TRANSPORT_MOTOR                    (0x070003)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_4_TRANSPORT_MOTOR                    (0x070004)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_5_TRANSPORT_MOTOR                    (0x070005)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_6_TRANSPORT_MOTOR                    (0x070006)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_7_TRANSPORT_MOTOR                    (0x070007)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_8_TRANSPORT_MOTOR                    (0x070008)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_9_TRANSPORT_MOTOR                    (0x070009)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_10_TRANSPORT_MOTOR                   (0x07000A)                                          /**< @deprecated See #MIN_RECYCLER_MOTOR_ID. (2013-04-15). */
#define RECYCLER_1_MOTOR                              (RECYCLER_1_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_1_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_2_MOTOR                              (RECYCLER_2_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_2_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_3_MOTOR                              (RECYCLER_3_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_3_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_4_MOTOR                              (RECYCLER_4_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_4_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_5_MOTOR                              (RECYCLER_5_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_5_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_6_MOTOR                              (RECYCLER_6_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_6_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_7_MOTOR                              (RECYCLER_7_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_7_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_8_MOTOR                              (RECYCLER_8_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_8_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_9_MOTOR                              (RECYCLER_9_TRANSPORT_MOTOR)                        /**< @deprecated Use #RECYCLER_9_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_10_MOTOR                             (RECYCLER_10_TRANSPORT_MOTOR)                       /**< @deprecated Use #RECYCLER_10_TRANSPORT_MOTOR (2012-12-04). */
#define RECYCLER_1_TAPE_POSITION                      (0x0D0001)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_2_TAPE_POSITION                      (0x0D0002)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_3_TAPE_POSITION                      (0x0D0003)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_4_TAPE_POSITION                      (0x0D0004)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_5_TAPE_POSITION                      (0x0D0005)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_6_TAPE_POSITION                      (0x0D0006)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_7_TAPE_POSITION                      (0x0D0007)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_8_TAPE_POSITION                      (0x0D0008)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_9_TAPE_POSITION                      (0x0D0009)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define RECYCLER_10_TAPE_POSITION                     (0x0D000A)                                          /**< @deprecated See #MIN_TAPE_POSITION_ID. (2012-12-17). */
#define LO1_M1                                        (LOADER_1_TRANSPORT_MOTOR)                          /**< @deprecated Use #LOADER_1_TRANSPORT_MOTOR (2012-12-04). */
#define LOADER_1_PRESSURE_PLATE                       (0x110001)                                          /**< @deprecated See #MIN_PRESSURE_PLATE_ID. (2012-12-17). */
#define OUTLET_FLAP_DETECTOR                          (OUTLET_FLAP_SENSOR)                                /**< @deprecated Use #OUTLET_FLAP_SENSOR (2012-12-04). */
#define SIZE_OF_COMPONENT_DETAILS                     (SIZE_OF_MODULE_USER_DATA)                          /**< @deprecated No more used (2011-06-22). */
#define sizeOfComponentDetails                        (SIZE_OF_COMPONENT_DETAILS)                         /**< @deprecated No more used (2009-09-29). */
#define sizeOfCurrencyCode                            (SIZE_OF_CURRENCY_CODE)                             /**< @deprecated Use #SIZE_OF_CURRENCY_CODE (2009-09-29). */
#define sizeOfPhysicalName                            (SIZE_OF_PHYSICAL_NAME)                             /**< @deprecated Use #SIZE_OF_PHYSICAL_NAME (2009-09-29). */
#define sizeOfUnitId                                  (SIZE_OF_UNIT_ID)                                   /**< @deprecated Use #SIZE_OF_UNIT_ID (2009-09-29). */
#define SP_D12                                        (SPINE_BILL_SENSOR_1)                               /**< @deprecated Use #SPINE_BILL_SENSOR_1 (2012-12-04). */
#define SP_D32                                        (SPINE_BILL_SENSOR_3)                               /**< @deprecated Use #SPINE_BILL_SENSOR_3 (2012-12-04). */
#define SP_D52                                        (SPINE_BILL_SENSOR_5)                               /**< @deprecated Use #SPINE_BILL_SENSOR_5 (2012-12-04). */
#define SP_D72                                        (SPINE_BILL_SENSOR_7)                               /**< @deprecated Use #SPINE_BILL_SENSOR_7 (2012-12-04). */
#define SP_D92                                        (SPINE_BILL_SENSOR_9)                               /**< @deprecated Use #SPINE_BILL_SENSOR_9 (2012-12-04). */
#define SP_DIV1                                       (SPINE_DIVERTER_CLASS + 0x01)                       /**< @deprecated See #MIN_SPINE_DIVERTER_ID. (2012-12-04). */
#define SP_DIV2                                       (SPINE_DIVERTER_CLASS + 0x02)                       /**< @deprecated See #MIN_SPINE_DIVERTER_ID. (2012-12-04). */
#define SP_DIV3                                       (SPINE_DIVERTER_CLASS + 0x03)                       /**< @deprecated See #MIN_SPINE_DIVERTER_ID. (2012-12-04). */
#define SP_DIV4                                       (SPINE_DIVERTER_CLASS + 0x04)                       /**< @deprecated See #MIN_SPINE_DIVERTER_ID. (2012-12-04). */
#define SP_DIV5                                       (SPINE_DIVERTER_CLASS + 0x05)                       /**< @deprecated See #MIN_SPINE_DIVERTER_ID. (2012-12-04). */
#define SPINEDIVERTERMAX                              (MAX_SPINE_DIVERTER_ID)                             /**< @deprecated Use #MAX_SPINE_DIVERTER_ID (2012-12-04). */
#define SPINEDIVERTERMIN                              (SPINE_DIVERTER_CLASS)                              /**< @deprecated See #MIN_SPINE_DIVERTER_ID or use #SPINE_DIVERTER_CLASS (2012-12-04). */
#define STACKER                                       (STACKER_ID)                                        /**< @deprecated Use #STACKER_ID (2009-01-26). */
#define STACKER_HOME_POSITION_DETECTOR                (STACKER_HOME_POSITION_SENSOR)                      /**< @deprecated Use #STACKER_HOME_POSITION_SENSOR (2012-12-04). */

#define T_AntiFishingErrorCode        T_FlapErrorCode               /**< @deprecated Use #T_FlapErrorCode (2009-06-26). */
#define T_AntiFishingFunctionalStatus T_FlapFunctionalStatus        /**< @deprecated Use #T_FlapFunctionalStatus (2009-06-26). */
#define T_AntiFishingStatus           T_FlapStatus                  /**< @deprecated Use #T_FlapStatus (2009-06-26) */
#define T_CashboxErrorCode            T_ModuleErrorCode             /**< @deprecated Use #T_ModuleErrorCode (2012-02-28).*/
#define T_DetectorFunctionalStatus    T_SensorFunctionalStatus      /**< @deprecated Use #T_SensorFunctionalStatus (2009-01-26). */
#define T_DetectorErrorCode           T_SensorErrorCode             /**< @deprecated Use #T_SensorErrorCode (2009-06-26). */
#define T_DetectorStatus              T_SensorStatus                /**< @deprecated Use #T_SensorStatus (2009-06-26). */
#define T_HibouErrorCode              T_RecognitionSensorErrorCode  /**< @deprecated Use #T_RecognitionSensorErrorCode (2009-06-26). */
#define T_HibouStatus                 T_RecognitionSensorStatus     /**< @deprecated Use #T_RecognitionSensorStatus (2009-06-26). */
#define T_ModuleOperationalStatus     T_OperationalStatus           /**< @deprecated Use #T_OperationalStatus (2011-05-04). */
#define T_PositionnerStatus           T_PositionerStatus            /**< @deprecated Use #T_PositionerStatus (2009-01-26). */

typedef char T_XfsComponentDetails[SIZE_OF_COMPONENT_DETAILS];      /**< @deprecated No more used (2011-06-22). */
typedef T_XfsThresholdStatus T_ThresholdStatus ;                    /**< @deprecated Use #T_XfsThresholdStatus (2009-09-29). */
/** @deprecated No more used (2009-06-26).*/
typedef union ElementStatusItem {                                                     
  T_ElementStatus       elementStatus;
  T_SensorStatus        sensorStatus;
  T_SensorStatus        detectorStatus;
  T_MotorStatus         motorStatus;
  T_DiverterStatus      diverterStatus;
  T_TapePositionStatus  tapePositionStatus;
} T_ElementStatusItem;                                              

//@} Deprecated

#endif //DISABLE_DEPRECATED_DEFINITIONS

// restore packing
#ifndef WIN32
#pragma pack(pop)
#else
#include <poppack.h>
#endif

#endif // _BNR_PROP_W32_H_
