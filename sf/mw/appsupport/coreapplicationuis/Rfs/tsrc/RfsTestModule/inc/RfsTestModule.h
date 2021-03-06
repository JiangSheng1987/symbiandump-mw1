/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: STIF testclass declaration
*
*/

#ifndef RFSTESTMODULE_H
#define RFSTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <rfsHandler.h>
#include <W32STD.H> // TKeyEvent
#include <eikclbd.h>
#include <EIKENV.H>
#include <rfsclient.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KRfsTestModuleLogPath, "\\logs\\testframework\\RfsTestModule\\" ); 
// Log file
_LIT( KRfsTestModuleLogFile, "RfsTestModule.txt" ); 
_LIT( KRfsTestModuleLogFileWithTitle, "RfsTestModule_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CRfsTestModule;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CRfsTestModule test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CRfsTestModule) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CRfsTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CRfsTestModule();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CRfsTestModule( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
        
        // CRfsHandler test methods
        //void SetupEmptyL();        
        //void TeardownEmpty();        
        TInt SetupL(CStifItemParser& aItem);        
        TInt Teardown(CStifItemParser& aItem); 
        TInt SpecialSetupL(CStifItemParser& aItem);
        TInt SpecialTeardown(CStifItemParser& aItem);         
        TInt TestCRfsHandlerL(CStifItemParser& aItem);        
        TInt TestActivateRfsLL(CStifItemParser& aItem);        
        TInt TestCancelL(CStifItemParser& aItem);
        
        
        static TInt SimulateRightSoftkey( TAny* aPtr);
        void DoSimulateRightL();
        
        // RRfsClient test
        TInt RRfsClientSetupEmptyL(CStifItemParser& aItem);
        TInt RRfsClientSetupL(CStifItemParser& aItem);
        TInt RRfsClientTeardown(CStifItemParser& aItem);
        TInt RRfsClientTeardownEmpty(CStifItemParser& aItem);
               
        TInt TestConnectAndCloseL(CStifItemParser& aItem);
        TInt TestPerformRfsNormalL(CStifItemParser& aItem);
        TInt TestPerformRfsDeepL(CStifItemParser& aItem);
        TInt TestPerformPostEikonRfsL(CStifItemParser& aItem);
        TInt TestPerformPostEikonRfsDeepL(CStifItemParser& aItem);        
        TInt TestRunScriptInDescriptorNoErrorL(CStifItemParser& aItem);
        TInt TestRunScriptInDescriptorSyntaxErrorL(CStifItemParser& aItem);
        TInt TestRunScriptInFileNoErrorL(CStifItemParser& aItem);
        TInt TestRunScriptInFileNotFoundL(CStifItemParser& aItem);

        
        
        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        //virtual TInt ExampleL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;
        CRfsHandler* iCRfsHandler;
        CPeriodic* iPeriodic;
        CActiveScheduler* iScheduler;
        RRfsClient iRRfsClient;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // RFSTESTMODULE_H

// End of File
