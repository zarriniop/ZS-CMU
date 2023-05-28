/*
 * lansocket.h
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */

#ifndef LIB_LANSOCKET_H_
#define LIB_LANSOCKET_H_

#define 	LANSOCKET_THREADS_NUMBER	4

#define		UCI_GET_LANPROTO		"uci get communication.lansocket.protocol"
#define		UCI_GET_LANMODE			"uci get communication.lansocket.mode"
#define		UCI_GET_LANADDR1		"uci get communication.lansocket.server1address"
#define		UCI_GET_LANPORT1		"uci get communication.lansocket.server1port"
#define		UCI_GET_LANLISTEN		"uci get communication.lansocket.listenport"
#define		UCI_GET_LANKEEPALV		"uci get communication.lansocket.keepalive"



extern CLIENTSOCKET  ClientSocket;
extern SERVERSOCKET ServerSocket;


inline void lansocket_Lan_Send(SerialPort *SerialPORT);
void *lansocket_Lan_Receive(void *arg);

inline void lansocket_LANClient_SocketOpen(SocketConnection *lansocket);
void *lansocket_LANServer_ConnectionCheck(void *arg);
void *lansocket_LANClient_ConnectionCheck(void *arg);


inline void lansocket_TCPClient_SocketOpen(uint8_t n);
void *lansocket_TCPClient_ConnectionCheck(void *arg);
void *lansocket_TCPClient_Receive(void *arg);

inline void lansocket_TCPServer_Send(SerialPort *SerialPORT);
void *lansocket_TCPServer_ConnectionCheck(void *arg);
void *lansocket_TCPServer_Receive(void *arg);

void lansocket_AES_Encrypt(SerialPort *PORT);
void lansocket_AES_Decrypt(AES *aes,uint8_t *Buffer,uint16_t count);




class LANSOCKET{       // The class
	public:

		SocketConnection		lanconnection;
		SocketMode				ConnectionMode;
		pthread_t				threads[LANSOCKET_THREADS_NUMBER];
		result					Start();    	//open port and start communication
		result					Stop();
		result					Init();
};


//Read socket on lan communication service data from uci config file
result LANSOCKET::Init()
{
	char str[20],pos=0;
	exec(UCI_GET_MODE, str, 1);
	if(str[0]=='2')			//Modem works as Client on internet connection
	{
		ConnectionMode=CLIENT;

		//Read Server1 IP address from uci config file
		memset(str,0,20);
		exec(UCI_GET_ADDRESS1, str, 1);
		string ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[0].IP=ss;

		//Read Server2 IP address from uci config file
		memset(str,0,20);
		exec(UCI_GET_ADDRESS2, str, 1);
		ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[1].IP=ss;

		//Read Server3 IP address from uci config file
		memset(str,0,20);
		exec(UCI_GET_ADDRESS3, str, 1);
		ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[2].IP=ss;

		//Read Server1 PORT from uci config file
		memset(str,0,20);
		exec(UCI_GET_PORT1, str, 1);
		ClientSocket.Parameters[0].PORT=stoi(str);

		//Read Server2 PORT from uci config file
		memset(str,0,20);
		exec(UCI_GET_PORT2, str, 1);
		ClientSocket.Parameters[1].PORT=stoi(str);

		//Read Server3 PORT from uci config file
		memset(str,0,20);
		exec(UCI_GET_PORT3, str, 1);
		ClientSocket.Parameters[2].PORT=stoi(str);

		//Read Communication protocol from uci config file
		exec(UCI_GET_PROTOCOL, str, 1);
		if(str[0]=='2')
		{
			for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
				ClientSocket.Parameters[n].Type=TCP;
		}
		else
		{
			for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
				ClientSocket.Parameters[n].Type=UDP;
		}

		//Set default value for control flags
		ClientSocket.ReceiveFlag=-1;

		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			ClientSocket.Status[n].Connected=false;
			ClientSocket.Status[n].Opened=false;
			ClientSocket.create(n);
		}

		//Read server Keep alive parameter from uci config file
		exec(UCI_GET_KEEPALIVE, str, 1);
		ClientSocket.keepalive =stoi(str);
		ClientSocket.keepalive/=10;
		if(ClientSocket.keepalive <6) ClientSocket.keepalive=6;

	}
	else		//Modem works as Server on internet connection
	{
		ConnectionMode=SERVER;

		//Read listen port from uci config file
		memset(str,0,20);
		exec(UCI_GET_LISTENPORT, str, 1);
		ServerSocket.Parameters.PORT= stoi(str);

		//Read Communication protocol from uci config file
		exec(UCI_GET_PROTOCOL, str, 1);
		if(str[0]=='2') ServerSocket.Parameters.Type=TCP;
		else			ServerSocket.Parameters.Type=UDP;

		//Set default value for control flags
		ServerSocket.ReceiveFlag=-1;
		ServerSocket.Server_fd=0;
	}


	//Read lan socket settings from uci config file
	exec(UCI_GET_LANMODE, str, 1);
	if(str[0]=='2')			//Modem works as Client on lan connection
	{
		lanconnection.mode=CLIENT;

		//Read Server IP address from uci config file
		memset(str,0,20);
		exec(UCI_GET_LANADDR1, str, 1);
		string ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		lanconnection.client.Parameters[0].IP=ss;

		//Read Server PORT from uci config file
		memset(str,0,20);
		exec(UCI_GET_LANPORT1, str, 1);
		lanconnection.client.Parameters[0].PORT=stoi(str);

		//Read Communication protocol from uci config file
		exec(UCI_GET_LANPROTO, str, 1);
		if(str[0]=='2')
			lanconnection.client.Parameters[0].Type=TCP;
		else
			lanconnection.client.Parameters[0].Type=UDP;

		//Set default value for control flags
		lanconnection.client.ReceiveFlag=-1;
		lanconnection.client.Status[0].Connected=false;
		lanconnection.client.Status[0].Opened=false;
		lanconnection.client.create(0);

		//Read server Keep alive parameter from uci config file
		exec(UCI_GET_LANKEEPALV, str, 1);
		lanconnection.client.keepalive =stoi(str);
		lanconnection.client.keepalive/=10;
		if(lanconnection.client.keepalive <6) lanconnection.client.keepalive=6;

	}
	else			//Modem works as Server on lan connection
	{
		lanconnection.mode=SERVER;

		//Read listen port from uci config file
		memset(str,0,20);
		exec(UCI_GET_LANLISTEN, str, 1);
		lanconnection.server.Parameters.PORT= stoi(str);

		//Read Communication protocol from uci config file
		exec(UCI_GET_LANPROTO, str, 1);
		if(str[0]=='2') lanconnection.server.Parameters.Type=TCP;
		else			lanconnection.server.Parameters.Type=UDP;

		//Set default value for control flags
		lanconnection.server.ReceiveFlag=-1;
		lanconnection.server.Server_fd=0;

	}

	//Read AES enable
	exec(UCI_GET_AES, str, 1);
	if(str[0]=='1') lanconnection.Aes.Enable=true;
	else	lanconnection.Aes.Enable=false;


	//Read AES encryption key
	for(uint8_t i=0;i<16;i++)
	{
		char str2[35],str1[3];
		uint8_t cc=0;
		string ss=str1;
		sprintf(str2,"%s%u",UCI_GET_AESKEY,i);
		memset(str,0,20);
		exec(str2, str1, 1);
		ss=str1;
		cc=stoi(ss,0,16);
		lanconnection.Aes.Key[i]=cc;
	}

	//Set default value for control flags
	lanconnection.RX_Count=0;
	lanconnection.TX_Count=0;

	return OK;
}

result LANSOCKET::Start()
{
	Timer timer;

	//Initialize socket on lan communication service : Read parameters from uci config file
	this->Init();

	//Initialize AES if needed
	if(lanconnection.Aes.Enable==true)
	{
		AES_init_ctx(&lanconnection.Aes.ctx, lanconnection.Aes.Key);
	}


	if(lanconnection.mode==SERVER)		//Modem works in server mode on lan connection
	{
		//Create Socket
		lanconnection.server.create();

		//Listen on socket
		lanconnection.server.get_listen();

		//Create a thread for Manage connection from Clients
		pthread_create( &threads[0] , NULL , lansocket_LANServer_ConnectionCheck ,(void*) &lanconnection);
	}
	else	//Modem works in client mode on lan connection
	{
		//Create a thread for Manage connection to Servers
		pthread_create( &threads[0] , NULL , lansocket_LANClient_ConnectionCheck ,(void*) &lanconnection);
	}

	if(ConnectionMode==SERVER)		//Modem works in server mode on internet connection
	{
		//Create Socket
		ServerSocket.create();

		//Listen on socket
		ServerSocket.get_listen();

		//Create a thread for Manage connection from Clients
		pthread_create( &threads[1] , NULL , lansocket_TCPServer_ConnectionCheck ,(void*) &lanconnection);

		//Create a thread for Receive data from Clients on internet connection
		pthread_create( &threads[2] , NULL , lansocket_TCPServer_Receive ,(void*) &lanconnection);

		//Create a thread for Receive data from lan connection
		pthread_create( &threads[3] , NULL , lansocket_Lan_Receive ,(void*) &lanconnection);


	}
	else
	{
		//Create a thread for Receive data from Servers on internet connection
		pthread_create( &threads[1] , NULL , lansocket_TCPClient_Receive ,(void*) &lanconnection);

		//Create a thread for Receive data from lan connection
		pthread_create( &threads[2] , NULL , lansocket_Lan_Receive ,(void*) &lanconnection);

		//Create a thread for Manage connection to Servers
		pthread_create( &threads[3] , NULL , lansocket_TCPClient_ConnectionCheck ,(void*) &lanconnection);
	}

	return OK;
}


//Stop communication services
result LANSOCKET::Stop()
{
	printf("\r\n LANSOCKET_Stopping.....\r\n");


	if(lanconnection.mode==SERVER)	//Close Socket in Server mode (lan Connection)
	{
		lanconnection.server.get_close();

	}
	else		//Close Sockets in client mode (lan Connection)
	{
		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			lanconnection.client.get_close(n);
		}
	}

	printf("\r\n LANSOCKET_closing threads.....\r\n");

	//Stop running threads
	pthread_cancel(threads[0]);
	pthread_cancel(threads[1]);
	pthread_cancel(threads[2]);
	pthread_cancel(threads[2]);


	if(ConnectionMode==SERVER)	//Close Socket in Server mode (Internet Connection)
	{
			ServerSocket.get_close();
	}
	else	//Close Sockets in client mode (Internet Connection)
	{
		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			ClientSocket.get_close(n);
		}
	}



	printf("\r\n LANSOCKET_Stopped\r\n");
	report("s","LANSOCKET --> Stopped  !");

	return OK;
}




#endif /* LIB_LANSOCKET_H_ */
