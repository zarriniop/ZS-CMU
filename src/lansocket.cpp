/*
 * lansocket.c
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */
#include "main.h"
#include <chrono>
#include "tools.h"

#define		LANSOCKET_BUFFER_SIZE			8192
#define		LANSOCKET_TIMEOUT_VAL			20000
#define		SOCKET_TIMEOUT_VAL				20000
#define		RESPONSE_TIMEOUT_VAL			5



#define		LED_SKTG_SHOT			"echo 1 > /sys/devices/platform/leds/leds/LED:SktG/shot"
#define		LED_SKTR_SHOT			"echo 1 > /sys/devices/platform/leds/leds/LED:SktR/shot"
#define		LED_STATUS_OFFDLY1		"echo 500 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY2		"echo 1000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY3		"echo 2000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY4		"echo 4000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_GET_OFFDLY	"cat /sys/devices/platform/leds/leds/LED:Status/delay_off"

extern CLIENTSOCKET ClientSocket;
extern SERVERSOCKET ServerSocket;
extern SerialPort Dnp_Buffer;




using namespace std;

void lansocket_AES_Decrypt(AES *aes,uint8_t *Buffer,uint16_t count)
{
	int packet=count/PACKET_SIZE;

	for(int p=0;p<packet;p++)
			AES_ECB_decrypt(&aes->ctx,Buffer+(p*16));

}

void lansocket_AES_Encrypt(SocketConnection *lansocket)
{
	int packet=0;

	//Add Empty data to the end of buffer to create 16 byte data frame
	packet=lansocket->RX_Count%PACKET_SIZE;
	if(packet>0)
	{
		memset(lansocket->RX_Buffer+lansocket->RX_Count ,PACKET_EMPTY_CHAR,PACKET_SIZE-packet);
		lansocket->RX_Count+=PACKET_SIZE-packet;
	}

	//Encrypt data frames
	packet=lansocket->RX_Count/PACKET_SIZE;
	for(int p=0;p<packet;p++)
		AES_ECB_encrypt(&lansocket->Aes.ctx,lansocket->RX_Buffer+(p*16));

}


//Send data to socket on lan
inline void lansocket_Lan_Send(SocketConnection *lansocket)
{
	if(lansocket->Aes.Enable==true) lansocket_AES_Decrypt(&lansocket->Aes,lansocket->TX_Buffer,lansocket->TX_Count);

	if(lansocket->mode==SERVER  && lansocket->server.Client_fd[0]>0 ) send(lansocket->server.Client_fd[0],lansocket->TX_Buffer,lansocket->TX_Count,0);
	else if(lansocket->client.Status[0].Connected)	send(lansocket->client.Server_fd[0],lansocket->TX_Buffer,lansocket->TX_Count,0);

	memcpy(Dnp_Buffer.RX_Buffer,lansocket->TX_Buffer,lansocket->TX_Count);
	Dnp_Buffer.RX_Count=lansocket->TX_Count;

	lansocket->TX_Count=0;

	//Turn skt led for a little time
	exec2(LED_SKTR_SHOT);

}

//Try to open a socket connection to the servers
inline void lansocket_TCPClient_SocketOpen(uint8_t n)
{
	if(ClientSocket.Status[n].Opened==true)	//===============( Close Connection )======================
	{
		ClientSocket.get_close(n);
		ClientSocket.Status[n].Opened=false;
		report("sds","LANSOCKET --> TCP-Client:Socket",n,"--> CONNECTION_CLOSE!!");

											//===============( Create Socket )=========================
		if(ClientSocket.create(n)==OK)
		{
			report("sds","LANSOCKET --> TCP-Client:Socket",n,"--> CREATE_OK!");
			report("sds","LANSOCKET --> TCP-Client:Socket",n,"--> Connecting .....!");
		}

	}
	 	 	 	 	 	 	 	 	 	    //===============( Connect to Server )=========================
	if(ClientSocket.get_connect(n)==OK)
	{
		ClientSocket.Status[n].Connected=true;
		ClientSocket.Status[n].Opened=true;

		report("sds","LANSOCKET --> TCP-Client:Socket",n,"--> Connected !");
	}
	else
	{
		ClientSocket.Status[n].Connected=false;

	}

}

//Check the status of Sockets connection to servers
void *lansocket_TCPClient_ConnectionCheck(void *arg)
{
	uint8_t i=0;
	uint8_t r1=0,r2=0;

	report("s","LANSOCKET --> TCP_CLIENT_CONNECTION_CHECK_Thread Started !");

	while(1)
	{
		for(i=0;i<MAX_SERVER_NUMBER;i++)
		{
			if(ClientSocket.Status[i].Connected==false && ClientSocket.Status[i].Valid==true)
			{
				lansocket_TCPClient_SocketOpen(i);
			}

			ClientSocket.Error=0;
			ClientSocket.ErrorLen=sizeof(ClientSocket.Error);
			getsockopt (ClientSocket.Server_fd[i], SOL_SOCKET, SO_ERROR, &ClientSocket.Error, &ClientSocket.ErrorLen);

			if(ClientSocket.Error!=0) report("sdsd","LANSOCKET --> TCP-Client:Socket",i,"--> Connection Error =",ClientSocket.Error);
		}

		r1=0;
		for(i=0;i<MAX_SERVER_NUMBER;i++)
			if(ClientSocket.Status[i].Connected==true) r1=1;

		if(r1==1 && r2==0)  		//When there is a connection to the servers
		{
			exec2(LED_STATUS_OFFDLY3);	//Blink status led slow
			r2=1;
		}

		else if(r1==0 && r2==1)    //When there is not any connection to the servers
		{
			exec2(LED_STATUS_OFFDLY1);	//Blink status led fast
			r2=0;
		}

		sleep(2);

	}
}

//Check the status of Sockets connection from clients
void *lansocket_TCPServer_ConnectionCheck(void *arg)
{
	fd_set readfds;
	int activity=0;

	FD_ZERO(&readfds);
	FD_SET(ServerSocket.Server_fd, &readfds);

	uint8_t r1=0,r2=0;

	report("s","LANSOCKET --> TCP_SERVER_CONNECTION_CHECK_Thread Started !");


	while(1)
	{
		r1=0;
		for(int i=0;i<MAX_CLIENT_NUMBER;i++)
		{
			if(ServerSocket.Client_fd[i]>0) r1=1;
		}

		if(r1==1)  exec2(LED_STATUS_OFFDLY3);	//When there is a connection from the clients -- Blink status led slow
		else 	   exec2(LED_STATUS_OFFDLY1);	//When there is not any connection from the clients -- Blink status led fast


		 //wait for an activity on one of the sockets , timeout is NULL ,
		activity = select( ServerSocket.Server_fd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			report("s","LANSOCKET --> TCP-Server --> Connection Check select error !");
		}

		//Check for New Connection To this Server
		if (FD_ISSET(ServerSocket.Server_fd, &readfds))
		{
			report("s","LANSOCKET --> TCP-Server --> New Connection !!");
			ServerSocket.accept_connection();
		}

		sleep(2);
	}
}

//Check the status of Sockets connection from client on lan socket
void *lansocket_LANServer_ConnectionCheck(void *arg)
{
	SocketConnection &LanSocket = *((SocketConnection *)arg);
	fd_set readfds;
	int activity=0;


	report("s","LANSOCKET --> TCPLAN_SERVER_CONNECTION_CHECK_Thread Started !");

	FD_ZERO(&readfds);
	FD_SET(LanSocket.server.Server_fd, &readfds);

	while(1)
	{
		 //wait for an activity on one of the sockets , timeout is NULL ,
		activity = select( LanSocket.server.Server_fd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			report("s","LANSOCKET --> TCPLAN-Server --> Connection Check select error !");
		}

		//Check for New Connection To this Server
		if (FD_ISSET(LanSocket.server.Server_fd, &readfds))
		{
			report("s","LANSOCKET --> TCPLAN-Server --> New Connection !!");
			LanSocket.server.accept_connection();
		}

		sleep(2);
	}
}


//Check the status of Sockets connection to server on lan socket
inline void lansocket_LANClient_SocketOpen(SocketConnection *lansocket)
{

	if(lansocket->client.Status[0].Opened==true)	//===============( Close Connection )======================
	{
		lansocket->client.get_close(0);
		lansocket->client.Status[0].Opened=false;

		report("s","LANSOCKET --> TCPLAN-Client:Socket --> CONNECTION_CLOSE!!");
											//===============( Create Socket )=========================
		if(lansocket->client.create(0)==OK)
		{
			report("s","LANSOCKET --> TCPLAN-Client:Socket --> CREATE_OK!");
			report("s","LANSOCKET --> TCPLAN-Client:Socket --> Connecting .....!");
		}

	}

	//===============( Connect to Server )=========================
	if(lansocket->client.get_connect(0)==OK)
	{
		lansocket->client.Status[0].Connected=true;
		lansocket->client.Status[0].Opened=true;
		report("s","LANSOCKET --> TCPLAN-Client:Socket --> Connected .....!");
	}
	else
	{
		lansocket->client.Status[0].Connected=false;

	}

}

//Check the status of Sockets connection to server on lan socket
void *lansocket_LANClient_ConnectionCheck(void *arg)
{
	SocketConnection &LanSocket = *((SocketConnection *)arg);

	report("s","LANSOCKET --> TCPLAN_CLIENT_CONNECTION_CHECK_Thread Started !");

	while(1)
	{

		if(LanSocket.client.Status[0].Connected==false && LanSocket.client.Status[0].Valid==true)
		{
			lansocket_LANClient_SocketOpen(&LanSocket);
		}
		LanSocket.client.Error=0;
		LanSocket.client.ErrorLen=sizeof(LanSocket.client.Error);


		getsockopt (LanSocket.client.Server_fd[0], SOL_SOCKET, SO_ERROR, &LanSocket.client.Error, &LanSocket.client.ErrorLen);

		if(LanSocket.client.Error!=0) report("sdsd","LANSOCKET --> TCPLAN-Client:Socket --> Connection Error =",LanSocket.client.Error);

		sleep(2);

	}
}

//Receive data from socket connection on lan
void *lansocket_TCPClient_Receive(void *arg)
{
	SocketConnection &LanSocket = *((SocketConnection *)arg);
	int num_bytes=0;
	uint8_t Buffer[LANSOCKET_BUFFER_SIZE],i;

	fd_set readfds;
	char activity=0,tmp_fd=0;
	timeval	tim;

	tim.tv_usec=SOCKET_TIMEOUT_VAL;
	tim.tv_sec=0;

	ClientSocket.ReceiveFlag=-1;

	report("s","LANSOCKET --> TCP-CLIENT_RX_Thread Started !");

	while(1)
	{
		//add all socket fds to one global fd
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


		//poll for an activity on global fd
		activity = select( tmp_fd + 1 , &readfds , NULL , NULL , &tim);


		if(activity==0)  //data received
		{

			 if(ClientSocket.ReceiveFlag >-1)
			 {
				 if(LanSocket.TX_Count>0)
				 {
					 report("sdsd","LANSOCKET -->  TCP-Client:Socket",ClientSocket.ReceiveFlag,"  ->>- PORT --  Count=",LanSocket.TX_Count);
					 lansocket_Lan_Send(&LanSocket);
				 }

				 if(diff_time_sec( &ClientSocket.Flag_Time) > RESPONSE_TIMEOUT_VAL) ClientSocket.ReceiveFlag=-1;
			 }

		}
		else if( activity>0)    //connection error
		{
			//printf("Activity=%d\n",activity);
			for(i=0 ; i < MAX_SERVER_NUMBER ; i++)
			{
				if (FD_ISSET( ClientSocket.Server_fd[i] , &readfds))
				{
					num_bytes = recv( ClientSocket.Server_fd[i] , Buffer, LANSOCKET_BUFFER_SIZE,NULL);
					if(num_bytes == 0)
					{
						report("sds","LANSOCKET --> TCP-Client:Socket",i,"--> Connection Error !");
						ClientSocket.Status[i].Connected=false;
					}
					else if(num_bytes>0)
					{
						if(ClientSocket.ReceiveFlag==-1 || ClientSocket.ReceiveFlag==i)
						{
							ClientSocket.ReceiveFlag=i;
							gettimeofday( &ClientSocket.Flag_Time,NULL );
							memcpy(LanSocket.TX_Buffer+LanSocket.TX_Count,Buffer,num_bytes);
							LanSocket.TX_Count+=num_bytes;
						}
					}
					else
					{
						report("sdsd","LANSOCKET --> TCP-Client:Socket",i," --> ReadFD error =",errno);
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
void *lansocket_TCPServer_Receive(void *arg)
{
	SocketConnection &LanSocket = *((SocketConnection *)arg);
	fd_set readfds;
	int tmp_fd=0,activity=0,num_bytes=0;
	uint8_t Buffer[LANSOCKET_BUFFER_SIZE],i=0;

	struct sockaddr_in 	Serv_addr;
	int 				AddLen;
	timeval	tim;

	tim.tv_usec=SOCKET_TIMEOUT_VAL;
	tim.tv_sec=0;

	ServerSocket.ReceiveFlag=-1;

	report("s","LANSOCKET --> TCP_SERVER_RX_Thread Started !");

	while(1)
	{
		//add all socket fds to one global fd
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


		//poll for an activity on global fd
		activity = select( tmp_fd + 1 , &readfds , NULL , NULL , &tim);

		if(activity==0)  //data received
		{
			//printf("activity=%d\n",activity);
			 if(ServerSocket.ReceiveFlag >-1)
			 {
				 if(LanSocket.TX_Count>0)
				 {
					 report("sdsd","LANSOCKET -->  TCP-Server:Socket",ServerSocket.ReceiveFlag,"  ->>- LAN --  Count=",LanSocket.TX_Count);
					 lansocket_Lan_Send(&LanSocket);
				 }


				 if(diff_time_sec( &ServerSocket.Flag_Time )> RESPONSE_TIMEOUT_VAL) ServerSocket.ReceiveFlag=-1;
			 }

		}

		else if(activity>0)   //connection error
		{
			//printf("activity=%d\n",activity);
			for(i=0 ; i < MAX_CLIENT_NUMBER ; i++)
			{
				if (FD_ISSET( ServerSocket.Client_fd[i] , &readfds))
				{

					num_bytes = read( ServerSocket.Client_fd[i] , Buffer, LANSOCKET_BUFFER_SIZE);
					if(num_bytes == 0)
					{
						getpeername(ServerSocket.Client_fd[i] , (struct sockaddr*)&Serv_addr.sin_addr,(socklen_t*)&AddLen);
						report("sssd","LANSOCKET --> TCP-Server -> Host disconnected: ip=",inet_ntoa(Serv_addr.sin_addr)," , port=",ntohs(Serv_addr.sin_port));
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
							memcpy(LanSocket.TX_Buffer+LanSocket.TX_Count,Buffer,num_bytes);
							LanSocket.TX_Count+=num_bytes;
						}

					}
				}
			}
		}
	}
}


//Send data to server socket connection
inline void lansocket_TCPServer_Send(SocketConnection *LanSocket)
{
	if(ServerSocket.ReceiveFlag==-1)
	{
		int res=0;
		for(int i=0 ; i<MAX_CLIENT_NUMBER ; i++)
		{
			if(ServerSocket.Client_fd[i]>0)
			{
				if(LanSocket->Aes.Enable==true) lansocket_AES_Encrypt(LanSocket);
				if(Dnp_Buffer.TX_Count>0)
				{
					send(ServerSocket.Client_fd[i],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
					report("sdsd","DNP3.0 -->  DNP ->>- TCP-Server:Socket",i," --  Count=",Dnp_Buffer.TX_Count);
				}
				else
				{
					send(ServerSocket.Client_fd[i],LanSocket->RX_Buffer,LanSocket->RX_Count,0);
					report("sdsd","LANSOCKET -->  LAN ->>- TCP-Server:Socket",i," --  Count=",LanSocket->RX_Count);
				}
				break;
			}
		}
	}
	else
	{
		if(LanSocket->Aes.Enable==true) lansocket_AES_Encrypt(LanSocket);
		if(Dnp_Buffer.TX_Count>0)
		{
			send(ServerSocket.Client_fd[uint8_t(ServerSocket.ReceiveFlag)],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
			report("sdsd","DNP3.0 -->  DNP ->>- TCP-Server:Socket",ServerSocket.ReceiveFlag," --  Count=",Dnp_Buffer.TX_Count);
		}
		else
		{
			send(ServerSocket.Client_fd[uint8_t(ServerSocket.ReceiveFlag)],LanSocket->RX_Buffer,LanSocket->RX_Count,0);
			report("sdsd","LANSOCKET -->  LAN ->>- TCP-Server:Socket",ServerSocket.ReceiveFlag," --  Count=",LanSocket->RX_Count);
		}
	}
	LanSocket->RX_Count=0;
	Dnp_Buffer.TX_Count=0;
}


//Send data to lan client socket connection
inline void lansocket_TCPClient_Send(SocketConnection *LanSocket)
{
	if(ClientSocket.ReceiveFlag==-1)
	{
		int res=0;
		for(int i=0 ; i<MAX_SERVER_NUMBER ; i++)
		{
			if(ClientSocket.Server_fd[i]>0 && ClientSocket.Status[i].Connected==true)
			{
				if(LanSocket->Aes.Enable==true) lansocket_AES_Encrypt(LanSocket);

				if(Dnp_Buffer.TX_Count>0)
				{
					send(ClientSocket.Server_fd[i],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
					report("sdsd","DNP3.0 -->  DNP ->>- TCP-Client:Socket",i," --  Count=",Dnp_Buffer.TX_Count);
				}
				else
				{
					res=send(ClientSocket.Server_fd[i],LanSocket->RX_Buffer,LanSocket->RX_Count,0);
					report("sdsd","LANSOCKET -->  LAN ->>- TCP-Client:Socket",i," --  Count=",LanSocket->RX_Count);
				}
				break;
			}

		}
	}
	else
	{
		if(LanSocket->Aes.Enable==true) lansocket_AES_Encrypt(LanSocket);
		if(Dnp_Buffer.TX_Count>0)
		{
			send(ClientSocket.Server_fd[uint8_t(ClientSocket.ReceiveFlag)],Dnp_Buffer.TX_Buffer,Dnp_Buffer.TX_Count,0);
			report("sdsd","DNP3.0 -->  DNP ->>- TCP-Client:Socket",uint8_t(ClientSocket.ReceiveFlag)," --  Count=",Dnp_Buffer.TX_Count);
		}
		else
		{
			send(ClientSocket.Server_fd[uint8_t(ClientSocket.ReceiveFlag)],LanSocket->RX_Buffer,LanSocket->RX_Count,0);
			report("sdsd","LANSOCKET -->  LAN ->>- TCP-Client:Socket",ClientSocket.ReceiveFlag," --  Count=",LanSocket->RX_Count);
		}
	}

	LanSocket->RX_Count=0;
	Dnp_Buffer.TX_Count=0;
}


//Receive data from connection on lan
void *lansocket_Lan_Receive(void *arg)
{
	char Buffer[LANSOCKET_BUFFER_SIZE];
	SocketConnection &LanSocket = *((SocketConnection *)arg);
	int num_bytes,SocketFD;
	fd_set readfds;
	timeval	tim;
	char activity=0,Connected=0;



	report("s","LANSOCKET --> LANSOCKET_LAN_RX_Thread Started !");

	tim.tv_usec=SOCKET_TIMEOUT_VAL;
	tim.tv_sec=0;

	LanSocket.RX_Count=0;

	while(1)
	{
		FD_ZERO(&readfds);

		if(LanSocket.mode==CLIENT)
		{
			SocketFD=LanSocket.client.Server_fd[0];
			if(LanSocket.client.Status[0].Connected==true) FD_SET( SocketFD , &readfds);
			activity = select( SocketFD + 1 , &readfds , NULL , NULL , &tim);


			if(activity>0)
			{
				if (FD_ISSET( SocketFD , &readfds))
				{
					num_bytes = read( SocketFD , Buffer, LANSOCKET_BUFFER_SIZE);
					if(num_bytes == 0)
					{
						report("s","LANSOCKET --> TCPLAN-Client:Socket --> Connection Error !");
						LanSocket.client.Status[0].Connected=false;
					}
					else if(num_bytes>0)
					{
							memcpy(LanSocket.RX_Buffer+LanSocket.RX_Count,Buffer,num_bytes);
							LanSocket.RX_Count+=num_bytes;
							if(ServerSocket.Server_fd!=0) lansocket_TCPServer_Send(&LanSocket);
							else lansocket_TCPClient_Send(&LanSocket);
							exec2(LED_SKTG_SHOT);
					}
					else
					{
						report("sd","LANSOCKET --> TCPLAN-Client:Socket --> ReadFD error=",errno);
					}
				}
			}
		}
		else								///==================  Server Mode =====================
		{
			SocketFD=LanSocket.server.Client_fd[0];
			if(SocketFD>0) FD_SET( SocketFD , &readfds);
			activity = select( SocketFD + 1 , &readfds , NULL , NULL , &tim);

			if(activity>0)
			{

				if (FD_ISSET( SocketFD , &readfds))
				{
					num_bytes = read( SocketFD , Buffer, LANSOCKET_BUFFER_SIZE);
					if(num_bytes == 0)
					{
						struct sockaddr_in 	Serv_addr;
						int 				AddLen;
						getpeername(LanSocket.server.Client_fd[0] , (struct sockaddr*)&Serv_addr.sin_addr,(socklen_t*)&AddLen);
						report("sssd","LANSOCKET --> TCPLAN-Server -> Host disconnected: ip=",inet_ntoa(Serv_addr.sin_addr)," , port=",ntohs(Serv_addr.sin_port));
						close(LanSocket.server.Client_fd[0]);
						LanSocket.server.Client_fd[0]=0;
					}
					else if(num_bytes>0)
					{
							memcpy(LanSocket.RX_Buffer+LanSocket.RX_Count,Buffer,num_bytes);
							LanSocket.RX_Count+=num_bytes;
							if(ServerSocket.Server_fd!=0) lansocket_TCPServer_Send(&LanSocket);
							else lansocket_TCPClient_Send(&LanSocket);

							exec2(LED_SKTG_SHOT);
					}
					else
					{
						//printf("Read Error \n");
						//printf("((%s)) TCP-Client:Socket0 --> ReadFD error=%d \n",get_time(),errno);
					}
				}
			}
		}


		//Send data from DNP to internet
		if(Dnp_Buffer.TX_Count>0)
		{
			if(ServerSocket.Server_fd!=0)
			{
				if(ServerSocket.Parameters.Type==TCP) lansocket_TCPServer_Send(&LanSocket);
			}
			else
			{
				if(ClientSocket.Parameters->Type==TCP ) lansocket_TCPClient_Send(&LanSocket);
			}
			report("s","LANSOCKET --> After DNP packet send!");
		}

	}
	return NULL;
}











