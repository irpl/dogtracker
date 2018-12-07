/*
 * DogTrackerProject.c
 *
 * Created: 29/11/2018 8:35:09 PM
 * Author : Justine
 */ 

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BAUD 9600
int threshold=0;
#define BAUD_PRESCALAR ( (F_CPU / (BAUD * 16UL) ) - 1 )

ISR(USART_RX_vect){
	
	
	
}


void USART_init() {
	UBRR0H = (uint8_t)(BAUD_PRESCALAR >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALAR);
	UCSR0B |= (1 << RXEN0)|(1 << TXEN0);
	
	UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00);
}

void USART_send_char(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART_send_str(char data[]) {
	int  i = 0;
	
	while (data[i] != 0) {
		USART_send_char(data[i]);
		i++;
	}
}


unsigned char USART_receive_char()
{
	PORTC |= (1<<4);
	while(!(UCSR0A & (1<<RXC0)));
	PORTC = 0;
	return(UDR0);
}

int i;
void getLatLong(char * nmea){
	
	USART_send_str("AT+CGNSINF\r\n");
	PORTC |= (1<<5);
	while(!USART_receive_char());
	
	i=0;
	while (1) {
		
		nmea[i] = USART_receive_char();	
		if (nmea[i] == 'O') break;
		i++;
	}
	nmea[i] = '\0';
	PORTC = 0x00;
	
	//USART_send_str(nmea);
	//while(1);
	
}

char data[300];
int size;
char sizeChar[4];
void post(){
	
	
	// initialize http connection
	USART_send_str("AT+HTTPINIT\r\n");
	_delay_ms(100);
	
	// set up  headers
	// SET CID to 1 , User agent : Fona , URL : server Url , Content : application/json
	USART_send_str("AT+HTTPPARA=\"CID\",1\r\n");
	_delay_ms(100);
	
	USART_send_str("AT+HTTPPARA=\"UA\",\"FONA\"\r\n");
	_delay_ms(100);
	
	USART_send_str("AT+HTTPPARA=\"URL\",\"http://philliplogan.com:5000/data\"\r\n");
	_delay_ms(100);
	
	USART_send_str("AT+HTTPPARA=\"CONTENT\",\"text/plain\"\r\n");		// sending the data as a json
	_delay_ms(100);
	
	//getLatLong(&latitude,&longitude);
	
	getLatLong(data);
	
	// specify the size of the data to be sent and specify a time out
	size = strlen(data);

	
	itoa(size,sizeChar,10);
	USART_send_str("AT+HTTPDATA=");
	USART_send_str(sizeChar);
	USART_send_str(",10000\r\n");		 //timeout
	_delay_ms(100);
	
	
	// Send data

	USART_send_str(data);
	USART_send_str("\r\n");
	_delay_ms(2000);
	
	// specify the type of request (get / post )
	USART_send_str("AT+HTTPACTION=1\r\n");
	_delay_ms(2000);
	
	// read response from server
	USART_send_str("AT+HTTPREAD\r\n");
	_delay_ms(1000);
	
	//terminate the http connection
	USART_send_str("AT+HTTPTERM\r\n");
	_delay_ms(1000);
	//free(data);
	
}

void gpsinit(){
	USART_send_str("AT+CGNSPWR=1\r\n");
	_delay_ms(2000);
}

void httpinit(){
	
	_delay_ms(60000);

	// set apn of fona to ppinternet to connect to GPRS network for Flow prepaid
	USART_send_str("AT+SAPBR=3,1,\"APN\",\"ppinternet\"\r\n");
	_delay_ms(15000);
	
	// Connect to bearer x2
	USART_send_str("AT+SAPBR=1,1\r\n");
	_delay_ms(10000);
	USART_send_str("AT+SAPBR=1,1\r\n");
	_delay_ms(5000);
}

// 
// void compare(){
// 	char rangeLat = 17.3;
// 	unsigned char rangeLong= -76.746;
// 	char data[200];
// 	getLatLong(data);
// 	if ( (data != rangeLat) && (data != rangeLong) ){
// 		PORTC |= (1<<4);
// 		send_text();
// 	}
// 	else
// 	PORTC &= 0x00;	
// }
// 
// 
// void send_text (){
// 		
// 		USART_send_str("AT+CMGF=1\r\n");
// 		_delay_ms(100);
// 		USART_send_str("AT+CMGS=\"8763709288\"\r\n");
// 		_delay_ms(100);
// 		USART_send_str("Your Dog has gone out of Range\r\n");
// 		USART_send_str("Click here: http://philliplogan.com:5000");
// 		USART_send_char(26);
// 		
//  }

int main() {
	
	DDRC |= 0xFF;   //setting port c as output
	DDRD |= (1 << 1);
	sei();
	
	USART_init();
	// when mc starts turn on red led after 15seconds turn it off ad turn on a green one to let us know when it starts
	USART_send_str("AT\r\n");
	_delay_ms(200);
		
	USART_send_str("AT\r\n");
	_delay_ms(200);
		
	USART_send_str("AT\r\n");
	_delay_ms(200);
	gpsinit();
   // httpinit();
	
	while(1)
	{
		post();
		_delay_ms(10000);
//		compare();
	}
	return 0;
}

