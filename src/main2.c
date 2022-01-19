#include "FreeRTOS.h"   // inclusão das bibliotecas do kernel FreeRTOS
#include "task.h"       // inclusão da biblioteca do módulo tasks
#include <stdio.h>      // biblioteca de standard input e output
#include "queue.h"      // inclusão da biblioteca do módulo queue
#include <string.h>

QueueHandle_t canBufferSPEED;    // declaração da variável de handler da queue
QueueHandle_t canBufferTEMP;    // declaração da variável de handler da queue

void vTaskECU(void *pvParameters);    // declaração do prototipo da função das task A
void vTaskAIRBAG(void *pvParameters);    // declaração do prototipo da função das task B
void vTaskFAN(void *pvParameters);    // declaração do prototipo da função das task B

int main(void)  // ponto de entrada do programa
{
    char buffer[25];
    canBufferSPEED = xQueueCreate(5,sizeof(buffer));
    canBufferTEMP = xQueueCreate(5,sizeof(buffer));

    xTaskCreate(&vTaskECU, "ECU Task", 1024, NULL, 1, NULL);    // criar a task
    xTaskCreate(&vTaskFAN, "FAN Task", 1024, NULL, 1, NULL);    // criar a task
    xTaskCreate(&vTaskAIRBAG, "Airbag Task", 1024, NULL, 1, NULL);    // criar a task

    vTaskStartScheduler();  // escalonador, inicializa o kernel FreeRTOS

    return 0;   // retorna 0 para uma variavel de ambiente do sistema operativo quando termina com sucesso
}

void vTaskECU(void *pvParameters)
{
    char txBuffer[25];
    for (int i=0;i<1;i++)
    {
        // sprintf(txBuffer, "Speed+100");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Speed+120");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Speed+110");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Speed+110");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Speed+50");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Speed+0");
        // xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);

        // sprintf(txBuffer, "Temp+100");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Temp+105");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Temp+120");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Temp+105");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Temp+100");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        // sprintf(txBuffer, "Temp+90");
        // xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);

        printf(" ECU Task - executing \r\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void vTaskAIRBAG(void *pvParameters)
{
    char rxBuffer[25];
    // char *token1 = NULL;
    // int previousSpeed = 0;
    // int actualSpeed = 0;
    while(1){
        if(0!=canBufferSPEED){
            xQueueReceive(canBufferSPEED, (void*) rxBuffer, (TickType_t) 4);
            printf("Speed CAN readings: %s\r\n",rxBuffer);
            // if(NULL != strstr(rxBuffer, "Speed")){
            //     token1 = strtok(rxBuffer,"+");
            //     token1 = strtok(NULL,"+");
            //     strcpy(rxBuffer,token1);
            //     actualSpeed = atoi(rxBuffer);
            //     if(50<(previousSpeed - actualSpeed)){
            //         printf("!!! activate aribag !!!: %s\r\n",rxBuffer);
            //         vTaskDelay(pdMS_TO_TICKS(1000));
            //     }
            //     previousSpeed = actualSpeed;
            // }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void vTaskFAN(void *pvParameters)
{
    char rxBuffer[25];
    // char *token1 = NULL;
    // int temperature = 0;
    while(1){
        if(0!=canBufferTEMP){
            xQueueReceive(canBufferTEMP, (void*) rxBuffer, (TickType_t) 4);
            printf("Temperature CAN readings: %s\r\n",rxBuffer);
            // if(NULL != strstr(rxBuffer, "Temp")){
            //     token1 = strtok(rxBuffer,"+");
            //     token1 = strtok(NULL,"+");
            //     strcpy(rxBuffer,token1);
            //     temperature = atoi(rxBuffer);
            //     if(115<temperature){
            //         printf("!!! turn on the cooler FAN !!!: %s\r\n",rxBuffer);
            //         vTaskDelay(pdMS_TO_TICKS(1000));
            //     }
            // }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
