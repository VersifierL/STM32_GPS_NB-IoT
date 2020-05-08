#include "dht11.h"


void DHT11_IO_OUT (void){ //端口变为输出
	GPIO_InitTypeDef  GPIO_InitStructure; 	
  GPIO_InitStructure.Pin = GPIO_PIN_7; //选择端口号（0~15或all）                        
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //选择IO接口工作方式       
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; //设置IO接口速度（2/10/50MHz）    
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_IO_IN (void){ //端口变为输入
	GPIO_InitTypeDef  GPIO_InitStructure; 	
  GPIO_InitStructure.Pin = GPIO_PIN_7; //选择端口号（0~15或all）                        
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT ; //选择IO接口工作方式       
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//复位DHT11
void DHT11_Rst(void)           
{                 
    DHT11_IO_OUT();         //SET OUTPUT
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);                 //设置DQ为0
    delay_ms(20);            //延时18ms以上
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);                    //DQ=1
    delay_us(30);             //延时20~40us
}
//检查DHT11是否存在 1不存在，0存在
uint8_t DHT11_Check(void)            
{   
    uint8_t retry=0;
    DHT11_IO_IN();//SET INPUT
    while (DHT11_DQ_IN&&retry<100)//DHT11延时40~80us
    {
        retry++;
        delay_us(3);// 1us
    };
    if(retry>=100)return 1;
    else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11延时40~80us
    {
        retry++;
        delay_us(3);
    };
    if(retry>=100)return 1;
    return 0;
}
//读取单个位数据
uint8_t DHT11_Read_Bit(void)                          
{
    uint8_t retry=0;
    while (DHT11_DQ_IN&&retry<100)//DHT11延时40~80us
    {
        retry++;
        delay_us(3);
    }
    retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11延时40~80us
    {
        retry++;
        delay_us(3);
    }
    delay_us(50);//??40us
    if(DHT11_DQ_IN)
        return 1;
    else
        return 0;
}
//读取字节数据值

uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat|=DHT11_Read_Bit();
    }
    return dat;
}
//读取温湿度数据值
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
    uint8_t buf[5];
    uint8_t i;
    DHT11_Rst();
    if(DHT11_Check()==0)
    {
        for(i=0;i<5;i++)//读取40bit数据
        {
            buf[i]=DHT11_Read_Byte();
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *humi=buf[0];
            *temp=buf[2];
        }
    }else return 1;
    return 0;
}
//初始化DHT11温湿度传感器           
uint8_t DHT11_Init(void)
{          
    DHT11_Rst();  //复位HT11
    return DHT11_Check();//查看DHT11的返回
}

