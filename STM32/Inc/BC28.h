#include "usart.h"
#include "main.h"
#include "string.h"
#define SERVERIP "49.4.85.232" //»ªÎªÔÆ
#define SERVERPORT 40001
#define BUFLEN 256      
void Clear_Buffer(void);
int send_data_ack(char *cmd, char *ack, __IO uint32_t nTime);
void send_data(char *cmd, __IO uint32_t nTime);

int BC28_Init(void);
void BC28_Senddata(uint8_t* len,uint8_t *data);

int BC28_UDP_Init(void);
void BC28_UDP_Senddata(uint8_t* len,uint8_t *data);

int BC28_ALIYUN_Init(void);
void BC28_ALIYUN_Senddata1(char *temp_data, char *humi_data, char *voltage_data);
void BC28_ALIYUN_Senddata2(char *latitude_data, char *longtitude_data, char *rail_data);
void BC28_ALIYUN_Recievedata(void);
char is_translate_status(void);
