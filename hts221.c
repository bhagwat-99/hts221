/*      @author         Bhagwat Shinde
        name            hts221.c
        Description     Library for interfacing hts221 light and humidity sensor
        date            30 April 2022

        How to use      include hts221.h file in your mainn code and you are ready to use fuctions
        compilation     gcc main.c hts221.c I2C.c -o output

        Note            change the i2c_bus (i2c adapter as per your board)
*/
#include "hts221.h"
#include "I2C.h"



// calibration parameters

//fixed value - read only once

uint8_t H0;             //0x30
uint8_t H1;             //0x31

int16_t H2;             //0x36 & 0x37
int16_t H3;             //0x3a & 0x3b

uint16_t T0;            //0x32
uint16_t T1;            //0x33

uint16_t raw;            //0x35

int16_t T2;              //0x3c & 0x3d
int16_t T3;              //0x3e & 0x3f

// this two values get updated 

int16_t hum;             //0x28 & 0x29
int16_t temp;            //0x2a & 0x2b


//configuring the sensor
int configure_sensor()
{
        uint8_t reg_addr = 0x10;
        uint8_t reg_value[2];
        reg_value[0] = 0x1b;
        uint8_t n_bytes = 1;
        uint8_t ret_value = i2c_write(SLAVE_ADDR, reg_addr,reg_value,n_bytes);
        if(ret_value < 0)
        {
                printf("configuring sensor failed\n");
                return -1;
        }

        reg_addr = 0x20;
        reg_value[0] = 0x85;
        n_bytes = 1;
        ret_value = i2c_write(SLAVE_ADDR, reg_addr,reg_value,n_bytes);
        if(ret_value < 0)
        {
                printf("configuring sensor failed\n");
                return -1;
        }

        return 0;
}


// read calibration data only once
int read_calibration_data()
{
        uint8_t reg_addr = 0x30;
        uint8_t reg_value[2]={0};
        uint8_t n_bytes = 1;
        uint8_t * p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);
        if(p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }


        H0 = *(p_ret_data);
        H0 = H0/2;

        reg_addr = 0x31;
        n_bytes = 1;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }

        H1 = *(p_ret_data);
        H1 = H1/2;



        reg_addr = 0x36 | 0x80;// | 0x80 auto increment reg value
        n_bytes = 2;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }
        
        H2 = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;




        reg_addr = 0x3a | 0x80;// | 0x80 auto increment reg value
        n_bytes = 2;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }
        
        H3 = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;



        reg_addr = 0x32;
        n_bytes = 1;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }
       
        T0 = *(p_ret_data);



        reg_addr = 0x33;
        n_bytes = 1;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }
        
        T1 = *(p_ret_data);

        reg_addr = 0x35;
        n_bytes = 1;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }
       
        raw = *(p_ret_data);

        // Convert the temperature Calibration values to 10-bits
        T0 = ((raw & 0x03) * 256) + T0;
        T1 = ((raw & 0x0C) * 64) + T1;



        reg_addr = 0x3c | 0x80;// | 0x80 auto increment reg value
        n_bytes = 2;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }

        T2 = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;


        reg_addr = 0x3e | 0x80;// | 0x80 auto increment reg value
        n_bytes = 2;
        p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("configuring sensor failed\n");
                return -1;
        }

        T3 = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;


        return 0;        
}

//read temperature data
float read_temperature()
{
        uint8_t reg_addr = 0x2a | 0x80;// | 0x80 auto increment reg value
        uint8_t n_bytes = 2;
        uint8_t * p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("read temperature failed\n");
                return -1;
        }

        temp = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;

        float cTemp = ((T1 - T0) / 8.0) * (temp - T2) / (T3 - T2) + (T0 / 8.0);
        //float fTemp = (cTemp * 1.8 ) + 32;
        return cTemp;

}

//read humidity value
float read_humidity()
{
        uint8_t reg_addr = 0x28 | 0x80;// | 0x80 auto increment reg value
        uint8_t n_bytes = 2;
        uint8_t * p_ret_data = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);

        if( p_ret_data == NULL)
        {
                printf("read humidity failed\n");
                return -1;
        }


        hum = (uint16_t)(*(p_ret_data+1)) << 8 | *p_ret_data ;

        float humidity = ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
        return humidity;
}

// enable internal heater for humidity saturation
int EnableHeater()
{
        //Read 1 byte of data from address(0x21)
        uint8_t reg_addr = 0x21;
        uint8_t n_bytes = 1;
        uint8_t * p_ret_val = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);
        if( p_ret_val == NULL)
        {
                printf("Can not enable heater\n");
                return -1;
        }

        uint8_t reg_data[2];
        reg_data[0] = *p_ret_val | 0x02;
        int ret_val = i2c_write(SLAVE_ADDR, reg_addr,reg_data, n_bytes);
        if(ret_val < 0 )
        {
                printf("Can not enable heater\n");
                return -1;
        }
        printf("Heater enabled.\n");
        return 0;

}

// Heater disable function
int DisableHeater()
{
        //Read 1 byte of data from address(0x21)
        uint8_t reg_addr = 0x21;
        uint8_t n_bytes = 1;
        uint8_t * p_ret_val = i2c_read(SLAVE_ADDR, reg_addr,n_bytes);
        if( p_ret_val == NULL)
        {
                printf("Can not disable heater\n");
                return -1;
        }

        uint8_t reg_data[2];
        reg_data[0] = *p_ret_val & ~(0x02);
        i2c_write(SLAVE_ADDR, reg_addr,reg_data, n_bytes);
        int ret_val = i2c_write(SLAVE_ADDR, reg_addr,reg_data, n_bytes);
        if(ret_val < 0 )
        {
                printf("Can not disable heater\n");
                return -1;
        }
        printf("Heater Disabled.\n");
        return 0;
}

int write_to_file()
{

        FILE *fptr;
        while(1)
        {
        // reading humidity value
        float humidity = read_humidity();
        if(humidity <0)
        {
                printf("error reading humidity \n");
                return -1;
        }
        if(humidity > 100)
        {
                humidity = 100;
        }

        while(humidity > 95)
        {
        //enable heater
        int ret_val = EnableHeater();
        if(ret_val < 0)
        {
                printf("Failed to enable heater\n");
        }
        sleep(30);
        // disabling the heater
        //DisableHeater();
        ret_val = DisableHeater();
        if(ret_val < 0)
        {
                printf("Failed to disable heater\n");
        }
        humidity = read_humidity();

        }

        // reading temperature data
        float cTemp = read_temperature();
        float fTemp = (cTemp * 1.8 ) + 32;

        //opening the file
        fptr = fopen("/tmp/ambient_data","w");
        if( fptr == NULL)
        {
                printf("Can not open file /tmp/ambient_data\n");
                return -1;
        }



        //writing Relative humidity to file
        if(fprintf(fptr,"Relative Humidity : %.2f %%\n",humidity )<0)
        {
                printf("error writing relative humidity to file \n");
                return -1;     
        }



        //writing Temperature to file
        if(fprintf(fptr,"Temperature in C: %.2f C\n",cTemp )<0)
        {
                printf("error writing temperature to file \n");
                return -1;     
        }

        //writing Temperature to file
        if(fprintf(fptr,"Temperature in F: %.2f F\n",fTemp )<0)
        {
                printf("error writing temperature to file \n");   
                return -1;  
        }


        if( fclose(fptr) == EOF)
        {
                printf("Can not close file /tmp/ambient_data\n");
                return -1;
        }
        sleep(10);

        }
        return 0;
}

