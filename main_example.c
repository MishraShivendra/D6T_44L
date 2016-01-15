/*
Author: Shivendra Mishra
Usage: The example shows the way to use library functions
*/



#define F_CPU 14745600UL
#define BAUD 115200
#define START_VALUE 0b11100000
#define CH_0 0x01
#define CH_1 0x02
#define BYTES_TO_RECEIVE 33
#define TOTAL_PIXCEL 35
#define D6T_COMMAND 0x4C
#include "i2cmaster.h"
#include "global_vars.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/setbaud.h>

unsigned char deta[TOTAL_PIXCEL];
void pca_ch0()						// This function reads PCA9545A channel 0 
{
	unsigned char st,sts;
	int i=0;
	sts = hw_i2c_rep_start(START_VALUE);
	if (sts)
	{
		PORTF |= 0xff;				// Show error - For debug purpose
	}
	hw_i2c_write(CH_0);
	hw_i2c_stop();
	sts = hw_i2c_rep_start(sensor_write);
	if (sts)
	{
		PORTF |= 0xff;
	}
	hw_i2c_write(D6T_COMMAND);
	hw_i2c_rep_start(sensor_read);
	for (i=0;i<=33;i++)
	{
		deta[i] = hw_i2c_readAck();
		fire_trns(deta[i]); 
	}
	deta[BYTES_TO_RECEIVE+1] = hw_i2c_readNak();
	fire_trns(deta[BYTES_TO_RECEIVE+1]);
	hw_i2c_stop();
}
void pca_ch1()						// This function reads PCA9545A Channel 1 
							// (assuming D6T-44L sensor is connected) 
{
	unsigned char st,sts,deta[35];
	int i;
	sts = hw_i2c_rep_start(START_VALUE);
	if (sts)
	{
		PORTF |= 0xff;
	}
	hw_i2c_write(CH_1);
	hw_i2c_stop();
	st = hw_i2c_rep_start(sensor_write);
	st = hw_i2c_write(D6T_COMMAND);
	hw_i2c_rep_start(sensor_read);
	for (i=0;i<=BYTES_TO_RECEIVE;i++)
	{
		deta[i] = hw_i2c_readAck();
		fire_trns(deta[i]); 
	}
	deta[BYTES_TO_RECEIVE+1] = hw_i2c_readNak();
	fire_trns(deta[BYTES_TO_RECEIVE+1]);
	hw_i2c_stop();
}
void read_only_d6t_44l()
{
	hw_i2c_rep_start(sensor_write);
	hw_i2c_write(D6T_COMMAND);
	hw_i2c_rep_start(sensor_read);
	for (i=0;i<=BYTES_TO_RECEIVE;i++)
	{ 
		deta[i] = hw_i2c_readAck();
	}
	deta[BYTES_TO_RECEIVE+1] = hw_i2c_readNak();
	hw_i2c_stop();
}


int main(void)
{
	hw_i2c_init();		// intialize twi
	pca_ch();		// This function will write 35 byte of data into deta[35] (from ch_0)
	pca_ch1();		// This function will write 35 byte of data into deta[35] (from ch_1)
	read_only_d6t_44l();	//This function can be used if PCA9545A is not used, It will write 35 byte of data into deta[35]
	return 0; 
}
