/*
 * rtg_main.c
 *
 * External task implementations (StartTask1, StartTask2) and SEGGER SystemView initialization.
 * Task objects are created by STM32CubeMX in Core/Src/freertos.c (MX_FREERTOS_Init).
 */
#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "rtg_main.h"

#if (USE_SEGGER_SW == 1)
volatile uint8_t sysview_init = 0;
#define SYSVIEW_WAIT_TIMEOUT_MS    5000U
#endif

void rtg_main(void)
{
	// Before SystemView is initialized (sysview_init == 0), this prints via printf (UART)
	RTG_PRINTF("Starting Application\r\n");

#if (USE_SEGGER_SW == 1)
	// Initialize SEGGER SystemView before tasks are created in MX_FREERTOS_Init()
	SEGGER_SYSVIEW_Conf();
	sysview_init = 1;
#endif
}

// Default task created by CubeMX (configured as 'As weak' in CubeMX):
// Immediately terminates itself on startup so only Task1 and Task2 run.
void StartDefaultTask(void *argument)
{
	(void)argument;
	osThreadExit();
}

// FreeRTOS task (created by CubeMX as Task1 with osPriorityAboveNormal):
// * Turns on the blue LED (LD2) to indicate the FreeRTOS scheduler has started.
// * When USE_SEGGER_SW == 1, waits for the SystemView host app to start Record mode.
// * Flashes the green LED (LD1) every 500ms.
void StartTask1(void *argument)
{
	(void)argument;
	uint32_t iterationCount = 0;

	// Turn on the blue LED, to indicate the FreeRTOS scheduler has started.
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

#if (USE_SEGGER_SW == 1)
	// Wait until the SystemView host application starts Record mode so no messages are lost.
	while (SEGGER_SYSVIEW_IsStarted() == 0);
#endif

	RTG_PRINTF("Task1: loop starting...\r\n");

	for (;;) {
		// Toggle the green LED
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);

		iterationCount++;

		RTG_PRINTF("Task1: LED toggled. Iteration: %u\r\n", (unsigned int)iterationCount);

		// Delay 500ms (CMSIS-RTOS2)
		osDelay(500);
	}
}

// FreeRTOS task (created by CubeMX as Task2 with osPriorityNormal):
// * Runs after Task1 blocks on osDelay(500).
// * Flashes the red LED (LD3) every 500ms.
void StartTask2(void *argument)
{
	(void)argument;
	uint32_t iterationCount = 0;

	for (;;) {
		// Toggle the red LED
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

		iterationCount++;

		RTG_PRINTF("Task2: LED toggled. Iteration: %u\r\n", (unsigned int)iterationCount);

		// Delay 500ms (CMSIS-RTOS2)
		osDelay(500);
	}
}
