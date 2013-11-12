// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// The class definitions for the simple application for testing using T_SynAppListAndFileUpdate.cpp
// containing a single view with the text "App Installed !" drawn on it.
// 
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/


#ifndef __TESTAPPINSTALL_H
#define __TESTAPPINSTALL_H

#include <coeccntx.h>

#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <techview/eikmenup.h>

#include <techview/eikon.hrh>

#include <testappinstall.rsg>
#include "TestAppInstall.hrh"



//
//
// CExampleApplication
//
//

class CExampleApplication : public CEikApplication
	{
private: 
	// Inherited from class CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

//
//
// CExampleAppView
//
//
class CExampleAppView : public CCoeControl
    {
public:
	static CExampleAppView* NewL(const TRect& aRect);
	CExampleAppView();
	~CExampleAppView();
    void ConstructL(const TRect& aRect);

private:
	// Inherited from CCoeControl
	void Draw(const TRect& /*aRect*/) const;

private:
	HBufC*  iExampleText;
    };


//
//
// CExampleAppUi
//
//
class CExampleAppUi : public CEikAppUi
    {
public:
    void ConstructL();
	~CExampleAppUi();

private:
	// Inherirted from class CEikAppUi
	void HandleCommandL(TInt aCommand);

private:
	CCoeControl* iAppView;
	};


//
//
// CExampleDocument
//
//
class CExampleDocument : public CEikDocument
	{
public:
	static CExampleDocument* NewL(CEikApplication& aApp);
	CExampleDocument(CEikApplication& aApp);
	void ConstructL();
private: 
	// Inherited from CEikDocument
	CEikAppUi* CreateAppUiL();
	};


#endif
