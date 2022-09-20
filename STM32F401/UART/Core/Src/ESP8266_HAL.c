/*
 * ESP8266_HAL.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */


#include "UartRingbuffer_multi.h"
#include "ESP8266_HAL.h"
#include "stdio.h"
#include "string.h"
#include "main.h"


char buffer[20];

/*****************************************************************************************************************************************/

void ESP_Init (char *SSID, char *PASSWD)
{
	char data[80];

	Ringbuf_init();
    /*
	Step1 Reset ESP8266  wait for 10ms
	*/
	Uart_sendstring("AT+RST\r\n", device_uart);
	Uart_sendstring("RESETTING.", pc_uart);
	for (int i=0; i<5; i++)
	{
		Uart_sendstring(".", pc_uart);
		HAL_Delay(1000);
	}

     
	/********* AT **********/
	//ESP8266 UART sending AT  and wait for ESP8266 receive OK
	Uart_sendstring("AT\r\n", device_uart);
	while(!(Wait_for("AT\r\r\n\r\nOK\r\n", device_uart)));
	Uart_sendstring("AT---->OK\n\n", pc_uart);


	/********* AT+CWMODE=1 **********/
	//set module as Station mode Access Point 
	Uart_sendstring("AT+CWMODE=1\r\n", device_uart);
	while (!(Wait_for("AT+CWMODE=1\r\r\n\r\nOK\r\n", device_uart)));
	Uart_sendstring("CW MODE---->1\n\n", pc_uart);


	/********* AT+CWJAP="SSID","PASSWD" **********/
	//Join the target IP  connect to an Access Point (like a router)
	Uart_sendstring("connecting... to the provided AP\n", pc_uart);
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data, device_uart);
	while (!(Wait_for("WIFI GOT IP\r\n\r\nOK\r\n", device_uart)));
	sprintf (data, "Connected to,\"%s\"\n\n", SSID);
	Uart_sendstring(data,pc_uart);


	/********* AT+CIFSR **********/
	//This AT Command is used to obtain the IP Address of the ESP8266 WiFi Module
	Uart_sendstring("AT+CIFSR\r\n", device_uart);
	while (!(Wait_for("CIFSR:STAIP,\"", device_uart)));
	while (!(Copy_upto("\"",buffer, device_uart)));
	while (!(Wait_for("OK\r\n", device_uart)));
	int len = strlen (buffer);
	buffer[len-1] = '\0';
	sprintf (data, "IP ADDR: %s\n\n", buffer);
	Uart_sendstring(data, pc_uart);

    /********* AT+CIPMUX **********/
	//This AT Command is used to enable or disable multiple TCP Connections
	Uart_sendstring("AT+CIPMUX=1\r\n", device_uart);
	while (!(Wait_for("AT+CIPMUX=1\r\r\n\r\nOK\r\n", device_uart)));
	Uart_sendstring("CIPMUX---->OK\n\n", pc_uart);

	Uart_sendstring("AT+CIPSERVER=1,80\r\n", device_uart);
	while (!(Wait_for("OK\r\n", device_uart)));
	Uart_sendstring("CIPSERVER---->OK\n\n", pc_uart);

	Uart_sendstring("Now Connect to the IP ADRESS\n\n", pc_uart);

}



