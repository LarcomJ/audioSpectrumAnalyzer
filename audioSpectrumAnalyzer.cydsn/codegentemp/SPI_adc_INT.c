/*******************************************************************************
* File Name: SPI_adc_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_adc_PVT.h"
#include "cyapicallbacks.h"

/* User code required at start of ISR */
/* `#START SPI_adc_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: SPI_adc_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_adc_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPI_adc_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SPI_adc_txBuffer[SPI_adc_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_adc_TX_ISR)
{
    #if(SPI_adc_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SPI_adc_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_adc_TX_ISR_ENTRY_CALLBACK
        SPI_adc_TX_ISR_EntryCallback();
    #endif /* SPI_adc_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_adc_TX_ISR_START` */

    /* `#END` */
    
    #if(SPI_adc_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SPI_adc_txBufferRead != SPI_adc_txBufferWrite)
        {
            tmpStatus = SPI_adc_GET_STATUS_TX(SPI_adc_swStatusTx);
            SPI_adc_swStatusTx = tmpStatus;

            if(0u != (SPI_adc_swStatusTx & SPI_adc_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SPI_adc_txBufferFull)
                {
                   SPI_adc_txBufferRead++;

                    if(SPI_adc_txBufferRead >= SPI_adc_TX_BUFFER_SIZE)
                    {
                        SPI_adc_txBufferRead = 0u;
                    }
                }
                else
                {
                    SPI_adc_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(SPI_adc_TXDATA_PTR, 
                                             SPI_adc_txBuffer[SPI_adc_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SPI_adc_txBufferRead == SPI_adc_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SPI_adc_TX_STATUS_MASK_REG &= ((uint8) ~SPI_adc_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SPI_adc_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_adc_TX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_adc_TX_ISR_EXIT_CALLBACK
        SPI_adc_TX_ISR_ExitCallback();
    #endif /* SPI_adc_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: SPI_adc_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_adc_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SPI_adc_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SPI_adc_rxBuffer[SPI_adc_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_adc_RX_ISR)
{
    #if(SPI_adc_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (SPI_adc_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_adc_RX_ISR_ENTRY_CALLBACK
        SPI_adc_RX_ISR_EntryCallback();
    #endif /* SPI_adc_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_adc_RX_ISR_START` */

    /* `#END` */
    
    #if(SPI_adc_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SPI_adc_GET_STATUS_RX(SPI_adc_swStatusRx);
        SPI_adc_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SPI_adc_swStatusRx & SPI_adc_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(SPI_adc_RXDATA_PTR);

            /* Set next pointer. */
            SPI_adc_rxBufferWrite++;
            if(SPI_adc_rxBufferWrite >= SPI_adc_RX_BUFFER_SIZE)
            {
                SPI_adc_rxBufferWrite = 0u;
            }

            if(SPI_adc_rxBufferWrite == SPI_adc_rxBufferRead)
            {
                SPI_adc_rxBufferRead++;
                if(SPI_adc_rxBufferRead >= SPI_adc_RX_BUFFER_SIZE)
                {
                    SPI_adc_rxBufferRead = 0u;
                }

                SPI_adc_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SPI_adc_rxBuffer[SPI_adc_rxBufferWrite] = rxData;

            tmpStatus = SPI_adc_GET_STATUS_RX(SPI_adc_swStatusRx);
            SPI_adc_swStatusRx = tmpStatus;
        }

    #endif /* (SPI_adc_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_adc_RX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_adc_RX_ISR_EXIT_CALLBACK
        SPI_adc_RX_ISR_ExitCallback();
    #endif /* SPI_adc_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
