#define SSD_GLOBAL
#include "ssd1306.h"
#include "ssd1306_font.h"

static void spi_write_dat(const ssd_spi_opt* opt, uint8_t dat)
{
    opt->dc(true);
    opt->io(dat, false);
}

static void spi_write_cmd(const ssd_spi_opt* opt, uint8_t cmd)
{
    opt->dc(false);
    opt->io(cmd, false);
}

static inline void spi_set_page_pos(const ssd_spi_opt* opt, const unsigned int x, const unsigned int y)
{
    spi_write_cmd(opt, 0xb0 + y);
    spi_write_cmd(opt, (x&0xf0)>>4 | 0x10);
    spi_write_cmd(opt, (x&0x0f) | 0x01);
}

void ssd_spi_move_to(const ssd_spi_opt* opt, unsigned int x, unsigned int y)
{
    x &= 127;
    y &= 7;
    spi_set_page_pos(opt, x, y);
}

void ssd_spi_fill(const ssd_spi_opt* opt, const uint8_t dat)
{
    int i, j;
    for(i=8; i>0; i--)
    {
        spi_write_cmd(opt, 0xb0 + i);
        spi_write_cmd(opt, 0x01);
        spi_write_cmd(opt, 0x10);
        for(j=128; j>0; j--)
            spi_write_dat(opt, dat);
    }
}

void ssd_spi_page_write(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const uint8_t dat)
{
    x &= 127;
    y &= 7;
    spi_set_page_pos(opt, x, y);
    spi_write_dat(opt, dat);
}

void ssd_spi_set_pixel(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const bool light)
{
    x &= 127;
    y &= 63;
    spi_set_page_pos(opt, x, y>>3);
    if(light)
        spi_write_dat(opt, 1<<(y&7));
    else
        spi_write_dat(opt, ~(1<<(y&7)));
}

static const uint8_t code spi_init_cmds[] = {
    0xae, 0x00, 0x10, 0x40, 0x81,
    0xcf, 0xa1, 0xc8, 0xa6, 0xa8,
    0x3f, 0xd3, 0x00, 0xd5, 0x80,
    0xd9, 0xf1, 0xda, 0x12, 0xdb,
    0x40, 0x20, 0x02, 0x8d, 0x14,
    0xa4, 0xa6, 0xaf,
    };

void ssd_spi_init(const ssd_spi_opt* opt)
{
    const uint8_t* buff = spi_init_cmds;
    size_t i;

    opt->res(false);
    opt->sleep(200*1000);
    opt->res(true);

    opt->dc(false);
    for(i=sizeof(spi_init_cmds)-1; i>0; i--)
    {
        opt->io(*buff++, true);
    }
    opt->io(*buff, false);

    ssd_spi_fill(opt, 0x00);
    spi_set_page_pos(opt, 0, 0);
}

static void spi_put_char(const ssd_spi_opt* opt, const uint8_t ch)
{
    spi_write_dat(opt, FASCII[ch-' '][0]);
    spi_write_dat(opt, FASCII[ch-' '][1]);
    spi_write_dat(opt, FASCII[ch-' '][2]);
    spi_write_dat(opt, FASCII[ch-' '][3]);
    spi_write_dat(opt, FASCII[ch-' '][4]);
    spi_write_dat(opt, FASCII[ch-' '][5]);
}

static inline bool is_shown_char(char ch)
{
    return ((ch>=' ' && ch<='=') || ch == '\n');
}

void ssd_spi_puts(const ssd_spi_opt* opt, unsigned int x, unsigned int y, const char* str)
{
    int flg = 0;

    if(*str)
        return;

    do{
        x &= 127;
        /* no enough space for next character? */
        if(x > 127-6)
        {
            x=0;
            y++;
            flg++;
        }
        y &= 7;
        /* need to reset position? */
        if(flg)
        {
            spi_set_page_pos(opt, x, y);
            flg = 0;
        }
        if(is_shown_char(*str))
        {
            /* need to move to next line? */
            if('\n' == *str)
            {
                x = 0;
                y++;
            }
            else
            {
                spi_put_char(opt, *str);
                x += 6;
            }
        }
        else
            str++;
    }while(*str++);
}

void ssd_spi_puts_mv(const ssd_spi_opt* opt, unsigned int* x, unsigned int* y, const char* str)
{
    int flg = 0;

    if(*str)
        return;

    do{
        *x &= 127;
        /* no enough space for next character? */
        if(*x > 121)
        {
            *x=0;
            (*y)++;
            flg++;
        }
        *y &= 7;
        /* need to reset position? */
        if(flg)
        {
            spi_set_page_pos(opt, *x, *y);
            flg = 0;
        }
        if(is_shown_char(*str))
        {
            /* need to move to next line? */
            if('\n' == *str)
            {
                *x = 0;
                (*y)++;
            }
            else
            {
                spi_put_char(opt, *str);
                *x += 6;
            }
        }
        else
            str++;
    }while(*str++);
}

