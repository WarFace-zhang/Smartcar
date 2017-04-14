#ifndef camera
#define camera

// typedef char uint8;
#define width 60
#define height 80

#define limit_x
#define limit_y

// #define point_count 80

// uint8 left_line[point_count][2];
// struct Site_t
// {
//     uint16 x;
//     uint16 y;
// };

Site_t left_line[point_count],right_line[point_count],mid_line[point_count];
uint8 line_cnt_left, line_cnt_right, line_cnt_mid;
// 0-黑色 255-白色
void simplemid(uint8 **image_array)
{
    uint8 i, j;
    uint8 tmp_left, tmp_right;
    line_cnt_left = line_cnt_right = line_cnt_right = 0;
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
        left_line[line_cnt_left].y = i; left_line[line_cnt_left].x = tmp_left;
        line_cnt_left++;

        right_line[line_cnt_right].y = i; right_line[line_cnt_right].x = tmp_right;
        line_cnt_right++;

        mid_line[line_cnt_mid].y = ( left_line[line_cnt_mid].y + right_line[line_cnt_mid].y ) / 2; 
        mid_line[line_cnt_mid].x = ( left_line[line_cnt_mid].x + right_line[line_cnt_mid].x ) / 2;
        line_cnt_mid++;
    }

    //单独扫描左线
    
    uint8 left_i, limit_y0, limit_y1;
    uint8 scan_direct = 1;
    uint8 n = 0;
    while( n < point_count )
    {
        line_get_left();
        line_get_right();







    }

    


    // for(left_i = i; ; (left_i += scan_direct))
    // {
    //     // 找到任何一个点
    //     int flag_left = 0;
        
    //     // left_j = ( left_line[line_p-1].y - limit_y ) >= 0? -limit_y
    //     limit_y0 = (left_line[line_cnt_left-1].y - limit_y) > 0 ? left_line[line_cnt_left-1].y : 1;
    //     limit_y1 = (left_line[line_cnt_left-1].y + limit_y) <= width - 1 ? left_line[line_cnt_left-1].y : width - 1;
        
    //     for( ; limit_y0 <= limit_y1; limit_y0++)
    //     {
    //         if(scan_direct == 1)
    //         {
    //             if(image_array[left_i][limit_y0] > image_array[left_i][limit_y0-1])
    //             {
    //                 flag_left = 1;
    //                 left_line[line_cnt_left].x = left_i;
    //                 left_line[line_cnt_left].y = limit_y0;
    //                 line_cnt_left++;
    //             }
    //         }
    //         else
    //         {
    //             if(image_array[left_i][limit_y0] < image_array[left_i][limit_y0-1])
    //             {
    //                 flag_left = 1;
    //                 left_line[line_cnt_left].x = left_i;
    //                 left_line[line_cnt_left].y = limit_y0 - 1;
    //                 line_cnt_left++;
    //             }
    //         }

    //     }
    //     //没有找到点,反向查找
    //     if(!flag_left){
    //         scan_direct = -1;
    //         left_line[line_cnt_left].x = left_line[line_cnt_left - 1].x;
    //         left_line[line_cnt_left].y = left_line[line_cnt_left - 1].y;
    //     }  

    // }


    
}



bool line_get_left(uint8 n,)
{


}














#endif
