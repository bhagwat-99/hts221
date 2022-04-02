#include "hts221.h"


int main()
{
        unsigned char reg;
        i2c_init();

        //write configuration register 0x10
        reg = 0x10;
        i2c_write(slave_addr, reg, 0x1B );

        //write configuration register 0x20
        reg = 0x20;
        i2c_write(slave_addr, reg, 0x85 );

        //Read 1 byte of data from address(0x30)
        reg = 0x30;
        __uint8_t H0 = i2c_read_1Byte(slave_addr, reg);
        H0 = H0 /2;

        //Read 1 byte of data from address(0x31)
        reg = 0x31;
        __uint8_t H1 = i2c_read_1Byte(slave_addr, reg);
        H1 = H1 /2;

        
        //Read 2 byte of data from address(0x36 and 0x37)
        reg = 0x36 | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t H2 =  i2c_read_2Byte(slave_addr, reg);

        //Read 2 byte of data from address(0x3a and 0x3b)
        reg = 0x3a | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t H3 =  i2c_read_2Byte(slave_addr, reg);

        //Read 1 byte of data from address(0x32)
        reg = 0x32;
        __uint16_t T0 = i2c_read_1Byte(slave_addr, reg);

        //Read 1 byte of data from address(0x33)
        reg = 0x33;
        __uint16_t T1 = i2c_read_1Byte(slave_addr, reg);

        //Read 1 byte of data from address(0x35)
        reg = 0x35;
        __uint16_t raw = i2c_read_1Byte(slave_addr, reg);

        // Convert the temperature Calibration values to 10-bits
	T0 = ((raw & 0x03) * 256) + T0;
	T1 = ((raw & 0x0C) * 64) + T1;

        //Read 2 byte of data from address(0x3C and 0x3D)
        reg = 0x3C | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t T2 =  i2c_read_2Byte(slave_addr, reg);

        //Read 2 byte of data from address(0x3E and 0x3F)
        reg = 0x3E | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t T3 =  i2c_read_2Byte(slave_addr, reg);

        //Read 2 byte of data from address(0x28 and 0x29)
        reg = 0x28 | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t hum =  i2c_read_2Byte(slave_addr, reg);

        //Read 2 byte of data from address(0x2A and 0x2B)
        reg = 0x2A | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t temp =  i2c_read_2Byte(slave_addr, reg);
        
        if(temp > 32767)
        {
                temp -= 65536;
        }

        float humidity = ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
        if( humidity > 100.0)
	{
		EnableHeater();
                sleep(5);
                DisableHeater();
                sleep(1)
	}
        float cTemp = ((T1 - T0) / 8.0) * (temp - T2) / (T3 - T2) + (T0 / 8.0);
        float fTemp = (cTemp * 1.8 ) + 32;
    
        printf("Relative humidity : %.2f % \n", humidity);
        printf("Temperature in Celsius : %.2f C \n", cTemp);
        printf("Temperature in Fahrenheit : %.2f F \n", fTemp);

        // if( humidity > 100.0)
	// {
	// 	EnableHeater();
        //         sleep(5);
        //         DisableHeater();
        //         sleep(1);
	// }

        i2c_close();
        return 0;
}
