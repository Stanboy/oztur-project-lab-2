/*******************************************************************************
* File Name: PinA.h  
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

#if !defined(CY_PINS_PinA_H) /* Pins PinA_H */
#define CY_PINS_PinA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "PinA_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    PinA_Write(uint8 value) ;
void    PinA_SetDriveMode(uint8 mode) ;
uint8   PinA_ReadDataReg(void) ;
uint8   PinA_Read(void) ;
uint8   PinA_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PinA_DRIVE_MODE_BITS        (3)
#define PinA_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - PinA_DRIVE_MODE_BITS))

#define PinA_DM_ALG_HIZ         (0x00u)
#define PinA_DM_DIG_HIZ         (0x01u)
#define PinA_DM_RES_UP          (0x02u)
#define PinA_DM_RES_DWN         (0x03u)
#define PinA_DM_OD_LO           (0x04u)
#define PinA_DM_OD_HI           (0x05u)
#define PinA_DM_STRONG          (0x06u)
#define PinA_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define PinA_MASK               PinA__MASK
#define PinA_SHIFT              PinA__SHIFT
#define PinA_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PinA_PS                     (* (reg32 *) PinA__PS)
/* Port Configuration */
#define PinA_PC                     (* (reg32 *) PinA__PC)
/* Data Register */
#define PinA_DR                     (* (reg32 *) PinA__DR)
/* Input Buffer Disable Override */
#define PinA_INP_DIS                (* (reg32 *) PinA__PC2)


#if defined(PinA__INTSTAT)  /* Interrupt Registers */

    #define PinA_INTSTAT                (* (reg32 *) PinA__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define PinA_DRIVE_MODE_SHIFT       (0x00u)
#define PinA_DRIVE_MODE_MASK        (0x07u << PinA_DRIVE_MODE_SHIFT)


#endif /* End Pins PinA_H */


/* [] END OF FILE */
