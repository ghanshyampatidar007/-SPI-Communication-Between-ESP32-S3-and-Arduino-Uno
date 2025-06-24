// Zephyr ESP32-S3 SPI Master - Send & receive 1 byte to/from Arduino SPI slave.


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(spi_master, LOG_LEVEL_INF);

#define SPI_NODE DT_NODELABEL(spi2)
#define CS_GPIO_CONTROLLER DT_NODELABEL(gpio0)
#define CS_PIN 10

int main(void)
{
    const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);
    const struct device *cs_gpio = DEVICE_DT_GET(CS_GPIO_CONTROLLER);

    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return 0;
    }

    if (!device_is_ready(cs_gpio)) {
        LOG_ERR("CS GPIO device not ready");
        return 0;
    }

    gpio_pin_configure(cs_gpio, CS_PIN, GPIO_OUTPUT_HIGH);

    struct spi_config spi_cfg = {
        .frequency = 1000000,
        .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
        .slave = 0,
    };

    uint8_t tx_data = 0x42; // Send ASCII 'B'
    uint8_t rx_data = 0x00;

    struct spi_buf tx_buf = {
        .buf = &tx_data,
        .len = 1
    };
    struct spi_buf rx_buf = {
        .buf = &rx_data,
        .len = 1
    };
    struct spi_buf_set tx = {
        .buffers = &tx_buf,
        .count = 1
    };
    struct spi_buf_set rx = {
        .buffers = &rx_buf,
        .count = 1
    };

    while (1) {
        gpio_pin_set(cs_gpio, CS_PIN, 0); // CS LOW
        int ret = spi_transceive(spi_dev, &spi_cfg, &tx, &rx);
        gpio_pin_set(cs_gpio, CS_PIN, 1); // CS HIGH

        if (ret) {
            LOG_ERR("SPI transceive failed: %d", ret);
        } else {
           // LOG_INF("Sent: 0x%02X | Received: 0x%02X", tx_data, rx_data);
            LOG_INF("Sent: %c | Received: %c", tx_data, rx_data);
        }

        k_sleep(K_SECONDS(2));
    }
}

/*
// Zephyr ESP32-S3 SPI Master - Send & receive string to/from Arduino SPI slave.


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(spi_master, LOG_LEVEL_INF);

#define SPI_NODE DT_NODELABEL(spi2)
#define CS_GPIO_CONTROLLER DT_NODELABEL(gpio0)
#define CS_PIN 10

void main(void)
{
    const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);
    const struct device *cs_gpio = DEVICE_DT_GET(CS_GPIO_CONTROLLER);

    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return;
    }

    if (!device_is_ready(cs_gpio)) {
        LOG_ERR("CS GPIO device not ready");
        return;
    }

    gpio_pin_configure(cs_gpio, CS_PIN, GPIO_OUTPUT_HIGH);

    struct spi_config spi_cfg = {
        .frequency = 1000000,
        .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
        .slave = 0,
    };

    const char tx_str[] = "22222";  // message to send
    char rx_buf_data[sizeof(tx_str)] = {0};   // same size receive buffer

    struct spi_buf tx_buf = {
        .buf = (void *)tx_str,
        .len = strlen(tx_str) //sizeof(tx_str)
    };                   

    struct spi_buf rx_buf = {
        .buf = rx_buf_data,
        .len = strlen(tx_str)  //sizeof(rx_buf_data)
    };

    struct spi_buf_set tx = {
        .buffers = &tx_buf,
        .count = 1
    };

    struct spi_buf_set rx = {
        .buffers = &rx_buf,
        .count = 1
    };

    while (1) {
        gpio_pin_set(cs_gpio, CS_PIN, 0); // CS LOW

        int ret = spi_transceive(spi_dev, &spi_cfg, &tx, &rx);

        gpio_pin_set(cs_gpio, CS_PIN, 1); // CS HIGH

        if (ret) {
            LOG_ERR("SPI transceive failed: %d", ret);
        } else {
            LOG_INF("Sent: \"%s\"", tx_str);
            LOG_INF("Received: \"%s\"", rx_buf_data);
        }

        k_sleep(K_SECONDS(2));
    }
}
*/
