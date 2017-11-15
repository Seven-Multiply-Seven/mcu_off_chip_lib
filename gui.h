#ifndef __GUI_H__
#   define __GUI_H__

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif /* __CPLUSPLUS */

#ifndef GUI_GLOBAL
#   define GUI_EXT extern
#else
#   define GUI_EXT
#endif

typedef void(*put_pixel)(unsigned int x, unsigned int y);
typedef struct
{
    unsigned int x, y;
}gui_vec;

GUI_EXT void gui_line(put_pixel pen, gui_vec* p0, gui_vec* p1);
GUI_EXT void gui_circle(put_pixel pen, gui_vec* rc, unsigned int r, bool fill);

#ifdef __cplusplus
    }
#endif /* __CPLUSPLUS */

#endif /* __GUI_H__ */
