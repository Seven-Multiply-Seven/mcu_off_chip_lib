#define GUI_GLOABL
#include "gui.h"

void gui_line(put_pixel pen, gui_vec* p0, gui_vec* p1)
{
    int dx = p1->x - p0->x;
    int dy = p1->y - p0->y;
    int ux = dx >0 ?1:-1;
    int uy = dx >0 ?1:-1;
    int dx2 = dx <<1;
    int dy2 = dy <<1;

    if(abs(dx)>abs(dy))
    {
        int e = -dx;
        int x = p0->x;
        int y = p0->y;
        for (; x < p1->x; x += ux)
        {
            pen(x, y);
            e += dy2;
            if (e > 0)
            {
                y += uy;
                e -= dx2;
            }
        }
    }
    else
    {
        int e = -dy;
        int x = p0->x;
        int y = p0->y;
        for (; y < p1->y; y += uy)
        {
            pen(x, y);
            e += dx2;
            if (e > 0)
            {
                x += ux;
                e -= dy2;
            }
        }
    }

}

static inline void __circle_points(put_pixel pen, gui_vec* pc, const unsigned int x, const unsigned int y)
{
    const unsigned int xc = pc->x;
    const unsigned int yc = pc->y;

    pen(xc+x, yc+y);
    pen(xc+x, yc-y);
    pen(xc-x, yc-y);
    pen(xc-x, yc+y);
    pen(xc+y, yc+x);
    pen(xc+y, yc-x);
    pen(xc-y, yc-x);
    pen(xc-y, yc+x);
}

void gui_circle(put_pixel pen, gui_vec* rc, unsigned int r, bool fill)
{
    int x, y, d;

    x=0;
    y = r;
    d = 3 - (r<<1);

    __circle_points(pen, rc, x, y);

    if(fill)
    {
        int yi;
        while(x <= y)
        {
            for(yi = x; yi <= y; yi++)
                __circle_points(pen, rc, x, yi);
            if(d<0)
                d += (x<<2) + 6;
            else
            {
                d += ((x-y)<<2) + 10;
                y--;
            }
            x++;
        }
    }
    else
    {
        while(x <= y)
        {
            __circle_points(pen, rc, x, y);
            if(d<0)
                d += (x<<2) + 6;
            else
            {
                d += ((x-y)<<2) + 10;
                y--;
            }
            x++;
        }
    }
}
