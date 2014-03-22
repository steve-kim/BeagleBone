/**
 * \file   gpioLEDBlink.c
 *
 *  \brief  This application uses a GPIO pin to blink the LED.
 *
 *          Application Configurations:
 *
 *              Modules Used:
 *                  GPIO1
 *
 *              Configuration Parameters:
 *                  None
 *
 *          Application Use Case:
 *              1) The GPIO pin GPIO1[23] is used as an output pin.
 *              2) This pin is alternately driven HIGH and LOW. A finite delay
 *                 is given to retain the pin in its current state.
 *
 *          Running the example:
 *              On running the example, the LED on beaglebone would be seen 
 *              turning ON and OFF alternatively.
 *
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
*/
/* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"

/****************************************************************************
**		FREERTOS DEMO DEFINITIONS
****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Standard demo includes. */
//#include "partest.h"


/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY         ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY            ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS                     ( 200 / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                                        ( 1 )

/* The LED toggled by the Rx task. */
//#define mainTASK_LED                                            ( 0 )

//Adding in this counter for now to get past compile errors
//We will need to actually implement this tick count somehow
volatile uint32_t ulHighFrequencyTimerCounts = 0;
/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define GPIO_INSTANCE_ADDRESS           (SOC_GPIO_1_REGS)
#define GPIO_INSTANCE_PIN_NUMBER        (23)

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void Delay(unsigned int count);
static void BlinkLED();

/*****************************************************************************
**                INTERNAL FUNCTION DEFINITIONS
*****************************************************************************/

static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

static QueueHandle_t xQueue = NULL;

/*
** The main function. Application starts here.
*/
int main()
{
    /* Enabling functional clocks for GPIO1 instance. */
//    GPIO1ModuleClkConfig();

    /* Selecting GPIO1[23] pin for use. */
//    GPIO1Pin23PinMuxSetup();
    
    /* Enabling the GPIO module. */
//    GPIOModuleEnable(GPIO_INSTANCE_ADDRESS);

    /* Resetting the GPIO module. */
//    GPIOModuleReset(GPIO_INSTANCE_ADDRESS);

    /* Setting the GPIO pin as an output pin. */
/*    GPIODirModeSet(GPIO_INSTANCE_ADDRESS,
                   GPIO_INSTANCE_PIN_NUMBER,
                   GPIO_DIR_OUTPUT); */

    /*Create the queue*/
    xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

    if (xQueue != NULL) 
    {
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate( prvQueueReceiveTask,				/* The function that implements the task. */
					"Rx", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
					NULL, 								/* The parameter passed to the task - not used in this case. */
					mainQUEUE_RECEIVE_TASK_PRIORITY, 	/* The priority assigned to the task. */
					NULL );								/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was either insufficient FreeRTOS heap memory available for the idle
	and/or timer tasks to be created, or vTaskStartScheduler() was called from
	User mode.  See the memory management section on the FreeRTOS web site for
	more details on the FreeRTOS heap http://www.freertos.org/a00111.html.  The
	mode from which main() is called is set in the C start up code and must be
	a privileged mode (not user mode). */
	for( ;; );


} 

/*
** A function which is used to generate a delay.
*/
static void Delay(volatile unsigned int count)
{
    while(count--);
}

#if 0
static void BlinkLED()
{
    while(1)
    {
        /* Driving a logic HIGH on the GPIO pin. */
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_HIGH);

        Delay(0x3FF00);

        /* Driving a logic LOW on the GPIO pin. */
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_LOW);

        Delay(0x3FF00);
    }
}
#endif

static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;

        /* Remove compiler warning about unused parameter. */
        ( void ) pvParameters;

        /* Initialise xNextWakeTime - this only needs to be done once. */
        xNextWakeTime = xTaskGetTickCount();

        for( ;; )
        {
                /* Place this task in the blocked state until it is time to run again. */
                vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

                /* Send to the queue - causing the queue receive task to unblock and
                toggle the LED.  0 is used as the block time so the sending operation
                will not block - it shouldn't need to block as the queue should always
                be empty at this point in the code. */
                xQueueSend( xQueue, &ulValueToSend, 0U );
        }
}


static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 100UL;

        /* Remove compiler warning about unused parameter. */
        ( void ) pvParameters;

        for( ;; )
        {
                /* Wait until something arrives in the queue - this task will block
                indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
                FreeRTOSConfig.h. */
                xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

                /*  To get here something must have been received from the queue, but
                is it the expected value?  If it is, toggle the LED. */
                if( ulReceivedValue == ulExpectedValue )
                {
//                        vParTestToggleLED( mainTASK_LED );
                        ulReceivedValue = 0U;
                }
        }
}

//Function stub to get rid of compile time errors.
//Need to implement this functionality somehow
void vApplicationIRQHandler( uint32_t ulICCIAR ) {
}

/******************************* End of file *********************************/
