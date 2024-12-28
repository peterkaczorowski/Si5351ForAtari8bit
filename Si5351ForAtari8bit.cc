/*
 * SI5351 Configurations for Selected Frequencies Found in Atari 8-bit Computers
 *
 * Author: Piotr D. Kaczorowski
 * Organization: THEATARIAN.COM
 * Date: December 27, 2024
 * Version: 1.0
 *
 * Description:
 *   This program configures the SI5351 clock generator via I2C to produce
 *   custom frequencies tailored for various Atari system configurations.
 *   Frequencies generated include:
 *   - 1.773447 MHz (Atari XL/XE PAL)
 *   - 1.7897725 MHz (Atari XL/XE NTSC)
 *   - 1.7897875 MHz (Atari 400/800 NTSC)
 *   - 3.546894 MHz (Atari XL/XE PAL)
 *   - 3.579545 MHz (Atari XL/XE NTSC)
 *   - 3.579575 MHz (Atari 400/800 NTSC)
 *   - 4.433618 MHz (Atari PAL Crystal)
 *   - 8.3333 MHz (Atari XF551 Crystal)
 *   - 14.187576 MHz (Atari XL/XE PAL)
 *   - 14.31818 MHz (Atari XL/XE NTSC)
 *
 * Change History:
 *   Version 1.1 - replaced WiringPi library with functions from linux/i2c-dev.h
 *   Version 1.0 - Initial release
 *
 */

#include <fcntl.h>
#include <iostream> 
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SI5351_ADDRESS 0x60

#define SI5351_REGISTER_0_DEVICE_STATUS 0
#define SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL 3

#define SI5351_REGISTER_16_CLK0_CONTROL 16
#define SI5351_REGISTER_17_CLK1_CONTROL 17
#define SI5351_REGISTER_18_CLK2_CONTROL 18
#define SI5351_REGISTER_19_CLK3_CONTROL 19
#define SI5351_REGISTER_20_CLK4_CONTROL 20
#define SI5351_REGISTER_21_CLK5_CONTROL 21
#define SI5351_REGISTER_22_CLK6_CONTROL 22
#define SI5351_REGISTER_23_CLK7_CONTROL 23
#define SI5351_REGISTER_24_CLK3_0_DISABLE_STATE 24
#define SI5351_REGISTER_25_CLK7_4_DISABLE_STATE 25

#define SI5351_REGISTER_26_PLL_A_REG0 26
#define SI5351_REGISTER_27_PLL_A_REG1 27
#define SI5351_REGISTER_28_PLL_A_REG2 28
#define SI5351_REGISTER_29_PLL_A_REG3 29
#define SI5351_REGISTER_30_PLL_A_REG4 30
#define SI5351_REGISTER_31_PLL_A_REG5 31
#define SI5351_REGISTER_32_PLL_A_REG6 32
#define SI5351_REGISTER_33_PLL_A_REG7 33

#define SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1 42
#define SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2 43
#define SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3 44
#define SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4 45
#define SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5 46
#define SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6 47
#define SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7 48
#define SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8 49

#define SI5351_REGISTER_177_PLL_RESET 177


#define I2C_DEVICE "/dev/i2c-1"



/**
 * Initialize the I2C bus and set the target device address.
 *
 * @param deviceAddress The address of the I2C device.
 * @return File descriptor for the I2C bus, or -1 on failure.
 */
int wiringPiI2CSetup(int deviceAddress) {
    const char *device = I2C_DEVICE;
    int file;

    // Open the I2C bus
    if ((file = open(device, O_RDWR)) < 0) {
        perror("Failed to open the I2C bus");
        return -1;
    }

    // Set the I2C slave address
    if (ioctl(file, I2C_SLAVE, deviceAddress) < 0) {
        perror("Failed to set I2C slave address");
        close(file);
        return -1;
    }

    return file;
}

/**
 * Write an 8-bit value to a specific register on the I2C device.
 *
 * @param file File descriptor of the I2C bus (returned by wiringPiI2CSetup).
 * @param reg The register address to write to.
 * @param value The 8-bit value to write.
 * @return 0 on success, -1 on failure.
 */
int wiringPiI2CWriteReg8(int file, uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = reg;    // Register address
    buffer[1] = value;  // Value to write

    // Write the register address and value to the I2C device
    if (write(file, buffer, 2) != 2) {
        perror("Failed to write to I2C device");
        return -1;
    }

    return 0;
}





void writeRegister(int fd, int reg, int value) {
    if (wiringPiI2CWriteReg8(fd, reg, value) == -1) {
        std::cerr << "Regiter write error." << reg << std::endl;
    }
}

int main() {

    // I2C bus initialization
    int fd = wiringPiI2CSetup(SI5351_ADDRESS);
    if (fd == -1) {
        std::cerr << "I2C initialization error." << std::endl;
        return 1;
    }


    // Disable Outputs
    writeRegister(fd, SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0xFF);

    // Powerup only output #0
    writeRegister(fd, SI5351_REGISTER_16_CLK0_CONTROL, 0x4F);
    writeRegister(fd, SI5351_REGISTER_17_CLK1_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_18_CLK2_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_19_CLK3_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_20_CLK4_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_21_CLK5_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_22_CLK6_CONTROL, 0x80);
    writeRegister(fd, SI5351_REGISTER_23_CLK7_CONTROL, 0x80);

    writeRegister(fd, SI5351_REGISTER_24_CLK3_0_DISABLE_STATE, 0x00);
    writeRegister(fd, SI5351_REGISTER_25_CLK7_4_DISABLE_STATE, 0x00);

    // Test frequency: 10MHz
    //

    //  PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  800,000000000
    //  Feedback Divider = 32
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 10,000000000
    //  Multisynth Output Frequency (MHz) = 10,000000000
    //  Multisynth Divider = 80
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x00);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x01);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0e);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x00);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x00);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x26);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI PAL: 1.773447000 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  698,738100000
    //  Feedback Divider = 27  237381/250000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 1,773447000
    //  Multisynth Output Frequency (MHz) = 1,773447000
    //  Multisynth Divider = 394
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0xd0);
    writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x90);
    writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0b);
    writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0xf9);
    writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x32);
    writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x0e);
    writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x70);

    writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0xc3);
    writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI PAL: 3.546894 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  702,285000000
    //  Feedback Divider = 28  457/5000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 3,546894000
    //  Multisynth Output Frequency (MHz) = 3,546894000
    //  Multisynth Divider = 198
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x13);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x88);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x0b);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x0d);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0xa8);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x61);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI PAL: 14.187576 MHz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  709,378800000
    //  Feedback Divider = 28  23447/62500
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 14,187576000
    //  Multisynth Output Frequency (MHz) = 14,187576000
    //  Multisynth Divider = 50
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0xf4);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x24);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x30);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x04);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0xc0);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x17);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI 400/800 NTSC: 1.7897875 MHz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  698,017125000
    //  Feedback Divider = 27  184137/200000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 1,789787500
    //  Multisynth Output Frequency (MHz) = 1,789787500
    //  Multisynth Divider = 390
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x0d);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x40);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0b);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0xf5);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x32);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x96);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x40);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0xc1);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI 400/800 NTSC: 3.579575 MHz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  701,596700000
    //  Feedback Divider = 28  15967/250000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 3,579575000
    //  Multisynth Output Frequency (MHz) = 3,579575000
    //  Multisynth Divider = 196
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0xd0);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x90);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x08);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x30);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0xab);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x00);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x60);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI XL/XE NTSC: 1.7897725 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  698,011275000
    //  Feedback Divider = 27  920451/1000000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 1,789772500
    //  Multisynth Output Frequency (MHz) = 1,789772500
    //  Multisynth Divider = 390
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x42);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x40);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0b);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0xf5);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0xfc);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x7a);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x40);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0xc1);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI XL/XE NTSC: 3.579545 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  701,590800000
    //  Feedback Divider = 28  3977/62500
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 3,579545000
    //  Multisynth Output Frequency (MHz) = 3,579545000
    //  Multisynth Divider = 196
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0xf4);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x24);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x08);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x23);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x60);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x60);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI XL/XE NTSC: 14.31818 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  687,272640000
    //  Feedback Divider = 27  38352/78125
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 14,318180000
    //  Multisynth Output Frequency (MHz) = 14,318180000
    //  Multisynth Divider = 48
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x31);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x2d);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0b);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0xbe);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x10);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0xff);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x1a);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x16);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI PAL crystal: 4.433618 MHz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  700,511640000
    //  Feedback Divider = 28  12791/625000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 4,433618000
    //  Multisynth Output Frequency (MHz) = 4,433618000
    //  Multisynth Divider = 158
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0x89);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x68);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x02);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x95);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0xe8);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0xb0);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x4d);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // ATARI XF551 crystal: 8.3333 Mhz
    //

    // PLL A
    //  Input Frequency (MHz) = 25,000000000
    //  VCO Frequency (MHz) =  716,663800000
    //  Feedback Divider = 28  83319/125000
    //  SSC disabled
    //
    // Output Clocks
    // Channel 0
    //  Output Frequency (MHz) = 8,333300000
    //  Multisynth Output Frequency (MHz) = 8,333300000
    //  Multisynth Divider = 86
    //  R Divider = 1
    //  PLL source = PLLA
    //  Initial phase offset (ns) = 0,000
    //  Powered down = No
    //  Inverted = No
    //  Drive Strength = b11
    //  Disable State = Low
    //  Clock Source = b11

    // writeRegister(fd, SI5351_REGISTER_26_PLL_A_REG0, 0xe8);
    // writeRegister(fd, SI5351_REGISTER_27_PLL_A_REG1, 0x48);
    // writeRegister(fd, SI5351_REGISTER_28_PLL_A_REG2, 0x00);
    // writeRegister(fd, SI5351_REGISTER_29_PLL_A_REG3, 0x0c);
    // writeRegister(fd, SI5351_REGISTER_30_PLL_A_REG4, 0x55);
    // writeRegister(fd, SI5351_REGISTER_31_PLL_A_REG5, 0x10);
    // writeRegister(fd, SI5351_REGISTER_32_PLL_A_REG6, 0x9b);
    // writeRegister(fd, SI5351_REGISTER_33_PLL_A_REG7, 0x98);

    // writeRegister(fd, SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1, 0x00);
    // writeRegister(fd, SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2, 0x01);
    // writeRegister(fd, SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3, 0x00);
    // writeRegister(fd, SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4, 0x29);
    // writeRegister(fd, SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5, 0x00);
    // writeRegister(fd, SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6, 0x00);
    // writeRegister(fd, SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7, 0x00);
    // writeRegister(fd, SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8, 0x00);


    // Apply PLLA and PLLB soft reset
    writeRegister(fd, SI5351_REGISTER_177_PLL_RESET, 0xAC);

    // Enable desired outputs
    writeRegister(fd, SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL, 0);

    std::cout << "The setup for CLK0 has been completed." << std::endl;

    return 0;
}
