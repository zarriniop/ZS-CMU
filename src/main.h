/*
 * main.h
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <string>
#include <sstream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "aes.hpp"
#include "tools.h"

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>  /* for sockaddr_in */
#include <termios.h>
#include <errno.h>
#include <pthread.h>
#include <chrono>
#include <poll.h>
#include <stdarg.h>
 #include <sys/mman.h>

#include "Alias_Data.h"
#include "Software_Parameter.h"



using namespace std;


#define		DEBUG_MODE					0

#define 	PACKET_EMPTY_CHAR			0x00
#define		BUFFER_SIZE					8192
#define		PACKET_SIZE					16
#define		MAX_CLIENT_NUMBER			5
#define		MAX_SERVER_NUMBER			3


#undef ra_inl
#undef ra_outl
#define ra_inl(addr)  (*(volatile uint32_t *)(addr))
#define ra_outl(addr, value)  (*(volatile uint32_t *)(addr) = (value))
#define ra_and(addr, value) ra_outl(addr, (ra_inl(addr) & (value)))
#define ra_or(addr, value) ra_outl(addr, (ra_inl(addr) | (value)))


#define GPIO_SYSCTL	0x10000000
#define GPIO2_MODE	0x00000064
#define GPIO1_MODE	0x00000060



#define		GPIO_BASE			480
#define		GPIO_IN1			16
#define		GPIO_IN2			17
#define		GPIO_IN3			18
#define		GPIO_IN4			19


#define		GPIO_LED_G232		4
#define		GPIO_LED_R232		5
#define		GPIO_LED_G485		40
#define		GPIO_LED_R485		41
#define		GPIO_LED_SKTG		1
#define		GPIO_LED_SKTR		0

#define		GPIO_LED_SIG		36
#define		GPIO_LED_NET		37
#define		GPIO_LED_STATUS		39
#define		GPIO_OUT_RELAY1		2
#define		GPIO_OUT_RELAY2		3
#define		GPIO_OUT_GSMRST		14
#define		GPIO_OUT_GSMPWR		15
#define		GPIO_OUT_DIR		11


#define 	GPIO_REG		0x10000600
#define 	GPIO_DIR_0		0X00000000
#define 	GPIO_DIR_1		0X00000604
#define 	GPIO_DIR_2		0X00000608
#define 	GPIO_DATA_0		0X00000620
#define 	GPIO_DATA_1		0X00000624
#define	 	GPIO_DATA_2		0X00000628
#define 	GPIO_POL_0		0X00000610
#define 	GPIO_POL_1		0X00000614
#define 	GPIO_POL_2		0X00000618

#define 	GPIO_DSET_0		0X00000630
#define 	GPIO_DSET_1		0X00000634
#define 	GPIO_DSET_2		0X00000638
#define 	GPIO_DCLR_0		0X00000640
#define 	GPIO_DCLR_1		0X00000644
#define 	GPIO_DCLR_2		0X00000648


#define		UCI_GET_MODE			"uci get communication.datacenter.mode"
#define		UCI_GET_PROTOCOL		"uci get communication.datacenter.protocol"
#define		UCI_GET_ADDRESS1		"uci get communication.datacenter.server1address"
#define		UCI_GET_ADDRESS2		"uci get communication.datacenter.server2address"
#define		UCI_GET_ADDRESS3		"uci get communication.datacenter.server3address"
#define		UCI_GET_PORT1			"uci get communication.datacenter.server1port"
#define		UCI_GET_PORT2			"uci get communication.datacenter.server2port"
#define		UCI_GET_PORT3			"uci get communication.datacenter.server3port"
#define		UCI_GET_LISTENPORT		"uci get communication.datacenter.listenport"
#define		UCI_GET_KEEPALIVE		"uci get communication.datacenter.keepalive"
#define		UCI_GET_AES				"uci get communication.aes128.enable"
#define		UCI_GET_AESKEY			"uci get communication.aes128.key"



typedef enum {
	OK			=0,
	ERROR		=-1,
	NOT_ALLOW	=-2

}result;


typedef enum {
	TCP		=2,
	UDP		=1
}SocketType;


typedef enum {
	SERVER		=1,
	CLIENT		=2
}SocketMode;


typedef enum {
	BAUD0			=B0,
	BAUD50 			=B50,
	BAUD75 			=B75,
	BAUD110 		=B110,
	BAUD134 		=B134,
	BAUD150 		=B150,
	BAUD200 		=B200,
	BAUD300 		=B300,
	BAUD600 		=B600,
	BAUD1200 		=B1200,
	BAUD1800	 	=B1800,
	BAUD2400 		=B2400,
	BAUD4800 		=B4800,
	BAUD9600 		=B9600,
	BAUD19200 		=B19200,
	BAUD38400 		=B38400,
	BAUD57600 		=B57600,
	BAUD115200 		=B115200,
	BAUD230400 		=B230400,
	BAUD460800 		=B460800,

}Baud;


typedef enum {
	Bits5		=CS5,
	Bits6		=CS6,
	Bits7		=CS7,
	Bits8		=CS8,
}Bits;



typedef enum {
	PARITY_NONE			=1,
	PARITY_YES			=2,
}Pari;


typedef enum {
	STOPBITS_ONE		=1,
	STOPBITS_TWO		=2,
}StpBits;


typedef enum {
	AES_128			=0,
	AES_192			=1,
	AES_256			=2,
}AESType;


typedef enum {
	AES_ECB			=0,
	AES_CBC			=1,
	AES_CTR			=2,
}AESMode;



typedef struct {
	bool		Enable;
	uint8_t		Key[16];
	AESType		Type;
	AESMode		Mode;
	uint16_t	timeout;
	AES_ctx 	ctx;
}AES;


typedef struct {

	uint8_t		*RX_Buffer;
	uint8_t		*TX_Buffer;
	uint16_t	RX_Count;
	uint16_t	TX_Count;

}Link;


typedef struct {
	char		*File;
	Baud		BaudRate;
	Bits		bitsPbyte;
	Pari		Parity;
	StpBits		StopBits;
}PortParameters;


typedef struct {
	int				Port_fd;
	PortParameters	Parameters;
	uint16_t		TX_Count;
	uint16_t		RX_Count;
	uint8_t 		TX_Buffer[BUFFER_SIZE];
	uint8_t 		RX_Buffer[BUFFER_SIZE];
	AES				Aes;
	long			Timeout;
}SerialPort;




typedef struct {
	string		IP;
	uint16_t	PORT;
	SocketType	Type;
}SocktParameters;


typedef struct {
	bool		Connected;
	bool		Opened;
	bool		Valid;
}SocktStatus;


typedef struct {

	int 		RSRP;
	int 		RSRQ;
	int 		RSSI;
	int 		SINR;

}ModemSignal;

typedef struct {

	bool			Connected;
	ModemSignal		signal;

}ModemStatus;


class CLIENTSOCKET{
	public:
		SocktParameters		Parameters[MAX_SERVER_NUMBER];
		SocktStatus			Status[MAX_SERVER_NUMBER];
		int					Server_fd[MAX_SERVER_NUMBER];
		char 				ReceiveFlag;
		struct timeval		Flag_Time;
		struct sockaddr_in 	Serv_addr[MAX_SERVER_NUMBER];
		int 				Error;
		socklen_t 			ErrorLen;
		uint16_t			keepalive;

		result	create(uint8_t n)				//===============( Create client )======================
		{

			if ((Server_fd[n] = socket(AF_INET, Parameters[n].Type, 0)) < 0)
			{
				report("sds","SOCKET --> Client Socket",n," --> Create error !");
				return ERROR;
			}

			Serv_addr[n].sin_family = AF_INET;
			Serv_addr[n].sin_port = htons(Parameters[n].PORT);
			if(inet_pton(AF_INET, Parameters[n].IP.c_str(), & Serv_addr[n].sin_addr)<=0)
			{

				report("sds","SOCKET --> Client Socket",n," --> Invalid address !");

				Status[n].Valid=false;
				return ERROR;
			}
			else Status[n].Valid=true;
			return OK;
		}

		result	get_connect(uint8_t n)
		{
			struct timeval tv;
			tv.tv_sec  = 2;  /* Set 3 second timeout */
			tv.tv_usec = 0;
			int flags=1;
			int res=0;


			//res = fcntl(Server_fd[n], F_GETFL, 0);
			//fcntl(Server_fd[n], F_SETFL, flags|O_NONBLOCK);


			if(Parameters->Type==TCP)
			{
				res=setsockopt(Server_fd[n], SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
				if(res<0) report("sds","SOCKET --> Client Socket",n," Set Timeout Error !");


				flags=60;
				res=setsockopt(Server_fd[n], IPPROTO_TCP, TCP_KEEPIDLE, & flags, sizeof(flags));
				if(res<0) report("sds","SOCKET --> Client Socket",n," --> Set keep idle Error !");

				flags=keepalive;
				res=setsockopt(Server_fd[n], IPPROTO_TCP, TCP_KEEPCNT, & flags, sizeof(flags));
				if(res<0) report("sds","SOCKET --> Client Socket",n," --> Set keepalive counter Error !");


				flags=10;
				res=setsockopt(Server_fd[n], IPPROTO_TCP, TCP_KEEPINTVL, &flags, sizeof(flags));
				if(res<0) report("sds","SOCKET --> Client Socket",n," --> Set keepalive interval Error !");


				flags=1;
				res=setsockopt(Server_fd[n], SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
				if(res<0) report("sds","SOCKET --> Client Socket",n,"--> Set keepalive Error !");
			}

			res=connect(Server_fd[n], (struct sockaddr *)&Serv_addr[n], sizeof(Serv_addr[n]));
			if ( res< 0)
			{
				//if (errno != EINPROGRESS && errno != EWOULDBLOCK)
				//printf("\nSocket Connection Failed=%d \n",errno);
				return ERROR;
			}
			return OK;



		}
		result	get_close(uint8_t n)
		{
			printf("before close\n");
			int sh=shutdown(Server_fd[n], SHUT_RDWR);
			int cl=close(Server_fd[n]);

			//printf("s:%d  shut:%d  close:%d  \n",n,sh,cl);
			Server_fd[n]=0;
			Status[n].Connected=false;
			Status[n].Opened=false;
			return OK;
		}

};




class SERVERSOCKET{       // The class
	public:
		SocktParameters		Parameters;
		int					Client_fd[MAX_CLIENT_NUMBER];
		int					Server_fd;
		char 				ReceiveFlag;
		struct timeval		Flag_Time;

		result	create() 					//===============( Create Server )======================
		{
			int opt=1;
			struct sockaddr_in 	Serv_addr;

			for (uint8_t i=0;i<MAX_CLIENT_NUMBER;i++)
			{
				Client_fd[i]=0;
			}


			if ((Server_fd = socket(AF_INET, Parameters.Type, 0)) <= 0)
			{
				report("s","SOCKET --> Server Socket --> Create Error !");

				return ERROR;
			}

			if( setsockopt(Server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
			          sizeof(opt)) < 0 )
			{
				report("s","SOCKET --> Server Socket --> Set option error !");
				return ERROR;
			}


			Serv_addr.sin_family = AF_INET;
			Serv_addr.sin_port = htons(Parameters.PORT);
			Serv_addr.sin_addr.s_addr = INADDR_ANY;

			if (bind(Server_fd, (struct sockaddr *)&Serv_addr, sizeof(Serv_addr))<0)
			{
				report("s","SOCKET --> Server Socket --> Bind Failed !");
				return ERROR;
			}

			report("s","SOCKET --> Server Socket --> Create OK !");
			return OK;
		}
		result get_listen()					//===============( Listen as Server )======================
		{
			if (listen(Server_fd, 3) < 0)
			{
				report("s","SOCKET --> Server Socket --> Listen Failed !");
				return ERROR;

			}

			report("s","SOCKET --> Server Socket --> Waiting for Connections...... !");
			return OK;
		}

		result	accept_connection()			//===============( Get connection to server )======================
		{
			struct sockaddr_in 	Serv_addr;
			int 				AddLen=sizeof(Serv_addr);
			int new_socket=0;

			if ((new_socket = accept(Server_fd, (struct sockaddr *)&Serv_addr, (socklen_t*)&AddLen))<0)
			{
				report("s","SOCKET --> Server Socket --> Accept Failed !");
				return ERROR;
			}
			else
			{
				for(int i=0 ; i<MAX_CLIENT_NUMBER ; i++)
				{
					if(Client_fd[i]==0)
					{
						Client_fd[i]=new_socket;
						report("sdsssdsd","SOCKET --> Server Socket --> New connection= socketfd:",new_socket," , ip: ",inet_ntoa(Serv_addr.sin_addr)," , port: ",ntohs(Serv_addr.sin_port)," index=",i);
						break;
					}
				}
			}

			return OK;

		}
		result	get_close()
		{
			shutdown(Server_fd, SHUT_RDWR);
			close(Server_fd);
			Server_fd=0;
			for(int n=0 ; n<MAX_CLIENT_NUMBER ; n++) Client_fd[n]=0;
			return OK;
		}
};


typedef struct {
	CLIENTSOCKET	client;
	SERVERSOCKET	server;
	SocketMode		mode;
	uint16_t		TX_Count;
	uint16_t		RX_Count;
	uint8_t 		TX_Buffer[BUFFER_SIZE];
	uint8_t 		RX_Buffer[BUFFER_SIZE];
	AES				Aes;
}SocketConnection;


typedef struct {
	char		*Type;
	char		*RSSI;
	char		*RSRQ;
	char		*RSRP;
	char		*SNIR;

}SingalInfo;



#endif /* MAIN_H_ */
