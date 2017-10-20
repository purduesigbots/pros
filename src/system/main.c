#include <stdio.h>

#include "rtos/FreeRTOS.h"
#include "rtos/Task.h"
#include "rtos/semphr.h"

#include "ifi/v5_api.h"

extern void rtos_initialize();

void firstTask(void* ign)
{
	while (1) {
		vexDisplayStringAt(10, 60, "hello world from a task!");
		printf("Hello World\n");
		vTaskDelay(500);
		vexBackgroundProcessing();
	}
}

StaticTask_t firstTaskBuffer;
StackType_t firstTaskStack[8192];

int main()
{
	vexDisplayStringAt(10, 40, "hello world!");

	rtos_initialize();

	xTaskCreateStatic(firstTask, "firstTask", 8192, NULL,
	                  configMAX_PRIORITIES - 5, firstTaskStack,
	                  &firstTaskBuffer);

	xTaskCreate(firstTask, "secondTask", 8192, NULL,
	            configMAX_PRIORITIES - 5, NULL);

	vTaskStartScheduler();

	printf("Failed to start Scheduler\n");
	for (;;)
		;
}
