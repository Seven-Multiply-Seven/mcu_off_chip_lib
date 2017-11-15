/****
    *@file:nrf24l01.c
    *@brief:source file for nrf24l01 device driver.
    *@date: 10/30 2017
    *@author: sms, TongzhouXueshe of DMU, All rights reserved.
    *@attention: file encoding setting: UTF-8
****/
#define NRF_GLOBAL
#include "nrf24l01.h"
/* encoding setting: UTF-8 */
/* NRF24L01 Register operation command */
#define READ_REG_NRF    0x00  /**< 读配置寄存器,低5位为寄存器地址 */
#define WRITE_REG_NRF   0x20  /**< 写配置寄存器,低5位为寄存器地址 */
#define RD_RX_PLOAD     0x61  /**< 读RX有效数据,1~32字节 */
#define WR_TX_PLOAD     0xA0  /**< 写TX有效数据,1~32字节 */
#define FLUSH_TX        0xE1  /**< 清除TX FIFO寄存器.发射模式下用 */
#define FLUSH_RX        0xE2  /**< 清除RX FIFO寄存器.接收模式下用 */
#define REUSE_TX_PL     0xE3  /**< 重新使用上一包数据,CE为高,数据包被不断发送. */
#define NOP             0xFF  /**< 空操作,可以用来读状态寄存器 */
/* SPI(NRF24L01) Register address */
#define CONFIG          0x00  /**< 配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能; */
                              /**< bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能 */
#define EN_AA           0x01  /**< 使能自动应答功能  bit0~5,对应通道0~5 */
#define EN_RXADDR       0x02  /**< 接收地址允许,bit0~5,对应通道0~5 */
#define SETUP_AW        0x03  /**< 设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节; */
#define SETUP_RETR      0x04  /**< 建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us */
#define RF_CH           0x05  /**< RF通道,bit6:0,工作通道频率; */
#define RF_SETUP        0x06  /**< RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益 */
#define STATUS          0x07  /**< 状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发 */
                              /**< bit5:数据发送完成中断;bit6:接收数据中断; */

#define OBSERVE_TX      0x08  /**< 发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器 */
#define CD              0x09  /**< 载波检测寄存器,bit0,载波检测; */
#define RX_ADDR_P0      0x0A  /**< 数据通道0接收地址,最大长度5个字节,低字节在前 */
#define RX_ADDR_P1      0x0B  /**< 数据通道1接收地址,最大长度5个字节,低字节在前 */
#define RX_ADDR_P2      0x0C  /**< 数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等; */
#define RX_ADDR_P3      0x0D  /**< 数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等; */
#define RX_ADDR_P4      0x0E  /**< 数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等; */
#define RX_ADDR_P5      0x0F  /**< 数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等; */
#define TX_ADDR         0x10  /**< 发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等 */
#define RX_PW_P0        0x11  /**< 接收数据通道0有效数据宽度(1~32字节),设置为0则非法 */
#define RX_PW_P1        0x12  /**< 接收数据通道1有效数据宽度(1~32字节),设置为0则非法 */
#define RX_PW_P2        0x13  /**< 接收数据通道2有效数据宽度(1~32字节),设置为0则非法 */
#define RX_PW_P3        0x14  /**< 接收数据通道3有效数据宽度(1~32字节),设置为0则非法 */
#define RX_PW_P4        0x15  /**< 接收数据通道4有效数据宽度(1~32字节),设置为0则非法 */
#define RX_PW_P5        0x16  /**< 接收数据通道5有效数据宽度(1~32字节),设置为0则非法 */
#define NRF_FIFO_STATUS 0x17  /**< FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留 */
                              /**< bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环; */

#define TX_FULL         0x01  /**< TX FIFO 寄存器满标志。 1 为 满，0为 不满 */
#define MAX_TX          0x10  /**< 达到最大发送次数中断 */
#define TX_OK           0x20  /**< TX发送完成中断 */
#define RX_OK           0x40  /**< 接收到数据中断 */

static inline uint8_t read_reg(const nrf_opt* opt, const uint8_t reg)
{
    opt->io(reg, true);
    return opt->io(NOP, false);
}

static inline void write_reg(const nrf_opt* opt, const uint8_t reg, const uint8_t dat)
{
    opt->io(reg, true);
    opt->io(dat, false);
}

static uint8_t read_buf(const nrf_opt* opt, const uint8_t reg, uint8_t* buf, const size_t len)
{
    size_t i;
    opt->io(reg, true);
    for(i=len-1; i>0; i--)
    {
        *buf++ = opt->io(NOP, true);
    }
    return *buf = opt->io(NOP, false);
}

static void write_buf(const nrf_opt* opt, const uint8_t reg, const uint8_t* buf, const size_t len)
{
    size_t i;
    opt->io(reg, true);
    for(i=len-1; i>0; i--)
    {
        opt->io(*buf++, true);
    }
    opt->io(*buf, false);
}

static inline uint8_t clear_status(const nrf_opt* opt)
{
    uint8_t st;
    st = read_reg(opt, STATUS);
    write_reg(opt, WRITE_REG_NRF+STATUS, st);
    return st;
}

bool nrf_is_connect(nrf_obj* obj)
{
    int i;
    uint8_t test[] = {0xa5, 0xa5, 0xa5, 0xa5, 0xa5};

    write_buf(&obj->opt, WRITE_REG_NRF+TX_ADDR, test, 5);
    write_buf(&obj->opt, TX_ADDR, test, 5);
    read_buf(&obj->opt, TX_ADDR, test, 5);

    for(i=5; i>0; i--)
    {
        if(test[i] != 0xa5)
            return false;
    }
    /**< 确保收发长度不超过32个字节 */
    obj->tx_pload_width &= 31;
    obj->rx_pload_width &= 31;

    return true;
}

void nrf_set_tx_mode(const nrf_obj* obj)
{
    obj->opt.ce(false);

    write_buf(&obj->opt, WRITE_REG_NRF+TX_ADDR, obj->tx_addr, TX_ADR_WIDTH);
    write_reg(&obj->opt, FLUSH_TX, NOP);                 /**< 清除TX FIFO中的数据 */
    write_reg(&obj->opt, WRITE_REG_NRF+EN_AA, 0x01);     /**< 使能通道0的自动应答 */
    write_reg(&obj->opt, WRITE_REG_NRF+EN_RXADDR, 0x01); /**< 使能通道0的接收地址 */
    write_reg(&obj->opt, WRITE_REG_NRF+SETUP_RETR, 0x1a);/**< 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
    write_reg(&obj->opt, WRITE_REG_NRF+RF_CH, 40);       /**< 设置RF通道为40 */
    write_reg(&obj->opt, WRITE_REG_NRF+RF_SETUP, 0x0f);  /**< 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */
    write_reg(&obj->opt, WRITE_REG_NRF+CONFIG, 0x0e);    /**< 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断 */

    obj->opt.ce(true);

    obj->opt.sleep(2000);
}

void nrf_set_rx_mode(const nrf_obj* obj)
{
    obj->opt.ce(false);

    write_buf(&obj->opt, WRITE_REG_NRF+RX_ADDR_P0, obj->rx_addr, RX_ADR_WIDTH);
    write_reg(&obj->opt, WRITE_REG_NRF+EN_AA, 0x01);             /**< 使能通道0的自动应答 */
    write_reg(&obj->opt, WRITE_REG_NRF+EN_RXADDR, 0x01);         /**< 使能通道0的接收地址 */
    write_reg(&obj->opt, WRITE_REG_NRF+RF_CH, 40);               /**< 设置RF通信频率 */
    write_reg(&obj->opt, WRITE_REG_NRF+RX_PW_P0, obj->rx_pload_width);/**< 选择通道0的有效数据宽度 */
    write_reg(&obj->opt, WRITE_REG_NRF+RF_SETUP, 0x0f);          /**< 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */
    write_reg(&obj->opt, WRITE_REG_NRF+CONFIG, 0x0f);            /**< 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 */

    obj->opt.ce(true);

    obj->opt.sleep(2000);
}

void nrf_send_packet(nrf_obj* obj, const uint8_t* buff)
{
    uint8_t st;

    obj->opt.ce(false);
    write_buf(&obj->opt, WR_TX_PLOAD, buff, obj->tx_pload_width);
    obj->opt.ce(true);

    while(obj->opt.irq);

    st = clear_status(&obj->opt);

    if( st&MAX_TX )
    {
        write_reg(&obj->opt, FLUSH_TX, NOP);
        obj->status |= nrf_full;
    }
    if( st&TX_OK )
        obj->status |= nrf_ok;
    else
        obj->status |= nrf_error;
}

void nrf_crawl_packet(nrf_obj* obj, uint8_t* buff)
{
    char st;
    st = clear_status(&obj->opt);
    if(st & RX_OK)
    {
        obj->opt.ce(false);
        read_buf(&obj->opt, RD_RX_PLOAD, buff, obj->rx_pload_width);
        obj->opt.ce(true);
        obj->status |= nrf_ok;
        return;
    }
    obj->status |= nrf_timeout;
}

void nrf_it_callback(const nrf_obj* obj, const uint8_t* tx_buf, uint8_t* rx_buf, trans fail_hook)
{
    char st;
    st = clear_status(&obj->opt);

    if(st & TX_OK)
    {
        if(tx_buf)
        {
            obj->opt.ce(false);
            write_buf(&obj->opt, WR_TX_PLOAD, tx_buf, obj->tx_pload_width);
            obj->opt.ce(true);
        }
    }
    if(st & RX_OK)
    {
        if(rx_buf)
        {
            obj->opt.ce(false);
            read_buf(&obj->opt, RD_RX_PLOAD, rx_buf, obj->rx_pload_width);
            obj->opt.ce(true);
        }
        write_reg(&obj->opt, FLUSH_RX, NOP);
    }
    if(st & MAX_TX)
    {
        write_reg(&obj->opt, FLUSH_TX, NOP);
        if(fail_hook)
            fail_hook();
    }
}
