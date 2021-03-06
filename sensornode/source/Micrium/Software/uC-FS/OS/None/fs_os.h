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
*                                 FILE SYSTEM OPERATING SYSTEM LAYER
*
*                                                No OS
*
* Filename      : fs_os.h
* Version       : v4.07.00
* Programmer(s) : FBJ
*                 BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          INCLUDE GUARD
*********************************************************************************************************
*/

#ifndef  FS_OS_H
#define  FS_OS_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_OS_MODULE
#define  FS_OS_EXT
#else
#define  FS_OS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#ifdef  FS_OS_PRESENT
#undef  FS_OS_PRESENT
#endif


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


typedef  volatile  CPU_INT16U  FS_OS_SEM;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  FS_OS_SemCreate(FS_OS_SEM  *p_sem,
                             CPU_INT16U  cnt);

CPU_BOOLEAN  FS_OS_SemDel   (FS_OS_SEM  *p_sem);

CPU_BOOLEAN  FS_OS_SemPend  (FS_OS_SEM  *p_sem,
                             CPU_INT32U  timeout);

CPU_BOOLEAN  FS_OS_SemPost  (FS_OS_SEM  *p_sem);

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if     (FS_CFG_FILE_LOCK_EN == DEF_ENABLED)
#error  "FS_CFG_FILE_LOCK_EN    illegally #define'd in 'fs_cfg.h'   "
#error  "                       [MUST be DEF_DISABLED]              "
#endif


/*
*********************************************************************************************************
*                                          MODULE END
*********************************************************************************************************
*/

#endif
