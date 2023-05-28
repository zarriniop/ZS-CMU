/*
 * rs485.c
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */
#include "main.h"
#include <chrono>
#include "tools.h"
#include "gpio.h"

#define		RS485_BUFFER_SIZE			8192
#define		SOCKET_TIMEOUT_VAL			50000
#define		RESPONSE_TIMEOUT_VAL		5


#define		LED_R485_SHOT			"echo 1 > /sys/devices/platform/leds/leds/LED:R485/shot"
#define		LED_G485_SHOT			"echo 1 > /sys/devices/platform/leds/leds/LED:G485/shot"
#define		LED_STATUS_GET_OFFDLY	"cat /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY1		"echo 500 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY2		"echo 1000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY3		"echo 2000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY4		"echo 4000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"

extern CLIENTSOCKET ClientSocket;
extern SERVERSOCKET ServerSocket;
extern GPIO			gpio;
extern SerialPort Dnp_Buffer;



using namespace std;

void rs485_AES_Decrypt(AES *aes,uint8_t *Buffer,uint16_t count)
{
	int packet=count/PACKET_SIZE;

	for(int p=0;p<packet;p++)
			AES_ECB_decrypt(&aes->ctx,Buffer+(p*16));


}

void rs485_AES_Encrypt(SerialPort *PORT)
{
	int packet=0;

	//Add Empty data to the end of buffer to create 16 byte data frame
	packet=PORT->RX_Count%PACKET_SIZE;
	if(packet>0)
	{
		memset(PORT->RX_Buffer+PORT->RX_Count ,PACKET_EMPTY_CHAR,PACKET_SIZE-packet);
		PORT->RX_Count+=PACKET_SIZE-packet;
	}

	//Encrypt data frames
	packet=PORT->RX_Count/PACKET_SIZE;
	for(int p=0;p<packet;p++)
		AES_ECB_encrypt(&PORT->Aes.ctx,PORT->RX_Buffer+(p*16));

}

//Close RS485 serial port
result rs485_SerialPort_Close(SerialPort *SerialPORT)
{
	close(SerialPORT->Port_fd);
	return OK;
}

//Open RS485 Serial port
result rs485_SerialPort_Open(SerialPort *SerialPORT)
{

	SerialPORT->Port_fd = open(SerialPORT->Parameters.File, O_RDWR);
	struct termios tty;

	if (SerialPORT->Port_fd < 0) {
		report("ss","RS485 --> Error file open:", strerror(errno));
	}

	if(SerialPORT->Parameters.Parity==PARITY_NONE)
		tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	else
		tty.c_cflag |= PARENB;  // Set parity bit, enabling parity

	if(SerialPORT->Parameters.StopBits==STOPBITS_ONE)
		tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	else
		tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication

	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size

	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed


	tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, SerialPORT->Parameters.BaudRate);
	cfsetospeed(&tty, SerialPORT->Parameters.BaudRate);

	if (tcsetattr(SerialPORT->Port_fd , TCSANOW, &tty) != 0) {
		report("ss","RS485 --> PORT_OPEN_ERROR= ", strerror(errno));
	  return ERROR;
	}
	else
	{
		report("s","RS485 --> PORT_OPEN_SUCCESSFULL!");
		return OK;
	}

}

//Send data through RS485
inline void rs485_SerialPort_Send(SerialPort *SerialPORT)
{
	if(SerialPORT->Aes.Enable==true) rs485_AES_Decrypt(&SerialPORT->Aes,SerialPORT->TX_Buffer,SerialPORT->TX_Count);

	//Change dir pin to high level
	*(volatile uint32_t *)(gpio.BASE_ADDR + 0x630)=(1 <<GPIO_OUT_DIR);
	//usleep(1);
	write(SerialPORT->Port_fd,SerialPORT->TX_Buffer,SerialPORT->TX_Count);
	tcdrain(SerialPORT->Port_fd);

	//Change dir pin to low level
	*(volatile uint32_t *)(gpio.BASE_ADDR + 0x640)=(1 <<GPIO_OUT_DIR);

	memcpy(Dnp_Buffer.RX_Buffer,SerialPORT->TX_Buffer,SerialPORT->TX_Count);
	Dnp_Buffer.RX_Count=SerialPORT->TX_Count;

	SerialPORT->TX_Count=0;

	//turn RS485 for a little time
	exec2(LED_R485_SHOT);

}


//Try to open a socket connection to the servers
inline void rs485_TCPClient_SocketOpen(uint8_t n)
{
	if(ClientSocket.Status[n].Opened==true)	//===============( Close Connection )======================
	{
		ClientSocket.get_close(n);
		ClientSocket.Status[n].Opened=false;

		report("sds","RS485 --> TCP-Client:Socket",n,"--> CONNECTION_CLOSE!!");

											//===============( Create Socket )=========================
		if(ClientSocket.create(n)==OK)
		{
			report("sds","RS485 --> TCP-Client:Socket",n,"--> CREATE_OK!");
			report("sds","RS485 --> TCP-Client:Socket",n,"--> Connecting .....!");
		}

	}
	 	 	 	 	 	 	 	 	 	    //===============( Connect to Server )=========================
	if(ClientSocket.get_connect(n)==OK)
	{
		ClientSocket.Status[n].Connected=true;
		ClientSocket.Status[n].Opened=true;

		report("sds","RS485 --> TCP-Client:Socket",n,"--> Connected !");
	}
	else
	{
		ClientSocket.Status[n].Connected=false;
	}

}


//Check the status of Sockets connection to servers
void *rs485_TCPClient_ConnectionCheck(void *arg)
{
	uint8_t i=0;
	uint8_t r1=0,r2=0;

	report("s","RS485 --> TCP_CLIENT_CONNECTION_CHECK_Thread Started !");

	while(1)
	{
		for(i=0;i<MAX_SERVER_NUMBER;i++)
		{
			if(ClientSocket.Status[i].Connected==false && ClientSocket.Status[i].Valid==true)
			{
				rs485_TCPClient_SocketOpen(i);
			}

			ClientSocket.Error=0;
			ClientSocket.ErrorLen=sizeof(ClientSocket.Error);
			getsockopt (ClientSocket.Server_fd[i], SOL_SOCKET, SO_ERROR, &ClientSocket.Error, &ClientSocket.ErrorLen);

			if(ClientSocket.Error!=0) report("sdsd","RS485 --> TCP-Client:Socket",i,"--> Connection Error =",ClientSocket.Error);
		}

		r1=0;
		for(i=0;i<MAX_SERVER_NUMBER;i++)
			if(ClientSocket.Status[i].Connected==true) r1=1;

		if(r1==1 && r2==0)					//When there is a connection to the servers
		{
			exec2(LED_STATUS_OFFDLY3);		//Blink status led slow
			r2=1;
		}

		else if(r1==0 && r2==1)				//When there is not any connection to the servers
		{
			exec2(LED_STATUS_OFFDLY1);		//Blink status led fast
			r2=0;
		}


		sleep(2);
	}
}

//Check the status of Sockets connection from clients
void *rs485_TCPServer_ConnectionCheck(void *arg)
{
	fd_set readfds;
	int activity=0;
	uint8_t r1=0,r2=0;

	FD_ZERO(&readfds);
	FD_SET(ServerSocket.Server_fd, &readfds);

	report("s","RS485 --> TCP_SERVER_CONNECTION_CHECK_Thread Started !");

	while(1)
	{
		r1=0;
		for(int i=0;i<MAX_CLIENT_NUMBER;i++)
		{
			if(ServerSocket.Client_fd[i]>0) r1=1;
		}

		if(r1==1)	exec2(LED_STATUS_OFFDLY3);		//When there is a connection from the clients -- Blink status led slow
		else        exec2(LED_STATUS_OFFDLY1);		//When there is not any connection from the clients -- Blink status led fast



		 //wait for an activity on one of the sockets , timeout is NULL ,
		activity = select( ServerSocket.Server_fd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			report("s","RS485 --> TCP-Server --> Connection Check select error !");
		}

		//Check for New Connection To this Server
		if (FD_ISSET(ServerSocket.Server_fd, &readfds))
		{
			report("s","RS485 --> TCP-Server --> New Connection !!");
			ServerSocket.accept_connection();
		}



		sleep(2);
	}

}

//Check the status of Sockets connection from client on lan socket
void *rs485_TCPClient_Receive(void *arg)
{
	SerialPort &SerialPORT = *((SerialPort *)arg);
	uint16_t num_bytes=0;
	uint8_t Buffer[RS485_BUFFER_SIZE],i;

	fd_set readfds;
	char activity=0,tmp_fd=0;
	timeval	tim;

	tim.tv_usec=SOCKET_TIMEOUT_VAL;
	tim.tv_sec=0;

	ClientSocket.ReceiveFlag=-1;

	report("s","RS485 --> TCP_CLIENT_RX_Thread Started !");

	while(1)
	{
		//wait for an activity on one of the sockets , timeout is NULL
		tmp_fd=0;
		FD_ZERO(&readfds);
		for(i=0;i<MAX_SERVER_NUMBER;i++)
		{
			if(ClientSocket.Status[i].Connected==true)
			{
				FD_SET(ClientSocket.Server_fd[i], &readfds);
				if(ClientSocket.Server_fd[i]>tmp_fd) tmp_fd=ClientSocket.Server_fd[i];
			}
		}

		activity = select( tmp_fd + 1 , &readfds , NULL , NULL , &tim);

		if(activity==0)
		{
			 if(ClientSocket.ReceiveFlag >-1)
			 {
				 if(SerialPORT.TX_Count>0)
				 {
					 report("sdsd","RS485 -->  TCP-Client:Socket",ClientSocket.ReceiveFlag,"  ->>- PORT --  Count=",SerialPORT.TX_Count);
					 rs485_SerialPort_Send(&SerialPORT);
				 }

				 if( diff_time_sec( &ServerSocket.Flag_Time ) > RESPONSE_TIMEOUT_VAL) ClientSocket.ReceiveFlag=-1;
			 }

		}
		else if( activity>0)
		{
			for(i=0 ; i < MAX_SERVER_NUMBER ; i++)
			{
				if (FD_ISSET( ClientSocket.Server_fd[i] , &readfds))
				{
					num_bytes = read( ClientSocket.Server_fd[i] , Buffer, RS485_BUFFER_SIZE);

					if(num_bytes == 0)
					{
						report("sds","RS485 --> TCP-Client:Socket",i,"--> Connection Error !");
						ClientSocket.Status[i].Connected=false;
					}
					else if(num_bytes>0)
					{
						if(ClientSocket.ReceiveFlag==-1 || ClientSocket.ReceiveFlag==i)
						{
							ClientSocket.ReceiveFlag=i;
							gettimeofday( &ClientSocket.Flag_Time,NULL );
							memcpy(SerialPORT.TX_Buffer+SerialPORT.TX_Count,Buffer,num_bytes);
							SerialPORT.TX_Count+=num_bytes;
						}
					}
					else
					{

						report("sdsd","RS485 --> TCP-Client:Socket",i," --> ReadFD error =",errno);
						ClientSocket.Status[i].Connected=false;
					}
				}
			}
		}
		else
		{
			//printf("Activity=%d\n",activity);
		}

	}

}

//Receive data from socket connection on lan
void *rs485_TCPServer_Receive(void *arg)
{
	SerialPort &SerialPORT = *((SerialPort *)arg);
	fd_set readfds;
	int tmp_fd=0,activity=0,num_bytes=0;
	uint8_t Buffer[RS485_BUFFER_SIZE],i=0;

	struct sockaddr_in 	Serv_addr;
	int 				AddLen;
	timeval	tim;

	tim.tv_usec=SOCKET_TIMEOUT_VAL;
	tim.tv_sec=0;

	ServerSocket.ReceiveFlag=-1;

	report("s","RS485 --> TCP_SERVER_RX_Thread Started !");

	while(1)
	{
		FD_ZERO(&readfds);
		tmp_fd=ServerSocket.Server_fd;
		for (i = 0 ; i < MAX_CLIENT_NUMBER ; i++)
		{
			if(ServerSocket.Client_fd[i]>0)
			{
				 FD_SET( ServerSocket.Client_fd[i] , &readfds);
				 if(ServerSocket.Client_fd[i]>tmp_fd) tmp_fd=ServerSocket.Client_fd[i];
			}
		}

		activity = select( tmp_fd + 1 , &readfds , NULL , NULL , &tim);



		if(activity==0)
		{
			//printf("activity=%d\n",activity);
			 if(ServerSocket.ReceiveFlag >-1)
			 {
				 if(SerialPORT.TX_Count>0)
				 {
					 report("sdsd","RS485 -->  TCP-Server:Socket",ServerSocket.ReceiveFlag,"  ->>- PORT --  Count=",SerialPORT.TX_Count);
				 	 rs485_SerialPort_Send(&SerialPORT);
				 }


				 if( diff_time_sec( &ServerSocket.Flag_Time )> RESPONSE_TIMEOUT_VAL) ServerSocket.ReceiveFlag=-1;
			 }

		}

		else if(activity>0)
		{
			//printf("activity=%d\n",activity);
			for(i=0 ; i < MAX_CLIENT_NUMBER ; i++)
			{
				if (FD_ISSET( ServerSocket.Client_fd[i] , &readfds))
				{
					num_bytes = read( ServerSocket.Client_fd[i] , Buffer, RS485_BUFFER_SIZE);

					if(num_bytes == 0)
					{
						getpeername(ServerSocket.Client_fd[i] , (struct sockaddr*)&Serv_addr.sin_addr,(socklen_t*)&AddLen);
						report("sssd","RS485 --> TCP-Server -> Host disconnected: ip=",inet_ntoa(Serv_addr.sin_addr)," , port=",ntohs(Serv_addr.sin_port));
						close(ServerSocket.Client_fd[i]);
						ServerSocket.Client_fd[i]=0;
						uint8_t r=0;
						for(int i=0;i<MAX_CLIENT_NUMBER;i++)
						{
							if(ServerSocket.Client_fd[i]>0) r=1;
						}

						if(r==0 ) exec2(LED_STATUS_OFFDLY1);
					}
					else
					{
						if(ServerSocket.ReceiveFlag==-1 || ServerSocket.ReceiveFlag==i)
						{
							ServerSocket.ReceiveFlag=i;
							gettimeofday( &ServerSocket.Flag_Time ,NULL );
							memcpy(SerialPORT.TX_Buffer+SerialPORT.TX_Count,Buffer,num_bytes);
							SerialPORT.TX_Count+=num_bytes;
						}

					}
				}
			}
		}
	}
}
//Send data to server socket connection
inline void rs485_TCPServer_Send(SerialPort *SerialPORT)
{
	if(ServerSocket.ReceiveFlag==-1)
	{
		int res=0;
		for(int i=0 ; i<MAX_CLIENT_NUMBER ; i++)
		{
			if(ServerSocket.Client_fd[i]>0)
			{
				if(SerialPORT->Aes.Enable==true) rs485_AES_Encrypt(SerialPORT);

				if(Dnp_Buffer.TX_Count>0)
				{
					send(ServerSocket.Client_fd[i],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
					report("sdsd","DNP3.0 -->  DNP ->>- TCP-Server:Socket",i," --  Count=",Dnp_Buffer.TX_Count);
				}
				else
				{
					send(ServerSocket.Client_fd[i],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
					report("sdsd","RS485 -->  PORT ->>- TCP-Server:Socket",i," --  Count=",SerialPORT->RX_Count);
				}
				break;
			}
		}
	}
	else
	{
		if(SerialPORT->Aes.Enable==true) rs485_AES_Encrypt(SerialPORT);
		if(Dnp_Buffer.TX_Count>0)
		{
			send(ServerSocket.Client_fd[uint8_t(ServerSocket.ReceiveFlag)],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
			report("sdsd","DNP3.0 -->  DNP ->>- TCP-Server:Socket",ServerSocket.ReceiveFlag," --  Count=",Dnp_Buffer.TX_Count);
		}
		else
		{
			send(ServerSocket.Client_fd[uint8_t(ServerSocket.ReceiveFlag)],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
			report("sdsd","RS485 -->  PORT ->>- TCP-Server:Socket",ServerSocket.ReceiveFlag," --  Count=",SerialPORT->RX_Count);
		}
	}

	SerialPORT->RX_Count=0;
	Dnp_Buffer.TX_Count=0;

}

//Send data to  client socket connection
inline void rs485_TCPClient_Send(SerialPort *SerialPORT)
{
	if(ClientSocket.ReceiveFlag==-1)
	{
		int res=0;
		for(int i=0 ; i<MAX_SERVER_NUMBER ; i++)
		{
			if(ClientSocket.Server_fd[i]>0 && ClientSocket.Status[i].Connected==true)
			{
				if(SerialPORT->Aes.Enable==true) rs485_AES_Encrypt(SerialPORT);

				if(Dnp_Buffer.TX_Count>0)
				{
					send(ClientSocket.Server_fd[i],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
					report("sdsd","DNP3.0 -->  DNP ->>- TCP-Client:Socket",i," --  Count=",Dnp_Buffer.TX_Count);
				}
				else
				{
					send(ClientSocket.Server_fd[i],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
					report("sdsd","RS485 -->  PORT ->>- TCP-Client:Socket",i," --  Count=",SerialPORT->RX_Count);
				}
				break;
			}
		}
	}
	else
	{
		if(SerialPORT->Aes.Enable==true) rs485_AES_Encrypt(SerialPORT);

		 if(Dnp_Buffer.TX_Count>0)
		{
			send(ClientSocket.Server_fd[uint8_t(ClientSocket.ReceiveFlag)],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
			report("sdsd","DNP3.0 -->  DNP ->>- TCP-Client:Socket",uint8_t(ClientSocket.ReceiveFlag)," --  Count=",Dnp_Buffer.TX_Count);
		}
		else
		{
			send(ClientSocket.Server_fd[uint8_t(ClientSocket.ReceiveFlag)],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
			report("sdsd","RS485 -->  PORT ->>- TCP-Client:Socket",ClientSocket.ReceiveFlag," --  Count=",SerialPORT->RX_Count);
		}
	}

	Dnp_Buffer.TX_Count=0;
	SerialPORT->RX_Count=0;
}



void *rs485_UDPServer_Receive(void *arg)
{
	SerialPort &SerialPORT = *((SerialPort *)arg);
	int activity=0,num_bytes=0;
	uint8_t Buffer[RS485_BUFFER_SIZE],i=0;

	ClientSocket.ReceiveFlag=-1;

	report("s","RS485 --> UDP_SERVER_RX_Thread Started !");

	while(1)
	{
		activity=-1;
		for(i=0;i<MAX_SERVER_NUMBER;i++)
		{
			num_bytes= recvfrom(ClientSocket.Server_fd[i], Buffer, 1024, MSG_DONTWAIT,
					( struct sockaddr *) &ClientSocket.Serv_addr[i],&ClientSocket.ErrorLen);
			if(num_bytes>0)
			{
				activity=i;
				if(ClientSocket.ReceiveFlag==i || ClientSocket.ReceiveFlag==-1)
				{
					ClientSocket.ReceiveFlag=i;
					gettimeofday( &ClientSocket.Flag_Time,NULL );
					memcpy(SerialPORT.TX_Buffer+SerialPORT.TX_Count,Buffer,num_bytes);
					SerialPORT.TX_Count+=num_bytes;
				}
			}
		}


		if(activity==-1)
		{
			 if(ClientSocket.ReceiveFlag >-1)
			 {
				 if(SerialPORT.TX_Count>0)
				 {
					 report("sdsd","RS485 -->  UDP-Server:Socket",ClientSocket.ReceiveFlag," ->>- PORT --  Count=",SerialPORT.TX_Count);
					 rs485_SerialPort_Send(&SerialPORT);
				 }

				 if( diff_time_sec( &ClientSocket.Flag_Time) > RESPONSE_TIMEOUT_VAL) ClientSocket.ReceiveFlag=-1;
			 }

		}

		sleep(1);
	}

}


inline void rs485_UDPServer_Send(SerialPort *SerialPORT)
{
	if(ServerSocket.ReceiveFlag==-1)
	{
		int res=0;
		for(int i=0 ; i<MAX_CLIENT_NUMBER ; i++)
		{
			if(ServerSocket.Client_fd[i]>0)
			{
				res=send(ServerSocket.Client_fd[i],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
				if(res==SerialPORT->RX_Count)
				{
					report("sdsd","RS485 -->   PORT->>- UDP-Client:Socket",i," --  Count=",SerialPORT->RX_Count);
					break;
				}

			}

		}
		SerialPORT->RX_Count=0;
	}
	else
	{
		send(ServerSocket.Client_fd[uint8_t(ServerSocket.ReceiveFlag)],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
		 report("sdsd","RS485 -->    PORT ->>- UDP-Client:Socket",ServerSocket.ReceiveFlag," --  Count=",SerialPORT->RX_Count);
		 SerialPORT->RX_Count=0;
	}
}

void *rs485_UDPClient_Receive(void *arg)
{
	SerialPort &SerialPORT = *((SerialPort *)arg);
	int activity=0,num_bytes=0;
	uint8_t Buffer[RS485_BUFFER_SIZE],i=0;

	ClientSocket.ReceiveFlag=-1;

	report("s","RS485 --> UDP_CLIENT_RX_Thread Started !");

	while(1)
	{
		activity=-1;
		for(i=0;i<MAX_SERVER_NUMBER;i++)
		{
			num_bytes= recvfrom(ClientSocket.Server_fd[i], Buffer, 1024, MSG_DONTWAIT,
					( struct sockaddr *) &ClientSocket.Serv_addr[i],&ClientSocket.ErrorLen);
			if(num_bytes>0)
			{
				activity=i;
				if(ClientSocket.ReceiveFlag==i || ClientSocket.ReceiveFlag==-1)
				{
					ClientSocket.ReceiveFlag=i;
					gettimeofday( &ClientSocket.Flag_Time,NULL );
					memcpy(SerialPORT.TX_Buffer+SerialPORT.TX_Count,Buffer,num_bytes);
					SerialPORT.TX_Count+=num_bytes;
				}
			}
		}


		if(activity==-1)
		{
			 if(ClientSocket.ReceiveFlag >-1)
			 {
				 if(SerialPORT.TX_Count>0)
				 {
					 report("sdsd","RS485 -->  UDP-Server:Socket",ClientSocket.ReceiveFlag,"  ->>- PORT --  Count=",SerialPORT.TX_Count);
					 rs485_SerialPort_Send(&SerialPORT);
				 }

				 if( diff_time_sec( &ClientSocket.Flag_Time) > RESPONSE_TIMEOUT_VAL) ClientSocket.ReceiveFlag=-1;
			 }

		}

		sleep(1);
	}

}


inline void rs485_UDPClient_Send(SerialPort *SerialPORT)
{
	if(ClientSocket.ReceiveFlag==-1)
	{
		for(int i=0 ; i<MAX_SERVER_NUMBER ; i++)
		{
			if(ClientSocket.Server_fd[i]>0 )
			{
				//res=write(ClientSocket.Server_fd[i],SerialPORT->RX_Buffer,SerialPORT->RX_Count);
				sendto(ClientSocket.Server_fd[i], SerialPORT->RX_Buffer, SerialPORT->RX_Count, MSG_CONFIRM
						, (const struct sockaddr *) &ClientSocket.Serv_addr[i],sizeof(ClientSocket.Serv_addr[i]));
				report("sdsd","RS485 -->    PORT -->> UDP-Server:Socket",ClientSocket.ReceiveFlag," --  Count=",SerialPORT->RX_Count);

			}

		}
		SerialPORT->RX_Count=0;
	}
	else
	{
		//send(ClientSocket.Server_fd[],SerialPORT->RX_Buffer,SerialPORT->RX_Count,0);
		sendto(ClientSocket.Server_fd[uint8_t(ClientSocket.ReceiveFlag)], SerialPORT->RX_Buffer, SerialPORT->RX_Count, MSG_CONFIRM
			, (const struct sockaddr *) &ClientSocket.Serv_addr[uint8_t(ClientSocket.ReceiveFlag)],sizeof(ClientSocket.Serv_addr[uint8_t(ClientSocket.ReceiveFlag)]));

		 report("sdsd","RS485 -->    PORT -->> UDP-Server:Socket",ClientSocket.ReceiveFlag," --  Count=",SerialPORT->RX_Count);
		 SerialPORT->RX_Count=0;
	}
}



//Receive Data from rs485 serial port
void *rs485_SerialPort_Receive_thread(void *arg)
{
	char Buffer[RS485_BUFFER_SIZE];
	SerialPort &SerialPORT = *((SerialPort *)arg);
	uint16_t num_bytes;
	struct timeval  tim;

	report("s","RS485 --> PORT_RX_Thread Started !");

	SerialPORT.RX_Count=0;

	while(1)
	{
		num_bytes = read(SerialPORT.Port_fd, Buffer,1024);

		if(num_bytes>0)
		{
			gettimeofday (&tim, NULL);
			if(SerialPORT.RX_Count+num_bytes < RS485_BUFFER_SIZE)
			{
				memcpy(SerialPORT.RX_Buffer+SerialPORT.RX_Count,Buffer,num_bytes);
				SerialPORT.RX_Count+=num_bytes;
				num_bytes=0;
			}
			else
			{
				//==============( Send On Buffer Full )=================
				if(ServerSocket.Server_fd!=0)
				{
					if(ServerSocket.Parameters.Type==TCP) rs485_TCPServer_Send(&SerialPORT);
					else rs485_UDPServer_Send(&SerialPORT);
				}
				else
				{
					if(ClientSocket.Parameters->Type==TCP ) rs485_TCPClient_Send(&SerialPORT);
					else rs485_UDPClient_Send(&SerialPORT);
				}
				exec2(LED_G485_SHOT);
				memcpy(SerialPORT.RX_Buffer+SerialPORT.RX_Count,Buffer,num_bytes);
				SerialPORT.RX_Count+=num_bytes;
				num_bytes=0;

			}
		}
		else
		{
			if(SerialPORT.RX_Count>0)
			{
				//==============( Send On Timeout )=================
				if( diff_time_us(&tim) > SerialPORT.Timeout )
				{
					if(ServerSocket.Server_fd!=0)
					{
						if(ServerSocket.Parameters.Type==TCP) rs485_TCPServer_Send(&SerialPORT);
						else rs485_UDPServer_Send(&SerialPORT);
					}
					else
					{
						if(ClientSocket.Parameters->Type==TCP ) rs485_TCPClient_Send(&SerialPORT);
						else rs485_UDPClient_Send(&SerialPORT);
					}

					exec2(LED_G485_SHOT);
				}
			}

			//Send data from DNP to internet
			if(Dnp_Buffer.TX_Count>0)
			{
				if(ServerSocket.Server_fd!=0)
				{
					if(ServerSocket.Parameters.Type==TCP) rs485_TCPServer_Send(&SerialPORT);
				}
				else
				{
					if(ClientSocket.Parameters->Type==TCP ) rs485_TCPClient_Send(&SerialPORT);
				}
				report("s","RS485 --> After DNP packet send!");
			}
		}
		usleep(1000);
	}
	return NULL;
}











