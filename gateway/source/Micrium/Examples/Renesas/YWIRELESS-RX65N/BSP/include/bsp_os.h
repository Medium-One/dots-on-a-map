/*
*********************************************************************************************************
*                                             EXAMPLE CODE
*********************************************************************************************************
* Licensing terms:
*   This file is provided as an example on how to use Micrium products. It has not necessarily been
*   tested under every possible condition and is only offered as a reference, without any guarantee.
*
*   Please feel free to use any application code labeled as 'EXAMPLE CODE' in your application products.
*   Example code may be used as is, in whole or in part, or may be used as a reference only. This file
*   can be modified as required.
*
*   You can find user manuals, API references, release notes and more at: https://doc.micrium.com
*
*   You can contact us at: http://www.micrium.com
*
*   Please help us continue to provide the Embedded community with the finest software available.
*
*   Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            OS TICK BSP
*
* File : bsp_os.h
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  _BSP_OS_H_
#define  _BSP_OS_H_


/*
*********************************************************************************************************
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                                   /* See Note #1.                                         */
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void     BSP_OSTickInit (void);
CPU_ISR  BSP_OSTickISR  (void);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*********************************************************************************************************
*/

#ifdef __cplusplus
}                                                               /* End of 'extern'al C lang linkage.                    */
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */