/*
 * rtg_main.h
 *
 * Application entry point and external FreeRTOS task declarations.
 * Called from MX_FREERTOS_Init() (USER CODE BEGIN Init) in Core/Src/freertos.c.
 */

#ifndef INC_RTG_MAIN_H_
#define INC_RTG_MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#if (USE_SEGGER_SW == 1)
	#include "SEGGER_SYSVIEW.h"
	extern volatile uint8_t sysview_init;

	#define RTG_PRINTF(...)    do { \
		if (sysview_init) { \
			SEGGER_SYSVIEW_PrintfHost(__VA_ARGS__); \
			printf(__VA_ARGS__); \
		} else { \
			printf(__VA_ARGS__); \
		} \
	} while (0)
#else
	#define RTG_PRINTF(...)    printf(__VA_ARGS__)
#endif

void rtg_main(void);
void StartDefaultTask(void *argument);
void StartTask1(void *argument);
void StartTask2(void *argument);

#endif /* INC_RTG_MAIN_H_ */
