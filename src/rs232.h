/*
 * rs232.h
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */

#ifndef LIB_RS232_H_
#define LIB_RS232_H_

#define		RS232_SERIAL_FILE		"/dev/ttyS1"


#define		UCI_GET_232BAUD			"uci get communication.rs232.baudrate"
#define		UCI_GET_232BITS			"uci get communication.rs232.databits"
#define		UCI_GET_232PARITY		"uci get communication.rs232.parity"
#define		UCI_GET_232STOPBITS		"uci get communication.rs232.stopbits"
#define		UCI_GET_232TIMEOUT		"uci get communication.rs232.timeout"




extern CLIENTSOCKET  ClientSocket;
extern SERVERSOCKET ServerSocket;

result rs232_SerialPort_Close(SerialPort *SerialPORT);
result rs232_SerialPort_Open(SerialPort *SerialPORT);
inline void rs232_SerialPort_Send(SerialPort *SerialPORT);
void *rs232_SerialPort_Receive_thread(void *arg);


inline void rs232_TCPClient_SocketOpen(uint8_t n);
void *rs232_TCPClient_ConnectionCheck(void *arg);
inline void rs232_TCPClient_Send(SerialPort *SerialPORT);
void *rs232_TCPClient_Receive(void *arg);

void *rs232_UDPClient_Receive(void *arg);
inline void rs232_UDPClient_Send(SerialPort *SerialPORT);




void *rs232_TCPServer_ConnectionCheck(void *arg);
inline void rs232_TCPServer_Send(SerialPort *SerialPORT);
void *rs232_TCPServer_Receive(void *arg);

void *rs232_UDPServer_Receive(void *arg);
inline void rs232_UDPServer_Send(SerialPort *SerialPORT);


void rs232_AES_Encrypt(SerialPort *PORT);
void rs232_AES_Decrypt(AES *aes,uint8_t *Buffer,uint16_t count);





class RS232{       // The class
	public:

		SerialPort		SerialPORT;
		SocketMode		ConnectionMode;
		pthread_t		threads[3];
		result			Start();    	//open port and start communication
		result			Stop();
		result			Init();
};

//Read RS232 communication service data from uci config file
result RS232::Init()
{
	char str[20],pos=0;
	SerialPORT.Parameters.File=RS232_SERIAL_FILE;
	exec(UCI_GET_MODE, str, 1);
	if(str[0]=='2')	//Modem works as Client on internet connection
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




	//Read RS232 settings from uci config file
	exec(UCI_GET_232BAUD, str, 1);
	switch(str[0])
	{
		case '0':
			SerialPORT.Parameters.BaudRate=BAUD1200;
			break;

		case '1':
			if(str[1]=='0')	SerialPORT.Parameters.BaudRate=BAUD460800;
			else			SerialPORT.Parameters.BaudRate=BAUD1800;
			break;

		case '2':
			SerialPORT.Parameters.BaudRate=BAUD2400;
			break;

		case '3':
			SerialPORT.Parameters.BaudRate=BAUD4800;
			break;

		case '4':
			SerialPORT.Parameters.BaudRate=BAUD9600;
			break;

		case '5':
			SerialPORT.Parameters.BaudRate=BAUD19200;
			break;

		case '6':
			SerialPORT.Parameters.BaudRate=BAUD38400;
			break;

		case '7':
			SerialPORT.Parameters.BaudRate=BAUD57600;
			break;

		case '8':
			SerialPORT.Parameters.BaudRate=BAUD115200;
			break;

		case '9':
			SerialPORT.Parameters.BaudRate=BAUD230400;
			break;

		default:
			SerialPORT.Parameters.BaudRate=BAUD9600;
			break;
	}


	exec(UCI_GET_232STOPBITS, str, 1);
	if(str[0]=='0') SerialPORT.Parameters.StopBits=STOPBITS_ONE;
	else SerialPORT.Parameters.StopBits=STOPBITS_TWO;

	exec(UCI_GET_232PARITY, str, 1);
	if(str[0]=='0') SerialPORT.Parameters.Parity=PARITY_NONE;
	else SerialPORT.Parameters.Parity=PARITY_YES;

	exec(UCI_GET_232BITS, str, 1);
	switch(str[0])
	{
		case '0':
			SerialPORT.Parameters.bitsPbyte=Bits5;
			break;

		case '1':
			SerialPORT.Parameters.bitsPbyte=Bits6;
			break;

		case '2':
			SerialPORT.Parameters.bitsPbyte=Bits7;
			break;

		case '3':
			SerialPORT.Parameters.bitsPbyte=Bits8;
			break;

		default:
			SerialPORT.Parameters.bitsPbyte=Bits8;
			break;
	}

	memset(str,0,20);
	exec(UCI_GET_232TIMEOUT, str, 1);
	SerialPORT.Timeout= stoi(str)*1000;



	//Read AES enable
	exec(UCI_GET_AES, str, 1);
	if(str[0]=='1') SerialPORT.Aes.Enable=true;
	else	SerialPORT.Aes.Enable=false;



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
		SerialPORT.Aes.Key[i]=cc;
	}

	//Set default value for control flags
	SerialPORT.RX_Count=0;
	SerialPORT.TX_Count=0;

	return OK;
}

result RS232::Start()
{
	Timer timer;

	//Initialize socket on lan communication service : Read parameters from uci config file
	this->Init();


	//Open Rs232 Serial port
	rs232_SerialPort_Open(&SerialPORT);


	//Initialize AES if needed
	if(SerialPORT.Aes.Enable==true)
	{
		AES_init_ctx(&SerialPORT.Aes.ctx, SerialPORT.Aes.Key);
	}



	//Modem works in server mode on lan connection
	if(ConnectionMode==SERVER)
	{
		//Create Socket
		ServerSocket.create();

		//Listen on socket
		ServerSocket.get_listen();

		//Create a thread for Manage connection from Clients
		pthread_create( &threads[0] , NULL , rs232_TCPServer_ConnectionCheck ,(void*) &SerialPORT);

		//Create a thread for Receive data from Clients on internet connection
		pthread_create( &threads[1] , NULL , rs232_TCPServer_Receive ,(void*) &SerialPORT);

		//Create a thread for Receive data from RS232 connection
		pthread_create( &threads[2] , NULL , rs232_SerialPort_Receive_thread ,(void*) &SerialPORT);
	}
	else
	{
		if(ClientSocket.Parameters->Type==TCP)
		{
			//Create a thread for Receive data from Servers on internet connection
			pthread_create( &threads[0] , NULL , rs232_TCPClient_Receive ,(void*) &SerialPORT);

			//Create a thread for Manage connection to Servers
			pthread_create( &threads[2] , NULL , rs232_TCPClient_ConnectionCheck ,(void*) &SerialPORT);
		}
		else
		{
			ClientSocket.create(0);
			pthread_create( &threads[0] , NULL , rs232_UDPClient_Receive ,(void*) &SerialPORT);
		}
		//Create a thread for Receive data from RS232 connection
		pthread_create( &threads[1] , NULL , rs232_SerialPort_Receive_thread ,(void*) &SerialPORT);
	}

	return OK;
}


result RS232::Stop()
{


	//Stop running threads
	pthread_cancel(threads[0]);
	pthread_cancel(threads[1]);
	pthread_cancel(threads[2]);



	//Close RS232 Serial Port
	rs232_SerialPort_Close(&SerialPORT);



	if(ConnectionMode==SERVER) 			//Close Socket in Server mode (Internet Connection)
	{
		ServerSocket.get_close();

	}
	else								//Close Sockets in client mode (Internet Connection)
	{
		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			ClientSocket.get_close(n);
		}
	}

	report("s","RS232 --> Stopped  !");

	return OK;
}




#endif /* LIB_RS232_H_ */
