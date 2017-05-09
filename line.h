#ifndef __line__
#define __line__

typedef unsigned char uint8;
typedef short uint16;
typedef int uint32;
#define width 80
#define height 60
typedef  struct
{
   	uint16 x;
    uint16 y;
}Site_t;
uint8 (*image_array)[width];



/*******************************************************************/


#define _white_ 255
#define _black_ 0

#define point_max 60
#define point_array_max 80

#define search_area 8

#define dr_angle_cut 3 

Site_t  l[point_array_max], r[point_array_max], m[point_array_max];
int         l_cnt,        r_cnt,        m_cnt;
int         l_stat,       r_stat;
int         l_end,        r_end;

void image_array_init(uint8 *image);
void line_scan( Site_t *result_l, Site_t *result_r, int line);
int line_l_scan( Site_t *result_l,  Site_t start, int status);
int line_r_scan( Site_t *result_r,  Site_t start, int status);
int l_sc_point();
int r_sc_point();
int l_extend( Site_t *result, int l_start);
int r_extend( Site_t *result, int r_start);
int l_fill();
int r_fill();

#include "line.c"
#endif