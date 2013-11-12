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
* Description: XIMP Framework Test Code 
*
*/


//  CLASS HEADER
#include "T_PresenceCache.h"

//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>

EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_PresenceCache::NewL();
    }

#ifndef EKA2
GLDEF_C TInt E32Dll( TDllReason )
	{
	return KErrNone;
	}
#endif

//  END OF FILE