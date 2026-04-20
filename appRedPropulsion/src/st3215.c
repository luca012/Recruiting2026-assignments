#include "st3215.h"
#include <zephyr/logging/log.h>

// zepyhr logger for the driver, useful for debugging
LOG_MODULE_REGISTER(st3215_servo, LOG_LEVEL_DBG);

/* @brief Calculates the checksum of the ST3215 packet.
 * The checksum is the bitwise NOT of the sum of the bytes starting from the ID.
 */
static uint8_t st3215_calc_checksum(uint8_t *packet, uint8_t total_length) {
    uint32_t sum = 0;
    // Sommiamo dall'ID (indice 2) fino al byte prima del checksum
    for (int i = 2; i < total_length - 1; i++) {
        sum += packet[i];
    }
    return (uint8_t)(~sum); // NOT bit a bit
}

int st3215_init(struct st3215_device *dev) {
    LOG_INF("Initializing ST3215 Servo Driver...");

    // check if the UART device (PD5/PD6) is ready to be used
    if (!device_is_ready(dev->uart_dev)) {
        LOG_ERR("UART device not ready");
        return -ENODEV; // Error: No Device
    }

    // check if the GPIO pin (PD7) is ready
    if (!gpio_is_ready_dt(&dev->enable_pin)) {
        LOG_ERR("Direction control GPIO not ready");
        return -ENODEV;
    }

    // configure the Enable pin as an OUTPUT and set it to LOW initially.
    // LOW means "listen" mode (RX active, TX disabled).
    int ret = gpio_pin_configure_dt(&dev->enable_pin, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure direction control GPIO");
        return ret;
    }

    LOG_INF("ST3215 Servo Driver initialized successfully");
    return 0;
}

/*
 * @brief Sets the angle of the ST3215 servo.
 * @param dev Pointer to the ST3215 device.
 * @param angle Desired angle (0-4095).
 * @return 0 on success, a negative value on error.
 */
int st3215_set_angle(struct st3215_device *dev, uint16_t angle) {
    uint8_t packet[9];
    // since the angle input parameter is 16-bit, we need to split it into two bytes for the packet
    uint8_t angle_low = angle & 0xFF;
    uint8_t angle_high = (angle >> 8) & 0xFF;

    packet[0] = ST3215_HEADER;
    packet[1] = ST3215_HEADER;
    packet[2] = ST3215_ID_DEFAULT;   
    packet[3] = 0x05;
    packet[4] = ST3215_INST_WRITE;
    packet[5] = ST3215_REG_GOAL_POS;
    packet[6] = angle_low;
    packet[7] = angle_high;
    packet[8] = st3215_calc_checksum(packet, 9);

    LOG_INF("Sending angle command: %d (Checksum: 0x%02X)", angle, packet[8]);

    gpio_pin_set_dt(&dev->enable_pin, 1); // TX Mode to send data

    for (int i = 0; i < 9; i++) {
        uart_poll_out(dev->uart_dev, packet[i]); // writes byte by byte to the UART
    }

    k_usleep(100);
    gpio_pin_set_dt(&dev->enable_pin, 0); // back to RX mode after sending 

    return 0;
}