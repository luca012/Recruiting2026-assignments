#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "st3215.h" 

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

struct st3215_device servo = {
    // DEVICE_DT_GET looks for the node "&usart2" in the device tree
    .uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart2)), 
    
    // GPIO_DT_SPEC_GET looks for the node "enable_pin" and takes its "gpios" property
    .enable_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(enable_pin), gpios) 
};

int main(void) {
    int ret;

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED not ready!");
        return 0;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    LOG_INF("Starting ST3215 driver initialization...");
    
    ret = st3215_init(&servo); 
    if (ret < 0) {
        LOG_ERR("Error: failed to initialize the servo (Code: %d)", ret);
        return 0;
    }

    LOG_INF("System ready. Starting main loop...");

    uint16_t position = 1000; // between 0 and 4095, where 0 is 0° and 4095 is 360°

    while (1) {
        gpio_pin_toggle_dt(&led);

        st3215_set_angle(&servo, position);

        if (position == 1000) {
            position = 3000;
        } else {
            position = 1000;
        }

        k_msleep(2000);
    }

    return 0;
}