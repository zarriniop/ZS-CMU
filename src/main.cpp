//============================================================================
// Name        : ZS-LRM100.cpp
// Author      : mh.najafi
// Version     :
// Copyright   : ZarrinSamaneh
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "main.h"
#include "aes.hpp"
#include "rs485.h"
#include "rs232.h"
#include "lansocket.h"
#include "gpio.h"





using namespace std;

CLIENTSOCKET 	ClientSocket;
SERVERSOCKET 	ServerSocket;
LANSOCKET		LanSocket;


RS232 	rs232;
RS485 	rs485;
GPIO	gpio;

SerialPort Dnp_Buffer;


int main()
{
	uint8_t commod=0,run_flag=0;
	char Setting[4096];

	//Print spplication start information on system log
	AppStartPrint();


	//Read device serial number from mtd and write to uci config file
	get_sn();


	while(1)
	{

		if(commod==0)	//Before running communication service
		{
			//Read selected communication interface from uci config file
			commod=get_communication_mode();

			//Read all of communication setting parameters from uci config file
			Settings_Read(Setting);

			//Initialize GPIO and start services
			gpio.Start();

			//Run Services according to selected Communication interface
			if(commod=='3' || commod=='4') 		LanSocket.Start();
			else if(commod=='2') 	rs485.Start();
			else if(commod=='1') 	rs232.Start();
		}
		else		//After running communication service
		{
			//Check for any changes in Communication setting parameters from uci config file
			if(Settings_Change_Check(Setting)==1)
			{
				report("s","Communication settings has changes! -Restarting Communication Threads");

				//Stop current running communication service
				if(commod=='3' || commod=='4') 		LanSocket.Stop();
				else if(commod=='2') 	rs485.Stop();
				else if(commod=='1') 	rs232.Stop();
				gpio.Stop();

				//Update settings
				Settings_Read(Setting);
				commod=0;
			}
		}
		//Delay in seconds
		sleep(3);
	}

	return 0;
}
