# SI5351 Configurations for Selected Frequencies Found in Atari 8-bit Computers

## Overview
This project demonstrates how to configure the SI5351 clock generator for selected frequencies commonly found in Atari 8-bit computers. The program is written in C++ and utilizes the linux/i2c-dev.h library for I2C communication. It is designed to run on a Raspberry Pi with an attached extension module containing the SI5351 clock generator.

## Features
The program configures the SI5351 to generate the following frequencies:

- **1.773447 MHz** (Atari XL/XE PAL)
- **1.7897725 MHz** (Atari XL/XE NTSC)
- **1.7897875 MHz** (Atari 400/800 NTSC)
- **3.546894 MHz** (Atari XL/XE PAL)
- **3.579545 MHz** (Atari XL/XE NTSC)
- **3.579575 MHz** (Atari 400/800 NTSC)
- **4.433618 MHz** (Atari PAL Crystal)
- **8.3333 MHz** (Atari XF551 Crystal)
- **14.187576 MHz** (Atari XL/XE PAL)
- **14.31818 MHz** (Atari XL/XE NTSC)

## Requirements
- Raspberry Pi Zero/Zero 2/3/4/5
- C++17 compatible compiler
- SI5351 clock generator
- I2C interface

## Setup Instructions
1. Connect the SI5351 clock generator to your system via I2C.
2. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/peterkaczorowski/Si5351ForAtari8bit
   ```
3. Navigate to the project directory:
   ```bash
   cd Si5351ForAtari8bit
   ```
4. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
5. Run the program:
   ```bash
   ./Si5351ForAtari8bit
   ```

## How It Works
The program communicates with the SI5351 chip over I2C to configure its PLL and MultiSynth dividers, enabling it to generate precise clock frequencies. Specific settings for each Atari frequency are programmed using `wiringPiI2CWriteReg8`.

## Code Structure
- **`Si5351ForAtari8bit.cc`**: The main program file containing configuration logic.
- **`CMakeLists.txt`**: The build configuration file for CMake.

## Notes
- The I2C address for the SI5351 is set to `0x60`. Modify it in the code if your configuration differs.
- For additional frequency configurations, refer to the SI5351 datasheet.

## Author
Piotr D. Kaczorowski
December 28th, 2024
Version 1.1

## Contact
- Facebook: Atari Poland by The Atarian - https://www.facebook.com/groups/ataripoland
- Discord: Atari 8-bit Programming - https://discord.gg/GTapZjCsgp

## License
This project is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License. See the LICENSE file for more details.


