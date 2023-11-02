#include "main.h"

void LedToggleThread(void *arg);
void TelnetThread(void *arg);
int main(void)
{
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    system_clock_config();
    xTaskCreate(LedToggleThread, "LedTask", 90, NULL, 3, NULL);
    vTaskStartScheduler();

    while (1)
    {
        /* code */
    }

    return 0;
}

void TelnetThread(__attribute__((unused)) void *arg)
{
    error_status status;
    uint32_t count = 100;
    status = emac_system_init();

    while (status == ERROR)
        ;

    tcpip_stack_init();

    helloworld_init();
    while (count--)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

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