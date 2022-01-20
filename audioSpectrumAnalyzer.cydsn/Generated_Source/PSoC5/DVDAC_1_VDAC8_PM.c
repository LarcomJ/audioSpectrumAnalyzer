/*******************************************************************************
* File Name: DVDAC_1_VDAC8_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DVDAC_1_VDAC8.h"

static DVDAC_1_VDAC8_backupStruct DVDAC_1_VDAC8_backup;


/*******************************************************************************
* Function Name: DVDAC_1_VDAC8_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void DVDAC_1_VDAC8_SaveConfig(void) 
{
    if (!((DVDAC_1_VDAC8_CR1 & DVDAC_1_VDAC8_SRC_MASK) == DVDAC_1_VDAC8_SRC_UDB))
    {
        DVDAC_1_VDAC8_backup.data_value = DVDAC_1_VDAC8_Data;
    }
}


/*******************************************************************************
* Function Name: DVDAC_1_VDAC8_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void DVDAC_1_VDAC8_RestoreConfig(void) 
{
    if (!((DVDAC_1_VDAC8_CR1 & DVDAC_1_VDAC8_SRC_MASK) == DVDAC_1_VDAC8_SRC_UDB))
    {
        if((DVDAC_1_VDAC8_Strobe & DVDAC_1_VDAC8_STRB_MASK) == DVDAC_1_VDAC8_STRB_EN)
        {
            DVDAC_1_VDAC8_Strobe &= (uint8)(~DVDAC_1_VDAC8_STRB_MASK);
            DVDAC_1_VDAC8_Data = DVDAC_1_VDAC8_backup.data_value;
            DVDAC_1_VDAC8_Strobe |= DVDAC_1_VDAC8_STRB_EN;
        }
        else
        {
            DVDAC_1_VDAC8_Data = DVDAC_1_VDAC8_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: DVDAC_1_VDAC8_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  DVDAC_1_VDAC8_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void DVDAC_1_VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(DVDAC_1_VDAC8_ACT_PWR_EN == (DVDAC_1_VDAC8_PWRMGR & DVDAC_1_VDAC8_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        DVDAC_1_VDAC8_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        DVDAC_1_VDAC8_backup.enableState = 0u;
    }
    
    DVDAC_1_VDAC8_Stop();
    DVDAC_1_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: DVDAC_1_VDAC8_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  DVDAC_1_VDAC8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DVDAC_1_VDAC8_Wakeup(void) 
{
    DVDAC_1_VDAC8_RestoreConfig();
    
    if(DVDAC_1_VDAC8_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        DVDAC_1_VDAC8_Enable();

        /* Restore the data register */
        DVDAC_1_VDAC8_SetValue(DVDAC_1_VDAC8_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
