// ssyreferenceaccelerometer.h

/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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




/**
 @file
 @internalAll Sensor framework Reference SSY header file
 @test
*/


#ifndef SSYREFERENCEACCELEROMETER_H
#define SSYREFERENCEACCELEROMETER_H

//  INCLUDES
#include <e32base.h>
#include <sensors/sensrvtypes.h> 


// ACCELEROMETER RELATED CHANNELS 

/**
* - Name:          Accelerometer XYZ-axis data channel type
* - Type:          Rawdata
* - Datatype:      TSensrvAccelerometerAxisData
* - Description:   Accelerometer x-, y-, z-axis data  
*/
const TSensrvChannelTypeId KSensrvChannelTypeIdAccelerometerXYZAxisData = 0x1020507E;

// ACCELEROMETER RELATED PROPERTIES

/**
* - Name:         Axis active
* - Type:         TInt 
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Indicates is sensor axis activated.
*                 Value is one if the axis is activated, zero otherwise.
*/
const TSensrvPropertyId KSensrvPropIdAxisActive = 0x00001001;

/**
* - Name:         Axis threshold value
* - Type:         TReal
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Threshold value for a sensor channel
*/
const TSensrvPropertyId KSensrvPropIdAxisThresholdValue = 0x00001002;

// ACCELEROMETER RELATED DATATYPES

/**
* Accelerometer axis data type
*/
class TSensrvAccelerometerAxisData 
    {
public:
    /**
    * Channel data type Id number
    */      
    static const TSensrvChannelDataTypeId KDataTypeId = 0x1020507E;

    /**
     * Channel data type index numbers
     */
	enum TSensrvAccelerometerAxisDataIndexes
          {
           ETimeStamp = 0,
           EAxisX,
           EAxisY,
           EAxisZ
          }; 

public:
    /**
    * - Item name:   Sampling time.
    * - Item Index:  0
    * - Conditions:  None
    * - Description: Timestamp for a sample.
    */   
    TTime iTimeStamp;    

    /**
    * - Item name:   Accelerometer x-axis 
    * - Item Index:  1
    * - Conditions:  Single limit and range 
    * - Description: Accelerometer values from x-axis 
    */
    TInt iAxisX;  

    /**
    * - Item name:   Accelerometer y-axis 
    * - Item Index:  2
    * - Conditions:  Single limit and range 
    * - Description: Accelerometer values from y-axis 
    */ 
    TInt iAxisY;  

    /**
    * - Item name:   Accelerometer z-axis 
    * - Item Index:  3
    * - Conditions:  Single limit and range 
    * - Description: Accelerometer values from z-axis 
    */
    TInt iAxisZ;  
    };


// TAPPING RELATED CHANNELS 

/**
* - Name:          Wakeup event channel type
* - Type:          Event
* - Datatype:      TSensrvTappingData
* - Description:   Wakeup events 
*/
const TSensrvChannelTypeId KSensrvChannelTypeIdAccelerometerWakeupData = 0x1020507F;

/**
* - Name:          Double tapping event channel type
* - Type:          Event
* - Datatype:      TSensrvTappingData
* - Description:   Double tapping events 
*/
const TSensrvChannelTypeId KSensrvChannelTypeIdAccelerometerDoubleTappingData = 0x10205081;

// TAPPING RELATED PROPERTIES

/**
* - Name:         Tapping axis active
* - Type:         TInt 
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Indicates is sensor axis activated.
*                 Value is one if the axis is activated, zero otherwise.
*/
const TSensrvPropertyId KSensrvPropIdTappingAxisActive = 0x00001001;

/**
* - Name:         Tapping axis threshold value
* - Type:         TReal
* - Scope:        Channel item property
* - Mandatory:    No
* - Capability:   None
* - Description:  Threshold value for a sensor channel
*/
const TSensrvPropertyId KSensrvPropIdTappingAxisThresholdValue = 0x00001002;

/**
* - Name:         Tap duration in milliseconds
* - Type:         TInt
* - Scope:        Channel property
* - Mandatory:    No
* - Capability:   None
* - Description:  Tapping duration setting
*/
const TSensrvPropertyId KSensrvPropIdTapDurationValue = 0x00001003;

/**
* - Name:         Double tap latency 
* - Type:         TInt
* - Scope:        Channel property
* - Mandatory:    No
* - Capability:   None
* - Description:  Double tap latency in milliseconds
*/
const TSensrvPropertyId KSensrvPropIdDblTapLatency = 0x00001004;

/**
* - Name:         Double tap interval
* - Type:         TInt
* - Scope:        Channel property
* - Mandatory:    No
* - Capability:   None
* - Description:  Double tap interval in milliseconds
*/
const TSensrvPropertyId KSensrvPropIdDblTapInterval = 0x00001005;



// TAPPING RELATED DATATYPES

/**
* Direction of the tapping data. If direction (plus or minus) is not known, 
* direction is, for example in x-axis case 
* KSensrvAccelerometerDirectionXplus | KSensrvAccelerometerDirectionXminus
*/
const TUint8 KSensrvAccelerometerDirectionXplus 	= 0x01;
const TUint8 KSensrvAccelerometerDirectionXminus 	= 0x02;
const TUint8 KSensrvAccelerometerDirectionYplus 	= 0x04;
const TUint8 KSensrvAccelerometerDirectionYminus 	= 0x08;
const TUint8 KSensrvAccelerometerDirectionZplus 	= 0x10;
const TUint8 KSensrvAccelerometerDirectionZminus 	= 0x20;

/**
* Tapping data type
*/
class TSensrvTappingData 
    {
public:
    /**
    * Channel data type Id number
    */      
    static const TSensrvChannelDataTypeId KDataTypeId = 0x1020507F;

	/**
    * Channel data type index numbers
    */
	enum TSensrvAccelerometerAxisDataIndexes
          {
          ETimeStamp = 0,
          EDirection
          };

public:
    /**
    * - Item name:   Sampling time.
    * - Item Index:  0
    * - Conditions:  None
    * - Description: Timestamp for a sample.
    */   
    TTime iTimeStamp;    
    
    /**
    * - Item name:   Tapping direction bitmask
    * - Item Index:  1
    * - Conditions:  Binary
    * - Description: Direction bitmask of the tapping event. 
    *                See constant definitions above.
    */
    TUint32 iDirection;  
    };

#endif //SSYREFERENCEACCELEROMETER_H
