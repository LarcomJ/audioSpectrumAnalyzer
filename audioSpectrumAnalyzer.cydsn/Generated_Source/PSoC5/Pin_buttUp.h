/*******************************************************************************
* File Name: Pin_buttUp.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_buttUp_H) /* Pins Pin_buttUp_H */
#define CY_PINS_Pin_buttUp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_buttUp_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_buttUp__PORT == 15 && ((Pin_buttUp__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pin_buttUp_Write(uint8 value);
void    Pin_buttUp_SetDriveMode(uint8 mode);
uint8   Pin_buttUp_ReadDataReg(void);
uint8   Pin_buttUp_Read(void);
void    Pin_buttUp_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin_buttUp_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin_buttUp_SetDriveMode() function.
     *  @{
     */
        #define Pin_buttUp_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin_buttUp_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin_buttUp_DM_RES_UP          PIN_DM_RES_UP
        #define Pin_buttUp_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin_buttUp_DM_OD_LO           PIN_DM_OD_LO
        #define Pin_buttUp_DM_OD_HI           PIN_DM_OD_HI
        #define Pin_buttUp_DM_STRONG          PIN_DM_STRONG
        #define Pin_buttUp_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pin_buttUp_MASK               Pin_buttUp__MASK
#define Pin_buttUp_SHIFT              Pin_buttUp__SHIFT
#define Pin_buttUp_WIDTH              1u

/* Interrupt constants */
#if defined(Pin_buttUp__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin_buttUp_SetInterruptMode() function.
     *  @{
     */
        #define Pin_buttUp_INTR_NONE      (uint16)(0x0000u)
        #define Pin_buttUp_INTR_RISING    (uint16)(0x0001u)
        #define Pin_buttUp_INTR_FALLING   (uint16)(0x0002u)
        #define Pin_buttUp_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pin_buttUp_INTR_MASK      (0x01u) 
#endif /* (Pin_buttUp__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_buttUp_PS                     (* (reg8 *) Pin_buttUp__PS)
/* Data Register */
#define Pin_buttUp_DR                     (* (reg8 *) Pin_buttUp__DR)
/* Port Number */
#define Pin_buttUp_PRT_NUM                (* (reg8 *) Pin_buttUp__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_buttUp_AG                     (* (reg8 *) Pin_buttUp__AG)                       
/* Analog MUX bux enable */
#define Pin_buttUp_AMUX                   (* (reg8 *) Pin_buttUp__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_buttUp_BIE                    (* (reg8 *) Pin_buttUp__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_buttUp_BIT_MASK               (* (reg8 *) Pin_buttUp__BIT_MASK)
/* Bypass Enable */
#define Pin_buttUp_BYP                    (* (reg8 *) Pin_buttUp__BYP)
/* Port wide control signals */                                                   
#define Pin_buttUp_CTL                    (* (reg8 *) Pin_buttUp__CTL)
/* Drive Modes */
#define Pin_buttUp_DM0                    (* (reg8 *) Pin_buttUp__DM0) 
#define Pin_buttUp_DM1                    (* (reg8 *) Pin_buttUp__DM1)
#define Pin_buttUp_DM2                    (* (reg8 *) Pin_buttUp__DM2) 
/* Input Buffer Disable Override */
#define Pin_buttUp_INP_DIS                (* (reg8 *) Pin_buttUp__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_buttUp_LCD_COM_SEG            (* (reg8 *) Pin_buttUp__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_buttUp_LCD_EN                 (* (reg8 *) Pin_buttUp__LCD_EN)
/* Slew Rate Control */
#define Pin_buttUp_SLW                    (* (reg8 *) Pin_buttUp__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_buttUp_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_buttUp__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_buttUp_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_buttUp__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_buttUp_PRTDSI__OE_SEL0        (* (reg8 *) Pin_buttUp__PRTDSI__OE_SEL0) 
#define Pin_buttUp_PRTDSI__OE_SEL1        (* (reg8 *) Pin_buttUp__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_buttUp_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_buttUp__PRTDSI__OUT_SEL0) 
#define Pin_buttUp_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_buttUp__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_buttUp_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_buttUp__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pin_buttUp__SIO_CFG)
    #define Pin_buttUp_SIO_HYST_EN        (* (reg8 *) Pin_buttUp__SIO_HYST_EN)
    #define Pin_buttUp_SIO_REG_HIFREQ     (* (reg8 *) Pin_buttUp__SIO_REG_HIFREQ)
    #define Pin_buttUp_SIO_CFG            (* (reg8 *) Pin_buttUp__SIO_CFG)
    #define Pin_buttUp_SIO_DIFF           (* (reg8 *) Pin_buttUp__SIO_DIFF)
#endif /* (Pin_buttUp__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin_buttUp__INTSTAT)
    #define Pin_buttUp_INTSTAT            (* (reg8 *) Pin_buttUp__INTSTAT)
    #define Pin_buttUp_SNAP               (* (reg8 *) Pin_buttUp__SNAP)
    
	#define Pin_buttUp_0_INTTYPE_REG 		(* (reg8 *) Pin_buttUp__0__INTTYPE)
#endif /* (Pin_buttUp__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_buttUp_H */


/* [] END OF FILE */
