/******************************************************************************
 *
 *         BNR SDK Project
 *
 ******************************************************************************
 *
 * File :   XfsConsts.h
 * 
 * Author : MEI Inc.
 *
 * Date :   10.jan.2007
 *
 * History of modifications
 * Date       Rev.    Reason
 *
 ******************************************************************************
 * FILE DESCRIPTION *//** @file
 *   Transcription of constants defined in J/XFS
 *
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

#ifndef _XFSCONSTS_H_
#define _XFSCONSTS_H_

/************************ Defines and typedefs *******************************/

/*
 * Defines for configuration
 */
#define XFSERR               (1000)  /**< General constant: Any error start at number... */
#define XFSERREXT            (2000)  /**< General constant: Any extended Error starts at number ... */
#define XFSDEVICE_OFFSET     (3000)  /**< Offset constant: Any code defined by a specific device operation should start from this offset in order not to mix up with J/XFS definitions. */
#define XFS_CDR_OFFSET       (6000)  /**< Offset constant of known device types: CDR */
#define XFSDIRECTIO_OFFSET  (30000)  /**< Offset constant: Any rc or code defined by a specific device for a direct-IO operation should start from this offset in order not to mix up with J/XFS definitions. */
#define XFS_RC_SUCCESSFUL       (0)  /**< The operation complete successfully. */

/** @ingroup XFS_ERROR
 */
//@{
#define XFS_E_ILLEGAL           (1010)  /**< Illegal request. */
#define XFS_E_NOHARDWARE        (1011)  /**< The module is not connected to the BNR. */
#define XFS_E_NOEXIST           (1013)  /**< The requested item (reports or other data) does not exist. */
#define XFS_E_FAILURE           (1015)  /**< The operation failed. */
#define XFS_E_BUSY              (1017)  /**< The BNR is busy (moving or about to move bill). */
#define XFS_E_PARAMETER_INVALID (1018)  /**< One of the parameters given was invalid. */
#define XFS_E_NOT_SUPPORTED     (1022)  /**< The operation is not supported by this firmware. */
#define XFS_E_NOT_USED            (-1)  /**< Field not used. */
#define XFS_E_CANCELLED         (1021)  /**< The operation was cancelled. */
#define XFS_E_FIRMWARE          (1023)  /**< Error during firmware update or no runnable firmware in device. */
#define XFS_E_HARDWAREERROR     (1025)  /**< Error occurred that is hardware related. */
//@}

/** @ingroup BNR_SYSTEM_STATUS
 * Input and output positions.
 */
typedef enum XfsCdrPosition {
  XFS_C_CDR_POS_TOP =     1,  /**< Inlet position of the BNR. */
  XFS_C_CDR_POS_BOTTOM =  2   /**< Outlet position of the BNR. */
} T_XfsCdrPosition;

/* Cash Module configuration */
#define XFS_C_CDR_TYPE_NONE             (6010)
#define XFS_C_CDR_TYPE_DISPENSER        (6011)
#define XFS_C_CDR_TYPE_RECYCLER         (6012)
#define XFS_C_CDR_TYPE_ATM              (6013)
#define XFS_C_CDR_CURR_BILL             (6014)
#define XFS_C_CDR_CURR_COIN             (6015)
#define XFS_C_CDR_LCU_DISPENSE          (6016)
#define XFS_C_CDR_LCU_DEPOSIT           (6017)
#define XFS_C_CDR_LCU_RECYCLE           (6018)
#define XFS_C_CDR_LCU_BAIT_TRAP         (6020)
#define XFS_C_CDR_LCU_NA                (6019)
#define XFS_C_CDR_LCU_REJECT_CASSETTE   (6021)
#define XFS_C_CDR_LCU_OVERFLOW_CASSETTE (6022)
#define XFS_C_CDR_LCU_BILL_CASSETTE     (6023)
#define XFS_C_CDR_LCU_COIN_CYLINDER     (6024)
#define XFS_C_CDR_LCU_COIN_DISPENSER    (6025)
#define XFS_C_CDR_LCU_RETRACT_CASSETTE  (6026)
#define XFS_C_CDR_LCU_COUPON            (6027)
#define XFS_C_CDR_LCU_CURRENCY_CASSETTE (6059)
#define XFS_C_CDR_LCU_DOCUMENT          (6028)
#define XFS_C_CDR_LCU_ESCROW            (6029)
#define XFS_C_CDR_LCU_UNKNOWN           (6030)
#define XFS_C_CDR_LCU_OK                (6031)
#define XFS_C_CDR_LCU_FULL              (6032)
#define XFS_C_CDR_LCU_HIGH              (6033)
#define XFS_C_CDR_LCU_LOW               (6034)
#define XFS_C_CDR_LCU_EMPTY             (6035)
#define XFS_C_CDR_LCU_INOP              (6036)
#define XFS_C_CDR_LCU_MISSING           (6037)
#define XFS_C_CDR_LCU_NO_VALUE          (6038)
#define XFS_C_CDR_LCU_NO_REF            (6039)
#define XFS_C_CDR_LCU_NOT_DISPENSEABLE  (6040)

/* Cash Module Dispense algorithm definitions */
#define XFS_C_CDR_MIX_ALGORITHM       (XFS_CDR_OFFSET + 41)  /**< @deprecated Use #XFS_C_CDR_MXA_MIN_BILLS. */
#define XFS_C_CDR_MIX_TABLE           (XFS_CDR_OFFSET + 42)
#define XFS_C_CDR_MIX_DENOM           (XFS_CDR_OFFSET + 43)
#define XFS_C_CDR_MXA_ALGORITHM_BASE  (XFS_CDR_OFFSET + 63)  /**< Base constant for vendor specific mix algorithm. */
#define XFS_C_CDR_MXA_MIN_BILLS       (XFS_CDR_OFFSET + 44)  /**< Best change mix algorithm. */

/* BNR Dispense algorithm definitions */
#define BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE (XFS_C_CDR_MXA_ALGORITHM_BASE + 1)  /**< Change protecting the cash box filling. */

#define XFS_C_CDR_RA_REJECT       (6064)
#define XFS_C_CDR_RA_RETRACT      (6065)
#define XFS_C_CDR_RA_STACKER      (6066)
#define XFS_C_CDR_RA_TRANSPORT    (6067)
#define XFS_C_CDR_DO_NONE         (6068)
#define XFS_C_CDR_DO_DISPENSABLE  (6046)
#define XFS_C_CDR_DO_DELAYED      (6047)
#define XFS_C_CDR_DO_LAQ          (6048)
#define XFS_C_CDR_DO_ALL          (6049)
#define XFS_C_CDR_NO_VARIANT      (6050)

/* Cash Module error */
#define XFS_E_CDR_OFFSET                  (XFS_CDR_OFFSET + 71)
#define XFS_E_CDR_CASHIN_ACTIVE           (XFS_E_CDR_OFFSET + 1)  /**< The device has already issued a cashInStart command. */
#define XFS_E_CDR_CASH_DEVICE_ERROR       (XFS_E_CDR_OFFSET + 2)  /**< A selected cash device caused an error. */
#define XFS_E_CDR_CASH_UNIT_ERROR         (6074)                  /**< A selected cash unit caused an error. */
#define XFS_E_CDR_INVALID_CASH_UNIT       (6080)                  /**< Invalid cash unit ID. */
#define XFS_E_CDR_INVALID_CURRENCY        (6081)                  /**< The currency is not available (defined) in the BNR. */
#define XFS_E_CDR_INVALID_MIXNUMBER       (6085)                  /**< The number refers to a defined and reserved mix algorithm. */
#define XFS_E_CDR_NOT_DISPENSABLE         (6087)                  /**< The amount is not dispensable. */
#define XFS_E_CDR_NO_BILLS                (6088)                  /**< There were no bills on the stacker to present.*/
#define XFS_E_CDR_NO_CASHIN_STARTED       (6089)                  /**< CashInStart was not called. */
#define XFS_E_CDR_TOO_MANY_BILLS          (6092)                  /**< The request would require too many bills to be dispensed. */
#define BNRXFS_E_CDR_USER_ERROR           (XFSERREXT + 1)         /**< A person has stopped the device operation. i.e. manupulating the outlet flap or inserting a bill at the inlet when it is not expected. */

/* Cash Module operation */
#define XFS_O_CDR_OFFSET                  (6106)
#define XFS_O_CDR_DENOMINATE              (6107)
#define XFS_O_CDR_DISPENSE                (6108)
#define XFS_O_CDR_QUERY_CASH_UNIT         (6114)
#define XFS_O_CDR_RESET                   (6117)
#define XFS_O_CDR_UPDATE_CASH_UNIT        (6118)
#define XFS_O_CDR_GET_DATE_TIME           (6119)
#define XFS_O_CDR_SET_DATE_TIME           (6120)
#define XFS_O_CDR_QUERY_DENOMINATIONS     (6181)
#define XFS_O_CDR_UPDATE_DENOMINATIONS    (6182)
#define XFS_O_CDR_CASH_IN_START           (6121)
#define XFS_O_CDR_CASH_IN                 (6122)
#define XFS_O_CDR_CASH_IN_END             (6123)
#define XFS_O_CDR_CASH_IN_ROLLBACK        (6124)
#define XFS_O_CDR_EMPTY                   (6125)
#define XFS_O_CDR_PRESENT                 (6126)
#define XFS_O_CDR_REJECT                  (6127)
#define XFS_O_CDR_RETRACT                 (6128)

/* BNR specific operation */
#define BNRXFS_O_CDR_NO_OPERATION            (XFSDIRECTIO_OFFSET)
#define BNRXFS_O_CDR_MODULE_UPDATE_FIRMWARE  (XFSDIRECTIO_OFFSET + 4)
#define BNRXFS_O_CDR_ADD_DENOMINATION        (XFSDIRECTIO_OFFSET + 9)
#define BNRXFS_O_CDR_DELETE_DENOMINATION     (XFSDIRECTIO_OFFSET + 10)
#define BNRXFS_O_BNR_CALIBRATE_WITH_COUPON   (XFSDIRECTIO_OFFSET + 14)
#define BNRXFS_O_CDR_SELF_TEST               (XFSDIRECTIO_OFFSET + 22)
#define BNRXFS_O_CDR_MODULE_PARK             (XFSDIRECTIO_OFFSET + 42)
#define BNRXFS_O_CDR_PARK                    (XFSDIRECTIO_OFFSET + 43)
#define BNRXFS_O_CDR_CONFIGURE_CASH_UNIT     (XFSDIRECTIO_OFFSET + 44)
#define BNRXFS_O_CDR_EJECT                   (XFSDIRECTIO_OFFSET + 57)

/* Deprecated constants */
#define XFS_O_UPDATE_FIRMWARE                (BNRXFS_O_CDR_MODULE_UPDATE_FIRMWARE)  /**< @deprecated Use #BNRXFS_O_CDR_MODULE_UPDATE_FIRMWARE. */
#define XFS_O_ADD_DENOMINATION               (BNRXFS_O_CDR_ADD_DENOMINATION)        /**< @deprecated Use #BNRXFS_O_CDR_ADD_DENOMINATION. */
#define XFS_O_DELETE_DENOMINATION            (BNRXFS_O_CDR_DELETE_DENOMINATION)     /**< @deprecated Use #BNRXFS_O_CDR_DELETE_DENOMINATION. */
#define XFS_O_BNR_CALIBRATE_WITH_COUPON      (BNRXFS_O_BNR_CALIBRATE_WITH_COUPON)   /**< @deprecated Use #BNRXFS_O_BNR_CALIBRATE_WITH_COUPON. */
#define XFS_O_BNR_SELF_TEST                  (BNRXFS_O_CDR_SELF_TEST)               /**< @deprecated Use #BNRXFS_O_CDR_SELF_TEST. */

/* BNR Automatic operation */
#define BNRXFS_O_CDR_AUTOMATIC_BILL_TRANSFER (XFSDIRECTIO_OFFSET + 100)

/* Specfic device Event */
#define BNRXFS_INTERMEDIATE_CDR_OFFSET          (XFSDEVICE_OFFSET)      
#define BNRXFS_STATUS_CDR_OFFSET                (XFSDEVICE_OFFSET + 100)      

/* Cash Module intermediate event */
#define XFS_I_CDR_OFFSET                        (6140)
#define XFS_I_CDR_INPUT_REFUSED                 (6209)                                /**< Input refused. */
#define BNRXFS_I_CDR_SUBCASHIN                  (BNRXFS_INTERMEDIATE_CDR_OFFSET)      /**< A bill has been recognized during a cash in transaction, but the amount requested has not been reached yet. */
#define BNRXFS_I_CDR_BCC_INSERTED               (BNRXFS_INTERMEDIATE_CDR_OFFSET + 1)  /**< A coupon with barcode has been inserted and recognized during a cash in transaction, the BNR then waits for a bnr_SetRecognitionResult() call.*/
   
/* Cash Module status */
#define XFS_S_CDR_OFFSET                          (6152)
#define XFS_S_CDR_CASHUNIT_CHANGED                (6153)                      /**< One or more cash unit content has been changed. */
#define XFS_S_CDR_CASHUNIT_CONFIGURATION_CHANGED  (6154)                      /**< The cashunit configuration was changed. A physical cashunit has been inserted or removed from the BNR. */
#define XFS_S_CDR_CASHUNIT_THRESHOLD              (6155)                      /**< The Threshold status has changed.  */
#define XFS_S_CDR_DEVICE_STATUS_CHANGED           (6162)                      /**< The device status has been changed. */
#define XFS_S_CDR_TRANSPORT_CHANGED               (6167)                      /**< @deprecated No more used. */
#define XFS_S_CDR_DS_HARDWARE_ERROR               (6174)                      /**< The device is not operational due to a hardware error. */
#define XFS_S_CDR_DS_USER_ERROR                   (6175)                      /**< A person is preventing proper device operation. */
#define XFS_S_CDR_DS_OFF_LINE                     (6179)                      /**< The device is not operational. A reset command is needed to try to make it operational. */
#define XFS_S_CDR_DS_ON_LINE                      (6180)                      /**< The device is present and operational. */
#define XFS_S_CDR_DIS_OK                          (6181)                      /**< All dispenser logical cash units are ok. */
#define XFS_S_CDR_DIS_CU_STATE                    (6182)                      /**< One of the dispenser logical cash units present is in an abnormal state. */
#define XFS_S_CDR_DIS_CU_STOP                     (6183)                      /**< Due to a cash unit failure dispensing is impossible. */
#define XFS_S_CDR_DIS_CU_UNKNOWN                  (6184)                      /**< Due to a hardware error or other condition, the state of the cash units cannot be determined. */
#define XFS_S_CDR_IS_EMPTY                        (6185)                      /**< The intermediate stacker is empty. */
#define XFS_S_CDR_IS_NOT_EMPTY                    (6186)                      /**< The intermediate stacker is not empty. */
#define XFS_S_CDR_IS_UNKNOWN                      (6187)                      /**< @deprecated No more used. */
#define XFS_S_CDR_CASH_TAKEN                      (6192)                      /**< The cash was taken by the user. */
#define XFS_S_CDR_SD_OPEN                         (6194)                      /**< The cash module lock (safe door) is open. */
#define XFS_S_CDR_SD_LOCKED                       (6196)                      /**< The cash module lock (safe door) is closed and locked. */
#define XFS_S_CDR_SD_UNKNOWN                      (6197)                      /**< @deprecated No more used. */
#define XFS_S_CDR_MAINTENANCE_STATUS_CHANGED      (BNRXFS_STATUS_CDR_OFFSET)  /**< The maintenance status of one or more modules, has changed. Introduced in BNR FW v1.7.0. By default, this event is disabled. @see bnr_EnableStatusEvents(), module_MaintenanceDone() and module_SetMaintenanceInterval(). */

/** @ingroup BNR_SYSTEM_STATUS
 * Xfs Shutter status.
 */
typedef enum XfsCdrShutterStatus {
  XFS_S_CDR_SHT_CLOSED =        6198,  /**< The shutter is closed. */
  XFS_S_CDR_SHT_OPEN =          6199,  /**< The shutter is open. */
  XFS_S_CDR_SHT_NOT_SUPPORTED = 6201,  /**< The shutter status is not supported. */
  XFS_S_CDR_SHT_UNKNOWN =       6202   /**< The shutter state cannot be determined. */
} T_XfsCdrShutterStatus;

#define XFS_S_CDR_TP_OK           (6203)  /**< The transport is in a good state. */
#define XFS_S_CDR_TP_INOP         (6204)  /**< The transport is inoperative due to a hardware failure or media jam. */
#define XFS_S_CDR_TP_UNKNOWN      (6206)  /**< The transport state is unknown. */
#define XFS_S_CDR_CASH_AVAILABLE  (6223)  /**< Cash is available to the user. */

#endif // _XFSCONSTS_H_
