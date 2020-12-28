# Startup

This summorizes the boot operation for the Cortex-A5 microprocessor. 

### Standard boot strategy

When the prosessor comes out of reset, it will jump to adsress 0x00000000 and execute the reset vector. This is provided by the ROM code. The ROM code will scan conected extarnal NWM memmorices and try to find a booteble image in one of them. If a valid image is found, it will be copied into internal SRAM. The ROM code will then perform the remap; mapping the internal SRAM to address 0x00000000. Finaly the ROM code will release its resources and jump to address 0x00000000.

### Loading binary
There are to main ways of loading the binary into the MPU; using the ROM code or using an external debugger. The ROM code will attemt loading a file called boot.bin from the root directory a FAT formatet SD card. This is the preferred way. During development an external debugger will be used. The image will be directly written to memory, through the debugger. The registeres and the program counter will be manually modefied. 