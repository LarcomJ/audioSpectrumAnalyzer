/*******************************************************************************
* File Name: DVDAC_1.c
* Version 2.10
*
* Description:
*  This file provides the source code of APIs for the DVDAC component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DVDAC_1.h"
#include "DVDAC_1_VDAC8.h"
#include "CyDmac.h"
#include "DVDAC_1_DMA_dma.H"

#if(DVDAC_1_INTERNAL_CLOCK_USED)
    #include "DVDAC_1_IntClock.h"
#endif /* DVDAC_1_INTERNAL_CLOCK_USED */


static uint8 DVDAC_1_ditheredValues[16u] = {0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 
                                            0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u};


uint8 DVDAC_1_initVar = 0u;

static uint8 DVDAC_1_dmaChan;
static uint8 DVDAC_1_dmaTd = CY_DMA_INVALID_TD;

static void DVDAC_1_InitDma(void)  ;


/*******************************************************************************
* Function Name: DVDAC_1_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call DVDAC_1_Init() because
*  the DVDAC_1_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  All registers will be set to values according to the customizer Configure
*  dialog.
*
* Global variables:
*  None
*
*******************************************************************************/
void DVDAC_1_Init(void) 
{
    DVDAC_1_VDAC8_Init();

    if(CY_DMA_INVALID_TD == DVDAC_1_dmaTd)
    {
        DVDAC_1_InitDma();
    }
}


/*******************************************************************************
* Function Name: DVDAC_1_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary to
*  call DVDAC_1_Enable() because the DVDAC_1_Start() API calls
*  this function, which is the preferred method to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void DVDAC_1_Enable(void) 
{
    (void) CyDmaChEnable(DVDAC_1_dmaChan, 1u);

    #if(DVDAC_1_INTERNAL_CLOCK_USED)
        DVDAC_1_IntClock_Start();
    #endif /* DVDAC_1_INTERNAL_CLOCK_USED */

    DVDAC_1_VDAC8_Enable();
}


/*******************************************************************************
* Function Name: DVDAC_1_Start
********************************************************************************
*
* Summary:
*  Performs all of the required initialization for the component and enables
*  power to the block. The first time the routine is executed, the component is
*  initialized to the configured settings. When called to restart the DVDAC
*  following a DVDAC_1_Stop() call, the current component parameter
*  settings are retained.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the DVDAC_1_initVar variable is already set, this function only
*  calls the DVDAC_1_Enable() function.
*
* Global variables:
*  DVDAC_1_initVar - indicates whether the DVDAC has been initialized.
*  The variable is initialized to 0 and set to 1 the first time DVDAC_Start()
*  is called. This allows the component to restart without reinitialization
*  after the first call to the DVDAC_1_Start() routine. If
*  reinitialization of the component is required, then the
*  DVDAC_1_Init() function can be called before the
*  DVDAC_1_Start() or DVDAC_1_Enable() function.
*
*******************************************************************************/
void DVDAC_1_Start(void) 
{
    /* Hardware initiazation only needs to occure the first time */
    if(0u == DVDAC_1_initVar)
    {
        DVDAC_1_Init();
        DVDAC_1_initVar = 1u;
    }

    DVDAC_1_Enable();
}


/*******************************************************************************
* Function Name: DVDAC_1_Stop
********************************************************************************
*
* Summary:
*  Stops the component and turns off the analog blocks in the DVDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void DVDAC_1_Stop(void) 
{
    #if(DVDAC_1_INTERNAL_CLOCK_USED)
        DVDAC_1_IntClock_Stop();
    #endif /* DVDAC_1_INTERNAL_CLOCK_USED */

    (void) CyDmaChDisable(DVDAC_1_dmaChan);
    DVDAC_1_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: DVDAC_1_SetValue
********************************************************************************
*
* Summary:
*  Sets the DVDACs output. The function populates the SRAM array based on the
*  value and the resolution setting. That array is then transferred to the
*  internal VDAC by DMA.
*
* Parameters:
*  (uint16) value:  The maximum value will be dependent on the resolution
*  selected:
*   Resolution       Valid range
*        9 bits       0x0 - 0x1FE
*       10 bits       0x0 - 0x3FC
*       11 bits       0x0 - 0x7F8
*       12 bits       0x0 - 0xFF0
*
*  This value includes an integer portion and a fractional portion that varies
*  depending on number of bits of resolution:
*   - 9-bits:  1 fractional bit
*   - 10-bits: 2 fractional bits
*   - 11-bits: 3 fractional bits
*   - 12-bits: 4 fractional bits
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void DVDAC_1_SetValue(uint16 value) 
{
    uint8  lowCount  = (uint8)(value & DVDAC_1_FRACTIONAL_PORTION_MASK);
    uint8 highCount  = (uint8)(value >> DVDAC_1_INTEGER_PORTION_SHIFT);
    uint8 i;


    CYASSERT(value <= DVDAC_1_DVDAC_MAX_VALUE);

    if (DVDAC_1_INTEGER_PORTION_MAX_VALUE == highCount)
    {
        for (i = 0u; i < DVDAC_1_DITHERED_ARRAY_SIZE; i++)
        {
            DVDAC_1_ditheredValues[i] = DVDAC_1_INTEGER_PORTION_MAX_VALUE;
        }
    }
    else
    {
        for(i = 0u; i < DVDAC_1_DITHERED_ARRAY_SIZE; i++)
        {
            if(i < (DVDAC_1_DITHERED_ARRAY_SIZE - lowCount))
            {
                DVDAC_1_ditheredValues[i] = highCount;
            }
            else
            {
                DVDAC_1_ditheredValues[i] = highCount + 1u;
            }
        }
    }
}


/*******************************************************************************
* Function Name: DVDAC_1_InitDma
********************************************************************************
*
* Summary:
*  Configures a DMA transfer of channel values from memory to VDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void DVDAC_1_InitDma(void)  
{
    /***************************************************************************
    * Transfers channel values from SRAM to VDAC register. One byte transfer,
    * each burst requires a request.
    ***************************************************************************/
    DVDAC_1_dmaChan = DVDAC_1_DMA_DmaInitialize(
                                    DVDAC_1_DMA_BYTES_PER_BURST,
                                    DVDAC_1_DMA_REQUEST_PER_BURST,
                                    HI16(DVDAC_1_DMA_SRC_BASE),
                                    HI16(DVDAC_1_DMA_DST_BASE));

    DVDAC_1_dmaTd = CyDmaTdAllocate();


    /***************************************************************************
    * One TD looping on itself, increment the source address, but not the
    * destination address.
    ***************************************************************************/
    (void) CyDmaTdSetConfiguration( DVDAC_1_dmaTd,
                                    DVDAC_1_DITHERED_ARRAY_SIZE,
                                    DVDAC_1_dmaTd,
                                    (uint8) CY_DMA_TD_INC_SRC_ADR);

    /* Transfers the value for each channel from memory to VDAC */
    (void) CyDmaTdSetAddress(   DVDAC_1_dmaTd,
                                LO16((uint32)DVDAC_1_ditheredValues),
                                LO16((uint32)DVDAC_1_VDAC8_Data_PTR));

    (void) CyDmaChSetInitialTd(DVDAC_1_dmaChan, DVDAC_1_dmaTd);
}

/* [] END OF FILE */

