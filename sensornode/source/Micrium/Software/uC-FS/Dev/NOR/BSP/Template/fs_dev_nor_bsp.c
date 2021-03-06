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
*                                         TEMPLATE (PARALLEL)
*
* Filename      : fs_dev_nor_bsp.c
* Version       : v4.07.00.00
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    FS_DEV_NOR_BSP_MODULE
#include  <fs_dev_nor.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          REGISTER DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        REGISTER BIT DEFINES
*********************************************************************************************************
*/


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


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      FILE SYSTEM NOR FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        FSDev_NOR_BSP_Open()
*
* Description : Open (initialize) bus for NOR.
*
* Argument(s) : unit_nbr        Unit number of NOR.
*
*               addr_base       Base address of NOR.
*
*               bus_width       Bus width, in bits.
*
*               phy_dev_cnt     Number of devices interleaved.
*
* Return(s)   : DEF_OK,   if interface was opened.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) This function will be called EVERY time the device is opened.
*********************************************************************************************************
*/

CPU_BOOLEAN  FSDev_NOR_BSP_Open (FS_QTY      unit_nbr,
                                 CPU_ADDR    addr_base,
                                 CPU_INT08U  bus_width,
                                 CPU_INT08U  phy_dev_cnt)
{

    return DEF_FAIL;
}


/*
*********************************************************************************************************
*                                        FSDev_NOR_BSP_Close()
*
* Description : Close (uninitialize) bus for NOR.
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

void  FSDev_NOR_BSP_Close (FS_QTY  unit_nbr)
{

}


/*
*********************************************************************************************************
*                                        FSDev_NOR_BSP_Rd_XX()
*
* Description : Read from bus interface.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               p_dest      Pointer to destination memory buffer.
*
*               addr_src    Source address.
*
*               cnt         Number of words to read.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) Data should be read from the bus in words sized to the data bus; for any unit, only
*                   the function with that access width will be called.
*********************************************************************************************************
*/

void  FSDev_NOR_BSP_Rd_08 (FS_QTY       unit_nbr,
                           void        *p_dest,
                           CPU_ADDR     addr_src,
                           CPU_INT32U   cnt)
{
    CPU_INT08U  *p_dest_08;
    CPU_INT08U   datum;


    p_dest_08 = (CPU_INT08U *)p_dest;
    while (cnt > 0u) {
         datum = *(CPU_REG08 *)addr_src;
         MEM_VAL_SET_INT08U((void *)p_dest_08, datum);
         p_dest_08++;
         addr_src++;
         cnt--;
    }
}

void  FSDev_NOR_BSP_Rd_16 (FS_QTY       unit_nbr,
                           void        *p_dest,
                           CPU_ADDR     addr_src,
                           CPU_INT32U   cnt)
{
    CPU_INT08U  *p_dest_08;
    CPU_INT16U   datum;


    p_dest_08 = (CPU_INT08U *)p_dest;
    while (cnt > 0u) {
         datum = *(CPU_REG16 *)addr_src;
         MEM_VAL_SET_INT16U((void *)p_dest_08, datum);
         p_dest_08 += 2;
         addr_src  += 2;
         cnt--;
    }
}


/*
*********************************************************************************************************
*                                      FSDev_NOR_BSP_RdWord_XX()
*
* Description : Read word from bus interface.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               addr_src    Source address.
*
* Return(s)   : Word read.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) Data should be read from the bus in words sized to the data bus; for any unit, only
*                   the function with that access width will be called.
*********************************************************************************************************
*/

CPU_INT08U  FSDev_NOR_BSP_RdWord_08 (FS_QTY    unit_nbr,
                                     CPU_ADDR  addr_src)
{
    CPU_INT08U  datum;


    datum = *(CPU_REG08 *)addr_src;
    return (datum);
}

CPU_INT16U  FSDev_NOR_BSP_RdWord_16 (FS_QTY    unit_nbr,
                                     CPU_ADDR  addr_src)
{
    CPU_INT16U  datum;


    datum = *(CPU_REG16 *)addr_src;
    return (datum);
}


/*
*********************************************************************************************************
*                                      FSDev_NOR_BSP_WrWord_XX()
*
* Description : Write word to bus interface.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               addr_dest   Destination address.
*
*               datum       Word to write.
*
* Return(s)   : none.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : (1) Data should be written to the bus in words sized to the data bus; for any unit, only
*                   the function with that access width will be called.
*********************************************************************************************************
*/

void  FSDev_NOR_BSP_WrWord_08 (FS_QTY      unit_nbr,
                               CPU_ADDR    addr_dest,
                               CPU_INT08U  datum)
{
    *(CPU_REG08 *)addr_dest = datum;
}

void  FSDev_NOR_BSP_WrWord_16 (FS_QTY      unit_nbr,
                               CPU_ADDR    addr_dest,
                               CPU_INT16U  datum)
{
    *(CPU_REG16 *)addr_dest = datum;
}


/*
*********************************************************************************************************
*                                    FSDev_NOR_BSP_WaitWhileBusy()
*
* Description : Wait while NOR is busy.
*
* Argument(s) : unit_nbr    Unit number of NOR.
*
*               p_phy_data  Pointer to NOR phy data.
*
*               poll_fnct   Pointer to function to poll, if there is no harware ready/busy signal.
*
*               to_us       Timeout, in microseconds.
*
* Return(s)   : DEF_OK,   if NOR became ready.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Physical-layer driver.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  FSDev_NOR_BSP_WaitWhileBusy (FS_QTY                 unit_nbr,
                                          FS_DEV_NOR_PHY_DATA   *p_phy_data,
                                          CPU_BOOLEAN          (*poll_fnct)(FS_DEV_NOR_PHY_DATA  *p_phy_data_arg),
                                          CPU_INT32U             to_us)
{
    CPU_INT32U   time_cur_us;
    CPU_INT32U   time_start_us;
    CPU_BOOLEAN  rdy;


    time_cur_us   = 0u/* $$$$ GET CURRENT TIME, IN MICROSECONDS. */;
    time_start_us = time_cur_us;

    while (time_cur_us - time_start_us < to_us) {
        rdy = poll_fnct(p_phy_data);
        if (rdy == DEF_OK) {
            return (DEF_OK);
        }
        time_cur_us = 0u/* $$$$ GET CURRENT TIME, IN MICROSECONDS. */;
    }

    return (DEF_FAIL);
}
