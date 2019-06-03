/******************************************************************************
 *
 *      BNR SDK Project
 *
 ******************************************************************************
 *
 * File :   BnrCtlW32.h
 * 
 * Author : MEI Inc.
 *
 * Date :   20.août.2007
 *
 * History of modifications
 * Date       Rev.    Reason
 *
 ******************************************************************************
 * FILE DESCRIPTION  *//** @file
 * This file contains function definition of the BNR API for Win32.
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
#ifndef _BNR_CTL_W32_H_
#define _BNR_CTL_W32_H_

/*
 * Module Specific Include Files
 */
#include "./XfsConsts.h"
#include "./BnrPropW32.h"
#include "./BnrHistW32.h"

#ifdef WIN32
  #ifdef BNRDLL_EXPORTS
  #define BNRDLL_API __declspec(dllexport)
  #else
  #define BNRDLL_API __declspec(dllimport)
  #endif
  #define CALLTYPE __stdcall

#else
//Linux
  #include <stdint.h>
  #define CALLTYPE
  #define BNRDLL_API
  #define LONG32 int32_t
  #include <libusb-1.0/libusb.h>

    #define T_Time64 time_t
#endif

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef WIN32
BOOL BnrCtlL32_init();

#endif

/** @ingroup BNR_LISTENER
 * Callback function for OperationComplete event.
 *
 * @param[in] identificationId  The id which was given by the operation method to 
 *                              the application, #XFS_E_NOT_USED if not used.
 * @param[in] operationId       The operationID which states what kind of operation 
 *                              is complete.
 * @param[in] result            Specifies the operation result. It is #XFS_RC_SUCCESSFUL 
 *                              if everything was fine.
 * @param[in] extendedResult    An additional int specifying a result, #XFS_E_NOT_USED 
 *                              if not used.
 * @param[in] data              Depending on what asynchronous operation has completed,
 *                              data parameter is NULL, or is pointer to a T_XfsCashOrder.
 *
 * @see T_XfsCashOrder.
 */
typedef void (CALLTYPE *T_OperationCompleteOccuredMethod)(LONG32 identificationId, LONG32 operationId, LONG32 result, LONG32 extendedResult, void *data);

/** @ingroup BNR_LISTENER
 * Callback function for Status event.
 *
 * @param[in] status          The status the device has changed to. The appropriate 
 *                            XFS_S_xxx_CHANGED constants in the XfsConsts.h.
 * @param[in] result          Always set to #XFS_E_NOT_USED.
 * @param[in] extendedResult  Always set to #XFS_E_NOT_USED.
 * @param[in] details         Contains information about current device status, 
 *                            depending on status parameter value.
 *
 * Depending on status parameter value, details parameter has different 
 * meanings, and must be casted as follows :
 * - if status is #XFS_S_CDR_DEVICE_STATUS_CHANGED,
 *   if details is not NULL, it is a pointer to a UINT32 value set to a 
 *   XFS_S_CDR_DS_xxx value.
 * - if status is #XFS_S_CDR_CASH_AVAILABLE or #XFS_S_CDR_CASH_TAKEN,
 *   if details is not NULL, it is a pointer to a T_XfsCashOrder structure.
 * - if status is #XFS_S_CDR_CASHUNIT_CHANGED, #XFS_S_CDR_CASHUNIT_CONFIGURATION_CHANGED or 
 *   #XFS_S_CDR_CASHUNIT_THRESHOLD, details is a pointer to a T_XfsCashUnit structure containing the LCU and PCU that 
 *   have changed.
 * - if status is #XFS_S_CDR_MAINTENANCE_STATUS_CHANGED, if details is not NULL, it is a pointer to T_ModuleIdList structure 
 *   containing ids of module that maintenance is recommended.
 *
 * @see T_XfsCashOrder.
 */
typedef void (CALLTYPE *T_StatusOccuredMethod)(LONG32 status, LONG32 result, LONG32 extendedResult, void *details);

/** @ingroup BNR_LISTENER
 * Callback function for Intermediate event.
 *
 * @param[in] identificationId  The id which was given by the operation method to the application, #XFS_E_NOT_USED if not used.
 * @param[in] operationId       The operationID which states what kind of asynchronous operation is running.
 * @param[in] reason            Specifies what the reason for this event was (e.g. #XFS_I_CDR_INPUT_REFUSED).
 * @param[in] data              Contains the optionally added data for the application or null.
 *
 * Depending on reason parameter value, data parameter has different 
 * meanings, and must be casted as follows :
 * - if reason is #XFS_I_CDR_INPUT_REFUSED, if data is not NULL, it is a 
 *   pointer to a UINT32 containing internal reason code, which can take 
 *   one of the following values of enumeration T_SequenceRejectionCode.
 * - if reason is #BNRXFS_I_CDR_SUBCASHIN, if data is not NULL, it is a 
 *   pointer to a T_XfsCashOrder structure.
 * - if reason is #BNRXFS_I_CDR_BCC_INSERTED, if data is not NULL, it is a 
 *   pointer to a T_BarcodeChars, containing the barcode number.
 * .
 * @see T_XfsCashOrder, T_BarcodeChars, T_SequenceRejectionCode, bnr_CashIn().
 */
typedef void (CALLTYPE *T_IntermediateOccuredMethod)(LONG32 identificationId, LONG32 operationId, LONG32 reason, void *data);

/** @ingroup BNR_DENOMINATIONS
 * Adds a denomination to the BNR.
 *
 * This function will upload denomination data from a DLD file to the BNR. If the denomination
 * already exists, then this command updates the bill and the LCUs are not modified. If it is
 * a new denomination, a corresponding cashbox LCU is automatically created.
 *
 * The intermediate stacker must be empty in order to add denominations. After this command, the BNR is in 
 * #XFS_S_CDR_DS_OFF_LINE state. A bnr_Reset() command is needed to put it in #XFS_S_CDR_DS_ON_LINE state.
 *
 * @param[in] pathFileName Filepath of the DLD file to upload.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage:
 *  - #XFS_E_PARAMETER_INVALID - The given filepath does not point to a valid
 *    DLD file.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_AddDenomination().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 * 
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field data is always NULL, result field can take the following values:
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The intermediate stacker is not empty (e.g. if, during a refloat cycle, a jam
 *    occurred or the cash modules lock has been opened) or there is bills in the transport system and thus
 *    no denomination can be added at the moment.
 *  .
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_AddDenomination(char * pathFileName);


/** @ingroup BNR_DENOMINATIONS
 * Adds a denomination to the BNR.
 *
 * This function will upload denomination data from a DLD file to the BNR. If the denomination
 * already exists, then this command updates the bill and the LCUs are not modified. If it is
 * a new denomination, a corresponding cashbox LCU is automatically created.
 *
 * The intermediate stacker must be empty in order to add denominations. After this command, the BNR is in 
 * #XFS_S_CDR_DS_OFF_LINE state. A bnr_Reset() command is needed to put it in #XFS_S_CDR_DS_ON_LINE state.
 *
 * @param[in] buf   Buffer containing the denomination file data.
 * @param[in] size  Size of the buffer.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage:
 *  - #XFS_E_PARAMETER_INVALID - The given filepath does not point to a valid
 *    DLD file.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_AddDenomination().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field data is always NULL, result field can take the following values:
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The intermediate stacker is not empty (e.g. if, during a refloat cycle, a jam
 *    occurred or the cash modules lock has been opened) or there is bills in the transport system and thus
 *    no denomination can be added at the moment.
 *  .
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_AddDenominationBuffer(char * buf, UINT32 size);


/** @ingroup BNR_CASH_OPERATIONS
 * Attempts to cancel the current asynchronous operation. 
 * 
 * This command has no effect if the asynchronous command is waiting for 
 * a user action after a #XFS_S_CDR_CASH_AVAILABLE status event has been received. 
 * Use bnr_CancelWaitingCashTaken() in this case.
 *
 * If it can do so, an OperationCompleteEvent is sent with the result field 
 * containing #XFS_E_CANCELLED to indicate that the operation was cancelled. 
 * Otherwise, the current operation's messages will be sent as usual.
 * 
 * If this method is called after an operation has completed but before the 
 * operation complete event has been returned to the caller, then no operation 
 * will take place and no error will be returned. 
 * 
 * @return If function call is successful, return is #BXR_NO_ERROR.
 * Otherwise, return is strictly negative and its absolute value contains the
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @see T_BnrXfsResult.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Cancel();


/** @ingroup BNR_CASH_OPERATIONS
 * Asks the BNR to stop waiting for cash removal at the Bezel if any. 
 * 
 * If it can do so, an OperationCompleteEvent is sent with the result field 
 * containing #XFS_E_CANCELLED to indicate that the operation was cancelled. 
 * Otherwise, the current operation's messages will be sent as usual.
 * 
 * This method is meant to be called after the BNR has sent a
 * #XFS_S_CDR_CASH_AVAILABLE status event, and before #XFS_S_CDR_CASH_TAKEN 
 * status event.
 * If this method is called outside these conditions, then no operation will 
 * take place and no error will be returned. 
 * If this method is called after cash has been removed but before the 
 * #XFS_S_CDR_CASH_TAKEN status event has been returned to the caller, then no 
 * operation will take place and no error will be returned. 
 * 
 * @return If function call is successful, return is #BXR_NO_ERROR.
 * Otherwise, return is strictly negative and its absolute value contains the
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.5.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_CancelWaitingCashTaken();


/** @ingroup BNR_CASH_OPERATIONS
 * This command will cause inserted notes to be moved to the escrow. 
 * Notes will be held on the escrow until the current cashIn Transaction 
 * is either cancelled by bnr_CashInRollback() or confirmed by bnr_CashInEnd()
 * command. 
 * 
 * The notes may pass through the banknote reader for identification. 
 * Failure to identify notes does not mean that the command has failed - even 
 * if the banknote reader rejects some or all of the notes, the command may 
 * return #XFS_RC_SUCCESSFUL. 
 * In case of banknote rejection, first a #XFS_I_CDR_INPUT_REFUSED intermediate event
 * is issued, then a #XFS_S_CDR_CASH_AVAILABLE status event is issued to report that 
 * the bill is presented at the inlet, then a #XFS_S_CDR_CASH_TAKEN status event is 
 * issued to report that the user has removed the bill, and the command completes. 
 *
 * After #XFS_S_CDR_CASH_AVAILABLE status event, if no #XFS_S_CDR_CASH_TAKEN status 
 * event is received within a reasonable time period, the application should send a 
 * bnr_CancelWaitingCashTaken() to terminate the command, then send a bnr_Eject() 
 * to clear the bill from the inlet. 

 * The bnr_CashIn() command may be terminated by using bnr_Cancel(). However, if 
 * bnr_Cancel() is sent while a banknote is being processed, this may induce rejection 
 * of the banknote towards the Inlet. In this case, the description above applies.
 *
 * @param[in] amount        Amount to accept with this operation. If this 
 *                          parameter is NULL, the BNR will accept only one 
 *                          banknote. If the amount is 0, banknotes will be 
 *                          accepted until the escrow is full, or a bnr_Cancel()
 *                          command is called. If the amount is different from 
 *                          0, banknotes will be accepted until the amount is
 *                          reached, or the escrow is full, or a bnr_Cancel()
 *                          command is called. 
 * @param[in] currencyCode  Currency to accept during this operation. If this 
 *                          parameter is NULL or the string is empty, any 
 *                          currency will be accepted by the BNR.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, or a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_NO_CASHIN_STARTED - A successful bnr_CashInStart() call was 
 *    not done before calling bnr_CashIn().
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_CASH_AVAILABLE - Sent when a rejected bill is back at inlet, 
 *    presented to the user for removal.
 *  - #XFS_S_CDR_CASH_TAKEN - Sent when a bill has been taken by the user.
 *  - #XFS_S_CDR_CASHUNIT_CHANGED - Sent when a bill has accepted by the BNR and
 *    escrowed in cash unit.
 *
 * @par IntermediateEvent event:
 *  The field reason can take the following values :
 *  - #XFS_I_CDR_INPUT_REFUSED - Sent if the note has not been identified and 
 *    validated by the BNR. data field contains BNR internal reason code.
 *  - #BNRXFS_I_CDR_SUBCASHIN - Sent each time a banknote has been recognized 
 *    in the case where amount parameter is not NULL.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CANCELLED - The command has been terminated because:
 *    - bnr_Cancel() has been called while the BNR was waiting for bill insertion, and amount
 *      parameter was not NULL.
 *    - bnr_CancelWaitingCashTaken() has been called while the BNR was waiting for bill removal
 *      from the Bezel.
 *    .
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - Escrow full, the number of inserted bills 
 *    equals the maximum escrow capacity.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  - #BNRXFS_E_CDR_USER_ERROR - A person has stopped the device operation, i.e. manupulating
 *    the outlet flap or inserting a bill at the inlet when it is not expected.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * If result field is #XFS_E_ILLEGAL, data field is NULL. Otherwise, it is a
 * pointer to a T_XfsCashOrder structure with the following properties:
 *  - If all accepted bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If all accepted bills are of the same currency, then T_XfsCashOrder.denomination.amount
 *    holds the sum of all the bills' values; otherwise it is 0.
 *  - T_XfsCashOrder.denomination.cashbox is always 0.
 *  - T_XfsCashOrder.denomination.size holds number of elements in the items-array.
 *  - T_XfsCashOrder.denomination.items holds the number of stored bills for each LCU involved.
 *
 * @see bnr_CashInStart(), bnr_CashInEnd(), bnr_CashInRollback(), bnr_Cancel(), bnr_CancelWaitingCashTaken().
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_CashIn(T_Amount *amount, T_XfsCurrencyCode *currencyCode);


/** @ingroup BNR_CASH_OPERATIONS
 * This command is used to end the cash in transaction.
 *
 * All the notes cashed in since the last bnr_CashInStart() or 
 * bnr_CashInRollback() call are stored (encashed) in the BNR. 
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), a banknote is waiting at outlet, etc...
 *  - #XFS_E_CDR_NO_CASHIN_STARTED - A successful bnr_CashInStart() call was 
 *    not done before calling bnr_CashInEnd().
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL -  The command has been executed successfully. 
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  - #BNRXFS_E_CDR_USER_ERROR - A person has stopped the device operation, i.e. manupulating
 *    the outlet flap or inserting a bill at the inlet when it is not expected.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * If result field is #XFS_E_ILLEGAL, data field is NULL. Otherwise, it is a
 * pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If all stored bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If all stored bills are of the same currency, then T_XfsCashOrder.denomination.amount
 *    holds the sum of all the bills' values; otherwise it is 0.
 *  - T_XfsCashOrder.denomination.cashbox is always 0.
 *  - T_XfsCashOrder.denomination.size holds number of elements in the items-array.
 *  - T_XfsCashOrder.denomination.items holds the number of stored bills for each LCU involved.
 *
 * @see bnr_CashInStart(), bnr_CashIn().
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_CashInEnd();


/** @ingroup BNR_CASH_OPERATIONS
 * This command is used to roll back the cash in transaction. 
 *
 * All the notes cashed in since the last bnr_CashInStart() call are returned 
 * to the teller. 
 *
 * The command completes when the bills are taken by the user at the outlet of 
 * the BNR. A status event is generated to report that the user has removed 
 * the bills. If no event is received within a reasonable time period, 
 * the application should send a bnr_CancelWaitingCashTaken() to terminate the command,
 * then send a bnr_Retract() method to clear the bills from the outlet. 
 *
 * Cancelling this command using bnr_Cancel() will not stop the banknotes to be 
 * returned, but will prevent the BNR to wait for banknote removal at outlet. 
 * Then a bnr_Retract() command is needed to take the banknotes back inside the BNR.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_NO_CASHIN_STARTED - A successful bnr_CashInStart() call was 
 *    not done before calling bnr_CashIn().
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_CASH_AVAILABLE - Sent when the rolled back bills are at outlet, 
 *    presented to the user for removal. data field is a T_XfsCashOrder structure 
 *    containing the amount and bill list presented to the user.
 *  - #XFS_S_CDR_CASH_TAKEN - Sent when the bills have been taken by the user.
 *    The field data is a T_XfsCashOrder structure containing the amount and bill list 
 *    taken by the user.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because:
 *    - bnr_Cancel() has been called while the BNR was preparing the bills to be
 *      given back to the user.
 *    - bnr_CancelWaitingCashTaken() has been called while the BNR was waiting for bill removal
 *      from the Bezel.
 *    .
 *  - #XFS_E_CDR_NO_BILLS - No bills were rolled back because escrow is empty.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - The command failed because the bundler module
 *    became full, due to bad positionning of bills on it.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *  
 *  If result field is #XFS_E_ILLEGAL, data field is NULL. Otherwise, it is a
 *  pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If all returned bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If all returned bills are of the same currency, then T_XfsCashOrder.denomination.amount
 *    holds the sum of all the bills' values; otherwise it is 0.
 *  - T_XfsCashOrder.denomination.cashbox is always 0.
 *  - T_XfsCashOrder.denomination.size holds number of elements in the items-array.
 *  - T_XfsCashOrder.denomination.items holds the number of returned bills for each LCU involved.
 *  .
 *  If result field is #XFS_E_CDR_NO_BILLS, data field is a pointer to a 
 *  #T_XfsCashOrder structure with the fields T_XfsCashOrder.denomination.amount, 
 *  T_XfsCashOrder.denomination.cashbox and T_XfsCashOrder.denomination.size to 0.
 *  .
 *
 * @see bnr_CashInStart(), bnr_CashInEnd(), bnr_CashIn(), bnr_CancelWaitingCashTaken().
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_CashInRollback();


/** @ingroup BNR_CASH_OPERATIONS
 * This command is used to start the cash in transaction with the BNR.
 *
 * Each cash in procedure has to be handled as a transaction that can be 
 * rolled back in any case if a difference occurs between the amount counted 
 * by the BNR and the amount the teller inserted. 
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A bnr_CashInStart() call has already been issued.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - The BNR is not in an operational state. 
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  @deprecated #XFS_E_HARDWAREERROR - The command was differed because it was received 
 *    while the BNR was performing an internal test, and the result of the test 
 *    set the BNR to not operational state - no more used since FW v1.3.0, use 
 *    #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 *  data field is not used.
 *
 * @see bnr_CashIn(), bnr_CashInEnd().
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_CashInStart();


/** @ingroup BNR_INIT
 * Ends the communication with the BNR and terminates the thread that has been started by a previous bnr_Open() call.
 * 
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @see bnr_Open().
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Close();


/** @ingroup BNR_CASH_UNITS
 * Configures the BNR's cash unit. This function is used to add or remove Logical and Physical Cash Unit in the BNR.
 *
 * Those settings are persistent over power cycles.
 *
 * @param[in]      transportCount        Number of bills in the transport system
 * @param[in, out] logicalCashUnitList   T_LogicalCashUnitList structure
 * @param[in, out] physicalCashUnitList  T_PhysicalCashUnitList structure
 *
 * @return #BXR_NO_ERROR if successful, or the following error code :
 *  - #XFS_E_BUSY - The BNR is moving bill or about to move and cannot update CashUnit information,
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. a dispense transaction is active in the BNR, the Cash Modules Lock is opened, etc...
 *  - #XFS_E_PARAMETER_INVALID - At least one of the parameters is a NULL pointer, or there are inconsistencies between LCU list and PCU list,
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_ConfigureCashUnit(),
 *  - #XFS_E_CDR_TOO_MANY_BILLS - No transaction is active, but the BNR has bills in escrow, or in its transport system,
 *  - #XFS_E_CDR_INVALID_CASH_UNIT - An invalid Cash Unit information was sent to the BNR; typically, this error can happen when :
 *    - an invalid PCU name is passed,
 *    - a PCU/LCU is listed more than once in the PCU/LCU list.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - An unauthorized Cash Unit modification was requested; typically , this error can happen when :
 *    - a PCU bill count doesn't match with the sum of all LCU bill counts attached to this LCU,
 *    - a Recycler PCU/LCU count doesn't match with the counts inside the BNR,
 *    - a Recycler or Bundler PCU threshold mode is set to SensorMode instead of CountMode,
 *    - the Bundler PCU threshold levels don't match with the levels inside the BNR,
 *    - attempting to lock the Bundler or Cashbox PCU.
 *    - If more than one cash type (note series or denomination) is configured to the same cash unit AND the differential length among any two > 5 mm (LO and RE)  
 *    - If more than one cash type (note series or denomination) is configured to the same cash unit AND the differential width among any two is > 1 mm (LO only)
 *
 * @see bnr_QueryCashUnit(), bnr_UpdateCashUnit().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_ConfigureCashUnit( 
  UINT32 transportCount, T_LogicalCashUnitList * logicalCashUnitList, T_PhysicalCashUnitList * physicalCashUnitList);


/** @ingroup BNR_DENOMINATIONS
 * Deletes a denomination in the BNR.
 *
 * The intermediate stacker must be empty in order to delete denominations. After this command, the BNR is in 
 * #XFS_S_CDR_DS_OFF_LINE state. A bnr_Reset() command is needed to put it in #XFS_S_CDR_DS_ON_LINE state.
 *
 * @param[in] currencyCode  Currency code of the denomination to delete.
 * @param[in] value         Value of the denomination to delete (expressed in MDU).
 * @param[in] variant       Variant of the denomination to delete.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage:
 *  - #XFS_E_PARAMETER_INVALID - No denomination corresponding to the parameters was found in the BNR.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_AddDenomination().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, or a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field data is always NULL, result field can take the following values:
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The intermediate stacker is not empty (e.g. if, during a refloat cycle, a jam
 *    occurred or the cash modules lock has been opened) or there is bills in the transport system and thus
 *    no denomination can be deleted at the moment.
 *  .
 *
 * @see bnr_AddDenomination().
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_DeleteDenomination(char* currencyCode, UINT32 value, UINT32 variant);


/** @ingroup BNR_CASH_OPERATIONS
 * Determines if the amount requested by value or by bill list, is available for dispense.
 *
 * Three methods are possible:
 *
 *  - denominateRequest->mixNumber is #XFS_C_CDR_MXA_MIN_BILLS: The BNR chooses the banknotes
 *    to be distributed in order to obtain the total amount using the minimum number of banknotes. 
 *    Two parameters must be correctly set:
 *    - denominateRequest->denomination.amount has to be expressed in MDUs
 *    - denominateRequest->currency.currencyCode is a string. See this page for a full list of the existing ISO currency codes:
 *      http://www.iso.org/iso/home/standards/currency_codes.htm
 *
 *  - denominateRequest->mixNumber is #BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE: The BNR chooses the banknotes 
 *    to be distributed in order to obtain the total amount in a way that slows down cashbox filling. 
 *    As long as the low denomination Recyclers are not near to full, change is determined like with 
 *    the MinBills algorithm. But when a Recycler becomes nearly full (5/6 of Full threshold), this 
 *    algorithm will try to put more of this denomination in the change so that the Recycler doesn't 
 *    become full and this denomination doesn't start to be cashed.
 *    Two parameters must be correctly set:
 *    - denominateRequest->denomination.amount has to be expressed in MDUs
 *    - denominateRequest->currency.currencyCode is a string. See this page for a full list of the existing ISO currency codes:
 *      http://www.iso.org/iso/home/standards/currency_codes.htm
 *
 *  - denominateRequest->mixNumber is #XFS_C_CDR_MIX_DENOM: The user chooses through a list of 
 *    Logical Cash Units the banknotes to be distributed by the BNR in order to obtain the total amount. 
 *    The following parameters must be correctly set:
 *    - denominateRequest->denomination.size gives the size of the items array 
 *    - for each item of denominateRequest->denomination.items from 0 to (denominateRequest->denomination.size - 1):
 *      - denominateRequest->denomination.items[item].unit contains the number of a LCU from where banknotes must be distributed.
 *      - denominateRequest->denomination.items[item].count gives the number of banknotes to distribute from the LCU.
 *
 * @param[in] denominateRequest Requested amount or bill list.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - Another asynchronous command is already running.
 *  - #XFS_E_CDR_INVALID_MIXNUMBER - The mix number specified in the request is
 *    unknown, or is #XFS_C_CDR_MXA_MIN_BILLS or #BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE, 
 *    but the requested amount is 0.
 *  - #XFS_E_NOT_SUPPORTED - The unit is a BNA6.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event :
 * The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The BNR has enough bills to satisfy the request.
 *  - #XFS_E_CDR_NOT_DISPENSABLE - Not enough bills to satisfy the request.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The number of bills needed to satisfy the 
 *    request would exceed the BNR's limitation.
 *  - #XFS_E_CDR_NO_BILLS - The bill list passed as parameter was empty.
 *  - #XFS_E_PARAMETER_INVALID - At least one unitId in the list of specified 
 *    bills is unknown.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *
 * @par
 * If result field is #XFS_E_PARAMETER_INVALID or #XFS_E_ILLEGAL, data field is NULL.
 *
 * @par
 * If result field is #XFS_E_CDR_NO_BILLS, data field is a pointer to a T_XfsCashOrder structure with the fields 
 * T_XfsDenomination.amount, T_XfsDenomination.cashbox and T_XfsDenomination.size to 0.
 *
 * @par
 * Otherwise, it is a pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If all requested bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If not all requested bills are of the same currency, then the field
 *    T_XfsDenomination.amount is 0, otherwise it holds the sum of all requested
 *    bills (if result is #XFS_RC_SUCCESSFUL) or the amount which could be dispensed (if
 *    result is #XFS_E_CDR_NOT_DISPENSABLE or #XFS_E_CDR_TOO_MANY_BILLS).
 *  - If result is #XFS_RC_SUCCESSFUL or not all requested bills are of the same currency,
 *    T_XfsDenomination.cashbox is 0; otherwise (result is
 *    #XFS_E_CDR_NOT_DISPENSABLE or #XFS_E_CDR_TOO_MANY_BILLS) it is equal to the amount
 *    requested minus the amount dispensable.
 *  - T_XfsDenomination.size holds number of elements in the items-array.
 *  - T_XfsDenomination.items holds the number of stored bills for each LCU involved.
 *
 * @see bnr_Dispense().
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Denominate(T_XfsDispenseRequest* denominateRequest);


/** @ingroup BNR_CASH_OPERATIONS
 * Dispenses the amount requested by value or by  bill list. 
 *
 * The BNR will make a bundle of notes and wait for the bnr_Present() command 
 * to give it to the customer.
 *
 * Three methods are possible:
 *
 *  - dispenseRequest->mixNumber is #XFS_C_CDR_MXA_MIN_BILLS: The BNR chooses the banknotes
 *    to be distributed in order to obtain the total amount using the minimum number of banknotes. 
 *    Two parameters must be correctly set:
 *    - dispenseRequest->denomination.amount has to be expressed in MDUs
 *    - dispenseRequest->currency.currencyCode is a string. See this page for a full list of the existing ISO currency codes:
 *      http://www.iso.org/iso/home/standards/currency_codes.htm
 *
 *  - dispenseRequest->mixNumber is #BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE: The BNR chooses the banknotes
 *    to be distributed in order to obtain the total amount in a way that slows down cashbox filling. 
 *    As long as the low denomination Recyclers are not near to full, change is determined like with 
 *    the MinBills algorithm. But when a Recycler becomes nearly full (5/6 of Full threshold), this 
 *    algorithm will try to put more of this denomination in the change so that the Recycler doesn't 
 *    become full and this denomination doesn't start to be cashed.
 *    Two parameters must be correctly set:
 *    - dispenseRequest->denomination.amount has to be expressed in MDUs
 *    - dispenseRequest->currency.currencyCode is a string. See this page for a full list of the existing ISO currency codes:
 *      http://www.iso.org/iso/home/standards/currency_codes.htm
 *
 *  - dispenseRequest->mixNumber is #XFS_C_CDR_MIX_DENOM: The user chooses through a list of 
 *    Logical Cash Units the banknotes to be distributed by the BNR in order to obtain the total amount. 
 *    The following parameters must be correctly set:
 *    - dispenseRequest->denomination.size gives the size of the items array 
 *    - for each item of dispenseRequest->denomination.items from 0 to (dispenseRequest->denomination.size - 1):
 *      - dispenseRequest->denomination.items[item].unit contains the number of a LCU from where banknotes must be distributed.
 *      - dispenseRequest->denomination.items[item].count gives the number of banknotes to distribute from the LCU.
 *
 * @param[in] dispenseRequest Amount or bill list requested for dispense.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a
 *    successful bnr_Dispense() call has already been done, etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd() 
 *    before calling bnr_Dispense().
 *  - #XFS_E_CDR_INVALID_MIXNUMBER - The mix number specified in the request is
 *    unknown, or is #XFS_C_CDR_MXA_MIN_BILLS or #BNRXFS_C_CDR_MXA_OPTIMUM_CHANGE, 
 *    but the requested amount is 0.
 *  - #XFS_E_NOT_SUPPORTED - The unit is a BNA6.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event :
 * The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The request is available and can be presented to the
 *    customer. The data field is a T_XfsCashOrder containing the amount and bill 
 *    list that can be presented to the customer with a bnr_Present() call.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - During bills move, a cash unit has less bills
 *    than expected (based on its bill count) at the beginning of the operation 
 *    or the bundler is full. The data field is a T_XfsCashOrder containing the 
 *    amount and bill list that may be presented to the customer with a 
 *    bnr_Present() call.
 *  - #XFS_E_CDR_NOT_DISPENSABLE - Not enough bills to satisfy the request. No 
 *    bill has been moved and the data field is a T_XfsCashOrder containing the 
 *    maximum amount and bill list that could be used for this request.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The number of bills needed to satisfy the 
 *    request exceeds the BNR's limitation. No bill has been moved and the data
 *    field is a T_XfsCashOrder containing the list of bills that should be used 
 *    for this request.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_CDR_NO_BILLS - The bill list passed as parameter was empty.
 *  - #XFS_E_PARAMETER_INVALID - At least one unitId in the list of specified 
 *    bills is unknown.
 *  - #XFS_E_CANCELLED - The command has been terminated because:
 *    - bnr_Cancel() has been called while the BNR was preparing the bills to be
 *      dispensed to the user.
 *    - bnr_CancelWaitingCashTaken() has been called while the BNR was waiting for bill removal
 *      from the Bezel (in AutoPresent mode).
 *    .
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  - #BNRXFS_E_CDR_USER_ERROR - A person has stopped the device operation, i.e. manupulating
 *    the outlet flap or inserting a bill at the inlet when it is not expected.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * @par
 * If result field is #XFS_E_PARAMETER_INVALID or #XFS_E_ILLEGAL, data field is NULL.
 *
 * @par
 * If result field is #XFS_E_CDR_NO_BILLS, data field is a pointer to a T_XfsCashOrder structure with the fields 
 * T_XfsDenomination.amount, T_XfsDenomination.cashbox and T_XfsDenomination.size to 0.
 *
 * @par
 * If result field is #XFS_RC_SUCCESSFUL, #XFS_E_CDR_NO_BILLS, #XFS_E_CDR_NOT_DISPENSABLE or #XFS_E_CDR_TOO_MANY_BILLS, 
 * data field is a pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If all requested bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If not all requested bills are of the same currency, then the field
 *    T_XfsCashOrder.denomination.amount is 0, otherwise it holds the sum of all requested
 *    bills (if result is #XFS_RC_SUCCESSFUL) or the amount which could be dispensed (if
 *    result is #XFS_E_CDR_NOT_DISPENSABLE or #XFS_E_CDR_TOO_MANY_BILLS).
 *  - If result is #XFS_RC_SUCCESSFUL or not all requested bills are of the same currency,
 *    T_XfsCashOrder.denomination.cashbox is 0; otherwise (result is
 *    #XFS_E_CDR_NOT_DISPENSABLE or #XFS_E_CDR_TOO_MANY_BILLS) it is equal to the amount
 *    requested minus the amount dispensable.
 *  - T_XfsDenomination.size holds number of elements in the items-array.
 *  - T_XfsDenomination.items holds the number of stored bills for each LCU involved.
 *
 * @par
 * If result field is #XFS_E_CDR_CASH_UNIT_ERROR, #XFS_E_CDR_CASH_DEVICE_ERROR,
 * #XFS_E_HARDWAREERROR or #XFS_E_CANCELLED, data field is a pointer to a T_XfsCashOrder
 * structure with the following properties:
 *  - If all bills that made it into the Bundler are of the same currency, then
 *    T_XfsCashOrder.currency holds the information of that currency; otherwise it
 *    is empty (meaning that exponent = 0, currencyCode consists of NULL-characters).
 *  - If all bills that made it into the Bundler are of the same currency, then the field
 *    T_XfsDenomination.amount holds the sum of those bills, otherwise it is 0.
 *  - If all bills that did not make it into the Bundler are of the same currency,
 *    T_XfsDenomination.cashbox holds the sum of those bills, otherwise it is 0.
 *  - T_XfsDenomination.size holds number of elements in the items-array.
 *  - T_XfsDenomination.items holds the number of stored bills for each LCU involved.
 *
 * @see bnr_Denominate(), bnr_Present(), bnr_Reject().
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Dispense(T_XfsDispenseRequest* dispenseRequest);


/** @ingroup BNR_CASH_OPERATIONS
 * This command allows the application to force cash that has been presented to be ejected from the bezel. 
 *
 * - If this command is called during a CashIn transaction, the application can then continue this transaction, 
 * using bnr_CashIn(), bnr_CashInRollback() or bnr_CashInEnd();
 * - If this command is called during a Dispense transaction, if it succeeds, then the transaction is finished.
 *
 * This method may only be called after bills have been presented at the inlet or the outlet following a 
 * bnr_CashIn() (bill has been refused), bnr_Dispense() (if autoPresent mode is active), bnr_CashInRollback() 
 * or bnr_Present() method call, and before the bills have been taken by the user. 
 *
 * @note 
 * An asynchronous method must not be called before the preceding one is terminated (i.e. 
 * OperationComplete event has been received); typically before calling bnr_Eject(), the preceding
 * command must be terminated by calling bnr_CancelWaitingCashTaken().
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, no CashIn nor Dispense transaction active, etc...
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_CASH_TAKEN - Sent when bills have been taken by the user, or ejected outside the BNR.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully; also when
 *    bills have already been taken by the user.
 *  - #XFS_E_ILLEGAL - No bills have been presented at the Bezel. 
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #BNRXFS_E_CDR_USER_ERROR - Bills could not be correctly ejected, probably due to the user. 
 *  .
 * @par
 * If result is #XFS_RC_SUCCESSFUL, data field is a pointer to a #T_XfsCashOrder structure filled in the
 * same manner as the previous #XFS_S_CDR_CASH_AVAILABLE StatusEvent.
 * 
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.5.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_CancelWaitingCashTaken().
 * @see T_BnrXfsResult.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Eject();


/** @ingroup BNR_CASH_OPERATIONS
 * Empties a recycler or loader cash unit in the cashbox.
 *
 * bnr_Cancel() will terminate the command after the current bundle is finished 
 * and stored in the cashbox.
 *
 * @param[in] pcuName Name of the physical cash unit to empty.
 * @param[in] toFloat If TRUE, the command empties up to the low threshold of the Physical Cash Unit, otherwise to zero.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_PARAMETER_INVALID - The PCU name is not valid.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd() 
 *    before calling bnr_Empty().
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 * The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully, and 
 *    data field is a T_XfsCashOrder structure containing the bills moved to 
 *    the cashbox.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - There was a failure because the Bundler was not
 *    empty in the beginning; or because the Bundler or the Cashbox became full
 *    while moving bills; or because a cash unit is not operational.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because bnr_Cancel() 
 *    command has been called.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation.
 *  - #BNRXFS_E_CDR_USER_ERROR - A person has stopped the device operation, i.e. manupulating
 *    the outlet flap or inserting a bill at the inlet when it is not expected.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * @par
 * If result field is #XFS_E_ILLEGAL, data field is NULL. Otherwise, it is a
 * pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If all encashed bills are of the same currency, then T_XfsCashOrder.currency
 *    holds the information of that currency; otherwise it is empty (meaning that
 *    exponent = 0, currencyCode consists of NULL-characters).
 *  - If all encashed bills are of the same currency, then T_XfsDenomination.amount
 *    holds the sum of all the bills' values; otherwise it is 0.
 *  - T_XfsDenomination.cashbox is always 0.
 *  - T_XfsDenomination.size holds number of elements in the items-array.
 *  - T_XfsDenomination.items holds the number of stored bills for each LCU involved.
 *
 * @see T_BnrXfsResult, T_XfsCashOrder
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Empty(char * pcuName, BOOL toFloat);

/** @ingroup BNR_SYS_CONFIG
 * Enables in the BNR the status events listed in parameter.
 *
 * Those settings are not persistent over power cycles, and must be set on 
 * power up by bnr_EnableStatusEvents(). If not set, by default on power up, 
 * all events defined prior to FW v1.7.0 are enabled and all new 
 * events introduced in this version and later, are disabled to preserve the 
 * legacy integrations. In other words:
 * -# default enabled status events are:
 *   - #XFS_S_CDR_CASHUNIT_CHANGED,
 *   - #XFS_S_CDR_CASHUNIT_CONFIGURATION_CHANGED,
 *   - #XFS_S_CDR_CASHUNIT_THRESHOLD,
 *   - #XFS_S_CDR_DEVICE_STATUS_CHANGED,
 *   - #XFS_S_CDR_CASH_TAKEN,
 *   - #XFS_S_CDR_CASH_AVAILABLE.
 * -# default disabled status events are:
 *   - #XFS_S_CDR_MAINTENANCE_STATUS_CHANGED.
 *
 * @param[in]  list List of status event to enable
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_GetEnabledStatusEvents(), T_StatusEventIdList.
 * @see T_StatusOccuredMethod.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_EnableStatusEvents(const T_StatusEventIdList *list);


/** @ingroup BNR_SYSTEM_STATUS
 * Creates a file with the data of history and report command.
 *
 * By default, the generated file will not contain BillTransportEventReports; if
 * these data are needed, BnrCtl.ini must define ENABLE_FULL_AUDIT_REPORT=1 in
 * section [Capabilities].
 *
 * The generated file can be compressed to a zip format if the BnrCtl.ini file
 * defines ENABLE_AUDIT_REPORT_FILE_COMPRESSION=1 in section [Capabilities].
 *
 * @param[in]  pathFileName Full path and file name for the report.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 * @see @ref INI_FILE.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GenerateAuditReport(const char * pathFileName);


/** @ingroup BNR_HISTORY
 * Gets the BNR Bill acceptance history.
 *
 * @param[out]  history Bill Acceptance History data.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetBillAcceptanceHistory(T_BillAcceptanceHistory *history);


/** @ingroup BNR_HISTORY
 * Gets the Bill dispense history.
 *
 * @param[out]  history Bill Dispense History data.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *  - #BXR_API_INVALID_PARAMETER - The function was called with a null pointer as a parameter.
 *  - #XFS_E_NOT_SUPPORTED - The unit is a BNA6.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetBillDispenseHistory(T_BillDispenseHistory *history);


/** @ingroup BNR_REPORTS
 * Gets the bill transport error reports.
 *
 * @param[out]  list List of bill transport error reports.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetBillTransportErrorReports(T_BillTransportErrorReportList *list);


/** @ingroup BNR_REPORTS
 * Gets the bill transport event reports.
 *
 * @param[out]  list List of bill transport event reports.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetBillTransportEventReports(T_BillTransportEventReportList *list);

/** @ingroup BNR_SYS_CONFIG
 * Gets the BNR capabilities.
 *
 * @note When calling this function, you must set capabilities->positionCapabilitiesList.maxSize to its correct
 *        value. (The standard value is #MAX_NR_OF_POSITIONS, but this may vary in your case.)
 *
 * @param[out] capabilities BNR's capabilities information.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_SetCapabilities(), T_XfsCapabilities.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetCapabilities(T_XfsCapabilities *capabilities);


/** @ingroup BNR_SYS_CONFIG
 * Gets the BNR date and time.
 *
 * @param[out]  dateTime Date and time returned by the BNR. If not set by bnr_SetDateTime(), the BNR time starts from 2001/01/01 00:00:00 at 
 *              power up.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_SetDateTime().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetDateTime(T_Time64 *dateTime);


/** @ingroup BNR_SYS_CONFIG
 * Gets the default bill routing table of the BNR.
 *
 * @param[in] routingTable Bill Routing Table
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_SetDefaultBillRouting().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetDefaultBillRouting(T_BillRoutingTable *routingTable);

/** @ingroup BNR_SYS_CONFIG
 * Gets the satus events enabled in the BNR.
 *
 * @param[out]  list List of status events enabled in the BNR.
 *
 * @return #BXR_NO_ERROR if successful or the error code. 
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_EnableStatusEvents(), T_StatusEventIdList.
 * @see T_StatusOccuredMethod.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetEnabledStatusEvents(T_StatusEventIdList *list);

/** @ingroup BNR_HISTORY
 * Gets the BNR failure history.
 *
 * @param[out]  history System Failure History data.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetFailureHistory(T_SystemFailureHistory *history);

/** @ingroup BNR_SYS_CONFIG
 * Gets the firmware package version of the BNR and tells if the BNR firmwares (including modules and Fpga) 
 * are up to date compared to this version.
 *
 * @param[out] firmwarePackageVersion   Version of the firmware package.
 * @param[out] isUpToDate               TRUE if the versions of all module firmwares are equal to the 
 *                                      corresponding ones in the package, FALSE otherwise.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetFirmwarePackageVersion(T_FullVersion *firmwarePackageVersion, BOOL *isUpToDate);


/** @ingroup BNR_REPORTS
 * Gets the history reports.
 *
 * @param[out] list List of history reports.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_SaveHistoryReport(), T_HistoryReport.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetHistoryReports(T_HistoryReportList *list);


/** @ingroup BNR_SYS_CONFIG
 * Gets the ids of all modules defined in the BNR. Those ids are then used for each module_XXXX call.
 *
 * @param[out] moduleList Memory allocated structure of type T_ModuleIdList that will be filled with the information.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetModules(T_ModuleIdList *moduleList);


/** @ingroup BNR_HISTORY
 * Gets the BNR Restart history.
 *
 * @param[out]  history System restart and opening History data.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetRestartHistory(T_SystemRestartHistory *history);


/** @ingroup BNR_REPORTS
 * Gets the BNR simple failure reports.
 *
 * @param[out]  list List of simple failure reports.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetSimpleFailureReports(T_SimpleFailureReportList *list);


/** @ingroup BNR_SYS_CONFIG
 * Gets the special configuration status of the BNR.
 *
 * @param[out] specialConfiguration String which will receive the special 
 *                                  configuration of the BNR ("00000000-00" if
 *                                  no special configuration is active).
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage:
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. 
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.5.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_SetSpecialConfiguration().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetSpecialConfiguration(T_SpecialConfiguration *specialConfiguration);


/** @ingroup BNR_SYSTEM_STATUS
 * Gets the XFS status of the BNR. This status is typically used by the 
 * transaction controller.
 *
 * @param[out] bnrStatus T_XfsCdrStatus structure.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. 
 *
 * @see 
 *  - T_BnrXfsResult for the full error code description.
 *  - bnr_GetSystemStatus and module_GetStatus
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetStatus(T_XfsCdrStatus * bnrStatus);


/** @ingroup BNR_SYS_CONFIG
 * Gets the system configuration status of the BNR.
 *
 * @param[out] configuration Pointer to a #T_SystemConfiguration structure which
 *                            will receive the system configuration of the BNR.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage:
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. 
 *
 * @see bnr_SetSystemConfiguration().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetSystemConfiguration(T_SystemConfiguration* configuration);


/** @ingroup BNR_SYSTEM_STATUS
 * Gets the BNR specific status. This command is typically used for diagnostics and 
 * detailed information.
 *
 * @note When calling this function, you must set status.maxSize to its correct
 *        value. (The standard value is #MAX_NR_OF_MODULES, but this may vary in your case.)
 *
 * @param[out] status Pointer to an initialized #T_SystemStatusOverview structure; will receive
 *                          the current system status of the BNR. Note that the field
 *                          status.maxSize must be correctly initialized by the caller.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. 
 *
 * @see 
 *  - T_BnrXfsResult for the full error code description.
 *  - bnr_GetStatus() and module_GetStatus().
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetSystemStatus(T_SystemStatusOverview * status);


/** @ingroup BNR_SYSTEM_STATUS
 * Gets the BNR transaction status.
 *
 * The transaction status aims to help the host to recover with the BNR operation after
 * a communication breakdown.
 *
 * @param[out] status Pointer to a #T_TransactionStatus structure; will receive
 *                    the current transaction status of the BNR.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. 
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetTransactionStatus(T_TransactionStatus *status);


/** @ingroup BNR_HISTORY
 * Gets the BNR use history.
 *
 * @param[out]  history System time and usage History data.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_GetUseHistory(T_SystemUseHistory *history);


/** @ingroup BNR_INIT
 * Issues a request through the USB to determine if a BNR is connected, and if
 * a BNR is connected and powered, starts the thread to monitor the BNR and 
 * to communicate with it. 
 * This function takes three callback functions as parameters to handle the 
 * different events the BNR can send.
 *
 * @param[in] operationCompleteOccured  Callback function for operation 
 *                                      complete event.
 * @param[in] statusOccured             Callback function for status change
 *                                      event.
 * @param[in] intermediateOccured       Callback function for intermediate 
 *                                      event.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage :
 *  - #BXR_USB_NO_SUCH_DEVICE - No BNR has been detected.
 *  - #BXR_API_MISSING_DEPENDENCY - The API was not able to delay-load a DLL
 *      it needs (check the error log for further details).
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * T_BnrXfsResult for the full error code description.
 *
 * @see bnr_Close().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Open(
  T_OperationCompleteOccuredMethod operationCompleteOccured, 
  T_StatusOccuredMethod statusOccured,
  T_IntermediateOccuredMethod intermediateOccured);


/** @ingroup BNR_INIT
 * This is an overloaded member function of bnr_Open(), provided for 
 * convenience. It differs from the above function only in what arguments it 
 * accepts.
 *
 * @param[in] serialNr                  BNR's serial number to be used.
 * @param[in] operationCompleteOccured  Callback function for operation 
 *                                      complete event.
 * @param[in] statusOccured             Callback function for status change
 *                                      event.
 * @param[in] intermediateOccured       Callback function for intermediate 
 *                                      event.
 *
 * @return See bnr_Open().
 *
 * @see bnr_Open(), bnr_Close().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_OpenWithSerialNr(
  T_XfsUnitId serialNr,
  T_OperationCompleteOccuredMethod operationCompleteOccured,
  T_StatusOccuredMethod statusOccured,
  T_IntermediateOccuredMethod intermediateOccured);

/** @ingroup BNR_MAINTENANCE
 * Asynchronous command which prepares all modules in the BNR to be removed.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage:
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_FAILURE - No "parkable" module was found in the BNR.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during some module, due to 
 *    a component failure, or opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because bnr_Cancel() 
 *    command has been called.
 *
 * @see module_Park().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Park();

/** @ingroup BNR_CASH_OPERATIONS
 * This command activates the presentation of the cash. 
 * 
 * It can only be used following the dispense method.
 * 
 * A #XFS_S_CDR_CASH_AVAILABLE status event is issued to report that the bills 
 * are presented at the outlet, then a #XFS_S_CDR_CASH_TAKEN status event is issued to 
 * report that the user has removed the bills, and the command completes. 
 *
 * After #XFS_S_CDR_CASH_AVAILABLE status event, if no #XFS_S_CDR_CASH_TAKEN status 
 * event is received within a reasonable time period, the application should send a 
 * bnr_CancelWaitingCashTaken() to terminate the command, then send a bnr_Retract() 
 * to clear the bills from the outlet. 
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    successful bnr_Dispense() call was not done before calling bnr_Present(), etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd()
 *  - #XFS_E_NOT_SUPPORTED - The unit is a BNA6.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_CASH_AVAILABLE - Sent when the bills are at outlet, 
 *    presented to the user for removal. data field is a T_XfsCashOrder structure 
 *    containing the amount and bill list presented to the user.
 *  - #XFS_S_CDR_CASH_TAKEN - Sent when the bills have been taken by the user.
 *    data field is a T_XfsCashOrder structure containing the amount and bill list 
 *    taken by the user.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *    The data field is a T_XfsCashOrder structure containing the bills
 *    presented to the user.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because bnr_CancelWaitingCashTaken() 
 *    command has been called while the BNR was waiting for bills removal from the Bezel.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - During bills move, a cash unit has less bills
 *    than expected (based on its bill count) at the beginning of the operation.
 *  - #BNRXFS_E_CDR_USER_ERROR - A person has stopped the device operation, i.e. manupulating
 *    the outlet flap or inserting a bill at the inlet when it is not expected.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements encountered
 *    an error during bill move - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * @par
 * The field data is a pointer to a #T_XfsCashOrder structure with the following properties:
 *  - If result is #XFS_RC_SUCCESSFUL and all presented bills are of the same currency,
 *    then T_XfsCashOrder.currency holds the information of that currency; if result is
 *    not #XFS_RC_SUCCESSFUL and all bills which should have been presented, then
 *    T_XfsCashOrder.currency holds the information of that currency; in all other cases
 *    T_XfsCashOrder.currency is empty (meaning that exponent = 0, currencyCode
 *    consists of NULL-characters).
 *  - If not all presented bills are of the same currency or result is not
 *    #XFS_RC_SUCCESSFUL, T_XfsCashOrder.denomination.amount is 0; otherwise, it holds
 *    the sum of all presented bills.
 *  - If result is #XFS_RC_SUCCESSFUL or not all bills which should have been presented
 *    are of the same currency, T_XfsCashOrder.denomination.cashbox is 0; otherwise it
 *    holds the sum of all bills that should have been presented.
 *  - T_XfsCashOrder.denomination.size holds number of elements in the items-array.
 *  - T_XfsCashOrder.denomination.items holds the number of stored bills for each LCU involved.
 *
 * @see bnr_Dispense(), bnr_CancelWaitingCashTaken().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Present();


/** @ingroup BNR_CASH_UNITS
 * Gets the complete state of all physical and logical cash units in the BNR.
 *
 * Although defined as an asynchronous operation, it is a synchronous operation
 * in the BNR implementation, which means that the result is sent back in the method
 * response instead of sending it in an operationCompleteEvent method call.
 *
 * @note When calling this function, you must set cashUnit.logicalCashUnitList.maxSize
 *        and cashUnit.physicalCashUnitList.maxSize to their correct values. (The standard
 *        values are #NBMAXLCU and #NBMAXPCU, but they may vary in your case.)
 *
 * @param[in, out] cashUnit Pointer to an initialized #T_XfsCashUnit structure; will receive the cash
 *                          units currently defined in the BNR. Note that the fields
 *                          cashUnit.logicalCashUnitList.maxSize and cashUnit.physicalCashUnitList.maxSize
 *                          must be correctly initialized by the caller.
 * @return #BXR_NO_ERROR if successful or the error code. Specific error code usage :
 *  - #XFS_E_BUSY - The BNR is moving bill or about to move and cannot return an accurate CashUnit information.
 *
 * @see bnr_UpdateCashUnit(), bnr_ConfigureCashUnit().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_QueryCashUnit(T_XfsCashUnit * cashUnit);


/** @ingroup BNR_DENOMINATIONS
 * Gets a list of denomination in the BNR.
 *
 * @note When calling this function, you must set denominationList.maxSize to its correct
 *        value. (The standard value is #MAX_NR_OF_DENOMINATION, but this may vary in your case.)
 *
 * @param[in, out] denominationList Pointer to an initialized #T_DenominationList structure; will receive
 *                                  the denominations currently defined in the BNR. Note that the field
 *                                  denominationList.maxSize must be correctly initialized by the caller.
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_UpdateDenominations().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_QueryDenominations(T_DenominationList * denominationList);


/** @ingroup BNR_INIT
 * Reboots the BNR. This call puts the BNR in the same state than a power cycle (power off/on).
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Reboot();


/** @ingroup BNR_CASH_OPERATIONS
 * This command allows the application to clear the intermediate stacker(escrow). 
 *
 * Bills are either stored in the recyclers or move to the cashbox.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, the state of the 
 *    transaction does not allow this command, etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  .
 * @par
 * If result is #XFS_RC_SUCCESSFUL, data field is a pointer to a #T_XfsCashUnit
 * structure with the cashUnit changes.
 * 
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.3.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult, T_XfsCashUnit, bnr_Dispense(), bnr_Retract().
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Reject();


/** @ingroup BNR_INIT
 * This method is used to put the BNR into a defined operational state.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - Another asynchronous command is already running.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd() 
 *    before calling bnr_Reset().
 *  - #XFS_E_FAILURE - The call cannot be executed due to an internal failure.
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_DEVICE_STATUS_CHANGED - Sent if the device status has changed 
 *    during the operation. The data field contains the new device status code (XFS_S_CDR_DS_xxx).
 *  - #XFS_S_CDR_CASHUNIT_CHANGED - Notify that one or more cash units may have 
 *    changed during the operation.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully (note: with FW versions
 *    older than v1.6.0, the BNR is not necessarily operational). 
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill was detected in the transport system, or 
 *    opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because bnr_Cancel() command has been 
 *    called during operation.
 *  - #XFS_E_ILLEGAL - The Cash Modules Lock or a cover was open when bnr_Reset() 
 *    has been initiated.
 *  @deprecated #XFS_E_HARDWAREERROR - One of the electro-mechanical elements 
 *    is not functionning - no more used since FW v1.3.0, use #XFS_E_CDR_CASH_DEVICE_ERROR instead.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Reset();


/** @ingroup BNR_CASH_OPERATIONS
 * This command allows the application to force cash that has been presented to be retracted. 
 *
 * Retracted bills will be moved to the intermediate stacker area and accounted in the Bundler LCU. 
 * The application can then present bills to the user, using bnr_CashInRollback() or bnr_Present() 
 * (depending of the kind od the transaction) or clear the intermediate stacker area using the 
 * bnr_Reject() method.
 *
 * This method may only be called after bills have been presented at the outlet following a 
 * bnr_Dispense() (if autoPresent mode is active), bnr_CashInRollback() or bnr_Present() method call,
 * and before the bills have been taken by the user. 
 *
 * @note 
 * An asynchronous method must not be called before the preceding one is terminated (i.e. 
 * OperationComplete event has been received); typically before calling bnr_Retract(), the preceding
 * command must be terminated by calling bnr_CancelWaitingCashTaken().
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage :
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, the state of the 
 *    transaction does not allow this command, etc...
 *  .
 * Other error codes might be used in the future without notice. See 
 * #T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_CASH_TAKEN - Sent when a bill has been taken by the user.
 *  - #XFS_S_CDR_CASHUNIT_CHANGED - Sent when a bill has been accepted by the BNR and
 *    escrowed in cash unit.
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully. Also when
 *    bills has already benn taken by the user.
 *  - #XFS_E_ILLEGAL - No bills have been presented at the Outlet. 
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or a bill jam, or opening of the system. 
 *  - #BNRXFS_E_CDR_USER_ERROR - Bills are held back by the user.
 *  .
 * @par
 * if result is #XFS_RC_SUCCESSFUL, data field is a pointer to a #T_XfsCashUnit
 * structure with the cashUnit changes.
 * 
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.3.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult, T_XfsCashUnit, bnr_CancelWaitingCashTaken(), bnr_Reject().
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_Retract();


/** @ingroup BNR_REPORTS
 * Sends a request to save an HistoryReport to the BNR.
 *
 * @param[in] reportInfo  String containing a short description of the reason
 *                        for creating the report; the maximum size of the 
 *                        string is fixed by #SIZE_OF_CHANGE_INFO.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage:
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is in progress. The transaction must be ended with bnr_CashInEnd().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is running, a dispense 
 *    is active (after a bnr_Dispense() and before a bnr_Present() call), etc...
 *  - #XFS_E_CDR_TOO_MANY_BILLS - There are bills in the escrow.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * T_BnrXfsResult for the full error code description.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_GetHistoryReports(), T_HistoryReport.
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SaveHistoryReport(const char *reportInfo);


/** @ingroup BNR_CASH_OPERATIONS
 * Allows BNR to perform self tests in order to stay operational. 
 * 
 * This command should be called on a regular basis if the 
 * selfTestMode capability is set to #STM_SLAVE (#STM_AUTO being the default).
 * The self tests include:
 *  - static tests
 *  - automatic recovery movements to clear an error condition
 *  - repositionning of internal devices that lost idle status. Example: Bundler drum, Piston Stacking Mech.
 *  - Refloat of Recyclers from Loader.
 * All these operations may include movements.
 *
 * This command is cancelled by the bnr_Cancel() command and is terminated by a 
 * OperationComplete event. In this case, any movement in progress is terminated 
 * (ex: bill transfer from Loader to Recycler) and the BNR stops any other self test.
 *
 * @par Recommendation:
 * As soon as the BNR is not used in a transaction, bnr_SelfTest() should be 
 * called and not cancelled before the next transaction. 
 * 
 * @par
 * If this cannot be applied, bnr_SelfTest() should be called typically every 
 * 20 seconds (but at least once per minute) and can be cancelled one second after 
 * the call. This will insure a BNR self test can be done.
 *
 * @par Events:
 *  - OperationComplete
 * 
 * @par XFS Exceptions:
 * #XFS_E_ILLEGAL will be returned if:
 *  - the BNR is in exchange state
 *  - the BNR is in cash in state
 *  - the BNR is in dispense state
 *  - the property SelfTestMode is defined to #STM_AUTO
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_Cancel().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SelfTest();


/** @ingroup BNR_SYS_CONFIG
 * Sets the BNR capabilities.
 *
 * Those settings are persistent over power cycles; please refer to 
 * T_XfsCapabilities for more details about settable capabilities, and their 
 * default values.
 *
 * @param[in] capabilities T_XfsCapabilities structure with the new settings.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_GetCapabilities(), T_XfsCapabilities.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetCapabilities(T_XfsCapabilities *capabilities);


/** @ingroup BNR_SYS_CONFIG
 * Sets the BNR date and time.
 *
 * Those settings are not persistent over power cycles, and must be set on power up by bnr_SetDateTime().
 * If not set, the BNR time starts from 2001/01/01 00:00:00 on power up. 
 *
 * @param[out]  dateTime Date and time returned by the BNR.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_GetDateTime().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetDateTime(T_Time64 dateTime);


/** @ingroup BNR_SYS_CONFIG
 * Sets the default bill routing table. This table will be applied to all 
 * existing denominations and to the new one that may be added later.
 *
 * Those settings are persistent over power cycles; please refer to 
 * T_BillRoutingTable for more details about default routing table values.
 *
 * @param[in] routingTable New routing table settings.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see bnr_GetDefaultBillRouting().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetDefaultBillRouting(T_BillRoutingTable *routingTable);


/** @ingroup BNR_CASH_OPERATIONS
 * Indicates to the BNR the result of the external recognition 
 * (for instance for coupon recognition).
 *
 * @param[in] recognitionResult Result from the host recognition process.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.5.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetRecognitionResult(T_RecognitionResult recognitionResult);


/** @ingroup BNR_SYS_CONFIG
 * Activates a special configuration of the BNR.
 *
 * Those settings are persistent over power cycles; default special 
 * configuration value is "00000000-00".
 *
 * @param[in] specialConfiguration String identifying the special configuration 
 *                                 to activate ("00000000-00" to turn off 
 *                                 special configuration).
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage:
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer, or 
 *    parameter did not fit with a known configuration.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_TOO_MANY_BILLS - There are bills in the escrow.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * T_BnrXfsResult for the full error code description.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.5.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see bnr_GetSpecialConfiguration().
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetSpecialConfiguration(const char *specialConfiguration);


/** @ingroup BNR_SYS_CONFIG
 * Sets the system configuration status of the BNR.
 *
 * Those settings are persistent over power cycles; please refer to 
 * T_SystemConfiguration for more details about default values.
 *
 * @param[in] configuration Pointer to a T_SystemConfiguration structure which
 *                            contains the new system configuration of the BNR.
 *
 * @return If function call is successful, return is equal to #BXR_NO_ERROR. 
 * Otherwise, return is strictly negative and its absolute value contains the 
 * error code. Specific error code usage:
 *  - #BXR_API_NOT_OPENED - bnr_Open() was not previously called with success.
 *  - #BXR_API_INVALID_PARAMETER - The function was passed a NULL pointer.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_TOO_MANY_BILLS - There are bills in the escrow.
 *  - Other BXR_USB_* error code - USB communication error, refer to chapter 
 *    Error Recovery.
 *  .
 * Other error codes might be used in the future without notice. See 
 * T_BnrXfsResult for the full error code description.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_DEVICE_STATUS_CHANGED - When the state of the BNR is changed
 *    to IDLE.
 *
 * @see bnr_GetSystemConfiguration().
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_SetSystemConfiguration(T_SystemConfiguration *configuration);


/** @ingroup BNR_CASH_UNITS
 * Updates the BNR's cash unit. This function is used to change counts and thresholds of the BNR cashUnits.
 *
 * Those settings are persistent over power cycles.
 *
 * @param[in] transportCount        Number of bills in the transport system.
 * @param[in] logicalCashUnitList   T_LogicalCashUnitList structure
 * @param[in] physicalCashUnitList  T_PhysicalCashUnitList structure
 *
 * @return #BXR_NO_ERROR if successful, or the following error code :
 *  - #XFS_E_BUSY - The BNR is moving bill or about to move and cannot update CashUnit information,
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. a dispense transaction is active in the BNR, the Cash Modules Lock is opened, etc...
 *  - #XFS_E_PARAMETER_INVALID - At least one of the parameters is a NULL pointer, or there are inconsistencies between LCU list and PCU list,
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_UpdateCashUnit(),
 *  - #XFS_E_CDR_INVALID_CASH_UNIT - An invalid Cash Unit information was sent to the BNR typically , this error can happen when :
 *    - an invalid PCU name is passed,
 *    - a PCU/LCU is listed more than once in the PCU/LCU list.
 *  - #XFS_E_CDR_CASH_UNIT_ERROR - An unauthorized Cash Unit modification was requested; typically , this error can happen when :
 *    - a PCU bill count doesn't match with the sum of all LCU bill counts attached to this LCU,
 *    - a Recycler PCU/LCU count doesn't match with the counts inside the BNR,
 *    - a Recycler or Bundler PCU threshold mode is set to SensorMode instead of CountMode,
 *    - the PCU/LCU list is not coherent with the current BNR CashUnit configuration (allowed with bnr_ConfigureCashUnit()),
 *    - the Bundler PCU threshold levels don't match with the levels inside the BNR.
 *
 * @see bnr_QueryCashUnit(), bnr_ConfigureCashUnit().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_UpdateCashUnit(UINT32 transportCount, T_LogicalCashUnitList * logicalCashUnitList, T_PhysicalCashUnitList * physicalCashUnitList);


/** @ingroup BNR_DENOMINATIONS
 * This function is used to update the settings for a list of denominations.
 * For each xfsDenominationInfo element of the T_DenominationList, the application
 * can update its validation settings.
 *
 * Those settings are persistent over power cycles; please refer to 
 * T_XfsDenominationInfo for more details about settable properties, and their 
 * default values.
 *
 * @param[in] denominationList  This list of denominations will be a modified 
 *                              version of the one obtained from 
 *                              bnr_QueryDenominations() call.
 * 
 * @return #BXR_NO_ERROR if successful or the following error code :
 *  - #XFS_E_ILLEGAL - A dispense command is already active on the BNR.
 *  - #XFS_E_NOT_SUPPORTED - operation not supported by the BNR firmware version.
 *  - #XFS_E_PARAMETER_INVALID - Invalid array size. The array size is bigger than expected.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A cashIn command has been issued and is already active.
 *  - #XFS_E_FAILURE - a command is already running on the BNR or an internal error occured.
 *
 * @see bnr_QueryDenominations().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE bnr_UpdateDenominations(T_DenominationList* denominationList);


/** @ingroup MODULE_HISTORY
 * Gets the billCassette exchange history of a module.
 *
 * @param[in]   id        Id of the module to read.
 * @param[out]  history   Bill cassette exchange history for the module.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetBillCassetteExchangeHistory(T_ModuleId id, T_BillCassetteExchangeHistory *history);


/** @ingroup MODULE_CONFIG
 * Gets the cashType of the bills stored in a module (loader or recycler)
 *
 * @param[in]   id        Id of the module to read.
 * @param[out]  cashType  The cashType that is stored in the module.
 *
 * @return #BXR_NO_ERROR if successful or 
 * - #XFS_E_PARAMETER_INVALID - the module is not valid (only Loader or Recycleur are accepted).
 * - #XFS_E_FAILURE - the module did not response correctly to the command or has error.
 * - #XFS_E_NOHARDWARE - the module is missing.
 *
 * @see module_SetCashType().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetCashType(T_ModuleId id, T_XfsCashType* cashType);


/** @ingroup MODULE_HISTORY
 * Gets the failure history of a module.
 *
 * @param[in]   id        Id of the module to read.
 * @param[out]  history   Failure history of the module.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetFailureHistory(T_ModuleId id, T_ModuleFailureHistory *history);


/** @ingroup MODULE_CONFIG
 * Gets identification data of a module.
 *
 * @param[in]       id              Id of the module to read.
 * @param[out]      identification  Module identification.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see module_SetIdentification().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetIdentification(T_ModuleId id, T_ModuleIdentification * identification);

/** @ingroup MODULE_MAINTENANCE
 * Gets the maintenance information of a module.
 * Applicable for:
 * - Main Module,
 * - Bundler,
 * - Spine,
 * - Loader,
 * - Recycler.
 *
 * @param[in]     id                Id of the module to read.
 * @param[out]    maintenanceInfo   Module maintenance information.
 *
 * @return #BXR_NO_ERROR if successful or 
 * - #XFS_E_PARAMETER_INVALID - The module is not valid.
 * - #XFS_E_ILLEGAL - The state of the BNR does not allow this command (e.g. asynchronous command running, 
 *   cash module lock open, etc...).
 * - #XFS_E_NOT_SUPPORTED - The command is not supported by the current firmware.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see module_MaintenanceDone(), module_SetMaintenanceInterval(), T_MaintenanceInfo.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetMaintenanceInfo(T_ModuleId id, T_MaintenanceInfo *maintenanceInfo);


/** @ingroup MODULE_HISTORY
 * Gets the restart history of a module.
 *
 * @param[in]     id        Id of the module to read.
 * @param[out]    history   Module restart history.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetRestartHistory(T_ModuleId id, T_ModuleRestartHistory *history);


/** @ingroup MODULE_STATUS
 * Gets the status of a module.
 *
 * @param[in]       id            Id of the module to read.
 * @param[in, out]  moduleStatus  Status of the module and its sub element
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see 
 *  - T_BnrXfsResult for the full error code description.
 *  - bnr_GetStatus() and bnr_GetSystemStatus().
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetStatus(T_ModuleId id, T_ModuleStatus * moduleStatus);

/** @ingroup MODULE_HISTORY
 * Gets the use history of a module.
 *
 * @param[in]   id          Id of the module to read.
 * @param[out]  history     Use history of the module.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_GetUseHistory(T_ModuleId id, T_ModuleUseHistory *history);

/** @ingroup MODULE_MAINTENANCE
 * Indicates to the module that the maintenance operation was done 
 * (resets T_MaintenanceInfo.cycleSinceLastMaintenance counter and sets T_MaintenanceInfo.maintenanceStatus to #MS_OK).
 * Applicable for:
 * - Main Module,
 * - Bundler,
 * - Spine,
 * - Loader,
 * - Recycler.
 *
 * A call to this function also leads to the creation of a new HistoryReport in the BNR.
 *
 * @param[in]     id                Id of the module.
 *
 * @return #BXR_NO_ERROR if successful or  
 * - #XFS_E_PARAMETER_INVALID - The module is not valid.
 * - #XFS_E_BUSY - A transaction is in progress.
 * - #XFS_E_ILLEGAL - The state of the BNR does not allow this command (e.g. asynchronous command running, 
 *   cash module lock open, etc...).
 * - #XFS_E_NOHARDWARE - The module is missing.
 * - #XFS_E_FAILURE - The command un-succeeded due to an internal failure.
 * - #XFS_E_NOT_SUPPORTED - The command is not supported by the current firmware.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_MAINTENANCE_STATUS_CHANGED - if the command call modifies the T_MaintenanceInfo::maintenanceStatus.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see module_GetMaintenanceInfo(), module_SetMaintenanceInterval(), T_MaintenanceInfo, T_HistoryReport.
 * @see  T_StatusOccuredMethod.
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_MaintenanceDone(T_ModuleId id);

/** @ingroup MODULE_MAINTENANCE
 * Asynchronous command which prepares one module in the BNR to be removed.
 *
 * If this function is called for a module which does not need to do any
 * movements, #XFS_E_PARAMETER_INVALID will be returned.
 *
 * @param[in]   id    Id of the module to park.
 *
 * @return If function call is successful, return is strictly positive and 
 * represents the identificationId. Otherwise, return is strictly negative and its 
 * absolute value contains the error code. Specific error code usage:
 *  - #XFS_E_PARAMETER_INVALID - Module id is invalid.
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress. The transaction must be ended with bnr_CashInEnd().
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *
 * @par OperationComplete event:
 *  The field result can take the following values :
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_CDR_CASH_DEVICE_ERROR - A failure occured during operation, due to 
 *    a component failure, or opening of the system. 
 *  - #XFS_E_CANCELLED - The command has been terminated because bnr_Cancel() command
 *    has been called during operation.
 *
 * @see bnr_Pak().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_Park(T_ModuleId id);


/** @ingroup MODULE_CONFIG
 * Erases the main application of the specified module. 
 *
 * In case of the MainModule, this command erases also the billset and the 
 * Logical Cash Units.
 *
 * @param[in]       id              Id of the module.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 *
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_RevertToBoot(T_ModuleId id);


/** @ingroup MODULE_CONFIG
 * Sets the cashType of the bills stored in the module (Loader only).
 * Access to a BNR's module cannot be done while the BNR is moving bills, the cash module lock is open or an 
 * asynchronous command is in progress.
 *
 * Those settings are persistent over power cycles.
 *
 * @param[in]   id        Id of the loader to access.
 * @param[in]   cashType  CashType to set.
 *
 * @return #BXR_NO_ERROR if successful or 
 * - #XFS_E_PARAMETER_INVALID - the module is not valid (only Loader is accepted) or the cashType is incorrect.
 * - #XFS_E_BUSY - the module cannot be access while the BNR is moving bills.
 * - #XFS_E_ILLEGAL - the state of the BNR does not allow this command (e.g. asynchronous command running, 
 *   cash module lock open, etc...).
 * - #XFS_E_NOHARDWARE - the module is missing.
 * - #XFS_E_FAILURE - the module did not applied the cashType.
 *
 * @see module_GetCashType().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_SetCashType(T_ModuleId id, T_XfsCashType* cashType);

/** @ingroup MODULE_CONFIG
 * Sets the user identification of a module.
 * Access to a BNR's module cannot be done while the BNR is moving bills, the cash module lock is open or an 
 * asynchronous command is in progress.
 *
 * Those settings are persistent over power cycles.
 *
 * @param[in]   id                  Id of the module to access.
 * @param[in]   userIdentification  The string to be written in the BNR; the maximum length is 255 printable ASCII characters.
 *
 * @return #BXR_NO_ERROR if successful or 
 * - #XFS_E_PARAMETER_INVALID - the module id is not valid.
 * - #XFS_E_BUSY - the module cannot be access while the BNR is moving bills.
 * - #XFS_E_ILLEGAL - the state of the BNR does not allow this command (e.g. asynchronous command running, 
 *   cash module lock open, etc...).
 * - #XFS_E_NOHARDWARE - the module is missing.
 * 
 * @par NOTE: If the programmer sends characters in userData that are 
 * - Non-printable ASCII characters OR
 * - The less-than, greater-than or hashtag characters
 * Those characters will be replaced by an '!' when written to the BNR. 
 * @see module_GetIdentification().
 * @see T_BnrXfsResult for the full error code description.
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_SetIdentification(T_ModuleId id, char * userIdentification);

/** @ingroup MODULE_MAINTENANCE
 * Sets the maintenance interval for a module.
 * Applicable for:
 * - Main Module,
 * - Bundler,
 * - Spine,
 * - Loader,
 * - Recycler.
 *
 * Those settings are persistent over power cycles; please refer to 
 * T_MaintenanceInfo for more details about default maintenance interval values.
 *
 * @param[in]   id         Id of the module.
 * @param[in]   interval   Number of cycles between two maintenance operations.
 *
 * @return #BXR_NO_ERROR if successful or  
 * - #XFS_E_PARAMETER_INVALID - The module is not valid.
 * - #XFS_E_CDR_CASHIN_ACTIVE - A cashIn command has been issued and is already active.
 * - #XFS_E_ILLEGAL - The state of the BNR does not allow this command (e.g. asynchronous command running, 
 *   cash module lock open, etc...).
 * - #XFS_E_NOHARDWARE - The module is missing.
 * - #XFS_E_FAILURE - The command un-succeeded due to an internal failure.
 * - #XFS_E_NOT_SUPPORTED - The command is not supported by the current firmware.
 *
 * @par StatusEvent event:
 *  The field status can take the following values :
 *  - #XFS_S_CDR_MAINTENANCE_STATUS_CHANGED - if the command call modifies the T_MaintenanceInfo::maintenanceStatus.
 * 
 * @par Firmware Compatibility
 * This function requires a BNR FW v1.7.0 or newer. With older FW versions, 
 * the return will be #XFS_E_NOT_SUPPORTED.
 *
 * @see module_GetMaintenanceInfo(),  module_MaintenanceDone(), T_MaintenanceInfo.
 * @see T_StatusOccuredMethod.
 * @see T_BnrXfsResult for the full error code description.
 * 
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_SetMaintenanceInterval(T_ModuleId id, T_LongAbsoluteCount interval);

/** @ingroup MODULE_CONFIG
 * Asynchronous command to update a firmware component of the given module.
 *
 * @param[in] id            Id of the module.
 * @param[in] pathFileName  Path of the file to upload.
 *
 * @return The identificationId if successful or the following error code :
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_AddDenomination().
 *  - #XFS_E_PARAMETER_INVALID - The module ID is wrong, not corresponding to the file or the file is not version 
 *    compatible with the hardware.
 *  - #XFS_E_NOT_SUPPORTED - The update command for the given module is not supported (i.e. Bundler).
 *  - #XFS_E_FIRMWARE - The CRC on the data is wrong, the data size is too big or the data can not be written with 
 *    success in the flash memory.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The intermediate stacker is not empty (e.g. if, during a
 *    refloat cycle, a jam occurred or the cash modules lock has been opened) and
 *    thus no firmware can be updated at the moment.
 *  - #XFS_E_NOHARDWARE - The specified module is not present in the BNR.
 *
 * @par OperationComplete event:
 *  The field data is always NULL, result field can take the following values:
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_FIRMWARE - An error occurred during the firmware update.
 *  .
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_UpdateFirmware(T_ModuleId id, char * pathFileName);


/** @ingroup MODULE_CONFIG
 * Asynchronous command to update a firmware component of the given module.
 *
 * @param[in] id        Id of the module.
 * @param[in] buf       Buffer containing the information about the firmware.
 * @param[in] size      Size of the buffer.
 *
 * @return The identificationId if successful or the following error code :
 *  - #XFS_E_ILLEGAL - The BNR is in a state where it cannot execute the operation,
 *    e.g. another asynchronous command is already running, a 
 *    dispense is active (after a bnr_Dispense() and before a bnr_Present() 
 *    call), etc...
 *  - #XFS_E_CDR_CASHIN_ACTIVE - A transaction started with bnr_CashInStart() 
 *    is already in progress and must be ended with bnr_CashInEnd()
 *    before calling bnr_AddDenomination().
 *  - #XFS_E_PARAMETER_INVALID - The module ID is wrong, not corresponding to the file or the file is not version 
 *    compatible with the hardware.
 *  - #XFS_E_NOT_SUPPORTED - The update command for the given module is not supported (i.e. Bundler).
 *  - #XFS_E_FIRMWARE - The CRC on the data is wrong, the data size is too big or the data can not be written with 
 *    success in the flash memory.
 *  - #XFS_E_CDR_TOO_MANY_BILLS - The intermediate stacker is not empty (e.g. if, during a
 *    refloat cycle, a jam occurred or the cash modules lock has been opened) and
 *    thus no firmware can be updated at the moment.
 *  - #XFS_E_NOHARDWARE - The specified module is not present in the BNR.
 *
 * @par OperationComplete event:
 *  The field data is always NULL, result field can take the following values:
 *  - #XFS_RC_SUCCESSFUL - The command has been executed successfully.
 *  - #XFS_E_FIRMWARE - An error occurred during the firmware update.
 *  .
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE module_UpdateFirmwareBuffer(T_ModuleId id, char * buf, UINT32 size);

/** @ingroup USB_INTERFACE
 * This function is used to get the serial numbers of all BNR connected.
 *
 * This function may be called before any bnr_Open() or bnr_OpenWithSerialNr(). It gives a way to select a specific 
 * BNR using then the bnr_OpenWithSerialNr() function. If a Bnr is already open by an other application, 
 * it will not be present in the serial number list.
 *
 * @param[in,out] list List to receive the serial numbers. The field maxSize must be initialised before the call.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE usb_GetBnrSerialNrList(T_BnrSerialNrList *list);

/** @ingroup USB_INTERFACE
 * This function is used to kill and reload device driver. All the pending transactions on the bus should be aborted. 
 * Then the BNR is in closed state
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE usb_KillAndReload();

/** @ingroup USB_INTERFACE
 * This function is used to reset the Device port. All the pending transactions on the bus should be aborted. The request 
 * causes all of the status and configuration values associated with endpoints in the affected interfaces, to be set 
 * to their default values.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE usb_ResetDevice();

/** @ingroup USB_INTERFACE
 * Resets the endpoint and its corresponding pipe.
 *
 * This function resets a stalled pipe. It cancels all pending I/O on the pipe and sends CLEAR_FEATURE with 
 * USB_FEATURE_ENDPOINT_STALL selector for the specified endpoint. When receiving USB_STATUS_STALL_PID error code on 
 * bulk or interrupt transfers, the Client application should try to reset the pipe. If this does not help, the Client 
 * should try to reset the Device. 
 *
 * The USB communication for the BNR uses 4 endpoints other than the control 
 * endpoint(0) :
 *  - Endpoint 1 defines has input to receive the method response from the BNR.
 *  - Endpoint 2 defines has output to send the method call to the BNR.
 *  - Endpoint 3 defines has interrupt to receive the method call from the BNR.
 *  - Endpoint 4 defines has output to send the method response to the BNR.
 *
 * @param[in] endpointNr Number of the endpoint to reset.
 *
 * @return #BXR_NO_ERROR if successful or the error code.
 * @see T_BnrXfsResult
 */
BNRDLL_API T_BnrXfsResult CALLTYPE usb_ResetEndpoint(int endpointNr);

/** @ingroup USB_INTERFACE
 * Registers a specified window handle has listener of the USB device change 
 * notifications.
 *
 * The notification becomes as the WM_DEVICECHANGE window message, where the 
 * lParam parameter points to the buffer with DEV_BROADCAST_DEVICEINTERFACE 
 * structure, from which the Client application can extract required fields 
 * about notification.
 *
 * @param[in] listener specifies window handle for which notification enables.
 * @return #BXR_NO_ERROR if successful or the error code.
 * @see T_BnrXfsResult
 */
#ifdef WIN32
BNRDLL_API T_BnrXfsResult CALLTYPE usb_SetDeviceChangeListener(HWND listener);
#endif


#ifdef __cplusplus    
  }
#endif

#endif //_BNR_CTL_W32_H_
