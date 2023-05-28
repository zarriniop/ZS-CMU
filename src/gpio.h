/*
 * gpio.h
 *
 *  Created on: Nov 21, 2021
 *      Author: mhn
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "tools.h"

#define		GPIO_REFRESH_TIME	3


#define		GPIO_WRITE1_LED_G232		"echo 1 > /sys/devices/platform/leds/leds/LED:G232/brightness"
#define		GPIO_WRITE1_LED_R232		"echo 1 > /sys/devices/platform/leds/leds/LED:R232/brightness"
#define		GPIO_WRITE1_LED_G485		"echo 1 > /sys/devices/platform/leds/leds/LED:G485/brightness"
#define		GPIO_WRITE1_LED_R485		"echo 1 > /sys/devices/platform/leds/leds/LED:R485/brightness"
#define		GPIO_WRITE1_LED_SKTG		"echo 1 > /sys/devices/platform/leds/leds/LED:SktG/brightness"
#define		GPIO_WRITE1_LED_SKTR		"echo 1 > /sys/devices/platform/leds/leds/LED:SktR/brightness"
#define		GPIO_WRITE1_LED_STATUS		"echo 1 > /sys/devices/platform/leds/leds/LED:Status/brightness"
#define		GPIO_WRITE1_OUT_RELAY2		"echo 1 > /sys/devices/platform/leds/leds/output:Relay2/brightness"
#define		GPIO_WRITE1_OUT_RELAY1		"echo 1 > /sys/devices/platform/leds/leds/output:Relay1/brightness"
#define		GPIO_WRITE1_OUT_GSMRST		"echo 1 > /sys/devices/platform/leds/leds/output:GSMrst/brightness"
#define		GPIO_WRITE1_OUT_GSMPWR		"echo 1 > /sys/devices/platform/leds/leds/output:GSMpwr/brightness"
#define		GPIO_WRITE1_OUT_DIR			"echo 1 > /sys/devices/platform/leds/leds/output:Dir/brightness"


#define		GPIO_WRITE0_LED_G232		"echo 0 > /sys/devices/platform/leds/leds/LED:G232/brightness"
#define		GPIO_WRITE0_LED_R232		"echo 0 > /sys/devices/platform/leds/leds/LED:R232/brightness"
#define		GPIO_WRITE0_LED_G485		"echo 0 > /sys/devices/platform/leds/leds/LED:G485/brightness"
#define		GPIO_WRITE0_LED_R485		"echo 0 > /sys/devices/platform/leds/leds/LED:R485/brightness"
#define		GPIO_WRITE0_LED_SKTG		"echo 0 > /sys/devices/platform/leds/leds/LED:SktG/brightness"
#define		GPIO_WRITE0_LED_SKTR		"echo 0 > /sys/devices/platform/leds/leds/LED:SktR/brightness"
#define		GPIO_WRITE0_LED_STATUS		"echo 0 > /sys/devices/platform/leds/leds/LED:Status/brightness"
#define		GPIO_WRITE0_OUT_RELAY2		"echo 0 > /sys/devices/platform/leds/leds/output:Relay2/brightness"
#define		GPIO_WRITE0_OUT_RELAY1		"echo 0 > /sys/devices/platform/leds/leds/output:Relay1/brightness"
#define		GPIO_WRITE0_OUT_GSMRST		"echo 0 > /sys/devices/platform/leds/leds/output:GSMrst/brightness"
#define		GPIO_WRITE0_OUT_GSMPWR		"echo 0 > /sys/devices/platform/leds/leds/output:GSMpwr/brightness"
#define		GPIO_WRITE0_OUT_DIR			"echo 0 > /sys/devices/platform/leds/leds/output:Dir/brightness"


#define		GPIO_READ_IN1		"cat /sys/class/gpio/gpio496/value"
#define		GPIO_READ_IN2		"cat /sys/class/gpio/gpio497/value"
#define		GPIO_READ_IN3		"cat /sys/class/gpio/gpio498/value"
#define		GPIO_READ_IN4		"cat /sys/class/gpio/gpio499/value"

#define		GPIO_IN1_FILE			"/sys/class/gpio/gpio496/value"
#define		GPIO_IN2_FILE			"/sys/class/gpio/gpio497/value"
#define		GPIO_IN3_FILE			"/sys/class/gpio/gpio498/value"
#define		GPIO_IN4_FILE			"/sys/class/gpio/gpio499/value"

#define		GPIO_ADD_IN1		"echo \"496\" > /sys/class/gpio/export"
#define		GPIO_ADD_IN2		"echo \"497\" > /sys/class/gpio/export"
#define		GPIO_ADD_IN3		"echo \"498\" > /sys/class/gpio/export"
#define		GPIO_ADD_IN4		"echo \"499\" > /sys/class/gpio/export"

#define		GPIO_ACTLOW_IN1		"echo 1 > /sys/class/gpio/gpio496/active_low"
#define		GPIO_ACTLOW_IN2		"echo 1 > /sys/class/gpio/gpio497/active_low"
#define		GPIO_ACTLOW_IN3		"echo 1 > /sys/class/gpio/gpio498/active_low"
#define		GPIO_ACTLOW_IN4		"echo 1 > /sys/class/gpio/gpio499/active_low"

#define		LED_G232_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:G232/trigger"
#define		LED_R232_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:R232/trigger"
#define		LED_G485_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:G485/trigger"
#define		LED_R485_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:R485/trigger"
#define		LED_SKTG_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:SktG/trigger"
#define		LED_SKTR_SHOTEN		"echo oneshot > /sys/devices/platform/leds/leds/LED:SktR/trigger"

#define		LED_G232_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:G232/delay_on"
#define		LED_R232_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:R232/delay_on"
#define		LED_G485_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:G485/delay_on"
#define		LED_R485_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:R485/delay_on"
#define		LED_SKTG_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:SktG/delay_on"
#define		LED_SKTR_ONDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:SktR/delay_on"

#define		LED_G232_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:G232/delay_off"
#define		LED_R232_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:R232/delay_off"
#define		LED_G485_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:G485/delay_off"
#define		LED_R485_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:R485/delay_off"
#define		LED_SKTG_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:SktG/delay_off"
#define		LED_SKTR_OFFDLY		"echo 10 > /sys/devices/platform/leds/leds/LED:SktR/delay_off"



#define		LED_STATUS_TIMEREN	"echo timer > /sys/devices/platform/leds/leds/LED:Status/trigger"
#define		LED_STATUS_ONDLY	"echo 100 > /sys/devices/platform/leds/leds/LED:Status/delay_on"
#define		LED_STATUS_OFFDLY1	"echo 500 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY2	"echo 1000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY3	"echo 2000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"
#define		LED_STATUS_OFFDLY4	"echo 4000 > /sys/devices/platform/leds/leds/LED:Status/delay_off"

#define 	UCI_GET_DNPSRC		"uci get communication.dnp.src"
#define 	UCI_GET_DNPDIST		"uci get communication.dnp.dist"

#define 	TIMER0_VALUE		*(volatile uint32_t *)(gpio.BASE_ADDR + 0x118)




void *GPIO_Referesh(void *arg);
void *DNP_Thread(void *arg);


class GPIO{       // The class
	public:

	uint8_t*  	BASE_ADDR=NULL ;
	pthread_t		threads[2];

	result Start()
	{

		//Initialize GPIO
		this->Init();

		//Create a thread for read Digital inputs and write Digital outputs
		pthread_create( &threads[0] , NULL , GPIO_Referesh ,NULL);
		pthread_create( &threads[1] , NULL , DNP_Thread ,NULL);

		return OK;
	}


	result Stop()
	{

		report("s","GPIO --> Closing threads.....  !");

		pthread_cancel(threads[0]);
		pthread_cancel(threads[1]);
		report("s","GPIO --> Stopped  !");

		return OK;
	}


	result Init()
	{
		int mem_fd;

		//Remap System RAM address
		if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
		{
			report("s","GPIO --> can't open /dev/mem !");
			//exit(-1);
		}
		BASE_ADDR = (uint8_t*) mmap(NULL, 1024, PROT_READ | PROT_WRITE,MAP_FILE | MAP_SHARED, mem_fd, 0x10000000);
		if (BASE_ADDR == MAP_FAILED) {
			perror("foo");
			fprintf(stderr, "failed to mmap\n");
			BASE_ADDR = NULL;
			close(mem_fd);
		}
		close(mem_fd);

		//Define processor pins as gpio class
		exec2(GPIO_ADD_IN1);
		exec2(GPIO_ADD_IN2);
		exec2(GPIO_ADD_IN3);
		exec2(GPIO_ADD_IN4);

		//Set Digital inputs as Active low
		exec2(GPIO_ACTLOW_IN1);
		exec2(GPIO_ACTLOW_IN2);
		exec2(GPIO_ACTLOW_IN3);
		exec2(GPIO_ACTLOW_IN4);

		//Set led working mode
		exec2(LED_G232_SHOTEN);
		exec2(LED_R232_SHOTEN);
		exec2(LED_G485_SHOTEN);
		exec2(LED_R485_SHOTEN);
		exec2(LED_SKTG_SHOTEN);
		exec2(LED_SKTR_SHOTEN);

		//Set led on time
		exec2(LED_G232_ONDLY);
		exec2(LED_R232_ONDLY);
		exec2(LED_G485_ONDLY);
		exec2(LED_R485_ONDLY);
		exec2(LED_SKTG_ONDLY);
		exec2(LED_SKTR_ONDLY);

		//Set led off time
		exec2(LED_G232_OFFDLY);
		exec2(LED_R232_OFFDLY);
		exec2(LED_G485_OFFDLY);
		exec2(LED_R485_OFFDLY);
		exec2(LED_SKTG_OFFDLY);
		exec2(LED_SKTR_OFFDLY);

		//Set led working mode
		exec2(LED_STATUS_TIMEREN);
		exec2(LED_STATUS_ONDLY);
		exec2(LED_STATUS_OFFDLY1);


		return OK;
	}

	void Write(uint8_t GPIO,bool value)
	{

		if(value)
		{
			switch(GPIO)
			{
				case GPIO_LED_G232:
					exec2(GPIO_WRITE1_LED_G232);
					break;

				case GPIO_LED_R232:
					exec2(GPIO_WRITE1_LED_R232);
					break;

				case GPIO_LED_G485:
					exec2(GPIO_WRITE1_LED_G485);
					break;

				case GPIO_LED_R485:
					exec2(GPIO_WRITE1_LED_R485);
					break;

				case GPIO_LED_SKTG:
					exec2(GPIO_WRITE1_LED_SKTG);
					break;

				case GPIO_LED_SKTR:
					exec2(GPIO_WRITE1_LED_SKTR);
					break;

				case GPIO_LED_STATUS:
					exec2(GPIO_WRITE1_LED_STATUS);
					break;

				case GPIO_OUT_RELAY1:
					exec2(GPIO_WRITE1_OUT_RELAY1);
					break;

				case GPIO_OUT_RELAY2:
					exec2(GPIO_WRITE1_OUT_RELAY2);
					break;

				case GPIO_OUT_GSMRST:
					exec2(GPIO_WRITE1_OUT_GSMRST);
					break;

				case GPIO_OUT_GSMPWR:
					exec2(GPIO_WRITE1_OUT_GSMPWR);
					break;

				case GPIO_OUT_DIR:
					exec2(GPIO_WRITE1_OUT_DIR);
					break;

				default:
					break;
			}
		}
		else
		{
			switch(GPIO)
			{
				case GPIO_LED_G232:
					exec2(GPIO_WRITE0_LED_G232);
					break;

				case GPIO_LED_R232:
					exec2(GPIO_WRITE0_LED_R232);
					break;

				case GPIO_LED_G485:
					exec2(GPIO_WRITE0_LED_G485);
					break;

				case GPIO_LED_R485:
					exec2(GPIO_WRITE0_LED_R485);
					break;

				case GPIO_LED_SKTG:
					exec2(GPIO_WRITE0_LED_SKTG);
					break;

				case GPIO_LED_SKTR:
					exec2(GPIO_WRITE0_LED_SKTR);
					break;

				case GPIO_LED_STATUS:
					exec2(GPIO_WRITE0_LED_STATUS);
					break;

				case GPIO_OUT_RELAY1:
					exec2(GPIO_WRITE0_OUT_RELAY1);
					break;

				case GPIO_OUT_RELAY2:
					exec2(GPIO_WRITE0_OUT_RELAY2);
					break;

				case GPIO_OUT_GSMRST:
					exec2(GPIO_WRITE0_OUT_GSMRST);
					break;

				case GPIO_OUT_GSMPWR:
					exec2(GPIO_WRITE0_OUT_GSMPWR);
					break;

				case GPIO_OUT_DIR:
					exec2(GPIO_WRITE0_OUT_DIR);
					break;

				default:
					break;
			}
		}

	}

	uint8_t Read(uint8_t pin)
	{
		//uint8_t tmp=0;
		char str[10];
		FILE* fd ;

		//tmp = *(volatile uint32_t *)(BASE_ADDR + GPIO_DATA_0);
		//tmp = (tmp >> pin) & 1u;
		switch(pin)
		{
			case GPIO_IN1:
				//exec(GPIO_READ_IN1, str, 1);
				fd = fopen(GPIO_IN1_FILE, "rw+");
				if(fd) fgets(str,5,fd);
				fclose(fd);
				break;
			case GPIO_IN2:
				//exec(GPIO_READ_IN2, str, 1);
				fd = fopen(GPIO_IN2_FILE, "rw+");
				if(fd) fgets(str,5,fd);
				fclose(fd);
				break;
			case GPIO_IN3:
				//exec(GPIO_READ_IN3, str, 1);
				fd = fopen(GPIO_IN3_FILE, "rw+");
				if(fd) fgets(str,5,fd);;
				fclose(fd);
				break;
			case GPIO_IN4:
				//exec(GPIO_READ_IN4, str, 1);
				fd = fopen(GPIO_IN4_FILE, "rw+");
				if(fd) fgets(str,5,fd);
				fclose(fd);
				break;
			default:
				return 0;
				break;
		}

		if(str[0]=='0') return 0;
		else if(str[0]=='1') return 1;
		else return 0;
		//return tmp;
	}

};





#endif /* GPIO_H_ */
