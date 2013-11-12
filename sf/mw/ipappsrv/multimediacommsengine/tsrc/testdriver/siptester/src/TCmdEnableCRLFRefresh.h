/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  See class definition below.
*
*/

#ifndef TCMDENABLECRLFREFRESH_H
#define TCMDENABLECRLFREFRESH_H

// INCLUDES
#include "TCmdUNSAFTimers.h"

// FORWARD DECLARATIONS
class CRepository;

// CLASS DEFINITION
/**
 * Command for disabling/enabling keepalive CRLF over UDP to a defined SIP proxy
 */
class TCmdEnableCRLFRefresh
	: public TCmdUNSAFTimers
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdEnableCRLFRefresh( MTcTestContext& aContext ) :
			TCmdUNSAFTimers( aContext ) {}

	public: // From TTcSIPCommandBase

		void ExecuteL();

	public: // New methods

		/**
		 * Static function for matching the name of this command to
		 * a function identifier.
		 *
		 * @param aId An initialized identifier containing a function name.
		 * @return ETrue if this command matches the specified name.
		 */
		static TBool Match( const TTcIdentifier& aId );

		/**
		 * Static fuction for creating an instance of this class
		 *
		 * @param aContext SIP Test case context
		 * @return An initialized instance of this class.
		 */
		static TTcCommandBase* CreateL( MTcTestContext& aContext );
	
	private: // New functions, for internal use
	};

#endif // TCMDENABLECRLFREFRESH_H