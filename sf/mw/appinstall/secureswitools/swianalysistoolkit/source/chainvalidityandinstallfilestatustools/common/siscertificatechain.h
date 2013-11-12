/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/


/**
 @file 
 @internalComponent 
*/

#ifndef __SISCERTIFICATECHAIN_H__
#define __SISCERTIFICATECHAIN_H__
#pragma warning(disable: 4786)

#include "options.h"
#include "swicertstore.h"
#include "launchprocess.h"
#include "certificatechain.h"
#include "dumpchainvaliditytool.h"
#include "osinterface.h"
#include <algorithm>

typedef std::vector <std::string>  StringVector ; 

class SWICertStore;
class CertificateChain;

class SISCertificateChain
{
public:

	SISCertificateChain();

	~SISCertificateChain();

	/**
	  Iterates the directory containing the certificate chains(as generated by SignSIS) and calls functions which 
	  validates the certificate chains .
	  @param	aDirectory	Reference of the directory containing the certificate chains(1 pem file per chain).
	  @return	1			If sis file is signed.
				0			If sis file is unsigned.
	 */
	int Run(const Options& aOptions , const SWICertStore& aStore , const std::string& aDirectory); 

	/**
	  Checks whether all the mandatory certificate available in the certstore are used to sign
	  the sis file.This function helps to diagnose why installation failed,even though the sis file
	  is signed with a chain validated by certstore.
	 */
	void VerifyMandatory(const SWICertStore& aStore);

	/** Returns devcert capabilities .
	  */
	int GetDevCertCapabilities() const { return iDevCertCapabilities;}

	/** Returns a vector containing the signatures of the root certificates present in the 
	  * certstore which validates a certificate chain .
	  */
	const StringVector& GetValidatedRootSignature() const {return iValidatedRootSignature;}

	/** Returns a vector containing the signatures of the root certificates present in the 
	  * certstore which validates a certificate chain having a devcert as end entity certificate.
	  */
	const StringVector& GetDevCertValidatedRootSignature() const {return iDevCertValidatedRootSignature;}
	
private:

	int iDevCertCapabilities;
	CertificateChain* iCertChain;
	StringVector iValidatedRootSignature;
	StringVector iDevCertValidatedRootSignature;
};

#endif