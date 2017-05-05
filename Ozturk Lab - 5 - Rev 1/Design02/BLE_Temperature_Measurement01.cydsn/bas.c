/*******************************************************************************
* File Name: bas.c
*
* Version: 1.0
*
* Description:
*  This file contains BAS callback handler function.
*
* Hardware Dependency:
*  CY8CKIT-042 BLE
* 
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "common.h"
#include "bas.h"

uint16 batterySimulation = 0u;
uint16 batteryMeasure = 0u;


/*******************************************************************************
* Function Name: BasCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive service specific events from 
*   Battery Service.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
* Return:
*  None.
*
********************************************************************************/
void BasCallBack(uint32 event, void *eventParam)
{
    uint8 locServiceIndex;
    
    locServiceIndex = ((CYBLE_BAS_CHAR_VALUE_T *)eventParam)->serviceIndex;
    printf("BAS event: %lx, ", event);
    
    switch(event)
    {
        case CYBLE_EVT_BASS_NOTIFICATION_ENABLED:
            printf("EVT_BASS_NOTIFICATION_ENABLED: %x \r\n", locServiceIndex);
            if(BAS_SERVICE_SIMULATE == locServiceIndex)
            {
                batterySimulation = ENABLED;
            }
            if(BAS_SERVICE_MEASURE == locServiceIndex)
            {
                batteryMeasure = ENABLED;
            }
            break;
                
        case CYBLE_EVT_BASS_NOTIFICATION_DISABLED:
            printf("EVT_BASS_NOTIFICATION_DISABLED: %x \r\n", locServiceIndex);
            if(BAS_SERVICE_SIMULATE == locServiceIndex)
            {
                batterySimulation = DISABLED;
            }
            if(BAS_SERVICE_MEASURE == locServiceIndex)
            {
                batteryMeasure = DISABLED;
            }
            break;
        case CYBLE_EVT_BASC_NOTIFICATION:
            break;
        case CYBLE_EVT_BASC_READ_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_READ_DESCR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_WRITE_DESCR_RESPONSE:
            break;
		default:
            printf("Not supported event\r\n");
			break;
    }
}


/*******************************************************************************
* Function Name: MeasureBattery()
********************************************************************************
*
* Summary:
*   This function measures the battery voltage and sends it to the client.
*
*******************************************************************************/
void MeasureBattery(void)
{
    Disconnect_LED_Write(LED_ON);
	//int16 adcResult;
    //int32 mvolts;
	//uint32 sarControlReg;
    uint8 batteryLevel = 0x05;
    //uint8 button;
    // - CYBLE_API_RESULT_T apiResult;
    //SimulateBattery();
    
    //static uint32 batteryTimer = BATTERY_TIMEOUT;
    
    
    
    
        //batteryTimer = BATTERY_TIMEOUT;
        
        
        
        //batteryLevel = 0x00; //(batteryLevel+1)%11;
        
        
        if(batteryLevel < LOW_BATTERY_LIMIT)
        {
            LowPower_LED_Write(LED_ON);
        }
        else
        {
            LowPower_LED_Write(LED_OFF);
        }

        /* Update Battery Level characteristic value */
        //apiResult = CyBle_BassSendNotification(cyBle_connHandle, BAS_SERVICE_MEASURE, CYBLE_BAS_BATTERY_LEVEL, 
        //                sizeof(batteryLevel), &batteryLevel);
        //if(apiResult != CYBLE_ERROR_OK)
        //{
        //    printf("BassSendNotification API Error: %x \r\n", apiResult);
        //    batteryMeasure = DISABLED;
        //}
        //else
        //{
        //    printf("MeasureBatteryLevelUpdate: %d \r\n",batteryLevel);
        //}
        
        Disconnect_LED_Write(LED_OFF);
    
    
}


/*******************************************************************************
* Function Name: SimulateBattery()
********************************************************************************
*
* Summary:
*   The custom function to simulate Battery Voltage.
*
*******************************************************************************/
void SimulateBattery(void)
{
    static uint32 batteryTimer = BATTERY_TIMEOUT;
    static uint8 batteryLevel;
    double scale;
    double scale2;
    //uint32 sarControlReg;
    int16 adcResult;
    int32 adcmVolts;
    CYBLE_API_RESULT_T apiResult;
    
    ADC_StartConvert();
    
    
    if((CyBle_GattGetBusyStatus() == CYBLE_STACK_STATE_FREE) && (--batteryTimer == 0u))
    {
        Disconnect_LED_Write(LED_ON);
        
        batteryTimer = BATTERY_TIMEOUT;
        
         //ADC Conversion
        
        //if(ADC_IsEndConversion(ADC_RETURN_STATUS) !=0)
        //{
        //    adcResult = ADC_GetResult16(0x00);
            
        //    adcmVolts = ADC_CountsTo_Volts(0x00,adcResult);
            
           
        //}
        
        /* Set the reference to VBG and enable reference bypass */
    	///sarControlReg = ADC_SAR_CTRL_REG & ~ADC_VREF_MASK;
    	//ADC_SAR_CTRL_REG = sarControlReg | ADC_VREF_INTERNAL1024BYPASSED;
    	
    	/* 25 ms delay for reference capacitor to charge */
    	//CyDelay(25);             
    	
    	/* Set the reference to VDD and disable reference bypass */
    	//sarControlReg = ADC_SAR_CTRL_REG & ~ADC_VREF_MASK;
    	//ADC_SAR_CTRL_REG = sarControlReg | ADC_VREF_VDDA;
        
        adcResult = ADC_GetResult16(0x00);
    	/* Calculate input voltage by using ratio of ADC counts from reference
    	*  and ADC Full Scale counts. 
        */
    	adcmVolts = ADC_CountsTo_mVolts(0x00,adcResult);
        
        //adcmVolts = adcmVolts * 25;
        
        
        /* Battery Level simulation */
        if(batterySimulation == ENABLED)
        {
           // batteryLevel += SIM_BATTERY_INCREMENT;
           // if(batteryLevel > SIM_BATTERY_MAX)
           // {
           //     batteryLevel = SIM_BATTERY_MIN; 
           // }
            
            if(adcmVolts < ZeroMax)
            {
                scale = ZeroMax - 0;
                scale2 = ((adcmVolts) - 0)/scale;
                batteryLevel = 0 + scale2 * 10;
            }
            else if(adcmVolts < OneMax)
            {
                scale = OneMax - ZeroMax;
                scale2 = ((adcmVolts) - ZeroMax)/scale;
                batteryLevel = 10 + scale2 * 10;
            }
            else if(adcmVolts < TwoMax)
            {
                scale = TwoMax - OneMax;
                scale2 = ((adcmVolts) - OneMax)/scale;
                batteryLevel = 20 + scale2 * 10;
            }
            else if(adcmVolts < ThreeMax)
            {
                scale = ThreeMax - TwoMax;
                scale2 = ((adcmVolts) - TwoMax)/scale;
                batteryLevel = 30 + scale2 * 10;
            }
            else if(adcmVolts < FourMax)
            {
                scale = FourMax - ThreeMax;
                scale2 = ((adcmVolts) - ThreeMax)/scale;
                batteryLevel = 40 + scale2 * 10;
            }
            else if(adcmVolts < FiveMax)
            {
                scale = FiveMax - FourMax;
                scale2 = ((adcmVolts) - FourMax)/scale;
                batteryLevel = 50 + scale2 * 10;
            }
            else if(adcmVolts < SixMax)
            {
                scale = SixMax - FiveMax;
                scale2 = ((adcmVolts) - FiveMax)/scale;
                batteryLevel = 60 + scale2 * 10;
            }
            else if(adcmVolts < SevenMax)
            {
                scale = SevenMax - SixMax;
                scale2 = ((adcmVolts) - SixMax)/scale;
                batteryLevel = 70 + scale2 * 10;
            }
            else if(adcmVolts < EightMax)
            {
                scale = EightMax - SevenMax;
                scale2 = ((adcmVolts) - SevenMax)/scale;
                batteryLevel = 80 + scale2 * 10;
            }
            else if(adcmVolts < NineMax)
            {
                scale = NineMax - EightMax;
                scale2 = ((adcmVolts) - EightMax)/scale;
                batteryLevel = 90 + scale2 * 10;
            }
            else if(adcmVolts < TenMax)
            {
                batteryLevel = 0x0A;
            }
            else
            {
                batteryLevel = adcmVolts;
            }
        }
        if(batteryLevel < LOW_BATTERY_LIMIT)
            {
                //LowPower_LED_Write(LED_ON);
            }
        else
        {
            //LowPower_LED_Write(LED_OFF);
        }
        
        

        /* Update Battery Level characteristic value */
         apiResult = CyBle_BassSendNotification(cyBle_connHandle, BAS_SERVICE_SIMULATE, CYBLE_BAS_BATTERY_LEVEL, 
                            sizeof(batteryLevel), &batteryLevel);
        
        if(apiResult != CYBLE_ERROR_OK)
        {
            printf("BassSendNotification API Error: %x \r\n", apiResult);
            batterySimulation = DISABLED;
        }
        else
        {
            printf("SimulBatteryLevelUpdate: %d \r\n",batteryLevel);
        }
        
        Disconnect_LED_Write(LED_OFF);
    }
}



/* [] END OF FILE */
