/*******************************************************************************
* File Name: SPI_led_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_led_PVT.h"

static SPI_led_BACKUP_STRUCT SPI_led_backup =
{
    SPI_led_DISABLED,
    SPI_led_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_led_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_led_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_led_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_led_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_led_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_led_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_led_Sleep(void) 
{
    /* Save components enable state */
    SPI_led_backup.enableState = ((uint8) SPI_led_IS_ENABLED);

    SPI_led_Stop();
}


/*******************************************************************************
* Function Name: SPI_led_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_led_backup - used when non-retention registers are restored.
*  SPI_led_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_led_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_led_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_led_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_led_Wakeup(void) 
{
    #if(SPI_led_RX_SOFTWARE_BUF_ENABLED)
        SPI_led_rxBufferFull  = 0u;
        SPI_led_rxBufferRead  = 0u;
        SPI_led_rxBufferWrite = 0u;
    #endif /* (SPI_led_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_led_TX_SOFTWARE_BUF_ENABLED)
        SPI_led_txBufferFull  = 0u;
        SPI_led_txBufferRead  = 0u;
        SPI_led_txBufferWrite = 0u;
    #endif /* (SPI_led_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_led_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_led_backup.enableState)
    {
        SPI_led_Enable();
    }
}


/* [] END OF FILE */
