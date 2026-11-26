/*
 * rtg_main.h
 *
 * Application entry point. Called from main() (USER CODE BEGIN 2)
 * after CubeMX has initialised the clocks and peripherals.
 */

#ifndef INC_RTG_MAIN_H_
#define INC_RTG_MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"

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

#endif /* INC_RTG_MAIN_H_ */
