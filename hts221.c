// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HTS221
// This code is designed to work with the HTS221_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=HTS221_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

void main() 
{
	// // Create I2C bus
	// int file;
	// char *bus = "/dev/i2c-1";
	// if((file = open(bus, O_RDWR)) < 0) 
	// {
	// 	printf("Failed to open the bus. \n");
	// 	exit(1);
	// }
	// // Get I2C device, HTS221 I2C address is 0x5F(95)
	// ioctl(file, I2C_SLAVE, 0x5F);

	// // Select average configuration register(0x10)
	// // Temperature average samples = 16, humidity average samples = 32(0x1B)
	// char config[2] = {0};
	// config[0] = 0x10;
	// config[1] = 0x1B;
	// write(file, config, 2);
	// sleep(1);
	// // Select control register1(0x20)
	// // Power on, block data update, data rate o/p = 1 Hz(0x85)
	// config[0] = 0x20;
	// config[1] = 0x85;
	// write(file, config, 2);
	// sleep(1);

	// Read Calibration values from the non-volatile memory of the device
	// Humidity Calibration values
	//Read 1 byte of data from address(0x30)
	unsigned char reg[1] = {0x30};
	//write(file, reg, 1);
	unsigned char data[1] = {0};
	// if(read(file, data, 1) != 1)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	unsigned char data_0 = 0x3c;//data[0];
	// Read 1 byte of data from address(0x31)
	// reg[0] = 0x31;
	// write(file, reg, 1);
	// read(file, data, 1);
	unsigned char data_1 = 0x8b;//data[0];

	__uint8_t H0 = data_0/2;
	__uint8_t H1 = data_1/2;
	 
	//  H0 = H0 / 2 ;
	//  H1 = H1 / 2 ;

	printf("H0 = %d\n",H0);
	printf("H1 = %d\n",H1);

	
	//Read 1 byte of data from address(0x36)
	// reg[0] = 0x36;
	// write(file, reg, 1);
	// if(read(file, data, 1) != 1)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	data_0 = 0xf7;//data[0];
	// Read 1 byte of data from address(0x37)
	// reg[0] = 0x37;
	// write(file, reg, 1);
	// read(file, data, 1);
	data_1 = 0xff;//data[0];

	__int16_t H2 = (data_1 * 256) + data_0;

	
	
	// //Read 1 byte of data from address(0x3A)
	// reg[0] = 0x3A;
	// write(file, reg, 1);
	// if(read(file, data, 1) != 1)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	data_0 = 0xd1;//data[0];
	// Read 1 byte of data from address(0x3B)
	// reg[0] = 0x3B;
	// write(file, reg, 1);
	// read(file, data, 1);
	data_1 = 0xd5;//data[0];

	__int16_t H3 = (data_1 * 256) + data_0;
	
	// Temperature Calibration values
	// Read 1 byte of data from address(0x32)
	// reg[0] = 0x32;
	// write(file, reg, 1);
	// read(file, data, 1);
	
	__uint16_t T0 = 0xa7;//data[0];
	
	// // Read 1 byte of data from address(0x33)
	// reg[0] = 0x33;
	// write(file, reg, 1);
	// read(file, data, 1);
	
	__uint16_t T1 = 0x11;//data[0];
	
	// Read 1 byte of data from address(0x35)
	// reg[0] = 0x35;
	// write(file, reg, 1);
	// read(file, data, 1);

	data[0]=0xc4;
	
	__uint16_t raw = data[0];
	
	// Convert the temperature Calibration values to 10-bits
	T0 = ((raw & 0x03) * 256) + T0;
	T1 = ((raw & 0x0C) * 64) + T1;
	
	printf("T0 = %d\n",T0);
	printf("T1 = %d\n",T1);
	//Read 1 byte of data from address(0x3C)
	// reg[0] = 0x3C;
	// write(file, reg, 1);
	// if(read(file, data, 1) != 1)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	data_0 = 0x00;//data[0];
	// Read 1 byte of data from address(0x3D)
	// reg[0] = 0x3D;
	// write(file, reg, 1);
	// read(file, data, 1);
	data_1 = 0xff;//data[0];

	__int16_t T2 = (data_1 * 256) + data_0;

	printf("T2 = %d\n",T2);
	
	// //Read 1 byte of data from address(0x3E)
	// reg[0] = 0x3E;
	// write(file, reg, 1);
	// if(read(file, data, 1) != 1)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	data_0 = 0xe2;//data[0];
	// Read 1 byte of data from address(0x3F)
	// reg[0] = 0x3F;
	// write(file, reg, 1);
	// read(file, data, 1);
	data_1 = 0x02;//data[0];

	__int16_t T3 = (data_1 * 256) + data_0;

	printf("T3 = %d\n",T3);


	//0x28
	data_0 = 0x99;
	//0x29
	data_1 = 0xea;

	__int16_t hum = (data_1 * 256) + data_0;

	//0x2a
	data_0 = 0xfe;
	//0x2b
	data_1 = 0x00;

	__int16_t temp = data_1 * 256 + data_0;
	
	printf("Temp = %d\n",temp);



	if(temp > 32767)
	{
		temp -= 65536;
	}
	float humidity = ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
	float cTemp = ((T1 - T0) / 8.0) * (temp - T2) / (T3 - T2) + (T0 / 8.0);
	float fTemp = (cTemp * 1.8 ) + 32;
	
	// Output data to screen
	printf("Relative humidity : %.2f Humidity \n", humidity);
	printf("Temperature in Celsius : %.2f C \n", cTemp);
	printf("Temperature in Fahrenheit : %.2f F \n", fTemp);


	
	// Read 4 bytes of data(0x28 | 0x80)
	// hum msb, hum lsb, temp msb, temp lsb
	// reg[0] = 0x28 | 0x80;
	// write(file, reg, 1);
	// //char data[4] = {0};
	// if(read(file, data, 4) != 4)
	// {
	// 	printf("Erorr : Input/output Erorr \n");
	// 	exit(1);
	// }
	// else
	// {
	// 	// Convert the data
	// 	int hum = (data[1] * 256) + data[0]; 
	// 	int temp = (data[3] * 256) + data[2];
	// 	if(temp > 32767)
	// 	{
	// 		temp -= 65536;
	// 	}
	// 	float humidity = ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
	// 	float cTemp = ((T1 - T0) / 8.0) * (temp - T2) / (T3 - T2) + (T0 / 8.0);
	// 	float fTemp = (cTemp * 1.8 ) + 32;
		
	// 	// Output data to screen
	// 	printf("Relative humidity : %.2f Humidity \n", humidity);
	// 	printf("Temperature in Celsius : %.2f C \n", cTemp);
	// 	printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	// }
}
