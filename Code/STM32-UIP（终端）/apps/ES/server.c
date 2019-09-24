#ifdef TARGET_SERVER
#include "server.h"
#include "uart.h"
#include "uip.h"
#include "BD_1.h"
#include "usart.h"
#include "delay.h"
#include "uart5.h"
#include <string.h>

u8 confirm[] = {0x68, 0x32, 0, 0x32, 0, 0x68, 0, 0x11, 0x20, 0x70, 0x14, 0x02, 0, 0x65, 0, 0, 0x01, 0, 0x1D, 0x16};   //确认报文，针对登录、心跳等报文
u8 SEND_ADDR[3] = {0x04, 0xE3, 0xC9};   //目的北斗终端地址
u8 buff[256];
unsigned char *data_send;
unsigned char data_len;
unsigned int LEN0;
extern int gprs_on;

//Boolean BD_1_ZJ_FLAG = FALSE;
//Boolean BD_1_IC_FLAG = FALSE;

/*
*网口接收数据处理函数
网口发给串口
*/
void 
DATA_PROCESS(u16 len,unsigned char *data)
{
	unsigned int i;

	if(data[6] == 0xF9)
	{
		printf("心跳包数据\r\n");
		data_len = 20;
		data_send = confirm;
	}
	else if(data[6] == 0xD4 && data[12] == 0x0C)
	{
//		uip_send(confirm, 20);
		BD_process_STA=1;
		printf("待分发数据包\r\n");
		for(i=0; i<len; i++)
		{
			buff[i] = data[i];
		}
		file_deal(buff, len, 60);	
	}
	else if(data[6] == 0x88 && len <70)
	{
		printf("时间信息数据\r\n");
		//sendstring1	("send to uart4\r\n");	
		BD_TXSQ_SEND(SEND_ADDR, data, len);
	}
	else
	{
		printf("无法识别数据\r\n");
	}
}                                                                                                                                              
/*
*网口发送函数
*/
int packet_send(int len,unsigned char *data)
{
//	uip_ipaddr_t ipaddr;
	
	
	if(data[1] != 0xff)
	{
		data_len = len-5;
		data_send = &data[5];
	}
	else
	{
		BD_retransmission(len, data);//判断是否需要丢包重传
	}
	
	//data_len = len;
	//data_send = data;

	return 0;
}

void
server_init(void)
{
  uip_listen(HTONS(8080));    //服务端监听端口号
}

void
server_appcall(void)
{
	static unsigned char buf[16]="Hello";
  uip_tcp_appstate_t *s = &(uip_conn->appstate);


	if(uip_aborted()) 
	{ 
	
	}
	
	if(uip_timedout()) 
	{  
		
	}
	
	if(uip_closed()) 
	{ 
//		UART_WRITE(sizeof("Closed"),"Closed");
		printf("关闭\r\n");
	}
	
	if(uip_connected()) 
	{ 
//		UART_WRITE(sizeof("Connected"),"Connected");
		printf("连接成功\r\n");

	}

	if(uip_acked()) 
	{    
		
	}
	
	if(uip_newdata()) 
	{ 
		printf("UIP接收到新数据\r\n");
		printf("BD_process_STA=%d\r\n",BD_process_STA);
		
		if(gprs_on==1&&BD_process_STA==0) 
		UART5_WRITE(uip_len,uip_appdata);
		else{
			if(BD_process_STA==0)
			{
				DATA_PROCESS(uip_len,uip_appdata);  
			}
			else printf("北斗进程繁忙\r\n");
		} 

	}
	
	if(uip_rexmit())
	{
		
	} 
	
	if(uip_poll()) 
	{
		uip_send(data_send,data_len);//网口发送数据
		data_len=0;
		//uip_close();
	}
}
#endif
