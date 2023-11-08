#include "at32f435_437_clock.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
// #include "hUsart/hal_usart.h"

void LedToggleThread(void *arg);
// void UsartHelloThread(void *arg);

uint8_t GlobalHello[] = "GLOBAL Hello\r\n";
uint8_t buf[] = "hello world\r\n";

int main(void)
{
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    system_clock_config();
    xTaskCreate(LedToggleThread, "LedTask", 90, NULL, 3, NULL);
    // xTaskCreate(UsartHelloThread, "Hello_task", 512, NULL, 4, NULL);
    vTaskStartScheduler();

    while (1)
    {
        /* code */
    }

    return 0;
}

// void UsartHelloThread(__attribute__((unused)) void *arg)
// {
//     gpio_init_type gpio_uart;

//     crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
//     gpio_default_para_init(&gpio_uart);

//     gpio_uart.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
//     gpio_uart.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
//     gpio_uart.gpio_mode = GPIO_MODE_MUX;
//     gpio_uart.gpio_pins = GPIO_PINS_9;
//     gpio_uart.gpio_pull = GPIO_PULL_NONE;

//     gpio_init(GPIOA, &gpio_uart);
//     gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9, GPIO_MUX_7);

//     hUsartHand_t u1Hand = {.usart_x = USART1,
//                            .param = {115200, USART_DATA_8BITS, USART_STOP_1_BIT}};

//     hUsartInit(&u1Hand);

//     hEdmaUsartHand_t edma1Hand;
//     taskENTER_CRITICAL();

//     hEdmaDescriptorData_t headData;
//     headData.bufSize = sizeof(GlobalHello);
//     headData.ptrBuf = GlobalHello;
//     headData.usart_x = u1Hand.usart_x;
//     hUsartEdmaAddDescriptors(&headData, EDMA_STREAM1);

//     hEdmaDescriptorData_t simpleData;
//     simpleData.bufSize = sizeof(buf);
//     simpleData.ptrBuf = buf;
//     simpleData.usart_x = u1Hand.usart_x;
//     hUsartEdmaAddDescriptors(&simpleData, EDMA_STREAM1);

//     edma1Hand.edma_streamx = EDMA_STREAM1;
//     edma1Hand.edmamux_channelx = EDMAMUX_CHANNEL1;
//     edma1Hand.usart_x = USART1;
//     hUsartEdmaInit(&edma1Hand);

//     PrintList(EDMA_STREAM1);

//     taskEXIT_CRITICAL();
//     while (1)
//     {
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//         hUsartEdmaStartTransmit(&edma1Hand);

//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

void LedToggleThread(__attribute__((unused)) void *arg)
{
    gpio_init_type gpio_init_struct;
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = GPIO_PINS_13;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(GPIOD, &gpio_init_struct);

    while (1)
    {
        GPIOD->odt ^= GPIO_PINS_13;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
