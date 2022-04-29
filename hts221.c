#include "hts221.h"

int fd_i2c = -1; // i2c bus file descriptor
const char *i2c_bus = "/dev/apalis-i2c1";

// Read the given I2C slave device's register and return the read value in `*result`:
__uint16_t i2c_read_2Byte(unsigned char slave_address, unsigned char reg) 
{
        unsigned char outbuf[1];
        unsigned char inbuf[2];

        outbuf[0]= reg;
        inbuf[0] = 0;
        inbuf[1] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = slave_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                perror("ioctl(I2C_RDWR) in i2c_read");
                return -1;
        }

        __uint16_t reg_value = inbuf[1]*256 + inbuf[0] ;
        return reg_value;
}

__uint8_t i2c_read_1Byte(unsigned char slave_address, unsigned char reg) 
{
        unsigned char outbuf[1];
        unsigned char inbuf[1];

        outbuf[0]= reg;
        inbuf[0] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = slave_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                perror("ioctl(I2C_RDWR) in i2c_read");
                return -1;
        }

        __uint8_t reg_value = inbuf[0] ;
        return reg_value;
}


int i2c_init(void)
{
    if ((fd_i2c = open(i2c_bus, O_RDWR)) < 0)
    {
        printf("Failed to open apalis-i2c1.\n");
        return -1;
    }
    return fd_i2c;
}

void i2c_close(void) 
{
    close(fd_i2c);
}

// Write to an I2C slave device's register:
int i2c_write(unsigned char slave_address, unsigned char reg, unsigned char byte )
{
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    unsigned char outbuf[2];

    outbuf[0] = reg;
    outbuf[1] = byte;

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;// 0 for write 
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
        perror("ioctl(I2C_RDWR) in i2c_write");
        return -1;
    }

    return 0;
}

int EnableHeater()
{
        //Read 1 byte of data from address(0x21)
        unsigned char reg = 0x21;
        __uint8_t reg_value = i2c_read_1Byte(slave_addr, reg);
        reg_value |= 0x02 ;
        i2c_write(slave_addr, reg, reg_value );
        printf("Heater enabled.\n");
        return 0;

}
int DisableHeater()
{
        //Read 1 byte of data from address(0x21)
        unsigned char reg = 0x21;
        __uint8_t reg_value = i2c_read_1Byte(slave_addr, reg);
        reg_value &= ~(0x02);
        i2c_write(slave_addr, reg, reg_value );
        printf("Disabled.\n");
        return 0;
}



