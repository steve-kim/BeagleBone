/*****************************************************************************
* @file asm_vectors.s
*
* This file contains the initial vector table for the Cortex A8 processor
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who     Date     Changes
* ----- ------- -------- ---------------------------------------------------

Save lr when profiling is enabled
Implemented ARM Erratas. Please refer to file 'xil_errata.h' for errata description
******************************************************************************/

/* #include "xil_errata.h" */ /*Add errata for A8 */

.org 0
.text
.arm

.global _boot
.global _freertos_vector_table

.global FIQInterrupt
.global DataAbortInterrupt
.global PrefetchAbortInterrupt
.global vPortInstallFreeRTOSVectorTable

.extern FreeRTOS_IRQ_Handler
.extern FreeRTOS_SWI_Handler

.section .freertos_vectors
_freertos_vector_table:
	B	  _boot
	B	  FreeRTOS_Undefined
	ldr   pc, _swi
	B	  FreeRTOS_PrefetchAbortHandler
	B	  FreeRTOS_DataAbortHandler
	NOP	  /* Placeholder for address exception vector*/
	LDR   PC, _irq
	B	  FreeRTOS_FIQHandler

_irq:   .word FreeRTOS_IRQ_Handler
_swi:   .word FreeRTOS_SWI_Handler


.align 4
FreeRTOS_FIQHandler:			/* FIQ vector handler */
	stmdb	sp!,{r0-r3,r12,lr}	/* state save from compiled code */
FIQLoop:
	blx	FIQInterrupt			/* FIQ vector */
	ldmia	sp!,{r0-r3,r12,lr}	/* state restore from compiled code */
	subs	pc, lr, #4			/* adjust return */

.align 4
FreeRTOS_Undefined:				/* Undefined handler */
	b		.

.align 4
FreeRTOS_DataAbortHandler:		/* Data Abort handler */
#ifdef CONFIG_ARM_ERRATA_775420
	dsb
#endif
	stmdb	sp!,{r0-r3,r12,lr}	/* state save from compiled code */
	blx	DataAbortInterrupt		/*DataAbortInterrupt :call C function here */
	ldmia	sp!,{r0-r3,r12,lr}	/* state restore from compiled code */
	subs	pc, lr, #4			/* adjust return */

.align 4
FreeRTOS_PrefetchAbortHandler:	/* Prefetch Abort handler */
#ifdef CONFIG_ARM_ERRATA_775420
	dsb
#endif
	stmdb	sp!,{r0-r3,r12,lr}	/* state save from compiled code */
	blx	PrefetchAbortInterrupt	/* PrefetchAbortInterrupt: call C function here */
	ldmia	sp!,{r0-r3,r12,lr}	/* state restore from compiled code */
	subs	pc, lr, #4			/* adjust return */

.align 4
.type vPortInstallFreeRTOSVectorTable, %function
vPortInstallFreeRTOSVectorTable:

	/* Set VBAR to the vector table that contains the FreeRTOS handlers. */
	ldr	r0, =_freertos_vector_table
	mcr	p15, 0, r0, c12, c0, 0
	dsb
	isb
	bx lr


.end




