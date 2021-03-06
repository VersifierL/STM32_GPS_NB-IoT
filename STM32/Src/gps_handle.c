#include "gps_handle.h"

static int GpsUtc(char *p1, char *p2) //获取UTC时间
{
    char tmp;

    tmp = (*p1++ - '0')*10;
    tmp +=(*p1++ - '0');
    GPS_Data.time_hour = tmp;

    tmp = (*p1++ - '0')*10;
    tmp +=(*p1++ - '0');
    GPS_Data.time_min = tmp;

    tmp = (*p1++ - '0')*10;
    tmp +=(*p1++ - '0');
    GPS_Data.time_sec = tmp;

    tmp = (*p2++ - '0')*10;
    tmp +=(*p2++ - '0');
    GPS_Data.time_day = tmp;

    tmp = (*p2++ - '0')*10;
    tmp +=(*p2++ - '0');
    GPS_Data.time_month = tmp;

    tmp = (*p2++ - '0')*10;
    tmp +=(*p2++ - '0');
    GPS_Data.time_year = tmp + 2000;

    return 1 ;
}
static int split_string(char split1, char split2, char *data, char **p_group, int group_size) //分割函数
{
    int current_site;
    int num = 0;
    int start = 0;
    int len = strlen(data);
    for(current_site = 0; current_site < len; current_site++)
    {
        if(data[current_site] == split1||data[current_site] == split2)
        {
            *(p_group+num) = (char* )&data[start];
            start = current_site + 1;  // point next one
            data[current_site] = '\0';
            num++;
            if (num > group_size)
                return num;
        }
    }
    if(num == group_size)
        return num;
    else if(start <= current_site && num < group_size)
       {
            *(p_group+num) = &data[start];
            num++;
       }
    return num;
}
static void DataHandle(char * data)//GPS数据解析
{
    char *p_group[20] ={0};
    double fbuf=0.0;
    int cnt;
    int tmc;
    int str_tmp;
    char strbuf[4];
    char *ptr;
    cnt = split_string(',', '*', data, p_group, 20); //根据',''/'分割函数
    printf("cnt:%d\n",cnt);
    tmc = GpsUtc(&p_group[1][0],&p_group[9][0]);  //GpsUtc(p_group[1],p_group[9]) also is ok；

    //lat
		int32_t lat;
    ptr = &p_group[3][0];
    strbuf[0] = *ptr ++;
    strbuf[1] = *ptr ++;
    strbuf[2] = '\0';
    str_tmp = (uint32_t)atoi(strbuf)*100000UL;
    fbuf = atof(ptr)*100000UL/60;
    str_tmp = str_tmp + fbuf;
    //
    ptr = &p_group[4][0];
    if(*ptr == 'N')
        GPS_Data.lat_ns = 1;
    else
        GPS_Data.lat_ns = 0;
    lat = (GPS_Data.lat_ns==1) ? str_tmp : -str_tmp;
		if(lat != 0)
		{
			GPS_Data.lat = lat;
		}
    //lon
		int32_t lon;
    ptr = &p_group[5][0];
    strbuf[0] = *ptr ++;
    strbuf[1] = *ptr ++;
    strbuf[2] = *ptr ++;
    strbuf[3] = '\0';
    str_tmp = (uint32_t)atoi(strbuf)*100000UL;
    fbuf = atof(ptr)*100000UL/60;
    str_tmp = str_tmp + fbuf;
    //
    ptr = &p_group[6][0];
    if(*ptr == 'E')
        GPS_Data.lon_ew = 1;
    else
        GPS_Data.lon_ew = 0;
    lon = (GPS_Data.lon_ew==1) ? str_tmp : -str_tmp;
		if(lon != 0)
		{
			GPS_Data.lon = lon;
		}
    //speed
    ptr = &p_group[7][0];
    GPS_Data.speed = (uint16_t)(atof(ptr)*1.852f+0.5);
    //course
    ptr = &p_group[8][0];
    GPS_Data.course = (uint16_t)(atof(ptr));
		if(GPS_Data.lat!=0&&GPS_Data.lon!=0)
		{
			GPS_Data.is_fixed_position = 1;
		}
		else
		{
			GPS_Data.is_fixed_position = 0;
		}
}

void get_gps_data()
{
    if(GPS_Data.is_get_data)
    {
        DataHandle((char *)GPS_Data.GPS_Buffer);
				uart_data_handle();//测试
				//printf("DATA:%s\n",GPS_Data.GPS_Buffer);
				if(GPS_Data.is_fixed_position == 1)
				{
					if(GPS_Data.lat > GPS_Data_Test.lat_one && GPS_Data.lat < GPS_Data_Test.lat_two && GPS_Data.lon > GPS_Data_Test.lon_one && GPS_Data.lon < GPS_Data_Test.lon_two)
					{
               GPS_Data_Test.in_rail = 1;
					}
          else
          {
               GPS_Data_Test.in_rail = 0;
          }
          printf("in_rail:%d\n",GPS_Data_Test.in_rail);
					printf("GPS_Data.year:%d\n",GPS_Data.time_year);
					printf("GPS_Data.month:%d\n",GPS_Data.time_month);
					printf("GPS_Data.day:%d\n",GPS_Data.time_day);
					printf("GPS_Data.hour:%d\n",GPS_Data.time_hour);
					printf("GPS_Data.min:%d\n",GPS_Data.time_min);
					printf("GPS_Data.sec:%d\n",GPS_Data.time_sec);
					printf("GPS_Data.lat:%lf\n",(double)GPS_Data.lat/100000);
					printf("GPS_Data.lon:%lf\n",(double)GPS_Data.lon/100000);
					printf("GPS_Data.speed:%d\n",GPS_Data.speed);
					printf("GPS_Data.course:%d\n",GPS_Data.course);
				}
     //   HAL_UART_Transmit(&huart2,GPS_Data.GPS_Buffer,strlen(GPS_Data.GPS_Buffer),300);
        GPS_Data.is_get_data = 0;
    }
}

void uart_data_handle()
{
		int cnt;
	  char *p_group[20] ={0};
		if(Uart_Data.is_get_data)
		{
			cnt = split_string('+', ',', Uart_Data.buffer, p_group, 20); //根据',''/'分割函数
			GPS_Data.lat = atoi(p_group[2]);
			GPS_Data.lon = atoi(p_group[3]);
			printf("%d\n",GPS_Data.lat);
			printf("%d\n",GPS_Data.lon);
			Uart_Data.is_get_data = 0;
		}
}

//设置围栏
static void GPS_data_test_handle(char * data)
{
    char *p_group[100];
		int tmp_a;
		int tmp_b;
		int tmp_c;
		int tmp_d;
	  p_group[1] = strstr(data,"\"longtitude_two\":"); 
	  p_group[2] = strstr(p_group[1],"\"latitude_one\":"); 
	  p_group[3] = strstr(p_group[2],"\"latitude_two\":"); 
		p_group[0] = strstr(p_group[3],"\"longtitude_one\":"); 
	
		tmp_b = strlen("\"longtitude_two\":");
		p_group[1][tmp_b+8] = '\0';
		GPS_Data_Test.lon_two = atoi(&p_group[1][tmp_b]);
		
		tmp_c = strlen("\"latitude_one\":");
		p_group[2][tmp_c+7] = '\0';
		GPS_Data_Test.lat_one = atoi(&p_group[2][tmp_c]);
		
		tmp_d = strlen("\"latitude_two\":");
		p_group[3][tmp_d+7] = '\0';
		GPS_Data_Test.lat_two = atoi(&p_group[3][tmp_d]);
		
		tmp_a = strlen("\"longtitude_one\":");
		p_group[0][tmp_a+8] = '\0';
		GPS_Data_Test.lon_one = atoi(&p_group[0][tmp_a]);
		
		printf("GPS_Data_Test.lon_one: %d\n",GPS_Data_Test.lon_one);
    printf("GPS_Data_Test.lon_two: %d\n",GPS_Data_Test.lon_two);
    printf("GPS_Data_Test.lat_one: %d\n",GPS_Data_Test.lat_one);
    printf("GPS_Data_Test.lat_two: %d\n",GPS_Data_Test.lat_two);
}

void set_gps_data()
{   
		if(strlen((const char *)GPS_Data_Test.GPS_Buffer)>10){
			GPS_data_test_handle((char *)GPS_Data_Test.GPS_Buffer);
		}
}
