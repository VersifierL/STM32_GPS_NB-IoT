#include "BC28.h"
#include <stdlib.h>

/*阿里云三元组*/
#define PRODUCEKEY "a1K7KsoyAYX"        //修改产品秘钥
#define DEVICENAME "A001"           //修改设备名称
#define DEVICESECRET "EdR2N2b75BLSRg8pcMIzBoiwg9XpMFgh" //修改设备秘钥
#define SUBSCRIBE "/a1K7KsoyAYX/A001/user/get"      //订阅入口
#define ISSUE "/sys/a1K7KsoyAYX/A001/thing/event/property/post"


char *strx;
char atstr[BUFLEN];//发送数据区
int err = 0;//全局变量

//发送数据等待回应（重发机制）
int send_data_ack(char *cmd, char *ack, __IO uint32_t nTime)
{			
		clear_nbiot_buffer();
		HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), 100);  //发送AT指?
		HAL_Delay(nTime);				//等待
		printf("%s",USART_RX_NBIOT_BUF);
		strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//判断
		 clear_nbiot_buffer();	//清空bufferr
		if(strx != NULL)//判断
		{
			printf("success\n");	
			return 0;		
		}
		else
		{
			printf("error\n");
			return 1;
		}
}
//发送数据等待回应（一直等待）
int send_data_wait_ack(char *cmd, char *ack, __IO uint32_t nTime)
{			
		clear_nbiot_buffer();
		HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), 100);  //发送AT指?
		HAL_Delay(nTime);				//等待
		printf("%s",USART_RX_NBIOT_BUF);
		strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//判断
		while(strx==NULL)
    {
        strx = strstr((const char*)USART_RX_NBIOT_BUF,(const char*)ack);//判断
    }
		clear_nbiot_buffer();	//清空bufferr

}
//发送数据
void send_data(char *cmd, __IO uint32_t nTime)
{
		HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), 100);   //发送AT指令
		HAL_Delay(nTime);				//等待
		clear_nbiot_buffer();	//清空buffer
}
//华为云
int BC28_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//允许错误值
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //只要不是ERROR基本读到卡号
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP附着
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //激活成功获取IP地址
		while(send_data_ack("AT+NCDP=49.4.85.232\r\n","OK",300));
		while(send_data_ack("AT+CGPADDR\r\n","OK",300));
		return err;
}

void BC28_Senddata(uint8_t* len,uint8_t *data)
{	
		memset(atstr,0,BUFLEN);
		uint8_t data_temp[20]={'0','0','0','1'};
		strncat(data_temp, data,12);
		sprintf(atstr,"AT+NMGS=%s,%s\r\n",len,data_temp);
		send_data_ack(atstr,"OK",300); 
}
//UDP
int BC28_UDP_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//允许错误值
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //只要不是ERROR基本读到卡号
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP附着
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //激活成功获取IP地址
		send_data_ack("AT+NSOCL=0\r\n","OK",300);//清socket
		while(send_data_ack("AT+NSOCR=DGRAM,17,8888,1\r\n","OK",300)); //建立一个udp socket
		return err;
}
void BC28_UDP_Senddata(uint8_t* len,uint8_t *data)
{	
		memset(atstr,0,BUFLEN);
	  sprintf(atstr,"AT+NSOST=0,47.116.9.52,8888,%s,%s\r\n",len,data);
		send_data_ack(atstr,"OK",300); 
}

//阿里云 MQTT
int BC28_ALIYUN_Init()
{
		while(send_data_ack("AT\r\n","OK",300)); 
		while(send_data_ack("AT+CGSN=1\r\n","OK",300)); 
		while(send_data_ack("AT+CMEE=1\r\n","OK",300));//允许错误值
		while(!send_data_ack("AT+CIMI\r\n","ERROR",300)); //只要不是ERROR基本读到卡号
		while(send_data_ack("AT+CGATT=1\r\n","OK",300)); //PDP附着
		while(send_data_ack("AT+CGATT?\r\n","+CGATT:1",300)); //激活成功获取IP地址
    send_data("AT+QMTCLOSE=0\r\n",300);//Disconnect a client from MQTT server
		send_data("AT+QMTDISC=0\r\n",300);//删除句柄
		
		//配置可选连接阿里云可选项?
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"OK",300); 
	
		send_data_wait_ack("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n","+QMTOPEN: 0,0",300); //登录阿里云平台 
		
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"+QMTCONN: 0,0,0",300);//发送链接到阿里云
		
		memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
		send_data_wait_ack(atstr,"+QMTSUB: 0,1,0,1",300);//订阅
		return err;
}

void BC28_ALIYUN_Senddata1(uint8_t *temp_data, uint8_t *humi_data, uint8_t *voltage_data)
{	
	char buf5[]={0x1A,0x0D,0x0A};
    memset(atstr,0,BUFLEN); //发送数据命令
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
		
    memset(atstr,0,BUFLEN); //发送数据
		sprintf(atstr,"{params:{temp:%s,humi:%s,voltage:%s}}",temp_data,humi_data,voltage_data);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
	
    HAL_Delay(30);
    HAL_UART_Transmit(&huart3, buf5, strlen(buf5), 30); 
}

void BC28_ALIYUN_Senddata2(uint8_t *latitude_data, uint8_t *longtitude_data)
{	
	char buf5[]={0x1A,0x0D,0x0A};
    memset(atstr,0,BUFLEN); //发送数据命令
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
		
    memset(atstr,0,BUFLEN); //发送数据
		sprintf(atstr,"{params:{latitude:%s,longtitude:%s}}",latitude_data,longtitude_data);
    printf("atstr = %s \r\n",atstr);
    send_data(atstr,300);
	
    HAL_Delay(30);
    HAL_UART_Transmit(&huart3, buf5, strlen(buf5), 30); 
}


