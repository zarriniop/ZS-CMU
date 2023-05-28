/*
 * tools.cpp
 *
 *  Created on: Nov 13, 2021
 *      Author: mhn
 */


#include "main.h"

#define		UCI_GET_INTERFACE	"uci get communication.type.type"
#define		UCI_GET_FLAG     	"uci get communication.type.flag"
#define		UCI_SET_FLAG     	"uci set communication.type.flag=0"
#define		UCI_COMMIT_CMU		"uci commit communication"
#define 	UCI_GET_SN			"hexdump -n 4 -d /dev/mtd2"

bool Executing=false;

void delay(int t)
{
	for (int c = 1; c <= t; c++)
	{
		for (int d = 1; d <= 32767; d++)
		{
		}
	}
}


void AppStartPrint()
{
	report("s","====================================");
	report("s","   ZS-LRM      ");
	report("s","   Build: 2023-01-19   ");
	report("s","   Company: ZarrinSamaneSharq  ");
	report("s","   Developer: M.H.Najafi  ");
	report("s","====================================");
}

void exec(const char* cmd,char *resultt,uint32_t time) {
    uint16_t cnt=0;
    clock_t clk,t=0;
    char buffer[1000],count;

	FILE* pipe = popen(cmd, "r");
	if (!pipe) report("popen() failed!\n");
	try
	{
		memset(buffer,0,1000);
		memset(resultt,0,sizeof(resultt));
		t=time*1000;
		clk=clock();

		while(clock()-clk<t)
		{
			if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			{

				memcpy(resultt+cnt,buffer,strlen(buffer));
				cnt+=strlen(buffer);
				//memcpy(resultt,buffer,sizeof(buffer));
				count=0;
				//printf("---------------------eeeee=%s\r\n",buffer);
			}
			else
			{
				count++;
				usleep(100);
				//if(count>20 && cnt>0) break;
			}

		}

	}
	catch (...)
	{
		pclose(pipe);
		//throw;
	}
	pclose(pipe);


}

void exec2(const char* cmd) {

	//FILE* pipe = popen(cmd, "r");
	//if (!pipe) printf("popen() failed!\n");

	//pclose(pipe);
	std::system(cmd);

}

char get_communication_mode()
{
	char str[5];
	exec(UCI_GET_INTERFACE, str, 1);
	return str[0];
}

char Settings_Change_Check(char *Setting)
{
	char buffer[4096];

	memset(buffer,0,sizeof(buffer));
	exec("uci show communication", buffer, 10);

	for(uint16_t i=0; i< strlen(buffer) ; i++)
	{
		if( buffer[i]!= Setting[i] )
		{

			return 1;
		}
	}
	return 0;
}

void Settings_Read(char *Setting)
{
	memset(Setting,0,sizeof(Setting));
	exec("uci show communication", Setting, 10);
}




void get_sn()
{
	char str[100],str2[10],str3[10];

	memset(str,0,sizeof(str));
	memset(str2,0,sizeof(str2));
	memset(str3,0,sizeof(str3));

	//Read Serial number from mtd block
	exec(UCI_GET_SN, str, 1);

	memcpy(str2,str+10,5);
	memcpy(str3,str+18,5);


	try
	{
		uint32_t v1=stoi(str+10);
		uint32_t v2=stoi(str+18);

		//uint32_t sn=( (v2 << 16 ) & 0xFF000000 ) |  ( (v2 << 8 ) & 0xFF0000 ) | ( (v1 << 8) & 0xFF00 ) | ( (v1 >> 8) & 0xFF );
		uint32_t sn= (v2 << 16 ) |  v1;

		///printf("sn==  %x \n",sn);
		memset(str,0,sizeof(str));

		//Write Serial Number to uci config file
		sprintf(str,"uci set system.@system[0].sn=%d",sn);
		exec2(str);

		//Read Firmware version file and write to uci config file
		exec("cat /root/version", str2, 10);
		sprintf(str,"uci set system.@system[0].firmware=%s",str2);
		exec2(str);

		exec2("uci commit system");

	}
	catch (...)
	{
		report("s","ZS-CMU >> Get SN Error");
	}


}



long diff_time_us(struct timeval *start)
{
	 struct timeval  tv;
	 long diff=0;
	 gettimeofday (&tv, NULL);
	 diff=tv.tv_usec - start->tv_usec;
	 if(diff < 0) diff+=1000000;

	 return diff;
}

long diff_time_sec(struct timeval *start)
{
	 struct timeval  tv;
	 long diff=0;
	 gettimeofday (&tv, NULL);
	 diff=tv.tv_sec - start->tv_sec;

	 return diff;
}


const char * get_time(void) {
    static char time_buf[128];
    struct timeval  tv;
    time_t time;
    suseconds_t millitm;
    struct tm *ti;

    gettimeofday (&tv, NULL);

    time= tv.tv_sec;
    millitm = (tv.tv_usec + 500) / 1000;

    if (millitm == 1000) {
        ++time;
        millitm = 0;
    }

    ti = localtime(&time);
    sprintf(time_buf, "%02d-%02d_%02d:%02d:%02d:%03d", ti->tm_mon+1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec, (int)millitm);
    return time_buf;
}


void report(char *format, ... )
{
	va_list args;
	int i=0,count=0;
	char ss[100],str[400];
	va_start( args, format );

	int		d;
	float   f;
	char   *s;

	memset(str,0,400);

	for( i = 0; format[i] != '\0'; ++i )
	{
		switch( format[i] )
		{
	         case 'd':
	        	d=va_arg(args, int );
	            sprintf(ss,"%d\0",d);
	         break;

	         case 'f':
	        	 f=va_arg(args, double );
	             sprintf(ss,"%f\0",f);
	         break;


	         case 's':
	        	 s=va_arg(args, char *);
	             sprintf(ss,"%s\0",s);
	         break;

	         default:
	         break;
	      }

			memcpy(str+count,ss,strlen(ss));
			count=strlen(str);

	}


	va_end( args );



	printf("((%s)) %s\n",get_time(),str);
	char str1[500];
	memset(str1,0,500);
	sprintf(str1,"logger -t \"=======(  ZS-LRM200  )=======\" \"%s\" ",str);
	std::system(str1);

}




