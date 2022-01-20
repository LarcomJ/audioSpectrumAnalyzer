/*******************************************************************************
* File Name: SPI_adc_PM.c
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

#include "SPI_adc_PVT.h"

static SPI_adc_BACKUP_STRUCT SPI_adc_backup =
{
    SPI_adc_DISABLED,
    SPI_adc_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_adc_SaveConfig
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
void SPI_adc_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_adc_RestoreConfig
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
void SPI_adc_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_adc_Sleep
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
*  SPI_adc_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_adc_Sleep(void) 
{
    /* Save components enable state */
    SPI_adc_backup.enableState = ((uint8) SPI_adc_IS_ENABLED);

    SPI_adc_Stop();
}


/*******************************************************************************
* Function Name: SPI_adc_Wakeup
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
*  SPI_adc_backup - used when non-retention registers are restored.
*  SPI_adc_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_adc_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_adc_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_adc_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_adc_Wakeup(void) 
{
    #if(SPI_adc_RX_SOFTWARE_BUF_ENABLED)
        SPI_adc_rxBufferFull  = 0u;
        SPI_adc_rxBufferRead  = 0u;
        SPI_adc_rxBufferWrite = 0u;
    #endif /* (SPI_adc_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_adc_TX_SOFTWARE_BUF_ENABLED)
        SPI_adc_txBufferFull  = 0u;
        SPI_adc_txBufferRead  = 0u;
        SPI_adc_txBufferWrite = 0u;
    #endif /* (SPI_adc_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_adc_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_adc_backup.enableState)
    {
        SPI_adc_Enable();
    }
}


/* [] END OF FILE */
