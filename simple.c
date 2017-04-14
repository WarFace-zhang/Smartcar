#ifndef camera
#define camera

typedef char uint8;
typedef short uint16;
typedef int uint32;

#define width 60
#define height 80

#define WHITE 255
#define BLACK 0

#define limit_x 7
#define limit_y 7

#define crossroad_search_height 10
#define crossroad_search_width 4
//弯道复现参数
#define bend_discover 4
#define cross_discover 10

#define point_max 80
#define point_count 60

// uint8 line_left[point_count][2];
struct Site_t
{
    uint16 x;
    uint16 y;
};

int get_line_left(uint8 direc);
int get_line_right(uint8 direc);

struct Site_t line_left[point_max],  line_right[point_max],  line_mid[point_max];

uint8 line_cnt_left,  line_cnt_right,  line_cnt_mid;



// 0-黑色 255-白色
//全局图像参数
uint8 **image_array;


void simplemid()
{

    //行和列
    uint16 i, j;
    //临时保存左右边界
    uint16 tmp_left, tmp_right;
    //扫描到当前行数计数
    line_cnt_left = line_cnt_right = line_cnt_right = 0;

    //前三行扫描，扫描失败保存左右边界
    for(  i = height - 1  ;  i > height - 4  ;  i --  )
    {
        tmp_left = 0, tmp_right = width - 1;
        for(j = 1; j < (width - 1 - j); j++ )
        {
            if(image_array[i][j] > image_array[i][j - 1])
            {
                tmp_left = j;
            }
            if(image_array[i][width - 1 - j] > image_array[i][width - j])
            {
                tmp_right = width - 1 - j;
            }
        }
        line_left[line_cnt_left].y = i; line_left[line_cnt_left].x = tmp_left;
        line_cnt_left++;

        line_right[line_cnt_right].y = i; line_right[line_cnt_right].x = tmp_right;
        line_cnt_right++;

        line_mid[line_cnt_mid].y = ( line_left[line_cnt_mid].y + line_right[line_cnt_mid].y ) / 2; 
        line_mid[line_cnt_mid].x = ( line_left[line_cnt_mid].x + line_right[line_cnt_mid].x ) / 2;
        line_cnt_mid++;
    }

    //扫描结束标记
    uint8 line_left_flag = 1;
    uint8 line_right_flag = 1;

    //扫描方向
    uint8 line_left_direc = -1;
    uint8 line_right_direc = -1;

    while( line_left_flag || line_right_flag )
    {
        // 返回值
            // -2      判为弯道
            // -1      判为十字，整行空白
            // 0       图像边界
            // 1       找到一个数据（一个点
            // 2       找到多个数据（十字
        if(line_left_flag)
        {
            uint8 tmp_l = get_line_left(line_left_direc);
            if(tmp_l == -2) line_left_direc = 1;
            if(tmp_l == -1 || tmp_l == 0 || line_cnt_left >= point_count)   line_left_flag = 0;
        }

        if(line_right_flag)
        {
            uint8 tmp_r = get_line_right(line_right_direc);
            if(tmp_r == -2) line_right_direc = 1;
            if(tmp_r == -1 || tmp_r == 0 || line_cnt_right >= point_count)  line_right_flag = 0;
        }

        for( ; line_cnt_mid < line_cnt_left && line_cnt_mid < line_cnt_right ; line_cnt_mid++ )
        {
            line_mid[line_cnt_mid].y = ( line_left[line_cnt_mid].y + line_right[line_cnt_mid].y ) / 2;
            line_mid[line_cnt_mid].x = ( line_left[line_cnt_mid].x + line_right[line_cnt_mid].x ) / 2;
        }

    }    
}


// direc 扫描方向

int get_line_left(uint8 direc)
{
    // 记录白色点数
    uint8 line_left_black = 0;
    // 记录找到边界点衡坐标
    uint16 line_left_pos = -1;

    // 限制范围查找
    uint8 line_left_limit_x0  =  (line_left[line_cnt_left-1].x - limit_x) > 0    ?    line_left[line_cnt_left-1].x    :    1    ;
    uint8 line_left_limit_x1  =  (line_left[line_cnt_left-1].x + limit_x) <= width - 1    ?    line_left[line_cnt_left-1].x    :    width - 1    ;

    uint8 line_left_tmp_i = line_left[line_cnt_left-1].y + direc > 0 ? line_left[line_cnt_left-1].y + direc : 0;
    line_left_tmp_i = line_left_tmp_i < height ? line_left_tmp_i : height - 1;

    for(    ;  line_left_limit_x0 <= line_left_limit_x1  ;  line_left_limit_x0++  )
    {
            if( 0  ==  image_array[line_left_tmp_i][line_left_limit_x0] )     line_left_black ++;

            if(  (direc * -255)  ==  ( image_array[ line_left_tmp_i ][ line_left_limit_x0 ] - image_array[ line_left_tmp_i ][ line_left_limit_x0 - 1 ] )   )
            {
                line_left_pos = line_left_limit_x0;
            }
    }


    //当前行没有搜索到黑点
    if(  direc == -1  &&  ( line_left_black == 0 || line_left_pos == -1 )  )
    {
        uint16 tmp_rec = -1;
        uint16 tmp_x;
        uint16 tmp_y;

        //扫描结束,遇边界，无发现 return 0
        if( line_left[line_cnt_left-1].y - 2 <= 0 )      return 0;
        else tmp_y = line_left[line_cnt_left-1].y - 2;

        for(  ;  tmp_y > 0  &&  tmp_y > line_left[line_cnt_left-1].y - crossroad_search_height  ;  tmp_y --  )
        {
            tmp_x = line_left[line_cnt_left-1].x - crossroad_search_width > 0 ? line_left[line_cnt_left-1].x - crossroad_search_width :  1 ;

            //弯道复现
            uint16 tmp_black = 0;

            for(  tmp_x = -crossroad_search_width  ;  tmp_x <= width - 1 && tmp_x <= crossroad_search_width  ;  tmp_x ++  )
            {   
                if( image_array[tmp_y][tmp_x] == 0 )    tmp_black ++ ;
                //十字复现
                if(  image_array[tmp_y][tmp_x] - image_array[tmp_y][tmp_x - 1] == 255  )
                {
                    tmp_rec = tmp_x;
                }
            }

            if( -1 != tmp_rec)  break;
            else if( tmp_black > bend_discover )    tmp_rec = -2;
        }

        // tmp > 0 记录到复现位置, 十字复现记录数据返回 return 1
        if( tmp_rec > 0 )
        {
            uint16 tmp_yy = line_left[line_cnt_left-1].y;
            uint16 tmp_xx = line_left[line_cnt_left-1].x;
            for(  ;  tmp_yy > tmp_y  ;  tmp_yy -- )
            {
                line_left[line_cnt_left].y = tmp_yy;
                line_left[line_cnt_left].x = tmp_x;
                line_cnt_left ++;
            }
            return 2;
        }

        // tmp == -1 未查找到点,当前为十字路口，返回 return -1
        if( -1 == tmp_rec)
        {
            line_left_black = 0;
            tmp_y = line_left[line_cnt_left-1].y - 2;
            
            for( tmp_x = 0 ; tmp_x < width ; tmp_x ++)
            {
                if( 0 == image_array[tmp_y][tmp_x] )    line_left_black ++;
            }

            if( line_left_black < cross_discover )  
            {
                   
                return -1;
            }
        }

        // return -2 当前为弯道
        return -2;

    }


    //找到一个点 返回 return 1
    else
    {
        line_left[line_cnt_left].y = line_left[line_cnt_left-1].y + direc;
        line_left[line_cnt_left].x = line_left_pos;
        line_cnt_left ++;
        return 1;
    }


}



// direc 扫描方向

int get_line_right(uint8 direc)
{
    // 记录白色点数
    uint8 line_right_black = 0;
    // 记录找到边界点衡坐标
    uint16 line_right_pos = -1;

    // 限制范围查找
    uint8 line_right_limit_x0  =  (line_right[line_cnt_right-1].x - limit_x) > 0  ?  line_right[line_cnt_right-1].x  :  1  ;
    uint8 line_right_limit_x1  =  (line_right[line_cnt_right-1].x + limit_x) <= width - 1  ?  line_right[line_cnt_right-1].x  :  width - 1  ;

    uint8 line_right_tmp_i = line_right[line_cnt_right-1].y + direc > 0 ? line_right[line_cnt_right-1].y + direc : 0;
    line_right_tmp_i = line_right_tmp_i < height ? line_right_tmp_i : height - 1;

    for(    ;  line_right_limit_x0 <= line_right_limit_x1  ;  line_right_limit_x0++  )
    {
            if( 0  ==  image_array[line_right_tmp_i][line_right_limit_x0] )     line_right_black ++;

            if(  (direc * 255)  ==  ( image_array[ line_right_tmp_i ][ line_right_limit_x0 ] - image_array[ line_right_tmp_i ][ line_right_limit_x0 - 1 ] )   )
            {
                line_right_pos = line_right_limit_x0 - 1;
            }
    }


    //当前行没有搜索到黑点
    if(  direc == -1  &&  ( line_right_black == 0 || line_right_pos == -1 )  )
    {
        uint16 tmp_rec = -1;
        uint16 tmp_x;
        uint16 tmp_y;

        //扫描结束,遇边界，无发现 return 0
        if( line_right[line_cnt_right-1].y - 2 <= 0 )      return 0;
        else tmp_y = line_right[line_cnt_right-1].y - 2;

        for(  ;  tmp_y > 0  &&  tmp_y > line_right[line_cnt_right-1].y - crossroad_search_height  ;  tmp_y --  )
        {
            tmp_x = line_right[line_cnt_right-1].x - crossroad_search_width > 0 ? line_right[line_cnt_right-1].x - crossroad_search_width :  1 ;

            //弯道复现参数
            uint16 tmp_black = 0;

            for(  tmp_x = -crossroad_search_width  ;  tmp_x <= width - 1 && tmp_x <= crossroad_search_width  ;  tmp_x ++  )
            {   
                if( image_array[tmp_y][tmp_x] == 0 )    tmp_black ++ ;
                //十字复现
                if(  image_array[tmp_y][tmp_x - 1] - image_array[tmp_y][tmp_x] == 255  )
                {
                    tmp_rec = tmp_x;
                }
            }

            if( -1 != tmp_rec)  break;
            else if( tmp_black > bend_discover )    tmp_rec = -2;
        }

        // tmp > 0 记录到复现位置, 十字复现记录数据返回 return 1
        if( tmp_rec > 0 )
        {
            uint16 tmp_yy = line_right[line_cnt_right-1].y;
            uint16 tmp_xx = line_right[line_cnt_right-1].x;
            for(  ;  tmp_yy > tmp_y  ;  tmp_yy -- )
            {
                line_right[line_cnt_right].y = tmp_yy;
                line_right[line_cnt_right].x = tmp_x;
                line_cnt_right ++;
            }
            return 2;
        }

        // tmp == -1 未查找到点,当前为十字路口，返回 return -1
        if( -1 == tmp_rec)
        {
            line_right_black = 0;
            tmp_y = line_right[line_cnt_right-1].y - 2;
            
            for( tmp_x = 0 ; tmp_x < width ; tmp_x ++)
            {
                if( 0 == image_array[tmp_y][tmp_x] )    line_right_black ++;
            }

            if( line_right_black < cross_discover )  
            {
                   
                return -1;
            }
        }

        // return -2 当前为弯道
        return -2;

    }


    //找到一个点 返回 return 1
    else
    {
        line_right[line_cnt_right].y = line_right[line_cnt_right-1].y + direc;
        line_right[line_cnt_right].x = line_right_pos;
        line_cnt_right ++;
        return 1;
    }


}














#endif
