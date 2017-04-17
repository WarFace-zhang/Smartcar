#ifdef __line__
#define __line__

typedef char uint8;
typedef short uint16;
typedef int uint32;
#define width 80
#define height 60
struct Site_t
{
    uint16 x;
    uint16 y;
};
uint8 **image_array;


/*******************************************************************/


#define _white_ 255
#define _black_ 0

#define point_max 80
#define point_cnt 60

#define search_area 8

struct Site_t  l[point_max], r[point_max], m[point_max];
uint16         l_cnt,        r_cnt,        m_cnt;
uint16         l_stat,       r_stat;


void line()
{
    l_cnt = r_cnt = m_cnt = 0;
    l_stat = r_stat = 0;

    uint16 i, j;
    for( i = 0; i < 5; i ++ )
    {
        uint16 tmp_l = 0, tmp_r = width - 1;
        for( j = 1; j; j++ )
        {
            if( image_array[ height-1-i ][ j ] - image_array[ height-1-i ][ j-1 ] == _white_ - _black_ )
            {
                tmp_l = j;
            }

            if( image_array[ height-1-i ][ width-j ] - image_array[ height-1-i ][ width-j-1] == _black_ - _white_ )
            {
                tmp_r = width-j-1;
            }
        }

        l[ l_cnt ].y = height-1-i;
        l[ l_cnt ].x = tmp_l;
        l_cnt ++;
        r[ r_cnt ].y = height-1-i;
        r[ r_cnt ].x = tmp_r;
        r_cnt ++;
        m[ m_cnt ].x = ( l[ m_cnt ].x + r[ m_cnt ].x ) / 2;
        m[ m_cnt ].y = ( l[ m_cnt ].y + r[ m_cnt ].y ) / 2;
        m_cnt ++;



    }

    for( ; ; )
    {
        /*
        *  返回值
        *      1       找到一个点，正常返回
        *      0       达到边界，正常返回
        *      -1      找不到点，正常返回
        *      -0xf    异常返回
        */
        uint16 tmp_l = 0, tmp_r = 0;
        tmp_l = line_l();
        tmp_r = line_r();

        if( -1 == tmp_l )
        {
            l_stat = l_cnt - 1;
            uint16 tmp_3 = line_l();
            if( 1 == tmp_3 )    tmp_l = 1;
            if( -1 == tmp_3 )
            {
                l_stat = -l_stat;
                tmp_3 = line_l();
                if( -1 == tmp_3)    {/*error*/}
            }
        }

        
        if( -1 == tmp_r )
        {
            r_stat = r_cnt - 1;
            uint16 tmp_4 = line_r();
            if( 1 == tmp_4 )    tmp_r = 1;
            if( -1 == tmp_4 )
            {
                r_stat = -r_stat;
                tmp_4 = line_r();
                if( -1 == tmp_4)    {/*error*/}
            }
        }

    }

}




uint16 line_l()
{
    uint16 last_i = l[ l_cnt-1 ].y;
    uint16 last_j = l[ l_cnt-1 ].x;

    if( 0 == l_stat )
    {
        uint16 s_y, s_x0, s_x1;
        
        s_y = last_i - 1;
        if(s_y < 0) return 0;
        
        s_x0 = last_j - search_area;
        s_x1 = last_j + search_area;
        s_x0 = s_x0 < 0 ? 0 : s_x0;
        s_x1 = s_x1 > width - 1 ? width - 1 : s_x1;

        uint16 fflag = -1;

        for( ; s_x0 < s_x1; s_x0 ++ )
        {
            if( image_array[ s_y ][ s_x0 ] - image_array[ s_y ][ s_x0+1 ] == _black_ - _white_ )
            {
                uint16 tmp_1 = fflag - last_j;
                uint16 tmp_2 = s_x0 + 1 - last_j;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;
                
                if( ~fflag && tmp_1 < tmp_2 )
                {

                }
                else
                {
                    fflag = s_x0 + 1;
                }
            }
        }

        if( -1 == fflag )
        {
            //todo
            return -1;
        }
        else
        {
            l[ l_cnt ].y = s_y;
            l[ l_cnt ].x = fflag;
            l_cnt ++;
            return 1;
        }


    }
    else if( l_stat < 0 )
    {
        uint16 s_x = last_j - 1;
        if( s_x < 0 )   return 0;

        uint16 s_y0 = last_i - search_area;
        uint16 s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        uint16 fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _white_ - _black_ )
            {
                uint16 tmp_1 = fflag - last_i; 
                uint16 tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ~fflag && tmp_1 < tmp_2 )
                {
                }
                else
                {
                    fflag = s_y0;
                }
            }
        }

        if( -1 == fflag )
        {
            return -1;
        }
        else
        {
            l[ l_cnt ].y = fflag;
            l[ l_cnt ].x = s_x;
            l_cnt ++;
            return 1;
        }

        
    }
    else if( l_stat > 0 )
    {
        uint16 s_x = last_j + 1;
        if( s_x > width - 1 )   return 0;

        uint16 s_y0 = last_i - search_area;
        uint16 s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        uint16 fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _black_ - _white_ )
            {
                uint16 tmp_1 = fflag - last_i; 
                uint16 tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ~fflag && tmp_1 < tmp_2 )
                {
                }
                else
                {
                    fflag = s_y0 + 1;
                }
            }
        }

        if( -1 == fflag )
        {
            return -1;
        }
        else
        {
            l[ l_cnt ].y = fflag;
            l[ l_cnt ].x = s_x;
            l_cnt ++;
            return 1;
        }



    }
    else
    {
        return -0xf;
    }

}

uint16 line_r()
{
    uint16 last_i = r[ r_cnt-1 ].y;
    uint16 last_j = r[ r_cnt-1 ].x;

    if( 0 == r_stat )
    {
        uint16 s_y, s_x0, s_x1;
        
        s_y = last_i - 1;
        if(s_y < 0) return 0;
        
        s_x0 = last_j - search_area;
        s_x1 = last_j + search_area;
        s_x0 = s_x0 < 0 ? 0 : s_x0;
        s_x1 = s_x1 > width - 1 ? width - 1 : s_x1;

        uint16 fflag = -1;

        for( ; s_x0 < s_x1; s_x0 ++ )
        {
            if( image_array[ s_y ][ s_x0 ] - image_array[ s_y ][ s_x0+1 ] == _white_ - _black_ )
            {
                uint16 tmp_1 = fflag - last_j;
                uint16 tmp_2 = s_x0 + 1 - last_j;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;
                
                if( ~fflag && tmp_1 < tmp_2 )
                {

                }
                else
                {
                    fflag = s_x0;
                }
            }
        }

        if( -1 == fflag )
        {
            //todo
            return -1;
        }
        else
        {
            r[ r_cnt ].y = s_y;
            r[ r_cnt ].x = fflag;
            r_cnt ++;
            return 1;
        }


    }
    else if( r_stat < 0 )
    {
        uint16 s_x = last_j - 1;
        if( s_x < 0 )   return 0;

        uint16 s_y0 = last_i - search_area;
        uint16 s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        uint16 fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _black_ - _white_ )
            {
                uint16 tmp_1 = fflag - last_i; 
                uint16 tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ~fflag && tmp_1 < tmp_2 )
                {
                }
                else
                {
                    fflag = s_y0 + 1;
                }
            }
        }

        if( -1 == fflag )
        {
            return -1;
        }
        else
        {
            r[ r_cnt ].y = fflag;
            r[ r_cnt ].x = s_x;
            r_cnt ++;
            return 1;
        }

        
    }
    else if( r_stat > 0 )
    {
        uint16 s_x = last_j + 1;
        if( s_x > width - 1 )   return 0;

        uint16 s_y0 = last_i - search_area;
        uint16 s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        uint16 fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _white_ - _black_ )
            {
                uint16 tmp_1 = fflag - last_i; 
                uint16 tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ~fflag && tmp_1 < tmp_2 )
                {
                }
                else
                {
                    fflag = s_y0;
                }
            }
        }

        if( -1 == fflag )
        {
            return -1;
        }
        else
        {
            r[ r_cnt ].y = fflag;
            r[ r_cnt ].x = s_x;
            r_cnt ++;
            return 1;
        }



    }
    else
    {
        return -0xf;
    }

}


#endif