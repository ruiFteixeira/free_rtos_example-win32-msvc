#include "FreeRTOS.h"   // inclusão das bibliotecas do kernel FreeRTOS
#include "task.h"       // inclusão da biblioteca do módulo tasks
#include <stdio.h>      // biblioteca de standard input e output
#include "queue.h"      // inclusão da biblioteca do módulo queue
#include <string.h>     // biblioteca com funções para trabalhar com strings

QueueHandle_t canBufferSPEED;       // declaração da variável de handler da queue, escopo global
QueueHandle_t canBufferTEMP;        // declaração da variável de handler da queue, escopo global

void vTaskECU(void *pvParameters);      // declaração do prototipo da função da task da ECU
void vTaskAIRBAG(void *pvParameters);   // declaração do prototipo da função da task do Airbag
void vTaskFAN(void *pvParameters);      // declaração do prototipo da função da task da ventoinha do radiador

int main(void)  // ponto de entrada da aplicação
{
    char buffer[25];    // array de caracteres com 25 posições
    canBufferSPEED = xQueueCreate(6,sizeof(buffer));    // alocação de memória para a queue, com 6 posições, e cada posição com a dimensão de buffer
    canBufferTEMP = xQueueCreate(6,sizeof(buffer));     // alocação de memória para a queue, com 6 posições, e cada posição com a dimensão de buffer

    xTaskCreate(&vTaskECU, "ECU Task", 1024, NULL, 1, NULL);        // criar a task
    xTaskCreate(&vTaskFAN, "FAN Task", 1024, NULL, 1, NULL);        // criar a task
    xTaskCreate(&vTaskAIRBAG, "Airbag Task", 1024, NULL, 1, NULL);  // criar a task

    vTaskStartScheduler();  // escalonador, inicializa o kernel FreeRTOS

    return 0;   // retorna 0 para uma variavel de ambiente do sistema operativo quando termina
}

void vTaskECU(void *pvParameters) // função da task ECU, para preencher os "buffers de CAN" com os valores de velocidade do carro e temperatura do motor, respetivamente
{
    char txBuffer[25];      // array de caracteres com 25 posições
    for (int i=0;i<1;i++)   // (i<1) -> será executada apenas umas vez
    {
        sprintf(txBuffer, "Speed+112");     // escrever para a variavel txBuffer, a velocidade em km/h, com o prefixo Speed para permitir posteriormente fazer o parse 
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);   // escrever o conteúdo de txBuffer para a queue
        sprintf(txBuffer, "Speed+120");
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Speed+113");
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Speed+110");
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Speed+50");
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Speed+0");
        xQueueSend(canBufferSPEED, (void*) txBuffer, (TickType_t) 0);

        sprintf(txBuffer, "Temp+110");  // escrever para a variavel txBuffer, a temperatura do motor em graus celsius, com o prefixo Temp (temperatura) para permitir posteriormente fazer o parse
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);    // escrever o conteúdo de txBuffer para a queue
        sprintf(txBuffer, "Temp+113");
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Temp+118");
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Temp+105");
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Temp+100");
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);
        sprintf(txBuffer, "Temp+90");
        xQueueSend(canBufferTEMP, (void*) txBuffer, (TickType_t) 0);

        printf(" ECU Task - executing \r\n");
        vTaskDelay(pdMS_TO_TICKS(2000));    // delay de 2 segundos
    }
}

void vTaskAIRBAG(void *pvParameters)    // função da task do Airbag, para ler os "buffers de CAN" com os valores de velocidade do carro, e fazer atuar o airbag, caso se verifique uma desaceleração superior a 50 km/h entre dois valores consecutivos
{
    char rxBuffer[25];      // array de caracteres que será utilizado para armazenar a string lida da queue
    char *token1 = NULL;    // apontador do tipo char que será utilizado no processo de parsing da strind lida da queue
    int previousSpeed = 0;  // variavel para guardar o valor de velocidade anteriormente lido
    int actualSpeed = 0;    // variavel para guardar o valor de velocidade atualmente lido
    while(1){   // ciclo infinito
        if(0!=canBufferSPEED){  // caso a queue não esteja vazia
            xQueueReceive(canBufferSPEED, (void*) rxBuffer, (TickType_t) 4);    // ler a queue e armazenar o valor lido na variável rxBuffer
            printf("Speed CAN readings: %s\r\n",rxBuffer);  // imprimir para o ecrã o conteúdo de rxBuffer
            if(NULL != strstr(rxBuffer, "Speed")){          // se for encontrado em rxBuffer a string "Speed"
                token1 = strtok(rxBuffer,"+");              // fazer o parsing de "Speed+valor" para extrair apenas o valor, e deixar cair o prefixo "Speed"
                token1 = strtok(NULL,"+");
                strcpy(rxBuffer,token1);                    // copiar o valor extraido em token1, para substituir no rxBuffer e ficar apenas com o valor
                actualSpeed = atoi(rxBuffer);               // converter o valor de char para int, e atribuir à variavel actualSpeed
                if(50<(previousSpeed - actualSpeed)){       // caso se verifique uma "desaceleração" superior a 50 km/h
                    printf("!!! activate airbag !!!: %s\r\n",rxBuffer); // printf de "ativar o airbag" para simular o que seria um comando de atuação do airbag
                    vTaskDelay(pdMS_TO_TICKS(1000));        // delay de 1 segundo
                }
                previousSpeed = actualSpeed;    // passar o valor de velocidade lido nesta iteração, para ser utilizado na próxima iteração, na qual será o "valor anterior" para comparação
            }
            vTaskDelay(pdMS_TO_TICKS(1000));    // delay de 1 segundo
        }
    }
}

void vTaskFAN(void *pvParameters)   // função da task da ventoinha do cooler, para ler o "buffer de CAN" com os valores de temperatura do motor do carro, e ligar a ventoinha do radiador, caso a temperatura exceda os 115 graus celsius
{
    char rxBuffer[25];      // array de caracteres que será utilizado para armazenar a string lida da queue
    char *token1 = NULL;    // apontador do tipo char que será utilizado no processo de parsing da strind lida da queue
    int temperature = 0;    // variavel para guardar o valor de temperatura do motor
    while(1){   // ciclo infinito
        if(0!=canBufferTEMP){   // caso a queue não esteja vazia
            xQueueReceive(canBufferTEMP, (void*) rxBuffer, (TickType_t) 4); // ler a queue e armazenar o valor lido na variável rxBuffer
            printf("Temperature CAN readings: %s\r\n",rxBuffer);    // imprimir para o ecrã o conteúdo de rxBuffer
            if(NULL != strstr(rxBuffer, "Temp")){       // se for encontrado em rxBuffer a string "Temp" (de temperatura)
                token1 = strtok(rxBuffer,"+");          // fazer o parsing de "Temp+valor" para extrair apenas o valor, e deixar cair o prefixo "Temp"
                token1 = strtok(NULL,"+");
                strcpy(rxBuffer,token1);                // copiar o valor extraido em token1, para substituir no rxBuffer e ficar apenas com o valor
                temperature = atoi(rxBuffer);           // converter o valor de char para int, e atribuir à variavel temperature
                if(115<temperature){                    // caso a temperatura do motor exceda os 115 graus celsius
                    printf("!!! turn on the cooler FAN !!!: %s\r\n",rxBuffer);  // printf de "ligar a ventoinha do radiador" para simular o que seria um comando de atuação da ventoinha
                    vTaskDelay(pdMS_TO_TICKS(1000));    // delay de 1 segundo
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1000));    // delay de 1 segundo
        }
    }
}
