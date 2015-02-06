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

uint8_t addr = 0x18;
struct mpsse_context *mcp = NULL;
char *data = NULL;

uint16_t read16(uint8_t reg) {
	uint16_t val = 0;

	uint8_t bytes[3];
	bytes[0] = addr+1;
	bytes[1] = reg;
	Write(mcp, (char *)bytes, 2);

	if(GetAck(mcp) == ACK)
	{
		data = Read(mcp, 2);
		if(data)
		{

		}
		/* Tell libmpsse to send NACKs after reading data */
		SendNacks(mcp);

		/* Read in one dummy byte, with a NACK */
		Read(mcp, 1);
	}


	val = (bytes[0] << 8) | bytes[1];

	return val;
}

float readTempC( void )
{


	uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

	float temp = t & 0x0FFF;
	temp /=  16.0;
	if (t & 0x1000) temp -= 256;

	return temp;
}


int main(int argc, char **argv) {


	if((mcp = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)) != NULL && mcp->open)
	{
		Start(mcp);
		printf("%s initialized at %dHz (I2C)\n", GetDescription(mcp), GetClock(mcp));
		int a;
		a = read16(MCP9808_REG_MANUF_ID);

		readTempC();
	}
	Stop(mcp);
	Close(mcp);
}
