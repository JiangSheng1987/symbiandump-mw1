/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionExecutionHandler class declaration.
 *
*/


#ifndef CFACTIONEXECUTIONHANDLER_H
#define CFACTIONEXECUTIONHANDLER_H

#include <e32base.h>	// For CActive, link against: euser.lib

#include <e32std.h>		// For RTimer, link against: euser.lib

// FORWARD DECLARATIONS
class CCFActionPlugInThread;

NONSHARABLE_CLASS( CCFActionExecutionHandler ): public CActive
    {
public:
    // Cancel and destroy
    ~CCFActionExecutionHandler();

    // Two-phased constructor.
    static CCFActionExecutionHandler* NewL(
        CCFActionPlugInThread* aActionPluginThread );

    // Two-phased constructor.
    static CCFActionExecutionHandler* NewLC(
        CCFActionPlugInThread* aActionPluginThread );

public: // New functions
    // Function for making the initial request
    void StartAsyncActionsWait();

private:
    // C++ constructor
    CCFActionExecutionHandler( CCFActionPlugInThread* aActionPluginThread );

    // Second-phase constructor
    void ConstructL();

private: // From CActive
    // Handle completion
    void RunL();

    // How to cancel me
    void DoCancel();

    // Override to handle leaves from RunL(). Default implementation causes
    // the active scheduler to panic.
    TInt RunError( TInt aError );

private:

    CCFActionPlugInThread* iActionPlugInThread;
    };

#endif // CFACTIONEXECUTIONHANDLER_H

