#ifndef camera
#define camera

// typedef char uint8;
#define width 60
#define height 80

#define limit_x
#define limit_y


#define point_count 80

// uint8 line_left[point_count][2];
// struct Site_t
// {
//     uint16 x;
//     uint16 y;
// };

Site_t line_left[point_count],line_right[point_count],line_mid[point_count];
uint8 line_cnt_left, line_cnt_right, line_cnt_mid;
// 0-黑色 255-白色
void simplemid(uint8 **image_array)
{

    //行和列
    uint8 i, j;
    //临时保存左右边界
    uint8 tmp_left, tmp_right;
    //扫描到当前行数计数
    line_cnt_left = line_cnt_right = line_cnt_right = 0;

    //前三行扫描，扫描失败保存左右边界
    for(i = 0; i < 3; i++)
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


    //单独扫描左线
    // uint8 left_i, limit_y0, limit_y1;
    // uint8 scan_direct = 1;

    //扫描方向
    uint8 line_left_direc = 1;
    uint8 line_right_direc = 1;

    //扫描数
    uint8 n = 3; 

    while(n < point_count)
    {
        // 找到任何一个点
        // int flag_left = 0;
        uint8 line_left_limit_x0  =  (line_left[line_cnt_left-1].x - limit_x) > 0    ?    line_left[line_cnt_left-1].x    :    1    ;
        uint8 line_left_limit_x1  =  (line_left[line_cnt_left-1].x + limit_x) <= width - 1    ?    line_left[line_cnt_left-1].x    :    width - 1    ;
        uint8 line_left_space = 0;
        uint8 line_left_pos = -1;
        // left_j = ( line_left[line_p-1].y - limit_y ) >= 0? -limit_y
        // limit_y0 = (line_left[line_cnt_left-1].y - limit_y) > 0 ? line_left[line_cnt_left-1].y : 1;
        // limit_y1 = (line_left[line_cnt_left-1].y + limit_y) <= width - 1 ? line_left[line_cnt_left-1].y : width - 1;
        
        for(  uint8 line_left_tmp_i = line_left[n-1].y + line_left_direc  ;  line_left_limit_x0 <= line_left_limit_x1  ;  line_left_limit_x0++  )
        {
            // if( == 1)
            // {
                if( 255  ==  image_array[line_left_tmp_i][line_left_limit_x0] )     line_left_space ++;

                if(  (line_left_direc == 1)  ==  ( image_array[ line_left_tmp_i ][ line_left_limit_x0 ] > image_array[ line_left_tmp_i ][ line_left_limit_x0 - 1 ] )   )
                {
                    line_left_pos = line_left_limit_x0;
                }
            // }


            // else
            // {
            //     if(image_array[left_i][limit_y0] < image_array[left_i][limit_y0-1])
            //     {
            //         flag_left = 1;
            //         line_left[line_cnt_left].x = left_i;
            //         line_left[line_cnt_left].y = limit_y0 - 1;
            //         line_cnt_left++;
            //     }
            // }

        }
        //没有找到点,反向查找
        if(!flag_left){
            scan_direct = -1;
            line_left[line_cnt_left].x = line_left[line_cnt_left - 1].x;
            line_left[line_cnt_left].y = line_left[line_cnt_left - 1].y;
        }  

    }


    
}



// bool line_get_left(uint8 n,)
// {


// }














#endif
