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

    LOG_INF("Running PING on the servo...");
    ret = st3215_ping(&servo);
    if (ret == 0) {
        LOG_INF("PING completed successfully. Servo is responsive.");
    } else {
        LOG_ERR("PING failed (Code: %d)", ret);
        return 0; // uncomment if you still want to continue even if the ping fails
    }

    LOG_INF("System ready. Starting main loop...");

    uint16_t target_angle = 1000; // between 0 and 4095, where 0 is 0 deg and 4095 is 360 de
    uint16_t current_angle = 0;

    while (1) {
        gpio_pin_toggle_dt(&led);

        st3215_set_angle(&servo, target_angle);

        k_msleep(500); // wait for the servo to move to the target position

        if (st3215_get_angle(&servo, &current_angle) == 0) {
            LOG_INF("Current position read: %d", current_angle);
        }

        target_angle = (current_angle == 1000) ? 3000 : 1000;
        
        k_msleep(2000);
    }

    return 0;
}