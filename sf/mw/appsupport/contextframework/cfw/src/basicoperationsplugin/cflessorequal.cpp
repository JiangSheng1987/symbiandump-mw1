/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFLessOrEqual class implementation.
*
*/



// INCLUDES
#include "cflessorequal.h"
#include "cfcontextobject.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptLessOrEqualName, "lessOrEqual" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFLessOrEqual::CCFLessOrEqual
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFLessOrEqual::CCFLessOrEqual( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource,
    const CCFContextOperation::TCmpType aType )
    :   CCFContextOperation( aServices, aParent, aName, aSource ),
        iType( aType )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFLessOrEqual::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFLessOrEqual::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    iCmpValue = aCmpVal.AllocL();
    }

// -----------------------------------------------------------------------------
// CCFLessOrEqual::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFLessOrEqual* CCFLessOrEqual::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    CCFLessOrEqual* self
        = NewLC( aServices, aParent, aName, aSource, aType, aCmpVal );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFLessOrEqual::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFLessOrEqual* CCFLessOrEqual::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFLessOrEqual* self = new( ELeave ) CCFLessOrEqual( aServices,
            aParent,
            name,
            source,
            aType );
    CleanupStack::Pop( source );
    CleanupStack::Pop( name );

    CleanupStack::PushL( self );
    self->ConstructL( aCmpVal );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFLessOrEqual::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFLessOrEqual* CCFLessOrEqual::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptLessOrEqualName ) != 0 )
        {
        return NULL; // Cannot create less or equal operation from the node.
        }

    TPtrC contextSource;
    TPtrC contextType;
    TCmpType cmpType( CCFContextOperation::EInvalidCmpType );
    TPtrC cmpValue;
    TInt contextLevelDelay( 0 );
    TBool argsOK = CFContextOperationUtils::ParseTwoComparisonArgs( aNode,
            contextSource,
            contextType,
            cmpType,
            cmpValue,
            contextLevelDelay );

    CCFLessOrEqual* self = NULL;
    if ( argsOK )
        {
        self = NewL( aServices,
                aParent,
                contextType,
                contextSource,
                cmpType,
                cmpValue );
        if ( contextLevelDelay )
            {
            self->iContextLevelDelay = contextLevelDelay;
            }
        }
    else
        {
        INFO( "CCFLessOrEqual::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFLessOrEqual::~CCFLessOrEqual()
    {
    FUNC_LOG;

    delete iCmpValue;
    }

// -----------------------------------------------------------------------------
// CCFLessOrEqual::IsTrueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCFLessOrEqual::IsTrueL( const CCFContextObject& aContextObject )
    {
    FUNC_LOG;

    TBool value( EFalse );
    switch ( iType )
        {
        case CCFContextOperation::EIntCmp:
            {
            TInt a = CFContextOperationUtils::StringToIntL( *iCmpValue );
            TInt b = CFContextOperationUtils::StringToIntL(
                    aContextObject.Value() );
            value = b <= a;
            break;
            }
        case CCFContextOperation::ETimeCmp:
            {
            TTime a = CFContextOperationUtils::StringToTimeL( *iCmpValue );
            TTime b = CFContextOperationUtils::StringToTimeL(
                    aContextObject.Value() );
            value = b <= a;
            break;
            }
        case CCFContextOperation::EFloatCmp:
            {
            TReal a = CFContextOperationUtils::StringToRealL( *iCmpValue );
            TReal b = CFContextOperationUtils::StringToRealL(
                    aContextObject.Value() );
            value = b <= a;
            break;
            }
        case CCFContextOperation::EStringCmp:
        default:
            {
            value = aContextObject.Value() <= ( *iCmpValue );
            break;
            }
        }

    return value;
    }
