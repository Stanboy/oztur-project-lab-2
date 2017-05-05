/*******************************************************************************
* File Name: Analog.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Analog_H) /* Pins Analog_H */
#define CY_PINS_Analog_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Analog_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Analog_Write(uint8 value) ;
void    Analog_SetDriveMode(uint8 mode) ;
uint8   Analog_ReadDataReg(void) ;
uint8   Analog_Read(void) ;
uint8   Analog_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Analog_DRIVE_MODE_BITS        (3)
#define Analog_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Analog_DRIVE_MODE_BITS))

#define Analog_DM_ALG_HIZ         (0x00u)
#define Analog_DM_DIG_HIZ         (0x01u)
#define Analog_DM_RES_UP          (0x02u)
#define Analog_DM_RES_DWN         (0x03u)
#define Analog_DM_OD_LO           (0x04u)
#define Analog_DM_OD_HI           (0x05u)
#define Analog_DM_STRONG          (0x06u)
#define Analog_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Analog_MASK               Analog__MASK
#define Analog_SHIFT              Analog__SHIFT
#define Analog_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Analog_PS                     (* (reg32 *) Analog__PS)
/* Port Configuration */
#define Analog_PC                     (* (reg32 *) Analog__PC)
/* Data Register */
#define Analog_DR                     (* (reg32 *) Analog__DR)
/* Input Buffer Disable Override */
#define Analog_INP_DIS                (* (reg32 *) Analog__PC2)


#if defined(Analog__INTSTAT)  /* Interrupt Registers */

    #define Analog_INTSTAT                (* (reg32 *) Analog__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Analog_DRIVE_MODE_SHIFT       (0x00u)
#define Analog_DRIVE_MODE_MASK        (0x07u << Analog_DRIVE_MODE_SHIFT)


#endif /* End Pins Analog_H */


/* [] END OF FILE */
