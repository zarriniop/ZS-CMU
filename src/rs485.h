/*
 * rs485.h
 *
 *  Created on: Aug 6, 2021
 *      Author: mhn
 */

#ifndef LIB_RS485_H_
#define LIB_RS485_H_

#define		RS485_SERIAL_FILE		"/dev/ttyS2"


#define		UCI_GET_485BAUD			"uci get communication.rs485.baudrate"
#define		UCI_GET_485BITS			"uci get communication.rs485.databits"
#define		UCI_GET_485PARITY		"uci get communication.rs485.parity"
#define		UCI_GET_485STOPBITS		"uci get communication.rs485.stopbits"
#define		UCI_GET_485TIMEOUT		"uci get communication.rs485.timeout"



extern CLIENTSOCKET  ClientSocket;
extern SERVERSOCKET ServerSocket;

result rs485_SerialPort_Close(SerialPort *SerialPORT);
result rs485_SerialPort_Open(SerialPort *SerialPORT);
inline void rs485_SerialPort_Send(SerialPort *SerialPORT);
void *rs485_SerialPort_Receive_thread(void *arg);


inline void rs485_TCPClient_SocketOpen(uint8_t n);
void *rs485_TCPClient_ConnectionCheck(void *arg);
inline void rs485_TCPClient_Send(SerialPort *SerialPORT);
void *rs485_TCPClient_Receive(void *arg);

void *rs485_UDPClient_Receive(void *arg);
inline void rs485_UDPClient_Send(SerialPort *SerialPORT);




void *rs485_TCPServer_ConnectionCheck(void *arg);
inline void rs485_TCPServer_Send(SerialPort *SerialPORT);
void *rs485_TCPServer_Receive(void *arg);

void *rs485_UDPServer_Receive(void *arg);
inline void rs485_UDPServer_Send(SerialPort *SerialPORT);


void rs485_AES_Encrypt(SerialPort *PORT);
void rs485_AES_Decrypt(AES *aes,uint8_t *Buffer,uint16_t count);



class RS485{       // The class
	public:

		SerialPort		SerialPORT;
		SocketMode		ConnectionMode;
		pthread_t		threads[3];
		result			Start();    	//open port and start communication
		result			Stop();
		result			Init();
};

result RS485::Init()
{
	char str[20],pos=0;
	SerialPORT.Parameters.File=RS485_SERIAL_FILE;
	exec(UCI_GET_MODE, str, 1);
	if(str[0]=='2')
	{
		ConnectionMode=CLIENT;
		memset(str,0,20);
		exec(UCI_GET_ADDRESS1, str, 1);
		string ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[0].IP=ss;

		memset(str,0,20);
		exec(UCI_GET_ADDRESS2, str, 1);
		ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[1].IP=ss;

		memset(str,0,20);
		exec(UCI_GET_ADDRESS3, str, 1);
		ss=str;
		pos=ss.find("\n");
		ss=ss.substr(0,pos);
		ClientSocket.Parameters[2].IP=ss;

		memset(str,0,20);
		exec(UCI_GET_PORT1, str, 1);
		ClientSocket.Parameters[0].PORT=stoi(str);
		memset(str,0,20);
		exec(UCI_GET_PORT2, str, 1);
		ClientSocket.Parameters[1].PORT=stoi(str);
		memset(str,0,20);
		exec(UCI_GET_PORT3, str, 1);
		ClientSocket.Parameters[2].PORT=stoi(str);


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

		ClientSocket.ReceiveFlag=-1;

		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			ClientSocket.Status[n].Connected=false;
			ClientSocket.Status[n].Opened=false;
			ClientSocket.create(n);
		}

		exec(UCI_GET_KEEPALIVE, str, 1);
		ClientSocket.keepalive =stoi(str);
		ClientSocket.keepalive/=10;
		if(ClientSocket.keepalive <6) ClientSocket.keepalive=6;


	}
	else
	{
		ConnectionMode=SERVER;

		memset(str,0,20);
		exec(UCI_GET_LISTENPORT, str, 1);
		ServerSocket.Parameters.PORT= stoi(str);

		exec(UCI_GET_PROTOCOL, str, 1);
		if(str[0]=='2') ServerSocket.Parameters.Type=TCP;
		else			ServerSocket.Parameters.Type=UDP;

		ServerSocket.ReceiveFlag=-1;
		ServerSocket.Server_fd=0;
	}


	exec(UCI_GET_485BAUD, str, 1);
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


	exec(UCI_GET_485STOPBITS, str, 1);
	if(str[0]=='0') SerialPORT.Parameters.StopBits=STOPBITS_ONE;
	else SerialPORT.Parameters.StopBits=STOPBITS_TWO;

	exec(UCI_GET_485PARITY, str, 1);
	if(str[0]=='0') SerialPORT.Parameters.Parity=PARITY_NONE;
	else SerialPORT.Parameters.Parity=PARITY_YES;

	exec(UCI_GET_485BITS, str, 1);
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
	exec(UCI_GET_485TIMEOUT, str, 1);
	SerialPORT.Timeout= stoi(str)*1000;

	exec(UCI_GET_AES, str, 1);
	if(str[0]=='1') SerialPORT.Aes.Enable=true;
	else	SerialPORT.Aes.Enable=false;

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

	SerialPORT.RX_Count=0;
	SerialPORT.TX_Count=0;

	return OK;
}

result RS485::Start()
{

	Timer timer;


	this->Init();

	rs485_SerialPort_Open(&SerialPORT);

	if(SerialPORT.Aes.Enable==true)
	{
		AES_init_ctx(&SerialPORT.Aes.ctx, SerialPORT.Aes.Key);
	}


	if(ConnectionMode==SERVER)
	{
		ServerSocket.create();
		ServerSocket.get_listen();
		pthread_create( &threads[0] , NULL , rs485_TCPServer_ConnectionCheck ,(void*) &SerialPORT);
		pthread_create( &threads[1] , NULL , rs485_TCPServer_Receive ,(void*) &SerialPORT);
		pthread_create( &threads[2] , NULL , rs485_SerialPort_Receive_thread ,(void*) &SerialPORT);
	}
	else
	{
		if(ClientSocket.Parameters->Type==TCP)
		{
			pthread_create( &threads[0] , NULL , rs485_TCPClient_Receive ,(void*) &SerialPORT);
			pthread_create( &threads[2] , NULL , rs485_TCPClient_ConnectionCheck ,(void*) &SerialPORT);
		}
		else
		{
			ClientSocket.create(0);
			pthread_create( &threads[0] , NULL , rs485_UDPClient_Receive ,(void*) &SerialPORT);
		}
		pthread_create( &threads[1] , NULL , rs485_SerialPort_Receive_thread ,(void*) &SerialPORT);
	}

	return OK;
}



result RS485::Stop()
{

	printf("\r\n RS485_closing threads.....\r\n");

	pthread_cancel(threads[0]);
	pthread_cancel(threads[1]);
	pthread_cancel(threads[2]);


	printf("\r\n RS485_Stopping.....\r\n");
	rs485_SerialPort_Close(&SerialPORT);


	if(ConnectionMode==SERVER)
	{
		ServerSocket.get_close();

	}
	else
	{
		for(int n=0 ; n<MAX_SERVER_NUMBER ; n++)
		{
			ClientSocket.get_close(n);
		}
	}


	printf("\r\n RS485_Stopped\r\n");
	report("s","RS485 --> Stopped  !");

	return OK;
}




#endif /* LIB_RS485_H_ */
