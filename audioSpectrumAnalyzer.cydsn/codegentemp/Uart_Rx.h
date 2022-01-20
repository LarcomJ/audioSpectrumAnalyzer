/*******************************************************************************
* File Name: Uart_Rx.h  
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

#if !defined(CY_PINS_Uart_Rx_H) /* Pins Uart_Rx_H */
#define CY_PINS_Uart_Rx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Uart_Rx_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Uart_Rx__PORT == 15 && ((Uart_Rx__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Uart_Rx_Write(uint8 value);
void    Uart_Rx_SetDriveMode(uint8 mode);
uint8   Uart_Rx_ReadDataReg(void);
uint8   Uart_Rx_Read(void);
void    Uart_Rx_SetInterruptMode(uint16 position, uint16 mode);
uint8   Uart_Rx_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Uart_Rx_SetDriveMode() function.
     *  @{
     */
        #define Uart_Rx_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Uart_Rx_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Uart_Rx_DM_RES_UP          PIN_DM_RES_UP
        #define Uart_Rx_DM_RES_DWN         PIN_DM_RES_DWN
        #define Uart_Rx_DM_OD_LO           PIN_DM_OD_LO
        #define Uart_Rx_DM_OD_HI           PIN_DM_OD_HI
        #define Uart_Rx_DM_STRONG          PIN_DM_STRONG
        #define Uart_Rx_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Uart_Rx_MASK               Uart_Rx__MASK
#define Uart_Rx_SHIFT              Uart_Rx__SHIFT
#define Uart_Rx_WIDTH              1u

/* Interrupt constants */
#if defined(Uart_Rx__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Uart_Rx_SetInterruptMode() function.
     *  @{
     */
        #define Uart_Rx_INTR_NONE      (uint16)(0x0000u)
        #define Uart_Rx_INTR_RISING    (uint16)(0x0001u)
        #define Uart_Rx_INTR_FALLING   (uint16)(0x0002u)
        #define Uart_Rx_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Uart_Rx_INTR_MASK      (0x01u) 
#endif /* (Uart_Rx__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Uart_Rx_PS                     (* (reg8 *) Uart_Rx__PS)
/* Data Register */
#define Uart_Rx_DR                     (* (reg8 *) Uart_Rx__DR)
/* Port Number */
#define Uart_Rx_PRT_NUM                (* (reg8 *) Uart_Rx__PRT) 
/* Connect to Analog Globals */                                                  
#define Uart_Rx_AG                     (* (reg8 *) Uart_Rx__AG)                       
/* Analog MUX bux enable */
#define Uart_Rx_AMUX                   (* (reg8 *) Uart_Rx__AMUX) 
/* Bidirectional Enable */                                                        
#define Uart_Rx_BIE                    (* (reg8 *) Uart_Rx__BIE)
/* Bit-mask for Aliased Register Access */
#define Uart_Rx_BIT_MASK               (* (reg8 *) Uart_Rx__BIT_MASK)
/* Bypass Enable */
#define Uart_Rx_BYP                    (* (reg8 *) Uart_Rx__BYP)
/* Port wide control signals */                                                   
#define Uart_Rx_CTL                    (* (reg8 *) Uart_Rx__CTL)
/* Drive Modes */
#define Uart_Rx_DM0                    (* (reg8 *) Uart_Rx__DM0) 
#define Uart_Rx_DM1                    (* (reg8 *) Uart_Rx__DM1)
#define Uart_Rx_DM2                    (* (reg8 *) Uart_Rx__DM2) 
/* Input Buffer Disable Override */
#define Uart_Rx_INP_DIS                (* (reg8 *) Uart_Rx__INP_DIS)
/* LCD Common or Segment Drive */
#define Uart_Rx_LCD_COM_SEG            (* (reg8 *) Uart_Rx__LCD_COM_SEG)
/* Enable Segment LCD */
#define Uart_Rx_LCD_EN                 (* (reg8 *) Uart_Rx__LCD_EN)
/* Slew Rate Control */
#define Uart_Rx_SLW                    (* (reg8 *) Uart_Rx__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Uart_Rx_PRTDSI__CAPS_SEL       (* (reg8 *) Uart_Rx__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Uart_Rx_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Uart_Rx__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Uart_Rx_PRTDSI__OE_SEL0        (* (reg8 *) Uart_Rx__PRTDSI__OE_SEL0) 
#define Uart_Rx_PRTDSI__OE_SEL1        (* (reg8 *) Uart_Rx__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Uart_Rx_PRTDSI__OUT_SEL0       (* (reg8 *) Uart_Rx__PRTDSI__OUT_SEL0) 
#define Uart_Rx_PRTDSI__OUT_SEL1       (* (reg8 *) Uart_Rx__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Uart_Rx_PRTDSI__SYNC_OUT       (* (reg8 *) Uart_Rx__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Uart_Rx__SIO_CFG)
    #define Uart_Rx_SIO_HYST_EN        (* (reg8 *) Uart_Rx__SIO_HYST_EN)
    #define Uart_Rx_SIO_REG_HIFREQ     (* (reg8 *) Uart_Rx__SIO_REG_HIFREQ)
    #define Uart_Rx_SIO_CFG            (* (reg8 *) Uart_Rx__SIO_CFG)
    #define Uart_Rx_SIO_DIFF           (* (reg8 *) Uart_Rx__SIO_DIFF)
#endif /* (Uart_Rx__SIO_CFG) */

/* Interrupt Registers */
#if defined(Uart_Rx__INTSTAT)
    #define Uart_Rx_INTSTAT            (* (reg8 *) Uart_Rx__INTSTAT)
    #define Uart_Rx_SNAP               (* (reg8 *) Uart_Rx__SNAP)
    
	#define Uart_Rx_0_INTTYPE_REG 		(* (reg8 *) Uart_Rx__0__INTTYPE)
#endif /* (Uart_Rx__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Uart_Rx_H */


/* [] END OF FILE */
