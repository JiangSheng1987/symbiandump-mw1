/*
* Copyright (c) 2001-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  EAP and WLAN authentication protocols.
*
*/

/*
* %version: %
*/

#if !defined(_ABS_SIMPLE_CONFIG_MESSAGE_RECORD_PROCESS_H_)
#define _ABS_SIMPLE_CONFIG_MESSAGE_RECORD_PROCESS_H_

#include "eap_am_export.h"
// Start: added by script change_export_macros.sh.
#if defined(EAP_NO_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H)
	#define EAP_CLASS_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_NONSHARABLE 
	#define EAP_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
	#define EAP_C_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
	#define EAP_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
	#define EAP_C_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
#elif defined(EAP_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H)
	#define EAP_CLASS_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_EXPORT 
	#define EAP_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_FUNC_EXPORT 
	#define EAP_C_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_C_FUNC_EXPORT 
	#define EAP_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_FUNC_EXPORT 
	#define EAP_C_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_C_FUNC_EXPORT 
#else
	#define EAP_CLASS_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_IMPORT 
	#define EAP_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_FUNC_IMPORT 
	#define EAP_C_FUNC_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H EAP_C_FUNC_IMPORT 
	#define EAP_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
	#define EAP_C_FUNC_EXPORT_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H 
#endif
// End: added by script change_export_macros.sh.

/// This class declares the functions message classes of SIMPLE_CONFIG
/// requires from the SIMPLE_CONFIG.
class EAP_CLASS_VISIBILITY_ABS_SIMPLE_CONFIG_APPLY_CIPHER_SPEC_H abs_simple_config_apply_cipher_spec_c
{
private:
	//--------------------------------------------------

	//--------------------------------------------------
protected:
	//--------------------------------------------------

	//--------------------------------------------------
public:
	//--------------------------------------------------

	/// Destructor does nothing.
	virtual ~abs_simple_config_apply_cipher_spec_c()
	{
	}

	/// Constructor does nothing.

	abs_simple_config_apply_cipher_spec_c()
	{
	}

	/**
	 * This function applies the send cipher suite to record message.
	 * @param simple_config_record_message_buffer includes the buffer of the whole SIMPLE_CONFIG-record.
	 */
	virtual eap_status_e apply_send_cipher_suite(
		eap_variable_data_c * const simple_config_record_message_buffer) = 0;

	//--------------------------------------------------
}; // class abs_simple_config_apply_cipher_spec_c

#endif //#if !defined(_ABS_SIMPLE_CONFIG_MESSAGE_RECORD_PROCESS_H_)

//--------------------------------------------------



// End.
