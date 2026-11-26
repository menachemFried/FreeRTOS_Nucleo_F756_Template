/*
 * io_tools.c
 *
 * UART3 stdio retargeting (__io_putchar, _write, _read) and vPrintString helper.
 */
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtg_main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart3;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

int _write(int file, char *ptr, int len)
{
	(void) file;  /* Not used, avoid warning */
	HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 0xFFFF);
	return len;
}

// for scanf
int _read(int file, char *ptr, int len)
{
	(void) file;
	(void) len;
	int ch = 0;
	HAL_UART_Receive(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	if (ch == 13)
	{
		ch = 10;
		HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	}
	else if (ch == 8)
	{
		ch = 0x30;
		HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	}

	*ptr = (char)ch;

	return 1;
}

void vPrintString(const char *pcString)
{
#if (USE_SEGGER_SW == 1)
	if (sysview_init) {
		SEGGER_SYSVIEW_PrintfHost("%s", pcString);
		return;
	}
#endif
	taskENTER_CRITICAL();
	{
		printf("%s", pcString);
	}
	taskEXIT_CRITICAL();
}
