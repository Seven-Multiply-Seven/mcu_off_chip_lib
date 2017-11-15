/****
    *@file:nrf24l01.h
    *@brief:head file for nrf24l01 device driver.
    *@date: 10/30 2017
    *@author: sms, TongzhouXueshe of DMU, All rights reserved.
    *@attention: file encoding setting: UTF-8
****/
#ifndef __NRF24L01_H__
#   define __NRF24L01_H__

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef NRF_GLOBAL
#   define NRF_EXT extern
#else
#   define NRF_EXT
#endif

#define TX_ADR_WIDTH (5)
#define RX_ADR_WIDTH (5)

#ifndef SPI_READ_WRITE_TYPE
#   define SPI_READ_WRITE_TYPE
typedef uint8_t(*spi_read_write)(uint8_t ch, bool cs);
#endif /* SPI_READ_WRITE_TYPE */

#ifndef GPIO_PIN_CTRL_TYPE
#   define GPIO_PIN_CTRL_TYPE
typedef void(*gpio_pin_ctrl)(bool set);
#endif /* GPIO_PIN_CTRL_TYPE */

#ifndef GPIO_IS_PIN_SET_TYPE
#   define GPIO_IS_PIN_SET_TYPE
typedef bool(*gpio_is_pin_set)(void);
#endif /* GPIO_IS_PIN_SET_TYPE */

#ifndef DELAY_US_TYPE
#   define DELAY_US_TYPE
typedef void(*delay_us)(uint32_t);
#endif /* DELAY_US_TYPE */

#ifndef TRANS_TYPE
#   define TRANS_TYPE
typedef void(*trans)(void);
#endif /* TRANS_TYPE */

enum nrf_status
{
    nrf_ok = 0,
    nrf_full = 1,
    nrf_timeout = 2,
    nrf_error = 4,
};

typedef struct
{
    spi_read_write  io;
    gpio_pin_ctrl   ce;
    gpio_is_pin_set irq;
    delay_us        sleep;
}nrf_opt;

typedef struct
{
    nrf_opt         opt;
    uint8_t*        tx_addr;
    uint8_t*        rx_addr;
    uint8_t         tx_pload_width;
    uint8_t         rx_pload_width;
    uint8_t         status;
}nrf_obj;

NRF_EXT bool nrf_is_connect(nrf_obj* obj);
NRF_EXT void nrf_set_tx_mode(const nrf_obj* obj);
NRF_EXT void nrf_set_rx_mode(const nrf_obj* obj);
NRF_EXT void nrf_send_packet(nrf_obj* obj, const uint8_t* buff);
NRF_EXT void nrf_crawl_packet(nrf_obj* obj, uint8_t* buff);
NRF_EXT void nrf_it_callback(const nrf_obj* obj, const uint8_t* tx_buf, uint8_t* rx_buf, trans fail_hook);

#ifdef __cplusplus
    }
#endif

#endif /* __NRF24L01_H__ */
