
/*      @author         Bhagwat Shinde
        name            main.c
        Description     
        date            30 April 2022

        compilation     gcc main.c hts221.c I2C.c -o TH_reading

        execution       ./TH_reading

        reading Temperature and humidity data - cat /tmp/ambient_data

        Note            change the i2c_bus (i2c adapter as per your board)
*/

#include "hts221.h"
#include "I2C.h"

// for apalis imx8 
char *i2c_bus = "/dev/apalis-i2c1";

//for colibri imx6ulL
//char *i2c_bus = "/dev/i2c-0";


int main()
{
        //initilize the i2c bus
        if (i2c_init(i2c_bus) < 0)
        {
                printf("Failed to open %s \n",i2c_bus);
                return -1;
        }

        //configuring the sensor
        if (configure_sensor() < 0)
        {
                return -1;
        }

        // reading the calibration data - read only once
        read_calibration_data();

        // writing humidity and temperature data to ram file i.e. /tmp/ambient_data
        write_to_file();

        //close the i2c bus
        i2c_close(i2c_bus);
        return 0;
}
