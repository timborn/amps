////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012-2017 60East Technologies Inc., All Rights Reserved.
//
// This computer software is owned by 60East Technologies Inc. and is
// protected by U.S. copyright laws and other laws and by international
// treaties.  This computer software is furnished by 60East Technologies
// Inc. pursuant to a written license agreement and may be used, copied,
// transmitted, and stored only in accordance with the terms of such
// license agreement and with the inclusion of the above copyright notice.
// This computer software or any other copies thereof may not be provided
// or otherwise made available to any other person.
//
// U.S. Government Restricted Rights.  This computer software: (a) was
// developed at private expense and is in all respects the proprietary
// information of 60East Technologies Inc.; (b) was not developed with
// government funds; (c) is a trade secret of 60East Technologies Inc.
// for all purposes of the Freedom of Information Act; and (d) is a
// commercial item and thus, pursuant to Section 12.212 of the Federal
// Acquisition Regulations (FAR) and DFAR Supplement Section 227.7202,
// Government's use, duplication or disclosure of the computer software
// is subject to the restrictions set forth by 60East Technologies Inc..
//
////////////////////////////////////////////////////////////////////////////

#ifndef _AMPS_AMPS_API_H
#define _AMPS_AMPS_API_H

#include <amps_common.h>
#include <time.h>
#include <stdint.h>

/** \file amps_external_api.h
 *  This file contains functions for the use of AMPS plugin modules.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Struct that represents strings for the AMPS API.
 * This type consists of a pointer and a length. This struct
 * does _not_ own the string pointed to, and the lifetime of that string
 * must be managed.
 */

struct amps_cstring
{
 const char* data;
 size_t      length;
};

typedef void* amps_expression_value;
typedef void* amps_expression_value_array;


/**
 *  Returns a true value (1) if logging is enabled for logging module messages
 *  of the provided level, false (0) if logging is not enabled or the level
 *  passed is not a valid amps module log level. Notice that this function
 *  does not track ExcludedErrors from individual transports, and may
 *  overreport logging in some cases. A module can cache this value
 *  during calls to the module, but should not cache the value between calls.
 *  AMPS uses an equivalent function as part of the internal logging process,
 *  so this function is provided as an optimization for situations where
 *  a module will perform an expensive operation (such as serializing a large
 *  object graph or formatting a complex string) before logging. For many cases,
 *  such as logging constant strings, it is more efficient to call the logger
 *  and rely on the internal check rather than checking this value first.
 */

int amps_will_log(int module_log_level);

/**
 * AMPS logger function: takes a module_log_level and a NULL-terminated
 * string to log.
 */

typedef void  (*amps_module_logger)(int level,const char* message);

/**
 * Write formatted output to the AMPS log. As an optimization, simply returns
 * AMPS_SUCCESS if AMPS will not log the requested message without formatting
 * the string provided. Use with care: mismatches between the format string and
 * arguments will produce undefined behavior, up to and including segmentation
 * faults.
 * \returns AMPS_SUCCESS if the message does not need to be logged or if the
 *  message was successfully formatted and logged, AMPS_FAILURE if there is an
 *  error formatting the string.
 */
int amps_log_sprintf(amps_module_logger logger,
                     int module_log_level,
                     const char* format_, ...);

/**
 * Returns the description of the last error, if any, otherwise NULL.
 * \returns Description of last error.
 */
const char* amps_get_last_error(void);

/**
 * Frees the memory for the last error description, if any. This doesn't
 * need to be called, since AMPS will cleanup any errors not freed by users.
 */
void amps_free_last_error(void);

/**
 * Typedefs for amps_get_message_type_names
 */
typedef const char*             amps_message_type_name;
typedef amps_message_type_name* amps_message_type_names;

/**
 * Returns a null-terminated list of registered messsage types
 * The messageTypeNameList_ should be free'd by calling amps_free_message_type_names.
 * \param messageTypeNameList_ the pointer to the list to fill with message type names.
 *                          After processing this list, call
 *                          ::amps_free_message_type_names to free the list.
 * \returns AMPS_SUCCESS on success.
 * \sa ::amps_free_message_type_names
 */
int amps_get_message_type_names(amps_message_type_names *messageTypeNameList_);

/**
 * Frees a list returned from ::amps_get_message_type_names.
 * \param messageTypeNameList_  the pointer to the list to free
 * \returns AMPS_SUCCESS on success.
 */
int amps_free_message_type_names(amps_message_type_names messageTypeNameList_);

/**
 * Takes a message type string and length and returns AMPS_SUCCESS (0)
 * if the message type is valid.  Otherwise, it sets amps_get_last_error() to
 *  either "Invalid message type." or "Unknown exception."
 * \param messageType_     the message type string to check
 * \param messageTypeLen_  the length of the message type string
 * \returns AMPS_SUCCESS on success.
 */
int amps_validate_message_type(const char *messageType_,unsigned int messageTypeLen_);

/**
 * Takes a filter string and length and returns AMPS_SUCCESS (0)
 * if the format is acceptable.  Otherwise, it sets amps_get_last_error() to
 * either "Invalid syntax." or "Unknown exception." Entitlement modules can use
 * this function to test an entitlement filter before returning the filter
 * to AMPS.
 * \param filter_   the filter to check
 * \param length_   the length of the filter
 * \returns AMPS_SUCCESS on success.
 */
int amps_validate_filter(const char* filter_, unsigned int length_);

/**
 * Takes an orderBy string and length and returns AMPS_SUCCESS (0)
 * if the format is acceptable.
 * \param orderBy_   the orderBy string to check
 * \param length_   the length of the orderBy string
 * \returns AMPS_SUCCESS on success.
 */
int amps_validate_order_by(const char* orderBy_, unsigned int length_);

/**
 * Gets the running AMPS version.
 * \returns a null-terminated string containing the AMPS version number.
 * \sa ::amps_compare_version, ::amps_version_is_exactly
 */
char* amps_version(void);

/**
 * Compares the user-supplied version_ string against the version of the
 * running AMPS instance.
 * \param version_    a version against which to check eg: "3.8.0.1"  If the
 *                    supplied version cannot be parsed, version "0.0.0.0" is
 *                    assumed.
 * \returns 0 if the versions match, 1 if the running AMPS version is greater
 *            than version_, -1 if the running AMPS version is lower than
 *            version_.
 * \sa ::amps_version_is_exactly
 */
int amps_compare_version(const char* version_);

/**
 * Check whether the supplied version_ is lower than the running AMPS version.
 * \param version_     a version against which to check eg: "3.8.0.1"  If the
 *                     supplied version cannot be parsed, version "0.0.0.0" is
 *                     assumed.
 * \returns 1 if the AMPS version comes before the supplied version,
 *                     otherwise, returns 0.
 * \sa ::amps_compare_version, ::amps_version_is_after,
 *     ::amps_version_is_exactly
 */
inline int amps_version_is_before(const char* version_)
{
  return 0 > amps_compare_version(version_);
}

/**
 * An exact comparison check between the supplied version_ and the version
 * of the running AMPS instance.
 * \param version_     a version against which to check eg: "3.8.0.1"  If the
 *             supplied version cannot be parsed, version "0.0.0.0" is
 *             assumed.
 * \returns 1 if the running AMPS version matches version_,
 *      otherwise, returns 0.
 * \sa ::amps_compare_version, ::amps_version_is_before,
 *     ::amps_version_is_after
 */
inline int amps_version_is_exactly(const char* version_)
{
  return 0 == amps_compare_version(version_);
}

/**
 * Check whether the supplied version_ is after the running AMPS version.
 * \param version_     a version against which to check eg: "3.8.0.1"  If the
 *             supplied version cannot be parsed, version "0.0.0.0" is
 *             assumed.
 * \returns 1 if the AMPS version comes after the supplied version,
 *      otherwise, returns 0.
 * \sa ::amps_compare_version, ::amps_version_is_before,
 *     ::amps_version_is_exactly
 */
inline int amps_version_is_after(const char* version_)
{
  return 0 < amps_compare_version(version_);
}

/**
 * Converts a UTC ISO-8601 datetime into an integer equivalent.
 * \note The time provided represents an internal timestamp used by
 *       AMPS with microsecond precision, and is not a UNIX timestamp value.
 * \param datetime_    the datetime string data. eg: "20150101T031500"
 * \param length_      the length of the datetime string.
 * \param microseconds pointer to where to store the microseconds equivalent.
 * \returns AMPS_SUCCESS on success.
 *
 */
int amps_parse_datetime(const char*    datetime_,
                        unsigned       length_,
                        unsigned long* microseconds_);

/**
 * Converts a string containing an AMPS interval into an integer equivalent,
 * representing the number of nanoseconds that the interval signifies.
 * \param interval_    the interval string data. eg: "1h", "15m", etc.
 * \param length_      the length of the interval string.
 * \param nanoseconds_ pointer to where to store the nanoseconds equivalent.
 * \returns AMPS_SUCCESS on success.
 */
int amps_parse_interval(const char*    interval_,
                        unsigned       length_,
                        unsigned long* nanoseconds_);


/**
 * Converts a string containing a number of bytes into an integer equivalent.
 * \param bytestr_     the string containing bytes. eg: "1KB", "15GB", etc.
 * \param length_      the length of the bytestr string.
 * \param bytes_       pointer to where to store the bytes equivalent.
 * \returns AMPS_SUCCESS on success.
 */
int amps_parse_bytes(const char*    bytestr_,
                     unsigned       length_,
                     unsigned long* bytes_);

/**
 * Converts a string containing a boolean value into an integer equivalent.
 * \param bytestr_     the string containing bytes. eg: "True", "Yes", "Enabled", "False", "No", "Disabled", etc.
 * \param length_      the length of the bytestr string.
 * \param value_       pointer to where to store the boolean equivalent (0 for false).
 * \returns AMPS_SUCCESS on success.
 */
int amps_parse_boolean(const char*    bytestr_,
                       unsigned       length_,
                       unsigned long* value_);

/**
 * Extracts a value from the configuration file at the given XPath.
 * \param pXpath_         the string containing the xpath.
 * \param xpathLength_    the length of the pXpath_ string.
 * \param pOutputBuffer_  buffer in which to store the output string.
 * \param pOutputLength_  pointer to the size of pOutputBuffer_.
 * \returns AMPS_SUCCESS on success.
 * If AMPS_FAILURE is returned, call amps_get_last_error() for more information.
 * If AMPS_FAILURE is returned and pOutputLength_ is a non-zero value,
 * then pOutputBuffer_ needs to be at resized to at least that length
 * before calling amps_extract_config_value.
 * If AMPS_SUCCESS is returned, pOutputLength_ is overwritten with the
 * number of characters written to pOutputBuffer_.
 */
int amps_extract_config_value(const char* pXpath_,
                              size_t xpathLength_,
                              char* pOutputBuffer_,
                              size_t* pOutputLength_);

/**
 * Compresses journals that are older than the supplied age.
 * \param pAge_   the string containing the age. eg: "1d"
 * \returns AMPS_SUCCESS on success.
 */
int amps_compress_journals(const char* pAge_);

/**
 * Archives journals that are older than the supplied age.
 * \param pAge_   the string containing the age. eg: "1d"
 * \returns AMPS_SUCCESS on success.
 */
int amps_archive_journals(const char* pAge_);

/**
 * Removes journals that are older than the supplied age.
 * \param pAge_   the string containing the age. eg: "1d"
 * \returns AMPS_SUCCESS on success.
 */
int amps_remove_journals(const char* pAge_);

/**
 * Disables authentication.
 * \returns AMPS_SUCCESS on success.
 */
int amps_disable_authentication(void);

/**
 * Enables authentication.
 * \returns AMPS_SUCCESS on success.
 */
int amps_enable_authentication(void);

/**
 * Resets authentication on the provided transport.
 * \param pTransport_   the string containing name of the transport, or NULL
 *                      (0) to reset all transports for the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_reset_authentication(const char* pTransport_);

/**
 *  Returns the name of the current transport when called during
 *  amps_authentication_create_context or amps_entitlement_create_context.
 *  When called outside of one of those functions, returns NULL.
 *  \returns The name of the transport that is initializing the context, or
 *  NULL if called outside of context initialization. The pointer returned
 *  is not guaranteed to be valid outside the scope of amps_*_create_context.
 */
const char* amps_get_transport_name(void);

/**
 * Typedefs for amps_get_transport_name
 */
typedef const char* amps_transport_name;

/**
 * Typedef for a list of amps_transport_name values.
 */
typedef amps_transport_name* amps_transport_names;

/**
 * Returns a null-terminated list of all the transport names for this instance.
 * The transport_nameList_ must be freed by calling amps_free_transport_names.
 * \param transport_nameList_ the pointer to the list for the transport_names.
 *                            After processing this list, you must call
 *                            ::amps_free_transport_names to free the list.
 * \returns AMPS_SUCCESS on success.
 */
int amps_get_transport_names(amps_transport_names* transportNameList_);

/**
 * Frees a list returned from ::amps_get_transport_names.
 * \param transport_nameList_   the pointer to the list to free
 * \returns AMPS_SUCCESS on success.
 */
int amps_free_transport_names(amps_transport_names transportNameList_);

/**
 *  Returns the name of the current module when called during amps_module_init.
 *  This function will return NULL outside of calls to amps_module_init.
 *  \returns The name of the module being intialized, or NULL if called
 *  outside of amps_module_init. The pointer returned is not guaranteed to be
 *  valid outside the scope of amps_module_init.
 */
const char* amps_get_module_name(void);

/**
 * Returns the client name of the current client.  This is currently only
 * available/set for Authentication module amps_authenticate functions.
 * This function will return NULL outside of calls to amps_authenticate.
 * \returns The name of the client being authenticated, or NULL if called
 * outside of amps_authenticate. The pointer returned is not guaranteed to be
 * valid outside the scope of amps_authenticate.
 */
const char* amps_get_client_name(void);

/**
 * Returns the connection name of the current connection.  The connection name
 * is currently only available/set for Authentication module amps_authenticate
 * functions. This function will return NULL outside of calls to
 * amps_authenticate.
 * \returns The name of the connection being authenticated, or NULL if called
 * outside of amps_authenticate. The pointer returned is not guaranteed to be
 * valid outside the scope of amps_authenticate.
 */
const char* amps_get_connection_name(void);


/**
 * Returns the correlation ID submitted with the logon request for the
 * client. This is currently only available/set for Authentication module
 * amps_authenticate functions. This function will return NULL outside
 * of calls to amps_authenticate.
 *
 */

const char* amps_get_client_correlation_id(void);

/**
 * Disables entitlements. After calling this method, AMPS does not call
 * entitlement modules for the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_disable_entitlement(void);

/**
 * Enables entitlements. After calling this method, AMPS will call any
 * configured entitlement modules for the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_enable_entitlement(void);

/**
 * Resets entitlements on the provided transport.
 * \param pTransport_   the string containing name of the transport, or NULL
 *                      (0) to reset all transports in the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_reset_entitlement(const char* pTransport_);

/**
 * Typedefs for amps_get_authids
 */
typedef const char* amps_authid;
typedef amps_authid* amps_authids;

/**
 * Resets entitlements on the provided transport for the provided amps_authid.
 * \param pTransportName_   the string containing name of the transport, or
 *                          NULL (0) to reset all transports in the instance.
 * \param pClientAuthId_   the string containing the authid to reset.
 * \returns AMPS_SUCCESS on success.
 */
int amps_reset_entitlement_for_authid(const char* pTransportName_,
                                      amps_authid pClientAuthId_);

/**
 * Returns a null-terminated list of authids currently associated with
 * the given transport or all transports if name is NULL.
 * The authidList_ should be free'd by calling amps_free_authids.
 * \param pTransportName_   the string containing name of the transport, or
 *                          NULL (0) to return authids for all transports
 *                          in the instance.
 * \param authidList_       the pointer to the list to fill with authids.
 *                          After processing this list, you must call
 *                          ::amps_free_authids to free the list.
 * \returns AMPS_SUCCESS on success.
 * \note It is possible that an authentication id shares a name
 *       with another authentication id on a different transport. Users
 *       with multiple transports and authentication schemes rarely want
 *       to pass in a null pointer for the transport name because of
 *       this risk of collision.
 */
int amps_get_authids(const char* pTransportName_, amps_authids* authidList_);

/**
 * Frees a list returned from ::amps_get_authids.
 * \param authidList_   the pointer to the list to free
 * \returns AMPS_SUCCESS on success.
 */
int amps_free_authids(amps_authids authidList_);

/**
 * Disable the provided transport or all transports if name is NULL.
 * \param pTransportName_   the string containing name of the transport, or
 *                          NULL (0) to disable all transports in the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_disable_transport(const char* pTransportName_);

/**
 * Enable the provided transport or all transports if name is NULL.
 * \param pTransportName_   the string containing name of the transport, or
 *                          NULL (0) to enable all transports in the instance.
 * \returns AMPS_SUCCESS on success.
 */
int amps_enable_transport(const char* pTransportName_);

/**
 * Produce an AMPS minidump.
 * \returns AMPS_SUCCESS on success.
 */
int amps_minidump(void);

/**
 * Rotate logs for logging targets that support rotation.
 * \returns AMPS_SUCCESS on success.
 */
int amps_rotate_logs(void);

/**
 * Downgrade sync replication destinations that are behind by more than
 * the supplied age.
 * \param pAge_   the age, in AMPS interval format (for example, "1s")
 * \returns AMPS_SUCCESS on success.
 */
int amps_downgrade_replication_age(const char* pAge_);

/**
 * Upgrade sync replication destinations that were previously downgraded
 * and are now behind by less than the supplied age.
 * \param pAge_   the age, in AMPS interval format (for example, "5s")
 * \returns AMPS_SUCCESS on success.
 */
int amps_upgrade_replication_age(const char* pAge_);


/**
 * Delete all statistics older than the current time minus the provided age.
 * \param pAge_   the age, in AMPS interval format (for example, "1d"), as
 *                a NULL-terminated string.
 * \returns AMPS_SUCCESS on success.
 */
int amps_truncate_statistics(const char* pAge_);

/**
 * Free unused file space in the AMPS statistics store.
 * \returns AMPS_SUCCESS on success.
 */
int amps_vacuum_statistics(void);

/**
 * Gracefully shuts AMPS down.
 */
void amps_shutdown(void);

/**
 * Returns the startup progress of AMPS as an integer [0,100]. For example,
 *   if this function returns 33, then AMPS is 33% started.
 * \returns the statup progress as an integer [0,100].
 */
unsigned amps_startup_progress(void);

/**
 * Returns the shutdown progress of AMPS as an integer [0,100]. For example,
 *   if this function returns 33, then AMPS is 33% done shutting down.
 * \returns the shutdown progress as an integer [0,100].
 */
unsigned amps_shutdown_progress(void);


/**
 * Typedef for a pointer to a function to be
 * called at the end of startup.
 */
typedef int (*amps_startup_callback)(void*);


/**
 * Registers a function to be called at the end of startup. Any module initialization
 * that relies on other components should be deferred to a startup function.
 * \param The function to call. Functions will be called in order of registration.
 * \returns AMPS_SUCCESS on success.
 */
int amps_add_startup_function(amps_startup_callback pStartCb_, void* userData_);

/**
 * Typedef for a pointer to a function to be
 * called at the start of shutdown.
 */
typedef int (*amps_shutdown_callback)(void*);

/**
 * Registers a function to be called at the start of shutdown.
 * \param The function to call.
 * \returns AMPS_SUCCESS on success.
 */
int amps_add_shutdown_function(amps_shutdown_callback pShutdownCb_, void* userData_);


/**
 * Typedef for a pointer to a function to be called
 * when AMPS produces a minidump.
 */

typedef int (*amps_minidump_callback)(void*,const char*);

/**
 * Registers a function to be called on each minidump.
 * \param The function to call.
 * \returns AMPS_SUCCESS on success.
 */
int amps_add_minidump_function(amps_minidump_callback pMinidumpCb_,void* userData_);

/**
 * Tests whether the AMPS instance has completed startup.
 * \returns 1 when AMPS is fully started, 0 otherwise
 */

inline int amps_is_running(void) { return amps_startup_progress() == 100; }


/**
 * Tests whether the AMPS instance has been stopped and is in the shutdown
 * process.
 * \returns 1 when AMPS is in the process of shutting down, 0 otherwise
 */

inline int amps_is_stopped(void) { return amps_shutdown_progress() != 0; }

/**
 * Create a new symbol scope for Action On symbol propagation.
 */
int amps_symbol_scope_push(void);
/**
 * Destroy current symbol scope for Action On symbol propagation.
 */
int amps_symbol_scope_pop(void);
/**
 * Set callback function for key-value retrieval. (TODO: describe why you'd want this variant.)
 */
int amps_symbol_scope_use(void* userData_, void (*retrieve)(void* userData_, const char* key_, size_t keyLength_, const char** value_, size_t* valueLength_));
/**
 * Set symbol within current scope. (TODO: describe why you'd want this variant.)
 */
int amps_symbol_scope_put(const char* key_, size_t keyLength_, const char* value_, size_t valueLength_);

/**
 * Retrieve symbol within current scope.
 */
int amps_symbol_scope_get(const char* key_, size_t keyLength, const char** value_, size_t* valueLength_);




/**
 * Perform variable substitution on a string provided from an action module.
 * This function uses the current symbol scope to find the values of
 * variables to substitute. Any variable name that is not in the current
 * scope is substituted with a zero-length string.
 * \param input_     pointer to the input string
 * \param inputLen_  length of the input string
 * \param output_    pointer to the output string
 * \param outputLen_ pointer to the length of the output string.
 *                   When the function is called, the length must
 *                   be the capacity of the string. When the function
 *                   returns, the variable pointed to will be set to
 *                   the actual number of characters required to expand
 * \returns          AMPS_SUCCESS when the output string was large enough
 *                   to contain the input string expansion. When AMPS_FAILURE
 *                   is returned, the outputLen_ will be set to the
 *                   capacity required to be able to expand the string.
 *                   Resize the buffer to expand into and call the function
 *                   again with the new size.
 */
int amps_expand_action_string(const char* input_,
                              size_t inputLen_,
                              char* output_,
                              size_t* outputLen_);
/**
 * Validates an action string has proper form (balanced braces).
 * \param input_     pointer to the input string
 * \param inputLen_  length of the input string
 * \returns          AMPS_SUCCESS when the action string has proper form,
 *                   otherwise AMPS_FAILURE.
 */
int amps_validate_action_string(const char* input_, size_t inputLen_);


/////////////////////////////////////////////////////////////////////////////
//
// amps_expression_value functions
//
//   These functions are provided to process arguments and set return values
//   for user defined scalar and aggregate functions.
//
/////////////////////////////////////////////////////////////////////////////


/** Returns true if the provided value is of NULL type.
 * \param v the value to test
 */
int amps_expression_value_is_null(amps_expression_value v);
/** Returns true if the provided value is NAN.
 * \param v the value to test
 */
int amps_expression_value_is_nan(amps_expression_value v);
/** Returns true if the provided value is of reference type.
 * \param v the value to test
 */
int amps_expression_value_is_reference(amps_expression_value v);
/** Returns true if the provided value is of boolean type.
 * \param v the value to test
 */
int amps_expression_value_is_bool(amps_expression_value v);
/** Returns true if the provided value is of long type.
 * \param v the value to test
 */
int amps_expression_value_is_long(amps_expression_value v);
/** Returns true if the provided value is of double type.
 * \param v the value to test
 */
int amps_expression_value_is_double(amps_expression_value v);
/** Returns true if the provided value is of numeric type.
 * \param v the value to test
 */
int amps_expression_value_is_numeric(amps_expression_value v);
/** Returns true if the provided value is of string type.
 * \param v the value to test
 */
int amps_expression_value_is_string(amps_expression_value v);

/** Set the provided value to be NULL.
 * \param v the value to set
 */
void amps_expression_value_set_null(amps_expression_value v);
/** Set the provided value to be NAN.
 * \param v the value to set
 */
void amps_expression_value_set_nan(amps_expression_value v);
/** Set the provided value to the boolean value provided.
 * \param v the value to set
 * \param b the boolean to store in v
 */
void amps_expression_value_set_bool(amps_expression_value v,int b);
/** Set the provided value to the long value provided.
 * \param v the value to set
 * \param l the long to store in v
 */
void amps_expression_value_set_long(amps_expression_value v,long l);
/** Set the provided value to the double value provided.
 * \param v the value to set
 * \param d the double to store in v
 */
void amps_expression_value_set_double(amps_expression_value v,double d);
/** Set the provided value to the amps_expression_value provided.
 * \param v the value to set
 * \param v_in the value to store in v
 */
void amps_expression_value_set_value(amps_expression_value v, amps_expression_value v_in);


/**
 * Set the provided value to the string value provided.
 * Caller must ensure this string is static, or it must have
 *   been allocated with amps_expression_value_allocate_str().
 * \param v   The value to set.
 * \param p   The string value to store in v.
 * \param len The number of bytes in the string at p, not including
 *            any null-terminator.
 */
void amps_expression_value_set_cstr(amps_expression_value v,
                                    const char* p,
                                    uint32_t len);

/**
 * Allocate a string on the provided value.
 * AMPS will free this memory when the string value is no longer needed.
 * \param   v   The value to allocate a string on.
 * \param   len The length of the string, in bytes.
 * \return  A pointer to the allocated string.
 */
char* amps_expression_value_allocate_cstr(amps_expression_value v,
                                          uint32_t len);

/** Return the contents of the provided value as a boolean.
 * \param v the value to interpret
 * \returns the contents of v
 */
int    amps_expression_value_as_bool(amps_expression_value v);

/** Return the contents of the provided value as a long.
 * \param v the value to interpret
 * \returns the contents of v
 */
long   amps_expression_value_as_long(amps_expression_value v);

/** Return the contents of the provided value as a double.
 * \param v the value to interpret
 * \returns the contents of v
 */
double amps_expression_value_as_double(amps_expression_value v);

/** Return the contents of the provided value as a string.
 * \param v the value to interpret
 * \param out pointer to a char* array that will contain the result
 * \param outLen the length of the array pointed to by out
 */
void   amps_expression_value_as_string(amps_expression_value v,const char** out,size_t* outLen);

/** Return the amps_expression_value at the specified position in the array.
 * \param array the array from which to retrieve a value
 * \param pos the position in the array
 * \returns the expression value at the specified position
 */
amps_expression_value amps_expression_value_get(amps_expression_value_array array, size_t pos);

/**
 * Typedef for a pointer to the implementation of an AMPS user-defined function.
 */
typedef void (*amps_udf_t)(amps_expression_value, unsigned long, amps_expression_value);

/**
 * Value to provide as a parameter count when registering functions
 * that take a variable number of arguments.
 */

static const size_t AMPS_UDF_VARIADIC_PARAMETER_COUNT = 18446744073709551615UL;


/**
 * Register a User Defined Function that can be invoked by AMPS within
 * the AMPS expression language. Once this function is called to register
 * the UDF, the UDF becomes available for use in AMPS.
 * This is typically done during module initialization, to make the
 * UDF available as early as possible during the AMPS initialization process.
 * \param pudf       the function pointer associated with the UDF.
 * \param name       the name of the function to use in the content filters.
 * \param paramcount the number of parameters the function accepts.
 * \returns AMPS_SUCCESS on success.
 */
int amps_register_udf(amps_udf_t pudf, const char* name, size_t paramcount);

/**
 * Serialize a date and time in microseconds(int64_t) as an ISO-8601 string.
 * \param buffer_    the buffer to store the resulting timestamp string to.
 * \param length_    Both an input and an output. On input: the size of buffer_.
 *                                                On output: the number of bytes written to buffer_.
 * \param time_      The time value to convert, using the AMPS internal timestamp representation. If time_ is 0, the current time is used.
 * \returns AMPS_SUCCESS on success.
 */
int amps_get_datetime_string(char* buffer_, size_t* length_, int64_t time_);

/**
 * Get the current date and time as an int64, using the AMPS internal timestamp
 * format.
 * \note The time returned represents an internal timestamp used by AMPS with microsecond precision, and is not a UNIX timestamp value.
 * \param time_        a pointer to an integer in which to store the result.
 * \returns AMPS_SUCCESS on success.
 */
int amps_get_datetime_now(int64_t* time_);


/**
 * Handle for an AMPS embedded client.
 */
typedef void* amps_client_object;

/**
 * Handle for an AMPS embedded message.
 */
typedef void* amps_message_object;

/**
 * Handle for an AMPS command object.
 */
typedef void* amps_command_object;

/**
 * Handle for an AMPS message parser.
 */
typedef void* amps_message_parser;

/**
 * Handle for an AMPS hash value.
 */
typedef void* amps_hash_value;

/**
 * Handle for AMPS topic metadata.
 */
typedef void* amps_topic_metadata;

/**
 * Constants for the type of sow_delete.
 */

#define AMPS_SOW_DELETE_TYPE_NORMAL     0x01
#define AMPS_SOW_DELETE_TYPE_EXPIRATION 0x02
#define AMPS_SOW_DELETE_TYPE_ALL        0xFF
typedef char amps_sow_delete_type;

/**
 * Constants for the type of client disconnect.
 */
#define AMPS_CLIENT_DISCONNECT_TYPE_NORMAL            0x1
#define AMPS_CLIENT_DISCONNECT_TYPE_SLOW_CONSUMER     0x2
#define AMPS_CLIENT_DISCONNECT_TYPE_NAME_IN_USE       0x4
#define AMPS_CLIENT_DISCONNECT_TYPE_MESSAGE_TYPE      0x8
#define AMPS_CLIENT_DISCONNECT_TYPE_UNKNOWN_COMMAND   0x10
#define AMPS_CLIENT_DISCONNECT_TYPE_AUTH              0x20
#define AMPS_CLIENT_DISCONNECT_TYPE_ENTITLEMENT       0x40
#define AMPS_CLIENT_DISCONNECT_TYPE_ENTITLEMENT_RESET 0x80
#define AMPS_CLIENT_DISCONNECT_TYPE_ACTION            0x100
#define AMPS_CLIENT_DISCONNECT_TYPE_HEARTBEAT         0x200
#define AMPS_CLIENT_DISCONNECT_TYPE_ALL               0xFFFF
typedef int amps_client_disconnect_type;

/**
 * Constants for the reason an OOF message was delivered.
 */
#define AMPS_OOF_REASON_DELETED 0x1
#define AMPS_OOF_REASON_EXPIRED 0x2
#define AMPS_OOF_REASON_MATCH   0x3


/**
 * Handle for AMPS event request.
 */
typedef void* amps_event_request;

/**
 * Handle for AMPS event.
 */
typedef void* amps_event;

/**
 * Indicator for the type of an AMPS event.
 */
enum amps_event_type
{
  amps_et_delivery,
  amps_et_sow_delete,
  amps_et_disconnect,
  amps_et_connect,
  amps_et_logon,
  amps_et_offline_start,
  amps_et_offline_stop,
  amps_et_sow_expire
};

/**
 * Indicator for the type of an AMPS command.
 */
enum amps_command_type
{
  amps_ct_publish                 = AMPS_COMMAND_PUBLISH,
  amps_ct_delta_publish           = AMPS_COMMAND_DELTA_PUBLISH,
  amps_ct_subscribe               = AMPS_COMMAND_SUBSCRIBE,
  amps_ct_sow_delete              = AMPS_COMMAND_SOW_DELETE,
  amps_ct_sow                     = AMPS_COMMAND_SOW,
  amps_ct_sow_and_subscribe       = AMPS_COMMAND_SOW_AND_SUBSCRIBE,
  amps_ct_delta_subscribe         = AMPS_COMMAND_DELTA_SUBSCRIBE,
  amps_ct_sow_and_delta_subscribe = AMPS_COMMAND_SOW_AND_DELTA_SUBSCRIBE,
  amps_ct_oof                     = AMPS_COMMAND_OOF,
  amps_ct_group_begin             = AMPS_COMMAND_GROUP_BEGIN,
  amps_ct_group_end               = AMPS_COMMAND_GROUP_END,
  amps_ct_ack                     = AMPS_COMMAND_ACK
};

/**
 * Indicator for the reason a message went out of focus.
 */
enum amps_oof_reason
{
  amps_or_deleted = AMPS_OOF_REASON_DELETED,
  amps_or_expired = AMPS_OOF_REASON_EXPIRED,
  amps_or_match   = AMPS_OOF_REASON_MATCH
};

/**
 * Typedef for a function pointer to be called in response
 * to an event.
 */
typedef int (*amps_event_callback)(void *pUserData_,amps_event event_);

/**
 * Return the hash value of a given topic.
 */
int amps_hash_topic(amps_hash_value *topicHash_,const char *pTopic_,size_t topicLen_,amps_hash_value messageTypeHash_);

/**
 * Return the hash value of a given message type.
 */
int amps_hash_message_type(amps_hash_value *messageTypeHash_,const char *messageType_,size_t messageTypeLen_);

/**
 * Typedef for a function pointer to be called when
 * the internal client receives a message on a subscription or
 * SOW query.
 */
typedef int (*amps_message_handler_function)(amps_message_object messageObject_,void *pUserData_);

///////////////////////////////////////////////////////////////////////
////////////////////// Embedded Client API ////////////////////////////
///////////////////////////////////////////////////////////////////////

/**
 * Create an internal client.
 * \param clientHandle_    Pointer to the client handle that will hold the client
 * \param handler_         The message handler function to use when the client
 *                         receives messages.
 * \param pUserData_       Pointer to be passed to the message handler function
 *                         when the client receives messages.
 * \param messageTypeHash_ Message type of the internal connection.
 * \param clientName_      Name to use for the client. The name of the AMPS
 *                         instance will be added to the provided name.
 * \param clientNameLen_   Length of the client name.
 * \returns                AMPS_SUCCESS when client creation succeeds.
 *                         AMPS_FAILURE when client creation fails -- if
 *                         client creation fails, using the client is
 *                         undefined behavior and may result in server
 *                         instability.
 */
int amps_client_create(amps_client_object* clientHandle_,
                       amps_message_handler_function handler_,
                       void* pUserData_,
                       amps_hash_value messageTypeHash_,
                       const char* clientName_, // We add a {{AMPS_INSTANCE_NAME}} prefix
                       size_t clientNameLen_);

/**
 * Destroy a previously-created client.
 * \param clientHandle_   the client to destroy.
 */
int amps_client_destroy(amps_client_object clientHandle_);


/**
 * Retrieve the last message sequence number that has been acknowledged
 * for the provided client.
 * \param clientHandle_     the client to retrieve the sequence number for
 * \param pClientSequence_  pointer to the location that will hold the sequence
                            number
 */

int amps_client_get_last_acked_client_sequence(amps_client_object clientHandle_,size_t *pClientSequence_);

// These must be created/made synch in module_init
//    This could be used to support clients with their own ackmask bit
//int amps_client_set_synchronous(amps_client_object clientHandle_);

///////////////////////////////////////////////////////////////////////
//               Topic Properties
///////////////////////////////////////////////////////////////////////

// \note These function should only be called after amps has fully initialized.
//       Modules initialize very early and calling these functions too early can cause a segfault.
//       The amps_add_startup_function callback is a safe place to work with
//       topic metadata during the AMPS startup process.

/**
 * Returns a list of topics for the specified message type that match
 * the provided regular expression. After calling this function, you
 * must destroy the returned list.
 * \param pRegexTopic_      regular expression to match
 * \param topicLen_         length of the regular expression string
 * \param pTopicHashList_   pointer to the location that will hold the first
 *                          element of the returned list
 * \param pTopicHashCount_  pointer to the location that will hold the number
 *                          of elements in the returned list
 */
int amps_topic_get_list(const char *pRegexTopic_,size_t topicLen_,amps_hash_value messageTypeHash_,size_t **pTopicHashList_,size_t *pTopicHashCount_);

/**
 * Destroys a list of topics retrieved from amps_topic_get_list
 * \param pTopicHashList_   the list to be destroyed.
 */
int amps_topic_destroy_list(size_t *pTopicHashList_);

/**
 * Returns the size, in bytes, of the opaque (computed) SOW key for this topic.
 * The opaque SOW key is the internal representation that AMPS uses for SOW
 * keys. Notice that, for topics where the SOW key is supplied by the
 * publisher, this internal representation is not provided to subscribers.
 *
 * \param topicMetadata_  the topic for which to retrieve the SOW key size
 * \param pSowKeySize_    pointer to the location that will hold the size of
 *                        the opaque SOW key
 * \note the computed SOW key is 8 bytes in current releases of AMPS: however,
 *       this size is not guaranteed always be 8 bytes in future releases
 */
int amps_topic_metadata_get_opaque_sow_key_size(amps_topic_metadata topicMetadata_,size_t *pSowKeySize_);

/**
 * Returns the topic metadata handle for the topic with the
 * provided topic hash.
 * \param topicHash_  the hash value of the topic name for the topic to
 *                    retrieve metadata for.
 */
amps_topic_metadata amps_get_topic_metadata(amps_hash_value topicHash_);

/**
 * Destroys the provided topic metadata handle.
 */
int amps_topic_metadata_destroy(amps_topic_metadata topicMetadata_);

/**
 * Retrieves whether this topic is in the State of the World.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_is_sow_topic(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves whether this topic is queryable.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_is_queryable(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves whether this topic is a message queue.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_is_message_queue(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves whether this topic is a view.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_is_view_topic(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves whether this topic is a conflated topic.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_is_conflated_topic(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves whether this topic is recorded in the transaction log.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pBool_          pointer to a bool to hold the results of the query
 */
int amps_topic_metadata_in_transaction_log(amps_topic_metadata topicMetadata_,int *pBool_);

/**
 * Retrieves the name of this topic.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pTopic_         pointer to a cstring to hold the results of the query
 */
int amps_topic_metadata_get_topic_name(amps_topic_metadata topicMetadata_,struct amps_cstring *pTopic_);

/**
 * Retrieves the list of key fields for this topic.
 * \param topicMetaData_  the topic metadata handle to retrieve information on
 * \param pKeyList_       pointer to location that will hold a pointer to the
                          head of the list of key fields
 * \param pKeyCount_      pointer to the location that will hold the count of
 *                        key fields.
 */
int amps_topic_metadata_get_key_list(amps_topic_metadata topicMetadata_,struct amps_cstring **pKeyList_,size_t *pKeyCount_);

///////////////////////////////////////////////////////////////////////
//               Sow functions
///////////////////////////////////////////////////////////////////////

/**
 * Compact the specified SOW topic file.
 * \param topicHash_ the hash for the topic to compact
 */
int amps_sow_compact(amps_hash_value topicHash_);


/**
 * Compact all SOW topic files in the instance.
 */
int amps_sow_compact_all(void);

///////////////////////////////////////////////////////////////////////
//               Commands and Execution
///////////////////////////////////////////////////////////////////////

/**
 * Create a command for use with the embedded AMPS client. Notice that a
 * command is associated with a specific client, and should only be
 * used with that client. A command must be destroyed once the module
 * is finished with the command.
 * \param clientHandle_   the handle to the client that will execute the
 *                        command
 * \param commandObject_  pointer to the location that will hold the created
 *                        command
 * \param commandType_    type of command
 */
int amps_command_create(amps_client_object clientHandle_,
                        amps_command_object* commandObject_,
                        enum amps_command_type commandType_);

/**
 * Destroy a command.
 * \param commandObject_  the command to be destroyed.
 */
int amps_command_destroy(amps_command_object commandObject_);

// Unset a value by passing NULL,0

/**
 * Set the topic that the command applies to.
 * \param commandObject_ the command for which to set the topic
 * \param pTopic_        the name of the topic
 * \param topicLen_      the length of the name of the topic
 * \note the topic can be removed from the command by passing NULL for pTopic_
 *       and 0 for topicLen_.
 */
int amps_command_set_topic(amps_command_object commandObject_, const char* pTopic_, size_t topicLen_);

/**
 * Set the filter used with the command.
 * \param commandObject_  the command for which to set the filter
 * \param pFilter_        the name of the filter
 * \param filterLen_      the length of the name of the filter
 */
int amps_command_set_filter(amps_command_object commandObject_, const char* pFilter_, size_t filterLen_);

/**
 * Set the message sequence number for the command.
 * \param commandObject_    the command for which to set the sequence number
 * \param clientSequence_   the sequence number to set
 */
int amps_command_set_client_sequence(amps_command_object commandObject_,size_t clientSequence_);

/**
 * Set the correlation ID for the command.
 * \param commandObject_    the command for which to set the correlation ID
 * \param pCorrelationId_   the value to set for the correlation ID
 * \param correlationIdLen_ the length of the correlation ID
 */
int amps_command_set_correlation_id(amps_command_object commandObject_, const char* pCorrelationId_, size_t correlationIdLen_);

/**
 * Set the data for the command.
 * \param commandObject_    the command for which to set the data
 * \param pData_            the value to set for the data
 * \param dataLen_          the length of the data
 */
int amps_command_set_data(amps_command_object commandObject_, const char* pData_, size_t dataLen_);

/**
 * Set the options for the command. These are a comma-delimited list of option
 * values.
 * \param commandObject_    the command for which to set options
 * \param pOptions_         the options string
 * \param optionsLen_          the length of the options string
 */
int amps_command_set_options(amps_command_object commandObject_, const char* pOptions_, size_t optionsLen_);

/**
 * Set the acknowledgement types for the command. These are a comma-delimited
 * list of types.
 * \param commandObject_    the command for which to set options
 * \param pAckType_         the ack type string
 * \param ackTypeLen_          the length of the ack type string
 */
int amps_command_set_ack_type(amps_command_object commandObject_, const char* pAckType_, size_t ackTypeLen_);

/**
 * Set the command ID for the command. Notice that the external AMPS clients
 * generally provide the command ID automatically. With the embedded client,
 * the module must explicitly set the command ID if needed (for example,
 * to correlate `processed` acknowledgements with commands).
 * \param commandObject_    the command for which to set options
 * \param pCommandId_       the command ID string
 * \param commandIDLen_     the length of the command ID string
 */
int amps_command_set_command_id(amps_command_object commandObject_, const char* pCommandId, size_t commandIdLen_);

/**
 * Set the auth ID for the command. Since modules run within the AMPS
 * process space, the module is allowed to set any auth ID: no verification or
 * authentication is performed.
 * \param commandObject_    the command for which to set options
 * \param pAuthId_          the auth ID string
 * \param authIDLen_        the length of the auth ID string
 */
int amps_command_set_auth_id(amps_command_object commandObject_, const char* pAuthId_, size_t authIdLen_);

/**
 * Set the bookmark for the command.
 * \param commandObject_      the command for which to set options
 * \param pBookmark_          the string that contains the bookmark to set
 * \param bookmarkLen_        the length of the bookmark string
 */
int amps_command_set_bookmark(amps_command_object commandObject_, const char* pBookmark_, size_t bookmarkLen_);

/**
 * Set the order by clause for the command.
 * \param commandObject_      the command for which to set options
 * \param pOrderBy_          the string that contains the order by clause
 * \param orderByLen_        the length of the order by clause
 */
int amps_command_set_order_by(amps_command_object commandObject_, const char* pOrderBy_, size_t orderByLen_);

/**
 * Set the top n parameter for the command.
 * \param commandObject_      the command for which to set options
 * \param pTopN_              the string that contains the top N value
 * \param topNLen_          the length of the order by clause
 */
int amps_command_set_top_n(amps_command_object commandObject_, const char* pTopN_, size_t topNLen_);

/**
 * Clear all parameters in the command.
 * \param commandObject_ the command to be cleared
 */
int amps_command_clear(amps_command_object commandObject_);

/**
 * Execute the command. Unlike an external client, _all_ messages for an
 * embedded client are provided to the same  message handler, which is
 * specified when the client is constructed. This function does not accept a
 * message handler parameter.  The embedded client does not offer synchronous
 * message processing.
 * \param commandObject_ the command to be executed
 */
int amps_command_execute(amps_command_object commandObject_);

///////////////////////////////////////////////////////////////////////
//               Message Properties
///////////////////////////////////////////////////////////////////////
int amps_message_clone(amps_message_object messageObject_,amps_message_object *pMessageCopy_);
int amps_message_destroy(amps_message_object messageObject_);

int amps_message_get_command_type(amps_message_object messageObject_, enum amps_command_type* pCommandType_);
int amps_message_get_topic(amps_message_object messageObject_, struct amps_cstring* pTopic_);
int amps_message_get_topic_hash(amps_message_object messageObject_, amps_hash_value* pHash_);
int amps_message_get_is_replication(amps_message_object messageObject_, int* pBool_);
int amps_message_get_is_local(amps_message_object messageObject_, int* pBool_);
int amps_message_get_sow_key(amps_message_object messageObject_, struct amps_cstring* pSowKey);
// Note the size of the void* buffer is determined by amps_topic_metadata_get_opaque_sow_key_size
int amps_message_get_opaque_sow_key(amps_message_object messageObject_, void* pOpaqueSowKey_);
int amps_message_get_correlation_id(amps_message_object messageObject_, struct amps_cstring* pCorrelationId_);
int amps_message_get_client_sequence(amps_message_object messageObject_,size_t *pClientSequence_);
int amps_message_get_bookmark(amps_message_object messageObject_, struct amps_cstring* pBookmark_);
int amps_message_get_timestamp(amps_message_object messageObject_, struct amps_cstring* pTimestamp_);
// Note the *pTxtimestamp represents an AMPS internal timestamp in microseconds, not a UNIX timestamp.
int amps_message_get_timestamp_numeric(amps_message_object messageObject_,int64_t *pTxTimestamp_);
int amps_message_get_oof_reason(amps_message_object messageObject_, enum amps_oof_reason *pOOFReason_);
int amps_message_get_status(amps_message_object messageObject_, struct amps_cstring* pStatus_);
int amps_message_get_data(amps_message_object messageObject_, struct amps_cstring* pData_);
int amps_message_get_previous_data(amps_message_object messageObject_, struct amps_cstring* pData_);
int amps_message_get_ack_type(amps_message_object messageObject_, size_t* pAckType_);
int amps_message_get_reason(amps_message_object messageObject_, struct amps_cstring* pReason_);
int amps_message_get_client_name(amps_message_object messageObject_, struct amps_cstring* pClientName_);
int amps_message_get_publisher_name(amps_message_object messageObject_, struct amps_cstring* pClientName_);

///////////////////////////////////////////////////////////////////////
////////////////////// Event API //////////////////////////////////////
///////////////////////////////////////////////////////////////////////

int amps_event_request_create(amps_event_request *pEventRequest_,enum amps_event_type eventType_);
int amps_event_request_destroy(amps_event_request eventRequest_);

// Unset a value by passing NULL,0
int amps_event_set_topic(amps_event_request eventRequest_,const char *pTopic_,size_t topicLen_);
int amps_event_set_message_type_hash(amps_event_request eventRequest_,amps_hash_value messageTypeHash_);
int amps_event_set_sow_delete_type(amps_event_request eventRequest_,amps_sow_delete_type sowDeleteType_);

int amps_event_get_topic(amps_event event_,struct amps_cstring *pTopic_);
int amps_event_get_transport_name(amps_event event_,struct amps_cstring *pTransportName_);
int amps_event_get_client_name(amps_event event_,struct amps_cstring *pClientName_);
int amps_event_get_client_id(amps_event event_,struct amps_cstring *pClientId_);
int amps_event_get_data(amps_event event_,struct amps_cstring *pData_);
int amps_event_get_bookmark(amps_event event_,struct amps_cstring *pBookmark_);

int amps_event_get_sow_delete_type(amps_event event_,amps_sow_delete_type* pSowDeleteType_);
int amps_event_get_disconnect_type(amps_event event_,amps_client_disconnect_type *pClientDisconnectType_);
int amps_event_get_disconnect_reason(amps_event event_,struct amps_cstring *pReason_);

// starts listening for the event
int amps_event_listen(amps_event_request eventRequest_,amps_event_callback callback_,void *pUserData_);

///////////////////////////////////////////////////////////////////////
////////////////////// Message Parser API /////////////////////////////
///////////////////////////////////////////////////////////////////////

// Create message parser
int amps_message_parser_create(amps_message_parser* pParser_,amps_hash_value messageTypeHash_);
int amps_message_parser_destroy(amps_message_parser parser_);

// Register an XPath with message processor
int amps_message_parser_register_xpath(amps_message_parser parser_,const char *xpath_,size_t xpathLen_);
int amps_message_parser_register_filter(amps_message_parser parser_,const char* contentFilter_,size_t contentFilterLen_);
// Parse a message
int amps_message_parser_parse(amps_message_parser parser_,const char *data_,size_t dataLen_);

// Extract values: expressionValue vector needs to contain the same number of
//    amps_expression_value's as there were xpath's registered with the message
//    parser.
int amps_message_parser_extract(amps_message_parser parser_,amps_expression_value* pExpressionValueList_);
int amps_message_parser_evaluate_filter(amps_message_parser parser_,int* pExpressionValueList_);

int amps_message_parser_register_expression(amps_message_parser messageParser_,const char* expression_,size_t expressionLen_);
int amps_message_parser_evaluate_expression(amps_message_parser messageParser_,amps_expression_value* pExpressionValueList_);

/*  Example Message Parser Example

amps_message parser parser = NULL;
amps_message_parser_create(&parser, msgTypeHash);

amps_message_parser_register_xpath(parser, "/1", 2);
amps_message_parser_register_xpath(parser, "/2", 2);

amps_message_parser_register_filter(parser, "/1 IS NULL", 10);
amps_message_parser_register_filter(parser, "1=1 or /2=/3", *);
amps_message_parser_register_filter(parser, "1=0", 3);
amps_message_parser_register_expression(parser, "1*2+/5", *);

amps_message_parser_parse(parser, "1=123\x012=55\x01", 11);

amps_expression_value values[2];
amps_message_parser_extract(parser, values);

amps_expression_value results[3];
amps_message_parser_evaluate_filter(parser, results);

amps_expression_value expressionResults[1];
amps_message_parser_evaluate_expression(parser, results);

assert(amps_expression_value_as_long(values[0]) == 123);
assert(amps_expression_value_as_long(values[1]) == 55);

assert(amps_expression_value_as_bool(results[0]) == false);
assert(amps_expression_value_as_bool(results[1]) == true);
assert(amps_expression_value_as_bool(results[2]) == false);

amps_message_parser_destroy(parser);


*/

#ifdef __cplusplus
  }
#endif
#endif
