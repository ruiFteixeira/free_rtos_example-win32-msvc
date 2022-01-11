// #include "FreeRTOS.h"
// #include "task.h"

// void vTaskA(void *pvParameters);
// void vTaskB(void *pvParameters);

// int main(void)
// {
//     xTaskCreate(&vTaskA, "Task A", 1024, NULL, 1, NULL);
//     xTaskCreate(&vTaskB, "Task B", 1024, NULL, 1, NULL);

//     vTaskStartScheduler();

//     return 0;
// }

// void vTaskA(void *pvParameters)
// {
//     for (;;)
//     {
//         printf("Task A - X\r\n");
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void vTaskB(void *pvParameters)
// {
//     for (;;)
//     {
//         printf("Task B - X\r\n");
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }
