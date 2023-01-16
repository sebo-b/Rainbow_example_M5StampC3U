#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "led_strip.h"

#include "rainbow.h"

#define LED_GPIO 2
#define BTN_GPIO 9
#define DEBOUNCE_MS 250

led_strip_handle_t setup_led() {

    led_strip_handle_t ledStripHandle;

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &ledStripHandle));

    /* Set all LED off to clear all pixels */
    led_strip_clear(ledStripHandle);

    return ledStripHandle;
}

void btm_clicked_ISR(void* args) {

    static TickType_t lastTick = 0;
    TickType_t currTick = xTaskGetTickCountFromISR();

    // debounce
    if ( (currTick - lastTick)*portTICK_PERIOD_MS < DEBOUNCE_MS) {
        lastTick = currTick;
        return;
    }

    TaskHandle_t taskHandle = (TaskHandle_t)args;
    BaseType_t xHigherPriorityTaskWoke = pdFALSE;

    vTaskNotifyGiveFromISR(taskHandle,&xHigherPriorityTaskWoke);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoke);

    lastTick = currTick;
}

void setup_btn(void* isrArgs) {
    ESP_ERROR_CHECK(gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(BTN_GPIO, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(BTN_GPIO, GPIO_INTR_NEGEDGE));

    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(BTN_GPIO, btm_clicked_ISR, isrArgs));
}


void led_task_f(void* pvParam) {

    rainbow_color_t color = RAINBOW_INIT;
    led_strip_handle_t ledStripHandle = (led_strip_handle_t)pvParam;

    uint32_t r = 1;     // start with OFF

    while (1) {

        if (r != 0) {   // btn pressed
            led_strip_clear(ledStripHandle);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    //block until next press
        }

        // divide by 4 to dim the led
        led_strip_set_pixel(ledStripHandle, 0, color[0]>>2, color[1]>>2, color[2]>>2);
        led_strip_refresh(ledStripHandle);
        color_next(color,4);

        r = ulTaskNotifyTake(pdTRUE,50 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {

    led_strip_handle_t ledStripHandle = setup_led();

    TaskHandle_t ledTaskHandle = NULL;
    xTaskCreate(led_task_f,"led_task",2048,ledStripHandle,5,&ledTaskHandle);

    setup_btn(ledTaskHandle);
}
