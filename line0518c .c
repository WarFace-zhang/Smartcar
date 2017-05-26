#include "line.h"
#include "common.h"
#include  "FIRE_camera.h" 
#include  "FIRE_LCD.h" 


int         l_cnt,        r_cnt,        m_cnt;
int         l_stat,       r_stat;
int         l_end,        r_end;
int         l_flag,       r_flag;
int         ll_flag,      rr_flag;
int         l_change,     r_change;
int         l_trans,      r_trans;

Site_t  l[point_array_max], r[point_array_max], m[point_array_max];

uint8 (*image_array)[width];

Site_t q = {0,0};

void image_array_init(uint8 *image)
{
    image_array = (uint8 (*)[width])image;
}

void display_line(uint16 s,uint16 rgb)
{
    switch(s)
    {
        case 0: LCD_points(m,m_cnt,rgb);break;
        case 1: LCD_points(l,l_cnt,rgb);break;
        case 2: LCD_points(r,r_cnt,rgb);break;
        default : break;
    }
}

void FINDMID()
{
    memset(l,0,sizeof(l));
    memset(r,0,sizeof(r));
    memset(m,0,sizeof(m));

    l_cnt = r_cnt = m_cnt = 0;
    l_stat = r_stat = 0;
    l_end = r_end = 0;
    l_flag = r_flag = 0;
    ll_flag = rr_flag = 0;
    l_change = r_change = 0;
    l_trans = r_trans = 0;
  
    int i;
    
    for( i = height - 1 ; ( ( l_flag == 0 ) || ( r_flag == 0) ) && ( i >= 0 ) ; i-- )
    {
      if( l_flag == 0)
      {
        l_line_scan( &l[l_cnt] , i );
        l_cnt++ ;
      }        
      
      if( r_flag ==0 ) 
      {
        r_line_scan( &r[r_cnt] , i );
        r_cnt++ ;
      }
    }
               
        if(l_cnt > value_max && r_cnt < value_min)
          l_blank();
        
        else if(r_cnt > value_max && l_cnt < value_min)
          r_blank();
        
        else if(l_cnt > value_max && r_cnt > value_max )
          lr_blank();
        
        else
          lr_notblank();

    for ( ; m_cnt <= l_cnt && m_cnt < r_cnt ; m_cnt ++ )
    {
        m[m_cnt].x = ( l[m_cnt].x + r[m_cnt].x ) / 2;
        m[m_cnt].y = ( l[m_cnt].y + r[m_cnt].y ) / 2;
    }  
}


//普通行扫描
void l_line_scan( Site_t *result_l, int line)
{
    int tmp_l = 0 ;
    int j = 0;
    
    for( ; j < 2 * width / 3 ; j++ )
    {
        if( image_array[line][j] - image_array[line][j+1] == _black_ - _white_ )
        {
            tmp_l = j + 1;
            l_flag = 1;
        }
    }
    
    result_l->y = line;
    result_l->x = tmp_l;
    
}

void r_line_scan( Site_t *result_r, int line)
{
    int tmp_r = width - 1 ;
    int j = width - 1;
    
    for( ; j > width/3 ; j-- )
    {
        if( image_array[line][j] - image_array[line][j-1] == _black_ - _white_ )
        {
            tmp_r = j - 1;
            r_flag=1;
        }      
    }

    result_r->y = line;
    result_r->x = tmp_r;
    
}

/*
*  返回值
*      1       找到一个点，正常返回
*     -1      找不到点，正常返回
*/
int line_l_scan( Site_t *result_l,  Site_t start, int status)
{
    int last_i = start.y;
    int last_j = start.x;
    
    if( l_trans > 5)
      return -1;

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
            return -1;
        }
        else
        {
            result_l -> y = s_y;
            result_l -> x = fflag;
            
            if( l_change != 0)
            {
              l_change = 0;
              l_trans++;
            }
            
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
            result_l -> y = fflag;
            result_l -> x = s_x;
            
            if( l_change != -1)
            {
              l_change = -1;
              l_trans++;
            }
           
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
            result_l -> y = fflag;
            result_l -> x = s_x;
            
            if( l_change != 1)
            {
              l_change = 1;
              l_trans++;
            }
            
            return 1;
        }
    }
}

int line_r_scan( Site_t *result_r,  Site_t start, int status)
{
    int last_i = start.y;
    int last_j = start.x;
    
    if( r_trans > 5)
      return -1;

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
            return -1;
        }
        else
        {
            result_r -> y = s_y;
            result_r -> x = fflag;
            
            if( r_change != 0)
            {
              r_change = 0;
              r_trans++;
            }
            
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
            result_r -> y = fflag;
            result_r -> x = s_x;
            
            if( r_change != -1)
            {
              r_change = -1;
              r_trans++;
            }
           
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
            result_r -> y = fflag;
            result_r -> x = s_x;
            
            if( r_change != 1)
            {
              r_change = 1;
              r_trans++;
            }
            
            return 1;
        }
    }
}


int l_sc_point()
{
    int i = l_cnt - 1;
    int t1_y, t1_x;
    int result = -1;
    for( ; i > 0 ; i-- )
    {
        t1_y = l[i].y;
        t1_x = l[i].x;
        if( image_array[ t1_y ][ t1_x ] - image_array[ t1_y + 1 ][ t1_x ] == _white_ - _black_ )  continue;
        else
        {
            result = i;
            break;
        } 
    }
    return result;
}


int r_sc_point()
{
    int i = r_cnt - 1;
    int t1_y, t1_x;
    int result = -1;
    for( ; i > 0 ; i-- )
    {
        t1_y = r[i].y;
        t1_x = r[i].x;
        if( image_array[ t1_y ][ t1_x ] - image_array[ t1_y + 1 ][ t1_x ] == _white_ - _black_ )  continue;
        else
        {
            result = i;
            break;
        } 
    }
    return result;
}


int l_extend( Site_t *result, int l_start)
{
    int x_start = l[l_start].x ;
    int y_start = l[l_start].y ;

    int fflag = -1;
    
    for( ; y_start >= 0 ; y_start -- )
     if( image_array[y_start-1][x_start] - image_array[y_start][x_start] == _black_ - _white_ )
          break;

    for( ; x_start < width -1; x_start++ )
    {
       int y0 = y_start - search_area1;
       int y1 = y_start + search_area1;
       y0 = y0 < 0 ? 0 : y0;
       y1 = y1 >= width ? width - 1 : y1;

       fflag = -1;
    for(; y0 < y1 ; y0++)
    {
      if( image_array[y0][x_start] - image_array[y0+1][x_start] == _black_ - _white_ )
      {
        if( image_array[y0][x_start] - image_array[y0][x_start+1] == _black_ - _white_ )
        {    
          fflag = 1 ;
          break;
        }
      }
    }
    
    if(fflag == 1)
    {
        result->y = y_start;
        result->x = x_start;
        return 1;
        
     }
    }
    
    return -1;
}

int r_extend( Site_t *result, int r_start)
{
    int x_start = r[r_start].x ;
    int y_start = r[r_start].y ;

    int fflag = -1;
    
    for( ; y_start >= 0 ; y_start -- )
         if( image_array[y_start][x_start] - image_array[y_start-1][ x_start ] == _white_ - _black_ )
               break;
            

    for(; x_start > 0; x_start--)
    {
       int y0 = y_start - search_area1;
       int y1 = y_start + search_area1;
       y0 = y0 < 0 ? 0 : y0;
       y1 = y1 >= width ? width - 1 : y1;

       fflag = -1;
    for(; y0 < y1 ; y0++)
    {
      if( image_array[y0][x_start] - image_array[y0+1][x_start] == _black_ - _white_ )
      {
        if( image_array[y0][x_start-1] - image_array[y0][x_start] == _white_ - _black_)
        {
            fflag = 1 ;
            break;
        }
      }
    }
    
    if(fflag == 1)
    {
        result->y = y_start;
        result->x = x_start;
        return 1;
        
     }
    }
    
    return -1;
}


int l_fill(int l_cnt_temp)
{
    if(l_cnt_temp <= 0) return -1;
    l_cnt = l_cnt_temp;
    
    Site_t l_point;
    int l_result = l_extend( &l_point , l_cnt );
 
    if( l_result == -1 )    return -1;

    int x_minus = l_point.x - l[l_cnt].x;
    int y_minus = l[l_cnt].y - l_point.y;
    
    int y_start = l[l_cnt].y ;
    int x_start = l[l_cnt].x ;

    float x_dis =(float) x_minus  / y_minus;
    l_cnt ++ ;
    
    int i = 1;
    for( ; i < y_minus ; ++i )
    {
        l[l_cnt].x = x_start + (x_dis * i);
        l[l_cnt].y = y_start - i;
        l_cnt++;
    }

    //为精确，不可移入循环
    l[l_cnt].x = l_point.x;
    l[l_cnt].y = l_point.y;
    l_cnt++;
    
    q.x =l_point.x;
    q.y = l_point.y;
    

    l_stat = 0;

    return 1;
}



int r_fill(int r_cnt_temp)
{
    
    if( r_cnt_temp == -1 )  return -1;
    r_cnt = r_cnt_temp;
    
    Site_t r_point;
    int r_result = r_extend( &r_point , r_cnt );
    if( r_result == -1 )    return -1;

    int x_minus = r_point.x - r[r_cnt].x;
    int y_minus = r[r_cnt].y - r_point.y;
    int x_start = r[r_cnt].x;
    int y_start = r[r_cnt].y;
    
    int x_dis = ( x_minus * 100 ) / y_minus;
    r_cnt ++ ;

    int i = 1;
    for( ; i < y_minus ; ++i )
    {
        r[r_cnt].x = x_start + (x_dis * i) /100;
        r[r_cnt].y = y_start - i;
        r_cnt++;
    }

    //为精确，不可移入循环
    r[r_cnt].x = r_point.x;
    r[r_cnt].y = r_point.y;
    r_cnt++;
    
    r_stat = 0;

    return 1;
}

int back_fill(int line_mark, Site_t f)
{
	int x = f.x;
	int y = f.y;
	
	Site_t a, b;
	float dis;

	if( y >= 5 )
	{
		a.x = x; 	a.y = y - 2;
		b.x = x;	b.y = y - 4; 
		
		if( line_mark == _LINE_LEFT_ )
		{
			line_l_scan(&a, a, 0);
			line_l_scan(&b, b, 0);
		}
		else
		{
			line_r_scan(&a, a, 0);
			line_r_scan(&b, b, 0);
		}
		dis = (float)(b.x - a.x) / 2 ;
	}
	else
	{
		a.x = x;	a.y = y;
		dis = 0;
	}
	if(line_mark == _LINE_LEFT_)
	{
		l_cnt = height - 1 - a.y;
		int cnt = 0;
		for(int i = l_cnt; i >= 0; --i)
		{
			l[i].y = height - 1 - i;
			l[i].x = a.x - (cnt++) * dis;
			l[i].x = l[i].x < 0 ? 0 : l[i].x;
		}

	}
	else if(line_mark == _LINE_RIGHT_)
	{
		r_cnt = height - 1 - a.y;
		int cnt = 0;
		for(int i = r_cnt; i >= 0; --i)
		{
			r[i].y = height - 1 - i;
			r[i].x = a.x - (cnt++) * dis;
			r[i].x = r[i].x >= width ? width - 1 : r[i].x;
		}

	}


}


int is_blank()
{
    int rec =  l[l_cnt-1].y > r[r_cnt-1].y ? l[l_cnt-1].y : r[r_cnt-1].y;
    int maxy = rec;

    for( ; maxy >= 0 ; maxy--)
    {
        int x;
        for(x = 0; x < width; ++x)
        {
            if( image_array[maxy][x] == _black_ )    break;
        }
        if( width == x) return 1;
    }
    return 0;
}

void l_blank()
{
            int tmpe_r ;
            int p = 0;         
            int l_result , r_result ;
            Site_t r_site ;
            
            l_result = l_extend(&l[l_cnt] , l_cnt -1) ;
            
            if( l_result )
            {
               back_fill( _LINE_LEFT_ , l[l_cnt]);
               r[r_cnt -1].x = ( 2 * width ) / 3 ;
               r_result = r_extend( &r_site , r_cnt -1) ;
               back_fill( _LINE_RIGHT_ , r_site);
            }
            
            else
            {
            tmpe_r = line_r_scan( &r[r_cnt], r[r_cnt-1], 0 ); 
            
            while(tmpe_r == 1)
            {
                 tmpe_r = line_r_scan( &r[r_cnt], r[r_cnt-1], 0 ); 
                 r_cnt++;
                 if(r[r_cnt].x > r[r_cnt-1].x )
                   p++;
                 if(p > 3)
                   break;
                 
                 if(r_cnt >= point_max)  break;
            }  
            }
}

void r_blank()
{
            int tmpe_l ;
            int p = 0;
            int l_result , r_result ;
            Site_t l_site ;
            
            r_result = r_extend(&r[r_cnt] , r_cnt -1) ;
            
            if( r_result )
            {
               back_fill( _LINE_RIGHT_ , r[r_cnt]);
               l[l_cnt -1].x = ( 1 * width ) / 3 ;
               l_result = l_extend( &l_site , l_cnt -1) ;
               back_fill( _LINE_LEFT_ , l_site);
            }
            
            else
            {
            tmpe_l = line_l_scan( &l[l_cnt], l[l_cnt-1], 0 ); 
            
            while(tmpe_l == 1)
            {
                 tmpe_l = line_l_scan( &l[l_cnt], l[l_cnt-1], 0 ); 
                 l_cnt++;
                 if(l[l_cnt].x > l[l_cnt-1].x )
                   p++;
                 if(p > 3)
                   break;
                 
                 if(l_cnt >= point_max)  break;
            }   
            }            
}

void lr_blank()
{
      int l_result, r_result ;
      int tmp_l = 0 , tmp_r = 0 ;
      
      l_result = l_extend(&l[l_cnt] , l_cnt -1) ;
      r_result = r_extend(&r[r_cnt] , r_cnt -1) ;
      
      if(l_result)
        back_fill( _LINE_LEFT_ , l[l_cnt]);
      else
      {
         while(!l_end)
        {
            tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat ); 
            //line_left
            if( -1 == tmp_l )
            {
                l_stat = 1;
                tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat );
                if( -1 == tmp_l )
                {
                    l_stat = -l_stat;
                    tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat );
                    if( -1 == tmp_l)    {  l_end = 1;/*error，没找到点，直接到下一帧*/}
                }
            }
            if(tmp_l == 0) l_end = 1;
            l_cnt += (tmp_l == 1);
            
            if(l_cnt >= point_max)  l_end = 1;
        }
      }
      
      if(r_result)
         back_fill( _LINE_RIGHT_ , r[r_cnt]);
      else
      {
        while(!r_end)
        {
            tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat ); 
            //line_right
            if( -1 == tmp_r )
            {
                r_stat = 1;
                tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat );
                if( -1 == tmp_r )
                {
                    r_stat = -r_stat;
                    tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat );
                    if( -1 == tmp_r )    {   r_end = 1;/*error，没找到点，直接到下一帧*/}
                }
            }
            if(tmp_r == 0) r_end = 1;
            r_cnt += (tmp_r == 1); 
            
            if(r_cnt >= point_max)  r_end = 1;
        }
      }
}

void lr_notblank()
{ 
  if( (r_cnt-1) && (l_cnt-1) )
     lr_blank();
  
  
  else
  {
     for( ; ; )
     {
        int tmp_l = 0, tmp_r = 0;

        if(!l_end)
        {
            tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat ); 
            //line_left
            if( -1 == tmp_l )
            {
                l_stat = 1;
                tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat );
                if( -1 == tmp_l )
                {
                    l_stat = -l_stat;
                    tmp_l = line_l_scan( &l[l_cnt], l[l_cnt-1], l_stat );
                    if( -1 == tmp_l)    {  l_end = 1;/*error，没找到点，直接到下一帧*/}
                }
            }
            if(tmp_l == 0) l_end = 1;
            l_cnt += (tmp_l == 1);
        }
    
        if(!r_end)
        {
            tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat ); 
            //line_right
            if( -1 == tmp_r )
            {
                r_stat = 1;
                tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat );
                if( -1 == tmp_r )
                {
                    r_stat = -r_stat;
                    tmp_r = line_r_scan( &r[r_cnt], r[r_cnt-1], r_stat );
                    if( -1 == tmp_r )    {   r_end = 1;/*error，没找到点，直接到下一帧*/}
                }
            }
            if(tmp_r == 0) r_end = 1;
            r_cnt += (tmp_r == 1);        
        }

        //竖直扫描 ---- 弯道 or 十字
        if((l_stat != 0 || r_stat != 0) && is_blank())
        {
            if( l_stat < 0 && r_stat > 0 )
            {
               tmp_l = l_fill(l_sc_point());
               tmp_r = r_fill(r_sc_point());
               if( tmp_l == -1 )   l_end = 1;
               if( tmp_r == -1 )   r_end = 1;
            }
        }

        //水平扫描 ---- 直道 or 环形
    	/*else
        {
            //圆环
            if( (l[l_cnt-1].x <= l[l_cnt-4].x - 2) && (r[r_cnt-1].x > r[r_cnt-4].x+2) )
            {
                //默认补右线
                tmp_l = r_fill(r_cnt - 5);
                if( -1 == tmp_r )   r_end = 1;
            }
        }*/

        if(l_cnt >= point_max)  l_end = 1;
        if(r_cnt >= point_max)  r_end = 1;
        if( l_end && r_end )    break;
     }
  }
}