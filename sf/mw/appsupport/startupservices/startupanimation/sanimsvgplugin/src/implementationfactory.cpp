/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SAnimSvgPlugIn ECOM implementation factory.
*
*/


#include "sanimmifplugin.h"
#include "sanimsvgplugin.h"

#include <ecom/implementationproxy.h>

// Define the interface UIDs.
const TImplementationProxy ImplementationTable[] =
    {
    // Remove PCLint note "C-style cast"
    //lint -e1924
    // Remove PCLint warning "Suspicious cast"
    //lint -e611
    IMPLEMENTATION_PROXY_ENTRY( 0x2000B11E, CSAnimMifPlugin::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( 0x2000B122, CSAnimSvgPlugin::NewL )
    //lint +e1924
    //lint +e611
    };


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy.
//
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }


//  End of File
