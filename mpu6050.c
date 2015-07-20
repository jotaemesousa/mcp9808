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

uint8_t addr = 0xD0;
struct mpsse_context *mcp = NULL;
FILE *f;
int do_log = 0;

void write8(uint8_t reg, uint8_t data)
{
    uint16_t val = 0;
	uint8_t bytes[3];
	bytes[0] = addr;
	bytes[1] = reg;
        bytes[2] = data;
	Start(mcp);
	Write(mcp, (char *)bytes, 3);
	if(GetAck(mcp) == ACK)
	{
		Stop(mcp);
	}
	Stop(mcp);
    
}

uint8_t read8(uint8_t reg)
{
    uint8_t val = 0;
	char *data = NULL;
	data = malloc(4);
	uint8_t bytes[3];
	bytes[0] = addr;
	bytes[1] = reg;
	Start(mcp);
	Write(mcp, (char *)bytes, 2);
	if(GetAck(mcp) == ACK)
	{
		Stop(mcp);
		Start(mcp);
		bytes[0] = addr+1;
		Write(mcp, (char *)bytes, 1);
		if(GetAck(mcp) == ACK)
		{
			SendAcks(mcp);
			data = Read(mcp, 1);

			SendNacks(mcp);
			Read(mcp, 1);
			if(data)
			{

			}

		}
		/* Tell libmpsse to send NACKs after reading data */


	}
	Stop(mcp);

        val = data[0];
	return val;
    
}

uint16_t read16(uint8_t reg) {
	uint16_t val = 0;
	char *data = NULL;
	data = malloc(4);
	uint8_t bytes[3];
	bytes[0] = addr;
	bytes[1] = reg;
	Start(mcp);
	Write(mcp, (char *)bytes, 2);
	if(GetAck(mcp) == ACK)
	{
		Stop(mcp);
		Start(mcp);
		bytes[0] = addr+1;
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

void int_ctrlc(int a)
{
	if(do_log)
	{
		fclose(f);
	}
	Close(mcp);
	exit(0);
}

int main(int argc, char **argv) {

	int i_sec = 0;

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
		write8(0x6B, 0);
                
                printf("power mngt %d\n", read8(0x6B));
                

		while(1)
		{
			i_sec++;
			usleep(20000);
			printf("read ACC x=%06d, y=%06d, z=%06d\n", (int16_t)read16(0x3B),(int16_t)read16(0x3D),(int16_t)read16(0x3F));
		}
	}
	
	Close(mcp);
	return 0;
}
