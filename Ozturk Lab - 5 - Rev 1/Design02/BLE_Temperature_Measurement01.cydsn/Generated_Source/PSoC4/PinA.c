/*******************************************************************************
* File Name: PinA.c  
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
#include "PinA.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        PinA_PC =   (PinA_PC & \
                                (uint32)(~(uint32)(PinA_DRIVE_MODE_IND_MASK << (PinA_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (PinA_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: PinA_Write
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
void PinA_Write(uint8 value) 
{
    uint8 drVal = (uint8)(PinA_DR & (uint8)(~PinA_MASK));
    drVal = (drVal | ((uint8)(value << PinA_SHIFT) & PinA_MASK));
    PinA_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: PinA_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  PinA_DM_STRONG     Strong Drive 
*  PinA_DM_OD_HI      Open Drain, Drives High 
*  PinA_DM_OD_LO      Open Drain, Drives Low 
*  PinA_DM_RES_UP     Resistive Pull Up 
*  PinA_DM_RES_DWN    Resistive Pull Down 
*  PinA_DM_RES_UPDWN  Resistive Pull Up/Down 
*  PinA_DM_DIG_HIZ    High Impedance Digital 
*  PinA_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void PinA_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(PinA__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: PinA_Read
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
*  Macro PinA_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 PinA_Read(void) 
{
    return (uint8)((PinA_PS & PinA_MASK) >> PinA_SHIFT);
}


/*******************************************************************************
* Function Name: PinA_ReadDataReg
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
uint8 PinA_ReadDataReg(void) 
{
    return (uint8)((PinA_DR & PinA_MASK) >> PinA_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(PinA_INTSTAT) 

    /*******************************************************************************
    * Function Name: PinA_ClearInterrupt
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
    uint8 PinA_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(PinA_INTSTAT & PinA_MASK);
		PinA_INTSTAT = maskedStatus;
        return maskedStatus >> PinA_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
