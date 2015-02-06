/*
 * main.c
 *
 *  Created on: Feb 6, 2015
 *      Author: joao
 */
#include "mpsse.h"
#include "stdint.h"
#include "mcp9808.h"
#include "stdio.h"
#include <signal.h>

uint8_t addr = 0x30;
struct mpsse_context *mcp = NULL;
FILE *f;
int do_log = 0;

uint16_t read16(uint8_t reg) {
	uint16_t val = 0;
	char *data = NULL;
	data = malloc(4);
	uint8_t bytes[3];
	bytes[0] = 0x30;
	bytes[1] = reg;
	Start(mcp);
	Write(mcp, (char *)bytes, 2);
	if(GetAck(mcp) == ACK)
	{
		Stop(mcp);
		Start(mcp);
		bytes[0] = 0x31;
		Write(mcp, (char *)bytes, 1);
		if(GetAck(mcp) == ACK)
		{
			SendAcks(mcp);
			data = Read(mcp, 2);

			SendNacks(mcp);
			Read(mcp, 1);
			if(data)
			{

			}

		}
		/* Tell libmpsse to send NACKs after reading data */


	}
	Stop(mcp);

	val = ((uint8_t)data[0] << 8) | (uint8_t)data[1];

	return val;
}

float readTempC( void )
{


	uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);
	//printf("temp = %x ºC\n",t);
	float temp = t & 0x0FFF;
	temp /=  16.0;
	if (t & 0x1000) temp -= 256;

	return temp;
}

int int_ctrlc(int a)
{
	if(do_log)
	{
		fclose(f);
	}
	Close(mcp);
	exit(0);
}

int main(int argc, char **argv) {

	int i_sec;

	signal(SIGINT, int_ctrlc);


	if(argc == 2)
	{
		f = fopen(argv[1], "w");
		do_log = 1;
	}

	if(do_log)
	{
		if (f == NULL)
		{

			printf("Error opening file!\n");
			exit(1);
		}
	}

	if((mcp = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)) != NULL && mcp->open)
	{

		printf("%s initialized at %dHz (I2C)\n", GetDescription(mcp), GetClock(mcp));
		int a;
		a = read16(MCP9808_REG_MANUF_ID);printf("06 = %x\n",a);
		a = read16(MCP9808_REG_MANUF_ID);printf("07 = %x\n",a);

		while(1)
		{
			i_sec++;
			usleep(1000000);
			float temp = readTempC();
			printf("Temp = %f ºC, %d:%d.%d\n",temp, i_sec / 3600, i_sec/60, i_sec%60);
			if(do_log)
			{
				fprintf(f, "Temp = %f ºC, %d:%d.%d\n",temp, i_sec / 3600, i_sec/60, i_sec%60);
			}
		}
	}
}
