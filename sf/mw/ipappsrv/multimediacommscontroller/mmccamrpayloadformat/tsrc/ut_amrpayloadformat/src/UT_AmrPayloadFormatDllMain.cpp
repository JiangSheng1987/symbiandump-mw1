/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/




//  CLASS HEADER
#include "UT_CAmrCommonUtility.h"
#include "UT_TStreamDecoder.h"
#include "UT_TStreamEncoder.h"
#include "UT_CAmrPayloadFormatWrite.h"
#include "UT_CAmrPayloadFormatRead.h"
#include "UT_CAmrPayloadDecoder.h"
#include "UT_CAmrPayloadEncoder.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    //return UT_CAmrCommonUtility::NewL();
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CAmrCommonUtility::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TStreamDecoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TStreamEncoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CAmrPayloadFormatWrite::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CAmrPayloadFormatRead::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CAmrPayloadEncoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CAmrPayloadDecoder::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }



//  END OF FILE





