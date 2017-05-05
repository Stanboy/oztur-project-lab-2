/*******************************************************************************
* File Name: Analog.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Analog.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Analog_PC =   (Analog_PC & \
                                (uint32)(~(uint32)(Analog_DRIVE_MODE_IND_MASK << (Analog_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Analog_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: Analog_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void Analog_Write(uint8 value) 
{
    uint8 drVal = (uint8)(Analog_DR & (uint8)(~Analog_MASK));
    drVal = (drVal | ((uint8)(value << Analog_SHIFT) & Analog_MASK));
    Analog_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: Analog_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  Analog_DM_STRONG     Strong Drive 
*  Analog_DM_OD_HI      Open Drain, Drives High 
*  Analog_DM_OD_LO      Open Drain, Drives Low 
*  Analog_DM_RES_UP     Resistive Pull Up 
*  Analog_DM_RES_DWN    Resistive Pull Down 
*  Analog_DM_RES_UPDWN  Resistive Pull Up/Down 
*  Analog_DM_DIG_HIZ    High Impedance Digital 
*  Analog_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void Analog_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(Analog__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: Analog_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Analog_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Analog_Read(void) 
{
    return (uint8)((Analog_PS & Analog_MASK) >> Analog_SHIFT);
}


/*******************************************************************************
* Function Name: Analog_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Analog_ReadDataReg(void) 
{
    return (uint8)((Analog_DR & Analog_MASK) >> Analog_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Analog_INTSTAT) 

    /*******************************************************************************
    * Function Name: Analog_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Analog_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(Analog_INTSTAT & Analog_MASK);
		Analog_INTSTAT = maskedStatus;
        return maskedStatus >> Analog_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
