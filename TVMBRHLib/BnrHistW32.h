/******************************************************************************
 *
 *         BNR SDK Project
 *
 ******************************************************************************
 *
 * File :   BnrHistW32.h
 * 
 * Author : MEI Inc.
 *
 * Date :   28.05.08
 *
 * History of modifications
 * Date       Rev.    Reason
 *
 ******************************************************************************
 * FILE DESCRIPTION *//** @file
 * This file contains history constants and types declaration for the BNR API for Win32.
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

#ifndef _BNR_HIST_W32_H_
#define _BNR_HIST_W32_H_

#ifdef WIN32
#include "windows.h"
#else
//typedef unsigned short UINT16;
#endif

#include "BnrPropW32.h"

// Set 2 bytes struct member aligment 
#ifndef WIN32
#pragma pack(push,2)
#else
#include <pshpack2.h>
#endif
   
/** @ingroup MODULE_STATUS
 *   Maximum number of items in T_StoredErrorList
 */
#define MAX_NR_STORED_ERRORS_PER_MODULE (2)

/** @ingroup MODULE_STATUS
 *  T_StoredError elementId value indicating the host has written the last error
 */
#define STORED_ERROR_ELEMENT_ID_ERROR_WRITTEN_BY_HOST (0x970000)

/** @ingroup MODULE_STATUS
 *   Structure containing the module id, last error raised
 *  and cycle count for that module at the time of the error.
 */
typedef struct StoredError {
  T_ElementId          elementId;   /**< ID of the element that raised the last error. STORED_ERROR_ELEMENT_ID_ERROR_WRITTEN_BY_HOST indicates the error was set by the host. */
  UINT32               errorCode;   /**< Last error code ranging from 0-127. */
  T_LongAbsoluteCount  cycleCount;  /**< The number of cycles in the module when the error was written. */
} T_StoredError;

/** @ingroup MODULE_STATUS
 *  Structure containing list of stored errors
 *  @see MAX_NR_STORED_ERRORS_PER_MODULE
 */
typedef struct StoredErrorList {
  UINT32               maxSize;                                 /**< Maximum number of T_StoredError items that can be stored in the array. */
  UINT32               size;                                    /**< Number of items in the array. */
  T_StoredError        items[MAX_NR_STORED_ERRORS_PER_MODULE];  /**< List of T_StoredError for the module. */
} T_StoredErrorList;

/** @ingroup BNR_HISTORY
 * Details about bill insertion rejects.
 */
typedef struct InsertionRejectDetails {
  T_FreeRunningCount   forcedInCount;          /**< The number of times a bill is forced in the BNR before the insertion starts. */
  T_FreeRunningCount   removedCount;           /**< The number of times a bill is pulled back after the insertion started. */
  T_FreeRunningCount   heldBackCount;          /**< The number of times a bill is held at the inlet that does not completely enter the Bill Intake path in less than 1 second. */
  T_FreeRunningCount   tooThickCount;          /**< The number of times an 'object' that is either too thick or too hard object blocks the Bill Intake insertion rollers. */
  T_FreeRunningCount   badRoughShapeCount;     /**< The number of times the initial bill size checkis negative (bill defined as not transportable). */
  T_FreeRunningCount   badShapeCount;          /**< The number of times the main bill size check is negative (bill defined as not transportable). */
  T_FreeRunningCount   stringDetectionCount;   /**< The number of times an object is detected in the Inlet area after the bill has been inserted. */
  T_FreeRunningCount   suspisciousEventCount;  /**< @deprecated No more used, always set to 0 (2011-06-17). */
  T_FreeRunningCount   inletDetectionCount;    /**< Suspiscious detection at inlet */
} T_InsertionRejectDetails;

/** @ingroup BNR_HISTORY
 * Details about transport rejects.
 */
typedef struct TransportRejectDetails {
  T_FreeRunningCount   positioningFailedCount;  /**< The number of times the bill positioning process has failed. */
  T_FreeRunningCount   transportEventCount;     /**< The number of times a bill has been blocked and rejected. */
  T_FreeRunningCount   systemEventCount;        /**< The number of times a failure occurred and the bill has been rejected. */
} T_TransportRejectDetails;

/** @ingroup BNR_HISTORY
 * History of reject reasons (recognition algorithms) of rolled back banknotes.
 */
typedef struct RecognitionRejectDetails {   
  T_LongAbsoluteCount  suspectCount;        /**< The number of times a bill is identified as suspect (see Art.6 definitions). */
  T_LongAbsoluteCount  forgeryCount;        /**< The number of times a bill is identified as a forgery (see Art.6 definitions). */
  T_LongAbsoluteCount  fitnessRejectCount;  /**< The number of times a bill that is badly damaged has been rejected (soiled, holes in security zones, stained ...). */
  T_LongAbsoluteCount  confusionCount;      /**< The number of times a bill has been recognised as more than one downloaded CashType (not the case of several versions of a same denomination). The counters of all the recognised CashType are incremented. */
} T_RecognitionRejectDetails;

/** @ingroup BNR_HISTORY
 * Acceptance history per cashType.
 */
typedef struct CashTypeAcceptanceHistory {
  T_XfsCashType        cashType;
  T_FreeRunningCount   confusionCount;      /**< The number of times a bill has been recognised as more than one downloaded CashType (not the case of several versions of a same denomination). The counters of all the recognised CashType are incremented. */
  T_FreeRunningCount   forgeryCount;        /**< The number of times a bill is identified as a forgery (see Art.6 definitions). */
  T_FreeRunningCount   fitnessRejectCount;  /**< Detailed counter of forgeryCount. The number of times a bill that is badly damaged has been rejected (soiled, holes in security zones, stained ...). */
  T_LongAbsoluteCount  validCount;          /**< The number of times a bill has been accepted, fit or unfit,  by the recognition algorithm. */
  T_LongAbsoluteCount  validUnfitCount;     /**< The number of times a bill that is badly damaged has been rejected (soiled, holes in security zones, stained ...). */
  T_FreeRunningCount   suspectCount;        /**< The number of times a bill is identified as suspect (see Art.6 definitions). */
} T_CashTypeAcceptanceHistory;

#define MAX_NR_OF_CASH_TYPE_HISTORY (MAX_NR_OF_DENOMINATION)

/** @ingroup BNR_HISTORY
 * Acceptance for bills inserted in the inlet.
 *
 * There are 2 types of audit counters : General counters that record events independant of CashType, and specific 
 * counters that record events by CashType.
 */
typedef struct InletAcceptanceHistory {
  T_LongAbsoluteCount          insertionStartCount;       /**< The number of times a bill is presented at the Inlet (detection and Bill Intake rollers start). */
  T_LongAbsoluteCount          insertionRejectCount;      /**< The number of times a bill is rejected due to an insertion problem. Incremented every time one of its detailled counters is incremented. */
  T_InsertionRejectDetails     insertionRejectDetails;    /**< Detailled insertion reject counters. */
  T_LongAbsoluteCount          transportRejectCount;      /**< The number of times a bill is rejected due to a bill transport problem. Incremented every time one of its detailled counters is increment. */
  T_TransportRejectDetails     transportRejectDetails;    /**< Detailed transport reject counters. */
  T_LongAbsoluteCount          cancelRejectCount;         /**< The number of times a bill has been rejected after a bnr_Cancel() command is received by the BNR, or after a loss of communications. */
  T_LongAbsoluteCount          unknownCount;              /**< The number of times a bill failed denomination. */
  T_LongAbsoluteCount          confusionCount;            /**< Detailed counter of unknownCount. The number of times a bill was classified as unknown due to a confusion between two or more bills. */
  T_LongAbsoluteCount          forgeryCount;              /**< The number of times a bill is identified as a forgery (see Art.6 definitions). */
  T_LongAbsoluteCount          fitnessRejectCount;        /**< Detailed counter of forgeryCount. The number of times a bill that was denominated but rejected because it is badly damaged (soiled, holes in security zones, stained ...). */
  T_LongAbsoluteCount          validCount;                /**< The number of times a bill has been accepted, fit or unfit,  by the recognition algorithm. */
  T_LongAbsoluteCount          validUnfitCount;           /**< The number of times a bill that is badly damaged has been rejected (soiled, holes in security zones, stained ...). */
  T_LongAbsoluteCount          suspectCount;              /**< The number of times a bill is identified as suspect (see Art.6 definitions). */
  T_LongAbsoluteCount          stainedCount;              /**< The number of times a vaild bill with stain has been detected. */
  T_LongAbsoluteCount          configurationRejectCount;  /**< The number of times a bill has been rejected by configuration. I.e. Inhibited BillType or Bill routing table set up. */
  T_LongAbsoluteCount          billRolledBackCount;       /**< The number of times a bill has been rejected after a Rollback command was received by the BNR. */
  T_LongAbsoluteCount          cashInTransactionCount;    /**< The number of times a transaction starts with at least one bill. This allows the average number of bills per successful transaction to be calculated. */
  UINT32                       maxSize;                   /**< Maximum number of T_CashTypeAcceptanceHistory items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                       size;                      /**< Number of T_CashTypeAcceptanceHistory items available in the array. */
  T_CashTypeAcceptanceHistory  cashTypeHistoryItems[MAX_NR_OF_CASH_TYPE_HISTORY]; /**< Details per cashType. */
} T_InletAcceptanceHistory;

/** @ingroup BNR_HISTORY
 * Detailed extract error counters
 */
typedef struct ExtractionRejectDetails {
  T_FreeRunningCount  tooLongCount;       /**< The number of times a bill that is too long is detected (long misfeed). */
  T_FreeRunningCount  superimposedCount;  /**< The number of times 2 or more superimposed bills are detected. */
} T_ExtractionRejectDetails;

/** @ingroup BNR_HISTORY
 * History of Loader banknotes encashed by recognition algorithm.
 */
typedef struct LoaderRecognitionRejectDetails {
  T_FreeRunningCount  forgeryCount;  /**< The number of times a bill is identified as a forgery (see Art.6 definitions). */
  T_FreeRunningCount  unknownCount;  /**< The number of times a bill failed denomination. */
} T_LoaderRecognitionRejectDetails;

typedef UINT32 T_SlotNumber;

/** @ingroup BNR_HISTORY
 * Loader acceptance per slot
 */
typedef struct LoaderSlotAcceptanceHistory {
  T_SlotNumber                      slotNr;                    /**< Slot number in the spine starting at 1 for the first slot under the 600 bills cashbox. */
  T_LongAbsoluteCount               billExtractedCount;        /**< The number of times a bill has been extracted from the Loader. */
  T_ExtractionRejectDetails         extractionRejectDetails;   /**< Detailed extract error counters. */
  T_LoaderRecognitionRejectDetails  recognitionRejectDetails;  /**< Detailed recognition reject counters. */
  T_FreeRunningCount                otherDenominationCount;    /**< The number of times a bill of other denomination has been found by the recognition algorithm. */
  T_FreeRunningCount                validUnfitCount;           /**< The number of times a bill that is badly damaged has been rejected (soiled, holes in security zones, stained ...). */
} T_LoaderSlotAcceptanceHistory;

#define MAX_NR_OF_LOADERS (4)

/** @ingroup BNR_HISTORY
 * Acceptance for bills extracted from Loader.
 *
 * For the bills extracted from Loader, there are General audit counters and counters by Loader slot.
 */
typedef struct LoaderAcceptanceHistory {
  T_LongAbsoluteCount            billExtractedCount;        /**< The number of times a bill has been extracted from the Loader. */
  T_LongAbsoluteCount            extractionRejectCount;     /**< The number of times a bill has been stored in the cashbox due to a problem extracting from the Loader. Incremented every time one of the following detailled counters is incremented. */
  T_LongAbsoluteCount            transportRejectCount;      /**< The number of times a bill is rejected due to a bill transport problem. Incremented every time one of the following detailled counters is increment. @see T_TransportRejectDetails. */
  T_TransportRejectDetails       transportRejectDetails;    /**< Detailed counters. */
  T_LongAbsoluteCount            recognitionRejectCount;    /**< The number of times a bill is rejected by the bill recognition algorithm. Incremented every time one of the following detailed counters is incremented. @see T_RecognitionRejectDetails and T_BillAcceptanceHistory::unknownCount. */
  T_LongAbsoluteCount            configurationRejectCount;  /**< @deprecated No more used, always set to 0 (2011-06-17). */
  T_LongAbsoluteCount            badShapeRejectCount;       /**< The number of times a bill is rejected due to its bad size (bill defined as not transportable). */
  UINT32                         maxSize;                   /**< Maximum number of T_LoaderSlotAcceptanceHistory items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                         size;                      /**< Number of T_LoaderSlotAcceptanceHistory items available in the array. */
  T_LoaderSlotAcceptanceHistory  slots[MAX_NR_OF_LOADERS];  /**< Loader acceptance per slot. */
} T_LoaderAcceptanceHistory;

/** @ingroup BNR_HISTORY
 * Recycler acceptance history item indicating how many times a note of a primary cash type has been stacked instead of recycled due
 * to the recycler being full. 
 *
 */
typedef struct CashTypeRecycleHistoryItem {
  T_XfsCashType    cashType;                       /**< The primary cash type for the recycler. */
  T_AbsoluteCount  stackedWhileRecyclerFullCount;  /**< The number of times a note of the specified cash type was stacked instead of recycled due to the recycler being full. */
} T_CashTypeRecycleHistoryItem;

#define MAX_NR_CASH_TYPE_RECYCLE_HISTORY_ITEMS (10)

/** @ingroup BNR_HISTORY
 * The collection of T_CashTypeRecycleHistoryItem for all cash types from the BNR.
 * @see T_CashTypeRecycleHistoryItem
 * @see bnr_GetBillAcceptanceHistory
 */
typedef struct CashTypeRecycleHistoryItems {
  UINT32                        size;                                           /**< The current number of T_CashTypeRecyclerHistoryItem in the items array. */
  UINT32                        maxSize;                                        /**< The maximum number of T_CashTypeRecyclerHistoryItem in the items array. */
  T_CashTypeRecycleHistoryItem  items[MAX_NR_CASH_TYPE_RECYCLE_HISTORY_ITEMS];  /**< The array of T_CashTypeRecycleHistoryItem. */
} T_CashTypeRecycleHistoryItems;

/** @ingroup BNR_HISTORY
 * Performance and statistics of banknotes acceptation.
 *
 * For the banknote acceptation, we count separately banknotes inserted at inlet and those comming from Loader.
 *
 * The data are essentially banknotes input refused couters that allow to measure QP and estimate Acceptance Rate. 
 * 
 * @see bnr_GetBillAcceptanceHistory().
 */
typedef struct BillAcceptanceHistory {
  T_InletAcceptanceHistory       inletAcceptanceHistory;    /**< Acceptance for bills inserted in the inlet. */
  T_LoaderAcceptanceHistory      loaderAcceptanceHistory;   /**< Acceptance for bills extracted from Loader. */
  T_CashTypeRecycleHistoryItems  recycleAcceptanceHistory;  /**< Acceptance for bills configured for recycler but stacked due to recycler being full. */
} T_BillAcceptanceHistory;

/** @ingroup BNR_HISTORY
 * Dispense history per cashType.
 */
typedef struct CashTypeDispenseHistory {
  T_XfsCashType        cashType;
  T_LongAbsoluteCount  billRequestedCount;     /**< The number of bills requested by using the bnr_Denominate() command with amount. */
  T_FreeRunningCount   billNotAvailableCount;  /**< The number of bills requested by using the bnr_Denominate() command with amount, that were not available due to a Recycler or the Loader being empty. Incremented when a denomination is missing using the first choice change algorithm. However this denomination may have been replaced by another combination of denominations. */
} T_CashTypeDispenseHistory;

/** @ingroup BNR_HISTORY
 * Bill dispense history.
 *
 * The data essentially consists of counters measuring availability rate of bills for change giving.
 *
 * This information is relevant mainly when the host systematically uses the command bnr_Dispense() with amount, 
 * i.e. when the BNR manages the Loader.
 *
 * Counters by ?Dispensable ?denomination are used, from Recyclers or from Loaders.  
 *
 * @see bnr_GetBillDispenseHistory().
 */
typedef struct BillDispenseHistory {
  T_LongAbsoluteCount        denominateAmountCount;    /**< The number of times the bnr_Denominate() command with amount has been used. */
  T_LongAbsoluteCount        amountNotAvailableCount;  /**< The number of times the bnr_Denominate() command with amount returned unsuccessfully. */
  T_LongAbsoluteCount        billRequestedCount;       /**< The number of bills requested by using the bnr_Denominate() command with amount. */
  T_LongAbsoluteCount        billNotAvailableCount;    /**< The number of bills requested by using the bnr_Denominate() command with amount, that were not available due to a Recycler or the Loader being empty. Incremented when a denomination is missing using the first choice change algorithm. However this denomination may have been replaced by another combination of denominations. */
  T_AbsoluteCount            tooManyBillsCount;        /**< The number of times the bnr_Denominate() command with amount returned unsuccessfully because the Bundler capacity was exceeded. */
  T_AbsoluteCount            directFromLoaderCount;    /**< The number of bills taken directly from the Loader while dispensing due to insufficient Refloat in the recyclers. */
  UINT32                     maxSize;                  /**< Maximum number of T_CashTypeDispenseHistory items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32                     size;                     /**< Number of T_CashTypeDispenseHistory items available in the array. */
  T_CashTypeDispenseHistory  cashTypeDispenseHistoryItems[MAX_NR_OF_CASH_TYPE_HISTORY]; /**< Details per cashType. */
  T_AbsoluteCount            dispenseAmountCount;      /**< Number of times dispense by amount has been called */
} T_BillDispenseHistory;

/** @ingroup BNR_HISTORY
 * Detailed opening history.
 */
typedef struct SystemOpeningDetails {
  T_FreeRunningCount  withBillStoppedCount;         /**< The number of times that the BNR has been opened when a bill is being transported in the bill path. An emptying cycle will occur after the Reset command is sent. */
  T_AbsoluteCount     cashModulesLockCount;         /**< The number of times the CashModulesLock has been opened with the BNR powered up. */
  T_AbsoluteCount     billIntakeCoverCount;         /**< The number of times the Bill Intake (upper) has been opened with the BNR powered up. */
  T_AbsoluteCount     recognitionSensorCoverCount;  /**< The number of times the recognition sensor sub module has been opened with the BNR powered up. */
  T_AbsoluteCount     spineCoverCount;              /**< The number of times the Spine module has been opened with the BNR powered up. */
} T_SystemOpeningDetails;

/** @ingroup BNR_HISTORY
 * Hardware reset and system opening history.
 *
 * These counters relate to Power-fails, Internal Resets and opening of Lock/Covers.
 * All these counters are reset during the first power-up only.
 *
 * @see bnr_GetRestartHistory().
 */
typedef struct SystemRestartHistory {
  T_LongAbsoluteCount     powerUpCount;                       /**< The number of times the system has been powered up. */
  T_FreeRunningCount      powerDownWithBillStoppedCount;      /**< The number of times that the BNR has powered down when a bill is being transported in the bill path. An emptying cycle will occur after the Reset command is sent. */
  T_LongAbsoluteCount     internalResetCount;                 /**< The number of internal internal resets (software errors, watchdog...). */
  T_FreeRunningCount      internalResetWithBillStoppedCount;  /**< The number of times an internal reset occured when a bill was being transported in the bill path. An emptying cycle will occur after the Reset command is sent. */
  T_LongAbsoluteCount     systemOpeningCount;                 /**< The number of times the system has been opened while powered up. */
  T_SystemOpeningDetails  systemOpeningDetails;               /**< Detailled system opening. */
} T_SystemRestartHistory;

/** @ingroup BNR_HISTORY
 * Incident Start Section History. 
 * Indicates where a jam or fault has started.
 *
 * @see T_PerSectionFailureHistory, T_SystemFailureHistory
 */
typedef struct IncidentStartSectionCounters {
  T_AbsoluteCount  positionerCount;         /**< Number of times incident started in the Main Module positioner section */
  T_AbsoluteCount  recognitionSystemCount;  /**< Number of times incident started in the Main Module recognition system section */
  T_AbsoluteCount  bottomTransportCount;    /**< Number of times incident started in the Main Module bottom transport section */
  T_AbsoluteCount  bundlerCount;            /**< Number of times incident started  in the Main Module bundler section */
  T_AbsoluteCount  spineCount;              /**< Number of times incident started in the spine */
  T_AbsoluteCount  module0Count;            /**< Number of times incident started in module 0 */
  T_AbsoluteCount  module1Count;            /**< Number of times incident started in module 1 */
  T_AbsoluteCount  module2Count;            /**< Number of times incident started in module 2 */
  T_AbsoluteCount  module3Count;            /**< Number of times incident started in module 3 */
  T_AbsoluteCount  module4Count;            /**< Number of times incident started in module 4 */
  T_AbsoluteCount  module5Count;            /**< Number of times incident started in module 5 */
  T_AbsoluteCount  module6Count;            /**< Number of times incident started in module 6 */
  T_AbsoluteCount  module7Count;            /**< Number of times incident started in module 7 */
  T_AbsoluteCount  module8Count;            /**< Number of times incident started in module 8 */
  T_AbsoluteCount  module9Count;            /**< Number of times incident started in module 9 */
  T_AbsoluteCount  interface0Count;         /**< Number of times incident started in interface 0 */
  T_AbsoluteCount  interface1Count;         /**< Number of times incident started in interface 1 */
  T_AbsoluteCount  interface2Count;         /**< Number of times incident started in interface 2 */
  T_AbsoluteCount  interface3Count;         /**< Number of times incident started in interface 3 */
  T_AbsoluteCount  interface4Count;         /**< Number of times incident started in interface 4 */
  T_AbsoluteCount  interface5Count;         /**< Number of times incident started in interface 5 */
  T_AbsoluteCount  interface6Count;         /**< Number of times incident started in interface 6 */
  T_AbsoluteCount  interface7Count;         /**< Number of times incident started in interface 7 */
  T_AbsoluteCount  interface8Count;         /**< Number of times incident started in interface 8 */
  T_AbsoluteCount  interface9Count;         /**< Number of times incident started in interface 9 */
} T_IncidentStartSectionCounters;

/** @ingroup BNR_HISTORY
 * Main module section history. 
 *
 * @see T_PerSectionFailureHistory, T_SystemFailureHistory
 */
typedef struct MainModuleSectionCounters {
  T_AbsoluteCount  positionerCount;          /**< Number of times bill has stopped in Main Module positioner. */
  T_AbsoluteCount  recognitionSystemCount;   /**< Number of times bill has stopped in Main Module recognition system. */
  T_AbsoluteCount  bottomTransportCount;     /**< Number of times bill has stopped in Main Module bottom transport. */
  T_AbsoluteCount  bundlerCount;             /**< Number of times bill has stopped in Main Module bundler. */
} T_MainModuleSectionCounters;

/** @ingroup BNR_HISTORY
 * Per-section failure history. A member of T_SystemFailureHistory.
 *
 * @see T_SystemFailureHistory
 */
typedef struct PerSectionFailureHistory {
  T_MainModuleSectionCounters     billsEndingInMMSection;
  T_IncidentStartSectionCounters  incidentStartSection;
} T_PerSectionFailureHistory;

/** @ingroup BNR_HISTORY
 * BNR failure history.
 *
 * @see bnr_GetFailureHistory().
 */
typedef struct SystemFailureHistory {
  T_AbsoluteCount             hardwareFailureCount;                  /**< The number of times the system has reported a hardware failure. Incremented when the failure occurs. The value will not change again until the system is back to Operational. */
  T_AbsoluteCount             hardwareFailureWithBillStoppedCount;   /**< The number of times the system has reported a hardware failure when a bill is being transported in the bill path. An emptying cycle will occur after the bnr_Reset() command is sent. */
  T_AbsoluteCount             operationalDegradedCount;              /**< Reserved for future use. */
  T_AbsoluteCount             billJamCount;                          /**< The number of times the system has reported a bill jam. @see #BTS_BILL_JAM. The value will not change again until the system is back to Operational. This counter can not be used directly to calculate the BNR's MCBJ as bill jams must be confirmed by human analysis. */
  T_AbsoluteCount             environmentErrorCount;                 /**< Reserved for future use. */
  T_AbsoluteCount             billErrorCount;                        /**< The number of times the system has detected a document that cannot be transported in the system. @see #BTS_BILL_ERROR.*/
  T_AbsoluteCount             transportErrorCount;                   /**< The number of times the system has reported a transportation error it failed to recover from after multiple attempts. @see #BTS_TRANSPORT_ERROR. The value will not change again until the system is back to Operational. This counter can not be used directly to calculate the BNR's MCBJ as bill jams must be confirmed by human analysis. */
  T_AbsoluteCount             billTooShortInBottomTransportBWCount;  /**< The number of times the system has reported a bill too short coming from the spine to the bottom transport.*/
  T_AbsoluteCount             billTooLongInBottomTransportBWCount;   /**< The number of times the system has reported a bill too long coming from the spine to the bottom transport.*/
  T_AbsoluteCount             billTooShortInSpineFWCount;            /**< The number of times the system has reported a bill too short coming from the Recognition Sensor to the Spine.*/
  T_AbsoluteCount             billTooLongInSpineFWCount;             /**< The number of times the system has reported a bill too long coming from the Recognition Sensor to the Spine.*/
  T_AbsoluteCount             missingModuleCount;                    /**< The number of times the system has been resetted with a configured module that was missing.*/
  T_AbsoluteCount             configurationErrorCount;               /**< The number of times the system has been resetted with an incompatible module.*/
  T_AbsoluteCount             incompatibleSoftwareCount;             /**< The number of times the system has been resetted with a module having an incompatible software or a boot software.*/
  T_AbsoluteCount             resetWithCoverOpenCount;               /**< The number of times the system has issued a Reset command while one of the BNR's covers were open. */
  T_AbsoluteCount             resetWithInterlockOpenCount;           /**< The number of times the system has issued a Reset command while one of the BNR's interlock was open. */
  T_PerSectionFailureHistory  perSectionHistory;                     /**< For each module, module section and module interface, the count of times where a jam started and/or ended.*/
} T_SystemFailureHistory;

#define NR_OF_HIBOU_MODULES (4)

/** @ingroup BNR_HISTORY
 * BNR use history.
 *
 * These data indicate system up time and number of cycles.
 */
typedef struct SystemUseHistory {
  T_Time64  currentDateTime;       /**< Current date and time in the BNR. Start to January 1st, 2001 at Power Up. @see bnr_SetDateTime(), bnr_GetDateTime(). */
  T_Time64  upTime;                /**< Elapsed time, in second, since last Power Up. */
  T_Time64  totalUpTime;           /**< Total elapsed time, in second, with the system powered up. The value is reset during the first power-up only. */
  T_Time64  timeSinceOperational;  /**< Elapsed time, in second, that the system has been operational. Used to measure elapsed time between failures. */

  /**
   * The number of system cycles. Used to evaluate MCBF, MCBJ.
   *
   * This counter increments (+1) every time :
   *  - Insertion cycle starts, Bill Intake motors start (Bill In).
   *  - A bill is extracted out of the system for a dispense (Bill Out).
   *  - A bill is transferred from the Loader to another module.
   *  - A bill is transferred from the Recycler to the Cashbox (Empty).
   *  - A reset command is executed.
   *
   * @see bnr_GetUseHistory().
   */
  T_LongAbsoluteCount  systemCycleCount;
  UINT8                systemTemperature;                    /**< System temperature in Celsius.*/
  UINT8                rsTemperatures[NR_OF_HIBOU_MODULES];  /**< Temperature of each Recognition Sensor's module in Celsius.*/
  UINT16               powerSupplyVoltage;                   /**< Power supply voltage.*/
} T_SystemUseHistory;

/** @ingroup MODULE_HISTORY
 * Details about Loader transport error.
 * Each time T_DefaultModuleFailureHistory.transportErrorCount is incremented, one of the following counters is also incremented.
 */
typedef struct LoaderTransportErrorDetails {
  T_AbsoluteCount  noFeedCount;         /**< The number of time a noFeed caused a transportation error. Incremented when the Loader failed to find bill for extraction.*/
  T_AbsoluteCount  badFeedCount;        /**< The number of time a badFeed caused a transportation error. Incremented when the Loader failed to prepare bill for extraction.*/
  T_AbsoluteCount  badExtractionCount;  /**< The number of time a badExtraction causes a transportation error. Incremented when the Loader failed to extract bill. */
} T_LoaderTransportErrorDetails;

/** @ingroup MODULE_HISTORY
 * Module failure history.
 *
 * Error counters specific to Spine and Recycler module.
 */
typedef struct DefaultModuleFailureHistory {
  T_AbsoluteCount  hardwareFailureCount;  /**< The number of hardware failures of the module. Incremented at the same time as the system counter. */
  T_AbsoluteCount  billJamCount;          /**< The number of times the module has reported a bill jam. @see T_BillTransportStatus::BTS_BILL_JAM. @see T_SystemFailureHistory::billJamCount. The value will not change again until the system is back to Operational. This counter can not be used directly to calculate the module's MCBJ as bill jams must be confirmed by human analysis. */
  T_AbsoluteCount  billErrorCount;        /**< The number of times the module has detected a document that cannot be transported in the system. @see T_BillTransportStatus::BTS_BILL_ERROR.*/
  T_AbsoluteCount  transportErrorCount;   /**< The number of times the module has reported a transportation error it failed to recover from after multiple attemts. @see T_BillTransportStatus::BTS_TRANSPORT_ERROR. The value will not change again until the system is back to Operational. This counter can not be used directly to calculate the BNR's MCBJ as bill jams must be confirmed by human analysis. */
} T_DefaultModuleFailureHistory;

/** @ingroup MODULE_HISTORY
 * Cashbox failure history.
 *
 * Error counters specific to Cashbox.
 */
typedef struct CashboxFailureHistory {
  T_DefaultModuleFailureHistory  moduleFailureHistory;  /**< Counters common with other modules. */
  T_AbsoluteCount                notArmedCount;         /**< The number of times the Cashbox was inserted without being armed. */
} T_CashboxFailureHistory;

/** @ingroup MODULE_HISTORY
 * Loader failure history.
 *
 * Error counters specific to Loader.
 */
typedef struct LoaderFailureHistory {
  T_DefaultModuleFailureHistory  moduleFailureHistory;   /**< Counters common with other modules. */
  T_AbsoluteCount                extractionEventCount;   /**< The number of times a bill extracted from the Loader has generated a "misfeed error" or a TransportReject Error. Incremented every time one of the following counters increments : ExtractionRejectCnt ?TransportRejectCnt. */
  T_LoaderTransportErrorDetails  transportErrorDetails;  /**< Details counter of  transportErrorCount.*/
} T_LoaderFailureHistory;


/** @ingroup MODULE_HISTORY
 * Module failure history.
 *
 * @see module_GetFailureHistory().
 */
typedef union ModuleFailureHistory {
  T_DefaultModuleFailureHistory  module;         /**< Any module history with common data. */
  T_LoaderFailureHistory         loaderModule;   /**< Loader specific history. */
  T_CashboxFailureHistory        cashboxModule;  /**< Cashbox specific history. */
} T_ModuleFailureHistory;

/** @ingroup MODULE_HISTORY
 * Module resets history.
 *
 * @see module_GetRestartHistory().
 */
typedef struct ModuleRestartHistory {
  T_AbsoluteCount  powerUpCount;        /**< The number of times the module has been powered up. */
  T_AbsoluteCount  internalResetCount;  /**< The number of internal resets (software errors, watchdog...). */
} T_ModuleRestartHistory;

/** @ingroup MODULE_HISTORY
 * Loader usage history.
 */
typedef struct LoaderUseHistory {
  T_AbsoluteCount      repairCount;           /**< The number of times this module has been repaired. */
  T_LongAbsoluteCount  cycleCount;            /**< The number of times a bill was pre-fed, ready for an extraction. */
  T_XfsUnitId          lastMainModuleId;      /**< Identitification code of  the last connected Main Module. */
  T_AbsoluteCount      moduleInsertionCount;  /**< The number of times the Loader has been inserted in a BNR. */
} T_LoaderUseHistory;

/** @ingroup MODULE_HISTORY
 * Recycler usage history.
 */
typedef struct RecyclerUseHistory {
  T_AbsoluteCount      repairCount;       /**< The number of times this module has been repaired. */
  T_LongAbsoluteCount  cycleCount;        /**< The number of times the recycler motor (RE.M1) has started. Initialisation (+3). */
  T_XfsUnitId          lastMainModuleId;  /**< Identitification code of  the last connected Main Module. */
} T_RecyclerUseHistory;

#define MAX_NR_OF_SPINE_SLOTS (8)

/** @ingroup MODULE_HISTORY
 * Spine diverter usage history.
 */
typedef struct DiverterUseHistory {
  T_ElementId          id;          /**< Id of the diverter. */
  T_LongAbsoluteCount  cycleCount;  /**< The number of times a diverter takes the ?to module ?position. */
} T_DiverterUseHistory;

/** @ingroup MODULE_HISTORY
 * Spine usage history.
 */
typedef struct SpineUseHistory {
  T_AbsoluteCount       repairCount;                                  /**< The number of times this module has been repaired. */
  T_LongAbsoluteCount   cycleCount;                                   /**< The number of times the Transport System starts (belts wearing control). */
  T_XfsUnitId           lastMainModuleId;                             /**< Identitification code of  the last connected Main Module. */
  UINT16                maxSize;                                      /**< Maximum number of diverter count that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT16                size;
  T_DiverterUseHistory  diverterHistoryItems[MAX_NR_OF_SPINE_SLOTS];  /**< History of the diverters. */
} T_SpineUseHistory;

/** @ingroup MODULE_HISTORY
 * Bundler usage history.
 */
typedef struct BundlerUseHistory {
  T_LongAbsoluteCount  cycleCount;  /**< The number of times BU.M1 (Bundler Motor) starts. */
} T_BundlerUseHistory;

/** @ingroup MODULE_HISTORY
 * MainModule usage history.
 */
typedef struct MainModuleUseHistory {
  T_AbsoluteCount      repairCount;                 /**< The number of times this module has been repaired. */
  T_LongAbsoluteCount  mainTransportCycleCount;     /**< The number of times MM.M1 (Main Motor) starts. Same count as BT.M1. */
  T_LongAbsoluteCount  stackerCycleCount;           /**< The number of cycles of the stacker. Every encashment (+1), Initialisation (+2). */
  T_LongAbsoluteCount  bundlerDivSystemCycleCount;  /**< The number of cycles of the Bundler Diverter System. Every position change (+1), Initialisation (+4). */
  T_LongAbsoluteCount  billIntakeCycleCount;        /**< The number of times BI.M1 (Bill Intake motor) starts. */
  T_LongAbsoluteCount  positionerCycleCount;        /**< The number of cycles of the Positioning System. */
} T_MainModuleUseHistory;

/** @ingroup MODULE_HISTORY
 * BarcodeReader usage history.
 */
typedef struct BarcodeReaderUseHistory {
  T_AbsoluteCount      repairCount;       /**< The number of times this module has been repaired. */
  T_XfsUnitId          lastMainModuleId;  /**< The last main module Id*/
  T_LongAbsoluteCount  cycleCount;        /**< The Cycle Count*/
} T_BarcodeReaderUseHistory;

/** @ingroup MODULE_HISTORY
 * Cashbox usage history.
 */
typedef struct CashBoxUseHistory {
  T_AbsoluteCount      repairCount;       /**< The number of times this module has been repaired. */
  T_XfsUnitId          lastMainModuleId;  /**< The last main module Id*/
  T_LongAbsoluteCount  cycleCount;        /**< The Cycle Count*/
} T_CashBoxUseHistory;


/** @ingroup MODULE_HISTORY
 * Use history data for module.
 *
 * @see module_GetUseHistory().
 */
typedef union ModuleUseHistory {
  T_MainModuleUseHistory     mainModuleUseHistory;
  T_SpineUseHistory          spineUseHistory;
  T_RecyclerUseHistory       recyclerUseHistory;
  T_LoaderUseHistory         loaderUseHistory;
  T_BundlerUseHistory        bundlerUseHistory;
  T_CashBoxUseHistory        cashBoxUseHistory;
  T_BarcodeReaderUseHistory  barcodeReaderUseHistory;
} T_ModuleUseHistory;

/** @ingroup MODULE_HISTORY
 * Bill cassette handling history for Loader.
 */
typedef struct LoaderExchangeHistory {
  T_FreeRunningCount  moduleRemovalCount;  /**< The number of times a Loader has been removed from the slot, with the BNR powered up. If the Loader is removed several times without a reset command sent to the BNR, this counter is incremented only once. */
  T_FreeRunningCount  moduleChangedCount;  /**< The number of times the BNR detects that the Loader Identification number has changed. Tested and incremented during the reset cycle. */
  T_FreeRunningCount  levelChangeCount;    /**< The number of times the BNR detects a bill level change in the Recycler. Tested and incremented during the reset cycle. */
  T_FreeRunningCount  moduleEmptyCount;    /**< The number of times the Loader has been detected empty. Once incremented, the counter won't change until a non empty Loader is inserted. */
  T_FreeRunningCount  wrongBillsCount;     /**< The number of times the Loader has been set in error wrongDenimination when testing content. */
  T_FreeRunningCount  billNotValidCount;
} T_LoaderExchangeHistory;

/** @ingroup MODULE_HISTORY
 * Bill cassette handling history for Recycler.
 */
typedef struct RecyclerExchangeHistory {
  T_FreeRunningCount  moduleRemovalCount;  /**< The number of times a Recycler has been removed from the slot, with the BNR powered up. If the Recycler is removed several times without a reset command sent to the BNR, this counter is incremented only once. */
  T_FreeRunningCount  moduleChangedCount;  /**< The number of times the BNR detects that the Recycler Identification number has changed. Tested and incremented during the reset cycle. */
  T_FreeRunningCount  levelChangeCount;    /**< The number of times the BNR detects a bill level change in the Recycler. Tested and incremented during the reset cycle. */
} T_RecyclerExchangeHistory;

/** @ingroup MODULE_HISTORY
 * Bill cassette handling history for Cashbox.
 *
 * These counters relate to the modules that contain bills.
 * Every counter relates to a Spine position and is reset during the first power-up only
 */
typedef struct CashboxExchangeHistory {
  T_FreeRunningCount  moduleRemovalCount;  /**< The number of times a Cashbox has been removed from the slot, with the BNR powered up. If the cashbox is removed several times without a reset command sent to the BNR, this counter is incremented only once. */
  T_FreeRunningCount  moduleChangedCount;  /**< The number of times the BNR detects that the Cashbox Identification number has changed. Tested and incremented during the reset cycle. */
  T_FreeRunningCount  levelChangeCount;    /**< The number of times the BNR detects a bill level change in the Cashbox. Tested and incremented during the reset cycle. */
  T_FreeRunningCount  moduleFullCount;     /**< The number of times the BNR detects that the Cashbox is full. After being incremented, the value cannot change until a non-full Cashbox is detected. */
} T_CashboxExchangeHistory;

/** @ingroup MODULE_HISTORY
 * Bill cassette (including Recycler) handling history.
 * 
 * These counters are used to monitor the manipulation of modules containing banknotes in a BNR.
 *
 * Every counter is linked to a bay in the Spine and is setted to zero only on first power on.
 *
 * @see module_GetBillCassetteExchangeHistory().
 */
typedef union BillCassetteExchangeHistory {
  T_CashboxExchangeHistory  cashboxExchangeHistory;   /**< Cashbox exchange history */
  T_LoaderExchangeHistory   loaderExchangeHistory;    /**< Loader exchange history */
  T_RecyclerExchangeHistory recyclerExchangeHistory;  /**< Recycler exchange history */
} T_BillCassetteExchangeHistory;

/** @ingroup BNR_REPORTS
 * Contains the full status of the BNR system, including its modules.
 */
typedef struct FullSystemStatus {
  T_SystemStatus  systemStatus;
  UINT32          maxSize;                            /**< Maximum number of T_ModuleStatus items that can be stored in the array. This value must be initialized by the caller and may change with the API version. */
  UINT32          size;                               /**< Number of T_ModuleStatus items available in the array. */
  T_ModuleStatus  moduleStatuses[MAX_NR_OF_MODULES];  /**< Module statuses. */
} T_FullSystemStatus;

/** @ingroup BNR_REPORTS
 * Structure for ModuleIdentifications for Reports
 */
typedef struct ModuleIdentificationReport {
  T_ModuleId              id;
  T_ModuleIdentification  moduleIdentification;
} T_ModuleIdentificationReport;


/** @ingroup BNR_REPORTS
 * Structure for ModuleIdentifications for Reports
 */
typedef struct ModuleIdentificationsReportList {
  UINT32                        size;
  T_ModuleIdentificationReport  ids[NBMAXMODULES];
} T_ModuleIdentificationsReportList;


/** @ingroup BNR_REPORTS
 * Summarized Physical Cash Unit data with only name and bill count, saved in BNR error reports.
 */
typedef struct SimplifiedPcu {
  T_XfsPcuName     name;   /**< Name of the Physical Cash Unit.*/
  T_AbsoluteCount  count;  /**< Bill count in the Physical Cash Unit.*/
} T_SimplifiedPcu;

/** @ingroup BNR_REPORTS
 * List of summarized Physical Cash Unit data, saved in BNR error reports.
 */
typedef struct SimplifiedPcuList {
  UINT8                 size;             /**< Number of items in the list.*/
  T_SimplifiedPcu       items[NBMAXPCU];  /**< List of summarized Physical Cash Unit data.*/
} T_SimplifiedPcuList;

/** @ingroup BNR_REPORTS
 * Simple failure report.
 *
 * This report relates to no bills being processed in the transport bill path.
 */
typedef struct SimpleFailureReport {
  T_FreeRunningCount                 reportNr;          /**< The report number. */
  T_SystemUseHistory                 systemUseHistory;  /**< System time and usage when the failure occurred. */
  T_BillCount                        escrowContent;     /**< The number of bills in the escrow position when the failure occurred. */
  T_FullSystemStatus                 fullSystemStatus;  /**< Detailed status of the system and modules when the failure occurred. */
  T_ModuleIdentificationsReportList  moduleIds;         /**< Module Identifications for report.*/
  T_SimplifiedPcuList                pcuList;           /**< List of summarized Physical Cash Unit data.*/
} T_SimpleFailureReport;

#define MAX_NR_OF_FAILURE_REPORTS (100)

/** @ingroup BNR_REPORTS
 * Simple failure report list.
 *
 * List of the last reports in a FIFO buffer. Latest report is at the first position in the list.
 *
 * @see bnr_GetSimpleFailureReports().
 */
typedef struct SimpleFailureReportList {
  UINT32                 maxSize;                             /**< Maximum number of T_SimpleFailureReport items that can be stored in the array. */
  UINT32                 size;                                /**< Number of T_SimpleFailureReport items available in the array. */
  T_SimpleFailureReport  reports[MAX_NR_OF_FAILURE_REPORTS];  /**< Failure reports. */
} T_SimpleFailureReportList;

/** @ingroup BNR_REPORTS
 * History data of a module, saved in HistoryReports.
 */
typedef struct ModuleHistory {
  T_ModuleId                     id;                           /**< Identifier of the module.*/
  T_ModuleIdentification         identification;               /**< Versions of software and hardware components of the module.*/
  T_ModuleFailureHistory         failureHistory;               /**< Failure history of the module.*/
  T_ModuleRestartHistory         restartHistory;               /**< Restart history of the module.*/
  T_ModuleUseHistory             useHistory;                   /**< Use history of the module.*/
  T_BillCassetteExchangeHistory  billCassetteExchangeHistory;  /**< Bill cassette exchange of the module.*/
  T_MaintenanceInfo              maintenanceInfo;              /**< Maintenance information of the module.*/
  T_StoredErrorList              storedErrors;                 /**< Non-persistent errors of the module. */
} T_ModuleHistory;

/** @ingroup BNR_REPORTS
 * List of modules history data, saved in an HistoryReport.
 * 
 * @see T_HistoryReport.
 */
typedef struct ModuleHistoryList {
  UINT32           size;                 /**< Size of the list.*/
  T_ModuleHistory  items[NBMAXMODULES];  /**< Elements of the list.*/
} T_ModuleHistoryList;

/**
 * Maximum length for field changeInfo in an HistoryReport (20 chars + 0 terminal).
 * 
 * @see T_ChangeInfo.
 */
#define SIZE_OF_CHANGE_INFO (21)

/**
 * String for the description of a HistoryReport.
 * 
 * @see T_HistoryReport.
 */
typedef char T_ChangeInfo[SIZE_OF_CHANGE_INFO];

/** @ingroup BNR_REPORTS
 * Reason for creation of a HistoryReport.
 * 
 * @see T_HistoryReport.
 */
typedef enum HistoryReportReason {
  HRR_NR_OF_CYCLES_REACHED,  /**< The system has done 10'000 cycles since the last HistoryReport.*/
  HRR_MODULE_CHANGE,         /**< A Spine or Recycler module has been replaced.*/
  HRR_FIRMWARE_CHANGE,       /**< A new application firmware has been downloaded (in Main Module or other module).*/
  HRR_BILLSET_CHANGE,        /**< Change or update in the downloaded denominations has been detected.*/
  HRR_MANUAL,                /**< A command bnr_SaveHistoryReport() was sent to the BNR.*/
  HRR_MAINTENANCE_DONE       /**< A command module_MaintenanceDone() was sent to the BNR.*/
} T_HistoryReportReason;

/** @ingroup BNR_REPORTS
 * History report.
 *
 * This report is a snapshot of history counters at a particular time, the field reason indicates 
 * why the HistoryReport was created.
 * On creation of an HistoryReport, if the current system cycles count is equal to the system cycles count
 * saved in the last HistoryReport, then the new HistoryReport replaces the previous one.
 *
 * @see T_HistoryReportReason, bnr_SaveHistoryReport(), bnr_GetHistoryReports().
 */
typedef struct HistoryReport {
  T_FreeRunningCount       reportNr;               /**< The report number. */
  T_HistoryReportReason    reason;                 /**< The reason why the report was created.*/
  T_ChangeInfo             changeInfo;             /**< Short description associated with the report.*/
  T_BillAcceptanceHistory  billAcceptanceHistory;  /**< Bill acceptance statistics.*/
  T_BillDispenseHistory    billDispenseHistory;    /**< Bill dispense statistics.*/
  T_SystemRestartHistory   systemRestartHistory;   /**< System restart and opening statistics.*/
  T_SystemFailureHistory   systemFailureHistory;   /**< System failure statistics.*/
  T_SystemUseHistory       systemUseHistory;       /**< System time and usage when the report was created. */
  T_ModuleHistoryList      moduleList;             /**< Modules specific statistics.*/
} T_HistoryReport;

#define MAX_NR_OF_HISTORY_REPORTS (50)

/** @ingroup BNR_REPORTS
 * History report list.
 *
 * List of the last history reports in a FIFO buffer. Latest report is at the first position in the list.
 *
 * @see bnr_GetHistoryReports().
 */
typedef struct HistoryReportList {
  UINT32           maxSize;                             /**< Maximum number of T_HistoryReport items that can be stored in the array. */
  UINT32           size;                                /**< Number of T_HistoryReport items available in the array. */
  T_HistoryReport  reports[MAX_NR_OF_HISTORY_REPORTS];  /**< History reports. */
} T_HistoryReportList;

/**
 * Internal BNR object.
 */
typedef struct BnrInternalObject {
  T_ClassType  superType;
  UINT32       subType;
  UINT32       instanceId;
} T_BnrInternalObject;

typedef UINT16 T_StepCount;     /**< Coding wheel step count. */
typedef UINT16 T_LogEntryType;  /**< Internal log entry. */

/**
 * Describes CPU reset reason.
 */
typedef enum ResetReasonType {
  RRT_NO_REASON,
  RRT_POWER_FAIL,
  RRT_WATCHDOG,
  RRT_REBOOT
} T_ResetReasonType;

/**
 * Base class of all Bill Transport Log Entry.
 */
typedef struct BillTransportLogEntryBaseClass {
  T_LogEntryType      logEntryType;  /**< Base class of all Bill Transport Log Entry. */
  T_BnrInternalObject object;        /**< Object that create this entry. */
  UINT32              time;          /**< Relative time to the begining of the sequence. */
  T_StepCount         stepCount;     /**< Step counter value. */
} T_BillTransportLogEntryBaseClass;

/**
 * Base class of all Step Log Entries.
 *
 * This class inherits from T_BillTransportLogEntryBaseClass.
 */
typedef struct StepLogEntryBaseClass {
  T_LogEntryType       logEntryType;  /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_BnrInternalObject  object;        /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  UINT32               time;          /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_StepCount          stepCount;     /**< Step counter value. */
} T_StepLogEntryBaseClass;  

/**
 * Base class of all Step Log Entries.
 *
 * This class inherits from T_BillTransportLogEntryBaseClass.
 */
typedef struct DestLogEntryBaseClass {
  T_LogEntryType      logEntryType;  /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_BnrInternalObject object;        /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  UINT32              time;          /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_StepCount         stepCount;     /**< Step counter value. */
} T_DestLogEntryBaseClass;

/**
 * Base class of all Step Log Entries.
 *
 * This class inherits from T_BillTransportLogEntryBaseClass.
 */
typedef struct ApplicationLogEntryBaseClass {
  T_LogEntryType      logEntryType;  /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_BnrInternalObject object;        /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  UINT32              time;          /**< Inherited field from T_BillTransportLogEntryBaseClass. */
  T_StepCount         stepCount;     /**< Step counter value. */
} T_ApplicationLogEntryBaseClass;

/**
 * Log Entry when a sequence starts.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct StartSequenceLogEntry {
  T_Time64  currentDateTime;  /**< time since Up Time */
  UINT16    command;          /**< BNR internal command. */
} T_StartSequenceLogEntry; 

/**
 * Segment exception log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct SegmentExceptionLogEntry {
  UINT16  exception;  /**< Segment exception code. */
} T_SegmentExceptionLogEntry;

/**
 * Cash unit position in the Spine module.
 *
 * The first unit starts at position 1.
 */
typedef UINT8 T_CashUnitSpinePosition;

/**
 * Section log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct SectionLogEntry {
  UINT16                   section;
  T_CashUnitSpinePosition  position;
} T_SectionLogEntry;

/**
 * Positioner status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct PositionerLogEntry {
  T_PositionerFunctionalStatus  functionalStatus;  /**< Positioner functional status. */
  T_PositionerErrorCode         errorCode;         /**< Positioner error code. */
} T_PositionerLogEntry;

/**
 * Detector status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct DetectorLogEntry {
  T_SensorFunctionalStatus  functionalStatus;  /**< Detector functional status. */
  T_SensorErrorCode         errorCode;         /**< Detector error code. */
} T_DetectorLogEntry;

/**
 * Motor status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct MotorLogEntry {
  T_MotorFunctionalStatus  functionalStatus;  /**< Motor functional status. */
  T_MotorErrorCode         errorCode;         /**< Motor error code. */
} T_MotorLogEntry;

/**
 * BundlerDivSystem status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct BundlerDivSystemLogEntry {
  T_BundlerDivSystemFunctionalStatus  functionalStatus;
  T_BundlerDivSystemErrorCode         errorCode;
  UINT8                               bundlerNPos;
} T_BundlerDivSystemLogEntry;

/**
 * Stacker status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct StackerLogEntry {
  T_StackerFunctionalStatus  functionalStatus;  /**< Stacker functional status. */
  T_StackerErrorCode         errorCode;         /**< Stacker error code. */
} T_StackerLogEntry;

/**
 * Recycler Tape Position status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct TapePositionLogEntry {
  T_TapePositionFunctionalStatus  functionalStatus;  /**< Recycler Tape Position functional status. */
  T_TapePositionErrorCode         errorCode;         /**< Recycler Tape Position error code. */
} T_TapePositionLogEntry;

/**
 * Step Trigger event log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct StepTriggerLogEntry {
  UINT8  id;  /**< Step Trigger Identification related to the current segment. */
} T_StepTriggerLogEntry;

/**
 * RecognitionSensor status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct HibouLogEntry {
  T_RecognitionSensorErrorCode  errorCode;  /**< Recognition Sensor error code. */
} T_HibouLogEntry;

/**
 * Diverter status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct DiverterLogEntry {
  T_DiverterFunctionalStatus  functionalStatus;  /**< Diverter functional status. */
  T_DiverterErrorCode         errorCode;         /**< Diverter error code. */
} T_DiverterLogEntry;

/**
 * Pressure plate status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct PressurePlateLogEntry {
  T_PressurePlateFunctionalStatus  functionalStatus;  /**< PressurePlate functional status. */
  T_PressurePlateErrorCode         errorCode;         /**< PressurePlate error code. */
} T_PressurePlateLogEntry;

/**
 * Loader antifishing flap status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct AntiFishingFlapLogEntry {
  T_FlapFunctionalStatus  functionalStatus;  /**< Flap functional status. */
  T_FlapErrorCode         errorCode;         /**< Flap error code. */
} T_AntiFishingFlapLogEntry;

/**
 * Cashbox module status change log entry.
 */
typedef struct CashboxLogEntry {
  T_ModuleErrorCode  errorCode;  /**< Cashbox module error code. */
} T_CashboxLogEntry;

/**
 * Loader module status change log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct LoaderLogEntry {
  T_LoaderFunctionalStatus  functionalStatus;  /**< Loader module functional status. */
  T_ModuleErrorCode         errorCode;         /**< Loader module error code. */
} T_LoaderLogEntry;

typedef struct RecyclerLogEntry {
  T_RecyclerFunctionalStatus  functionalStatus;  /**< Recycler module functional status. */
  T_ModuleErrorCode           errorCode;         /**< Recycler module error code. */
} T_RecyclerLogEntry;

typedef struct BundlerLogEntry{
  T_BundlerFunctionalStatus  functionalStatus;  /**< Bundler module functional status. */                     
  T_BundlerErrorCode         errorCode;         /**< Bundler error code. */
} T_BundlerLogEntry;

/**
 * Recognition result log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct RecognitionLogEntry {
  UINT16        recognitionResult;  /**< Result of the bill recognition. */
  UINT16        rejectionCode;      /**< Bill rejection code. */
  T_XfsCashType  xfsCashType;
} T_RecognitionLogEntry;

/**
 * Sequence interrupt log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct SequenceInterruptLogEntry {
  UINT16  reason;  /**< Describes why a sequence has been interrupted. */
} T_SequenceInterruptLogEntry;

/**
 * Command result log entry.
 *
 * This class inherits from T_StepLogEntryBaseClass.
 */
typedef struct CommandResultLogEntry {
  UINT16  reason;         /**< Describes why a command failled. */
  UINT16  rejectionCode;  /**< */
} T_CommandResultLogEntry;

/**
 * Shape Info log entry.
 */
typedef struct ShapeInfoLogEntry {
  UINT16  transportabilityTestRegister;   
  UINT16  shapeDetailRegister;
} T_ShapeInfoLogEntry;

#define MAX_BARCODE_CHARACTERS_LENGTH (24+1)

/**
 * Barcode log entry.
 */
typedef struct BarcodeLogEntry {
  UINT16  status;
  char    characters[MAX_BARCODE_CHARACTERS_LENGTH];  
} T_BarcodeLogEntry;

typedef union StepLogEntryType{
  T_StartSequenceLogEntry      startSequence;
  T_SegmentExceptionLogEntry   segmentException;
  T_SectionLogEntry            currentSection;     /**< Bill section in the transport system. */
  T_SectionLogEntry            mainDest;           /**< Main destination of the bill. */
  T_PositionerLogEntry         positioner;         /**< Positioner status change log. */
  T_DetectorLogEntry           detector;           /**< Detector status change log. */
  T_MotorLogEntry              motor;              /**< Motor status change log. */
  T_BundlerDivSystemLogEntry   bundlerDivSystem;   /**< BundlerDivSystem status change log. */
  T_StackerLogEntry            stacker;            /**< Stacker status change log. */
  T_TapePositionLogEntry       tapePosition;       /**< Recycler Tape Position status change log. */
  T_StepTriggerLogEntry        stepTrigger;        /**< Step Trigger status change log. */
  T_HibouLogEntry              hibou;
  T_DiverterLogEntry           diverter;
  T_PressurePlateLogEntry      pressurePlate;
  T_AntiFishingFlapLogEntry    antiFishingFlap;
  T_LoaderLogEntry             loader;
  T_RecyclerLogEntry           recycler;
  T_BundlerLogEntry            bundler;            /**< Bundler change log */
  T_RecognitionLogEntry        recognition;        /**< Recognition result log. */
  T_SequenceInterruptLogEntry  sequenceInterrupt;
  T_CommandResultLogEntry      commandResult;
  T_CashboxLogEntry            cashbox;
  T_ShapeInfoLogEntry          shapeInfo;
  T_BarcodeLogEntry            barcode;
} T_StepLogEntryType;

/**
 * struct of all T_StepLogEntry classes.
 *
 * Those classes inherit from T_StepLogEntryBaseClass.
 */
typedef struct StepLogEntry {
  T_StepLogEntryBaseClass  baseClass;
  T_StepLogEntryType       logtype;
} T_StepLogEntry;  

/**
 * Class for initial and final destination log entry.
 *
 * This class inherits from T_BillTransportLogEntryBaseClass.
 */
typedef struct DestinationLogEntry {
  T_DestLogEntryBaseClass  baseClass;
  UINT16                   initialSection;
  T_CashUnitSpinePosition  initialPosition;
  UINT16                   finalSection;
  T_CashUnitSpinePosition  finalPosition;
} T_DestinationLogEntry;

typedef struct ApplicationLogInfo {
  UINT8              reason;
  T_ResetReasonType  lastResetReason;
  UINT16             param1;
  UINT16             param2;
  UINT32             processedBillCntr;
  UINT16             threadId;
  UINT32             runCount;
} T_ApplicationLogInfo;

typedef struct ApplicationLogEntry {
  T_ApplicationLogEntryBaseClass  baseClass;
  T_ApplicationLogInfo            logInfo;
} T_ApplicationLogEntry;

typedef struct SystemStateLogEntry {
  T_ApplicationLogEntryBaseClass  baseClass;
  UINT16                          operationalState;
} T_SystemStateLogEntry;

/**
 * Union of all T_BillTransportLogEntry classes.
 */
typedef union BillTransportLogEntry {
  T_BillTransportLogEntryBaseClass  baseClass;       /**< Base class of all Bill Transport Log Entry. */
  T_StepLogEntry                    stepLog;         /**< Union of all T_StepLogEntry classes. Those classes inherit from T_StepLogEntryBaseClass. */
  T_DestinationLogEntry             destinationLog;  /**< Class for initial and final destination log entry. This class inherits from T_BillTransportLogEntryBaseClass. */
  T_ApplicationLogEntry             applicationLog;
  T_SystemStateLogEntry             systemLog;
} T_BillTransportLogEntry;

#define MAX_NR_OF_BILL_HANDLING_LOGS (650)  /**< Internal define used to generate audit report */
#define MAX_NR_OF_APPLICATION_LOGS    (60)  /**< Internal define used to generate audit report */
#define MAX_NR_OF_ALL_LOGS           (650)  /**< Internal define used to generate audit report */

/**
 * @private
 * Internal log category used to generate audit report.
 */
typedef enum LogCategory {
  LC_BILL_HANDLING = 1,
  LC_APPLICATION   = 2
} T_LogCategory;

/**
 * @private
 * Internal Structure for holding BNR log entries.
 */
typedef struct BillTransportLogEntries {
  UINT32                   size;
  T_BillTransportLogEntry  logEntries[MAX_NR_OF_ALL_LOGS];
} T_BillTransportLogEntries;

#define MAX_NR_OF_BILL_ERROR_TRANSPORT_LOG_ENTRY (200)

/** @ingroup BNR_REPORTS
 * Bill transport error report.
 *
 * This report is filled  for every failure (hardware or jam) ocurred when  bills are being processed in the transport 
 * bill path (failure not resolved by recovery cycles).
 */
typedef struct BillTransportErrorReport {
  T_FreeRunningCount                 reportNr;                                              /**< The report number. */
  T_SystemUseHistory                 systemUseHistory;                                      /**< System time and usage when the failure occurred. */
  T_BillCount                        escrowContent;                                         /**< The number of bills in the escrow position when the failure occurred. */
  T_FullSystemStatus                 fullSystemStatus;                                      /**< Detailed state of the system and modules when the failure occurred. */
  UINT32                             size;                                                  /**< Number of T_BillTransportLogEntry items available in the array. */
  T_BillTransportLogEntry            logEntries[MAX_NR_OF_BILL_ERROR_TRANSPORT_LOG_ENTRY];  /**< Transport log when the failure occurred. */
  T_ModuleIdentificationsReportList  moduleIds;                                             /**< Module identifications*/
  T_SimplifiedPcuList                pcuList;
} T_BillTransportErrorReport;

#define MAX_NR_OF_BILL_TRANSPORT_ERROR_REPORTS (50)

/** @ingroup BNR_REPORTS
 * Bill transport error report list.
 *
 * List of the last reports in a FIFO buffer. Latest report is at the first position in the list.
 *
 * @see bnr_GetBillTransportErrorReports().
 */
typedef struct BillTransportErrorReportList {
  UINT32                      maxSize;                                          /**< Maximum number of T_BillTransportErrorReport items that can be stored in the array. */
  UINT32                      size;                                             /**< Number of T_BillTransportErrorReport items available in the array. */
  T_BillTransportErrorReport  reports[MAX_NR_OF_BILL_TRANSPORT_ERROR_REPORTS];  /**< Failure reports. */
} T_BillTransportErrorReportList;

#define MAX_NR_OF_BILL_EVENT_TRANSPORT_LOG_ENTRY (100)

/** @ingroup BNR_REPORTS
 * Bill transport event report.
 *
 * This report is filled for every failure (hardware or jam) that the system has resolved with a successful 
 * recovery cycle. In case of recovery with multiple cycles, only the first error recovery is recorded.
 */
typedef struct BillTransportEventReport {
  T_FreeRunningCount                 reportNr;                                              /**< The report number. */
  T_SystemUseHistory                 systemUseHistory;                                      /**< System time and usage when the failure occurred. */
  T_BillCount                        escrowContent;                                         /**< The number of bills in escrow position when the failure occurred. */
  T_FullSystemStatus                 fullSystemStatus;                                      /**< Detailed state of the system and modules when the failure occurred. */
  UINT32                             size;                                                  /**< Number of T_BillTransportLogEntry items available in the array. */
  T_BillTransportLogEntry            logEntries[MAX_NR_OF_BILL_EVENT_TRANSPORT_LOG_ENTRY];  /**< Transport log when the failure occurred. */
  T_ModuleIdentificationsReportList  moduleIds;                                             /**< Module identifications for reports*/
  T_SimplifiedPcuList                pcuList;
} T_BillTransportEventReport;

#define MAX_NR_OF_BILL_TRANSPORT_EVENT_REPORTS (100)

/** @ingroup BNR_REPORTS
 * Bill transport event report list.
 *
 * List of the last reports in a FIFO buffer. Latest report is at the first position in the list.
 *
 * @see bnr_GetBillTransportEventReports().
 */
typedef struct BillTransportEventReportList {
  UINT32                      maxSize;                                          /**< Maximum number of T_BillTransportEventReport that can be stored in the array. */
  UINT32                      size;                                             /**< Number of T_BillTransportEventReport items available in the array. */
  T_BillTransportEventReport  reports[MAX_NR_OF_BILL_TRANSPORT_EVENT_REPORTS];  /**< Failure reports. */
} T_BillTransportEventReportList;

// restore packing
#ifndef WIN32
  #pragma pack(pop)
#else
  #include <poppack.h>
#endif

#endif // _BNR_HIST_W32_H_
