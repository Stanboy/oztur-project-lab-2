/*******************************************************************************
* File Name: hts.c
*
* Version: 1.0
*
* Description:
*  This file contains HTS callback handler function.
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
#include "hts.h"

uint16 temperatureMeasure = 0u;
uint32 temperatureTimer = 1u;

uint16 initialMeasurementInterval = 10;

/* Byte order: 
*   Flags
*   Temperature in float format (MSB 8 bits - exponent, LSB 24 bits - mantissa) 
*/
uint8 temp_data[HTS_TEMP_DATA_MIN_SIZE] = {0u, 0u, 0u, 0u, 0u};


/*******************************************************************************
* Function Name: HtsCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive service specific events from 
*   Health Thermometer Service.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
* Return:
*  None.
*
*******************************************************************************/
void HtsCallBack(uint32 event, void *eventParam)
{
    uint8 locCharIndex;
    locCharIndex = ((CYBLE_HTS_CHAR_VALUE_T *)eventParam)->charIndex;
    printf("HTS event: %lx, ", event);

    switch(event)
    {
        case CYBLE_EVT_HTSS_NOTIFICATION_ENABLED:
            break;
        case CYBLE_EVT_HTSS_NOTIFICATION_DISABLED:
            break;
        case CYBLE_EVT_HTSS_INDICATION_ENABLED:
            printf("EVT_HTSS_INDICATION_ENABLED: char: %x\r\n", locCharIndex);
            if(locCharIndex == CYBLE_HTS_TEMP_MEASURE)
            {
                temperatureMeasure = ENABLED;
            }
            break;
        case CYBLE_EVT_HTSS_INDICATION_DISABLED:
            printf("EVT_HTSS_INDICATION_DISABLED: char: %x\r\n", locCharIndex);
            if(locCharIndex == CYBLE_HTS_TEMP_MEASURE)
            {
                temperatureMeasure = DISABLED;
            }
            break;
        case CYBLE_EVT_HTSS_INDICATION_CONFIRMED:
            printf("EVT_HTSS_INDICATION_CONFIRMED\r\n");
            break;
        case CYBLE_EVT_HTSS_CHAR_WRITE:
            printf("EVT_HTSS_CHAR_WRITE: %x ", locCharIndex);
            ShowValue(((CYBLE_HTS_CHAR_VALUE_T *)eventParam)->value);
            if(locCharIndex == CYBLE_HTS_MEASURE_INTERVAL)
            {
                uint16 interval;
                interval = CyBle_Get16ByPtr(((CYBLE_HTS_CHAR_VALUE_T *)eventParam)->value->val);
                
                /* Store new interval value */
                initialMeasurementInterval = interval;
                if(interval != 0u)
                {
                    /* Reset the measurement interval and start the measurement process. */
                    temperatureTimer = 1u;
                    mainTimer++;
                }
            }
            break;
        case CYBLE_EVT_HTSC_NOTIFICATION:
            break;
        case CYBLE_EVT_HTSC_INDICATION:
            break;
        case CYBLE_EVT_HTSC_READ_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_WRITE_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_READ_DESCR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_WRITE_DESCR_RESPONSE:
            break;
		default:
            printf("Not supported event\r\n");
			break;
    }
}


/*******************************************************************************
* Function Name: MeasureTemperature()
********************************************************************************
*
* Summary:
*   This function measures the die temperature and sends it to the client.
*
*******************************************************************************/
void MeasureTemperature(void)
{
	int32 ADCCountsCorrected;
    double temperatureValue;
    static double temperatureCelsius;
    static uint8 measure = ENABLED; 
    int32 adcmVolts;
    int16 adcResult;
    CYBLE_API_RESULT_T apiResult;
    
    /* Do not measure temperature when 0 interval is set */
    if((initialMeasurementInterval != 0u) && (--temperatureTimer == 0u)) 
    {
        temperatureTimer = initialMeasurementInterval;
        
        /* Measure Die temperature only first time, later simulate */
        if(measure != 0u)
        {
            measure = 0u;
            /* Enables injection channel and performs a measurement. */
            ADC_EnableInjection();
        	ADC_StartConvert();
        	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
            
            /* Read conversion result and adjust data from ADC with respect to Vref value. */
        	ADCCountsCorrected = ADC_DEF_TEMP_REF * ADC_GetResult16(ADC_TEMPERATURE_CHANNEL) / ADC_DEFAULT_VREF_MV_VALUE;
       
            /* Convert the ADC output to degrees Celsius. */
           // temperatureCelsius = DieTemp_CountsTo_Celsius(ADCCountsCorrected);
            
            adcResult = ADC_GetResult16(0x00);
    	/* Calculate input voltage by using ratio of ADC counts from reference
    	*  and ADC Full Scale counts. 
        */
    	    adcmVolts = ADC_CountsTo_mVolts(0x00,adcResult);
            
            temperatureCelsius = adcmVolts / 10;

        }
        else /* Temperature Simulation */
        {
           // temperatureCelsius += SIM_TEMPERATURE_INCREMENT;
            //if(temperatureCelsius > SIM_TEMPERATURE_MAX)
            //{
             //   temperatureCelsius = SIM_TEMPERATURE_MIN; 
            //}
            
            adcResult = ADC_GetResult16(0x00);
    	/* Calculate input voltage by using ratio of ADC counts from reference
    	*  and ADC Full Scale counts. 
        */
    	    adcmVolts = ADC_CountsTo_mVolts(0x00,adcResult);
            
            temperatureCelsius = adcmVolts/10;
            
            temperatureValue = temperatureCelsius;
            
        }
        
        /* Convert Celsius to Fahrenheit if required */
        if((temp_data[0] & CYBLE_HTS_MEAS_FLAG_TEMP_UNITS_BIT) != 0u)
        {
            temperatureValue = 32 + ((temperatureCelsius * 9) / 5);
            
            
        }
        else
        {
            temperatureValue = temperatureCelsius;
        }
        
        /* Convert int32 to the IEEE-11073 FLOAT-Type.
        *  It is defined as a 32-bit value with a 24-bit mantissa and an 8-bit exponent.
        *  For the integer temperature, 24-bit value has been used with mantissa equal to 0 */
        //temperatureValue &= 0x00FFFFFF;

        /* Copy temperature to array */
        
        temperatureValue = 65;
        
        Set32ByPtr(temp_data + 1u, (double)temperatureValue);
        
        /* Send temperature to client */
        apiResult = CyBle_HtssSendIndication(cyBle_connHandle, CYBLE_HTS_TEMP_MEASURE, 
                        sizeof(temp_data), temp_data);
        if(apiResult != CYBLE_ERROR_OK)
        {
            printf("CyBle_HtssSendIndication API Error: %x \r\n", apiResult);
            temperatureMeasure = DISABLED;
        }
        else
        {
            printf("MeasureTemperature: %d %c  ", (int16)temperatureValue,
            (((temp_data[0] & CYBLE_HTS_MEAS_FLAG_TEMP_UNITS_BIT) != 0u) ? 'F' : 'C'));
        }
        
        /* Toggle the temperature unit flag on each temperature update */
        //temp_data[0] ^= CYBLE_HTS_MEAS_FLAG_TEMP_UNITS_BIT;
    }
}

/* [] END OF FILE */

