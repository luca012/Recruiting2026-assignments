#include "st3215.h"
#include <zephyr/logging/log.h>

// zepyhr logger for the driver, useful for debugging
LOG_MODULE_REGISTER(st3215_servo, LOG_LEVEL_DBG);

int st3215_init(struct st3215_device *dev) {
    LOG_INF("Initializing ST3215 Servo Driver...");

    // check if the UART device (PD5/PD6) is ready to be used
    if (!device_is_ready(dev->uart_dev)) {
        LOG_ERR("UART device not ready");
        return -ENODEV; // Error: No Device
    }

    // check if the GPIO pin (PD7) is ready
    if (!gpio_is_ready_dt(&dev->enable_pin)) {
        LOG_ERR("Direction Control GPIO not ready");
        return -ENODEV;
    }

    // configure the Enable pin as an OUTPUT and set it to LOW initially.
    // LOW means "listen" mode (RX active, TX disabled).
    int ret = gpio_pin_configure_dt(&dev->enable_pin, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure Direction Control GPIO");
        return ret;
    }

    LOG_INF("ST3215 Servo Driver Initialized Successfully");
    return 0;
}

int st3215_set_angle(struct st3215_device *dev, uint16_t angle) {
    // We will write the packet formatting and UART sending logic here next!
    return 0;
}

int st3215_get_angle(struct st3215_device *dev, uint16_t *angle) {
    // We will write the UART receiving and parsing logic here later!
    return 0;
}