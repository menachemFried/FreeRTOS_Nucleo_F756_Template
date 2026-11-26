/*
 * rtg_main.c
 *
 * Create your tasks here, then start the scheduler.
 */
#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtg_main.h"

#if (USE_SEGGER_SW == 1)
volatile uint8_t sysview_init = 0;
#define SYSVIEW_WAIT_TIMEOUT_MS    5000U
#endif

// FreeRTOS example-code:
void Task1(void *argument);
void Task2(void *argument);

void rtg_main(void)
{
	// FreeRTOS example-code:
	// 512 words (2048 bytes) gives plenty of stack for both SEGGER_SYSVIEW_PrintfHost and UART printf
	const static uint32_t stackSize = 512;

	// Before SystemView is initialized (sysview_init == 0), this prints via printf (UART)
	RTG_PRINTF("Starting Application\r\n");

#if (USE_SEGGER_SW == 1)
	// Initialize SEGGER SystemView
	SEGGER_SYSVIEW_Conf();
	sysview_init = 1;
#endif

	// FreeRTOS example-code:
	// Create Task1 and Task2, and start the FreeRTOS scheduler
	// Task1 has higher priority than Task2.
	if ( xTaskCreate(Task1, "task1", stackSize, NULL, tskIDLE_PRIORITY + 3, NULL)
			== pdPASS ) {
		if ( xTaskCreate(Task2, "task2", stackSize, NULL, tskIDLE_PRIORITY + 2,
				NULL) == pdPASS ) {
			// The scheduler shouldn't return unless there's a problem
			vTaskStartScheduler();
		}
	}

	/* Only reached if there was not enough heap for the idle/timer tasks. */
	RTG_PRINTF("Scheduler failed to start\r\n");
	while(1);
}


// FreeRTOS example-code:
// FreeRTOS task:
// * Turns on the blue LED, to indicate the FreeRTOS scheduler has started.
// * If the code that uses SystemView is un-commented:
//   * When the task is started, it waits up to SYSVIEW_WAIT_TIMEOUT_MS for the SystemView app in Record-mode
// * The task flashes the green LED
void Task1(void *argument) {

    uint32_t iterationCount = 0;

    // Turn on the blue LED, to indicate the FreeRTOS scheduler has started.
    // * LD1 and LD2 are specified in the dev-board's "User Manual"
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

#if (USE_SEGGER_SW == 1)
    // SystemView example-code:
    // The SystemView app's Record-mode must be started after the FreeRTOS scheduler is started.
    // To get all of the SEGGER_SYSVIEW_PrintfHost() messages, spin until Record mode is started.
    while(SEGGER_SYSVIEW_IsStarted()==0);
#endif

    RTG_PRINTF("Task1: loop starting...\r\n");

    for (;;) {
        // Toggle the green LED
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);

        iterationCount++;

        // SystemView example-code:
        // Generate SystemView message
        RTG_PRINTF("Task1: LED toggled. Iteration: %u\r\n", (unsigned int)iterationCount);

        // Delay 500ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// FreeRTOS example-code:
// FreeRTOS task:
// * In Task1, if its code that uses SystemView is un-commented:
//   * Task2 will not run until Task1 detects the SystemView app is started in Record mode (or timeout).
// * Task2 flashes the red LED
void Task2(void *argument) {

    uint32_t iterationCount = 0;

    for (;;) {
        // Toggle the red LED
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        // Generate SystemView message
        iterationCount++;

        // SystemView example-code:
        RTG_PRINTF("Task2: LED toggled. Iteration: %u\r\n", (unsigned int)iterationCount);

        // Delay 500ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
