#ifndef __SSD1306_H__
#   define __SSD1306_H__

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef SSD_GLOBAL
#   define SSD_EXT extern
#else
#   define SSD_EXT
#endif

#ifndef SPI_READ_WRITE_TYPE
#   define SPI_READ_WRITE_TYPE
typedef uint8_t(*spi_read_write)(uint8_t ch, bool cs);
#endif /* SPI_READ_WRITE_TYPE */

#ifndef GPIO_PIN_CTRL_TYPE
#   define GPIO_PIN_CTRL_TYPE
typedef void(*gpio_pin_ctrl)(bool set);
#endif /* GPIO_PIN_CTRL_TYPE */

#ifndef DELAY_US_TYPE
#   define DELAY_US_TYPE
typedef void(*delay_us)(uint32_t);
#endif /* DELAY_US_TYPE */

typedef struct
{
    spi_read_write      io;
    gpio_pin_ctrl       dc;
    gpio_pin_ctrl       res;
    delay_us            sleep;
}ssd_spi_opt;

SSD_EXT void ssd_spi_move_to(const ssd_spi_opt* opt, unsigned int x, unsigned int y);
SSD_EXT void ssd_spi_fill(const ssd_spi_opt* opt, const uint8_t dat);
SSD_EXT void ssd_spi_page_write(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const uint8_t dat);
SSD_EXT void ssd_spi_set_pixel(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const bool light);
SSD_EXT void ssd_spi_init(const ssd_spi_opt* opt);
SSD_EXT void ssd_spi_puts(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const char* str);
SSD_EXT void ssd_spi_puts_mv(const ssd_spi_opt* opt, unsigned int* x, unsigned int* y, const char* str);

#ifdef __cplusplus
    }
#endif

#endif /* __SSD1306_H__ */
