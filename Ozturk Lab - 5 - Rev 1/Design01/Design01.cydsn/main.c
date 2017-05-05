/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    int16 adcResult;
    float adcVolts;
    char tmpStr[25];
    
    ADC_Start();
    ADC_StartConvert();
    
    
    for(;;)
    {
        /* Place your application code here. */
        
        if(ADC_IsEndConversion(ADC_RETURN_STATUS) !=0)
        {
            adcResult = ADC_GetResult16(0x00);
            adcVolts = ADC_CountsTo_Volts(0x00,adcResult);
            sprintf(tmpStr,"%+1.3f Volts", adcVolts);
            
            
            
        }
    }
}

/* [] END OF FILE */
