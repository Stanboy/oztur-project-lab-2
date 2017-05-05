/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  Temperature measurement example project that simulates thermometer readings
*  and sends it over BLE Health Thermometer service.
*
* Note:
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
#include "bas.h"
#include "stdio.h"

uint8 busStatus = CYBLE_STACK_STATE_FREE;           /* Status of stack queue */

volatile uint32 mainTimer = 0;



/*******************************************************************************
* Function Name: AppCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive events from the BLE Component.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
*******************************************************************************/
void AppCallBack(uint32 event, void* eventParam)
{
    CYBLE_API_RESULT_T apiResult;
    CYBLE_GAP_BD_ADDR_T localAddr;
    CYBLE_GAP_AUTH_INFO_T *authInfo;
    uint8 i;

    switch (event)
	{
        /**********************************************************
        *                       General Events
        ***********************************************************/
		case CYBLE_EVT_STACK_ON: /* This event received when component is Started */
            /* Enter in to discoverable mode so that remote can search it. */
            apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            if(apiResult != CYBLE_ERROR_OK)
            {
                printf("StartAdvertisement API Error: %d \r\n", apiResult);
            }
            printf("Bluetooth On, StartAdvertisement with addr: ");
            CyBle_GetDeviceAddress(&localAddr);
            for(i = CYBLE_GAP_BD_ADDR_SIZE; i > 0u; i--)
            {
                printf("%2.2x", localAddr.bdAddr[i-1]);
            }
            printf("\r\n");
            break;
		case CYBLE_EVT_TIMEOUT: 
        	/* CYBLE_GAP_ADV_MODE_TO - Advertisement time set by application is expired */
    	    /* CYBLE_GAP_AUTH_TO - Authentication procedure timeout */
            /* CYBLE_GAP_SCAN_TO - Scan time set by application is expired */
            /* CYBLE_GATT_RSP_TO - GATT procedure timeout */
            printf("CYBLE_EVT_TIMEOUT: %x \r\n", *(uint8 *)eventParam);
			break;
		case CYBLE_EVT_HARDWARE_ERROR:    /* This event indicates that some internal HW error has occurred. */
            printf("Hardware Error \r\n");
			break;
        case CYBLE_EVT_HCI_STATUS:
            printf("EVT_HCI_STATUS: %x \r\n", *(uint8 *)eventParam);
			break;
        
        /**********************************************************
        *                       GAP Events
        ***********************************************************/
        case CYBLE_EVT_GAP_AUTH_REQ:
            printf("EVT_AUTH_REQ: security=%x, bonding=%x, ekeySize=%x, err=%x \r\n", 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).security, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).bonding, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).ekeySize, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).authErr);
            break;
        case CYBLE_EVT_GAP_PASSKEY_ENTRY_REQUEST:
            printf("EVT_PASSKEY_ENTRY_REQUEST \r\n");
            break;
        case CYBLE_EVT_GAP_PASSKEY_DISPLAY_REQUEST:
            printf("EVT_PASSKEY_DISPLAY_REQUEST %6.6ld \r\n", *(uint32 *)eventParam);
            break;
        case CYBLE_EVT_GAP_AUTH_COMPLETE:
            authInfo = (CYBLE_GAP_AUTH_INFO_T *)eventParam;
            printf("AUTH_COMPLETE: security:%x, bonding:%x, ekeySize:%x, authErr %x \r\n", 
                                    authInfo->security, authInfo->bonding, authInfo->ekeySize, authInfo->authErr);
            break;
        case CYBLE_EVT_GAP_AUTH_FAILED:
            printf("EVT_AUTH_FAILED: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            printf("CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP, state: %x\r\n", CyBle_GetState());
            if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
            {   
                /* Fast and slow advertising period complete, go to low power  
                 * mode (Hibernate mode) and wait for an external
                 * user event to wake up the device again */
                printf("Hibernate \r\n");
                Advertising_LED_Write(LED_OFF);
                Disconnect_LED_Write(LED_ON);
                LowPower_LED_Write(LED_OFF);
                while((UART_DEB_SpiUartGetTxBufferSize() + UART_DEB_GET_TX_FIFO_SR_VALID) != 0);
                SW2_ClearInterrupt();
                Wakeup_Interrupt_ClearPending();
                Wakeup_Interrupt_Start();
                CySysPmHibernate();
            }
            break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            printf("CYBLE_EVT_GAP_DEVICE_CONNECTED: %x \r\n", *(uint8 *)eventParam);
            Advertising_LED_Write(LED_OFF);
            break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            printf("CYBLE_EVT_GAP_DEVICE_DISCONNECTED\r\n");
            /* Put the device to discoverable mode so that remote can search it. */
            apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            if(apiResult != CYBLE_ERROR_OK)
            {
                printf("StartAdvertisement API Error: %x \r\n", apiResult);
            }
            break;
        case CYBLE_EVT_GAP_ENCRYPT_CHANGE:
            printf("ENCRYPT_CHANGE: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAPC_CONNECTION_UPDATE_COMPLETE:
            printf("EVT_CONNECTION_UPDATE_COMPLETE: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT:
            printf("CYBLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT \r\n");
            break;
            
        /**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
            printf("EVT_GATT_CONNECT_IND: %x, %x \r\n", cyBle_connHandle.attId, cyBle_connHandle.bdHandle);
            break;
        case CYBLE_EVT_GATT_DISCONNECT_IND:
            printf("EVT_GATT_DISCONNECT_IND \r\n");
            break;
        case CYBLE_EVT_GATTS_WRITE_REQ:
            printf("EVT_GATT_WRITE_REQ: %x = ",((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam)->handleValPair.attrHandle);
            ShowValue(&((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam)->handleValPair.value);
            (void)CyBle_GattsWriteRsp(((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam)->connHandle);
            break;
        case CYBLE_EVT_GATTS_INDICATION_ENABLED:
            printf("CYBLE_EVT_GATTS_INDICATION_ENABLED \r\n");
            break;
        case CYBLE_EVT_GATTS_INDICATION_DISABLED:
            printf("CYBLE_EVT_GATTS_INDICATION_DISABLED \r\n");
            break;
            
		default:
        #if (0)
            printf("OTHER event: %lx \r\n", event);
        #endif
			break;
	}
}


/*******************************************************************************
* Function Name: Timer_Interrupt
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the WDT timer.
*
*******************************************************************************/
CY_ISR(Timer_Interrupt)
{
    if(CySysWdtGetInterruptSource() & WDT_INTERRUPT_SOURCE)
    {
        static uint8 led = LED_OFF;
        
        /* Blink LED to indicate that device advertises */
        if(CYBLE_STATE_ADVERTISING == CyBle_GetState())
        {
            led ^= LED_OFF;
            Advertising_LED_Write(led);
        }
        
        /* Indicate that timer is raised to the main loop */
        mainTimer++;
        
        /* Clears interrupt request  */
        CySysWdtClearInterrupt(WDT_INTERRUPT_SOURCE);
    }
}

/*******************************************************************************
* Function Name: WDT_Start
********************************************************************************
*
* Summary:
*  Configures WDT to trigger an interrupt every second.
*
*******************************************************************************/

void WDT_Start(void)
{
    /* Unlock the WDT registers for modification */
    CySysWdtUnlock(); 
    /* Setup ISR */
    WDT_Interrupt_StartEx(&Timer_Interrupt);
    /* Write the mode to generate interrupt on match */
    CySysWdtWriteMode(WDT_COUNTER, CY_SYS_WDT_MODE_INT);
    /* Configure the WDT counter clear on a match setting */
    CySysWdtWriteClearOnMatch(WDT_COUNTER, WDT_COUNTER_ENABLE);
    /* Configure the WDT counter match comparison value */
    CySysWdtWriteMatch(WDT_COUNTER, WDT_1SEC);
    /* Reset WDT counter */
    CySysWdtResetCounters(WDT_COUNTER);
    /* Enable the specified WDT counter */
    CySysWdtEnable(WDT_COUNTER_MASK);
    /* Lock out configuration changes to the Watchdog timer registers */
    CySysWdtLock();    
}


/*******************************************************************************
* Function Name: WDT_Stop
********************************************************************************
*
* Summary:
*  This API stops the WDT timer.
*
*******************************************************************************/
void WDT_Stop(void)
{
    /* Unlock the WDT registers for modification */
    CySysWdtUnlock(); 
    /* Disable the specified WDT counter */
    CySysWdtDisable(WDT_COUNTER_MASK);
    /* Locks out configuration changes to the Watchdog timer registers */
    CySysWdtLock();    
}


int main()
{
    
   
    
    
    CYBLE_LP_MODE_T lpMode;
    CYBLE_BLESS_STATE_T blessState;
  
    CyGlobalIntEnable;  
    
    UART_DEB_Start(); 
    printf("BLE Temperature Measurement Example Project \r\n");

    Disconnect_LED_Write(LED_OFF);
    Advertising_LED_Write(LED_OFF);
    LowPower_LED_Write(LED_OFF);

    
    /* Start CYBLE component and register generic event handler */
    CyBle_Start(AppCallBack);
    /* Register service specific callback functions */
    CyBle_BasRegisterAttrCallback(BasCallBack);
    CyBle_HtsRegisterAttrCallback(HtsCallBack);
    
	
    ADC_Start();
    ADC_StartConvert();
    
   
    WDT_Start();

    /***************************************************************************
    * Main polling loop
    ***************************************************************************/
	while(1) 
    {   
        if(CyBle_GetState() != CYBLE_STATE_INITIALIZING)
        {
            /* Enter DeepSleep mode between connection intervals */
            lpMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
            CyGlobalIntDisable;
            blessState = CyBle_GetBleSsState();

            if(lpMode == CYBLE_BLESS_DEEPSLEEP) 
            {   
                if(blessState == CYBLE_BLESS_STATE_ECO_ON || blessState == CYBLE_BLESS_STATE_DEEPSLEEP)
                {
                    /* Put the device into the Deep Sleep mode only when all debug information has been sent */
                   if((UART_DEB_SpiUartGetTxBufferSize() + UART_DEB_GET_TX_FIFO_SR_VALID) == 0u)
                    {
                        CySysPmDeepSleep();
                    }
                    else
                    {
                        CySysPmSleep();
                    }
                }
            }
            else
            {
                if(blessState != CYBLE_BLESS_STATE_EVENT_CLOSE)
                {
                    CySysPmSleep();
                }
            }
            CyGlobalIntEnable;
        }
        
       

        
        /***********************************************************************
        * Wait for connection established with Central device
        ***********************************************************************/
        if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
        {
            /*******************************************************************
            *  Periodically measure a battery level and temperature and send 
            *  results to the Client
            *******************************************************************/        
            if(mainTimer != 0u)
            {
                mainTimer = 0u;
                if(temperatureMeasure == ENABLED)
                {
                    MeasureTemperature();
                    CyBle_ProcessEvents();
                }
                if(batterySimulation == ENABLED)
                {
                    
                    
                    //MeasureBattery();
                    
                    SimulateBattery();
                    
                    CyBle_ProcessEvents();
                }
            }
        }
        
        /*******************************************************************
        *  Processes all pending BLE events in the stack
        *******************************************************************/        
        CyBle_ProcessEvents();
	}   
}  


/* [] END OF FILE */

