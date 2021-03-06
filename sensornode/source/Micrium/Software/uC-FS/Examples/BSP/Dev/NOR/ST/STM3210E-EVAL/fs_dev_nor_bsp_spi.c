/*
*********************************************************************************************************
*                                             uC/FS V4
*                                     The Embedded File System
*
*                         (c) Copyright 2008-2014; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/FS is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      FILE SYSTEM DEVICE DRIVER
*
*                                          NOR FLASH DEVICES
*
*                                BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                      ST STM32F103VE ON THE ST STM3210E-EVAL EVALUATION BOARD
*
* Filename      : fs_dev_nor_bsp.c
* Version       : v4.07.00
* Programmer(s) : BAN
*********************************************************************************************************
* Note(s)       : (1) The STM32 SPI peripheral is described in documentation from ST :
*
*                         RM0008 Reference Manual.  Low-, medium- and high-density STM32F101xx, STM32F102xx
*                         and STM32F103xx advnaced ARM-based 32-bit MCUs.  Rev 6, September 2008.
*                         "22 Serial peripheral interface (SPI)".
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    FS_DEV_NOR_BSP_SPI_MODULE
#include  <fs.h>
#include  <fs_dev_nor.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  STM32_PCLK2_CLK_FREQ_MHZ                     72000000uL

#define  STM32_PIN_NOR_CS                           DEF_BIT_02  /* GPIOB.02                                             */
#define  STM32_PIN_SPI1_SCK                         DEF_BIT_05  /* GPIOA.05                                             */
#define  STM32_PIN_SPI1_MISO                        DEF_BIT_06  /* GPIOA.06                                             */
#define  STM32_PIN_SPI1_MOSI                        DEF_BIT_07  /* GPIOA.07                                             */

/*
*********************************************************************************************************
*                                          REGISTER DEFINES
*********************************************************************************************************
*/

#define  STM32_REG_SPI1_BASE                        0x40013000uL
#define  STM32_REG_SPI1_CR1         (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x000u))
#define  STM32_REG_SPI1_CR2         (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x004u))
#define  STM32_REG_SPI1_SR          (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x008u))
#define  STM32_REG_SPI1_DR          (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x00Cu))
#define  STM32_REG_SPI1_CRCPR       (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x010u))
#define  STM32_REG_SPI1_RXCRCR      (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x014u))
#define  STM32_REG_SPI1_TXCRCR      (*(CPU_REG32 *)(STM32_REG_SPI1_BASE  + 0x018u))

#define  STM32_REG_SPI2_BASE                        0x40003800uL
#define  STM32_REG_SPI2_CR1         (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x000u))
#define  STM32_REG_SPI2_CR2         (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x004u))
#define  STM32_REG_SPI2_SR          (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x008u))
#define  STM32_REG_SPI2_DR          (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x00Cu))
#define  STM32_REG_SPI2_CRCPR       (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x010u))
#define  STM32_REG_SPI2_RXCRCR      (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x014u))
#define  STM32_REG_SPI2_TXCRCR      (*(CPU_REG32 *)(STM32_REG_SPI2_BASE  + 0x018u))

#define  STM32_REG_RCC_BASE                         0x40021000uL
#define  STM32_REG_RCC_CR           (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x000u))
#define  STM32_REG_RCC_CFGR         (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x004u))
#define  STM32_REG_RCC_CIR          (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x008u))
#define  STM32_REG_RCC_APB2RSTR     (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x00Cu))
#define  STM32_REG_RCC_APB1RSTR     (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x010u))
#define  STM32_REG_RCC_AHBENR       (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x014u))
#define  STM32_REG_RCC_APB2ENR      (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x018u))
#define  STM32_REG_RCC_APB1ENR      (*(CPU_REG32 *)(STM32_REG_RCC_BASE   + 0x01Cu))

#define  STM32_REG_GPIOA_BASE                       0x40010800uL
#define  STM32_REG_GPIOA_CRL        (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x000u))
#define  STM32_REG_GPIOA_CRH        (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x004u))
#define  STM32_REG_GPIOA_IDR        (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x008u))
#define  STM32_REG_GPIOA_ODR        (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x00Cu))
#define  STM32_REG_GPIOA_BSRR       (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x010u))
#define  STM32_REG_GPIOA_BRR        (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x014u))
#define  STM32_REG_GPIOA_LCKR       (*(CPU_REG32 *)(STM32_REG_GPIOA_BASE + 0x018u))

#define  STM32_REG_GPIOB_BASE                       0x40010C00uL
#define  STM32_REG_GPIOB_CRL        (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x000u))
#define  STM32_REG_GPIOB_CRH        (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x004u))
#define  STM32_REG_GPIOB_IDR        (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x008u))
#define  STM32_REG_GPIOB_ODR        (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x00Cu))
#define  STM32_REG_GPIOB_BSRR       (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x010u))
#define  STM32_REG_GPIOB_BRR        (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x014u))
#define  STM32_REG_GPIOB_LCKR       (*(CPU_REG32 *)(STM32_REG_GPIOB_BASE + 0x018u))

#define  STM32_REG_GPIOC_BASE                       0x40011000uL
#define  STM32_REG_GPIOC_CRL        (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x000u))
#define  STM32_REG_GPIOC_CRH        (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x004u))
#define  STM32_REG_GPIOC_IDR        (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x008u))
#define  STM32_REG_GPIOC_ODR        (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x00Cu))
#define  STM32_REG_GPIOC_BSRR       (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x010u))
#define  STM32_REG_GPIOC_BRR        (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x014u))
#define  STM32_REG_GPIOC_LCKR       (*(CPU_REG32 *)(STM32_REG_GPIOC_BASE + 0x018u))

/*
*********************************************************************************************************
*                                        REGISTER BIT DEFINES
*********************************************************************************************************
*/

                                                                /* --------------------- CR1 BITS --------------------- */
#define  STM32_BIT_SPI_CR1_BIDIMODE             DEF_BIT_15      /* Bidirectional data mode enable.                      */
#define  STM32_BIT_SPI_CR1_BIDIOE               DEF_BIT_14      /* Output enable in bidirectional mode.                 */
#define  STM32_BIT_SPI_CR1_CRCEN                DEF_BIT_13      /* Hardware CRC calculation enable.                     */
#define  STM32_BIT_SPI_CR1_CRCNEXT              DEF_BIT_12      /* Transmit CRC next.                                   */
#define  STM32_BIT_SPI_CR1_DFF                  DEF_BIT_11      /* Data Frame Format (0 = 8-bit).                       */
#define  STM32_BIT_SPI_CR1_RXONLY               DEF_BIT_10      /* Receive only.                                        */
#define  STM32_BIT_SPI_CR1_SSM                  DEF_BIT_09      /* Software slave management.                           */
#define  STM32_BIT_SPI_CR1_SSI                  DEF_BIT_08      /* Internal slave select.                               */
#define  STM32_BIT_SPI_CR1_LSBFIRST             DEF_BIT_07      /* Frame format (0 = MSB first).                        */
#define  STM32_BIT_SPI_CR1_SPE                  DEF_BIT_06      /* SPI Enable.                                          */
#define  STM32_BIT_SPI_CR1_BR_MASK                  0x0038u     /* Baud Rate Control mask.                              */
#define  STM32_BIT_SPI_CR1_MSTR                 DEF_BIT_02      /* Master Selection (1 = master.                        */
#define  STM32_BIT_SPI_CR1_CPOL                 DEF_BIT_01      /* Clock Polarity.                                      */
#define  STM32_BIT_SPI_CR1_CPHA                 DEF_BIT_00      /* Clock Phase.                                         */

                                                                /* --------------------- CR2 BITS --------------------- */
#define  STM32_BIT_SPI_CR2_TXEIE                DEF_BIT_07      /* Tx buffer Empty Interrupt Enable.                    */
#define  STM32_BIT_SPI_CR2_RXNEIE               DEF_BIT_06      /* RX buffer Not Empty Interrupt Enable.                */
#define  STM32_BIT_SPI_CR2_ERRIE                DEF_BIT_05      /* Error Interrupt Enable.                              */
#define  STM32_BIT_SPI_CR2_SSOE                 DEF_BIT_02      /* SS Output Enable.                                    */
#define  STM32_BIT_SPI_CR2_TXDMAEN              DEF_BIT_01      /* Tx Buffer DMA Enable.                                */
#define  STM32_BIT_SPI_CR2_RXDMAEN              DEF_BIT_00      /* Rx Buffer DMA Enable.                                */

                                                                /* --------------------- SR BITS ---------------------- */
#define  STM32_BIT_SPI_SR_BSY                   DEF_BIT_07      /* Busy flag.                                           */
#define  STM32_BIT_SPI_SR_OVR                   DEF_BIT_06      /* Overrun flag.                                        */
#define  STM32_BIT_SPI_SR_MODF                  DEF_BIT_05      /* Mode fault.                                          */
#define  STM32_BIT_SPI_SR_CRCERR                DEF_BIT_04      /* CRC Error flag.                                      */
#define  STM32_BIT_SPI_SR_UDR                   DEF_BIT_03      /* Underrun flag.                                       */
#define  STM32_BIT_SPI_SR_CHSIDE                DEF_BIT_02      /* Channel side.                                        */
#define  STM32_BIT_SPI_SR_TXE                   DEF_BIT_01      /* Trnasmit buffer Empty.                               */
#define  STM32_BIT_SPI_SR_RXNE                  DEF_BIT_00      /* Receive buffer Not Empty.                            */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                        /* --------------- SPI API FNCTS -------------- */
static  CPU_BOOLEAN  FSDev_BSP_SPI_Open      (FS_QTY       unit_nbr);   /* Open (initialize) SPI.                       */

static  void         FSDev_BSP_SPI_Close     (FS_QTY       unit_nbr);   /* Close (uninitialize) SPI.                    */

static  void         FSDev_BSP_SPI_Lock      (FS_QTY       unit_nbr);   /* Acquire SPI lock.                            */

static  void         FSDev_BSP_SPI_Unlock    (FS_QTY       unit_nbr);   /* Release SPI lock.                            */

static  void         FSDev_BSP_SPI_Rd        (FS_QTY       unit_nbr,    /* Rd from SPI.                                 */
                                              void        *p_dest,
                                              CPU_SIZE_T   cnt);

static  void         FSDev_BSP_SPI_Wr        (FS_QTY       unit_nbr,    /* Wr to SPI.                                   */
                                              void        *p_src,
                                              CPU_SIZE_T   cnt);

static  void         FSDev_BSP_SPI_ChipSelEn (FS_QTY       unit_nbr);   /* En flash chip sel.                           */

static  void         FSDev_BSP_SPI_ChipSelDis(FS_QTY       unit_nbr);   /* Dis flash chip sel.                          */

static  void         FSDev_BSP_SPI_SetClkFreq(FS_QTY       unit_nbr,    /* Set SPI clk freq.                            */
                                              CPU_INT32U   freq);


/*
*********************************************************************************************************
*                                         INTERFACE STRUCTURE
*********************************************************************************************************
*/

const  FS_DEV_SPI_API  FSDev_NOR_BSP_SPI = {
    FSDev_BSP_SPI_Open,
    FSDev_BSP_SPI_Close,
    FSDev_BSP_SPI_Lock,
    FSDev_BSP_SPI_Unlock,
    FSDev_BSP_SPI_Rd,
    FSDev_BSP_SPI_Wr,
    FSDev_BSP_SPI_ChipSelEn,
    FSDev_BSP_SPI_ChipSelDis,
    FSDev_BSP_SPI_SetClkFreq
};


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                    FILE SYSTEM NOR SPI FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        FSDev_BSP_SPI_Open()
*
* Description : Open (initialize) SPI for serial flash.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : DEF_OK,   if interface was opened.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) This function will be called EVERY time the device is opened.
*
*               (2) Several aspects of SPI communication may need to be configured :
*
*                   (a) CLOCK POLARITY & PHASE (CPOL & CPHA).  SPI communication takes place in any of
*                       four modes, depending on the clock phase & clock polarity settings :
*
*                       (1) If CPOL = 0, the clock is low  when inactive.
*
*                           If CPOL = 1, the clock is high when inactive.
*
*                       (2) If CPHA = 0, data is 'read'    on the leading   edge of the clock &
*                                                'changed' on the following edge of the clock.
*
*                           If CPHA = 1, data is 'changed' on the following edge of the clock &
*                                                'read'    on the leading   edge of the clock.
*
*                       The driver should configure the SPI for CPOL = 0, CPHA = 0.  This means that
*                       data (i.e., bits) are read when a low->high clock transition occurs & changed
*                       when a high->low clock transition occurs.
*
*                   (b) TRANSFER UNIT LENGTH.  A transfer unit is always 8-bits.
*
*                   (c) SHIFT DIRECTION.  The most significant bit should always be transmitted first.
*
*                   (d) CLOCK FREQUENCY.  See 'FSDev_BSP_SPI_SetClkFreq()  Note #1'.
*
*               (3) The CS (Chip Select) MUST be configured as a GPIO output; it should not be controlled
*                   by the CPU's SPI peripheral.  The functions 'FSDev_BSP_SPI_ChipSelEn()' and
*                  'FSDev_BSP_SPI_ChipSelDis()' manually enable and disable the CS.
*
*               (4) A serial flash is connected to the following pins :
*
*                   ----------------------------------------------
*                   |  STM32 NAME   |  STM32 PIO #  | FLASH NAME |
*                   ----------------------------------------------
*                   |    ----       |     PB[2]     | S  (pin 1) |
*                   |    SPI1_MOSI  |     PA[7]     | D  (pin 5) |
*                   |    SPI1_SCK   |     PA[5]     | C  (pin 6) |
*                   |    SPI1_MISO  |     PA[6]     | Q  (pin 2) |
*                   ----------------------------------------------
*********************************************************************************************************
*/

static  CPU_BOOLEAN  FSDev_BSP_SPI_Open (FS_QTY  unit_nbr)
{
    CPU_INT32U  temp;


    if (unit_nbr != 1u) {
        FS_TRACE_INFO(("FSDev_NOR_BSP_SPI_Open(): Invalid unit nbr: %d.\r\n", unit_nbr));
        return (DEF_FAIL);
    }

    STM32_REG_RCC_APB2RSTR &= ~(DEF_BIT_12 | DEF_BIT_03 | DEF_BIT_04);
    STM32_REG_RCC_APB2ENR  |=  (DEF_BIT_12 | DEF_BIT_03 | DEF_BIT_04);      /* En SPI1, GPIOA, GPIOC clks.              */



                                                                /* --------------------- CFG GPIO --------------------- */
    temp                 = STM32_REG_GPIOA_CRL;                 /* Cfg SCK, MOSO, MISO as alt fnct push-pull.           */
    temp                &= 0x000FFFFFu;
    temp                |= 0xBBB00000u;
    STM32_REG_GPIOA_CRL  = temp;

    temp                 = STM32_REG_GPIOB_CRL;                 /* Cfg NOR CS as general purpose output.                */
    temp                &= 0xFFFFF0FFu;
    temp                |= 0x00000300u;
    STM32_REG_GPIOB_CRL  = temp;



                                                                /* ---------------------- INIT SPI -------------------- */
    STM32_REG_SPI1_CR1  = STM32_BIT_SPI_CR1_MSTR                /* Master.                                              */
                        | STM32_BIT_SPI_CR1_BR_MASK             /* Max baud rate div.                                   */
                        | STM32_BIT_SPI_CR1_SSI
                        | STM32_BIT_SPI_CR1_SSM;                /* Software slave management.                           */

    STM32_REG_SPI1_CR2  = 0u;
    STM32_REG_SPI1_CR1 |= STM32_BIT_SPI_CR1_SPE;                /* Enable SPI.                                          */

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        FSDev_BSP_SPI_Close()
*
* Description : Close (uninitialize) SPI for serial flash.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) This function will be called EVERY time the device is closed.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_Close (FS_QTY  unit_nbr)
{
   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */
}


/*
*********************************************************************************************************
*                                        FSDev_BSP_SPI_Lock()
*
* Description : Acquire SPI lock.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) This function will be called before the driver begins to access the SPI.  The
*                   application should NOT use the same bus to access another device until the matching
*                   call to 'FSDev_BSP_SPI_Unlock()' has been made.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_Lock (FS_QTY  unit_nbr)
{
   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */
}


/*
*********************************************************************************************************
*                                       FSDev_BSP_SPI_Unlock()
*
* Description : Release SPI lock.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) 'FSDev_BSP_SPI_Lock()' will be called before the driver begins to access the SPI.
*                   The application should NOT use the same bus to access another device until the
*                   matching call to this function has been made.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_Unlock (FS_QTY  unit_nbr)
{
   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */
}


/*
*********************************************************************************************************
*                                         FSDev_BSP_SPI_Rd()
*
* Description : Read from SPI.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               p_dest      Pointer to destination memory buffer.
*
*               cnt         Number of octets to read.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_Rd (FS_QTY       unit_nbr,
                                void        *p_dest,
                                CPU_SIZE_T   cnt)
{
    CPU_INT08U   *p_dest_08;
    CPU_BOOLEAN   rxd;
    CPU_INT08U    datum;


   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */

    p_dest_08 = (CPU_INT08U *)p_dest;
    while (cnt > 0u) {
        STM32_REG_SPI1_DR = 0xFFu;                              /* Tx dummy byte.                                       */

                                                                /* Wait to rx byte.                                     */
        do {
            rxd = DEF_BIT_IS_SET(STM32_REG_SPI1_SR, STM32_BIT_SPI_SR_RXNE);
        } while (rxd == DEF_NO);

        datum     = STM32_REG_SPI1_DR;                          /* Rd rx'd byte.                                        */
       *p_dest_08 = datum;
        p_dest_08++;
        cnt--;
    }
}


/*
*********************************************************************************************************
*                                         FSDev_BSP_SPI_Wr()
*
* Description : Write to SPI.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               p_src       Pointer to source memory buffer.
*
*               cnt         Number of octets to write.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_Wr (FS_QTY       unit_nbr,
                                void        *p_src,
                                CPU_SIZE_T   cnt)
{
    CPU_INT08U   *p_src_08;
    CPU_BOOLEAN   rxd;
    CPU_INT08U    datum;


   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */

    p_src_08 = (CPU_INT08U *)p_src;
    while (cnt > 0u) {
        datum             = *p_src_08;
        STM32_REG_SPI1_DR =  datum;                             /* Tx byte.                                             */
        p_src_08++;

                                                                /* Wait to rx byte.                                     */
        do {
            rxd = DEF_BIT_IS_SET(STM32_REG_SPI1_SR, STM32_BIT_SPI_SR_RXNE);
        } while (rxd == DEF_NO);

        datum = STM32_REG_SPI1_DR;                              /* Rd rx'd dummy byte.                                  */
       (void)&datum;

        cnt--;
    }
}


/*
*********************************************************************************************************
*                                      FSDev_BSP_SPI_ChipSelEn()
*
* Description : Enable serial flash chip select.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) The chip select is typically 'active low'; to enable the serial flash, the chip select
*                   pin should be cleared.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_ChipSelEn (FS_QTY  unit_nbr)
{
   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */

    STM32_REG_GPIOB_BSRR = DEF_BIT_18;
}


/*
*********************************************************************************************************
*                                     FSDev_BSP_SPI_ChipSelDis()
*
* Description : Disable serial flash chip select.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) The chip select is typically 'active low'; to disable the serial flash, the chip select
*                   pin should be set.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_ChipSelDis (FS_QTY  unit_nbr)
{
   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */

    STM32_REG_GPIOB_BSRR = DEF_BIT_02;
}


/*
*********************************************************************************************************
*                                     FSDev_BSP_SPI_SetClkFreq()
*
* Description : Set SPI clock frequency.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               freq        Clock frequency, in Hz.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) The effective clock frequency MUST be no more than 'freq'.  If the frequency cannot be
*                   configured equal to 'freq', it should be configured less than 'freq'.
*********************************************************************************************************
*/

static  void  FSDev_BSP_SPI_SetClkFreq (FS_QTY      unit_nbr,
                                        CPU_INT32U  freq)
{
    CPU_INT32U  clk_div;
    CPU_INT32U  clk_div_log;
    CPU_INT32U  clk_div_min;


   (void)&unit_nbr;                                             /* Prevent compiler warning.                            */

    clk_div     =  2u;
    clk_div_log =  1u;
    clk_div_min = (STM32_PCLK2_CLK_FREQ_MHZ + freq - 1u) / freq;

    if (clk_div_min >= 256u) {
        clk_div_log  = 8u;
    } else {
        while (clk_div < clk_div_min) {
            clk_div *= 2u;
            clk_div_log++;
        }
    }

    STM32_REG_SPI1_CR1 &= ~STM32_BIT_SPI_CR1_SPE;               /* Dis SPI.                                             */

    STM32_REG_SPI1_CR1 &= ~STM32_BIT_SPI_CR1_BR_MASK;           /* Clr baudrate div ...                                 */
    STM32_REG_SPI1_CR1 |= ((clk_div_log - 1u) << 3);            /* ... set new baudrate div.                            */
    STM32_REG_SPI1_CR1 |=  STM32_BIT_SPI_CR1_SPE;               /* Re-en SPI.                                           */
}
