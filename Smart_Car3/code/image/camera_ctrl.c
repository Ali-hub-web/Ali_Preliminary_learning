
/*
 * camera.c
 *
 *  Created on: 2025年4月16日
 *      Author: 25215
 */
#include "camera_ctrl.h"

int test = 0;
int row, line;      //定义行和列
uint8 Threshold;    //二值化阈值

int row_max = 59;   //行最大值59
int row_min = 19;    //行最小值0
int line_max =89;  //列最大值89
int line_min =0;   //列最大值0

int all_start = 59; //所有开始59
int all_end = 0; //所有结束0

int line_prospect = 0;  //动态前瞻

int middle_count = 0; //中线有限行数
int middle_start = 59; //中线开始59
int middle_end = 49; //中线结束0
uint8_t search_end_line=25;             //从下往上找2/3
uint8 final_mid_line;
uint8 last_mid_line;

int min_width = 15;      //最小赛道宽度
uint8 picture[MAX_Height][MAX_Wdith];
uint8 new_image[MT9V03X_H][MT9V03X_W]; //复制后的原始图像数组
uint8_t img_threshold;                  //阈值
int mid_line_list[60]={0};      //中线数组

int left_edge[60]={0};                          //左边界数组
int right_edge[60]={0};                          //右边界数组
uint8 track_width[60]={0};        //赛道宽度
uint8 MID_W=43;

int valid_left = 0, valid_right = 0;    // 有效行数(有边界)
int break_left_count = 0, break_right_count = 0;    //断线行数
int both_break = 0; //同时断线行数
struct nvision_gray_image base_image_struct[1];

int left_round_flag,right_round_flag; // 圆环标志位
int cross_flag; //十字路口标志位

int state = 0; //环岛状态
int left_roudabout=0;//左环岛内圆环切点
int right_roudabout=0;//右环岛内圆环切点

/*!
  * @brief    求整形绝对值
  * @param    x1：参数一
  * @param    x2: 参数二
  * @date     2022
*/
int abs_int(int x1,int x2)
{
    int end = 0;
    if (x1 - x2 >= 0)   end = x1 - x2;
    else    end = x2 - x1;
    return end;
}

/*!
  * @brief    求浮点型绝对值
  * @param    x1：参数一
  * @param    x2: 参数二
  * @date     2022
*/
float abs_float(float x1,float x2)
{
    float end = 0;
    if (x1 - x2 >= 0)   end = x1 - x2;
    else    end = x2 - x1;
    return end;
}
/*!
  * @brief    斜率计算
  * @param    row1：为图片行
  * @param    row2：为图片行
  * @param    type：1:左边界 2：右边界 3：中线
  * @note     思路：y = kx + b
  * @return   边界
  * @date     2022/3/20
  */
float compute_k(int row1 ,int row2,int type)
{
    float k = 0;
    if (type == 1)
    {
        k = (float)((left_edge[row1] - left_edge[row2])) / (row1 - row2);
    }
    if (type == 2)
    {
        k = (float)((right_edge[row1] - right_edge[row2])) / (row1 - row2);
    }
    if (type == 3)
    {
        k = (float)((mid_line_list[row1] - mid_line_list[row2])) / (row1 - row2);
    }
    return k;
}
/****************************************
 计算最后的中线，画线（补完线的线和初始的线不一样，所以没在处理完初始边线立即显示）
 *****************************************/
void draw_line(void)
{
    for(uint8 i=59;i>19;i--)
    {
        //算中线
        //mid_line_list[i]=(left_edge[i]+right_edge[i])/2;
        tft180_draw_point(left_edge[i],i,RGB565_BLUE);
        tft180_draw_point(left_edge[i]+1,i,RGB565_BLUE);
        tft180_draw_point(right_edge[i],i,RGB565_RED);
        tft180_draw_point(right_edge[i]-1,i,RGB565_RED);
        tft180_draw_point(mid_line_list[i],i,RGB565_GREEN);
        tft180_draw_point(mid_line_list[i]+1,i,RGB565_GREEN);
        //参考中线
        tft180_draw_point(44,i,RGB565_YELLOW);
    }
    tft180_draw_point(0,30,RGB565_YELLOW);

}
void image_size(uint8 image_yuan[YUAN_Height][YUAN_Wdith], uint8 image[MAX_Height][MAX_Wdith])
{
    uint32 row,line;
//    uint16 width = YUAN_Wdith;
//    uint16 height = YUAN_Height;
    uint16 dis_width = MAX_Wdith;
    uint16 dis_height = MAX_Height;
    for ( row = 0; row < dis_height; row++)
    {
        for ( line = 0; line < dis_width; line++)
        {
            image[row][line] = image_yuan[row*YUAN_Height/MAX_Height][line*YUAN_Wdith/MAX_Wdith];
        }
    }
}

/*!
  * @brief    大津法求阈值
  * @param    image：图片信息
  * @date     2022
*/
uint8 otsuThreshold(uint8 *image)
{
    #define GrayScale 256
    uint16 Image_Width  = MAX_Wdith;
    uint16 Image_Height = MAX_Height;
    uint8 threshold;
    int X;
    uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale];
    for (Y = 0; Y < GrayScale; Y++)
    {
        HistGram[Y] = 0; //初始化灰度直方图
    }
    for (Y = 0; Y <Image_Height; Y++)
    {
        for (X = 0; X < Image_Width; X++)
        {
            HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    int16 MinValue=0, MaxValue=0;
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //像素总数
    }
    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
        PixelBack = PixelBack + HistGram[Y];    //前景像素点数
        PixelFore = Amount - PixelBack;         //背景像素点数
        OmegaBack = (double)PixelBack / Amount;//前景像素百分比
        OmegaFore = (double)PixelFore / Amount;//背景像素百分比
        PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
        MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
        MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
        if (Sigma > SigmaB)//遍历最大的类间方差g
        {
            SigmaB = Sigma;
            threshold = (uint8)Y;
        }
    }
    return threshold;
}
/*!
  * @brief    二值化
  * @param    image：图片信息
  * @date     2022
*/
void image_Bin(uint8 image[MAX_Height][MAX_Wdith])
{
    int i,j;
    for(i = 0; i <= MAX_Height; i++)
    {
        for(j = 0; j <= MAX_Wdith; j++)
        {
            if(image[i][j] > img_threshold)
            {
                image[i][j] = 255;
            }
            else
            {
                image[i][j] = 0;
            }
        }
    }
}
void image_Handler(void)
{
    if(mt9v03x_finish_flag)
    {
        memcpy(new_image[0] , mt9v03x_image[0] , MT9V03X_IMAGE_SIZE);
        image_size(new_image,picture);
        //img_threshold=otsuThreshold(picture[0]);
        //这句最后拿到循环外
        nvision_image_create_gray_from_buffer(base_image_struct,picture[0],MT9V03X_W,MT9V03X_H,0);
        //image_Bin(picture);
        nvision_image_gray_threshold(base_image_struct,img_threshold,NVISION_GRAY_THRESHOLD_MODE_OTSU);
        edge_detect(picture,left_edge,right_edge);
        //Find_line(picture);


        total_judge();
        tft180_show_gray_image(0,0,base_image_struct->data,MAX_Wdith,MAX_Height,90,60,0);
        draw_line();

        final_mid_line=find_mid_line_weight();
        tft180_show_int(95,0,final_mid_line,2);
        tft180_show_int(95,25,state,1);
        tft180_show_int(95,50,cross_flag,1);
        tft180_show_int(95,75,left_round_flag,1);

        //显示原始的灰度图像，压缩了图像0.68倍，最后一位写阈值可二值化，写0不二值化

        mt9v03x_finish_flag = 0;
    }
}
/*元素判断
 *
 *
 * */
void total_judge(void)
{
    if(cross_flag==0&&left_round_flag==0&&right_round_flag==0)
    {
        cross_judge();
    }
    if(cross_flag==0&&left_round_flag==0&&right_round_flag==0)
    {
        round_judge();
    }

    if(cross_flag==1&&left_round_flag==0&&right_round_flag==0)
    {
        cross_pass_judge();
        cross_process();
    }
    if(left_round_flag==1||right_round_flag==1)
    {
        round_process();
    }
}
/**********************************************
 普通巡线，从中间行开始搜索,左右边线，中线数组+画线
 参数：    *image                  图像地址
         *line_left_edge         左边界数组地址
         *line_right_edge        右边界数组地址
         正常赛道写法，加特殊判断
         保存上次最下行中点加权滤波，并且更新赛道中值，应对急弯
         这个可以改一下，一行一行找，找左右，然后下一个从这个中断往左右寻
 **********************************************/
void edge_detect(uint8 Image[60][90],int *line_left_edge,int *line_right_edge)
{
    //MID_W=mid_line_list[59];
    //清空数据
    for(uint8 i=MAX_Height;i>19;i--)
    {
        left_edge[i]=0;
        right_edge[i]=89;
        mid_line_list[i]=0;
        track_width[i]=0;
    }
    break_left_count = 0;
    break_right_count = 0;
    valid_left = 0;
    valid_right = 0;
    both_break = 0;

    for(uint8 i=59;i>19;i--)
    {
//        if(i<58)
//        {
//            //从上行中点继续巡线
//            MID_W=mid_line_list[i+1];
//        }
        //先往左找
        for(uint8 j=MID_W;j>0;j--)
        {
            //黑白白
            if(Image[i][j-1]==0&&Image[i][j]==255&&Image[i][j+1]==255)
            {
                line_left_edge[i]=j;
                break;
            }
            if(j==1)
            {
                //没找到黑点
                if(Image[i][j-1]==255)
                    line_left_edge[i]=0;
                else
                    line_left_edge[i]=1;
                break;
            }

        }
        for(uint8 j=MID_W;j<89;j++)
             {
                //白白黑
                 if(Image[i][j+1]==0&&Image[i][j]==255&&Image[i][j-1]==255)
                 {
                     line_right_edge[i]=j;
                     break;
                 }
                 if(j==88)
                 {
                     //没找到黑点
                     if(Image[i][j+1]==255)
                         line_right_edge[i]=89;
                     else
                         line_right_edge[i]=88;
                     break;
                 }

             }
        track_width[line]=right_edge[line]-left_edge[line];
        mid_line_list[i]=(left_edge[i]+right_edge[i])/2;
        //没做上行中点在本行黑点的处理
        //数据处理
        if(left_edge[i]==0&&mid_line_list[i]!=0)
        {
            break_left_count++;
        }
        if(left_edge[i]!=0&&mid_line_list[i]!=0)
        {
            valid_left++;
        }
        if(right_edge[i]==0&&mid_line_list[i]!=0)
        {
            break_right_count++;
        }
        if(right_edge[i]!=0&&mid_line_list[i]!=0)
        {
            valid_right++;
        }
        if (left_edge[i] == left_break && right_edge[i] == right_break && mid_line_list[i] != 0)
        {
            both_break++;
        }

//        tft180_draw_point(left_edge[i],i,RGB565_BLUE);
//        tft180_draw_point(left_edge[i]+1,i,RGB565_BLUE);
//        tft180_draw_point(right_edge[i],i,RGB565_RED);
//        tft180_draw_point(right_edge[i]-1,i,RGB565_RED);
//        tft180_draw_point(mid_line_list[i],i,RGB565_GREEN);
//        tft180_draw_point(mid_line_list[i]+1,i,RGB565_GREEN);
    }
}
void Find_line(uint8 Image[60][90])
{
    //初始化标志位
    int temp=0;
    //上一行中线
    int last_midline=44;

    //清空数据
    for(uint8 i=MAX_Height;i>0;i--)
    {
        left_edge[i]=0;
        right_edge[i]=89;
        mid_line_list[i]=0;
        track_width[i]=0;
    }
    valid_left=0;
    valid_right=0;
    break_left_count=0;
    break_right_count=0;
    both_break=0;

    //初始状态判断（判断特殊状态）
    //先从中间往左找
    for(line=MID_W;line>0;line--)
    {
        if(line==2)
        {
            if(Image[row_max][0]==255&&Image[row_max][1]==255)
            {
                //左边线丢失，置标志位补线
                left_edge[row_max]=0;
                temp=1;
            }
            //黑黑白
            if(Image[row_max][line-1]==0&&Image[row_max][line]==255&&Image[row_max][line+1]==255)
            {
                left_edge[row_max]=line;
                temp=line+2;//扫右边界起始
                break;
            }
        }
    }
    //左边界往右扫
    for(line=temp;line<line_max;line++)
    {
        if(line==line_max-2)
        {
            if(Image[row_max][line_max]==255&&Image[row_max][line_max]==255)
            {
                right_edge[row_max]=89;
                break;
            }
            //白黑黑
            else if(Image[row_max][line]==255&&Image[row_max][line+1]==0&&Image[row_max][line+2]==0)
            {
                right_edge[row_max]=line;
                break;
            }
        }
    }
    mid_line_list[row_max]=(left_edge[row_max]+right_edge[row_max])/2;
    track_width[row_max]=right_edge[row_max]-left_edge[row_max];
    //数据处理
    if(left_edge[row_max]==0&&mid_line_list[row_max]!=0)
    {
        break_left_count++;
    }
    if(left_edge[row_max]!=0&&mid_line_list[row_max]!=0)
    {
        valid_left++;
    }
    if(right_edge[row_max]==0&&mid_line_list[row_max]!=0)
    {
        break_right_count++;
    }
    if(right_edge[row_max]!=0&&mid_line_list[row_max]!=0)
    {
        valid_right++;
    }
    if (left_edge[row_max] == left_break && right_edge[row_max] == right_break && mid_line_list[row_max] != 0)
    {
        both_break++;
    }

    //还是中间往两边找，拐弯上一行中点是黑点判断
    for(uint8 row=MAX_Height-2;row>0;row--)
    {
        MID_W=(left_edge[row+1]+right_edge[row+1])/2;//更新上一行中点
        //上行中点在本行是白点
        if(Image[row][MID_W]==255)
        {
        //从中间往左找
        for(uint8 j=MID_W;j>left_break;j--)
        {
            if(j==2)
            {
                if(Image[row][j]==255&&Image[row][1]==255)
                {
                    //左边线丢失
                    left_edge[row]=0;
                    break;
                }
                if(Image[row][j]==255&&Image[row][1]==0)
                {
                    left_edge[row]=1;
                    break;
                }
            }
            if(Image[row][j-2]==0&&Image[row][j-1]==0&&Image[line][j]==255)
            {
                left_edge[row]=j;
                break;
            }

        }
        //从中间往右扫
        for(uint8 j=MID_W;j<right_break;j++)
        {

            if(j==line_max-2)
            {
                if(Image[row][j]==255&&Image[row][j+1]==255)
                {
                    //左边线丢失
                    right_edge[row]=89;
                    break;
                }
                if(Image[row][j]==255&&Image[row][j+1]==0)
                {
                    right_edge[row]=88;
                    break;
                }
            }
            if(Image[row][j]==255&&Image[row][j+1]==0&&Image[row][j+2]==0)
            {
                right_edge[row]=j;
                break;
            }
        }
        mid_line_list[line]=(left_edge[line]+right_edge[line])/2;
        track_width[line]=right_edge[line]-left_edge[line];
        }
        //上行中点在本行是黑点
        else
        {
            if(Image[row][mid_line_list[row_max]]==0||left_edge[row+1]>=79||right_edge[row+1]<=10)
            {
                continue;
            }
            else
            {
                //从新重中间往左开始扫线
                //从中间往左找
                //???????????????????这块好像有点问题
                //从左到右一直黑
                for(uint8 j=mid_line_list[row_max];j>left_break;j--)
                {
                    if(j==2)
                    {
                        //没有左边界，有右边界，右边界在最左
                        if(Image[row][0]==255&&Image[row][1]==255)
                        {
                            //左边线丢失
                            left_edge[row]=0;
                            temp=1;
                        }
                        else if(Image[row][0]==255&&Image[row][1]==0)
                        {
                            left_edge[row]=0;
                            temp=1;
                        }
                        else
                        {
                            temp=0;
                        }
                    }
                    if(Image[row][j-2]==0&&Image[row][j-1]==0&&Image[line][j]==255)
                    {
                        left_edge[row]=j;
                        break;
                    }

                }
                //从中间往右扫
                for(uint8 j=temp;j<right_break;j++)
                {

                    if(j==line_max-2)
                    {
                        if(Image[row][j]==255&&Image[row][j+1]==255)
                        {
                            //右边线丢失
                            right_edge[row]=89;
                            break;
                        }
                        if(Image[row][j]==255&&Image[row][j+1]==0)
                        {
                            right_edge[row]=88;
                            break;
                        }
                    }
                    if(Image[row][j]==255&&Image[row][j+1]==0&&Image[row][j+2]==0)
                    {
                        right_edge[row]=j;
                        break;
                    }
                }
                mid_line_list[line]=(left_edge[line]+right_edge[line])/2;
                track_width[line]=right_edge[line]-left_edge[line];
            }
        }
        //数据处理
        if(left_edge[row]==0&&mid_line_list[row]!=0)
        {
            break_left_count++;
        }
        if(left_edge[row]!=0&&mid_line_list[row]!=0)
        {
            valid_left++;
        }
        if(right_edge[row]==0&&mid_line_list[row]!=0)
        {
            break_right_count++;
        }
        if(right_edge[row]!=0&&mid_line_list[row]!=0)
        {
            valid_right++;
        }
        if (left_edge[row] == left_break && right_edge[row] == right_break && mid_line_list[row] != 0)
        {
            both_break++;
        }
    }
}
/*!
  * @brief    两点之间补线
  * @param    x：为图片行
  * @param    y：为图片列
  * @param    type：1:左边界 2：右边界 3：中线
  * @note     思路：y = kx + b
  * @return   边界
  * @date     2022/3/20
  */
void straight(int x1, int y1, int x2, int y2,int type)
{
    float k = 0;
    float b = 0;
    int line  = 0;
    k = (float)((y2 - y1)) / (x2 - x1);
    b = y1 - k*x1;
    line = (int)(k*row + b);
    if (type == 1)
    {
        if (x1-x2>0)
        {
            for ( row = x1; row > x2; row--)
            {
                left_edge[row] = (int)(k*row + b);
                mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
            }
        }
        else
        {
            for ( row = x1; row < x2; row++)
            {
                left_edge[row] = (int)(k*row + b);
                mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
            }
        }
    }
    if (type == 2)
    {
        if (x1-x2>0)
        {
            for ( row = x1; row > x2; row--)
            {
                right_edge[row] = (int)(k*row + b);
                mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
            }
        }
        else
        {
            for ( row = x1; row < x2; row++)
            {
                right_edge[row] = (int)(k*row + b);
                mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
            }
        }
    }
    if (type == 3)
    {
        if (x1-x2>0)
        {
            for ( row = x1; row > x2; row--)
            {
                mid_line_list[row] = (int)(k*row + b);
            }
        }
        else
        {
            for ( row = x1; row < x2; row++)
            {
                mid_line_list[row] = (int)(k*row + b);
            }
        }
    }
}
/*
 * 根据已知斜率补线
 * k 已知斜率，x1起始点列数，y1起始点行数，x2结束行，
 * type 1：左边线 2：右边线 3：中线
 * */
void straight_k(float k,int x1, int y1, int x2,int type)
{
    float b = 0;
    b=y1-k*x1;
    switch (type)
    {
        case 1:
        {
            if (x1-x2>0)
            {
                for ( row = x1; row > x2; row--)
                {
                    left_edge[row] = (int)(k*row + b);
                    mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
                }
            }
            else
            {
                for ( row = x1; row < x2; row++)
                {
                    left_edge[row] = (int)(k*row + b);
                    mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
                }
            }
            break;
        }
        case 2:
        {
            if (x1-x2>0)
            {
                for ( row = x1; row > x2; row--)
                {
                    right_edge[row] = (int)(k*row + b);
                    mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
                }
            }
            else
            {
                for ( row = x1; row < x2; row++)
                {
                    right_edge[row] = (int)(k*row + b);
                    mid_line_list[row] = (right_edge[row] + left_edge[row]) / 2;
                }
            }
            break;
        }
        case 3:
        {
            if (x1-x2>0)
            {
                for ( row = x1; row > x2; row--)
                {
                    mid_line_list[row] = (int)(k*row + b);
                }
            }
            else
            {
                for ( row = x1; row < x2; row++)
                {
                    mid_line_list[row] = (int)(k*row + b);
                }
            }
            break;
        }
    }
}
/*
 *十字判断
 * 思路：左下一行比上一行小
 *
 * */
void cross_judge(void)
{
    int i=0;
    int left1=0;
    int right1=0;
    int middle_g = 0;
    int left_flag=0, right_flag=0;
    int middle_flag = 0;

    //正入十字
    for ( row = row_max; row >24; row--)
        {
            if (left_edge[row] > left_edge[row-1] && left_edge[row] != left_break && left_edge[row-1] != left_break)
            {
                for ( i = row; i > row-5; i--)
                {
                    if (left_edge[i] < left_edge[i-1])
                    {
                        left1 = 0;
                        left_flag = 0;
                        break;
                    }
                    else
                    {
                        left_flag = 1;
                        left1 = row;
                    }
                }
                if (left_flag)
                {
                    break;
                }
            }
        }
        for ( row = row_max; row >24; row--)
        {
            if (right_edge[row] < right_edge[row-1] && right_edge[row] != right_break && right_edge[row-1] == right_break)
            {
                for ( i = row; i > row-5; i--)
                {
                    if (right_edge[i] > right_edge[i-1] && right_edge[i] != right_break && right_edge[i-1] != right_break)
                    {
                        right1 = 0;
                        right_flag = 0;
                        break;
                    }
                    else
                    {
                        right_flag = 1;
                        right1 = row;
                    }
                }
                if (right_flag)
                {
                    break;
                }
            }
        }
        //如果下一行在上一行外面，用row算，因为反复了很多遍所以最后肯定是准的
        //算的最后一次的出十字的中点
        middle_g = (left_edge[left1] + right_edge[right1]) / 2;

            for (row = (left1+right1)/2; row >21; row--)
            {
                //最后搜索到直道拐弯边界
                if (picture[row][middle_g] == 255 && picture[row-1][middle_g] == 0 && picture[row-2][middle_g] == 0)
                {
                    middle_flag = 0;
                    break;
                }
                else
                {
                    middle_flag = 1;
                }
            }

            if ((right_flag && left_flag&& middle_flag) || both_break >=3)
            {
                cross_flag = 1;
                right_flag = 0;
                left_flag = 0;
                middle_flag = 0;
            }
        if(cross_flag==0)
        {
            left_flag=0;
            right_flag=0;
            for(row=59;row>56;row--)
            {
                if(left_edge[row-1]-left_edge[row]>5)
                {
                    left_flag=1;
                    break;
                }
            }
            for(row=59;row>56;row--)
            {
                if(right_edge[row-1]-right_edge[row]>5)
                {
                    right_flag=1;
                    break;
                }
            }
            if((left_flag&&right_flag)||both_break>=3)
                cross_flag=1;
        }
}
int c_flag = 0;
//已经进入十字，下面行完全断线的情况
void cross_process(void)
{
    int pointx1_l = 59, pointx2_l = 0, pointy1_l = 10, pointy2_l = 0;
    int pointx1_r = 59, pointx2_r = 0, pointy1_r = 78, pointy2_r = 89;
    //直接补中线
    float k_midline=0.0f;
    int row_midline_x=0,row_midline_y=0;
 //清除十字标志位
// if (!c_flag)
// {
//     for ( row = 59; row >49; row--)
//     {
//         if (left_edge[row] == left_break && right_edge[row] == right_break)
//         {
//             c_flag = 1;
//         }
//         else
//         {
//             c_flag = 0;
//             break;
//         }
//     }
// }
//     //全局变量，一直会进
//     if (c_flag)
//     {
//         //前十行都一直断线
//         //看靠近车头前五行，来判定出十字
//         for ( row = 59; row >54; row--)
//         {
//             if (left_edge[row] == left_break && right_edge[row] == right_break)
//             {
//                 continue;
//             }
//             else
//             {
//                 cross_flag = 0;
//                 break;
//             }
//         }
//     }
     cross_flag=0;
     //找左下跳变点
     for ( row =59; row >19; row--)
         {
             if (left_edge[59]<8)
             {
                 pointx1_l = 59;
                 pointy1_l = 10;
                 break;
             }
             if (left_edge[row] <= left_edge[row-1])
             {
                 continue;
             }
             if (left_edge[row] > left_edge[row-1])
             {
                 pointx1_l = row;
                 pointy1_l = left_edge[row];
                 break;
             }

             if (row == row_min+1)
             {
                 pointx1_l = 59;
                 pointy1_l = 10;
             }
         }
         //  找右下断点
         for ( row = 59 ; row >19; row--)
         {
             if (right_edge[59] >81)
             {
                 pointx1_r = 59;
                 pointy1_r = 78;
                 break;
             }
             if (right_edge[row] >= right_edge[row-1])
             {
                 continue;
             }
             if (right_edge[row]< right_edge[row-1])
             {
                 pointx1_r = row;
                 pointy1_r = right_edge[row];
                 break;
             }
             if (row==1)
             {
                 pointx1_r = 59;
                 pointy1_r = 78;
             }
         }
     //因为图像看到的范围太大了总是找到赛道外噪点，做特殊情况处理


         //认为找到噪点，重新找左上拐点
         for(row=pointx1_l-1;row>15;row--)
         {
             if(left_edge[row]>left_edge[pointx1_l])
             {
//                     for(uint8 i=row;i>row-2;row--)
//                     {
//                         if(abs_int(left_edge[i],left_edge[i+1])>2)
//                         {
//                             break;
//                         }
//                         if(i==row-1)
//                         {
//                             pointx2_l = row;
//                             pointy2_l = left_edge[row];
//                         }
//                     }
                 if(abs_int(left_edge[row],left_edge[row-1])<=2&&abs_int(left_edge[row-1],left_edge[row-2])<=2)
                 {
                     pointx2_l = row;
                     pointy2_l = left_edge[row];
                 }
                 if(pointx2_l)
                 {
                     break;
                 }
             }
         }
         //  找左上断点
//         for ( row = pointx1_l-1; row >19; row-- )
//         {
//             if (picture[row][pointy1_l] == 255 && picture[row-1][pointy1_l] == 0)
//             {
//                 for (int i = row-1; i > row-10; i--)
//                 {
//                     if (abs_int(left_edge[i],left_edge[i-1]) <= 1)
//                     {
//                         pointx2_l = i;
//                         pointy2_l = left_edge[i];
//                         break;
//                     }
//                 }
//                 if(row==19)
//                 //找到左上断点，跳出循环
//                 if (pointx2_l)
//                 {
//                     break;
//                 }
//             }
//         }
         //  找右上断点（发现每次右上没找到点为0就会卡死）
         for ( row = pointx1_r-1; row >19; row-- )
         {
             if (picture[row][right_edge[pointx1_r]] == 255 && picture[row-1][right_edge[pointx1_r]] == 0)
             {
                 for (int i = row-1; i > row-10; i--)
                 {
                     if (abs_int(right_edge[i],right_edge[i-1]) <= 1)
                     {
                         pointx2_r = i;
                         pointy2_r = right_edge[i];
                         break;
                     }
                 }
                 if (pointx2_r)
                 {
                     break;
                 }
             }
         }
//         row_midline_x=(pointx2_l+pointx2_r)/2+1;
//         k_midline=compute_k(row_midline_x,row_midline_x-2,3);
         //  补线
//         tft180_show_int(0,65,pointx1_l,2);//显示左下x
//         tft180_show_int(20,65,pointy1_l,2);//显示左下y
//         tft180_show_int(40,65,pointx2_l,2);//显示左上x
//         tft180_show_int(60,65,pointy2_l,2);//显示左上y
//
//         tft180_show_int(0,85,pointx1_r,2);//显示右下x
//         tft180_show_int(20,85,pointy1_r,2);//显示右下y
//         tft180_show_int(40,85,pointx2_r,2);//显示右上x
//         tft180_show_int(60,85,pointy2_r,2);//显示右上y

         straight(pointx1_l,pointy1_l,pointx2_l,pointy2_l,1);
         straight(pointx1_r,pointy1_r,pointx2_r,pointy2_r,2);
//         straight_k(k_midline,row_midline_x,row_midline_y,59,3);
}
/*
 *
 * 通过十字，标志位清零处理
 * 思路：出十字，识别到两边单调且不断线（正常直道）
 *
 * */
void cross_pass_judge(void)
{
    uint8 left_state=1,right_state=1;
//    if(left_edge[59]<8)
//    {
//        left_state=0;
//    }
    if(break_left_count!=0)
    {
        left_state=0;
    }
    if(break_right_count!=0)
    {
        right_state=0;
    }
    if(left_state)
    {
        for(row=row_max;row>30;row--)
        {
            if(left_edge[row]>left_edge[row-2]&&left_edge[row]>left_edge[row-4])
            {
                left_state=0;
                break;
            }
            if(left_edge[row]<=2)
            {
                left_state=0;
                break;
            }
            if(left_edge[row-1]-left_edge[row]>5)
            {
                left_state=0;
                break;
            }
        }
//        for(row=row_max;row>29;row--)
//        {
//        //左边线单调
//            uint8 temp=1;
//
//            if(abs_int(left_edge[row], left_edge[row-2])>2)
//            {
//                temp=0;
//                left_state=1;
//            }
//        //断线，差3行的值>3,相邻行绝对值>2
//            if(left_edge[row]==left_break||temp)
//            {
//                left_state=0;
//                break;
//            }
//        }
    }
//    if(right_edge[59]>81)
//    {
//        right_state=0;
//    }
//    if(right_state)
//    {
        for(row=row_max;row>30;row--)
        {
            if(right_edge[row]<right_edge[row-2]&&right_edge[row]<right_edge[row-4])
            {
                right_state=0;
                break;
            }
            if(right_edge[row]>=87)
            {
                right_state=0;
                break;
            }
            if(right_edge[row]-right_edge[row-1]>5)
            {
                right_state=0;
                break;
            }
//        }
//        for(row=row_max;row>29;row--)
//        {
//            uint8 temp=1;
//        //右边线单调
//
//            if(abs_int(right_edge[row], right_edge[row-2])>2)
//            {
//                right_state=1;
//            }
//            if(temp||right_edge[row]==right_break)
//            {
//                right_state=0;
//                break;
//            }
//          if(right_edge[row]==right_break||(right_edge[row]- left_edge[row-3]>3))
//           {
//              right_state=0;
//              break;
//           }
//        }
    }
    if(left_state&&right_state)
    {
        cross_flag=0;
    }
}
/*
 * 环岛判断
 * 思路：识别圆环
 *
 *
 * */
void round_judge(void)
{
    int leftState=0,rightState=0;
    int leftArr = 0,rightArr = 0;
    int left_break_flag = 0, right_break_flag = 0;   //断线标志位
    int left_i = 0 ,right_i = 0;    //断线前的坐标
    int acc_num = 0;
    int UP_flag = 0;
    //左环岛
    //先判断右边为直线,断线判断要和十字有区分
    for(uint8 i=59;i>19;i--)
    {
        if(abs_int(right_edge[i], right_edge[i-1])<=1)//right_edge[i]>=right_edge[i-1]
        {
            rightState=1;
        }
        //有断线，右环岛（十字？）
        if(abs_int(right_edge[i], right_edge[i-1])>1)
        {
            rightState=0;
            break;
        }
    }
    //判断左边是否为有圆弧 右边不丢线则是左环岛
    if (rightState  && break_right_count == 0)//&& break_left_count > 5
    {
        //最大行开始向下寻
        for (row = row_max; row > row_min+5; row--)
        {
            //用此行断线，下一行不断线判断内圆环不太准
            //离得远没有断线状态，用差
            if (left_edge[row-1]-left_edge[row]>15)
            {
                left_i = row - 1;  //记录断线前的坐标
                left_break_flag = 1;  //断线标志位
                leftArr = 1;
                break;
            }
        }
        //断线
//        if(left_break_flag)
//        {
//            for ( row = left_i; row >19; row--)
//            {
//                //找内圆环，
//                if (left_edge[row-1] - left_edge[row] >= 1)
//                {
//                    acc_num++;//递增的
//                }
//                if (left_edge[row-1] - left_edge[row] < 0 && left_edge[row-1] != 0)
//                {
//                    if (left_edge[row-1] >= left_edge[row-2] && left_edge[row-2] >= left_edge[row-3] &&
//                            left_edge[row-1] != 0 && left_edge[row-2] != 0 && left_edge[row-3] != 0)
//                    {
////                        if (acc_num >= 2)
////                        {
////                            leftArr = 1;
////
////                        }
//                        leftArr = 1;
//
//                        //先不记录内圆环切点，因为环岛判断只进入一次，切点会更新
//                    }
//                }
//            }
//
//        }
    }
    acc_num = 0;

    /******右环岛*****/
    /* 右边线是否为直线 */

    for ( row = row_max ; row >24 ;row--)
    {
        if(abs_int(left_edge[row], left_edge[row-1])<2)
        {
            leftState = 1;
        }
        if(abs_int(left_edge[row], left_edge[row-1])>=2)
        {
            leftState = 0;
            break;
        }
    }
    //  判断右边是否为有圆弧
    if (leftState && break_right_count > 5 && break_left_count == 0)
    {
        for (row = row_max; row >19; row--)
        {
            // 左边断线
            if (right_edge[row] == right_break && right_edge[row-1] != right_break)
            {
                right_i = row - 1;  //记录断线前的坐标
                right_break_flag = 1;  //断线标志位
                break;
            }
            if (row ==20)
            {
                right_break_flag = 0;
            }
        }
        //断线
        if(right_break_flag)
        {
            for ( row = right_i; row > right_i-15; row--)
            {
                if (right_edge[row] - right_edge[row-1] >= 1)
                {
                    acc_num++;
                }
                if (right_edge[row] - right_edge[row-1] < 0 && right_edge[row-1] != right_break)
                {
                    if (right_edge[row-1] <= right_edge[row-2] && right_edge[row-2] <= right_edge[row-3]
                        && right_edge[row-1] != right_break && right_edge[row-2] != right_break && right_edge[row-3] != right_break)
                    {
                        if (acc_num >= 2)
                        {
                            rightArr = 1;
                        }
                    }
                }
            }
            acc_num = 0;
        }
    }
    if ((leftArr && rightState) || (rightArr && leftState))
    {
        for ( row = row_max; row >19; row--)
        {
            if (picture[row][mid_line_list[row_max]] == 0)
            {
                if (row < 5)
                {
                    UP_flag = 1;
                    break;
                }
                else
                {
                    UP_flag = 0;
                    break;
                }
            }
            if (row ==20)
            {
                UP_flag = 1;
            }
        }
    }
    /* 左边是直线， 检测右侧是否是环岛 */
    if ( leftState==1 && rightArr==1 && leftArr == 0 && UP_flag && break_right_count  >= 5 && break_left_count == 0)
    {
        right_round_flag = 1;
        // printf("右环岛\r\n");
    }
    /* 右边直线， 检测左侧是否是环岛 */
    if (rightState==1 && leftArr ==1 && rightArr == 0 && break_right_count == 0)// && break_left_count >= 5&& UP_flag
    {
        left_round_flag = 1;
        // printf("左环岛\r\n");
    }
}
/*
 * 找内圆环切点
 *
 * */
void roundabout(void)
{
    int left_break_flag = 0, right_break_flag = 0;   //断线标志位
    int left_i=0,right_i=0;//断点坐标，内圆环起始
    int acc_num=0;
    if(left_round_flag==1)
    {
        for(row=59;row>19;row--)
        {
            if((left_edge[row]==left_break&&left_edge[row-1]!=left_break)||(left_edge[row-1]-left_edge[row]>=10))
            {
                left_i=row-1;
                left_break_flag=1;
                break;
            }
        }
        if(left_break_flag)
        {
            for(row=left_i;row>19;row--)
            {
//                if(left_edge[row-1]-left_edge[row]>=1)
//                {
//                    acc_num++;
//                }
//                if (left_edge[row-1] - left_edge[row] < 0 && left_edge[row-1] != 0)
//                {
//                    if(left_edge[row-1]>=left_edge[row-2]&&left_edge[row-2]>=left_edge[row-3]&&
//                        left_edge[row-1]!=0&&left_edge[row-2]!=0&&left_edge[row-3]!=0)
//                    {
//                        if(acc_num>=5)
//                        {
//                            left_roudabout=row-1;
//                            break;
//                        }
//                    }
//                }
                  if(left_edge[row]<=left_edge[row-1]&&left_edge[row-1]>=left_edge[row-2])
                  {
                      left_roudabout=row-1;
                  }
                  if(left_edge[row]-left_edge[row-1]>=10)
                  {
                      break;
                  }
            }
        }
    }
//    acc_num=0;
    if(right_round_flag==1)
        {
            for(row=59;row>19;row--)
            {
                if((right_edge[row]==right_break&&right_edge[row-1]!=right_break)||(right_edge[row]-right_edge[row-1]>=10))
                {
                    right_i=row-1;
                    right_break_flag=1;
                    break;
                }
            }
            if(right_break_flag)
            {
                for(row=right_i;row>19;row--)
                {
//                    if(right_edge[row]-right_edge[row-1]>=1)
//                    {
//                        acc_num++;
//                    }
//                    if (right_edge[row] - right_edge[row-1] < 0 && right_edge[row-1] != right_break)
//                    {
//                        if(right_edge[row-1]<=right_edge[row-2]&&right_edge[row-2]<=right_edge[row-3]&&
//                            right_edge[row-1]!=89&&left_edge[row-2]!=89&&left_edge[row-3]!=89)
//                        {
//                            if(acc_num>=5)
//                            {
//                                right_roudabout=row-1;
//                                break;
//                            }
//                        }
//                    }
                    if(right_edge[row]>=right_edge[row-1]&&right_edge[row-1]<=right_edge[row-2])
                    {
                        right_roudabout=row-1;
                    }
                    if(right_edge[row-1]-right_edge[row]>=10)
                    {
                        break;
                    }
                }
            }
        }
}
int flag_xielv = 0;
int flag_round = 0;
int flag_break = 0;
void round_process(void)
{
    int i = 0;
    int flag_g = 0;
    int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;
    int state1_flag=0;
    int pian_left = 0,pian_right = 0;//左偏移量，右偏移量
    //刚进入环岛，连接下和中点
    if(left_round_flag)
    {
        switch(state)
        {
            case 0://识别到环，还没入环
            {
                //先判断是否进入下一个状态
                if(track_width[59]>=60)
                {
                    for(row=59;row>30;row--)
                    {
//                    if (left_edge[row] <= left_edge[row-1])
//                    {
//                        continue;
//                    }
//                    if (left_edge[row] >= left_edge[row-1] && left_edge[row-1] >= left_edge[row-3] &&
//                            left_edge[row] != 0 && left_edge[row-1] != 0 && left_edge[row-3] != 0)
//                    {
//                        flag_g = row;//切点
//                        break;
//                    }
                        if(left_edge[row]<left_edge[row-1])
                        {
                            state=0;
                            break;
                        }
                        if(left_edge[row]>=left_edge[row-1])
                        {
                            state=1;
                        }
                        if(abs_int(right_edge[row], right_edge[row-1])>=10)
                        {
                            state=1;
                            break;
                        }
                    }
                }
//                if (flag_g >=30)
//                {
//                    for ( row = flag_g; row >= flag_g-5; row--)
//                    {
//                        if (left_edge[row] == 0)
//                        {
//                           state = 0;
//                           break;
//                        }
//                        else
//                        {
//                            state = 1;
//                        }
//                    }
//                }
                //找下拐点
                for ( row = row_max; row > 19; row--)
                {
                    if (left_edge[row_max] < 8 && left_edge[row_max-1] < 8)
                    {
                        x1 = row_max;
                        y1 = 15;
                        break;
                    }
                    if (left_edge[row]-left_edge[row-1] >=10)
                    {
                        x1 = row;
                        y1 = left_edge[row];
                        break;
                    }
                }
                if (x1 > 50)
                {
                    x1 = row_max;
                    y1 = 15;
                }
                //  找上拐点
//                for ( row = x1-1; row > 19; row--)
//                {
//                    if (left_edge[row] == 0 || left_edge[row-1] >= left_edge[row])
//                    {
//                        continue;
//                    }
//                    if (left_edge[row] > left_edge[row-1] && left_edge[row-1] >= left_edge[row-2]
//                        && left_edge[row-1] != left_break && left_edge[row-2] != left_break)
//                    {
//                        x2 = row;
//                        y2 = left_edge[row];
//                        break;
//                    }
//                }
                roundabout();
                x2=left_roudabout;
                y2=left_edge[left_roudabout];
                if (x2 == 0)
                {
                    x2 = 30;
                    y2 = 30;
                }
                //  出口补线
                straight(x1,y1,x2,y2,1);
                break;
            }
            case 1://看到入口入环
            {
                for(row = row_max - 10;row > row_max - 20;row--)
                {
                    if((right_edge[row] == right_break)||(abs_int(right_edge[row], right_edge[row-1])>=10))
                    {
                        state = 1;
                        break;
                    }
                    else
                    {
                        state = 2;
                    }
                }
                //找下拐点
                for ( row = row_max; row > row_min; row --)
                {
                    if (right_edge[row_max] > 80 && right_edge[row_max-1] > 80)
                    {
                        x1 = row_max;
                        y1 = 80;
                        break;
                    }
                    if (right_edge[row_max] == right_break)
                    {
                        x1 = row_max;
                        y1 = 80;
                        break;
                    }
                    if (left_edge[row] == left_break || left_edge[row] <= left_edge[row-1])
                    {
                        continue;
                    }
                    if (left_edge[row] > left_edge[row-1] && left_edge[row-1] != left_break)
                    {
                        x1 = row;
                        y1 = right_edge[row];
                        break;
                    }
                }
                //找上拐点
                for ( row = x1-1; row > row_min; row--)
                {
                    for ( line = 10; line < line_max-40; line++)
                    {
                        if (picture[row][line] == 255 && picture[row][line+1] == 0)
                        {
                            for ( i = line; i <= line+5; i++)
                            {
                                if (picture[row][i] == 0 && picture[row][i+1] == 255)
                                {
                                    x2 = row;
                                    y2 = line + 6;
                                    break;
                                }
                            }
                        }
                    }
                    if (y2)
                    {
                        break;
                    }
                }
                if ( y2 > 60)
                {
                    y2 = 20;

                }
                straight(x1,y1,x2,y2,2);
                break;
            }
            case 2://环内正常巡线
            {
                break;
            }
            case 3://出环岛
            {
                break;
            }
            case 4://走直线
            {
                break;
            }
        }

    }

}
uint8 mid_weight_list[60]=
{
     //1,1,1,1,1,1,1,1,1,1,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,1,1,1,1,1,
     1,1,1,1,5,6,7,8,9,10,
     10,9,8,7,6,6,6,5,5,5,
     5,5,5,1,1,1,1,1,1,1,

//     0,0,0,0,0,0,0,0,0,0,
//     1,1,1,1,1,5,5,5,5,5,
//     11,12,13,14,15,16,15,14,13,12,
//     11,10,9,8,7,5,5,5,5,5,
//     1,1,1,1,1,1,1,1,1,1,
};
//根据权重求中线值
uint8 find_mid_line_weight(void)
{
    uint8 mid_line_value=MID_W; //最终中线输出值
    uint8 mid_line=MID_W;       //本次中线值
    uint32 weight_midline_sum=0;//加权中线累加值
    uint32 weight_sum=0;        //权重累加值
    for(uint8 i=59;i>0;i--)
    {
        weight_midline_sum+=mid_line_list[i]*mid_weight_list[i];
        weight_sum+=mid_weight_list[i];
    }
    mid_line=(uint8)(weight_midline_sum/weight_sum);
    mid_line_value=last_mid_line*0.2+mid_line*0.8;
    last_mid_line=mid_line_value;
    return mid_line_value;
}
