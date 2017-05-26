#define width CAMERA_W
#define height CAMERA_H

#define _white_ 255
#define _black_ 0

#define P_MAX 60
#define POINT_ARRAY_MAX 80

#define LINE_LEFT   -1
#define LINE_RIGHT  1
#define LINE_MID    0


#define search_area 8
#define blank_area 4
#define filt_area 8
#define blank_col 75
#define blank_split 20

Site_t      l[POINT_ARRAY_MAX],     r[POINT_ARRAY_MAX],     m[POINT_ARRAY_MAX];
int         l_rec[POINT_ARRAY_MAX], r_rec[POINT_ARRAY_MAX];

int         l_cnt,          r_cnt,          m_cnt;
int         l_scnt,         r_scnt;
int         l_blank,        r_blank;
int         l_stat,         r_stat;
int         l_end,          r_end;
int         mode;


/******************************************************************/

void LineInit()
{
    memset(l, 0, sizeof(l));
    memset(r, 0, sizeof(r));
    memset(m, 0, sizeof(m));

    memset(l_rec, 0, sizeof(l_rec));
    memset(r_rec, 0, sizeof(r_rec));
    
    l_cnt = r_cnt = m_cnt = 0;
    l_scnt = r_scnt = 0;
    l_blank = r_blank = 0;
    l_stat = r_stat = 0;
    l_end = r_end = 0;
    mode = 0;
}

int LineLeftTrack( Site_t *result_l,  Site_t start, int status)
{
    int last_i = start.y;
    int last_j = start.x;

    if( 0 == status )
    {
        int s_y, s_x0, s_x1;
        
        s_y = last_i - 1;
        if(s_y <= 0) return 0;
        
        s_x0 = last_j - search_area;
        s_x1 = last_j + search_area;
        s_x0 = s_x0 < 0 ? 0 : s_x0;
        s_x1 = s_x1 > width - 1 ? width - 1 : s_x1;

        int fflag = -1;

        for( ; s_x0 < s_x1; s_x0 ++ )
        {
            if( image_array[ s_y ][ s_x0 ] - image_array[ s_y ][ s_x0+1 ] == _black_ - _white_ )
            {
                int tmp_1 = fflag - last_j;
                int tmp_2 = s_x0 + 1 - last_j;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;
                
                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_x0 + 1;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_l -> y = s_y;
            result_l -> x = fflag;
            return 1;
        }
    }



    else if( status < 0 )
    {
        int s_x = last_j - 1;
        if( s_x <= 0 )   return 0;

        int s_y0 = last_i - search_area;
        int s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        int fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _white_ - _black_ )
            {
                int tmp_1 = fflag - last_i; 
                int tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_y0;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_l -> y = fflag;
            result_l -> x = s_x;        
            return 1;
        }      
    }



    else if( status > 0 )
    {
        int s_x = last_j + 1;
        if( s_x >= width - 1 )   return 0;

        int s_y0 = last_i - search_area;
        int s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        int fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _black_ - _white_ )
            {
                int tmp_1 = fflag - last_i; 
                int tmp_2 = s_y0 + 1 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_y0 + 1;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_l -> y = fflag;
            result_l -> x = s_x;
            return 1;
        }
    }
}

int LineRightTrack( Site_t *result_r,  Site_t start, int status)
{
    int last_i = start.y;
    int last_j = start.x;
 
    if( 0 == status )
    {
        int s_y, s_x0, s_x1;
        
        s_y = last_i - 1;
        if(s_y <= 0) return 0;
        
        s_x0 = last_j - search_area;
        s_x1 = last_j + search_area;
        s_x0 = s_x0 < 0 ? 0 : s_x0;
        s_x1 = s_x1 > width - 1 ? width - 1 : s_x1;

        int fflag = -1;

        for( ; s_x0 < s_x1; s_x0 ++ )
        {
            if( image_array[ s_y ][ s_x0 ] - image_array[ s_y ][ s_x0+1 ] == _white_ - _black_ )
            {
                int tmp_1 = fflag - last_j;
                int tmp_2 = s_x0 - last_j;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;
                
                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_x0;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_r -> y = s_y;
            result_r -> x = fflag;            
            return 1;
        }
    }



    else if( status < 0 )
    {
        int s_x = last_j - 1;
        if( s_x <= 0 )   return 0;

        int s_y0 = last_i - search_area;
        int s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        int fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _black_ - _white_ )
            {
                int tmp_1 = fflag - last_i; 
                int tmp_2 = s_y0 + 1 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_y0 + 1;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_r -> y = fflag;
            result_r -> x = s_x;
            return 1;
        }        
    }



    else if( status > 0 )
    {
        int s_x = last_j + 1;
        if( s_x >= width - 1 )   return 0;

        int s_y0 = last_i - search_area;
        int s_y1 = last_i + search_area;
        s_y0 = s_y0 < 0 ? 0 : s_y0;
        s_y1 = s_y1 > height - 1 ? height - 1 : s_y1;

        int fflag = -1;

        for( ; s_y0 < s_y1 ; s_y0++)
        {
            
            if( image_array[ s_y0 ][ s_x ] - image_array[ s_y0+1 ][ s_x ] == _white_ - _black_ )
            {
                int tmp_1 = fflag - last_i; 
                int tmp_2 = s_y0 - last_i;
                tmp_1 = tmp_1 < 0 ? -tmp_1 : tmp_1;
                tmp_2 = tmp_2 < 0 ? -tmp_2 : tmp_2;

                if( ! ( ~fflag && tmp_1 < tmp_2 ) )
                    fflag = s_y0;
            }
        }

        if( -1 == fflag )   return -1;
        else
        {
            result_r -> y = fflag;
            result_r -> x = s_x;
            return 1;
        }
    }
}

int LineCommonTrack(int line_mark, Site_t *result, Site_t start, int status)
{
    if( LINE_LEFT == line_mark )
        return LineLeftTrack(result, start, status);
    else if( LINE_RIGHT == line_mark)
        return LineRightTrack(result, start, status);
    else
        return -1;
}


//普通行扫描
int l_line_scan( Site_t *result_l, int line)
{
    int tmp_l = 0 ;
    int j = 0;
    
    int flag = -1;
    for( ; j < 2 * width / 3 ; j++ )
    {
        if( image_array[line][j] - image_array[line][j+1] == _black_ - _white_ )
        {
            tmp_l = j + 1;
            flag = 1;
        }
    }
    result_l->y = line;
    result_l->x = tmp_l;
    
    if(flag == 1)   return 1;
    return -1;
}

int r_line_scan( Site_t *result_r, int line)
{
    int tmp_r = width - 1 ;
    int j = width - 1;
    
    int flag = -1;
    for( ; j > width/3 ; j-- )
    {
        if( image_array[line][j] - image_array[line][j-1] == _black_ - _white_ )
        {
            tmp_r = j - 1;
            flag=1;
        }      
    }

    result_r->y = line;
    result_r->x = tmp_r;
    
    if(flag == 1)   return 1;
    return -1;
}

int LineCommonScan(int line_mark, Site_t *result, int line)
{
    if( LINE_LEFT == line_mark )
        return l_line_scan(result, line);
    else if (LINE_RIGHT == line_mark)
        return r_line_scan(result, line);
    return -1;
}

int LineBlankScan(int line_mark, Site_t *result, Site_t start)
{
    if(LINE_LEFT == line_mark)
    {
        if(start.x > blank_area)    return -1;
        if(start.y <= 0)            return -1;

        int y = start.y - 1;
        int x = start.x;

        int blank_cnt = 0, flag = -1;
        for(int i = 0; i < search_area; ++i)
        {
            if( image_array[y][i] == _white_ )    
                if( -1 == flag)
                {
                    blank_cnt++;
                }
            else    flag = 1;
        }

        if(blank_cnt >= search_area)
        {
            result->x = 0;
            result->y = y;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if(LINE_RIGHT == line_mark)
    {
        if(start.x < width - 1 - blank_area)    return -1;
        if(start.y <= 0)                        return -1;

        int y = start.y - 1;
        int x = start.x;

        int blank_cnt = 0, flag = -1;
        for(int i = width - 1; i > width - 1 - search_area; --i)
        {
            if( image_array[y][i] == _white_ )    
                if( -1 == flag)
                {
                    blank_cnt++;
                }
            else    flag = 1;
        }

        if(blank_cnt >= search_area)
        {
            result->x = width - 1;
            result->y = y;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

void TrackLeft()
{
    int result, transcnt, trycnt;
    
    result = 1;
    transcnt = 0;
    trycnt = 0;

    while(1)
    {
        if(transcnt >= 5)
        {
            l_end = 1;
            break;
        }

        result = LineCommonTrack(LINE_LEFT, &l[l_cnt], l[l_cnt-1], l_stat ); 
        
        if( 1 != result )
        {
            if(trycnt > 2)  
            {
                l_end = 1;
                break;
            }

            switch (l_stat)
            {
                case 0:
                    l_stat = 1;
                    bre/ak;
                case 1:
                    l_stat = -1;
                    break;
                case -1:
                    l_stat = 0;
                    break;
            }
            trycnt ++;        
        }

        else 
        {
            if(l_cnt == l_blank)
            {
                l_rec[l_scnt++] = l_stat;
            }
            else if(trycnt > 0)
            {
                l_rec[l_scnt++] = l_stat;
                transcnt ++;
            }
            trycnt = 0;
            l_cnt ++;
        }
        
  
    }
}

void TrackRight()
{
    int result, transcnt, trycnt;

    result = 1;
    transcnt = 0;
    trycnt = 0;

    while(1)
    {
        if(transcnt >= 5)
        {
            r_end = 1;
            break;
        }

        result = LineCommonTrack(LINE_RIGHT, &r[r_cnt], r[r_cnt-1], r_stat ); 

        if( 1 != result )
        {
            if(trycnt > 2)  
            {
                r_end = 1;
                break;
            }

            switch (r_stat)
            {
                case 0:
                    r_stat = -1;
                    break;
                case -1:
                    r_stat = 1;
                    break;
                case 1:
                    r_stat = 0;
                    break;
            }

            trycnt ++;        
        }

        else 
        {
            if(trycnt > 0)
            {
                r_rec[r_scnt++] = r_stat;
                transcnt ++;
            }
            trycnt = 0;
            r_cnt ++;
        }
        
  
    }
}


void FirstTrack()
{
    TrackLeft();
    TrackRight();
}

void BlankLeft()
{
    while(1)
    {
        if( 1 == LineBlankScan(LINE_LEFT, &l[l_cnt], l[l_cnt-1]) )
        {
            l_blank ++;
            l_cnt ++;
        }
        else
        {
            break;
        }
    }
}

void BlankRight()
{
    while(1)
    {
        if( 1 == LineBlankScan(LINE_RIGHT, &r[r_cnt], r[r_cnt-1]) )
        {
            r_blank ++;
            r_cnt ++;
        }
        else
        {
            break;
        }
    }
}

void BlankScan()
{
    BlankLeft();
    BlankRight();   
}

void LineFilt()
{
    if(l_cnt < filt_area)
    {
        for(int i = 0; i < l_cnt; i++)
        {
            l[i].x = 0;
        }
        BlankLeft();
        TrackLeft();
    }

    if(r_cnt < filt_area)
    {
        for(int i = 0; i < r_cnt; i++)
        {
            r[i].x = width - 1;
        }
        BlankRight();
        TrackRight();
    }
}


int BlankMode()
{
    int rec =  l[l_cnt-1].y > r[r_cnt-1].y ? l[l_cnt-1].y : r[r_cnt-1].y;

    int status = 0;
    for( ; rec >= 0 ; rec --)
    {
        int x;
        int split = 0;
        int allbl = 0;
        int stack = 0;
        for(x = 0; x < width; ++x)
        {
            if( image_array[rec][x] == _black_ )
            {
                if( stack >= blank_col )    allbl ++;
                else if( stack >= blank_split )   split ++;
                stack = 0;
            }
            else stack ++;
        }
        if( stack >= blank_col )    allbl ++;
        else if( stack >= blank_split ) split ++;
        
        if( 0 == status )
        {
            status += ( allbl == 1 );
        }
        else if( 1 == status )
        {
            status += ( split == 2 );
        }
        else if( 2 == status )
        {
            status += ( allbl == 1 );
        }
    }
    return status;
}



int Modes(int l_stat_end, int r_stat_end)
{
    //两边均为横扫，均假设直道处理
    // if( l_stat_end == 0 && r_stat_end == 0 )
    // {
    //     return 0;
    // }

    //需要调整 search_area 十字圆环
    // if( l_stat_end == -1 && r_stat_end == 1 )
    
        int bm = BlankMode();
        
        //十字
        if( 1 == bm )   return 1;
        
        //圆环
        if( 2 <= bm )   return 3;

    
}


int LineSolve()
{
    LineInit();

    LineCommonScan(LINE_LEFT, &l[0], height-1); l_cnt++;
    LineCommonScan(LINE_RIGHT, &r[0], height-1); r_cnt++;

    BlankScan();
    FirstTrack();
    
    LineFilt();

    mode = Modes();



}