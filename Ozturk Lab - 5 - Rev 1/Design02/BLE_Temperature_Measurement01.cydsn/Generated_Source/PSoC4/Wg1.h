/*******************************************************************************
* File Name: Wg1.h  
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

#if !defined(CY_PINS_Wg1_H) /* Pins Wg1_H */
#define CY_PINS_Wg1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Wg1_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Wg1_Write(uint8 value) ;
void    Wg1_SetDriveMode(uint8 mode) ;
uint8   Wg1_ReadDataReg(void) ;
uint8   Wg1_Read(void) ;
uint8   Wg1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Wg1_DRIVE_MODE_BITS        (3)
#define Wg1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Wg1_DRIVE_MODE_BITS))

#define Wg1_DM_ALG_HIZ         (0x00u)
#define Wg1_DM_DIG_HIZ         (0x01u)
#define Wg1_DM_RES_UP          (0x02u)
#define Wg1_DM_RES_DWN         (0x03u)
#define Wg1_DM_OD_LO           (0x04u)
#define Wg1_DM_OD_HI           (0x05u)
#define Wg1_DM_STRONG          (0x06u)
#define Wg1_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Wg1_MASK               Wg1__MASK
#define Wg1_SHIFT              Wg1__SHIFT
#define Wg1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Wg1_PS                     (* (reg32 *) Wg1__PS)
/* Port Configuration */
#define Wg1_PC                     (* (reg32 *) Wg1__PC)
/* Data Register */
#define Wg1_DR                     (* (reg32 *) Wg1__DR)
/* Input Buffer Disable Override */
#define Wg1_INP_DIS                (* (reg32 *) Wg1__PC2)


#if defined(Wg1__INTSTAT)  /* Interrupt Registers */

    #define Wg1_INTSTAT                (* (reg32 *) Wg1__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Wg1_DRIVE_MODE_SHIFT       (0x00u)
#define Wg1_DRIVE_MODE_MASK        (0x07u << Wg1_DRIVE_MODE_SHIFT)


#endif /* End Pins Wg1_H */


/* [] END OF FILE */
