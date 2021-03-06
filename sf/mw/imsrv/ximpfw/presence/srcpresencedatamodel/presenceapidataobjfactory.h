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
* Description:  Codec to pack and unpack data objects.
*
*/

#ifndef PRESENCEAPIDATAOBJFACTORY_H
#define PRESENCEAPIDATAOBJFACTORY_H

#include <e32base.h>
#include "presencetypehelpers.h"
#include "ximpapidataobjbase.h"

// internal interface for the factory
#include "apidataobjfactory.h"

class RReadStream;
class RWriteStream;


/**
 * Codec to pack and unpack data object arrays.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceApiDataObjFactory ) : public CBase,
                                                  public MApiDataObjFactory
    {
public:

    static CPresenceApiDataObjFactory* NewL();
    
    // From base class
    CXIMPApiDataObjBase* NewFromStreamLC( TInt32 aDataObjInterfaceId,
                                          RReadStream& aStream );
    /**
     * Unpacks a request parameter array from a 
     * descriptor back to array format.
     * @param aDataObjArrayData The packed array
     * @param aArray The array which to fill
     */
    static void InternalizeL( 
            RReadStream& aStream,
            RPresenceReqParamArray& aArray
            );

    /**
     * Packs a given request parameter array into 
     * a stream.
     * @param aWs The stream to pack into
     * @param aArray The array to pack
     */    
    static void ExternalizeL( 
            RWriteStream& aWs,
            const RPresenceReqParamArray& aArray
            );

    /**
     * Internalize a CXIMPApiDataObjBase-derived object
     * from the given stream, ownership is returned to the caller.
     * @param aStream The stream
     * @return The object instance
     */
    static CXIMPApiDataObjBase* InternalizeLC(
            RReadStream& aStream );

    /**
     * Externalize a CXIMPApiDataObjBase-derived object
     * to the given stream.
     * @param aWs The write stream
     * @param aDataObj The data object
     */
    static void ExternalizeL(
            RWriteStream& aWs,
            const CXIMPApiDataObjBase& aDataObj );
    

    /*
     * Packs a given array into a stream.
     * @param aStream The stream
     * @param aArray The array
     */
    template< class INFOIMP>    
        static inline void InternalizeL( 
                RReadStream& aStream,
                RPointerArray<INFOIMP>& aArray
                );

    /**
     * Unpacks an array of info fields back to array format.
     * @param aWs The stream
     * @param aArray The array which to fill
     */
    template< class INFOIMP >
        static inline void ExternalizeL( 
                RWriteStream& aWs,
                const RPointerArray<INFOIMP>& aArray
                );


    ~CPresenceApiDataObjFactory();
private:

    CPresenceApiDataObjFactory();

    void ConstructL();

    };

// template methods need inline implementation
#include "presenceapidataobjfactory.inl"

#endif // PRESENCEAPIDATAOBJFACTORY_H
